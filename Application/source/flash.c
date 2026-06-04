#include "flash.h"
#include "config.h"
#include "core_mcu.h"

#define MCU_WORD_SIZE   (4u)
#define FLASH_END       (FLASH_BASE + FLASH_SIZE - 1)
#define FLASH_WORD_SIZE (1024u)

flash_status_t flash_write(uint32_t *addr, uint32_t *data, uint32_t length) {
    if (((uint32_t) addr % MCU_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    if ((addr + length - 1) > (uint32_t *) FLASH_END) {
        return FLASH_ERROR;
    }

    __disable_irq();
    flash_status_t status = ported_flash_write_aligned(addr, data, length);
    __enable_irq();

    return status;
}

flash_status_t flash_erase(uint8_t *addr, uint32_t length) {
    // Check start address to be page aligned
    if (((uint32_t) addr % FLASH_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    // Check size to erase to be page aligned
    if ((length % FLASH_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    if ((uint32_t) (addr + length - 1) > FLASH_END) {
        return FLASH_ERROR;
    }

    __disable_irq();
    flash_status_t status = ported_flash_erase_aligned(addr, length);
    __enable_irq();

    return status;
}
