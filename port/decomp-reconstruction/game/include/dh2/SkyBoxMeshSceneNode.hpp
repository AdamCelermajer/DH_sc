// SkyBoxMeshSceneNode: the sky dome mesh node. Its render is gated by a flag that the ctor sets, and it
// keeps a counted reference to the IMeshSceneNode it was built from. getType returns a fixed tag.
// Original 32-bit size 0x148 (328), inferred from the virtual base IReferenceCounted at +0x140 (8 bytes).
// No operator new site is recorded in class-facts.
// Reconstructed from libDungeonHunter2.so pseudocode (ctors 0x362810 / 0x36299c, dtors 0x362718 / 0x36278c /
// 0x3627a8, methods 0x36233c, 0x362398, 0x3625d0).
// The base is the accessor stub glitch::collada::CMeshSceneNode, which has no vtable. Virtual slots of
// the original are noted but not marked override, so this header compiles against the stub.
#pragma once

#include <cstdint>

// The accessor header brings its own boost::intrusive_ptr (deps stand-in); the counting form cannot be included beside it.
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch {
namespace collada {
class IMesh;
}  // namespace collada
namespace scene {
class IMeshSceneNode;
}  // namespace scene
}  // namespace glitch

class SkyBoxMeshSceneNode : public glitch::collada::CMeshSceneNode {
public:
    // Ctor. The mesh goes to the CMeshSceneNode base; the node is grabbed with a reference count +1.
    SkyBoxMeshSceneNode(const boost::intrusive_ptr<glitch::collada::IMesh>& mesh,
                        glitch::scene::IMeshSceneNode* node);           // 0x362810 (complete), 0x36299c (base-object form)
    virtual ~SkyBoxMeshSceneNode();                                     // 0x362718 (complete), 0x36278c (deleting), 0x3627a8 (VTT form)

    // Virtual slots of the original, in vtable order. Not override here (see header note).
    int getType() const;                                                // slot 0xc8: 0x36233c returns 1601792883
    void render(void* context);                                         // slot 0x28: 0x362398, tests m_renderEnabled first
    int onRegisterSceneNode();                                          // slot 0x1c: 0x3625d0 returns 1

    // CMeshSceneNode occupies +0x00 .. +0x137 (the stub maps uidHolder at +0x130 and namedMesh at +0x134).
    bool m_renderEnabled;                                               // +0x138 render gate: the ctor sets it to 1, render returns early when clear
    glitch::scene::IMeshSceneNode* unk_13c;                             // +0x13c the node passed to the ctor; grabbed by the ctor, dropped by the dtor
    uint8_t pad_140[8];                                                 // +0x140 virtual base IReferenceCounted: vptr at +0x140, refcount at +0x144 (1 from the complete ctor)
};
