#include "spi_helper.h"

int Initialize(void)
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

    if (status == kStatus_Success)
    {
    	return 0;
    }
    else
    {
    	return -1;
    }
}

int Transfer(uint8_t* rx_buffer, uint8_t* tx_buffer, size_t buffer_size)
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

void Dispose(void)
{
	LPSPI_RTOS_Deinit(&master_rtos_handle);
}
