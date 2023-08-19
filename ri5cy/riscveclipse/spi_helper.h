#ifndef SPI_HELPER_H
#define SPI_HELPER_H

#include "fsl_lpspi.h"
#include "fsl_lpspi_freertos.h"

/// @define SPI instance
#define LPSPI_MASTER_BASEADDR (LPSPI0)
/// @define SPI clock name
#define LPSPI_MASTER_CLOCK_NAME (kCLOCK_Lpspi0)
/// @define SPI clock source
#define LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
/// @define SPI clock frequency
#define LPSPI_MASTER_CLOCK_FREQ (CLOCK_GetIpFreq(LPSPI_MASTER_CLOCK_NAME))
/// @define SPI chip select pin for initialization
#define LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs2)
/// @define SPI chip select for data transfer
#define LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs2)

/// @define SPI transfer baud rate (bps)
#define TRANSFER_BAUDRATE (50000U)
/// @define Number of bits per each SPI data frame
#define BITS_PER_FRAME (8);
/// @define Nanosecond divisor
#define NANOSECOND_DIVISOR (1000000000)

/// @brief Master SPI RTOS handler
lpspi_rtos_handle_t master_rtos_handle;
/// @brief Master SPI configuration
lpspi_master_config_t master_config;

/// @brief Initialize the SPI
/// @returns '0' if the initialization was successful; otherwise '-1'
int Initialize_SPI(void);

/// @brief Transfer data (read and write) via SPI
/// @param[out] rx_buffer Receive buffer
/// @param[in] tx_buffer Transmission buffer
/// @param buffer_size Minimum transmission buffer and receive buffer size
/// @returns '0' if the transfer was successful; otherwise '-1'
/// @note For read-only or write-only transmission, 'tx_buffer' or 'rx_buffer' can be 'NULL' respectively.
int Transfer_SPI(uint8_t* rx_buffer, uint8_t* tx_buffer, size_t buffer_size);

/// @brief Try to read data via SPI using a polling mechanism
/// @param[out] rx_buffer Receive buffer
/// @param buffer_size Receive buffer size
/// @param timeout Polling timeout in milliseconds
/// @returns '0' if the read was successful within the given timeout; otherwise a negative number.
/// @remarks The function meant to be used within a loop.
int PollIn_SPI(uint8_t* rx_buffer, size_t buffer_size, uint32_t timeout);

/// @brief Dispose the SPI
void Dispose_SPI(void);

#endif
