#include "spi_wrapper.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
    	isTransferSuccessful = true;
    }
    else
    {
    	isTransferSuccessful = false;
    }

    isTransferCompleted = true;
    // TODO This callback should differ for send and receive
}

/*!
 * @brief Initialize function
 */
void InitializeSpi(void)
{
    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE); // Assign clock type to the SPI CLK

    /*DMA Mux setting and EDMA init*/
    uint32_t masterRxChannel, masterTxChannel;
    edma_config_t userConfig;

    masterRxChannel = 0;
    masterTxChannel = 1;

    /* DMA MUX init*/
    DMAMUX_Init(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE); // Init the DMA Core 0

    /* Bind the Ch0 on Core0 to SPI0_RX and then enable the channel */
    DMAMUX_SetSource(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE, masterRxChannel, EXAMPLE_LPSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE, masterRxChannel);

    DMAMUX_SetSource(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE, masterTxChannel, EXAMPLE_LPSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPSPI_MASTER_DMA_MUX_BASE, masterTxChannel);

    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_LPSPI_MASTER_DMA_BASE, &userConfig); // Init the eDMA controller based on the default config

    uint32_t srcClock_Hz;
    lpspi_master_config_t masterConfig;

    /*Master config*/
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame = 8 * TRANSFER_SIZE;
    /* Determining SPI mode */
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst; // MSB vs. LSB

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    srcClock_Hz = LPSPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    /*Set up lpspi master*/
    memset(&(lpspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(lpspiEdmaMasterRxRegToRxDataHandle));
    memset(&(lpspiEdmaMasterTxDataToTxRegHandle), 0, sizeof(lpspiEdmaMasterTxDataToTxRegHandle));

    /* Bind handle to DMA Core+Ch */
    EDMA_CreateHandle(&(lpspiEdmaMasterRxRegToRxDataHandle), EXAMPLE_LPSPI_MASTER_DMA_BASE, masterRxChannel);
    EDMA_CreateHandle(&(lpspiEdmaMasterTxDataToTxRegHandle), EXAMPLE_LPSPI_MASTER_DMA_BASE, masterTxChannel);

    /* Wrap all the things together with the callback */
    /* The user data will be NULL */
    LPSPI_MasterTransferCreateHandleEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, LPSPI_MasterUserCallback,
                                         NULL, &lpspiEdmaMasterRxRegToRxDataHandle,
                                         &lpspiEdmaMasterTxDataToTxRegHandle);

    isInitialized = 1;
}

Message_Status SpiSend(const SpiBuffer data, size_t size)
{
    if ((isInitialized == 1) && (size < TRANSFER_SIZE))
    {
        lpspi_transfer_t masterXfer;

        /*Start master transfer*/
        masterXfer.txData = data;
        masterXfer.rxData = NULL;
        masterXfer.dataSize = size;
        /* The byte swap is essential for some value of frame per transfer */
        masterXfer.configFlags =
            EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap | kLPSPI_MasterPcsContinuous;

        isTransferCompleted = false;
        LPSPI_MasterTransferEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, &masterXfer);

        /* Wait until transfer completed */
        while (!isTransferCompleted)
        {
        	__NOP();
        }

        if (isTransferSuccessful)
        {
        	return Transfer_Succeed;
        }
        else
        {
        	return Transfer_Failed;
        }
    }
    else
    {
    	return Transfer_Failed;
    }
}

void SpiBeginSend(SpiBuffer data, size_t size, SpiSendCallback callback)
{

}

Message_Status SpiReceive(SpiBuffer buffer, size_t size)
{
	//! \todo The receive in MCP2515 cannot be implemented like this. The TX buffer is required.

    if ((isInitialized == 1) && (size < TRANSFER_SIZE))
    {
        lpspi_transfer_t masterXfer;

        /* Start master transfer, receive data from slave */
        isTransferCompleted = false;
        masterXfer.txData = NULL;
        masterXfer.rxData = buffer;
        masterXfer.dataSize = size;
        masterXfer.configFlags =
            EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterByteSwap | kLPSPI_MasterPcsContinuous;

        LPSPI_MasterTransferEDMA(EXAMPLE_LPSPI_MASTER_BASEADDR, &g_m_edma_handle, &masterXfer);

        /* Wait until transfer completed */
        while (!isTransferCompleted)
        {
        	__NOP();
        }

        if (isTransferSuccessful)
        {
        	return Transfer_Succeed;
        }
        else
        {
        	return Transfer_Failed;
        }
    }
    else
    {
    	return Transfer_Failed;
    }
}

void SpiBeginReceive(SpiBuffer data, size_t size, SpiReceiveCallback callback)
{

}
