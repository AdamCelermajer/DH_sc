// Track B batch e1-glitch-scene-collada-gui-glitch_collada_detail_IColladaSkinTechnique-0:
// glitch::collada::detail::IColladaSkinTechnique (initProxyBuffer only).
// Declaration is in glitch-scene-collada-gui/include/glitch-scene-collada-gui/layouts/collada_members.hpp.

#include "glitch-scene-collada-gui/layouts/collada_members.hpp"

namespace glitch {
namespace collada {
namespace detail {

// 0x670ec8 glitch::collada::detail::IColladaSkinTechnique::initProxyBuffer(glitch::scene::CMeshBuffer*, glitch::collada::SSkinBuffer&, glitch::collada::SSkin&, glitch::video::IVideoDriver*)
//
// Partial: stub. The Hex-Rays body (1368 bytes of pseudocode) does the following:
//   - Reads the mesh buffer's CVertexStreams (meshBuffer+0x14) and its attribute mask (+0x04).
//   - When skinBuffer+0x00 (the proxy buffer holder) is empty, creates a 0x38-byte refcounted
//     object (vtable off_988098), fills it from skinBuffer+0x18..+0x2e, allocates its
//     CVertexStreams from the mask with flags 0x30040000, and stores it in skinBuffer+0x00.
//   - Sets skinBuffer+0x12 to the index of the proxy's stream entry (entry offset >> 4).
//   - Copies the masked stream entries of the mesh buffer's table into the proxy table, and
//     sets the table's stride and flags from the copy.
//   - Either takes a new refcounted object from the driver (vtable slot +0x78, out-pointer
//     convention) for skin+0x94, or keeps the existing one, depending on this+0x04 and the
//     shared holder at this+0x08 (refcount updates on both).
//   - Calls setStream on the proxy table twice, for two adjacent slots, both describing the
//     skin+0x94 object, with element size 4 * (skin+0x98 + 1).
//   - Returns the proxy object (new or existing).
//
// Not reconstructed, because each of these reads a layout this category does not declare:
//   - SSkinBuffer: the holder at +0x00, the index byte at +0x12 and the fields at +0x18..+0x2e.
//   - SSkin: the references at +0x80 and +0x94, and the byte at +0x98. Not declared.
//   - CVertexStreams: mask at +0x04, count at +0x0c, entries at +0x14, allocate, setStream and
//     the stride word. The glitch-video header is not on this category's include path, and the
//     deps/ stand-in has no members.
//   - IVideoDriver vtable slot +0x78 (the out-pointer call that returns a refcounted object):
//     not mapped.
//   - The 0x38-byte proxy object's class name (vtable off_988098): no class is declared.
// Until those land, the function returns nullptr and makes none of the side effects above
// (no holder replacement, no refcount change, no table copy, no setStream). Callers that
// need the proxy will see nullptr.
glitch::IReferenceCounted* IColladaSkinTechnique::initProxyBuffer(
    glitch::scene::CMeshBuffer* meshBuffer,
    glitch::collada::SSkinBuffer& skinBuffer,
    glitch::collada::SSkin& skin,
    glitch::video::IVideoDriver* driver) {
    (void)meshBuffer;
    (void)skinBuffer;
    (void)skin;
    (void)driver;
    return nullptr;
}

}  // namespace detail
}  // namespace collada
}  // namespace glitch
