#pragma once

#include "dl/gpio_output.hpp"

namespace cl
{

    class Relay
    {
    public:
        explicit Relay(dl::GpioOutput &output) : output_(output) {}
        void set(bool on);

    private:
        dl::GpioOutput &output_;
    };

}
