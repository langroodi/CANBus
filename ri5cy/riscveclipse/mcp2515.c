/*
 * mcp2515.c
 *
 *  Created on: Jul 15, 2023
 *      Author: armin
 */

#include "mcp2515.h"

static size_t getRegister(Register_Address address, uint8_t* payload)
{
	payload[INSTRUCTION_OFFSET] = ReadInstruction;
	payload[ADDRESS_OFFSET] = address;

	return READ_SIZE;
}

static size_t setRegister(
		Register_Address address,
		Register_Mask mask,
		uint8_t data,
		uint8_t* payload)
{
	// [Instruction][Address Byte][Mask Byte][Data Byte]
	payload[INSTRUCTION_OFFSET] = BitModifyInstruction;
	payload[ADDRESS_OFFSET] = address;
	payload[MASK_OFFSET] = mask;
	payload[DATA_MOD_OFFSET] = data;

	return BITMODIFY_SIZE;
}

size_t Reset(uint8_t* payload)
{
	payload[INSTRUCTION_OFFSET] = ResetInstruction;
	return RESET_SIZE;
}

size_t GetMode(uint8_t* payload)
{
	return getRegister(REGISTER_CANSTAT, payload);
}

size_t SetMode(OperationMode mode, uint8_t* payload)
{
	return setRegister(REGISTER_CANCTRL, OPMODE_MASK, mode, payload);
}
