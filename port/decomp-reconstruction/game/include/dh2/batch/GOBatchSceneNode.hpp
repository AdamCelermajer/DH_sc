// batch::GOBatchSceneNode: the batch scene node that BatchNodeCompiler builds for a level. It is a glitch
// CBatchSceneNode. updateSegmentVisibility recomputes which segments are visible for the scene manager's
// current intersector, and isSegmentVisible answers from the segment's GameObject.
// Original 32-bit size 0x168 (360), from the operator new site in BatchNodeCompiler::Compile. The first 0x160
// bytes are the CBatchSceneNode base (layouts/scene.hpp gives it as an empty stub; scene_evidence.hpp maps its members).
// Reconstructed from libDungeonHunter2.so pseudocode (dtors 0x50cf08 / 0x50dabc, isSegmentVisible 0x50cd44,
// updateSegmentVisibility 0x50d71c).
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace batch {

class GOBatchSceneNode : public glitch::scene::CBatchSceneNode {
public:
    // Complete dtor (slot 0x14, 0x50cf08) and deleting dtor (slot 0x18, 0x50dabc, which also frees the object).
    virtual ~GOBatchSceneNode();

    // Slot 0x114 of the vtable. Returns true when the segment's GameObject passes the visibility test. 0x50cd44.
    virtual bool isSegmentVisible(const glitch::scene::CBatchMesh::SSegment& segment);

    // Recomputes the visible segments for the current scene manager when the update flag at CBatchSceneNode +0x11c
    // is set. The original returns the this pointer as residue, so void here. 0x50d71c.
    void updateSegmentVisibility();

    // CBatchSceneNode occupies +0x00 .. +0x15f in the original.
    void* unk_160;                                                      // +0x160 vptr of the secondary subobject (off_96BF74 in the complete dtor, off_95B720 in the deleting dtor)
    int unk_164;                                                        // +0x164 set to 1 by the Compile new site before the ctor runs
};

}  // namespace batch
