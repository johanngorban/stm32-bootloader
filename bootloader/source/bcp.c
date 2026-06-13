#include "bcp.h"
#include "crc.h"
#include "uart.h"
#include <string.h>

#define BCP_RECV_MAX_ATTEMPTS (5)

uint8_t bcp_request_init(bcp_request_t *request) {
    if (request == NULL) {
        return 1;
    }
    memset(request, 0, sizeof(bcp_request_t));
    return 0;
}

uint8_t bcp_response_init(bcp_response_t *response) {
    if (response == NULL) {
        return 1;
    }
    memset(response, 0, sizeof(bcp_response_t));
    response->status = BCP_OK;

    return 0;
}

int8_t bcp_send_response(const bcp_response_t *response) {
    uint16_t packet_length = 1 + BCP_RESPONSE_HEADER_SIZE + response->length + 2;
    uint8_t packet[packet_length];

    packet[0] = BCP_SOF_BYTE;
    packet[1] = response->command;
    packet[2] = response->status;
    packet[3] = response->length;
    memcpy(&packet[4], response->data, response->length);

    uint16_t crc = crc16_modbus((const uint8_t *) response, response->length + BCP_RESPONSE_HEADER_SIZE);
    packet[4 + response->length] = (uint8_t) (crc >> 8) & 0xFF;
    packet[5 + response->length] = (uint8_t) crc & 0xFF;

    uart_send(packet, packet_length, 100);

    return 0;
}

int8_t bcp_recv_request(bcp_request_t *request) {
    uint8_t sof_byte = 0;
    do {
        if (uart_recv(&sof_byte, 1, UART_MAX_DELAY) != UART_OK) {
            return BCP_RECV_TIMEOUT;
        }
    } while (sof_byte != BCP_SOF_BYTE);

    if (uart_recv(&request->command, 1, UART_MAX_DELAY) != UART_OK) {
        return BCP_RECV_TIMEOUT;
    }
    if (uart_recv(&request->length, 1, UART_MAX_DELAY) != UART_OK) {
        return BCP_RECV_TIMEOUT;
    }
    if (request->length > 0) {
        if (uart_recv(request->data, request->length, UART_MAX_DELAY) != UART_OK) {
            return BCP_RECV_TIMEOUT;
        }
    }
    uint8_t actual_crc[2];
    if (uart_recv(actual_crc, 2, UART_MAX_DELAY) != UART_OK) {
        return BCP_RECV_TIMEOUT;
    }

    uint16_t expected_crc = crc16_modbus((const uint8_t *) request, request->length + BCP_REQUEST_HEADER_SIZE);
    if (expected_crc != ((uint16_t) (actual_crc[0] << 8) | (actual_crc[1]))) {
        return BCP_RECV_ERROR;
    }

    return BCP_RECV_OK;
}
