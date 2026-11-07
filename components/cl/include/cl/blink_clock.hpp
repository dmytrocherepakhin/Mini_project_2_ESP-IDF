#pragma once

#include "dl/timer.hpp"
#include "cl/event_dispatcher.hpp"

namespace cl
{

    class BlinkClock
    {
    public:
        explicit BlinkClock(EventDispatcher &events) : events_(events) {}

        void init();
        void start();
        void stop();
        void onTick();
        bool running() const { return running_; }
        bool phaseOn() const { return phase_on_; }

    private:
        static void timerCallback(void *arg);

        EventDispatcher &events_;
        dl::Timer timer_;
        bool running_ = false;
        bool phase_on_ = false;
    };

} 
