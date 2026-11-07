#include "cl/blink_clock.hpp"
#include "config.hpp"

namespace cl
{

    void BlinkClock::init()
    {
        timer_.init(&BlinkClock::timerCallback, this, "blink_500ms");
    }

    void BlinkClock::timerCallback(void *arg)
    {
        auto *self = static_cast<BlinkClock *>(arg);
        // Непряме використання FreeRTOS: обробник esp_timer надсилає подію в чергу із задачі.
        self->events_.post({EventType::BlinkTick, self});
    }

    void BlinkClock::start()
    {
        if (running_)
            return;
        phase_on_ = true;
        running_ = true;
        timer_.startPeriodic(config::timing::BLINK_HALF_PERIOD_MS);
    }

    void BlinkClock::stop()
    {
        if (!running_)
            return;
        timer_.stop();
        running_ = false;
        phase_on_ = false;
    }

    void BlinkClock::onTick()
    {
        if (running_)
            phase_on_ = !phase_on_;
    }

} 
