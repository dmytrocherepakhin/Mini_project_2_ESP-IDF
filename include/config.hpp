#pragma once

#include <cstdint>
#include "driver/gpio.h"

namespace config
{

    namespace pins
    {
        constexpr gpio_num_t SW1 = GPIO_NUM_7;
        constexpr gpio_num_t SW2 = GPIO_NUM_8;
        constexpr gpio_num_t SW3 = GPIO_NUM_15;
        constexpr gpio_num_t SW4 = GPIO_NUM_16;

        constexpr gpio_num_t D1 = GPIO_NUM_2;
        constexpr gpio_num_t D2 = GPIO_NUM_1;
        constexpr gpio_num_t D3_D4 = GPIO_NUM_17;
        constexpr gpio_num_t RELAY = GPIO_NUM_18;
    }

    namespace timing
    {
        constexpr uint32_t DEBOUNCE_MS = 50;
        constexpr uint32_t LONG_PRESS_MS = 2000;
        constexpr uint32_t DOUBLE_PRESS_MS = 300;
        constexpr uint32_t BLINK_HALF_PERIOD_MS = 500;
    }

    namespace events
    {
        constexpr uint32_t QUEUE_LENGTH = 32;
    }

}
