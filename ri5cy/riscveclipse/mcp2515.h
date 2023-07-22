#ifndef MCP2515_H
#define MCP2515_H

#include "spi_helper.h"

/// @define CAN Control Register
#define REGISTER_CANCTRL 0x0F
/// @define CAN Status Register
#define REGISTER_CANSTAT 0x0E
/// @define Operation Mode Register Mask (1110 0000)
#define OPMODE_MASK 0xE0

#define RESET_SIZE 1
/// @define Bit modification packet size
#define BITMODIFY_SIZE 4 // [Instruction][Address Byte][Mask Byte][Data Byte]
/// @define Read packet size for a single register
#define READ_SIZE 3

/// @define Instruction byte packet offset
#define INSTRUCTION_OFFSET 0
/// @define Register address byte packet offset
#define ADDRESS_OFFSET 1
/// @define Data byte(s) packet offset for a read/write instruction
#define DATA_RW_OFFSET 2
/// @define Mask offset for a modification instruction
#define MASK_OFFSET 2
/// @define Data byte(s) packet offset for a modification instruction
#define DATA_MOD_OFFSET 3

/// @note I don't what is wrong, but I cannot use the first 3 bytes of any array
#define BUFFER_OFFSET (3)
#define OFFSET_PTR(X) (X + BUFFER_OFFSET)

typedef uint8_t register_address_t;
typedef uint8_t register_mask_t;

typedef enum spi_instruction {
	kReset_Instruction = 0xC0U, 		// 1100 0000
	kRead_Instruction = 0x03U, 			// 0000 0011
	kReadRxBuffer_Instruction = 0x90U,	// 1001 0nm0
	kWrite_Instruction = 0x02U,			// 0000 0010
	kLoadTxBuffer_Instruction = 0x40U, 	// 0100 0abc
	kRts_Instruction = 0x80U, 			// 1000 0nnn
	kReadStatus_Instruction = 0xA0U, 	// 1010 0000
	kRxStatus_Instruction = 0xB0U, 		// 1011 0000
	kBitModify_Instruction = 0x05U, 	// 0000 0101
} spi_instruction_t;

typedef enum operation_mode {
	kNormal = 0x00U,
	kLoopback = 0x40U,
	kConfiguration = 0x80U
} operation_mode_t;

/// @brief Initialize the CAN controller
/// @returns '0' if the initialization was successful; otherwise '-1'
int Initialize_CAN(void);

/// @brief Reset the CAN controller
/// @returns '0' if the reset was successful; otherwise '-1'
/// @note It is recommended by Microchip Technology to reset the controller at the startup.
int Reset_CAN();

/// @brief Try to the CAN controller operation mode
/// @param[out] mode Current CAN controller operation mode
/// @returns '0' if the mode is fetched successfully; otherwise '-1'
/// @note In case of returning '-1', the 'mode' argument will be untouched.
int TryGet_CANMode(operation_mode_t* mode);

/// @brief Set the CAN controller operation mode
/// @param mode Mode to be set to the controller
/// @param interval Mode verification interval
/// @param timeout Mode verification timeout
/// @returns '0' if the mode was set successfully; otherwise a negative number
int Set_CANMode(operation_mode_t mode, uint32_t interval, uint32_t timeout);

/// @brief Set the CAN controller operation mode with default interval and timeout values
/// @param mode Mode to be set to the controller
/// @returns '0' if the mode was set successfully; otherwise a negative number
/// @see Set_CANMode
int Set_CANMode_Defaults(operation_mode_t mode);

/// @brief Dispose the CAN controller
void Dispose_CAN(void);

#endif
