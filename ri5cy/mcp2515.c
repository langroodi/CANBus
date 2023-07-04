//! \include MCP2515
#include "mcp2515.h"

//!============================================================================

//! \fn Initialize Timer
static void initializeTimer(void) {
    tpm_config_t tpmInfo;
    CLOCK_SetIpSrc(kCLOCK_Tpm0, kCLOCK_IpSrcFircAsync);
    TPM_GetDefaultConfig(&tpmInfo);
#ifndef TPM_PRESCALER
#define TPM_PRESCALER kTPM_Prescale_Divide_4
#endif
    tpmInfo.prescale = TPM_PRESCALER;
    TPM_Init(BOARD_TPM, &tpmInfo);
    TPM_SetTimerPeriod(
    		BOARD_TPM, USEC_TO_COUNT(TPM_INTERVAL, TPM_SOURCE_CLOCK));
    TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);
    //! \remark Enable the interrupt NOT routed from Interrupt Multiplexer Driver (INTMUX)
    EnableIRQ(BOARD_TPM_IRQ_NUM);
}

void InitializeCan(void) {
	InitializeSpi();
	initializeTimer();
}

//!============================================================================

static uint8_t getRegister(
		Register_Address address /*!< [in] Register Address */) {
	uint8_t _packet[READ_SIZE];
	_packet[INSTRUCTION_OFFSET] = ReadInstruction;
	_packet[ADDRESS_OFFSET] = address;

	SpiReceive(_packet, READ_SIZE);
	return _packet[DATA_RW_OFFSET];
} //!< Get a register value

int ReadRegisters(
		Register_Address baseAddress,
		ByteArray values, size_t n) {
	uint8_t _txCommand[n];

	_txCommand[INSTRUCTION_OFFSET] = ReadInstruction;
	_txCommand[ADDRESS_OFFSET] = baseAddress;

	//! \todo Send and Receive commands should be combine to keep CS pin low
	int _result = spiSendCommand(_txCommand, n);
	SpiReceive(values, n);

	return _result;
}

//!----------------------------------------------------------------------------

static void setRegister(
		Register_Address address /*!< [in] Register Address */,
		uint8_t value /*!< [in] Register Value */) {
	uint8_t _packet[WRITE_SIZE];
	_packet[INSTRUCTION_OFFSET] = WriteInstruction;
	_packet[ADDRESS_OFFSET] = address;
	_packet[DATA_RW_OFFSET] = value;

	SpiSend(_packet, WRITE_SIZE);
} //!< Set a register value


int WriteRegisters(
		Register_Address baseAddress,
		ByteArray values,
		size_t n) {
	values[INSTRUCTION_OFFSET] = WriteInstruction;
	values[ADDRESS_OFFSET] = baseAddress;

	//! \todo The following SPI command should be replaced.
	int _result = spiSendCommand(values, n);

	return _result;
}

//!----------------------------------------------------------------------------

static void modifyRegister(
		Register_Address address,
		Register_Mask mask,
		uint8_t data) {
	// [Instruction][Address Byte][Mask Byte][Data Byte]
	uint8_t _values[BITMODIFY_SIZE];
	_values[INSTRUCTION_OFFSET] = BitModifyInstruction;
	_values[ADDRESS_OFFSET] = address;
	_values[MASK_OFFSET] = mask;
	_values[DATA_MOD_OFFSET] = data;

	SpiSend(_values, BITMODIFY_SIZE);
}

//!============================================================================

//! \fn Verify Operation Mode
static bool verifyMode(Operation_Mode mode /*!< [in] Operation Mode */) {
	uint8_t _canstat = getRegister(REGISTER_CANSTAT);
	_canstat &= OPMODE_MASK;

	if (_canstat == mode) {
		return true;
	} else {
		return false;
	}
}

//! \fn Set Operation Mode Immediately
static void setModeImmediate(Operation_Mode mode /*!< [in] Operation Mode */) {
	modifyRegister(REGISTER_CANCTRL, OPMODE_MASK, mode);
}

//! \fn Set Operation Mode
int SetMode(
		Operation_Mode mode /*!< [in] Operation Mode */,
		uint32_t timeout /*!< [in] Mode Verification Timeout */) {
	int _result = -1;
	setModeImmediate(mode);
    TPM_StartTimer(BOARD_TPM, kTPM_SystemClock);

    while (true)
    {
        if (tpmIsrFlag)
        {
            millisecondCounter++;
            tpmIsrFlag = false;

            _result = verifyMode(mode);
            if ((_result == 0) || (millisecondCounter > timeout))
            {
            	break;
            }
        }
        //! \remark Wait for interrupt
        __WFI();
    }

    TPM_StopTimer(BOARD_TPM);
    millisecondCounter = 0U;
    return _result;
}

//!============================================================================

static Id_Block GetIdBlock(Can_Id id) {
	uint8_t msb = (uint8_t) (id >> 3);
	uint8_t lsb = (uint8_t) ((id & 0x07) << 5);
	Id_Block result;
	result.Msb = msb;
	result.Lsb = lsb;

	return result;
}

static int spiSendCommand(const uint8_t *command, size_t size) {
	Message_Status _result = SpiSend(command, size);

	if (_result == Transfer_Succeed) {
		return 0;
	} else {
		return -1;
	}
}

int RequestToSend(RtsTxBuffer rtsTx) {
	uint8_t _command = RtsInstruction | rtsTx;
	int _result = spiSendCommand(&_command, 1);

	return _result;
}



static uint8_t* idToArray(uint16_t id) {
	//! \todo The return value must be static or it is needed to be passed through
	uint8_t _result[CANID_LENGTH];

	_result[0] = id >> 3;
	_result[1] = (id & SID_MASK) << 5;
	_result[2] = 0;
	_result[3] = 0;

	return _result;
}

int SetFilter(RXF rxf, Id_Filter filter) {
	uint8_t* _filterArray = idToArray(filter);
	int _result = WriteRegister(rxf, _filterArray, CANID_LENGTH);

	return _result;
}

int SetFilterMask(RXM rxm, Id_Mask mask) {
	uint8_t* _filterArray = idToArray(mask);
	int _result = WriteRegister(rxm, _filterArray, CANID_LENGTH);

	return _result;
}

RxOverflow GetOverflow(void) {
	uint8_t _register = getRegister(REGISTER_EFLG);

	if ((_register & RxOverflowAll) == RxOverflowAll) {
		return RxOverflowAll;
	} else if ((_register & RxOverflow0) == RxOverflow0) {
		return RxOverflow0;
	} else if ((_register & RxOverflow1) == RxOverflow1) {
		return RxOverflow1;
	} else {
		return RxOverflowNone;
	}
}

void ResetOverflow(void) {
	modifyRegister(REGISTER_EFLG, RxOverflowAll, RxOverflowNone);
}

ErrorMode GetErrorMode(void) {
	uint8_t _register = getRegister(REGISTER_EFLG);

	if ((_register & ErrorWarning) == ErrorWarning) {
		return ErrorWarning;
	} else if ((_register & ErrorPassive) == ErrorPassive) {
		return ErrorPassive;
	} else if ((_register & ErrorBusOff) == ErrorBusOff) {
		return ErrorBusOff;
	} else {
		return ErrorActive;
	}
}

int Reset(void) {
	uint8_t _command = ResetInstruction;
	int _result;
	spiSendCommand(&_command, 1);

	return _result;
}

int* Poll(const PollItem *pollItems, size_t n) {
	//! \warning The following line is not supported by C98-based compilers
	int* _results[n];

	uint8_t _packet[2];
	_packet[INSTRUCTION_OFFSET] = ReadStatusInstruction;
	_packet[1] = DUMMY_BYTE;

	SpiReceive(_packet, 2);

	uint8_t _status = _packet[1];

	for (int i = 0; i < n; i++) {

		if ((pollItems[i] & _status) == pollItems[i]) {
			_results[i] = 0;
		} else {
			_results[i] = -1;
		}
	}

	return _results;
}

RxStatus GetRxStatus(void)
{
	RxStatus _result;

	uint8_t _packet[2];
	_packet[INSTRUCTION_OFFSET] = RxStatusInstruction;
	_packet[1] = DUMMY_BYTE;

	SpiReceive(_packet, 2);

	uint8_t _status = _packet[1];

	_result.Buffer = (_status & BUFFERMATCH_MASK);
	_result.Type = (_status & TYPEMATCH_MASK);
	_result.Filter = (_status & FILTERMATCH_MASK);

	return _result;
}

void EnableInterrupt(Interrupt interrupt) {
	modifyRegister(REGISTER_CANINTE, interrupt, interrupt);
}

void DisableInterrupt(Interrupt interrupt) {
	modifyRegister(REGISTER_CANINTE, interrupt, DUMMY_BYTE);
}

void ResetInterrupt(Interrupt interrupt) {
	modifyRegister(REGISTER_CANINTF, interrupt, DUMMY_BYTE);
}

static void BOARD_TPM_HANDLER(void)
{
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
    tpmIsrFlag = true;
}

//!============================================================================

void DisposeCan(void)
{
	TPM_Deinit(BOARD_TPM);
	DisposeSpi();
}

//!============================================================================
