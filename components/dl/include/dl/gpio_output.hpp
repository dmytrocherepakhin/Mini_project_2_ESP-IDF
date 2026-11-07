#pragma once

#include "driver/gpio.h"

namespace dl
{

    class GpioOutput
    {
    public:
        explicit GpioOutput(gpio_num_t pin) : pin_(pin) {}

        void init();
        void set(bool high);

    private:
        gpio_num_t pin_;
    };

}
