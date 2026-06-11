#pragma once

#include "flash_port.h"
#include <stdint.h>

#define FLASH_PAGE_ADDR(page) (FLASH_BASE + (page) * FLASH_PAGE_SIZE)

flash_status_t flash_write(uint32_t *addr, uint32_t *data, uint32_t length);

flash_status_t flash_erase(uint8_t *addr, uint32_t length);
