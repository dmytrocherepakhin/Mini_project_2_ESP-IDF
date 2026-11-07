#include "bl/crash_controller.hpp"

namespace bl
{

    void CrashController::enterEmergency()
    {
        emergency_ = true;
        red_on_ = false;
    }
    void CrashController::exitEmergency() { emergency_ = false; }
    void CrashController::toggleRed() { red_on_ = !red_on_; }
    void CrashController::resetRed() { red_on_ = false; }
    bool CrashController::emergency() const { return emergency_; }
    bool CrashController::redOn() const { return red_on_; }

}