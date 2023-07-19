/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "riscveclipse/mcp2515.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_freertos.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_intmux.h"
#include "clock_config.h"
/*******************************************************************************
* Definitions
******************************************************************************/
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs2)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs2)

#define TRANSFER_SIZE (4)
#define TRANSFER_BAUDRATE (50000U) /*! Transfer baudrate - 50k */

/// @note I don't what is wrong, but I cannot use the first bytes of any array
#define OFFSET_PTR(X) (X + 3)

/*******************************************************************************
* Variables
******************************************************************************/

lpspi_rtos_handle_t master_rtos_handle;
lpspi_master_config_t masterConfig;
uint32_t sourceClock;
status_t status;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void write_task(void *pvParameters);
static void read_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set clock source for LPSPI and get master clock source */
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);

    PRINTF("FreeRTOS LPSPI example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on a single board.\r\n");
    PRINTF("Master and slave are both use interrupt way.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is:\r\n");
    PRINTF("LPSPI_master -- LPSPI_slave\r\n");
    PRINTF("    CLK      --    CLK\r\n");
    PRINTF("    PCS      --    PCS\r\n");
    PRINTF("    SOUT     --    SIN\r\n");
    PRINTF("    SIN      --    SOUT\r\n");
    PRINTF("\r\n");

    LPSPI_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;
    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    sourceClock = EXAMPLE_LPSPI_MASTER_CLOCK_FREQ;

    status = LPSPI_RTOS_Init(&master_rtos_handle, EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, sourceClock);
    if (status != kStatus_Success)
    {
        PRINTF("LPSPI master: error during initialization. \r\n");
        vTaskSuspend(NULL);
    }

    uint8_t masterSendBuffer[TRANSFER_SIZE] = {0};

    size_t _tranferSize = Reset(masterSendBuffer);

    for (size_t i = _tranferSize; i < TRANSFER_SIZE; ++i)
    {
    	masterSendBuffer[i] = 0;
    }

    if (xTaskCreate(write_task, "Reset_task", configMINIMAL_STACK_SIZE + 64, masterSendBuffer, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    _tranferSize = GetMode(masterSendBuffer);

    for (size_t i = _tranferSize; i < TRANSFER_SIZE; ++i)
    {
    	masterSendBuffer[i] = 0;
    }

    if (xTaskCreate(read_task, "Read_mode_task", configMINIMAL_STACK_SIZE + 64, masterSendBuffer, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    for (;;)
        ;
}

/*!
 * @brief Task responsible for master SPI communication.
 */
static void write_task(void *pvParameters)
{
    lpspi_transfer_t masterXfer;

    /*Start master transfer*/
    uint8_t* _buffer = (uint8_t*)pvParameters;
    masterXfer.txData = _buffer;
    masterXfer.rxData = NULL;
    masterXfer.dataSize = TRANSFER_SIZE;
    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER;

    status = LPSPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);

    if (status == kStatus_Success)
    {
    	PRINTF("LPSPI master send completed successfully.\r\n");
    }
    else
    {
    	PRINTF("LPSPI master send completed with error.\r\n");
    }

    vTaskSuspend(NULL);
}

static void read_task(void *pvParameters)
{
	while (1)
	{
	    lpspi_transfer_t masterXfer;

	    /*Start master transfer*/
	    uint8_t* _sendBuffer = (uint8_t*)pvParameters;
	    uint8_t _receiveBuffer[] = {0,0,0,0,0,0};

	    masterXfer.txData = _sendBuffer;
	    masterXfer.rxData = OFFSET_PTR(_receiveBuffer);
	    masterXfer.dataSize = 3;
	    masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous;

	    status = LPSPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);

	    if (status == kStatus_Success)
	    {
	        PRINTF("LPSPI master receive completed successfully.\r\n");

	        uint8_t _canstat = _receiveBuffer[5];
	        _canstat &= OPMODE_MASK;
	        OperationMode _mode = (OperationMode)_canstat;

	        switch (_mode)
	        {
	        case NORMAL:
	        	PRINTF("I am normal!\r\n");
	        	break;
	        case LOOPBACK:
	        	PRINTF("I am looping back!\r\n");
	        	break;
	        case CONFIGURATION:
	        	PRINTF("I need to be configured\r\n");
	        	break;
	        }
	    }
	    else
	    {
	        PRINTF("LPSPI master receive completed with error.\r\n");
	    }

	    vTaskDelay(1000);
	}

    vTaskSuspend(NULL);
}
