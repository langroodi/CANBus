/*
 * mcp2515.h
 *
 *  Created on: Jul 15, 2023
 *      Author: armin
 */

#ifndef MCP2515_H_
#define MCP2515_H_

#include <stddef.h>
#include <stdint.h>
//!----------------------------------------------------------------------------

//! \define CAN Control Register
#define REGISTER_CANCTRL 0x0F
//! \define CAN Status Register
#define REGISTER_CANSTAT 0x0E
//! \define Operation Mode Register Mask (1110 0000)
#define OPMODE_MASK 0xE0

#define RESET_SIZE 1
//! \define Bit modification packet size
#define BITMODIFY_SIZE 4 // [Instruction][Address Byte][Mask Byte][Data Byte]
//! \define Read packet size for a single register
#define READ_SIZE 3

//! \define Instruction byte packet offset
#define INSTRUCTION_OFFSET 0
//! \define Register address byte packet offset
#define ADDRESS_OFFSET 1
//! \define Data byte(s) packet offset for a read/write instruction
#define DATA_RW_OFFSET 2
//! \define Mask offset for a modification instruction
#define MASK_OFFSET 2
//! \define Data byte(s) packet offset for a modification instruction
#define DATA_MOD_OFFSET 3

typedef uint8_t Register_Address;
typedef uint8_t Register_Mask;

typedef enum SpiInstructions {
	ResetInstruction = 0xC0U, 			// 1100 0000
	ReadInstruction = 0x03U, 			// 0000 0011
	ReadRxBufferInstruction = 0x90U,	// 1001 0nm0
	WriteInstruction = 0x02U,			// 0000 0010
	LoadTxBufferInstruction = 0x40U, 	// 0100 0abc
	RtsInstruction = 0x80U, 			// 1000 0nnn
	ReadStatusInstruction = 0xA0U, 		// 1010 0000
	RxStatusInstruction = 0xB0U, 		// 1011 0000
	BitModifyInstruction = 0x05U, 		// 0000 0101
} SpiInstruction;

typedef enum OperationModes {
	NORMAL = 0x00,
	LOOPBACK = 0x40,
	CONFIGURATION = 0x80
} OperationMode;

size_t Reset(uint8_t* payload);

size_t GetMode(uint8_t* payload);

size_t SetMode(OperationMode mode, uint8_t* payload);

#endif /* MCP2515_H_ */
