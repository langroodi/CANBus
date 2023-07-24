#include <string.h>
#include "can_frame.h"

void Serialize_CAN_Frame(const can_frame_t* frame, uint8_t* payload)
{
	if (frame->idType == kExtendedFrame_ID)
	{
		uint8_t sid_high_byte = (uint8_t)(frame->id >> (SID_LOW_BYTE_SIZE + EID_SIZE));
		payload[SID_HIGH_BYTE_INDEX] = sid_high_byte;

		uint8_t sid_low_byte = (uint8_t)((frame->id >> EID_SIZE) & SID_LOW_BYTE_MASK);
		payload[SID_LOW_BYTE_INDEX] = (uint8_t)(sid_low_byte << SID_LOW_BYTE_OFFSET);

		payload[IDE_FLAG_INDEX] |= (uint8_t)kExtendedFrame_ID;

		uint8_t eid_high_byte = (uint8_t)((frame->id >> 16) & EID_HIGH_BYTE_MASK);
		payload[EID_HIGH_BYTE_INDEX] |= eid_high_byte;

		uint8_t eid_middle_byte = (uint8_t)((frame->id >> 8) & EID_MIDDLE_BYTE_MASK);
		payload[EID_MIDDLE_BYTE_INDEX] = eid_middle_byte;

		uint8_t eid_low_byte = (uint8_t)((frame->id) & EID_LOW_BYTE_MASK);
		payload[EID_LOW_BYTE_INDEX] = eid_low_byte;
	}
	else
	{
		uint8_t sid_high_byte = (uint8_t)(frame->id >> SID_LOW_BYTE_SIZE);
		payload[SID_HIGH_BYTE_INDEX] = sid_high_byte;

		uint8_t sid_low_byte = (uint8_t)(frame->id & SID_LOW_BYTE_MASK);
		payload[SID_LOW_BYTE_INDEX] = (uint8_t)(sid_low_byte << SID_LOW_BYTE_OFFSET);

		payload[EID_MIDDLE_BYTE_INDEX] = 0;
		payload[EID_LOW_BYTE_INDEX] = 0;
	}

	if (frame->frameType == kDataFrame)
	{
		uint8_t trimmed_dlc = (uint8_t)TRIM_DLC(frame->dataLength);
		payload[DLC_INDEX] = trimmed_dlc;

		memcpy(payload + DATA_INDEX, frame->data, trimmed_dlc);
	}
	else
	{
		payload[RTR_FLAG_INDEX] = (uint8_t)kRemoteTransmitRequest;
	}
}
