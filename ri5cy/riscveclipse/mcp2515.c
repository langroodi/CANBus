#include "mcp2515.h"
#include "task.h"

int Initialize_CAN(void)
{
	int result = Initialize_SPI();

	return result;
}

int Reset_CAN(void)
{
	uint8_t tx_buffer = kReset_Instruction;
	int result = Transfer_SPI(NULL, &tx_buffer, RESET_SIZE);

	return result;
}

int Set_BaudRate(void)
{
	const size_t BUFFER_SIZE = (size_t)(SET_BAUDRATE_SIZE + BUFFER_OFFSET);

	const size_t DATA_OFFSET_CNF3 =  (size_t)(DATA_RW_OFFSET + BUFFER_OFFSET);
	const size_t DATA_OFFSET_CNF2 =  (size_t)(DATA_OFFSET_CNF3 + 1);
	const size_t DATA_OFFSET_CNF1 =  (size_t)(DATA_OFFSET_CNF2 + 1);

	// 5 kbps CAN bus speed with 8MHx xtal
	const uint8_t DATA_CNF3 = 0x87U;
	const uint8_t DATA_CNF2 = 0xbfU;
	const uint8_t DATA_CNF1 = 0x1fU;

	uint8_t tx_buffer[BUFFER_SIZE];
	// [Instruction][Address Byte][CNF3][CNF2][CNF1]
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = (uint8_t)kWrite_Instruction;
	tx_buffer[ADDRESS_OFFSET + BUFFER_OFFSET] = REGISTER_CNF;
	tx_buffer[DATA_OFFSET_CNF3] = DATA_CNF3;
	tx_buffer[DATA_OFFSET_CNF2] = DATA_CNF2;
	tx_buffer[DATA_OFFSET_CNF1] = DATA_CNF1;

	int result = Transfer_SPI(NULL, OFFSET_PTR(tx_buffer), SET_BAUDRATE_SIZE);

	return result;
}

static int try_Get_Register(register_address_t address, uint8_t* value)
{
	const size_t BUFFER_SIZE = (size_t)(READ_SIZE + BUFFER_OFFSET);

	uint8_t rx_buffer[BUFFER_SIZE];

	uint8_t tx_buffer[BUFFER_SIZE];
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = (uint8_t)kRead_Instruction;
	tx_buffer[ADDRESS_OFFSET + BUFFER_OFFSET] = address;

	int result = Transfer_SPI(OFFSET_PTR(rx_buffer), OFFSET_PTR(tx_buffer), READ_SIZE);
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
	const size_t BUFFER_SIZE = (size_t)(BITMODIFY_SIZE + BUFFER_OFFSET);

	uint8_t tx_buffer[BUFFER_SIZE];
	// [Instruction][Address Byte][Mask Byte][Data Byte]
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = (uint8_t)kBitModify_Instruction;
	tx_buffer[ADDRESS_OFFSET + BUFFER_OFFSET] = address;
	tx_buffer[MASK_OFFSET + BUFFER_OFFSET] = mask;
	tx_buffer[DATA_MOD_OFFSET + BUFFER_OFFSET] = data;

	int result = Transfer_SPI(NULL, OFFSET_PTR(tx_buffer), BITMODIFY_SIZE);

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

int RequestToSend(const can_frame_t* frame)
{
	const int LOAD_TX_BUFFER_FAILED = -2;
	const size_t BUFFER_SIZE = (size_t)(LOAD_TX_BUFFER_SIZE + BUFFER_OFFSET);

	uint8_t tx_buffer[BUFFER_SIZE];
	// [Instruction][Data Byte]
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = (uint8_t)kLoadTxBuffer_Instruction;
	Serialize_CAN_Frame(frame, OFFSET_PTR(tx_buffer) + LOAD_DATA_OFFSET);

	int result = Transfer_SPI(NULL, OFFSET_PTR(tx_buffer), LOAD_TX_BUFFER_SIZE);

	if (result != 0)
	{
		return LOAD_TX_BUFFER_FAILED;
	}

	uint8_t rts_rxb0 = (uint8_t)kRts_Instruction;
	rts_rxb0 = (uint8_t)(rts_rxb0 | RTS_RXB_MASK);
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = rts_rxb0;

	result = Transfer_SPI(NULL, OFFSET_PTR(tx_buffer), RTS_SIZE);

	return result;
}

int TryToReceive(can_frame_t* frame)
{
	const int FETCH_RX_STATUS_FAILED = -4;
	const int EMPTY_RX_BUFFER = -3;
	const int READ_RX_BUFFER_FAILED = -2;

	uint8_t instruction = (uint8_t)kRxStatus_Instruction;
	uint8_t rx_status;

	int result = Transfer_SPI(&rx_status, &instruction, RX_STATUS_SIZE);

	if (result != 0)
	{
		return FETCH_RX_STATUS_FAILED;
	}

	const uint8_t MESSAGE_IN_RXB = (uint8_t)RXB_STATUS_MASK;

	rx_status = (uint8_t)(rx_status & MESSAGE_IN_RXB);

	if (rx_status != MESSAGE_IN_RXB)
	{
		return EMPTY_RX_BUFFER;
	}

	const size_t BUFFER_SIZE = (size_t)(READ_RX_BUFFER_SIZE + BUFFER_OFFSET);

	uint8_t rx_buffer[BUFFER_SIZE];

	uint8_t tx_buffer[BUFFER_SIZE];
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = (uint8_t)kReadRxBuffer_Instruction;

	result = Transfer_SPI(OFFSET_PTR(rx_buffer), OFFSET_PTR(tx_buffer), READ_RX_BUFFER_SIZE);

	if (result != 0)
	{
		return READ_RX_BUFFER_FAILED;
	}

	result = Deserialize_CAN_Payload(OFFSET_PTR(rx_buffer) + READ_DATA_OFFSET, frame);

	return result;
}

int BeginToReceive(can_frame_t* frame, uint32_t timeout)
{
	const size_t BUFFER_SIZE = (size_t)(READ_RX_BUFFER_SIZE + BUFFER_OFFSET);

	uint8_t rx_buffer[BUFFER_SIZE];

	uint8_t tx_buffer[BUFFER_SIZE];
	tx_buffer[INSTRUCTION_OFFSET + BUFFER_OFFSET] = (uint8_t)kReadRxBuffer_Instruction;

	int result = Poll_SPI(OFFSET_PTR(rx_buffer), OFFSET_PTR(tx_buffer), READ_RX_BUFFER_SIZE, timeout);

	if (result != 0)
	{
		return result;
	}

	result = Deserialize_CAN_Payload(OFFSET_PTR(rx_buffer) + READ_DATA_OFFSET, frame);

	return result;
}

void Dispose_CAN(void)
{
	Dispose_SPI();
}
