#include <string.h>
#include "can_frame.h"

void Serialize_CAN_Frame(const can_frame_t* frame, uint8_t* payload)
{
	if (frame->idType == kExtendedFrame_ID)
	{
		const uint8_t sid_high_byte = (uint8_t)(frame->id >> (SID_LOW_BYTE_SIZE + EID_SIZE));
		payload[SID_HIGH_BYTE_INDEX] = sid_high_byte;

		const uint8_t sid_low_byte = (uint8_t)((frame->id >> EID_SIZE) & SID_LOW_BYTE_MASK);
		payload[SID_LOW_BYTE_INDEX] = (uint8_t)(sid_low_byte << SID_LOW_BYTE_OFFSET);

		payload[IDE_FLAG_INDEX] |= (uint8_t)kExtendedFrame_ID;

		const uint8_t eid_high_byte =
				(uint8_t)((frame->id >> (EID_SIZE - EID_HIGH_BYTE_SIZE)) & EID_HIGH_BYTE_MASK);
		payload[EID_HIGH_BYTE_INDEX] |= eid_high_byte;

		const uint8_t eid_middle_byte =
				(uint8_t)((frame->id >> EID_MIDDLE_BYTE_OFFSET) & EID_MIDDLE_BYTE_MASK);
		payload[EID_MIDDLE_BYTE_INDEX] = eid_middle_byte;

		const uint8_t eid_low_byte = (uint8_t)((frame->id) & EID_LOW_BYTE_MASK);
		payload[EID_LOW_BYTE_INDEX] = eid_low_byte;
	}
	else
	{
		const uint8_t sid_high_byte = (uint8_t)(frame->id >> SID_LOW_BYTE_SIZE);
		payload[SID_HIGH_BYTE_INDEX] = sid_high_byte;

		const uint8_t sid_low_byte = (uint8_t)(frame->id & SID_LOW_BYTE_MASK);
		payload[SID_LOW_BYTE_INDEX] = (uint8_t)(sid_low_byte << SID_LOW_BYTE_OFFSET);

		payload[EID_MIDDLE_BYTE_INDEX] = 0;
		payload[EID_LOW_BYTE_INDEX] = 0;
	}

	if (frame->frameType == kDataFrame)
	{
		const uint8_t trimmed_dlc = (uint8_t)TRIM_DLC(frame->dataLength);
		payload[DLC_INDEX] = trimmed_dlc;

		memcpy(payload + DATA_INDEX, frame->data, trimmed_dlc);
	}
	else
	{
		payload[RTR_FLAG_INDEX] = (uint8_t)kRemoteTransmitRequest;
	}
}

int Deserialize_CAN_Payload(const uint8_t* payload, size_t length, can_frame_t* frame)
{
	const frame_type_t frame_type = (frame_type_t)(payload[RTR_FLAG_INDEX] & kRemoteTransmitRequest);
	const uint8_t dlc = (uint8_t)(payload[DLC_INDEX] & DLC_MASK);

	if (frame_type == kRemoteTransmitRequest && dlc > 0)
	{
		// Sending data is not allowed via a RTR frame.
		return -1;
	}

	frame->frameType = frame_type;

	const can_id_t id_type = (can_id_t)(payload[IDE_FLAG_INDEX] & kExtendedFrame_ID);
	frame->idType = id_type;

	if (id_type == kExtendedFrame_ID)
	{
		uint32_t id = payload[SID_HIGH_BYTE_INDEX];
		id <<= SID_LOW_BYTE_SIZE + EID_SIZE;

		uint32_t sid_low_byte = (uint32_t)(payload[SID_LOW_BYTE_INDEX] >> SID_LOW_BYTE_OFFSET);
		sid_low_byte <<= EID_SIZE;
		id |= sid_low_byte;

		uint32_t eid_high_byte = (uint32_t)(payload[EID_HIGH_BYTE_INDEX] && EID_HIGH_BYTE_MASK);
		eid_high_byte <<= EID_SIZE - EID_HIGH_BYTE_SIZE;
		id |= eid_high_byte;

		uint32_t eid_middle_byte = payload[EID_MIDDLE_BYTE_INDEX];
		eid_middle_byte <<= EID_MIDDLE_BYTE_OFFSET;
		id |= eid_middle_byte;

		const uint32_t eid_low_byte = payload[EID_LOW_BYTE_INDEX];
		id |= eid_low_byte;

		frame->id = id;
	}
	else
	{
		uint32_t id = payload[SID_HIGH_BYTE_INDEX];
		id <<= SID_LOW_BYTE_SIZE;

		const uint32_t sid_low_byte = (uint32_t)(payload[SID_LOW_BYTE_INDEX] >> SID_LOW_BYTE_OFFSET);
		id |= sid_low_byte;

		frame->id = id;
	}

	if (frame_type == kDataFrame)
	{
		const uint8_t trimmed_dlc = (uint8_t)(TRIM_DLC(dlc));
		frame->dataLength = trimmed_dlc;

		memcpy(frame->data, payload + DATA_INDEX, trimmed_dlc);
	}
	else
	{
		frame->dataLength = 0;
	}

	return 0;
}
