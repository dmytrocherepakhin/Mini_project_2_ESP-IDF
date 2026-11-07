#include "bl/door_controller.hpp"

namespace bl
{

    void DoorController::toggle() { on_ = !on_; }
    void DoorController::forceOn() { on_ = true; }
    bool DoorController::on() const { return on_; }

}
