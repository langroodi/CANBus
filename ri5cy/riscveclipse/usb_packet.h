#ifndef USB_PACKET_H
#define USB_PACKET_H

#include <string.h>
#include "can_frame.h"

/// @brief USB packet maximum size in bytes
#define USB_PACKET_SIZE (15)
/// @brief USB packet header index
#define PACKET_HEADER_INDEX (0)
/// @brief USB packet header byte
#define USB_PACKET_HEADER (0xaaU)
/// @brief USB packet type index
#define PACKET_TYPE_INDEX (1)
/// @brief USB packet CAN frame representation type
#define USB_PACKET_TYPE (0xc0U)
/// @brief Standard CAN frame ID size in bytes
#define STANDARD_ID_SIZE (2)
/// @brief Extended CAN frame ID size in bytes
#define EXTENDED_ID_SIZE (4)
/// @brief Extended CAN frame USB packet byte mask
#define EXTENDED_FRAME_MASK (0x20U)
/// @brief Remote CAN frame USB packet byte mask
#define REMOTE_FRAME_MASK (0x10U)
/// @brief USB packet trailer byte
#define USB_PACKET_TRAILER (0x55U)

/// @brief Serialize a CAN frame into a USB packet byte array
/// @param[in] frame CAN frame to be serialized
/// @param[out] packet Pre-allocated USB packet
/// @returns USB packet size
/// @remarks No validation will be performed on the given frame.
size_t Serialize_To_USB_Frame(const can_frame_t* frame, uint8_t* packet);

#endif
