#ifndef SPI_WRAPPER_H
#define SPI_WRAPPER_H

/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_lpspi.h"
#include "board.h"
#include "fsl_lpspi_edma.h"
#include "fsl_dmamux.h"

#include <stddef.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LPSPI_MASTER_BASEADDR LPSPI0
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME kCLOCK_Lpspi0
#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT kLPSPI_Pcs2
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs2

#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE DMAMUX0
#define EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Rx
#define EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0LPSPI0Tx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE DMA0

#define TRANSFER_SIZE 64U         /* Transfer dataSize (#of bytes)*/
#define TRANSFER_BAUDRATE 125000U /* Transfer baudrate - 125k */

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
typedef uint8_t* SpiBuffer;

typedef enum Message_Statuses
{
	Transfer_Succeed,
	Transfer_Failed
} Message_Status;

typedef void (*SpiSendCallback)(Message_Status status);
typedef void (*SpiReceiveCallback)(Message_Status status, SpiBuffer data, size_t size);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION(lpspi_master_edma_handle_t g_m_edma_handle) = {0}; // The handle won't be cached in L1
edma_handle_t lpspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t lpspiEdmaMasterTxDataToTxRegHandle;

volatile bool isTransferSuccessful = false;
volatile bool isTransferCompleted = false;

static uint8_t isInitialized = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void InitializeSpi(void);

Message_Status SpiSend(const SpiBuffer data, size_t size);
void SpiBeginSend(SpiBuffer data, size_t size, SpiSendCallback callback);

Message_Status SpiReceive(SpiBuffer buffer, size_t size);
void SpiBeginReceive(SpiBuffer data, size_t size, SpiReceiveCallback callback);

/* LPSPI user callback */
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData);

void DisposeSpi(void);

#endif
