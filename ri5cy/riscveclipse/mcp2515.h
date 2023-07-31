#ifndef MCP2515_H
#define MCP2515_H

#include "spi_helper.h"
#include "can_frame.h"

/// @brief CAN control register
#define REGISTER_CANCTRL 0x0FU
/// @brief CAN status register
#define REGISTER_CANSTAT 0x0EU
/// @brief First transmission buffer (TXB0) control register
#define REGISTER_TXBCTRL 0x30U
/// @brief Operation mode register mask (1110 0000)
#define OPMODE_MASK 0xE0U
/// @brief RXB0 Request-to-Send mask (0000 0001)
#define RTS_RXB_MASK 0x01U
/// @brief Message in RXB0 status mask (0100 0000)
#define RXB_STATUS_MASK 0x40U

#define RESET_SIZE 1
/// @brief Bit modification packet size
#define BITMODIFY_SIZE 4 // [Instruction][Address Byte][Mask Byte][Data Byte]
/// @brief Read packet size for a single register
#define READ_SIZE 3
/// @brief Load TXB0 buffer instruction size
#define LOAD_TX_BUFFER_SIZE (1 + CAN_FRAME_SIZE) // [Instruction][Serialize CAN Frame]
/// @brief Request-to-Send instruction size
#define RTS_SIZE 1
/// @brief RX status instruction size
#define RX_STATUS_SIZE 1
/// @brief Read RXB0 buffer instruction size
#define READ_RX_BUFFER_SIZE (1 + CAN_FRAME_SIZE) // [Instruction][Maximum CAN Frame]

/// @brief Instruction byte packet offset
#define INSTRUCTION_OFFSET 0
/// @brief Register address byte packet offset
#define ADDRESS_OFFSET 1
/// @brief Data byte(s) packet offset for a load TX buffer instruction
#define LOAD_DATA_OFFSET 1
/// @brief Data byte(s) packet offset for a read RX buffer instruction
#define READ_DATA_OFFSET 1
/// @brief Data byte(s) packet offset for a read/write instruction
#define DATA_RW_OFFSET 2
/// @brief Mask offset for a modification instruction
#define MASK_OFFSET 2
/// @brief Data byte(s) packet offset for a modification instruction
#define DATA_MOD_OFFSET 3

/// @note I don't what is wrong, but I cannot use the first 3 bytes of any array
#define BUFFER_OFFSET 3
#define OFFSET_PTR(x) (x + BUFFER_OFFSET)

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

/// @brief Transmission buffer state flag
typedef enum tx_buffer_state
{
	kNone = 0x00U,				///< None
	kRequestedToSend = 0x08U,	///< Transmission is requested
	kBusError = 0x10U,			///< Transmission failed due to bus error occurrence
	kLostArbitration = 0x20U,	///< Transmission failed due to arbitration lost
	kAborted = 0x40U			///< Transmission is aborted
} tx_buffer_state_t;

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

/// @brief Check whether the first transmission buffer (TXB0) has the given state or not
/// @param state State to be checked
/// @returns '1' if the buffer has the state, if not '0', and '-1' in case of error
int Has_TxBuffer_State(tx_buffer_state_t state);

/// @brief Request to send a CAN frame on TXB0
/// @param frame Frame to be sent
/// @returns '0' if the request was successful; otherwise a negative number
/// @remarks Returning '0' does not mean a successful transmission on the bus.
/// @see GetTxBufferState
int RequestToSend(const can_frame_t* frame);

/// @brief Try to receive a CAN frame by reading the first receive buffer (RXB0)
/// @param [out] frame Received frame
/// @returns '0' if the receive was successful; otherwise a negative number
/// @remarks Successful receive leads to clearing the RX0IF flag.
/// @note By returning a negative number, the 'frame' will be untouched.
int TryToReceive(can_frame_t* frame);

/// @brief Dispose the CAN controller
void Dispose_CAN(void);

#endif
