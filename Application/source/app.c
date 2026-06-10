#include "app.h"
#include "bcp.h"
#include "fwp.h"
#include "config.h"
#include "jump.h"
#include "router.h"
#include "time.h"
#include <stdbool.h>

void app_init() {}

void app_run() {
    bcp_request_t request;
    bcp_request_init(&request);

    bool activity_detected = false;

    uint32_t start_time = get_tick();
    uint32_t waited = 0;
    while (1) {
        if (!activity_detected && waited >= MAX_WAIT_TIME_MS) {
            // Try to jump into slot 1
            jump_to_slot_with_verification(1);

            // Try to jump into slot 2
            jump_to_slot_with_verification(2);

            // If we did not jump, all images are invalid
            activity_detected = true;
        }

        int8_t status = bcp_recv_request(&request);
        if (status == BCP_RECV_OK) {
            activity_detected = true;

            router_handle_request(&request);
            bcp_request_init(&request);
        }

        waited = get_tick() - start_time;
    }
}
