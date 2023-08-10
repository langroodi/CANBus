/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"

#include "riscveclipse/usb_helper.h"

#include "pin_mux.h"

/*******************************************************************************
* Variables
******************************************************************************/

static char const *s_appName = "app task";

USB_DATA_ALIGNMENT static uint8_t s_currSendBuf[DATA_BUFF_SIZE];
volatile static uint32_t s_sendSize = 0;

/*******************************************************************************
* Code
******************************************************************************/

void on_usb_received(const uint8_t* rx_buffer, size_t buffer_size)
{
	// Copy Buffer to Send Buff
	for (size_t i = 0; i < buffer_size; i++)
	{
		s_currSendBuf[s_sendSize++] = rx_buffer[i];
	}

	if (s_sendSize)
	{
		size_t size = (size_t)s_sendSize;
		s_sendSize = 0;

		Send_USB(s_currSendBuf, size);
	}
}

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
void APPTask(void *handle)
{
    Initialize_USB();
    Set_Receive_USB_Callback(&on_usb_received);

    while (1)
    {
    	__NOP();
    }

    Dispose_USB();
}

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    if (xTaskCreate(APPTask,                         /* pointer to the task                      */
                    s_appName,                       /* task name for kernel awareness debugging */
                    5000L / sizeof(portSTACK_TYPE),  /* task stack size                          */
                    NULL,                      /* optional task startup argument           */
                    4,                               /* initial priority                         */
                    NULL /* optional task handle to create           */
                    ) != pdPASS)
    {
        usb_echo("app task create failed!\r\n");

        return 1;
    }

    vTaskStartScheduler();

    return 1;
}
