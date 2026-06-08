#pragma once

#define FLASH_SIZE ((*(volatile uint16_t *) 0x1FFFF7E0) * 1024)
#define MCU_WORD_SIZE (4u)
