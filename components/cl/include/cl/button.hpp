#pragma once

#include <cstdint>
#include "dl/gpio_input.hpp"
#include "dl/timer.hpp"
#include "cl/event_dispatcher.hpp"

namespace cl
{

    enum class ButtonId : uint8_t
    {
        SW1,
        SW2,
        SW3,
        SW4
    };
    enum class ButtonGesture : uint8_t
    {
        ShortPress,
        LongPress,
        DoublePress
    };
    using ButtonCallback = void (*)(ButtonId, ButtonGesture);

    class Button
    {
    public:
        Button(ButtonId id, gpio_num_t pin, bool hasLongPress,
               bool hasDoublePress, EventDispatcher &events, ButtonCallback callback);

        void init();
        void handle(EventType type);

    private:
        static void gpioIsr(void *arg);
        static void debounceTimerCallback(void *arg);
        static void gestureTimerCallback(void *arg);

        void onDebounceTimeout();
        void onGestureTimeout();
        void emit(ButtonGesture gesture);

        ButtonId id_;
        bool has_long_;
        bool has_double_;
        EventDispatcher &events_;
        ButtonCallback callback_;
        dl::GpioInput input_;
        dl::Timer debounce_timer_;
        dl::Timer gesture_timer_;

        bool stable_pressed_ = false;
        bool long_sent_ = false;
        bool waiting_second_ = false;
        bool second_press_ = false;
        int64_t press_at_us_ = 0;
        int64_t first_release_at_us_ = 0;
    };

} 
