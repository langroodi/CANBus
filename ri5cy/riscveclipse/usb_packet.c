#include <string.h>
#include "usb_packet.h"

void Serialize_To_USB_Frame(const can_frame_t* frame, uint8_t* packet)
{
	packet[PACKET_HEADER_INDEX] = USB_PACKET_HEADER;
	packet[PACKET_TYPE_INDEX] = USB_PACKET_TYPE;

	size_t id_size;
	size_t offset = PACKET_TYPE_INDEX + 1;

	if (frame->idType == kExtendedFrame_ID)
	{
		id_size = EXTENDED_ID_SIZE;
		packet[PACKET_TYPE_INDEX] |= EXTENDED_FRAME_MASK;
	}
	else
	{
		id_size = STANDARD_ID_SIZE;
	}

	uint32_t id_mask = 0x000000ffU;
	size_t id_shift = 0;
	for (size_t i = 0; i < id_size; ++i)
	{
		const uint8_t id_byte = (uint8_t)((frame->id & id_mask) >> id_shift);
		packet[offset] = id_byte;

		id_shift += 8;
		id_mask <<= 8;
		++offset;
	}

	if (frame->frameType == kDataFrame)
	{
		const uint8_t trimmed_dlc = (uint8_t)TRIM_DLC(frame->dataLength);
		packet[PACKET_TYPE_INDEX] |= trimmed_dlc;

		memcpy(packet + offset, frame->data, trimmed_dlc);
		offset += trimmed_dlc;
	}
	else
	{
		packet[PACKET_TYPE_INDEX] |= REMOTE_FRAME_MASK;
	}

	packet[offset] = USB_PACKET_TRAILER;
}
