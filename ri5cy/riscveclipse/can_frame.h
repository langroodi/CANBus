#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <stdint.h>

/// @brief CAN frame size in bytes
#define CAN_FRAME_SIZE (13)
/// @brief CAN frame data field maximum size in bytes
#define MAX_DATA_SIZE (8)
/// @brief Trim the data length code to avoid multi-chunk transmission
#define TRIM_DLC(x) (x < MAX_DATA_SIZE ? x : MAX_DATA_SIZE)

/// @brief Standard ID low-byte size
#define SID_LOW_BYTE_SIZE (3)
/// @brief Standard ID low-byte mask
#define SID_LOW_BYTE_MASK (0x07U)
/// @brief Standard ID low-byte offset
#define SID_LOW_BYTE_OFFSET (5)
/// @brief Extended ID size
#define EID_SIZE (18U)
/// @brief Extended ID high-byte size
#define EID_HIGH_BYTE_SIZE (2)
/// @brief Extended ID high-byte mask
#define EID_HIGH_BYTE_MASK (0x03U)
/// @brief Extended ID middle-byte mask
#define EID_MIDDLE_BYTE_MASK (0xffU)
/// @brief Extended ID low-byte mask
#define EID_LOW_BYTE_MASK (EID_MIDDLE_BYTE_MASK)
/// @brief Extended ID middle-byte offset
#define EID_MIDDLE_BYTE_OFFSET (8)
/// @brief Data length code mask
#define DLC_MASK (0x0fU)

/// @brief Standard ID high-byte array index
#define SID_HIGH_BYTE_INDEX (0)
/// @brief Standard ID low-byte array index
#define SID_LOW_BYTE_INDEX (SID_HIGH_BYTE_INDEX + 1)
/// @brief Extended ID flag array index
#define IDE_FLAG_INDEX (SID_LOW_BYTE_INDEX)
/// @brief Extended ID high-byte array index
#define EID_HIGH_BYTE_INDEX (SID_LOW_BYTE_INDEX)
/// @brief Extended ID middle-byte array index
#define EID_MIDDLE_BYTE_INDEX (EID_HIGH_BYTE_INDEX + 1)
/// @brief Extended ID low-byte array index
#define EID_LOW_BYTE_INDEX (EID_MIDDLE_BYTE_INDEX + 1)
/// @brief Remote transmission request flag array index
#define RTR_FLAG_INDEX (EID_LOW_BYTE_INDEX + 1)
/// @brief Data length code array index
#define DLC_INDEX (RTR_FLAG_INDEX)
/// @brief Frame data array index
#define DATA_INDEX (DLC_INDEX + 1)

/// @brief CAN frame ID type
typedef enum can_id
{
	kStandardFrame_ID = 0x00U,
	kExtendedFrame_ID = 0x08U
} can_id_t;

/// @brief CAN frame type
typedef enum frame_type
{
	kDataFrame = 0x00U,
	kRemoteTransmitRequest = 0x10U
} frame_type_t;

/// @brief CAN frame
typedef struct can_frame
{
	uint32_t id;
	can_id_t idType;
	frame_type_t frameType;
	uint8_t dataLength;
	uint8_t* data;
} can_frame_t;

/// @brief Serialize a CAN frame into byte array
/// @param[in] frame CAN frame to be serialized
/// @param[out] payload Serialized CAN frame payload
/// @remarks No validation will be performed on the given frame.
void Serialize_CAN_Frame(const can_frame_t* frame, uint8_t* payload);

/// @brief Deserialize a serialized CAN frame payload
/// @param[in] payload Payload to be deserialized
/// @param[out] frame Deserialized CAN frame from the given payload
/// @returns '0' if the deserialization was successful; otherwise '-1'.
/// @note The frame 'data' member should be initialized before passing.
/// @remarks The 'frame' will be untouched in case of returning '-1'.
int Deserialize_CAN_Payload(const uint8_t* payload, can_frame_t* frame);

#endif
