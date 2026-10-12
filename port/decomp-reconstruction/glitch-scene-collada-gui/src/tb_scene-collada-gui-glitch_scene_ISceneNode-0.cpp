// Bodies for the glitch::scene::ISceneNode batch e1-glitch-scene-collada-gui-glitch_scene_ISceneNode-0.
// Offsets are the 32-bit ones from the Hex-Rays pseudocode, kept as comments. The original methods are
// virtual (vtable slots are noted); the class in scene_node.hpp has no vptr yet, so the calls below are
// direct. Children and animators are intrusive lists in the original; scene_node.hpp models them as
// std::vector (same order: appended at the tail, removed by identity).
#include <algorithm>

#include "glitch-scene-collada-gui/scene_node.hpp"

namespace glitch::scene {

// 0x596d08 glitch::scene::ISceneNode::onRegisterSceneNode()
int ISceneNode::onRegisterSceneNode() {
    return 1;
}

// 0x596d6c glitch::scene::ISceneNode::onAnimate(unsigned int)
// PARTIAL: the animator loop over the +0xfc list calls animator->vtable[4](animator, this, timeMs). The
// ISceneNodeAnimator vtable slot is not named yet, so that loop is missing. The rest follows the pseudocode.
void ISceneNode::onAnimate(unsigned int timeMs) {
    const std::uint32_t f = flags;  // +0x11c
    if (((f & 0x400u) == 0 || (f & 0x1u) != 0) && (f & 0x200u) != 0) {
        // MISSING: for each animator in the +0xfc list: animator->vtable[4](animator, this, timeMs).
        updateAbsolutePosition(false);  // original: this->vtable[46](this, 0), slot 0xb8 (updateAbsolutePosition)
        for (ISceneNode* child : children) {  // original: list at +0xf4, child->vtable[5](child, timeMs)
            child->onAnimate(timeMs);
        }
        flags &= ~0x20u;  // +0x11c
    }
}

// 0x596e2c glitch::scene::ISceneNode::getName() const
// PARTIAL: returns the 32-bit word at +0x20. The type of the name storage is not confirmed; the
// original returns it as int, so the return type follows the pseudocode.
int ISceneNode::getName() const {
    return nameWord;  // +0x20
}

// 0x597004 glitch::scene::ISceneNode::removeChild(glitch::scene::ISceneNode*)
// PARTIAL: the list unlink is done on the std::vector stand-in. The original then calls
// IReferenceCounted::drop() on the child's base subobject; ISceneNode has no IReferenceCounted base in
// this header yet, so that release is missing.
int ISceneNode::removeChild(ISceneNode* child) {
    if (child->parent != this) {  // +0xec
        return 0;
    }
    auto it = std::find(children.begin(), children.end(), child);  // +0xf0 count, +0xf4 list
    if (it != children.end()) {
        children.erase(it);
    }
    child->parent = nullptr;  // +0xec
    // MISSING: child->drop() on the IReferenceCounted base (original computes the base via the vtable).
    return 1;
}

// 0x5970bc glitch::scene::ISceneNode::getScale() const
const core::vector3d<float>& ISceneNode::getScale() const {
    return scale;  // +0xc8..+0xd3
}

// 0x5970c4 glitch::scene::ISceneNode::setScale(glitch::core::vector3d<float> const&)
// The pseudocode returns this (r0); the C++ signature is void.
void ISceneNode::setScale(const core::vector3d<float>& value) {
    scale = value;   // +0xc8..+0xd3
    flags |= 0x2u;   // +0x11c, bit 1: scale changed
}

// 0x5970ec glitch::scene::ISceneNode::getRotation() const
const core::quaternion& ISceneNode::getRotation() const {
    return rotation;  // +0xb8..+0xc7
}

// 0x5970f4 glitch::scene::ISceneNode::setRotation(glitch::core::quaternion const&)
void ISceneNode::setRotation(const core::quaternion& value) {
    rotation = value;  // +0xb8..+0xc7
    flags |= 0x4u;     // +0x11c, bit 2: rotation changed
}

// 0x597124 glitch::scene::ISceneNode::getPosition() const
const core::vector3d<float>& ISceneNode::getPosition() const {
    return position;  // +0xac..+0xb7
}

// 0x59712c glitch::scene::ISceneNode::setPosition(glitch::core::vector3d<float> const&)
void ISceneNode::setPosition(const core::vector3d<float>& value) {
    position = value;  // +0xac..+0xb7
    flags |= 0x8u;     // +0x11c, bit 3: position changed
}

// 0x597154 glitch::scene::ISceneNode::setPosition(float, float, float)
// PARTIAL: the original calls setPosition(vector3d const&) through the vtable (slot 0xb0). The direct
// call here is the base implementation only; a derived override would be bypassed.
void ISceneNode::setPosition(float x, float y, float z) {
    setPosition(core::vector3d<float>{x, y, z});
}

// 0x597180 glitch::scene::ISceneNode::getAbsolutePosition() const
core::vector3d<float> ISceneNode::getAbsolutePosition() const {
    // The translation column of the absolute matrix (+0x54..+0x5f, word indices 12..14 of the 4x4).
    return core::vector3d<float>{absoluteTransform[12], absoluteTransform[13], absoluteTransform[14]};
}

// 0x597290 glitch::scene::ISceneNode::getParent() const
ISceneNode* ISceneNode::getParent() const {
    return parent;  // +0xec
}

// 0x59770c glitch::scene::ISceneNode::getTransformedBoundingBox() const
// PARTIAL: when +0x11c bit 8 is set, the original takes the source box from the pure virtual at slot
// 0x3c (not named in the vtable listing), stores it in +0xd4, transforms it by the absolute matrix and
// clears bit 8. That branch is missing, so the cached box is returned as it is.
const core::aabbox3d<float>& ISceneNode::getTransformedBoundingBox() const {
    // MISSING when (flags & 0x100) != 0: source box from vtable slot 0x3c, transformBoxEx, clear 0x100.
    return transformedBoundingBox;  // +0xd4..+0xeb
}

// 0x597c60 glitch::scene::ISceneNode::updateAbsolutePosition(bool)
// PARTIAL: with a parent, the absolute matrix is parent->getAbsoluteTransformation() times
// getRelativeTransformation() through CMatrix4Base::mult34 (glitch-core-io, not on this category's include
// path). That product is MISSING; the flag update is kept. The virtual calls in the original are direct
// calls here. The memcpy branch (no parent) and the child loop are direct calls as well.
void ISceneNode::updateAbsolutePosition(bool includeChildren) {
    if (parent != nullptr) {
        if ((parent->flags & 0x20u) != 0 || (flags & 0x5Eu) != 0) {
            // MISSING: absolute (+0x24, float[16]) = mult34(parent->getAbsoluteTransformation(),
            // getRelativeTransformation()). Original call: mult34(v9, v10, this + 36).
            flags = (flags & 0xFFFFFE8Fu) | 0x120u;  // +0x11c
        }
    } else if ((flags & 0x5Eu) != 0) {
        const core::CMatrix4<float>& relative = getRelativeTransformation();  // original: slot 0x4c
        for (int i = 0; i < 16; ++i) {
            absoluteTransform[i] = relative.M[i];  // memcpy of 0x41 bytes; flag byte at +0x64 not mapped
        }
        flags = (flags & 0xFFFFFE8Fu) | 0x120u;
    }
    if (includeChildren) {
        for (ISceneNode* child : children) {  // original: list at +0xf4, child->vtable[46](child, 1), slot 0xc4
            child->updateAbsolutePosition(true);
        }
    }
}

// 0x598864 glitch::scene::ISceneNode::addChild(glitch::scene::ISceneNode*)
// PARTIAL: the original (1) returns this when the child is this or null, (2) takes a reference on the
// child's IReferenceCounted base, (3) calls child->remove() (slot 0x74), (4) appends to the child list
// (+0xf8 tail), (5) calls setParent(child, this), (6) notifies the scene manager at +0x110 when it is set,
// and (7) returns the result of child->notifyVisibilityChanged(flags & 1) (slot 0xf8). Steps 2, 3, 5, 6,
// and 7 are missing here; the return value is the child.
ISceneNode* ISceneNode::addChild(ISceneNode* child) {
    if (child == this || child == nullptr) {
        return this;
    }
    children.push_back(child);  // step 4
    return child;  // MISSING: steps 2, 3, 5, 6, 7 (see above)
}

// 0x598908 glitch::scene::ISceneNode::getRelativeTransformation() const
// Rebuilds the relative matrix from the rotation and scale when bits 1 and 2 are set, then writes the
// translation from position, clears the flag byte and sets bit 4 of +0x11c.
const core::CMatrix4<float>& ISceneNode::getRelativeTransformation() const {
    const std::uint32_t current = flags;  // +0x11c
    if ((current & 0xEu) == 0) {
        return relativeTransform;  // +0x68
    }
    if ((current & 0x6u) != 0) {
        core::CMatrix4<float> rebuilt{};
        rebuilt.flag = false;  // the flag byte at +0x40 of the temporary is cleared before the call
        rotation.getMatrix_transposed(rebuilt);  // +0xb8
        relativeTransform = rebuilt;             // memcpy of 0x41 bytes
        if (scale.X != 1.0f || scale.Y != 1.0f || scale.Z != 1.0f) {
            relativeTransform.postScale(scale);  // +0xc8
        }
    }
    relativeTransform.M[12] = position.X;  // +0xac, written to +0x98
    relativeTransform.M[13] = position.Y;  // +0xb0, written to +0x9c
    relativeTransform.M[14] = position.Z;  // +0xb4, written to +0xa0
    relativeTransform.flag = false;        // +0xa8
    flags = (flags & 0xFFFFFFE1u) | 0x10u;  // +0x11c
    return relativeTransform;
}

}  // namespace glitch::scene
