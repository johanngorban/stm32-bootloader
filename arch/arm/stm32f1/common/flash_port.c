#include "flash_port.h"
#include "stm32f1_common.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

#define FLASH_WORD_SIZE (1024u)

flash_status_t ported_flash_erase_aligned(uint8_t *addr, uint32_t length) {
    // Check start address to be page aligned
    if (((uint32_t) addr % FLASH_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    // Check size to erase to be page aligned
    if ((length % FLASH_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    FLASH_EraseInitTypeDef EraseInit;
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInit.PageAddress = (uint32_t) addr;
    EraseInit.NbPages = length / FLASH_PAGE_SIZE;

    HAL_StatusTypeDef status = HAL_OK;
    uint32_t page_err = 0;

    __disable_irq();
    HAL_FLASH_Unlock();
    status = HAL_FLASHEx_Erase(&EraseInit, &page_err);
    HAL_FLASH_Lock();
    __enable_irq();

    if (status != HAL_OK) {
        return FLASH_ERROR;
    }

    return FLASH_OK;
}

flash_status_t ported_flash_write_aligned(uint32_t *addr, uint32_t *data, uint32_t length) {
    if (((uint32_t) addr % MCU_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }
    __disable_irq();
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < length; i++) {
        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t) (addr + i), data[i]);
        if (status != HAL_OK) {
            __enable_irq();
            HAL_FLASH_Lock();
            return FLASH_ERROR;
        }
    }
    __enable_irq();
    HAL_FLASH_Lock();
    return FLASH_OK;
}
