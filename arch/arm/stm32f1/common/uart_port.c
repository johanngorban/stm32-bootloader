#include "mcu.h"
#include "uart.h"
#include <stm32f1xx_hal.h>

static UART_HandleTypeDef huart1;

void uart_init(uint32_t baud) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    huart1.Instance = USART1;
    huart1.Init.BaudRate = baud;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
    // GPIOA->CRH &= ~0xFF0; // PA9
    // GPIOA->CRH |= 0x4B0;  // PA10

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uart_status_t uart_send(uint8_t *data, uint32_t length, uint32_t timeout) {
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, data, (uint16_t) length, timeout);
    if (status != HAL_OK) {
        return UART_TIMEOUT;
    }

    return UART_OK;
}

uart_status_t uart_recv(uint8_t *data, uint32_t length, uint32_t timeout) {
    HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, data, (uint16_t) length, timeout);
    if (status != HAL_OK) {
        return UART_TIMEOUT;
    }

    return UART_OK;
}
