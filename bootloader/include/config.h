#pragma once

#include "memory_map.h"

#define MAX_WAIT_TIME_MS (10000)

/**
 * Bootloader version
 */
#define BOOTLOADER_MAJOR_VERSION (1)
#define BOOTLOADER_MINOR_VERSION (2)
#define BOOTLOADER_PATCH_VERSION (0)
#define MAX_WAIT_TIME_MS         (10000)

/**
 * Memory map
 */
#define BOOTLOADER_START ((uint8_t *) 0x08000000)
#define BOOTLOADER_SIZE  (20 * 1024) // 20Kb
