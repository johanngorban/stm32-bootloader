#include "flash.h"
#include "config.h"
#include "memory_map.h"

flash_status_t flash_write(uint32_t *addr, uint32_t *data, uint32_t length) {
    if ((addr + length - 1) > (uint32_t *) FLASH_END) {
        return FLASH_ERROR;
    }

    flash_status_t status = ported_flash_write_aligned(addr, data, length);
    return status;
}

flash_status_t flash_erase(uint8_t *addr, uint32_t length) {
    if ((uint32_t) (addr + length - 1) > FLASH_END) {
        return FLASH_ERROR;
    }

    flash_status_t status = ported_flash_erase_aligned(addr, length);
    return status;
}
