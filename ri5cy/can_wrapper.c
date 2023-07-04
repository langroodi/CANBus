//! \include CAN Wrapper
#include "can_wrapper.h"

//!============================================================================

bool Initialize(void) {
	InitializeCan();
	bool _succeed = Reset();

	return _succeed;
}

//!============================================================================

//! \fn Copy Safely Data Field to a Serialized Frame
static bool copyDataField(
		const ByteArray source /*!< [in] Data Source */,
		ByteArray destination /*!< [out] Data Destination */,
		size_t count /*!< [in] Data Length Copy Count */) {
	//! \remark Prevention of stack overflow on unsafe memory move
	if ((source == nullptr) ||
		(destination == nullptr) ||
		(count > MAX_DATA_LENGTH)) {
		return false;
	}
	else {
		void* _to = memmove(destination, source, count);

		if (_to == nullptr) {
			return false;
		}
		else {
			return true;
		}
	}
}

//!----------------------------------------------------------------------------

//! \fn Serialize Standard Frame
static bool serializeStandardFrame(
		const StandardFrame_t* frame /*!< [in] Standard Frame Structure */,
		ByteArray serializedFrame /*!< [out] Serialized Frame Byte Array */) {
	serializedFrame[SERIALIZED_SIDH_OFFSET] = (uint8_t)(frame->ID >> 3);
	serializedFrame[SERIALIZED_SIDL_OFFSET] = (uint8_t)(frame->ID << 5);

	if (frame->RTR) {
		if (frame->DataSize > 0) {
			// Data field is not allowed while requesting remote transfer
			return false;
		}
		else {
			serializedFrame[SERIALIZED_DLC_OFFSET] = RTR_REGISTER_BYTE;

			return true;
		}
	}
	else
	{
		serializedFrame[SERIALIZED_DLC_OFFSET] = frame->DataSize;

		bool _succeed =
				copyDataField(
						frame->DataField,
						serializedFrame + SERIALIZED_DLC_OFFSET,
						frame->DataSize);

		return _succeed;
	}
}

//! \fn Serialize Extended Frame
static bool serializeExtendedFrame(
		const ExtendedFrame_t* frame /*!< [in] Extended Frame Structure */,
		ByteArray serializedFrame /*!< [out] Serialized Frame Byte Array */)
{
	// SID
	uint8_t _sIdMsb = (uint8_t)(frame->ID >> 21);
	uint8_t _sIdLsb = (uint8_t)(frame->ID >> 18);
	serializedFrame[SERIALIZED_SIDH_OFFSET] = _sIdMsb;
	serializedFrame[SERIALIZED_SIDL_OFFSET] = _sIdLsb << 5;

	// EID
	uint8_t _eIdMsb = (uint8_t)(frame->ID >> 16);
	_eIdMsb &= EID_REGISTER_BYTE;
	// Enable IDE bit
	_eIdMsb |= IDE_REGISTER_BYTE;
	serializedFrame[SERIALIZED_SIDL_OFFSET] |= _eIdMsb;
	serializedFrame[SERIALIZED_EIDH_OFFSET] = (uint8_t)(frame->ID >> 8);
	serializedFrame[SERIALIZED_EIDL_OFFSET] = (uint8_t)(frame->ID);

	if (frame->RTR) {
		if (frame->DataSize > 0) {
			// Data field is not allowed while requesting remote transfer
			return false;
		}
		else {
			serializedFrame[SERIALIZED_DLC_OFFSET] = RTR_REGISTER_BYTE;

			return true;
		}
	}
	else
	{
		serializedFrame[SERIALIZED_DLC_OFFSET] = frame->DataSize;

		bool _succeed =
				copyDataField(
						frame->DataField,
						serializedFrame + SERIALIZED_DLC_OFFSET,
						frame->DataSize);

		return _succeed;
	}
}

//!----------------------------------------------------------------------------

//! \fn Send Request to Send
static bool sendRts(TXB_t txb /*!< [in] TX Buffer */) {
	switch (txb) {
		case TXB0:
			return RequestToSend(RtsTx0);
		case TXB1:
			return RequestToSend(RtsTx1);
		case TXB2:
			return RequestToSend(RtsTx2);
		default:
			return false;
	}
}

//! \fn Is Transmission Successful
static bool isTransmissionSuccessful(TXB_t txb /*!< [in] TX Buffer */) {
	Register_Address _txbCtrl = txb - 1;
	uint8_t _rxArray[READ_SIZE];

	bool _succeed = ReadRegisters(_txbCtrl, _rxArray, READ_SIZE);

	if (_succeed) {
		_succeed = ((_rxArray[DATA_RW_OFFSET] & MLOA_TXERR_BYTE) == 0);
	}

	return _succeed;
}

//!============================================================================

bool SendStandardFrame(const StandardFrame_t* frame, TXB_t txb) {
	int _serializedFrameSize = SERIALIZED_HEADER_LENGTH + frame->DataSize;
	uint8_t _serializedFrame[_serializedFrameSize];

	bool _succeed = serializeStandardFrame(frame, _serializedFrame);

	if (_succeed)
	{
		_succeed = WriteRegisters(txb, _serializedFrame, _serializedFrameSize);

		if (_succeed) {
			_succeed = sendRts(txb);

			if (_succeed) {
				_succeed = isTransmissionSuccessful(txb);
			}
		}
	}

	return _succeed;
}

bool SendExtendedFrame(const ExtendedFrame_t* frame, TXB_t txb) {
	int _serializedFrameSize = SERIALIZED_HEADER_LENGTH + frame->DataSize;
	uint8_t _serializedFrame[_serializedFrameSize];

	bool _succeed = serializeExtendedFrame(frame, _serializedFrame);

	if (_succeed)
	{
		_succeed = WriteRegisters(txb, _serializedFrame, _serializedFrameSize);

		if (_succeed) {
			_succeed = sendRts(txb);

			if (_succeed) {
				_succeed = isTransmissionSuccessful(txb);
			}
		}
	}

	return _succeed;
}

//!============================================================================

//! \fn Deserialize Standard Frame
static bool deserializeStandardFrame(
		const ByteArray serializedFrame /*!< [in] Serialized Frame */,
		StandardFrame_t* frame /*!< [out] Standard Frame */,
		ByteArray dataField /*!< [out] Data Field */) {
	//! \remark Check whether the received frame is standard or not
	uint8_t _ide = (serializedFrame[SERIALIZED_SIDL_OFFSET] & IDE_REGISTER_BYTE);
	if (_ide == IDE_REGISTER_BYTE)
	{
		return false;
	}

	// ID
	StandardId _id = (StandardId)(serializedFrame[SERIALIZED_SIDH_OFFSET] << 3);
	_id |= (StandardId)(serializedFrame[SERIALIZED_SIDL_OFFSET] >> 5);
	frame->ID = _id;

	// RTR
	bool _rtr =
			((serializedFrame[SERIALIZED_DLC_OFFSET] & RTR_REGISTER_BYTE)
					== RTR_REGISTER_BYTE);
	frame->RTR = _rtr;

	// DLC
	size_t _dlc = (serializedFrame[SERIALIZED_DLC_OFFSET] & DLC_REGISTER_BYTE);
	frame->DataSize = _dlc;

	if((_rtr) && (_dlc > 0))
	{
		// Data field is not allowed while requesting remote transfer
		return false;
	}

	// Data
	frame->DataField = dataField;
	bool _succeed =
			copyDataField(
					serializedFrame + SERIALIZED_DLC_OFFSET,
					frame->DataField,
					frame->DataSize);

	return _succeed;
}

//! \fn Deserialize Extended Frame
static bool deserializeExtendedFrame(
		const ByteArray serializedFrame /*!< [in] Serialized Frame */,
		ExtendedFrame_t* frame /*!< [out] Extended Frame */,
		ByteArray dataField /*!< [out] Data Field */) {
	//! \remark Check whether the received frame is extended or not
	uint8_t _ide =
			(serializedFrame[SERIALIZED_SIDL_OFFSET] & IDE_REGISTER_BYTE);
	if (_ide != IDE_REGISTER_BYTE)
	{
		return false;
	}

	// ID
	ExtendedId _id = (ExtendedId)(serializedFrame[SERIALIZED_SIDH_OFFSET] << 21);
	uint8_t _sIdLsb = (uint8_t)(serializedFrame[SERIALIZED_SIDL_OFFSET] >> 5);
	_id |= (ExtendedId)(_sIdLsb << 18);
	uint8_t _eIdMsb = (uint8_t)(serializedFrame[SERIALIZED_SIDH_OFFSET] & EID_REGISTER_BYTE);
	_id |= (ExtendedId)(_eIdMsb << 16);
	_id |= (ExtendedId)(serializedFrame[SERIALIZED_EIDH_OFFSET] << 8);
	_id |= (ExtendedId)(serializedFrame[SERIALIZED_EIDL_OFFSET]);
	frame->ID = _id;

	// RTR
	bool _rtr =
			((serializedFrame[SERIALIZED_DLC_OFFSET] & RTR_REGISTER_BYTE)
					== RTR_REGISTER_BYTE);
	frame->RTR = _rtr;

	// DLC
	size_t _dlc = (serializedFrame[SERIALIZED_DLC_OFFSET] & DLC_REGISTER_BYTE);
	frame->DataSize = _dlc;

	if((_rtr) && (_dlc > 0))
	{
		// Data field is not allowed while requesting remote transfer
		return false;
	}

	// Data
	frame->DataField = dataField;
	bool _succeed =
			copyDataField(serializedFrame + SERIALIZED_DLC_OFFSET,
					frame->DataField,
					frame->DataSize);

	return _succeed;
}

//!----------------------------------------------------------------------------

//! \fn Reset Receiving Interrupt Flag
static void resetRxFlag(RXB_t rxb /*!< [in] RX Buffer */) {
	switch (rxb) {
		case RXB0:
			ResetInterrupt(InterruptRx0);
			break;
		case RXB1:
			ResetInterrupt(InterruptRx1);
			break;
	}
}

//!============================================================================

bool ReceiveStandardFrame(StandardFrame_t* frame, ByteArray data, RXB_t rxb) {
	int _serializedFrameSize = SERIALIZED_HEADER_LENGTH + MAX_DATA_LENGTH;
	uint8_t _serializedFrame[_serializedFrameSize];

	bool _succeed = ReadRegisters(rxb, _serializedFrame, _serializedFrameSize);
	if (_succeed) {
		_succeed = deserializeStandardFrame(_serializedFrame, frame, data);

		if (_succeed) {
			resetRxFlag(rxb);
		}
	}

	return _succeed;
}

bool ReceiveExtendedFrame(ExtendedFrame_t* frame, ByteArray data, RXB_t rxb ) {
	int _serializedFrameSize = SERIALIZED_HEADER_LENGTH + MAX_DATA_LENGTH;
	uint8_t _serializedFrame[_serializedFrameSize];

	int _succeed = ReadRegisters(rxb, _serializedFrame, _serializedFrameSize);
	if (_succeed == 0) {
		_succeed = deserializeExtendedFrame(_serializedFrame, frame, data);

		if (_succeed == 0) {
			resetRxFlag(rxb);
		}
	}

	return _succeed;
}

//!============================================================================



//!============================================================================
