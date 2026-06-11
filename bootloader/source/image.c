#include "image.h"
#include "config.h"
#include "crc.h"

int8_t image_verify(uint8_t slot) {
    if ((slot < 1) || (slot > 2)) {
        return -1;
    }

    const uint8_t *image_addr = FIRMWARE_SLOT_1_START + FIRMWARE_SLOT_SIZE * (slot - 1);
    const image_metadata_t *metadata = (const image_metadata_t *) image_addr;

    if (metadata->magic != IMAGE_MAGIC_NUMBER) {
        return -1;
    }

    if (metadata->size == 0) {
        return -1;
    }

    if ((metadata->size + IMAGE_METADATA_SIZE) > FIRMWARE_SLOT_SIZE) {
        return -1;
    }

    const uint8_t *image_body_addr = image_addr + IMAGE_METADATA_SIZE;
    uint32_t image_body_crc = crc32_iso_hdlc(image_body_addr, metadata->size);
    if (image_body_crc != metadata->crc) {
        return -1;
    }

    return 0;
}
