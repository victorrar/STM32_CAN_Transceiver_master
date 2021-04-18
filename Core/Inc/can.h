#ifndef STM32_CAN_TRANSCEIVER_MASTER_CAN_H
#define STM32_CAN_TRANSCEIVER_MASTER_CAN_H

#define MSG_LED_STATUS_ID  0x12
#define MSG_LED_RANDOM_ID  0x34
#define MCU_ID  0x01

/**
 * Send and pretty-print CAN ledStatus message
 * @param ledCode Led values to send.
 * Current led status may be gathered by LED_getCode()
 */
void CAN_sendLedStatusMessage(uint8_t ledCode);

/**
 * Send and pretty-print CAN ledRandom message. T
 * hat encourages listeners to ignite random leds
 */
void CAN_sendRandomLedMessage();

/**
 * Send and pretty-print CAN ledStatus remote request message
 */
void CAN_sendLedStatusRequestLedMessage();

/**
 * Received message polling. Myst be invoked in main loop
 */
void CAN_loopTick();

/**
 * Initialize CAN filters and start CAN bus. Must be invoked before any other CAN function
 */
void CAN_Init();

#endif //STM32_CAN_TRANSCEIVER_MASTER_CAN_H
