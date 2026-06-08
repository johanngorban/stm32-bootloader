#include "jump_port.h"
#include "stm32f1_common.h"

void jump_to_addr(uint8_t *addr) {
    uint32_t msp = *(volatile uint32_t *) addr;
    uint32_t reset_handler = *(volatile uint32_t *) (addr + 4);
    jump_to_app_t jump_to_app = (jump_to_app_t) reset_handler;

    __disable_irq();
    SCB->VTOR = (uint32_t) addr;
    __enable_irq();

    __set_MSP(msp);
    jump_to_app();
}
