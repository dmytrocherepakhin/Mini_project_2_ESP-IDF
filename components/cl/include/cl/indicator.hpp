#pragma once

#include "dl/gpio_output.hpp"

namespace cl
{

    class Indicator
    {
    public:
        explicit Indicator(dl::GpioOutput &output) : output_(output) {}
        void set(bool on);

    private:
        dl::GpioOutput &output_;
    };

}
