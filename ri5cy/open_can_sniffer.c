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

static char const *MAIN_TASK_NAME = "main task";
#define TASK_STACK_SIZE (5000L / sizeof(portSTACK_TYPE))
#define MAIN_TASK_PRIORITY (4)
#define MAIN_TASK_DELAY (500)

/*******************************************************************************
 * Code
 ******************************************************************************/

void main_Task(void *handle)
{
	const TickType_t xDelay = MAIN_TASK_DELAY / portTICK_PERIOD_MS;

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

	vTaskDelay(xDelay);

	successful = Set_BaudRate();

	if (successful == 0)
	{
	    PRINTF("Setting CAN baud-rate completed successfully.\r\n");
    }
    else
    {
        PRINTF("Reseting CAN baud-rate completed with error!\r\n");
    }

	const operation_mode_t REQUESTED_MODE = kNormal;
	successful = Set_CANMode(REQUESTED_MODE, 1000, 10000);

	if (successful == 0)
	{
	    PRINTF("Setting CAN mode completed successfully.\r\n");
    }
    else
    {
        PRINTF("Setting CAN mode completed with error!\r\n");
    }

	uint8_t usb_packet[USB_PACKET_SIZE + BUFFER_OFFSET];

	can_frame_t rx_frame;
	uint8_t rx_data[MAX_DATA_SIZE + BUFFER_OFFSET];
	rx_frame.data = OFFSET_PTR(rx_data);

    while (1)
    {
    	vTaskDelay(xDelay);
		successful = TryToReceive(&rx_frame);

	    if (successful == 0)
	    {
	    	PRINTF("LPSPI master frame received completed successfully!\r\n");

        	size_t size = Serialize_To_USB_Frame(&rx_frame, OFFSET_PTR(usb_packet));
    		Send_USB(OFFSET_PTR(usb_packet), size);
	    }
	    else
	    {
	        PRINTF("LPSPI master frame received completed with error!\r\n");
	    }
    }

    // In case of graceful dispose, uncomment the following lines
    /*
    Dispose_USB();
    Dispose_CAN();
    */
}

int main(void)
{
    // Initialize board hardware
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    int successful = Initialize_CAN();
    if (successful != 0)
    {
        PRINTF("LPSPI master: error during initialization!\r\n");
        vTaskSuspend(NULL);
    }

    if (xTaskCreate(main_Task,
    				MAIN_TASK_NAME,
                    TASK_STACK_SIZE,
                    NULL,
					MAIN_TASK_PRIORITY,
                    NULL
                    ) != pdPASS)
    {
        usb_echo("Main task creation failed!\r\n");

        return 1;
    }

    vTaskStartScheduler();

    for (;;)
        ;
}
