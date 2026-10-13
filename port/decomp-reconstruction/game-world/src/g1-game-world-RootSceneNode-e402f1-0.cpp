// Batch g1-game-world-RootSceneNode-e402f1-0 (part 1 of 2): the debug counters, the animation roots and the displacement,
// the cull gate, the bounding box, the animator hooks, NewAnim and the destructor. The constructors are in
// g1-game-world-RootSceneNode-e402f1-1 (part 2).
//
// Most bodies are partial for one reason. dh2/RootSceneNode.hpp derives from glitch::collada::CRootSceneNode, a stand-in
// with no ISceneNode base. In the binary the ISceneNode part sits at +0x00 of the object: the animators list (+0xfc), the
// children list (+0xf4), the flags (+0x11c), the transform (+0xac .. +0xd4), and the virtual calls on this node. None of
// them is reachable from this class yet (layout request in the ledger). A partial body keeps the parts that use only this
// class's own members and names the missing call in a comment.
//
// The three debug counters are defined here, since nothing else in the tree defines them. The camera statics
// s_enlargedCamera and s_frustumScale are not defined here (see UpdateEnlargedViewFrustum).

#include <cstdint>

#include "dh2/DebugSwitches.hpp"
#include "dh2/RootSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CEmptySceneNode.hpp"  // the displacement node is dropped by the destructor

// Debug counters, bumped by updateAbsolutePosition and onAnimate and cleared by ResetDebugCounters.
std::uint32_t RootSceneNode::s_dbgNumUpdateAbsolutePosition = 0;
std::uint32_t RootSceneNode::s_dbgAnimatedNodeCount = 0;
std::uint32_t RootSceneNode::s_dbgUpdateTimeNodeCount = 0;

// 0x35c058 _ZN13RootSceneNode18ResetDebugCountersEv
void RootSceneNode::ResetDebugCounters() {
    s_dbgNumUpdateAbsolutePosition = 0;
    s_dbgAnimatedNodeCount = 0;
    s_dbgUpdateTimeNodeCount = 0;
}

// 0x35c09c _ZN13RootSceneNode11ForwardAnimEj
// partial: the original walks the animators list (ISceneNode +0xfc, not reachable from this class). For each animator it
// calls vptr+0x44 (getTimelineCtrl) and, when the controller is not null, vptr+0x0c on it with (time + the controller's
// +0x04 word). The walk is missing, so nothing is forwarded.
void RootSceneNode::ForwardAnim(std::uint32_t time) {
    (void)time;
}

// 0x35c0f4 _ZN13RootSceneNode12onUpdateTimeEj
// partial: the gate is ISceneNode flags (+0x11c) & 0x201 == 0x201. When it holds, the original calls vptr+0x14
// (ISceneNodeAnimator::updateTime) with the time on each animator of the list at +0xfc. Both the flags and the list are
// ISceneNode members (base missing), so the body is empty.
void RootSceneNode::onUpdateTime(std::uint32_t time) {
    (void)time;
}

// 0x35c268 _ZN13RootSceneNode14PlainOnAnimateEj
// partial: a thunk to glitch::collada::CRootSceneNode::onAnimate, the engine base's body. The stand-in base declares no
// onAnimate, so the forwarding call cannot be written yet (engine layout request).
void RootSceneNode::PlainOnAnimate(std::uint32_t time) {
    (void)time;
}

// 0x35c26c _ZN13RootSceneNode10setVisibleEb
// partial: stores the request in m_visibleRequested (+0x209), which onAnimate applies and clears. When the request is
// false the original also calls glitch::scene::ISceneNode::setVisible(this, 0), which needs the ISceneNode base. That call
// is missing.
void RootSceneNode::setVisible(bool visible) {
    m_visibleRequested = visible;
}

// 0x35c27c _ZN13RootSceneNode22updateAbsolutePositionEb
// partial: counts the call, then the original returns glitch::scene::ISceneNode::updateAbsolutePosition(this, recurse).
// That forward needs the ISceneNode base and is missing.
void RootSceneNode::updateAbsolutePosition(bool recurse) {
    ++s_dbgNumUpdateAbsolutePosition;
    (void)recurse;
}

// 0x35c51c _ZN13RootSceneNode25UpdateEnlargedViewFrustumEPN6glitch5scene13CSceneManagerE
// partial: the view is copied from the level camera into s_enlargedCamera. The camera is the pointer at this+0xe4
// (ICameraSceneNode*). That offset lies inside the base object, so it is not declared here (layout request). The copy
// itself reads the camera's absolute position and then sets the enlarged camera's position, target, up vector, near,
// far and aspect ratio, sets its field of view to the camera's times s_frustumScale, and calls recalculateMatrices. It is
// not written here because it would reference the camera statics s_enlargedCamera and s_frustumScale, which have no
// definition in this tree yet. The body returns without copying.
void RootSceneNode::UpdateEnlargedViewFrustum(glitch::scene::CSceneManager* manager) {
    (void)manager;
}

// 0x35c784 _ZNK13RootSceneNode17IsVisibleFastTestEv
// partial: the switch read is complete. The culling test is not. When the switch is clear and m_cullBeforeUpdate is set,
// the original calls ObjectBase::TestCullingBeforeUpdate on m_object with the box at ObjectBase +0x12c, and that box is not
// declared in dh2/ObjectBase.hpp (layout request). Until then this body returns true for an object with the gate set
// (no culling applied) and false when the gate is set and there is no object, as the original does.
bool RootSceneNode::IsVisibleFastTest() const {
    DebugSwitches::s_inst.load();
    if (DebugSwitches::s_inst.GetSwitch("Optim_DisableUpdateTimeOnlyForCulledNode")) {
        return false;
    }
    if (!m_cullBeforeUpdate) {
        return true;
    }
    if (m_object == nullptr) {
        return false;
    }
    return true;  // the TestCullingBeforeUpdate result is missing (see above)
}

// 0x35c854 _ZN13RootSceneNode18RefreshBoundingBoxEv
// partial: the original calls glitch::collada::CSceneNode::computeBoundingBox, then subtracts the node's position (getPosition,
// vptr+0xa0) twice from six box words at +0x130 .. +0x144. Those words are base members, so the body is empty.
void RootSceneNode::RefreshBoundingBox() {
}

// 0x35cc0c _ZN13RootSceneNode21ResetPositionFromFileEv
// partial: the original runs only when the children list head (ISceneNode +0xf4) is non-null and not 4. It then zeroes the
// first child's position, sets the child's rotation from quaternion::set(0, 0, 0) and its scale to (1, 1, 1), zeroes this
// node's position, and calls the child's updateAbsolutePosition(false) (vptr+0xb8). The list and the calls need the
// ISceneNode base, so the body is empty.
void RootSceneNode::ResetPositionFromFile() {
}

// 0x35ccdc _ZN13RootSceneNode11GetAnimRootEb
// partial: the original searches the subtree of this node with findSceneNodeRef(this, name). The names are "root_camera"
// (only when alternate is false), then "Bip01", "Root" and "root_character", in that order. Passing this node as an
// ISceneNode* needs the base, which is missing, so the function returns null.
glitch::scene::ISceneNode* RootSceneNode::GetAnimRoot(bool alternate) {
    (void)alternate;
    return nullptr;
}

// 0x35cd5c _ZN13RootSceneNode10_CalcDeltaERN6glitch4core8vector3dIfEE
// partial: zeroes the delta. The original then adds, for each animator on the list at +0xfc (base missing), the delta of
// GetApplicator(animator) (the applicator's +0x24 .. +0x2c vector). When GetApplicator returns null, gAssertLevel 2 makes
// the original store to address 0, and level 1 prints RootSceneNode.cpp line 244 and moves on. That loop is missing.
void RootSceneNode::_CalcDelta(glitch::core::vector3d<float>& delta) {
    delta.X = 0.0f;  // +0x00
    delta.Y = 0.0f;  // +0x04
    delta.Z = 0.0f;  // +0x08
}

// 0x35ce6c _ZN13RootSceneNode11_ResetDeltaEj
// partial: for each animator on the list at +0xfc (base missing) the original calls GetApplicator(animator) and then
// vptr+0x0c (AnimApplicator::ResetDelta) with the time. For a null applicator, gAssertLevel 2 makes the original store to
// address 0 and level 1 prints RootSceneNode.cpp line 222. The loop is missing, so the body is empty.
void RootSceneNode::_ResetDelta(std::uint32_t time) {
    (void)time;
}

// 0x35cf48 _ZN13RootSceneNode19_HandleDisplacementEj
// partial: the original reads the animation root's position (m_animRoot, vptr+0xa0), takes the delta from _CalcDelta,
// scales it by this node's scale (ISceneNode +0xc8) and rotates it by this node's rotation (+0xb8). It then moves this
// node (vptr+0xa4 on this). With a displacement node it moves that node by the negated delta, and without one it moves the
// animation root and its alternate. All of those need the ISceneNode base. The return value is the original's "the node
// moved" test on the rotated delta, and it is also missing. The body returns false.
bool RootSceneNode::_HandleDisplacement(std::uint32_t time) {
    (void)time;
    return false;
}

// 0x35d168 _ZN13RootSceneNode9onAnimateEj
// partial: the original's visibility step needs the ISceneNode flags (+0x11c), setVisible and the scene manager at +0x110
// (CSceneManager::notifyVisibilityChanged). Its cull step needs IsVisibleFastTest, the object's box and
// glitch::scene::SViewFrustum::intersects against s_enlargedCamera. Its animator and child loops need the base. The
// displacement path (_HandleDisplacement when +0x1ec is set) and the pending update (updateAbsolutePosition(true) through
// vptr+0xb8 when the flag at +0x208 is set; that flag is then cleared, and the flag at +0x20a is cleared on that path) are
// missing too, as are the two counters. The one part written is the last time, which the original stores on every path.
void RootSceneNode::onAnimate(std::uint32_t time) {
    m_lastAnimTime = time;
}

// 0x35d4cc _ZN13RootSceneNode19_EnableDisplacementEb
// partial: the early path is written. When the request is off, or when an animation root is already set, the original
// stores the request at +0x1ec and returns. The creation path is missing: GetAnimRoot(false) and GetAnimRoot(true) for the
// two roots (each takes a reference), a CEmptySceneNode with id -1 as the displacement node at +0x1f8, the children moved
// under that node, the node added to this one, and SetRefNode (AnimApplicator vptr+0x08) with the root on each applicator.
// All of it needs the ISceneNode base. Without the creation path the flag is not stored, and the original's end-of-path
// store of the result is missing too.
void RootSceneNode::_EnableDisplacement(bool enable) {
    if (!enable || m_animRoot != nullptr) {
        m_displacementEnabled = enable;
        return;
    }
    // creation path missing (see above)
}

// 0x35d624 _ZN13RootSceneNode7NewAnimEb
// Calls _EnableDisplacement with the request. Then, when displacement is on and an animation root exists, and the last
// animation time is not zero, it runs _ResetDelta with (last time + 1) and calls onAnimate with the last time through the
// vtable. The callees _EnableDisplacement, _ResetDelta and onAnimate are partial, so this body is exact only as far as
// their behaviour is.
void RootSceneNode::NewAnim(bool enable) {
    _EnableDisplacement(enable);
    if (m_displacementEnabled && m_animRoot != nullptr) {
        const std::uint32_t lastTime = m_lastAnimTime;
        if (lastTime != 0) {
            _ResetDelta(lastTime + 1);
            onAnimate(lastTime);
        }
    }
}

// 0x35d67c _ZN13RootSceneNodeD1Ev
// Drops the animation root (+0x1f0), the alternate root (+0x1f4) and the displacement node (+0x1f8), and clears each
// pointer. The original's member strings (m_name at +0x1d4, then m_fileName at +0x1bc) and the base destructor run after
// that body. C++ runs them in the same order after this body. The vptr stores are the compiler's. The secondary vptr
// store at +0x20c is not written here (see the part 2 note on m_secondaryVptr).
RootSceneNode::~RootSceneNode() {
    if (m_animRoot != nullptr) {
        m_animRoot->drop();
        m_animRoot = nullptr;
    }
    if (m_animRootAlt != nullptr) {
        m_animRootAlt->drop();
        m_animRootAlt = nullptr;
    }
    if (m_displacementNode != nullptr) {
        m_displacementNode->drop();
        m_displacementNode = nullptr;
    }
}

// 0x35d748 _ZN13RootSceneNodeD0Ev
// Skipped: the deleting destructor. It calls the complete-object destructor above and then frees the object. The C++
// delete expression generates it from the destructor.

// 0x35d764 _ZN13RootSceneNodeD2Ev
// Skipped: the base-object destructor, which takes the VTT as its second argument and restores the vptrs of the virtual
// base from it. The compiler generates it from the destructor above.
