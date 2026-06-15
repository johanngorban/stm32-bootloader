#include "flash_port.h"
#include "mcu.h"
#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <string.h>

#define FLASH_PROGRAM_WORD (8u) // 64 bytes
#define FLASH_PAGE_SIZE (2048u) // 2Kb

flash_status_t ported_flash_erase_aligned(uint8_t *addr, uint32_t length) {
    // Check start address to be page aligned
    if (((uint32_t) addr % FLASH_PAGE_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    // Check size to erase to be page aligned
    if ((length % FLASH_PAGE_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    FLASH_EraseInitTypeDef erase;
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Banks = FLASH_BANK_1;
    erase.Page = (((uint32_t) addr - FLASH_BASE) / FLASH_PAGE_SIZE);
    erase.NbPages = length / FLASH_PAGE_SIZE;

    HAL_StatusTypeDef status = HAL_OK;
    uint32_t page_err = 0;

    __disable_irq();
    HAL_FLASH_Unlock();
    status = HAL_FLASHEx_Erase(&erase, &page_err);
    HAL_FLASH_Lock();
    __enable_irq();

    if (status != HAL_OK) {
        return FLASH_ERROR;
    }

    return FLASH_OK;
}

flash_status_t ported_flash_write_aligned(uint32_t *addr, uint32_t *data, uint32_t length) {
    if (((uint32_t) addr % FLASH_PROGRAM_WORD) || (length % 2)) {
        return FLASH_ALIGNMENT_ERROR;
    }

    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < length; i += 2) {
        uint64_t data_word;
        memcpy(&data_word, &data[i], FLASH_PROGRAM_WORD);

        HAL_StatusTypeDef status = HAL_FLASH_Program(
            FLASH_TYPEPROGRAM_DOUBLEWORD,
            (uint32_t) (addr + i),
            data_word
        );
        if (status != HAL_OK) {
            HAL_FLASH_Lock();
            return FLASH_ERROR;
        }
    }
    HAL_FLASH_Lock();
    return FLASH_OK;
}
