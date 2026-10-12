// Wave 13 test: the three empty bodies must be callable and leave their
// arguments untouched. No externs are needed, so no test doubles are used.
#include <cassert>
#include <cstdint>

#include "glitch-scene-collada-gui/recon/v2/wave_13.hpp"

namespace rc = glitch::collada::recon_v2_13;

int main() {
    // Only the addresses are passed; the bodies must not dereference them.
    glitch::scene::CBatchMesh mesh{};
    std::uint32_t sentinel = 0xdeadbeefu;
    rc::CBatchMesh_loadSegmentExtraData(&mesh, &sentinel, nullptr, true);
    assert(sentinel == 0xdeadbeefu);
    assert(mesh.referenceCount == 0);

    rc::CGUIModalScreen_serializeAttributes(nullptr, nullptr, nullptr);
    rc::CGUIModalScreen_deserializeAttributes(nullptr, nullptr, nullptr);

    static_assert(offsetof(glitch::scene::CBatchMesh, batchesBegin) == 0x20, "layout");
    return 0;
}
