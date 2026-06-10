#include "time.h"
#include "mcu.h"

static volatile uint32_t s_ticks = 0;

void time_init(void) {
}

uint32_t get_tick(void) {
    return 0;
}

void SysTick_Handler(void) {
    s_ticks++;
}
