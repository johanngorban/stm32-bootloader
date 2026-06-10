#pragma once

#include <stdint.h>

#define UART_MAX_DELAY (1000)

typedef enum {
    UART_OK,
    UART_TIMEOUT,
    UART_BUSY,
} uart_status_t;

void uart_init(uint32_t baud);

uart_status_t uart_send(uint8_t *data, uint32_t length, uint32_t timeout);

uart_status_t uart_recv(uint8_t *data, uint32_t length, uint32_t timeout);
