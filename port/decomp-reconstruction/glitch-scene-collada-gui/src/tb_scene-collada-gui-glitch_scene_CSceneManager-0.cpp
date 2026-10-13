// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CSceneManager-0: glitch::scene::CSceneManager.
// All four functions are partial. Each note names the callee, table or global that is not mapped yet.
//
// Vtable offsets in the notes are the Hex-Rays byte offsets. For CSceneManager the listing offset
// (tools/decomp_vtable.py) is the Hex-Rays offset plus 0x0c, so Hex-Rays +0x40 is listing +0x4c.
#include "glitch-scene-collada-gui/scene_nodes.hpp"

#include <vector>

namespace glitch::scene {

// Frustum-box half of CSceneManager::isCulled, case 1 (box mode). The original tests the node box
// (node vtable +0x34, listing +0x40 getTransformedBoundingBox) against the view frustum box (min at
// +0x6c, max at +0x78). Returns true when the node box is outside the frustum box (culled).
bool CSceneManager_isBoxCulledByFrustumBox(const core::aabbox3d<float>& box,
                                           const core::aabbox3d<float>& frustumBox) {
    if (box.MinEdge.X > frustumBox.MaxEdge.X) return true;   // v12 > f[30]
    if (box.MinEdge.Y > frustumBox.MaxEdge.Y) return true;   // v14 > f[31]
    if (box.MinEdge.Z > frustumBox.MaxEdge.Z) return true;   // v15 > f[32]
    if (box.MaxEdge.X < frustumBox.MinEdge.X) return true;   // v16 < f[27]
    if (box.MaxEdge.Y < frustumBox.MinEdge.Y) return true;   // v17 < f[28]
    return !(box.MaxEdge.Z >= frustumBox.MinEdge.Z);         // v13 >= f[29] is visible; the original returns !v19
}

// 0x58ab28 _ZNK6glitch5scene13CSceneManager8isCulledEPKNS0_10ISceneNodeE
// PARTIAL. The original (1922 bytes):
//   returns 0 when the culling enable byte (+0x250) is clear or the camera pointer (+0xe4) is null;
//   camera vtable +0x144 (listing +0x150, CCameraSceneNode::getViewFrustum) gives the frustum;
//   the node's culling mode (+0x118) selects the test, on the node's transformed box (node vtable +0x34, listing
//   +0x40, ISceneNode::getTransformedBoundingBox):
//     mode 1 (box): CSceneManager_isBoxCulledByFrustumBox against the frustum's box;
//     mode 2: !SViewFrustum::intersects(box), 0x35bec8;
//     mode 8: !SViewFrustum::intersects3(box), 0x58aa8c;
//     any other mode: returns 0.
// The enable byte, the active camera, the camera's getViewFrustum and the three frustum tests are mapped now.
// Missing: the node box is a virtual call in the original. ISceneNode has no vptr in this category, so the base
// ISceneNode::getTransformedBoundingBox is called directly (it is itself partial: its dirty-box branch, which
// asks the node for its local box and transforms it, is not reconstructed), and a node class that overrides
// the slot would be bypassed.
bool CSceneManager::isCulled(const ISceneNode* node) const {
    if (!cullingEnabled || activeCamera == nullptr) {  // +0x250, +0xe4
        return false;
    }
    const SViewFrustum* frustum = activeCamera->getViewFrustum();
    switch (node->automaticCulling) {  // +0x118
        case 2:
            return !frustum->intersects(node->getTransformedBoundingBox());
        case 8:
            return !frustum->intersects3(node->getTransformedBoundingBox());
        case 1:
            return CSceneManager_isBoxCulledByFrustumBox(node->getTransformedBoundingBox(), frustum->box);
        default:
            return false;
    }
}

// 0x58b728 _ZN6glitch5scene13CSceneManager7drawAllERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
// PARTIAL. The original calls, in order, on this (Hex-Rays offsets):
//   +0x40 drawInit(driver = word +0x14), collectAllNodes(this, nodes) (0x58b6c8),
//   +0x44 setupCamera(), +0x2c registerSceneNodes(nodes), +0x4c drawShadowReceivers(),
//   +0x48 renderLists(driver), then sets the render pass word (+0x174) to 9, then returns
//   the result of anonymous-namespace SStats::reset(this + 0x114, ...).
// Missing: the virtual slots are not declared on CSceneManager, the driver word (+0x14) is not
// mapped, the collectAllNodes overload is only a stand-in in another slice, and SStats is not
// reconstructed. This body writes only the render pass store and leaves the calls out.
void CSceneManager::drawAll(const std::vector<ISceneNode*>& /*nodes*/) {
    renderPass = 9;  // +0x174
}

// 0x58b7f4 _ZN6glitch5scene13CSceneManager7drawAllEPNS0_10ISceneNodeE
// PARTIAL. The original calls, in order, on this (Hex-Rays offsets):
//   +0x40 drawInit(driver = word +0x18), if (!node && hierarchyChanged (+0x288)) collectAllNodes(this),
//   +0x44 setupCamera(), +0x28 registerSceneNodes(node), +0x4c drawShadowReceivers(),
//   +0x48 renderLists(driver), then sets the render pass word (+0x174) to 9, then returns
//   the result of anonymous-namespace SStats::reset(this + 0x114, ...).
// Missing: the same virtual slots, the driver word (+0x18), the collectAllNodes stand-in and SStats.
// The hierarchyChanged test and the render pass store are mapped; the calls are left out.
void CSceneManager::drawAll(ISceneNode* /*node*/) {
    renderPass = 9;  // +0x174
}

// 0x58f348 _ZN6glitch5scene13CSceneManager24registerNodeForRenderingEPNS0_10ISceneNodeERKN5boost13intrusive_ptrINS_5video9CMaterialEEEPvNS0_24E_SCENE_NODE_RENDER_PASSEPKNS_4core8vector3dIfEEi
// PARTIAL. The 1596-byte original is a switch on the render pass (argument 5). Mapped parts:
//   the render pass cases append entries to vectors held in this (unsorted at +0x30 and +0x3c,
//   distance at +0x48, default at +0x78, transparent at +0x84, render-data sort at +0x54 and +0x64),
//   the distance entry is computed against the vector at this+0xe8 (its meaning is not identified),
//   pass 3 takes the transparent branch when the material technique has the transparent bit and the
//   flag byte at +0x28a is clear, pass 8 takes the default branch when that flag is set and the
//   transparent branch otherwise, and every handled case bumps the anonymous-namespace counter `Stats`.
//   The default branch bumps dword_9F687C and Stats and returns 0.
// Missing: the vector element types (SUnsortedNodeEntry, SRenderDataSortNodeEntry) are not declared,
// the sort order fallback reads CMaterial vtable +0xd8 (decomp +216), which is not declared, the
// STransparentNodeEntry construction is only partly declared, and the globals Stats and dword_9F687C
// are not mapped. Until then this body returns false and does not register the node.
bool CSceneManager::registerNodeForRendering(ISceneNode* /*node*/,
                                             const boost::intrusive_ptr<video::CMaterial>& /*material*/,
                                             void* /*userData*/, std::uint32_t /*pass*/,
                                             const core::vector3d<float>* /*position*/, int /*sortOrder*/) {
    return false;
}

}  // namespace glitch::scene
