#include "flash_port.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

flash_status_t ported_flash_erase_aligned(uint8_t *addr, uint32_t length) {
    FLASH_EraseInitTypeDef EraseInit;
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInit.PageAddress = (uint32_t) addr;
    EraseInit.NbPages = length / FLASH_PAGE_SIZE;

    HAL_StatusTypeDef status = HAL_OK;
    uint32_t page_err = 0;

    HAL_FLASH_Unlock();
    status = HAL_FLASHEx_Erase(&EraseInit, &page_err);
    HAL_FLASH_Lock();

    if (status != HAL_OK) {
        return FLASH_ERROR;
    }

    return FLASH_OK;
}

flash_status_t ported_flash_write_aligned(uint32_t *addr, uint32_t *data, uint32_t length) {
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < length; i++) {
        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t) (addr + i), data[i]);
        if (status != HAL_OK) {
            HAL_FLASH_Lock();
            return FLASH_ERROR;
        }
    }
    HAL_FLASH_Lock();
    return FLASH_OK;
}
