#include "jump.h"
#include "config.h"
#include "image.h"
#include "jump_port.h"
#include <stddef.h>

void jump_to_slot(uint8_t slot) {
    uint8_t *slot_addr = NULL;

    if (slot == 1) {
        slot_addr = FIRMWARE_SLOT_1_START;
    } else if (slot == 2) {
        slot_addr = FIRMWARE_SLOT_2_START;
    }

    uint8_t *app_addr = slot_addr + IMAGE_METADATA_SIZE;
    jump_to_addr(app_addr);
}

int8_t jump_to_slot_with_verification(uint8_t slot) {
    // Do not start, if image is invalid
    if (image_verify(slot) != 0) {
        return -1;
    }

    jump_to_slot(slot);
    return -1;
}
