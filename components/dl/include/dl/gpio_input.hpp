#pragma once

#include "driver/gpio.h"

namespace dl
{

    class GpioInput
    {
    public:
        explicit GpioInput(gpio_num_t pin) : pin_(pin) {}

        void init(gpio_isr_t handler, void *arg);
        bool pressed() const; 

    private:
        gpio_num_t pin_;
    };

}
