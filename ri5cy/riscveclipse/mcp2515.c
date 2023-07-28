#include "mcp2515.h"
#include "task.h"

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

int Set_CANMode(operation_mode_t mode, uint32_t interval, uint32_t timeout)
{
	const int ARGUMENT_ERROR_CODE = -2;
	const int VERIFICATION_ERROR_CODE = -3;

	if (timeout < interval)
	{
		return ARGUMENT_ERROR_CODE;
	}

	int result = set_Register(REGISTER_CANCTRL, OPMODE_MASK, mode);

	if (result != 0)
	{
		return result;
	}

	uint32_t elapsed_time = 0;

	while (elapsed_time < timeout)
	{
		vTaskDelay(interval);
		elapsed_time += interval;

		operation_mode_t current_mode;
		result = TryGet_CANMode(&current_mode);

		if (result == 0 && current_mode == mode)
		{
			return result;
		}
	}

	return VERIFICATION_ERROR_CODE;
}

int Set_CANMode_Defaults(operation_mode_t mode)
{
	const uint32_t DEFAULT_INTERVAL = 10;
	const uint32_t DEFAULT_TIMEOUT = 100;

	return Set_CANMode(mode, DEFAULT_INTERVAL, DEFAULT_TIMEOUT);
}

int Has_TxBuffer_State(tx_buffer_state_t state)
{
	uint8_t register_value;
	int result = try_Get_Register(REGISTER_TXBCTRL, &register_value);

	if (result == 0)
	{
		uint8_t state_byte = (uint8_t)state;
		uint8_t match = register_value & state_byte;
		result = match == 0U ? 0 : 1;
	}

	return result;
}

void Dispose_CAN(void)
{
	Dispose_SPI();
}
