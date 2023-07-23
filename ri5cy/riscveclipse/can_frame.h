#ifndef CAN_FRAME_H
#define CAN_FRAME_H

/// @brief CAN frame size in bytes
#define CAN_FRAME_SIZE (13)

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
void Serialize_CAN_Frame(const can_frame_t* frame, uint8_t* payload);

/// @brief Deserialize a serialized CAN frame payload
/// @param[in] payload Payload to be deserialized
/// @param[in] length Given payload length
/// @param[out] frame Deserialized CAN frame from the given payload
int Deserialize_CAN_Payload(const uint8_t* payload, size_t length, can_frame_t* frame);

#endif
