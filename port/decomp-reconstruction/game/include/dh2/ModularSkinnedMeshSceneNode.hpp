// ModularSkinnedMeshSceneNode: a modular skinned mesh node. It is BaseMeshSceneNode over glitch's
// CModularSkinnedMeshSceneNode and adds a second base subobject (not identified) at +0x184 and a flag word at +0x188
// set to 1 by the ctor. The base's render flag is at +0x17c and its virtual subobject at +0x180.
// Original 32-bit size not recovered (no operator new site in class-facts).
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x35af00 / 0x35af88, dtors 0x35ba4c / 0x35baa0 / 0x35babc,
// methods 0x35a8e8, 0x35a940, 0x35acb4).
#pragma once

#include <cstdint>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

#include "dh2/BaseMeshSceneNode.hpp"

namespace glitch {
namespace collada {
class IMesh;
}  // namespace collada
}  // namespace glitch

class ModularSkinnedMeshSceneNode : public BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode> {
public:
    explicit ModularSkinnedMeshSceneNode(const boost::intrusive_ptr<glitch::collada::IMesh>& mesh);  // 0x35af00 (complete), 0x35af88 (base-object form)
    ~ModularSkinnedMeshSceneNode() override;                                                         // 0x35ba4c (base-object dtor), 0x35baa0 (deleting), 0x35babc (VTT form)

    // Virtual methods in vtable slot order, after the destructors. The engine's ISceneNode declares the
    // first two, but the modular stand-in does not derive from it, so no override keyword.
    virtual int onRegisterSceneNode();                                 // 0x35a8e8: notifies the skin owner with this+0x180, then CMeshSceneNode::onRegisterSceneNode
    virtual void updateAbsolutePosition(bool includeChildren);         // 0x35a940: updates the owner pointer at +0x134, then ISceneNode::updateAbsolutePosition
    virtual void prepareSkinForRendering();                            // 0x35acb4: loops over the owner at +0x134 and calls its material virtuals (partial)

    // BaseMeshSceneNode occupies +0x00 .. +0x17f: render flag at +0x17c, its virtual subobject at +0x180.
    void* unk_184;                                                     // +0x184 vptr of a second base subobject, not identified
    std::int32_t unk_188;                                              // +0x188 set to 1 by the ctor; not read in the recovered code
};
