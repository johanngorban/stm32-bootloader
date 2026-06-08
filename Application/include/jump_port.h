#pragma once

#include <stdint.h>

typedef void (*jump_to_app_t)();

void jump_to_addr(uint8_t *addr);
