#pragma once

#include <cstdint>
// FreeRTOS: базові типи та API черг операційної системи.
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

namespace cl
{

    enum class EventType : uint8_t
    {
        GpioEdge,
        DebounceTimeout,
        GestureTimeout,
        BlinkTick
    };

    struct Event
    {
        EventType type;
        void *source;
    };

    class EventDispatcher
    {
    public:
        void init();
        bool postFromIsr(const Event &event);
        bool post(const Event &event);
        void run(void (*handler)(const Event &));

    private:
        // FreeRTOS: дескриптор черги.
        QueueHandle_t queue_ = nullptr;
    };

}
