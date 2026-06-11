#include "uart.h"
#include "mcu.h"

// TODO: implement
void uart_init(uint32_t baud) {
    GPIOA->CRH &= ~0xFF0; // PA9
    GPIOA->CRH |=  0x4B0; // PA10
}

// TODO: implement
uart_status_t uart_send(uint8_t *data, uint32_t length, uint32_t timeout) {
    return UART_OK;
}

// TODO: implement
uart_status_t uart_recv(uint8_t *data, uint32_t length, uint32_t timeout) {
    return UART_OK;
}
