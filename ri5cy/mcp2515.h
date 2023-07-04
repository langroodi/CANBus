#ifndef MCP2515_H
#define MCP2515_H

//!============================================================================

//! \include SPI Wrapper
#include "spi_wrapper.h"
//! \include Timer PWM Module
#include "fsl_tpm.h"
//! \include Clock Config
#include "clock_config.h"
//! \include Standard Library
#include <stdlib.h>
//! \include String Header for Memory Move
#include <string.h>

//!============================================================================

//! \define First Config Register
#define REGISTER_CNF1 0x28
//! \define Second Config Register
#define REGISTER_CNF2 0x29
//! \define Third Config Register
#define REGISTER_CNF3 0x2A

//! \define Error Flag Register
#define REGISTER_EFLG 0x2D

//! \define CAN Control Register
#define REGISTER_CANCTRL 0x0F
//! \define CAN Status Register
#define REGISTER_CANSTAT 0x0E

//! \define CAN Interrupts Enable
#define REGISTER_CANINTE 0x2B
//! \define CAN Interrupts Flag
#define REGISTER_CANINTF 0x2C

//!----------------------------------------------------------------------------
//! \define Operation Mode Register Mask (1110 0000)
#define OPMODE_MASK 0xE0

// Config for 125 Mbps CAN Bus Baud Rate using 8 MHz Crystal
#define CONFIG_CNF1 0x01
#define CONFIG_CNF2 0xB1
#define CONFIG_CNF3 0x85

//!----------------------------------------------------------------------------

//! \define Bit modification packet size
#define BITMODIFY_SIZE 4 // [Instruction][Address Byte][Mask Byte][Data Byte]
//! \define Read packet size for a single register
#define READ_SIZE 3
//! \define Write packet size for a single register
#define WRITE_SIZE 3

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

//! \define The byte that the module does not care about
#define DUMMY_BYTE 0x00

//! \define CAN ID Length
#define CANID_LENGTH 4

//! \define Standard CAN ID Mask (0000 0000 0000 0111)
#define SID_MASK 0x0007U

//!============================================================================

typedef uint8_t *ByteArray;
typedef uint8_t Register_Address;
typedef uint8_t Register_Mask;
typedef uint16_t Can_Id;
typedef uint16_t Id_Mask;
typedef uint16_t Id_Filter;

typedef struct Id_Blocks {
	uint8_t Msb;
	uint8_t Lsb;
} Id_Block;

typedef enum Operation_Modes {
	NORMAL = 0x0,
	LOOPBACK = 0x2,
	CONFIGURATION = 0x4
} Operation_Mode;

typedef enum RXFs {
	RXF0 = 0x00,
	RXF1 = 0x04,
	RXF2 = 0x08,
	RXF3 = 0x10,
	RXF4 = 0x14,
	RXF5 = 0x18
} RXF;

typedef enum RXMs {
	RXM0 = 0x20,
	RXM1 = 0x24,
} RXM;

typedef enum TransmitPriorities {
	LowestPriority = 0x00U,
	LowPriority = 0x01U,
	HighPriority = 0x02U,
	HighestPriority = 0x003u,
} TransmitPriority;

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

typedef enum RtsTxBuffers {
	RtsTx0 = 0x01U,
	RtsTx1 = 0x02U,
	RtsTx2 = 0x04U,
} RtsTxBuffer;

//! \typedef Message Receive/Send Polling Status
typedef enum PollItems {
	RxBuffer0 = 0x01U,
	RxBuffer1 = 0x02U,
	TxBuffer0 = 0x04U,
	TxBuffer1 = 0x10U,
	TxBuffer2 = 0x40U,
} PollItem;

//! \typedef Rx Overflow States
typedef enum RxOverflows {
	RxOverflowNone = 0x00U,
	RxOverflow0 = 0x40U,
	RxOverflow1 = 0x80U,
	RxOverflowAll = 0xC0U,
} RxOverflow;

//! \typedef Node Error Mode
typedef enum ErrorModes {
	ErrorActive = 0x00U,
	ErrorWarning = 0x01U,
	ErrorPassive = 0x18U,
	ErrorBusOff = 0x20U,
} ErrorMode;

//! \typedef Interrupts
typedef enum Interrupts {
	InterruptRx0 = 0x01U,
	InterruptRx1 = 0x02U,
	InterruptTx0 = 0x04U,
	InterruptTx1 = 0x08U,
	InterruptTx2 = 0x01U,
	InterruptError = 0x20U,
	InterruptWakeUp = 0x40U,
	InterruptMessageError = 0x80U,
	InterruptAll = 0xFFU,
} Interrupt;

//!----------------------------------------------------------------------------

//! \define Buffer Match Mask Byte (1100 0000)
#define BUFFERMATCH_MASK 0xC0
//! \define Type Match Mask Byte (0001 1000)
#define TYPEMATCH_MASK 0x18
//! \define Buffer Match Mask Byte (0000 0111)
#define FILTERMATCH_MASK 0x07

//! \typedef Rx Buffer Match
typedef enum BufferMatches
{
	RxMatchNone = 0x00U,
	RxMatch0 = 0x40U,
	RxMatch1 = 0x80U,
	RxMatchAll = 0xC0U,
} BufferMatch;

//! \typedef Rx Message Type Match
typedef enum TypeMatches
{
	StandardDataMatch = 0x00U,
	StandardRemoteMatch = 0x08U,
	ExtendedDataMatch = 0x10U,
	ExtendedRemoteMatch = 0x18U,
} TypeMatch;

//! \typedef Rx Message Filter Match
typedef enum FilterMatches
{
	FilterMatch0 = 0x00U,
	FilterMatch1 = 0x01U,
	FilterMatch2 = 0x02U,
	FilterMatch3 = 0x03U,
	FilterMatch4 = 0x04U,
	FilterMatch5 = 0x05U,
	RolloverMatch0 = 0x06U,
	RolloverMatch1 = 0x07U,
} FilterMatch;

//! \struct Rx Status
typedef struct RxStatuses
{
	BufferMatch Buffer;
	TypeMatch Type;
	FilterMatch Filter;
} RxStatus;

//!----------------------------------------------------------------------------

//! \define TPM Instance
#define BOARD_TPM TPM0
//! \define TPM Interrupt Number
#define BOARD_TPM_IRQ_NUM TPM0_IRQn
//! \define TPM Interrupt Handler
#define BOARD_TPM_HANDLER TPM0_IRQHandler
//! \define TPM Clock Source
#define TPM_SOURCE_CLOCK (CLOCK_GetIpFreq(kCLOCK_Tpm0) / 4)
//! \define TPM Interval in [us]
#define TPM_INTERVAL 1000U

//! \var TPM Interrupt Handler (ISR) Flag
volatile bool tpmIsrFlag = false;
//! \var Millisecond Counter
volatile uint32_t millisecondCounter = 0U;

//!============================================================================

//! \fn Initialize CAN
void InitializeCan(void);

//! \fn Set Module Operation Mode
int SetMode(
		Operation_Mode mode /*!< [in] Operation Mode */,
		uint32_t timeout /*!< [in] Timeout in millisecond */);

int ReadRegisters(Register_Address baseAddress, ByteArray values, size_t n);
int WriteRegisters(Register_Address baseAddress, ByteArray values, size_t n);

int SetFilter(RXF rxf, Id_Filter filter);
int SetFilterMask(RXM rxm, Id_Mask mask);

RxOverflow GetOverflow(void);
void ResetOverflow(void);
ErrorMode GetErrorMode(void);

int RequestToSend(RtsTxBuffer rtsTx);
int Reset(void);

//! \fn Poll IO (RX/TT)
//! \return IO Ready Polling Result
int* Poll(const PollItem *pollItems /*! < [in] IO Poll Items */,
		size_t n /*!< [in] Number of Poll Items */);

//! \fn Get RX Status
RxStatus GetRxStatus(void);

void EnableInterrupt(Interrupt interrupt);
void DisableInterrupt(Interrupt interrupt);
void ResetInterrupt(Interrupt interrupt);

//! \fn Dispose CAN
void DisposeCan(void);

//!============================================================================
#endif
