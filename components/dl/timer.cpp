#include "dl/timer.hpp"

#include "esp_check.h"

namespace dl
{

    void Timer::init(esp_timer_cb_t callback, void *arg, const char *name)
    {
        esp_timer_create_args_t args{};
        args.callback = callback;
        args.arg = arg;

        args.dispatch_method = ESP_TIMER_TASK;
        args.name = name;
        ESP_ERROR_CHECK(esp_timer_create(&args, &handle_));
    }

    void Timer::stop()
    {
        if (handle_ && esp_timer_is_active(handle_))
        {
            ESP_ERROR_CHECK(esp_timer_stop(handle_));
        }
    }

    void Timer::startOnce(uint32_t milliseconds)
    {
        stop();
        ESP_ERROR_CHECK(esp_timer_start_once(handle_,
                                             static_cast<uint64_t>(milliseconds) * 1000ULL));
    }

    void Timer::startPeriodic(uint32_t milliseconds)
    {
        stop();
        ESP_ERROR_CHECK(esp_timer_start_periodic(handle_,
                                                 static_cast<uint64_t>(milliseconds) * 1000ULL));
    }

} // namespace dl
