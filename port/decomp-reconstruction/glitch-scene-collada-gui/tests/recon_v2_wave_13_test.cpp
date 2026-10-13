// Wave 13 test: the two empty bodies must be callable and leave their
// arguments untouched. No externs are needed, so no test doubles are used.
#include <cassert>
#include <cstdint>

#include "glitch-scene-collada-gui/recon/v2/wave_13.hpp"

namespace rc = glitch::collada::recon_v2_13;

int main() {
    rc::CGUIModalScreen_serializeAttributes(nullptr, nullptr, nullptr);
    rc::CGUIModalScreen_deserializeAttributes(nullptr, nullptr, nullptr);
    return 0;
}
