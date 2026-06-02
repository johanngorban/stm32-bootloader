#include "app.h"
#include "bcp.h"
#include "bcp_io.h"
#include "fwpio.h"
#include "router.h"
#include "jump.h"
#include "config.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>

extern UART_HandleTypeDef huart1;

void app_init() {
    bcp_uart_init(&huart1);
    fwp_init(&huart1);
}

void app_run() {
    bcp_request_t request;
    bcp_request_init(&request);

    bool activity_detected = false;

    uint32_t start_time = HAL_GetTick();
    uint32_t waited = 0;
    while (1) {
        if (!activity_detected && waited >= MAX_WAIT_TIME_MS) {
            // Try to jump into slot 1
            jump_to_slot(1);

            // Try to jump into slot 2
            jump_to_slot(2);

            // If we did not jump, all images are invalid
            activity_detected = true;
        }

        int8_t status = bcp_recv_request(&request);
        if (status == BCP_RECV_OK) {
            activity_detected = true;

            router_handle_request(&request);
            bcp_request_init(&request);
        }

        waited = HAL_GetTick() - start_time;
    }
}
