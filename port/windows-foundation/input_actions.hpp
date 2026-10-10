#pragma once

namespace dh::foundation {
struct InputMove2D { float x = 0; float y = 0; };
// Device-independent intent. Positive x is camera right, positive y forward.
// Buttons describe this frame's request; the host decides edge versus held input.
struct InputActions {
    InputMove2D move2D{};
    bool attack = false;
    bool targetSelect = false;
    bool interact = false;
    bool run = false;
};
}
