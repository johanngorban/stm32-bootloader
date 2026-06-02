#include "jump.h"
#include "config.h"
#include "image.h"
#include "stm32f1xx.h"
#include <stddef.h>

typedef void (*jump_to_app_t)();

int8_t jump_to_slot(uint8_t slot) {
    uint8_t *slot_addr = NULL;

    if (slot == 1) {
        slot_addr = FIRMWARE_SLOT_1_START;
    } else if (slot == 2) {
        slot_addr = FIRMWARE_SLOT_2_START;
    } else {
        return -1;
    }

    // Do not start, if image is invalid
    if (image_verify(slot) != 0) {
        return -1;
    }

    uint8_t *app_addr = slot_addr + IMAGE_METADATA_SIZE;
    uint32_t app_msp = *(volatile uint32_t *) app_addr;
    uint32_t app_reset = *(volatile uint32_t *) (app_addr + 4);
    jump_to_app_t jump_to_app = (jump_to_app_t) app_reset;

    __disable_irq();
    SCB->VTOR = (uint32_t) app_addr;
    __enable_irq();

    __set_MSP(app_msp);
    jump_to_app();

    return -1;
}
