#include "cl/event_dispatcher.hpp"
#include "config.hpp"
#include <stdio.h>

namespace cl
{

    void EventDispatcher::init()
    {
        // FreeRTOS: створити чергу заданої місткості.
        queue_ = xQueueCreate(config::events::QUEUE_LENGTH, sizeof(Event));
        // FreeRTOS: перевірити успішність створення черги.
        configASSERT(queue_ != nullptr);
    }

    bool EventDispatcher::postFromIsr(const Event &event)
    {
        // FreeRTOS: API потребує адресу BaseType_t.
        BaseType_t wake = false;
        // FreeRTOS: надсилання з переривання без очікування.
        const bool sent = xQueueSendFromISR(queue_, &event, &wake) == true;
        if (wake == true)
        {
            // FreeRTOS: запит перемикання на розбуджену задачу після виходу з ISR.
            portYIELD_FROM_ISR();
        }
        return sent;
    }

    bool EventDispatcher::post(const Event &event)
    {
        // FreeRTOS: надсилання із задачі.
        return xQueueSend(queue_, &event, 0) == true;
    }

    void EventDispatcher::run(void (*handler)(const Event &))
    {
        Event event{};
        while (true)
        {
            // FreeRTOS: очікувати повідомлення з черги без тайм-ауту.
            if (xQueueReceive(queue_, &event, portMAX_DELAY) == true)
            {
                const char *name = "Unknown";
                switch (event.type)
                {
                case EventType::GpioEdge:
                    name = "GPIO changed";
                    break;
                case EventType::DebounceTimeout:
                    name = "Debounce timer expired";
                    break;
                case EventType::GestureTimeout:
                    name = "Gesture timer expired";
                    break;
                case EventType::BlinkTick:
                    name = "Blink tick";
                    break;
                }

                // FreeRTOS: кількість повідомлень, що залишилися після отримання поточного. Поточний знімок черги.
                const auto waiting = uxQueueMessagesWaiting(queue_);
                printf("Event: %s | Queue waiting: %u / %u\n",
                       name,
                       static_cast<unsigned>(waiting),
                       static_cast<unsigned>(config::events::QUEUE_LENGTH));

                handler(event);
            }
        }
    }

}
