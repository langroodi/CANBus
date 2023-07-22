#include "mcp2515.h"

int Initialize_CAN(void)
{
	int result = Initialize_SPI();

	return result;
}

int Reset_CAN()
{
	uint8_t tx_buffer = kReset_Instruction;
	int result = Transfer_SPI(NULL, &tx_buffer, RESET_SIZE);

	return result;
}

static int try_Get_Register(register_address_t address, uint8_t* value)
{
	const size_t BUFFER_SIZE = READ_SIZE + BUFFER_OFFSET;

	uint8_t rx_buffer[BUFFER_SIZE];

	uint8_t tx_buffer[BUFFER_SIZE];
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = kRead_Instruction;
	tx_buffer[ADDRESS_OFFSET + BUFFER_OFFSET] = address;

	int result = Transfer_SPI(OFFSET_PTR(rx_buffer), OFFSET_PTR(tx_buffer), BUFFER_SIZE);
	if (result == 0)
	{
		*value = rx_buffer[DATA_RW_OFFSET + BUFFER_OFFSET];
	}

	return result;
}

int TryGet_CANMode(operation_mode_t* mode)
{
	uint8_t canstat;
	int result = try_Get_Register(REGISTER_CANSTAT, &canstat);

	if (result == 0)
	{
		canstat &= OPMODE_MASK;
        *mode = (operation_mode_t)canstat;
	}

	return result;
}

static size_t set_Register(register_address_t address, register_mask_t mask, uint8_t data)
{
	const size_t BUFFER_SIZE = BITMODIFY_SIZE + BUFFER_OFFSET;

	uint8_t rx_buffer[BUFFER_SIZE];

	uint8_t tx_buffer[BUFFER_SIZE];
	// [Instruction][Address Byte][Mask Byte][Data Byte]
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = kBitModify_Instruction;
	tx_buffer[ADDRESS_OFFSET + BUFFER_OFFSET] = address;
	tx_buffer[MASK_OFFSET + BUFFER_OFFSET] = mask;
	tx_buffer[DATA_MOD_OFFSET + BUFFER_OFFSET] = data;

	int result = Transfer_SPI(OFFSET_PTR(rx_buffer), OFFSET_PTR(tx_buffer), BUFFER_SIZE);

	return result;
}

int Set_CANMode(operation_mode_t mode)
{
	return set_Register(REGISTER_CANCTRL, OPMODE_MASK, mode);
}

void Dispose_CAN(void)
{
	Dispose_SPI();
}
