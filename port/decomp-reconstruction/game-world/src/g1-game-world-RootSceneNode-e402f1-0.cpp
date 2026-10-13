// Batch g1-game-world-RootSceneNode-e402f1-0 (part 1 of 2): the debug counters, the animation roots and the displacement,
// the cull gate, the bounding box, the animator hooks, NewAnim and the destructor. The constructors are in
// g1-game-world-RootSceneNode-e402f1-1 (part 2).
//
// The engine base glitch::collada::CRootSceneNode derives from CSceneNode and so from glitch::scene::ISceneNode (canonical
// layout: animators list at +0xfc, children at +0xf0, flags at +0x11c). The bodies below use those members through the
// base. Bodies that still need an unknown are partial; the note in each one names the missing part.

#include <cstdint>

#include "dh2/AnimApplicator.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/global/GetApplicator.hpp"
#include "dh2/global/scene_nodes.hpp"
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
// Walks the animators list (+0xfc). For each animator: tc = getTimelineCtrl() (vptr+0x44); when tc is not null,
// tc->jumpTo(time + tc->currentTimeMs) (vptr+0x0c, the word at +0x04).
void RootSceneNode::ForwardAnim(std::uint32_t time) {
    for (auto it = getAnimators().begin(); it != getAnimators().end(); ++it) {
        glitch::scene::ITimelineController* tc = (*it)->getTimelineCtrl();
        if (tc != nullptr) {
            tc->jumpTo(static_cast<std::int32_t>(time + static_cast<std::uint32_t>(tc->currentTimeMs)));
        }
    }
}

// 0x35c0f4 _ZN13RootSceneNode12onUpdateTimeEj
// Gate: ISceneNode flags (+0x11c) & 0x201 == 0x201 (visible and animate). Then updateTime(time) (vptr+0x14) on each animator.
void RootSceneNode::onUpdateTime(std::uint32_t time) {
    if ((flags & (glitch::scene::ESNF_VISIBLE | glitch::scene::ESNF_ANIMATE)) == (glitch::scene::ESNF_VISIBLE | glitch::scene::ESNF_ANIMATE)) {
        for (auto it = getAnimators().begin(); it != getAnimators().end(); ++it) {
            (*it)->updateTime(time);
        }
    }
}

// 0x35c268 _ZN13RootSceneNode14PlainOnAnimateEj
// Thunk: forwards to the engine base's onAnimate.
void RootSceneNode::PlainOnAnimate(std::uint32_t time) {
    glitch::collada::CRootSceneNode::onAnimate(time);
}

// 0x35c26c _ZN13RootSceneNode10setVisibleEb
// Stores the request at +0x209. For false it also calls glitch::scene::ISceneNode::setVisible(false).
void RootSceneNode::setVisible(bool visible) {
    m_visibleRequested = visible;
    if (!visible) {
        glitch::scene::ISceneNode::setVisible(false);
    }
}

// 0x35c27c _ZN13RootSceneNode22updateAbsolutePositionEb
// Counts the call, then forwards to glitch::scene::ISceneNode::updateAbsolutePosition.
void RootSceneNode::updateAbsolutePosition(bool recurse) {
    ++s_dbgNumUpdateAbsolutePosition;
    glitch::scene::ISceneNode::updateAbsolutePosition(recurse);
}

// 0x35c51c _ZN13RootSceneNode25UpdateEnlargedViewFrustumEPN6glitch5scene13CSceneManagerE
// partial: the view is copied from the level camera (the pointer at this+0xe4, ICameraSceneNode*) into s_enlargedCamera.
// +0xe4 lies inside ISceneNode's transformedBoundingBox (+0xd4 .. +0xec) in the canonical layout, so it is not the camera
// slot the original reads; that camera member is still to be located. s_enlargedCamera and s_frustumScale have no
// definition in the tree. The body returns without copying.
void RootSceneNode::UpdateEnlargedViewFrustum(glitch::scene::CSceneManager* manager) {
    (void)manager;
}

// 0x35c784 _ZNK13RootSceneNode17IsVisibleFastTestEv
// partial: the switch read is complete. The culling test is not: when the switch is clear and m_cullBeforeUpdate is set,
// the original calls ObjectBase::TestCullingBeforeUpdate on m_object with the box at ObjectBase +0x12c, and that box is not
// declared in dh2/ObjectBase.hpp (layout request). Until then this body returns true for an object with the gate set
// and false when the gate is set and there is no object.
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
// vptr+0xa0) twice from six box words at +0x130 .. +0x144. The base layout puts the transformed box at ISceneNode +0xd4, so
// the +0x130 .. +0x144 words are not yet mapped to a member. The body is empty.
void RootSceneNode::RefreshBoundingBox() {
}

// 0x35cc0c _ZN13RootSceneNode21ResetPositionFromFileEv
// The original runs only when the children list has a first child. It then sets that child's position to zero, its
// rotation to quaternion (0, 0, 0) and its scale to (1, 1, 1), sets this node's position to zero, and calls the child's
// updateAbsolutePosition(false). Note: the original tests the raw list head against 4, which is a non-null child pointer
// (the child is head - 4). An empty list is not reproduced: the original reads the child count word as a child there.
void RootSceneNode::ResetPositionFromFile() {
    if (getChildren().empty()) {
        return;
    }
    glitch::scene::ISceneNode* child = *getChildren().begin();
    glitch::core::vector3d<float> zero;
    zero.X = 0.0f;
    zero.Y = 0.0f;
    zero.Z = 0.0f;
    child->setPosition(zero);
    glitch::core::quaternion rotation;
    rotation.set(0.0f, 0.0f, 0.0f);
    child->setRotation(rotation);
    glitch::core::vector3d<float> one;
    one.X = 1.0f;
    one.Y = 1.0f;
    one.Z = 1.0f;
    child->setScale(one);
    setPosition(zero);
    child->updateAbsolutePosition(false);
}

// 0x35ccdc _ZN13RootSceneNode11GetAnimRootEb
// Search order: root_camera (only when alternate is false), then Bip01, Root and root_character, each by findSceneNodeRef.
glitch::scene::ISceneNode* RootSceneNode::GetAnimRoot(bool alternate) {
    glitch::scene::ISceneNode* result = nullptr;
    if (alternate || (result = findSceneNodeRef(this, "root_camera")) == nullptr) {
        result = findSceneNodeRef(this, "Bip01");
        if (result == nullptr) {
            result = findSceneNodeRef(this, "Root");
            if (result == nullptr) {
                return findSceneNodeRef(this, "root_character");
            }
        }
    }
    return result;
}

// 0x35cd5c _ZN13RootSceneNode10_CalcDeltaERN6glitch4core8vector3dIfEE
// partial: zeroes the delta, then adds the delta of each animator's applicator (m_delta, +0x24 .. +0x2c). A null
// applicator is skipped here. The original's assert path for that case (print at assert level 1, store to address 0 at
// level 2, RootSceneNode.cpp line 244) is not written.
void RootSceneNode::_CalcDelta(glitch::core::vector3d<float>& delta) {
    delta.X = 0.0f;  // +0x00
    delta.Y = 0.0f;  // +0x04
    delta.Z = 0.0f;  // +0x08
    for (auto it = getAnimators().begin(); it != getAnimators().end(); ++it) {
        AnimApplicator* applicator = GetApplicator(*it);
        if (applicator == nullptr) {
            continue;  // assert path not written (see above)
        }
        delta.X += applicator->m_delta.X;
        delta.Y += applicator->m_delta.Y;
        delta.Z += applicator->m_delta.Z;
    }
}

// 0x35ce6c _ZN13RootSceneNode11_ResetDeltaEj
// partial: for each animator, ResetDelta(time) (AnimApplicator vptr+0x0c) on its applicator. A null applicator is skipped
// here; the original's assert path (RootSceneNode.cpp line 222 at level 1) is not written.
void RootSceneNode::_ResetDelta(std::uint32_t time) {
    for (auto it = getAnimators().begin(); it != getAnimators().end(); ++it) {
        AnimApplicator* applicator = GetApplicator(*it);
        if (applicator == nullptr) {
            continue;  // assert path not written (see above)
        }
        applicator->ResetDelta(time);
    }
}

// 0x35cf48 _ZN13RootSceneNode19_HandleDisplacementEj
// partial: the original reads the animation root's position (m_animRoot, vptr+0xa0), takes the delta from _CalcDelta,
// scales it by this node's scale (ISceneNode +0xc8) and rotates it by this node's rotation (+0xb8). It then moves this
// node (vptr+0xa4 on this). With a displacement node it moves that node by the negated delta, and without one it moves the
// animation root and its alternate. The return value is the original's "the node moved" test on the rotated delta.
// The body returns false.
bool RootSceneNode::_HandleDisplacement(std::uint32_t time) {
    (void)time;
    return false;
}

// 0x35d168 _ZN13RootSceneNode9onAnimateEj
// partial: the visibility step, the cull step (IsVisibleFastTest and SViewFrustum::intersects against s_enlargedCamera),
// the animator and child loops, the displacement path and the pending update are not written (see the ledger). The one
// part written is the last time, which the original stores on every path.
void RootSceneNode::onAnimate(std::uint32_t time) {
    m_lastAnimTime = time;
}

// 0x35d4cc _ZN13RootSceneNode19_EnableDisplacementEb
// partial: the early path is written. When the request is off, or when an animation root is already set, the original
// stores the flag at +0x1ec and returns. The creation path is not written: GetAnimRoot(false) and GetAnimRoot(true)
// (each takes a reference), a CEmptySceneNode with id -1 as the displacement node at +0x1f8, the children moved under it,
// the node added to this one, and SetRefNode (AnimApplicator vptr+0x08) on each applicator.
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
