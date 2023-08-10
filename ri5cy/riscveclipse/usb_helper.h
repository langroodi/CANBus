#ifndef USB_HELPER_H
#define USB_HELPER_H

#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define CONTROLLER_ID kUSB_ControllerKhci0
#define DATA_BUFF_SIZE FS_CDC_VCOM_BULK_OUT_PACKET_SIZE

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/* Currently configured line coding */
#define LINE_CODING_SIZE (0x07)
#define LINE_CODING_DTERATE (115200)
#define LINE_CODING_CHARFORMAT (0x00)
#define LINE_CODING_PARITYTYPE (0x00)
#define LINE_CODING_DATABITS (0x08)

/* Communications feature */
#define COMM_FEATURE_DATA_SIZE (0x02)
#define STATUS_ABSTRACT_STATE (0x0000)
#define COUNTRY_SETTING (0x0000)

/* Notification of serial state */
#define NOTIF_PACKET_SIZE (0x08)
#define UART_BITMAP_SIZE (0x02)
#define NOTIF_REQUEST_TYPE (0xA1)

/* Define the types for application */
typedef struct _usb_cdc_vcom_struct
{
    usb_device_handle deviceHandle; /* USB device handle. */
    class_handle_t cdcAcmHandle; /* USB CDC ACM class handle.                                                         */
    volatile uint8_t attach;     /* A flag to indicate whether a usb device is attached. 1: attached, 0: not attached */
    TaskHandle_t deviceTaskHandle;      /* USB device task handle. */
    uint8_t speed; /* Speed of USB device. USB_SPEED_FULL/USB_SPEED_LOW/USB_SPEED_HIGH.                 */
    volatile uint8_t
        startTransactions; /* A flag to indicate whether a CDC device is ready to transmit and receive data.    */
    uint8_t currentConfiguration; /* Current configuration value. */
    uint8_t currentInterfaceAlternateSetting
        [USB_CDC_VCOM_INTERFACE_COUNT]; /* Current alternate setting value for each interface. */
} usb_cdc_vcom_struct_t;

/* Define the infomation relates to abstract control model */
typedef struct _usb_cdc_acm_info
{
    uint8_t serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE]; /* Serial state buffer of the CDC device to notify the
                                                                     serial state to host. */
    bool dtePresent;          /* A flag to indicate whether DTE is present.         */
    uint16_t breakDuration;   /* Length of time in milliseconds of the break signal */
    uint8_t dteStatus;        /* Status of data terminal equipment                  */
    uint8_t currentInterface; /* Current interface index.                           */
    uint16_t uartState;       /* UART state of the CDC device.                      */
} usb_cdc_acm_info_t;

/// @brief USB data reception callback
typedef void (*receive_usb_callback_t)(const uint8_t*, size_t);

#define CONTROLLER_ID kUSB_ControllerKhci0
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/// @brief Initialize the USB CDC
/// @returns '0' if the initialization was successful; otherwise '-1'
int Initialize_USB(void);

/// @brief Send data via USB
/// @param tx_buffer Transmission buffer
/// @param buffer_size Transmission buffer size
/// @returns '0' if the transmission was successful; otherwise '-1'
int Send_USB(uint8_t* tx_buffer, size_t buffer_size);

/// @brief Set USB data received callback
/// @param callback Callback to be invoked when a data is received over USB
/// @see Reset_Receive_USB_Callback
void Set_Receive_USB_Callback(receive_usb_callback_t callback);

/// @brief Reset USB data received callback
/// @see Set_Receive_USB_Callback
void Reset_Receive_USB_Callback();

/// @brief Dispose the USB CDC
void Dispose_USB(void);

#endif
