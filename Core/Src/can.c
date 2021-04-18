#include "can.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "main.h"
#include "lcd.h"
#include "led.h"

extern CAN_HandleTypeDef hcan;

void CAN_sendLedStatusMessage(uint8_t ledCode) {
    CAN_TxHeaderTypeDef header;
    uint8_t data[8];
    data[0] = ledCode;
    data[1] = MCU_ID;

    header.DLC = 2; //Data length = 2 bytes
    header.StdId = MSG_LED_STATUS_ID;
    header.RTR = CAN_RTR_DATA;
    header.ExtId = 0;
    header.IDE = CAN_ID_STD;
    while (!HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {}  //wait for TX buffer
    uint32_t mailbox;
    HAL_CAN_AddTxMessage(&hcan, &header, data, &mailbox);
    LCD_CANPrintTxInfo(&header, data);
}

void CAN_sendRandomLedMessage() {
    CAN_TxHeaderTypeDef header;

    header.DLC = 0; //Data length = 0 bytes
    header.StdId = MSG_LED_RANDOM_ID;
    header.RTR = CAN_RTR_DATA;
    header.ExtId = 0;
    header.IDE = CAN_ID_STD;
    while (!HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {}
    uint32_t mailbox;
    HAL_CAN_AddTxMessage(&hcan, &header, NULL, &mailbox);
    LCD_CANPrintTxInfo(&header, NULL);
}


void CAN_sendLedStatusRequestLedMessage() {
    CAN_TxHeaderTypeDef header;

    header.DLC = 0; //Data length = 0 bytes
    header.StdId = MSG_LED_STATUS_ID;
    header.RTR = CAN_RTR_REMOTE;
    header.ExtId = 0;
    header.IDE = 0;
    while (!HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {}
    uint32_t mailbox;
    HAL_CAN_AddTxMessage(&hcan, &header, NULL, &mailbox);
    LCD_CANPrintTxInfo(&header, NULL);
}

void CAN_Init() {
    srand(HAL_GetTick());

    CAN_FilterTypeDef filterCfg;
    filterCfg.FilterMode = CAN_FILTERMODE_IDMASK;
    filterCfg.FilterScale = CAN_FILTERSCALE_16BIT;
    filterCfg.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filterCfg.FilterMaskIdLow = 0xfff8;
    filterCfg.FilterMaskIdHigh = 0xfFF8;
    filterCfg.FilterActivation = CAN_FILTER_ENABLE;

    //filter bank 0
    //got led color msg
    filterCfg.FilterBank = 0;
    filterCfg.FilterIdLow = MSG_LED_STATUS_ID << 5;
    filterCfg.FilterIdHigh = 0x00;

    HAL_CAN_ConfigFilter(&hcan, &filterCfg);

    //filter bank 1
    //Got LED random message
    filterCfg.FilterBank = 1;
    filterCfg.FilterIdLow = MSG_LED_RANDOM_ID << 5;
    filterCfg.FilterIdHigh = 0x00;

    HAL_CAN_ConfigFilter(&hcan, &filterCfg);
    //filter bank 2
    //Got led color request message
    filterCfg.FilterBank = 2;
    filterCfg.FilterIdLow = (MSG_LED_STATUS_ID << 5) | (1 << 4);
    filterCfg.FilterIdHigh = 0x00;

    HAL_CAN_ConfigFilter(&hcan, &filterCfg);

    //filter bank 4
    //default filter for all messages

    filterCfg.FilterBank = 4;
    filterCfg.FilterIdHigh = 0;
    filterCfg.FilterIdLow = 0;
    filterCfg.FilterMaskIdLow = 0;
    filterCfg.FilterMaskIdHigh = 0;
    HAL_CAN_ConfigFilter(&hcan, &filterCfg);

    HAL_CAN_Start(&hcan);

}

void CAN_loopTick() {
    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)) {
        CAN_RxHeaderTypeDef rxHeader;
        uint8_t data[8];
        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, data);

        LCD_clear();
        LCD_setCursor(0, 0);
        uint8_t ledCode;
        bool respondFlag = false;
        switch (rxHeader.FilterMatchIndex) {
            case 0:
                LCD_sendString("FB 0, F 0");
                LCD_setCursor(1, 0);
                LCD_sendString("LED status msg");
                LED_setCode(data[0]);
                break;
            case 2:
                LCD_sendString("FB 1, F 2");
                LCD_setCursor(1, 0);
                LCD_sendString("LED random msg");
                ledCode = rand() & 0b111; // NOLINT(cert-msc30-c, cert-msc50-cpp)
                LED_setCode(ledCode);
                break;
            case 4:
                LCD_sendString("FB 2, F 4");
                LCD_setCursor(1, 0);
                LCD_sendString("LED status RTR");
                respondFlag = true;
                break;
            default:
                LCD_sendString("FB 4, F 8");
                char stringBuff[17];
                LCD_setCursor(1, 0);
                sprintf(stringBuff, "%02ld", rxHeader.FilterMatchIndex);
                break;
        }
        HAL_Delay(1000);
        LCD_CANPrintRxInfo(&rxHeader, data);
        if (respondFlag) {
            HAL_Delay(1000);
            ledCode = LED_getCode();
            CAN_sendLedStatusMessage(ledCode);
        }
    }
}

