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
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void reset_task(void *pvParameters);
static void set_mode_task(void *pvParameters);
static void get_mode_task(void *pvParameters);

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

    int successful = Initialize_CAN();
    if (successful != 0)
    {
        PRINTF("LPSPI master: error during initialization. \r\n");
        vTaskSuspend(NULL);
    }

    if (xTaskCreate(reset_task, "Reset task", configMINIMAL_STACK_SIZE + 64, NULL, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Reset task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(set_mode_task, "Set loopback mode task", configMINIMAL_STACK_SIZE + 128, NULL, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Set mode Task creation failed!.\r\n");
        while (1)
            ;
    }

    if (xTaskCreate(get_mode_task, "Get operation mode task", configMINIMAL_STACK_SIZE + 64, NULL, master_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Get operation mode task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();

    for (;;)
        ;
}

static void reset_task(void *pvParameters)
{
	int successful = Reset_CAN();

	if (successful == 0)
	{
	    PRINTF("Reseting LPSPI master completed successfully.\r\n");
    }
    else
    {
        PRINTF("Reseting LPSPI master completed with error.\r\n");
    }

    vTaskSuspend(NULL);
}

static void set_mode_task(void *pvParameters)
{
	const operation_mode_t REQUESTED_MODE = kLoopback;
	int successful = Set_CANMode_Defaults(REQUESTED_MODE);

	if (successful == 0)
	{
	    PRINTF("Setting mode completed successfully.\r\n");
    }
    else
    {
        PRINTF("Setting mode completed with error.\r\n");
    }

    vTaskSuspend(NULL);
}

static void get_mode_task(void *pvParameters)
{
	while (1)
	{
		operation_mode_t mode;
		int successful = TryGet_CANMode(&mode);

	    if (successful == 0)
	    {
	        PRINTF("LPSPI master receive completed successfully.\r\n");

	        switch (mode)
	        {
	        case kNormal:
	        	PRINTF("I am normal!\r\n");
	        	break;
	        case kLoopback:
	        	PRINTF("I am looping back!\r\n");
	        	break;
	        case kConfiguration:
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
