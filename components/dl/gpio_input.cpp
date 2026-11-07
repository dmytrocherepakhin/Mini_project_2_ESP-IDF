#include "dl/gpio_input.hpp"

#include "esp_check.h"

namespace dl
{

    void GpioInput::init(gpio_isr_t handler, void *arg)
    {
        gpio_config_t cfg{};
        cfg.pin_bit_mask = 1ULL << static_cast<unsigned>(pin_);
        cfg.mode = GPIO_MODE_INPUT;
        cfg.pull_up_en = GPIO_PULLUP_ENABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_ANYEDGE;

        ESP_ERROR_CHECK(gpio_config(&cfg));
        ESP_ERROR_CHECK(gpio_isr_handler_add(pin_, handler, arg));
    }

    bool GpioInput::pressed() const
    {
        return gpio_get_level(pin_) == 0;
    }

} 
