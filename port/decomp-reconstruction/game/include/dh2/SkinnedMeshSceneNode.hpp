// SkinnedMeshSceneNode: the skinned mesh node. It is BaseMeshSceneNode over glitch's CSkinnedMeshSceneNode.
// Its own part is a non-virtual base subobject at +0x180 (only its vptr, which points at a vtable with the
// virtual prepareSkinForRendering) and the virtual base IReferenceCounted at +0x184 with refcount 1 at +0x188.
// Original 32-bit size 0x18c (396), inferred from the virtual base; no operator new site is recorded.
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x35b0a0 / 0x35b128, dtors 0x35b860 / 0x35b8f4 /
// 0x35b910, methods 0x35a914, 0x35a974, 0x35ad8c).
#pragma once

#include <cstdint>

#include "dh2/BaseMeshSceneNode.hpp"

namespace glitch {
namespace collada {
class CSkinnedMeshSceneNode;
class IMesh;
}  // namespace collada
}  // namespace glitch

class SkinnedMeshSceneNode : public BaseMeshSceneNode<glitch::collada::CSkinnedMeshSceneNode> {
public:
    explicit SkinnedMeshSceneNode(const boost::intrusive_ptr<glitch::collada::IMesh>& mesh);  // 0x35b0a0 (complete), 0x35b128 (base-object form)
    ~SkinnedMeshSceneNode() override;                                   // 0x35b860 (base-object dtor), 0x35b8f4 (deleting), 0x35b910 (VTT form)

    // Overrides of the base slots, in vtable order.
    int onRegisterSceneNode() override;                                 // slot 0x1c: 0x35a914, notifies the owner at +0x110 with this+0x180, then the base
    void updateAbsolutePosition(bool includeChildren) override;         // slot 0xc4: 0x35a974, updates the skin at +0x134 first

    // Virtual of the +0x180 subobject (slot 0x108 in this vtable, thunk at 0x114). Declared here, not in the base.
    virtual void prepareSkinForRendering();                             // 0x35ad8c, walks the skin's materials and sets each skinned material

    // BaseMeshSceneNode occupies +0x00 .. +0x17f: render flag at +0x17c.
    uint8_t pad_180[4];                                                 // +0x180 vptr of a non-virtual base subobject, no data; prepareSkinForRendering is its virtual
    uint8_t pad_184[8];                                                 // +0x184 virtual base IReferenceCounted: vptr at +0x184, refcount at +0x188 (1 from the complete ctor)
};
