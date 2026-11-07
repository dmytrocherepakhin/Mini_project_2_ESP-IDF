#pragma once

namespace bl
{

    class HazardController
    {
    public:
        void toggle();
        void forceOn();
        void reset();
        bool on() const;

    private:
        bool on_ = false;
    };

}