// UnderlaySceneNode: a scene node that keeps a list of underlay scene nodes and renders their hierarchies
// through the level's render path. render clears the level's render state around the underlay nodes,
// onRegisterSceneNode submits one material to the scene manager unless the IsUsingToggleDisplayMode or
// IsUsingCharacterShowRoom debug switch is set, and getBoundingBox is not supported (it asserts).
// Original 32-bit size not recorded (no operator new site and no ctor in class-facts or the queue).
// The base ISceneNode occupies +0x00 .. +0x12f. The underlay vector starts at +0x130. The second base
// subobject starts at +0x13c and is not identified (see BaseMeshSceneNode.hpp for the same subobject).
// Reconstructed from libDungeonHunter2.so pseudocode (dtors 0x381ad4 / 0x381b54, methods 0x326fcc,
// 0x381b90, 0x381cf0, 0x381e48).
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-shared/glitch_core.hpp"
#include "glitch-scene-collada-gui/layouts/scene.hpp"

class UnderlaySceneNode : public glitch::scene::ISceneNode {
public:
    // Frees the underlay list and resets both vptrs, then runs ~ISceneNode. Vtable slot 0x14 (complete, 0x381ad4).
    virtual ~UnderlaySceneNode();                                       // slot 0x14: 0x381ad4 (complete), 0x381b54 (deleting)

    // Virtual slots of the original, in vtable order. The ISceneNode base is a layout stub with no vtable,
    // so these are not marked override (same convention as SkyBoxMeshSceneNode.hpp).
    int onRegisterSceneNode();                                          // slot 0x1c: 0x381cf0, returns 1 when a material was submitted
    void render(void* renderContext);                                   // slot 0x28: 0x381b90, renders each underlay hierarchy
    const glitch::core::aabbox3d<float>& getBoundingBox() const;        // slot 0x3c: 0x381e48, asserts "not supported" and returns null

    // Adds node to the underlay list unless it is already there, then calls the node's setVisible(false) (slot 0x54 in
    // the decomp_vtable labels; the Hex-Rays call site shows it at byte offset 0x48). The Hex-Rays signature returns a
    // pointer, which looks like register residue, so void here.
    void AddUnderlayNode(glitch::scene::ISceneNode* node);              // 0x326fcc

    // ISceneNode occupies +0x00 .. +0x12f.
    std::vector<glitch::scene::ISceneNode*> m_underlayNodes;            // +0x130 begin, +0x134 end, +0x138 end of storage
    void* unk_13c;                                                      // +0x13c vptr of the unidentified second base subobject
};
