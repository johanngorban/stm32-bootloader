#include "flash.h"
#include "config.h"
#include "stm32f1xx_hal.h"

#define MCU_WORD_SIZE (4u)
#define FLASH_END (FLASH_BASE + FLASH_SIZE - 1)
#define FLASH_WORD_SIZE (1024u)

static inline flash_status_t __flash_write_aligned(uint32_t *addr, uint32_t *data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t) (addr + i), data[i]);
        if (status != HAL_OK) {
            return FLASH_ERROR;
        }
    }

    return FLASH_OK;
}

flash_status_t flash_write(uint32_t *addr, uint32_t *data, uint32_t length) {
    if (((uint32_t) addr % MCU_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    if ((addr + length - 1) > (uint32_t *) FLASH_END) {
        return FLASH_ERROR;
    }

    __disable_irq();

    flash_status_t status = __flash_write_aligned(addr, data, length);

    HAL_FLASH_Lock();
    __enable_irq();

    return status;
}

static inline flash_status_t __flash_erase_aligned(uint8_t *addr, uint32_t length) {
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

flash_status_t flash_erase(uint8_t *addr, uint32_t length) {
    // Check start address to be page aligned
    if (((uint32_t) addr % FLASH_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    // Check size to erase to be page aligned
    if ((length % FLASH_WORD_SIZE) > 0) {
        return FLASH_ALIGNMENT_ERROR;
    }

    if ((uint32_t) (addr + length - 1) > FLASH_END) {
        return FLASH_ERROR;
    }

    __disable_irq();
    flash_status_t status = __flash_erase_aligned(addr, length);
    __enable_irq();

    return status;
}
