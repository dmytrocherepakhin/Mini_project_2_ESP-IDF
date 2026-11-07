#include "dl/gpio_output.hpp"

#include "esp_check.h"

namespace dl
{

    void GpioOutput::init()
    {
        gpio_config_t cfg{};
        cfg.pin_bit_mask = 1ULL << static_cast<unsigned>(pin_);
        cfg.mode = GPIO_MODE_OUTPUT;
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_DISABLE;

        ESP_ERROR_CHECK(gpio_config(&cfg));
        set(false);
    }

    void GpioOutput::set(bool high)
    {
        ESP_ERROR_CHECK(gpio_set_level(pin_, high ? 1 : 0));
    }

} // namespace dl
