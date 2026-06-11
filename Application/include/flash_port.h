#pragma once

#include <stdint.h>

typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR = 1,
    FLASH_ALIGNMENT_ERROR = 2,
} flash_status_t;

flash_status_t ported_flash_erase_aligned(uint8_t *addr, uint32_t length);

// TODO: uint32_t *data -> uint8_t *data
flash_status_t ported_flash_write_aligned(uint32_t *addr, uint32_t *data, uint32_t length);
