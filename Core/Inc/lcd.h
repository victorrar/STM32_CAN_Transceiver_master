#ifndef STM32_CAN_TRANSCEIVER_MASTER_LCD_H
#define STM32_CAN_TRANSCEIVER_MASTER_LCD_H

#include "main.h"

/**
 * Blocking delay function
 * @param us    Time in micrseconds
 */
void LCD_delayMicros(uint16_t us);

/**
 * Sends characters to display. If display overflows, continues send characters to not displayed RAM area
 * @param string Character string to display
 */
void LCD_sendString(char *string);

/**
 * For internal use.
 * Sends to display 4 bits of a data/command.
 * @param data Data to send. Only 4 LSB are meaningful
 * @param rs rs = 1 for a data, rs=0 for a command
 */
void LCD_shiftOutHalfByte(uint8_t data, uint8_t rs);

/**
 * Send single 8-bit command to LCD
 * @param cmd Command
 */
void LCD_sendCommand(char cmd);

/**
 * Send single 8-bit character to the current cursor position
 * @param data Generally ASCII Character
 */
void LCD_sendData(char data);

/**
 * Sets cursor's position
 * @param row Row number For 1602: 0 <= row <= 1
 * @param col Column number For 1602: 0 <= col <= 15
 */
void LCD_setCursor(int row, int col);

/**
 * Display initialization. Must be called before any other method call.
 * Display pins are implicitly set by CubeMX
 */
void LCD_init(void);

/**
 * Display some data for debug purposes
 */
void LCD_test();

/**
 * Clear the display
 */
void LCD_clear();

/**
 * Pretty print received CAN message
 * Example:
 * ID:0x001 ri DL2
 * D:0xDE AD BE  RX
 * where:
 * 0x001 - std CAN id
 * r - not RTR message, R - RTR message
 * i - standard id, I - extended ID
 * DL2 - data length
 * D:0xDE AD BE - 3 first bytes of DATA. Not available data not displayed
 * @param header Header of the received message
 * @param data Received data. May be NULL
 */
void LCD_CANPrintRxInfo(CAN_RxHeaderTypeDef *header, uint8_t *data);

/**
 * Pretty print sent CAN message. See LCD_CANPrintRxInfo
 * @param header Header of the received message
 * @param data Received data. May be NULL
 */
void LCD_CANPrintTxInfo(CAN_TxHeaderTypeDef *header, uint8_t *data);

#endif //STM32_CAN_TRANSCEIVER_MASTER_LCD_H
