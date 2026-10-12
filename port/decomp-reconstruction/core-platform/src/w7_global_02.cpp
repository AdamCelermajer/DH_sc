// global_02 slice: reconstructed functions. Rows without a body here are
// recorded in the ledger as skipped or partial with the reason.

#include "core-platform/w7_global_02.hpp"

// 0x50f778 _Z18disableCullingModePN6glitch5scene10ISceneNodeE
// Thunk: a single tail branch to setCullingMode(node, 0).
int disableCullingMode(glitch::scene::ISceneNode* node) {
    return setCullingMode(node, 0);
}

// 0x52f0e8 getState
// Returns the constant 1.
extern "C" int getState() {
    return 1;
}
