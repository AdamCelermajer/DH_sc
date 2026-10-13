// ShadowModularSkinnedMeshSceneNode: the shadow-projection copy of a modular skinned mesh. It is
// BaseMeshSceneNode over glitch's CModularSkinnedMeshSceneNode. It keeps six vectors (two of 4-byte values, two
// of CMaterial and two of CMaterialVertexAttributeMap, indexed per shadow entry by regenerateShadowMaterial), and
// two ISceneNode pointers from the ctor. Bounds come from the first node.
// Original 32-bit size 0x1d8 (472), from operator new(472) in class-facts. The virtual base
// IReferenceCounted at +0x1d0 ends the object.
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x3619c8 / 0x361ab0, dtors 0x3601fc / 0x3602cc /
// 0x360aa8, methods 0x35dcd0, 0x35dcec, 0x35f348, 0x35f530, 0x3602e8, 0x3604a8, 0x361298).
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-shared/glitch_core.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

#include "dh2/BaseMeshSceneNode.hpp"

namespace glitch {
namespace collada {
class CModularSkinnedMeshSceneNode;
class IMesh;
}  // namespace collada
namespace scene {
class ISceneNode;
}  // namespace scene
namespace video {
class CMaterial;
class CMaterialVertexAttributeMap;
}  // namespace video
}  // namespace glitch

class ShadowModularSkinnedMeshSceneNode : public BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode> {
public:
    ShadowModularSkinnedMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> mesh,
                                      const glitch::scene::ISceneNode* node1,
                                      const glitch::scene::ISceneNode* node2);  // 0x3619c8 (complete), 0x361ab0 (VTT form)
    ~ShadowModularSkinnedMeshSceneNode() override;                      // 0x3601fc (base-object dtor), 0x3602cc (deleting), 0x360aa8 (VTT form)

    // Overrides of the base slots, in vtable order. The engine base CModularSkinnedMeshSceneNode (collada_scene_accessors.hpp)
    // declares none of the three ISceneNode slots below, so they are plain virtuals here and carry no override keyword
    // (same as ModularSkinnedMeshSceneNode.hpp). BaseMeshSceneNode declares render, so render keeps override.
    virtual const glitch::core::aabbox3d<float>& getBoundingBox() const;                  // slot 0x3c (ISceneNode K 0x30): 0x35dcd0, forwards to node1 (+0x1c8)
    virtual const glitch::core::aabbox3d<float>& getTransformedBoundingBox() const;       // slot 0x40 (ISceneNode K 0x34): 0x35dcec, forwards to node1 (+0x1c8, vptr+0x30)
    virtual int onRegisterSceneNode();                                  // slot 0x1c (ISceneNode K 0x10): 0x35f530, returns 1
    void render(void* context) override;                                // slot 0x28: 0x3604a8

    // Methods not in the vtable.
    void UpdateShadow();                                                // 0x35f348, reads node2 at +0x1cc, then calls the slot at 0x3c with a second argument
    void renderMesh(void* context);                                     // 0x3602e8
    void regenerateShadowMaterial();                                    // 0x361298, rebuilds the material and vertex-map vectors at +0x180 .. +0x1bc

    // Own part of the object, from +0x180 to +0x1cf. BaseMeshSceneNode occupies +0x00 .. +0x17f (render flag at +0x17c).
    // The six vectors are contiguous: 12 bytes each, begin, end and capacity words.
    std::vector<std::uint32_t> unk_180;                                                   // +0x180 4-byte entries written with loop indices by regenerate, trivially destructible
    std::vector<std::uint32_t> unk_18c;                                                   // +0x18c 4-byte entries, trivially destructible
    std::vector<boost::intrusive_ptr<glitch::video::CMaterial>> unk_198;                  // +0x198 one shadow material per module entry
    // +0x1a4 and +0x1bc hold CMaterialVertexAttributeMap handles. They are raw pointers here because glitch-core-io's
    // stand-ins (deps/glitch_core_objects.hpp) define no counting intrusive_ptr_add_ref/release for that class, so an
    // intrusive_ptr element would not compile in the destructor. The counts are not taken or released (see the batch notes).
    std::vector<glitch::video::CMaterialVertexAttributeMap*> unk_1a4;                     // +0x1a4 one vertex map per module entry (raw, not counted)
    std::vector<boost::intrusive_ptr<glitch::video::CMaterial>> unk_1b0;                  // +0x1b0 one material per module entry, flag byte +8 set to 3 by regenerate
    std::vector<glitch::video::CMaterialVertexAttributeMap*> unk_1bc;                     // +0x1bc one vertex map per module entry (raw, not counted)
    const glitch::scene::ISceneNode* unk_1c8;                                             // +0x1c8 ctor argument node1, the node whose bounds are forwarded
    const glitch::scene::ISceneNode* unk_1cc;                                             // +0x1cc ctor argument node2, read by UpdateShadow
    uint8_t pad_1d0[8];                                                                   // +0x1d0 virtual base IReferenceCounted: vptr at +0x1d0, refcount at +0x1d4 (1 from the complete ctor)
};
