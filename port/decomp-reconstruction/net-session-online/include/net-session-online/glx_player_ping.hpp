// GLXPlayerPing: the ping probe of the GLX player services. Prepare starts a probe and
// Finish ends it. Only Finish is reconstructed. Prepare needs Init (0x84008c, queued).
//
// Layout (original 32-bit offsets, kept as comments): +0x24 probe state (int); 1 when
// finished, 2 when prepared.
#pragma once

#include <cstdint>

class GLXPlayerPing {
public:
    // 0x8400ec _ZN13GLXPlayerPing6FinishEv: sets the state to 1 and returns 0.
    std::int32_t Finish() {
        state_ = 1;
        return 0;
    }

    // Not in the original: test seam.
    std::int32_t StateForTest() const { return state_; }

private:
    std::int32_t state_ = 0;  // +0x24
};
