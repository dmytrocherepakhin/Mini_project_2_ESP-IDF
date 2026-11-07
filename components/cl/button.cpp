#include "cl/button.hpp"
#include "config.hpp"

#include "esp_timer.h"

namespace cl
{

    Button::Button(ButtonId id, gpio_num_t pin, bool hasLongPress,
                   bool hasDoublePress, EventDispatcher &events,
                   ButtonCallback callback)
        : id_(id), has_long_(hasLongPress), has_double_(hasDoublePress),
          events_(events), callback_(callback), input_(pin) {}

    void Button::init()
    {
        debounce_timer_.init(&Button::debounceTimerCallback, this, "button_debounce");
        if (has_long_ || has_double_)
        {
            gesture_timer_.init(&Button::gestureTimerCallback, this, "button_gesture");
        }
        input_.init(&Button::gpioIsr, this);
    }

    void Button::gpioIsr(void *arg)
    {
        auto *self = static_cast<Button *>(arg);
        // Непряме використання FreeRTOS: передача події через ISR-безпечний метод черги.
        self->events_.postFromIsr({EventType::GpioEdge, self});
    }

    void Button::debounceTimerCallback(void *arg)
    {
        auto *self = static_cast<Button *>(arg);
        // Непряме використання FreeRTOS: обробник esp_timer надсилає подію з контексту задачі.
        self->events_.post({EventType::DebounceTimeout, self});
    }

    void Button::gestureTimerCallback(void *arg)
    {
        auto *self = static_cast<Button *>(arg);
        // Непряме використання FreeRTOS: обробник esp_timer надсилає подію з контексту задачі.
        self->events_.post({EventType::GestureTimeout, self});
    }

    void Button::handle(EventType type)
    {
        switch (type)
        {
        case EventType::GpioEdge:
            debounce_timer_.startOnce(config::timing::DEBOUNCE_MS);
            break;
        case EventType::DebounceTimeout:
            onDebounceTimeout();
            break;
        case EventType::GestureTimeout:
            onGestureTimeout();
            break;
        default:
            break;
        }
    }

    void Button::onDebounceTimeout()
    {
        const bool pressed_now = input_.pressed();
        if (pressed_now == stable_pressed_)
        {
            return;
        }

        stable_pressed_ = pressed_now;
        const int64_t now = esp_timer_get_time();

        if (stable_pressed_)
        {
            press_at_us_ = now;
            long_sent_ = false;

            if (has_double_ && waiting_second_)
            {
                const bool within_window =
                    now - first_release_at_us_ <= static_cast<int64_t>(config::timing::DOUBLE_PRESS_MS) * 1000;
                waiting_second_ = false;
                gesture_timer_.stop();
                if (within_window)
                {
                    second_press_ = true;
                    return;
                }
                emit(ButtonGesture::ShortPress);
            }

            if (has_long_)
            {
                gesture_timer_.startOnce(config::timing::LONG_PRESS_MS);
            }
            return;
        }

        if (long_sent_)
        {
            long_sent_ = false;
            return;
        }
        if (second_press_)
        {
            second_press_ = false;
            emit(ButtonGesture::DoublePress);
            return;
        }
        if (has_double_)
        {
            waiting_second_ = true;
            first_release_at_us_ = now;
            gesture_timer_.startOnce(config::timing::DOUBLE_PRESS_MS);
        }
        else
        {
            if (has_long_)
            {
                gesture_timer_.stop();
            }
            emit(ButtonGesture::ShortPress);
        }
    }

    void Button::onGestureTimeout()
    {
        if (has_long_ && stable_pressed_ && !long_sent_)
        {
            const int64_t held_us = esp_timer_get_time() - press_at_us_;
            if (input_.pressed() && held_us >= static_cast<int64_t>(config::timing::LONG_PRESS_MS) * 1000)
            {
                long_sent_ = true;
                emit(ButtonGesture::LongPress);
            }
        }
        else if (has_double_ && waiting_second_ && !stable_pressed_)
        {
            const int64_t elapsed_us = esp_timer_get_time() - first_release_at_us_;
            if (elapsed_us >= static_cast<int64_t>(config::timing::DOUBLE_PRESS_MS) * 1000)
            {
                waiting_second_ = false;
                emit(ButtonGesture::ShortPress);
            }
        }
    }

    void Button::emit(ButtonGesture gesture)
    {
        callback_(id_, gesture);
    }

}
