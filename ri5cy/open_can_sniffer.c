#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "board.h"

#include "riscveclipse/mcp2515.h"
#include "riscveclipse/usb_packet.h"
#include "riscveclipse/usb_helper.h"

#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define master_task_PRIORITY (configMAX_PRIORITIES - 1)

/* Task sizes */
#define SMALL_STACK_SIZE (configMINIMAL_STACK_SIZE + 64)
#define MEDIUM_STACK_SIZE (configMINIMAL_STACK_SIZE + 128)
#define LARGE_STACK_SIZE (configMINIMAL_STACK_SIZE + 256)

#define FRAME_DATA_LENGTH (2)

/*******************************************************************************
* Variables
******************************************************************************/

static char const *s_appName = "app task";

/*******************************************************************************
 * Code
 ******************************************************************************/

void APPTask(void *handle)
{
    Initialize_USB();

	int successful = Reset_CAN();

	if (successful == 0)
	{
	    PRINTF("Reseting CAN completed successfully.\r\n");
    }
    else
    {
        PRINTF("Reseting CAN completed with error!\r\n");
    }

	vTaskDelay(5000);

	successful = Set_BaudRate();

	if (successful == 0)
	{
	    PRINTF("Setting CAN baud-rate completed successfully.\r\n");
    }
    else
    {
        PRINTF("Reseting CAN baud-rate completed with error!\r\n");
    }

	const operation_mode_t REQUESTED_MODE = kLoopback;
	successful = Set_CANMode(REQUESTED_MODE, 1000, 10000);

	if (successful == 0)
	{
	    PRINTF("Setting mode completed successfully.\r\n");
    }
    else
    {
        PRINTF("Setting mode completed with error!\r\n");
    }

	can_frame_t tx_frame;
	uint8_t tx_data[] = { 'H', 'i' };
	uint8_t usb_packet[USB_PACKET_SIZE + BUFFER_OFFSET];

	tx_frame.id = 256U;
	tx_frame.idType = kStandardFrame_ID;
	tx_frame.frameType = kDataFrame;
	tx_frame.dataLength = FRAME_DATA_LENGTH;
	tx_frame.data = tx_data;

	can_frame_t rx_frame;
	uint8_t rx_data[FRAME_DATA_LENGTH];

	rx_frame.data = rx_data;

    while (1)
    {
		successful = RequestToSend(&tx_frame);

	    if (successful == 0)
	    {
	        PRINTF("LPSPI master frame sent successfully.\r\n");
	    }
	    else
	    {
	        PRINTF("LPSPI master frame sent completed with error!\r\n");
	    }

		successful = BeginToReceive(&rx_frame, 5000);

	    if (successful == 0)
	    {
	        if (rx_frame.id == tx_frame.id &&
	        	rx_frame.idType == tx_frame.idType &&
				rx_frame.frameType == tx_frame.frameType &&
				rx_frame.dataLength == tx_frame.dataLength &&
				memcmp(rx_data, tx_data, FRAME_DATA_LENGTH) == 0)
	        {
	        	PRINTF("Data loopback is verified.\r\n");

	        	size_t size = Serialize_To_USB_Frame(&rx_frame, OFFSET_PTR(usb_packet));
	    		Send_USB(OFFSET_PTR(usb_packet), size);
	        }
	        else
	        {
	        	PRINTF("Data loopback verification failed!\r\n");
	        }
	    }
	    else
	    {
	        PRINTF("LPSPI master frame received completed with error!\r\n");
	    }

	    vTaskDelay(5000);
    }

    Dispose_USB();
}

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
        PRINTF("LPSPI master: error during initialization!\r\n");
        vTaskSuspend(NULL);
    }

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

    for (;;)
        ;
}
