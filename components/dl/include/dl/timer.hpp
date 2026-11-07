#pragma once

#include <cstdint>
#include "esp_timer.h"

namespace dl
{

    class Timer
    {
    public:
        void init(esp_timer_cb_t callback, void *arg, const char *name);
        void startOnce(uint32_t milliseconds);
        void startPeriodic(uint32_t milliseconds);
        void stop();

    private:
        esp_timer_handle_t handle_ = nullptr;
    };

}