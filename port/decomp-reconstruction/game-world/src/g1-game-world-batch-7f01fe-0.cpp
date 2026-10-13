// Batch g1-game-world-batch-7f01fe-0: batch::BatchNodeCompiler, its segment callback
// (BatchNodeCompiler::SGameObjectLinkedCallback) and batch::GOBatchSceneNode.
// The owner headers are game/include/dh2/batch/BatchNodeCompiler.hpp and GOBatchSceneNode.hpp.
// Every function starts with its original address. Rows that are not complete are marked partial
// in game-world/ledger.d/g1-game-world-batch-7f01fe-0.csv with the missing part named.

#include "dh2/batch/BatchNodeCompiler.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

#include "dh2/GameObject.hpp"
#include "dh2/batch/GOBatchSceneNode.hpp"
#include "dh2/global/SceneNodeHelpers.hpp"
#include "glitch-scene-collada-gui/scene/CBatchMesh.hpp"
#include "glitch-scene-collada-gui/scene/CBatchSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

// core-platform/memory.hpp declares this. It is not included: its glitch_core stand-ins clash with glitch-shared.
extern int gAssertLevel;

namespace batch {

// 0x3ef180 batch::BatchNodeCompiler::SGameObjectLinkedCallback::~SGameObjectLinkedCallback()
// The complete destructor of the callback has an empty body (the original is a bare return).
BatchNodeCompiler::SGameObjectLinkedCallback::~SGameObjectLinkedCallback() {}

// 0x3ef394 batch::BatchNodeCompiler::SGameObjectLinkedCallback::~SGameObjectLinkedCallback() (deleting)
// skipped: the deleting destructor stores the vptr and calls CustomFree. The compiler generates it from the
// virtual destructor above (the same body plus operator delete), as for the other D0 destructors.

// 0x3efe58 batch::BatchNodeCompiler::SGameObjectLinkedCallback::operator()(glitch::scene::CBatchMesh *,void *,glitch::scene::SCompileInfo const&)
// PARTIAL. The original takes the rendered node from the scene manager (+0x04) and looks it up in the owner's
// m_nodeObjects (+0x0c). It then stores the GameObject (or 0) in the segment's slot and sets the visible byte of the
// object. The second argument is not a pointer in the binary: it is used as the segment index (r2 times the mesh's
// segment stride (+0x70 of CBatchMesh), plus 44). The third argument (SCompileInfo const&) is never read.
// Missing or not exact:
//  - getCurrentRenderedNode returns a 32-bit word in the engine. The original compares it with the 32-bit node keys.
//    The host keys are pointers, so the rendered word is converted to a pointer here (exact on a 32-bit host only).
//  - The slot is written as a host-width pointer at segment + 44. The 32-bit layout uses a 4-byte slot there, and
//    the segment stride is 44 in 32-bit terms. A write that does not fit the segment buffer is skipped here; the
//    original writes it unchecked.
//  - When no node is found, the original reports ASSERT(go != 0) and then writes the visible byte through the null
//    object pointer (a fault: dword_0 at level 2, or +0x2fc of null after the report at level 1). The write is not
//    repeated; the report is.
void BatchNodeCompiler::SGameObjectLinkedCallback::operator()(glitch::scene::CBatchMesh* mesh, void* segmentData,
                                                             const glitch::scene::SCompileInfo& /*info*/) {
    const std::uint32_t renderedWord = m_sceneManager->getCurrentRenderedNode(nullptr, nullptr);
    const auto* rendered = reinterpret_cast<const glitch::scene::ISceneNode*>(static_cast<std::uintptr_t>(renderedWord));

    // Lower bound of the key: found only when the key is equal to the rendered node.
    const auto& nodeObjects = m_compiler->m_nodeObjects;
    GameObject* object = nullptr;
    auto it = nodeObjects.lower_bound(rendered);
    if (it != nodeObjects.end() && !(rendered < it->first)) {
        object = it->second;
    }

    const auto index = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(segmentData));
    const std::size_t slot = static_cast<std::size_t>(mesh->segmentStride) * index + 44;
    if (slot + sizeof(object) <= mesh->segmentData.size()) {
        std::memcpy(mesh->segmentData.data() + slot, &object, sizeof(object));
    }

    if (object) {
        // GameObject +0x2fc (unk_2fc): the byte the original sets to 1 for a linked object.
        object->unk_2fc = 1;
        return;
    }
    // Not found: the original reports the failed assert. See the note above for the write it then makes.
    if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "go != 0",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources/Data/Collada/BatchNodeCompiler.h", 72);
    }
}

// 0x3f1868 batch::BatchNodeCompiler::Free()
void BatchNodeCompiler::Free() {
    // Drops the batch node through its IReferenceCounted base (the original uses the base at the vtable's offset).
    if (m_batchNode) {
        m_batchNode->drop();
    }
    m_batchNode = nullptr;
    m_gameObjects.clear();
    m_nodeObjects.clear();
}

// 0x3f1a48 batch::BatchNodeCompiler::~BatchNodeCompiler()
// Free, then the member destructors release the object list and the node map. The original frees the list storage
// with the node allocator for small blocks and CustomFree for large ones; std::vector does both.
BatchNodeCompiler::~BatchNodeCompiler() {
    Free();
}

// 0x3f5dcc batch::BatchNodeCompiler::_MapMeshNode(GameObject *,glitch::scene::ISceneNode *)
// Records object for node, then does the same for every child of node (the children hang off the child list at +0xf0).
void BatchNodeCompiler::_MapMeshNode(GameObject* object, glitch::scene::ISceneNode* node) {
    if (!node) {
        return;
    }
    m_nodeObjects[node] = object;
    const glitch::scene::SChildList& children = node->getChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        _MapMeshNode(object, *it);
    }
}

// 0x50cd44 batch::GOBatchSceneNode::isSegmentVisible(glitch::scene::CBatchMesh::SSegment const&)
// PARTIAL. The original reads the segment's index (SSegment +0x04), finds the GameObject stored in the segment slot
// (segment data + stride * index + 44) and returns false when the object hides it: GameObject::IsZonable (vtable
// +0xc4) with m_zoningEnabled (+0x2ee) and not m_inZone (+0x2f0), or the object byte at +0x80. Otherwise it returns the
// segment's visible byte (SSegment +0x20).
// Missing: CBatchMesh::SSegment declares no index (+0x04), and GameObject declares no byte at +0x80. Until both are
// declared, the object test cannot be reached, so this returns the visible byte. The original can hide a segment
// that this version shows.
bool GOBatchSceneNode::isSegmentVisible(const glitch::scene::CBatchMesh::SSegment& segment) {
    return segment.visible != 0;
}

// 0x50cf08 batch::GOBatchSceneNode::~GOBatchSceneNode() (complete)
// The original resets the vptrs of the bases and runs CBatchSceneNode's destructor. The compiler does the same for
// the empty body.
GOBatchSceneNode::~GOBatchSceneNode() {}

// 0x50cfa8 batch::BatchNodeCompiler::MakeNoBatchVisible(glitch::scene::ISceneNode *,bool)
// Sets the visibility of node and of its children when the node name contains "nobatch", and returns true when the
// node or one of its children matched.
bool BatchNodeCompiler::MakeNoBatchVisible(glitch::scene::ISceneNode* node, bool visible) {
    bool matched = false;
    const char* name = node->getName();
    if (std::strstr(name, "nobatch") != nullptr) {
        SetNodeVisibleRecur(node, visible);
        matched = true;
    }
    const glitch::scene::SChildList& children = node->getChildren();
    for (auto it = children.begin(); it != children.end(); ++it) {
        // The original calls the child first and then combines the result: every child is visited.
        const bool childMatched = MakeNoBatchVisible(*it, visible);
        matched = childMatched || matched;
    }
    return matched;
}

// 0x50d71c batch::GOBatchSceneNode::updateSegmentVisibility()
// PARTIAL. The original recomputes the visible segments for the current scene manager: it clears the visible
// records, loads the current level when the scene manager is missing, returns when the node is culled, and otherwise
// adds the visible segments for each batch with the intersector that the scene manager selects (universe, box,
// frustum box, frustum box 3). Then it stamps the tick of each batch record.
// Reconstructed here: the visibility gate, the clear, the scene manager check and the culling test.
// Missing:
//  - clearVisibleSegments returns the batch count in the original (Hex-Rays uses the result). The engine header
//    declares it void, so the batch count is not used below.
//  - The level load when the scene manager is missing (Application::LoadLevel with the temp_* arguments). The
//    function returns in that case.
//  - The intersector dispatch and the tick bookkeeping: addVisibleSegments<...> is not declared in the engine header.
void GOBatchSceneNode::updateSegmentVisibility() {
    if ((flags & glitch::scene::ESNF_VISIBLE) == 0) {
        return;
    }
    clearVisibleSegments();
    if (sceneManager == nullptr) {
        return;
    }
    if (sceneManager->isCulled(this)) {
        return;
    }
    // GAP: intersector dispatch and tick bookkeeping (see the note above).
}

// 0x50dabc batch::GOBatchSceneNode::~GOBatchSceneNode() (deleting)
// skipped: the deleting destructor is generated by the compiler from the virtual destructor above. The original also
// stores the secondary vptrs of its bases before it frees the object, which the compiler does too.

// 0x50dbbc batch::BatchNodeCompiler::Trim()
// Releases the object list storage (the vector is replaced by an empty one) and clears the node map.
void BatchNodeCompiler::Trim() {
    m_gameObjects.clear();
    std::vector<GameObject*>().swap(m_gameObjects);
    m_nodeObjects.clear();
}

// 0x50dc84 batch::BatchNodeCompiler::Compile(bool)
// PARTIAL. The original builds the batch node for the level and sets each object's batch state. Reconstructed here:
// Trim at the end, and the two object loops as far as their node type allows (see GAP G). Missing:
//  - GAP A: creating m_batchNode on first use. The original allocates a CBatchMesh with segment stride 48 and a
//    GOBatchSceneNode (operator new(360)), sets its refcount and vptrs, calls the CBatchSceneNode base constructor
//    (its arguments are not in the pseudocode) and sets the node's automatic culling to off. GOBatchSceneNode declares
//    no constructor, so the node is not created here and m_batchNode stays null.
//  - GAP B: the call on the scene manager's object (+0x04, vtable +0x5c) for each collected node.
//  - GAP C: the debug switch that the original reads for nobatch nodes. It can end loop 1 early when the value is
//    long. DebugSwitches::GetSwitch is declared to return bool, but the original uses it to fill a string.
//  - GAP D: the batch build (the scene manager's vtable +0x50 with the collected nodes and the linked callback),
//    the quantize step (when quantize is set and the driver allows it), FlushMeshBuffers and m_compiled = 1.
//  - GAP G: both loops need each visual's node as glitch::scene::ISceneNode. VisualObject::m_node is RootSceneNode*,
//    which is not an ISceneNode here, so the loops are not run (see the body).
//  - GAP F: loop 2's name test. GameObject +0x5c (the class name compared with "Decor" and "Module") is not declared,
//    so the objects that lose their visual object (SetVisualObject(0)) are not changed here.
void BatchNodeCompiler::Compile(bool quantize) {
    (void)quantize;  // used by GAP D

    // GAP A (see the note above).

    // GAP G: both loops need each object's visual node as a glitch::scene::ISceneNode. VisualObject::m_node is declared
    // RootSceneNode*, and RootSceneNode derives from glitch::collada::CRootSceneNode, which is not an ISceneNode in this
    // tree, so the conversion does not exist and the loops are not run. The intended loops:
    //  loop 1: for each object with a visual and a node, MakeNoBatchVisible(node, false) (GAP B and GAP C around it),
    //          then the node is added to the batch list in every case;
    //  GAP D:  build the batch from that list;
    //  loop 2: for each object with a visual and a node, MakeNoBatchVisible(node, true); when that returns false the
    //          name test of GAP F follows.

    Trim();
}

}  // namespace batch
