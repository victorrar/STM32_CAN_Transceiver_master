#include "led.h"

uint8_t LED_getCode() {
    uint8_t out = HAL_GPIO_ReadPin(LED_1_GPIO_Port, LED_1_Pin);
    out |= HAL_GPIO_ReadPin(LED_2_GPIO_Port, LED_2_Pin) << 1;
    out |= HAL_GPIO_ReadPin(LED_3_GPIO_Port, LED_3_Pin) << 2;
    return out;
}

void LED_setCode(uint8_t ledCode) {
    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, ledCode & 0x01);
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, ledCode & 0x02);
    HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, ledCode & 0x04);
}
