#pragma once

#pragma once
#if defined(STM32F1)
#include "stm32f1xx.h"
#elif defined(STM32G0)
#include "stm32g0xx.h"
#else
#error "MCU family not defined"
#endif
