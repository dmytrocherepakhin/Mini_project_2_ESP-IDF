#include "bl/hazard_controller.hpp"

namespace bl
{

    void HazardController::toggle() { on_ = !on_; }
    void HazardController::forceOn() { on_ = true; }
    void HazardController::reset() { on_ = false; }
    bool HazardController::on() const { return on_; }

}