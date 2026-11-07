#pragma once

namespace bl
{

    class DoorController
    {
    public:
        void toggle();
        void forceOn();
        bool on() const;

    private:
        bool on_ = false;
    };

}
