#ifndef CAN_WRAPPER_H
#define CAN_WRAPPER_H

//!============================================================================

//! \include MCP2515 Driver Header
#include "mcp2515.h"

//!============================================================================

//! \define Null Pointer
#define nullptr ((void*)0)

//!----------------------------------------------------------------------------

/*! \define Serialized Frame Header Size
 *! \note [1-Byte Instruction][1-Byte Address][4-Byte ID][1-Byte DLC] */
#define SERIALIZED_HEADER_LENGTH 7
//! \define Maximum CAN Data Size
#define MAX_DATA_LENGTH 8

//! \define Serialized Frame SIDH Offset
#define SERIALIZED_SIDH_OFFSET 2
//! \define Serialized Frame SIDL Offset
#define SERIALIZED_SIDL_OFFSET 3
//! \define Serialized Frame EIDH Offset
#define SERIALIZED_EIDH_OFFSET 4
//! \define Serialized Frame EIDL Offset
#define SERIALIZED_EIDL_OFFSET 5
//! \define Serialized Frame DLC Offset
#define SERIALIZED_DLC_OFFSET 6
//! \define Serialized Frame Data Offset
#define SERIALIZED_DATA_OFFSET 7

//!----------------------------------------------------------------------------

//! \define IDE Mask in RXnSIDL Register
#define IDE_REGISTER_BYTE 0x08 //0b00001000
//! \define EID MSB Mask in RXnSIDL Register
#define EID_REGISTER_BYTE 0x03 //0b00000011
//! \define DLC Mask in RXnDLC Register
#define DLC_REGISTER_BYTE 0x0F //0b00001111
//! \define RTR Mask in RXnDLC Register
#define RTR_REGISTER_BYTE 0x40 //0b01000000
//! \define Message Lost Arbitration and/or Transmission Error
#define MLOA_TXERR_BYTE 0x30 //0b00110000

//!============================================================================

//! \typedef Tx Buffer Enumeration
typedef enum TXB {
	TXB0 = 0x31U,
	TXB1 = 0x41U,
	TXB2 = 0x51U,
} TXB_t;

//! \typedef Rx Buffer Enumeration
typedef enum RXB {
	RXB0 = 0x61U,
	RXB1 = 0x71U,
} RXB_t;

//!----------------------------------------------------------------------------

//! \typedef Standard CAN Frame ID
typedef uint16_t StandardId;
//! \typedef Extended CAN Frame ID
typedef uint32_t ExtendedId;

//!----------------------------------------------------------------------------

//! \typedef Standard CAN Frame Structure
typedef struct StandardFrame {
	StandardId ID;
	bool RTR;
	ByteArray DataField;
	size_t DataSize;
} StandardFrame_t;

//! \typedef Extended CAN Frame Structure
typedef struct ExtendedFrame {
	ExtendedId ID;
	bool RTR;
	ByteArray DataField;
	size_t DataSize;
} ExtendedFrame_t;

//!============================================================================

//! \fn Initialize
bool Initialize(void);

//!----------------------------------------------------------------------------

//! \fn Send Standard Frame
bool SendStandardFrame(
		const StandardFrame_t* frame /*!< [in] Standard Frame to Send */,
		TXB_t txb /*!< [in] Tx Buffer used for Sending */);

//! \fn Send Extended Frame
bool SendExtendedFrame(
		const ExtendedFrame_t* frame /*!< [in] Extended Frame to Send */,
		TXB_t txb /*!< [in] Tx Buffer used for Sending */);

//!----------------------------------------------------------------------------

//! \fn Receive Standard Frame
bool ReceiveStandardFrame(
		StandardFrame_t* frame /*!< [out] Received Standard Frame */,
		ByteArray data /*!< [out] Frame Data Pointer */,
		RXB_t rxb /*!< [in] Rx Buffer used for Receiving */);

//! \fn Receive Extended Frame
bool ReceiveExtendedFrame(
		ExtendedFrame_t* frame /*!< [out] Received Extended Frame */,
		ByteArray data /*!< [out] Frame Data Pointer */,
		RXB_t rxb /*!< [in] Rx Buffer used for Receiving */);

//!----------------------------------------------------------------------------

//! \fn Dispose
void Dispose(void);

//!============================================================================

#endif
