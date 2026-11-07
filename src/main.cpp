#include "config.hpp"
#include "esp_check.h"
#include <cstdio>

#include "dl/gpio_output.hpp"
#include "cl/button.hpp"
#include "cl/blink_clock.hpp"
#include "cl/event_dispatcher.hpp"
#include "cl/indicator.hpp"
#include "cl/relay.hpp"
#include "bl/door_controller.hpp"
#include "bl/hazard_controller.hpp"
#include "bl/turn_controller.hpp"
#include "bl/crash_controller.hpp"

namespace
{

    void applyOutputs();
    void onButtonGesture(cl::ButtonId id, cl::ButtonGesture gesture);
    void dispatch(const cl::Event &event);

    cl::EventDispatcher events;

    dl::GpioOutput d1_gpio(config::pins::D1);
    dl::GpioOutput d2_gpio(config::pins::D2);
    dl::GpioOutput red_gpio(config::pins::D3_D4);
    dl::GpioOutput relay_gpio(config::pins::RELAY);

    cl::Indicator d1(d1_gpio);
    cl::Indicator d2(d2_gpio);
    cl::Indicator red(red_gpio);
    cl::Relay relay(relay_gpio);
    cl::BlinkClock blink(events);

    cl::Button sw1(cl::ButtonId::SW1, config::pins::SW1, true, false, events, onButtonGesture);
    cl::Button sw2(cl::ButtonId::SW2, config::pins::SW2, false, false, events, onButtonGesture);
    cl::Button sw3(cl::ButtonId::SW3, config::pins::SW3, false, true, events, onButtonGesture);
    cl::Button sw4(cl::ButtonId::SW4, config::pins::SW4, true, false, events, onButtonGesture);

    bl::DoorController door;
    bl::HazardController hazard;
    bl::TurnController turns;
    bl::CrashController crash;

    void applyOutputs()
    {
        const bool needs_blink = crash.emergency() || hazard.on() || turns.any();

        if (needs_blink && !blink.running())
            blink.start();
        if (!needs_blink && blink.running())
            blink.stop();

        const bool hazard_active = crash.emergency() || hazard.on();
        const bool phase = blink.phaseOn();

        d1.set(phase && (hazard_active || turns.left()));
        d2.set(phase && (hazard_active || turns.right()));
        red.set(crash.redOn());
        relay.set(crash.emergency() || door.on());
    }

    void onButtonGesture(cl::ButtonId id, cl::ButtonGesture gesture)
    {
        using cl::ButtonGesture;
        using cl::ButtonId;

        // SW1 LONG: єдиний вихід з аварійного режиму.
        if (id == ButtonId::SW1 && gesture == ButtonGesture::LongPress)
        {
            crash.exitEmergency();
            crash.resetRed();
            hazard.reset();
            turns.reset();
            door.forceOn();
            applyOutputs();
            return;
        }

        // У Emergency ігноруємо всі команди, крім SW1 LONG, обробленої вище.
        if (crash.emergency())
            return;

        switch (id)
        {
        case ButtonId::SW1:
            if (gesture == ButtonGesture::ShortPress)
                door.toggle();
            break;

        case ButtonId::SW2:
            if (gesture == ButtonGesture::ShortPress)
                hazard.toggle();
            break;

        case ButtonId::SW3:
            if (gesture == ButtonGesture::ShortPress)
                turns.toggleLeft();
            if (gesture == ButtonGesture::DoublePress)
                turns.toggleRight();
            break;

        case ButtonId::SW4:
            if (gesture == ButtonGesture::ShortPress)
                crash.toggleRed();
            if (gesture == ButtonGesture::LongPress)
            {
                crash.enterEmergency();
                door.forceOn();
                hazard.forceOn();
            }
            break;
        }

        applyOutputs();
    }

    void dispatch(const cl::Event &event)
    {
        if (event.type == cl::EventType::BlinkTick)
        {
            auto *clock = static_cast<cl::BlinkClock *>(event.source);
            clock->onTick();
            applyOutputs();
        }
        else
        {
            auto *button = static_cast<cl::Button *>(event.source);
            button->handle(event.type);
        }
    }

}

extern "C" void app_main(void)
{
    // Непряме використання FreeRTOS: створення черги всередині диспетчера.
    events.init();
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    d1_gpio.init();
    d2_gpio.init();
    red_gpio.init();
    relay_gpio.init();

    blink.init();
    sw1.init();
    sw2.init();
    sw3.init();
    sw4.init();

    applyOutputs();
    std::puts("Initialized. Waiting for events.");
    // Непряме використання FreeRTOS: обробка отриманих подій.
    events.run(&dispatch);
}
