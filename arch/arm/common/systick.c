#include "mcu.h"
#include "time.h"

static volatile uint32_t s_ticks = 0;

// TODO: implement
void time_init() {
}

// TODO: implement
uint32_t get_tick() {
    return 0;
}

void SysTick_Handler() {
    s_ticks++;
}
