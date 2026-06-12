#include "mcu.h"
#include "time.h"

static volatile uint32_t ticks = 0;

extern uint32_t SystemCoreClock;

// TODO: implement
void time_init() {
    SysTick_Config(SystemCoreClock / 1000U);
}

// TODO: implement
uint32_t get_tick() {
    return ticks;
}

void SysTick_Handler() {
    ticks++;
}
