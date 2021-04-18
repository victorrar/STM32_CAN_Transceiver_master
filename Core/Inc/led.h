#ifndef STM32_CAN_TRANSCEIVER_MASTER_LED_H
#define STM32_CAN_TRANSCEIVER_MASTER_LED_H

#include "main.h"

/**
 * Get current LEDs' output state.
 * @return LEDs' state. Only 3 LSB are significant
 */
uint8_t LED_getCode();

/**
 * Sets LEDs' output
 * @param ledCode LEDs' state. Only 3 LSB are significant
 */
void LED_setCode(uint8_t ledCode);

#endif //STM32_CAN_TRANSCEIVER_MASTER_LED_H
