#pragma once

namespace bl
{

    class TurnController
    {
    public:
        void toggleLeft();
        void toggleRight();
        void reset();
        bool left() const;
        bool right() const;
        bool any() const;

    private:
        bool left_ = false;
        bool right_ = false;
    };

}
