#include "bl/turn_controller.hpp"

namespace bl
{

    void TurnController::toggleLeft()
    {
        left_ = !left_;
        right_ = false;
    }
    void TurnController::toggleRight()
    {
        right_ = !right_;
        left_ = false;
    }
    void TurnController::reset()
    {
        left_ = false;
        right_ = false;
    }
    bool TurnController::left() const { return left_; }
    bool TurnController::right() const { return right_; }
    bool TurnController::any() const { return left_ || right_; }

}