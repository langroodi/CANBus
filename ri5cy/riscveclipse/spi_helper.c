#include "spi_helper.h"

static lpspi_transfer_t pollerXfer;

int Initialize_SPI(void)
{
    // Set clock source for LPSPI and get master clock source
    CLOCK_SetIpSrc(LPSPI_MASTER_CLOCK_NAME, LPSPI_MASTER_CLOCK_SOURCE);

    LPSPI_MasterGetDefaultConfig(&master_config);

    master_config.baudRate = TRANSFER_BAUDRATE;
    master_config.bitsPerFrame = BITS_PER_FRAME;
    master_config.cpol = kLPSPI_ClockPolarityActiveHigh;
    master_config.cpha = kLPSPI_ClockPhaseFirstEdge;
    master_config.direction = kLPSPI_MsbFirst;
    master_config.pcsToSckDelayInNanoSec = NANOSECOND_DIVISOR / master_config.baudRate;
    master_config.lastSckToPcsDelayInNanoSec = NANOSECOND_DIVISOR / master_config.baudRate;
    master_config.betweenTransferDelayInNanoSec = NANOSECOND_DIVISOR / master_config.baudRate;
    master_config.whichPcs = LPSPI_MASTER_PCS_FOR_INIT;
    master_config.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
    master_config.pinCfg = kLPSPI_SdiInSdoOut;
    master_config.dataOutConfig = kLpspiDataOutRetained;

    uint32_t source_clock = LPSPI_MASTER_CLOCK_FREQ;
    status_t status =
    		LPSPI_RTOS_Init(
    				&master_rtos_handle,
					LPSPI_MASTER_BASEADDR,
					&master_config,
					source_clock);

    if (status != kStatus_Success)
    {
    	return -1;
    }

    return 0;
}

int Transfer_SPI(uint8_t* rx_buffer, uint8_t* tx_buffer, size_t buffer_size)
{
	lpspi_transfer_t masterXfer;

    masterXfer.rxData = rx_buffer;
    masterXfer.txData = tx_buffer;
    masterXfer.dataSize = buffer_size;
    masterXfer.configFlags = LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous;

    status_t status = LPSPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);

    if (status == kStatus_Success)
    {
    	return 0;
    }
    else
    {
    	return -1;
    }
}

int Poll_SPI(uint8_t* rx_buffer, uint8_t* tx_buffer, size_t buffer_size, uint32_t timeout)
{
	const int BUSY_SPI = -4;
	const int TRANSMISSION_FAILURE = -3;
	const int SEMAPHORE_TIMEOUT = -2;

	pollerXfer.rxData = rx_buffer;
	pollerXfer.txData = tx_buffer;
	pollerXfer.dataSize = buffer_size;
	pollerXfer.configFlags = LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous;

    // Lock the resource mutex
    if (xSemaphoreTake(master_rtos_handle.mutex, timeout) != pdTRUE)
    {
        return BUSY_SPI;
    }

    status_t status = LPSPI_MasterTransferNonBlocking(master_rtos_handle.base, &master_rtos_handle.drv_handle, &pollerXfer);
    if (status != kStatus_Success)
    {
        xSemaphoreGive(master_rtos_handle.mutex);
        return TRANSMISSION_FAILURE;
    }

    // Wait for transfer to finish or timeout
    if (xSemaphoreTake(master_rtos_handle.event, timeout) != pdTRUE)
    {
    	xSemaphoreGive(master_rtos_handle.mutex);
    	return SEMAPHORE_TIMEOUT;
    }

    // Unlock the resource mutex
    xSemaphoreGive(master_rtos_handle.mutex);

    return 0;
}

void Dispose_SPI(void)
{
	LPSPI_RTOS_Deinit(&master_rtos_handle);
}
