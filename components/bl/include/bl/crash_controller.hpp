#pragma once

namespace bl
{

    class CrashController
    {
    public:
        void enterEmergency();
        void exitEmergency();
        void toggleRed();
        void resetRed();
        bool emergency() const;
        bool redOn() const;

    private:
        bool emergency_ = false;
        bool red_on_ = false;
    };

}
