// glitch::scene::CCameraSceneNode bodies. The class is declared in scene_nodes.hpp; the frustum it owns is in
// tb/glitch_scene_SViewFrustum.hpp. Where this file calls "the virtual at vptr+0x158", the call is
// recalculateProjectionMatrix(): a Hex-Rays offset is the listing offset of tools/decomp_vtable.py minus 0x0c, so
// 0x158 is listing +0x164.
#include <cmath>

#include "glitch-scene-collada-gui/core_matrix_builders.hpp"
#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x581f50 _ZNK6glitch5scene16CCameraSceneNode19getProjectionMatrixEv
// Original returns this + 628 (+0x274), the frustum's projection matrix (frustum +0x10c).
const core::CMatrix4<float>& CCameraSceneNode::getProjectionMatrix() const {
    return frustum.projectionMatrix;
}

// 0x581f58 _ZNK6glitch5scene16CCameraSceneNode13getViewMatrixEv
// Original returns this + 492 (+0x1ec), the frustum's view matrix (frustum +0x84).
const core::CMatrix4<float>& CCameraSceneNode::getViewMatrix() const {
    return frustum.viewMatrix;
}

// 0x581f68 _ZN6glitch5scene16CCameraSceneNode7onEventERKNS_6SEventE
// Returns 0 when the input receiver byte (+0x165) is clear. Otherwise walks the animator list (+0xfc, in order) and
// returns 1 at the first animator that is an enabled event receiver (animator vtable +0x1c) and handles the event
// (animator vtable +0x20); 0 when none does. The list is the std::vector of ISceneNode here.
bool CCameraSceneNode::onEvent(const glitch::SEvent& event) {
    if (!inputReceiverEnabled) {
        return false;
    }
    for (ISceneNodeAnimator* animator : animators) {
        if (animator->isEventReceiverEnabled() && animator->onEvent(event)) {
            return true;
        }
    }
    return false;
}

// 0x581fe4 _ZN6glitch5scene16CCameraSceneNode9setTargetERKNS_4core8vector3dIfEE
// Copies three words into +0x138..+0x140.
void CCameraSceneNode::setTarget(const core::vector3d<float>& value) {
    target = value;
}

// 0x582000 _ZNK6glitch5scene16CCameraSceneNode9getTargetEv
// Original returns this + 312 (+0x138).
const core::vector3d<float>& CCameraSceneNode::getTarget() const {
    return target;
}

// 0x582008 _ZN6glitch5scene16CCameraSceneNode11setUpVectorERKNS_4core8vector3dIfEE
// Copies three words into +0x144..+0x14c.
void CCameraSceneNode::setUpVector(const core::vector3d<float>& value) {
    upVector = value;
}

// 0x582024 _ZNK6glitch5scene16CCameraSceneNode11getUpVectorEv
// Original returns this + 324 (+0x144).
const core::vector3d<float>& CCameraSceneNode::getUpVector() const {
    return upVector;
}

// 0x58202c _ZNK6glitch5scene16CCameraSceneNode12getNearValueEv
// Original returns the word at +0x15c as raw bits (soft-float float return).
float CCameraSceneNode::getNearValue() const {
    return nearValue;
}

// 0x582034 _ZNK6glitch5scene16CCameraSceneNode11getFarValueEv
// Original returns the word at +0x160 as raw bits.
float CCameraSceneNode::getFarValue() const {
    return farValue;
}

// 0x58203c _ZNK6glitch5scene16CCameraSceneNode14getAspectRatioEv
// Original returns the word at +0x158 as raw bits.
float CCameraSceneNode::getAspectRatio() const {
    return aspectRatio;
}

// 0x582044 _ZNK6glitch5scene16CCameraSceneNode6getFOVEv
// Original returns the word at +0x154 as raw bits.
float CCameraSceneNode::getFOV() const {
    return fov;
}

// 0x58204c _ZN6glitch5scene16CCameraSceneNode17getFarToInifinityEv
// Returns the byte at +0x164 as a truth value.
bool CCameraSceneNode::getFarToInifinity() {
    return farToInfinity;
}

// 0x582054 _ZN6glitch5scene16CCameraSceneNode12setNearValueEf
// Stores +0x15c, then calls the virtual at vptr+0x158 (listing +0x164): recalculateProjectionMatrix().
void CCameraSceneNode::setNearValue(float value) {
    nearValue = value;
    recalculateProjectionMatrix();
}

// 0x58206c _ZN6glitch5scene16CCameraSceneNode11setFarValueEf
// Stores +0x160, then recalculateProjectionMatrix().
void CCameraSceneNode::setFarValue(float value) {
    farValue = value;
    recalculateProjectionMatrix();
}

// 0x582084 _ZN6glitch5scene16CCameraSceneNode14setAspectRatioEf
// Stores +0x158, then recalculateProjectionMatrix().
void CCameraSceneNode::setAspectRatio(float value) {
    aspectRatio = value;
    recalculateProjectionMatrix();
}

// 0x58209c _ZN6glitch5scene16CCameraSceneNode6setFOVEf
// Stores +0x154, then recalculateProjectionMatrix().
void CCameraSceneNode::setFOV(float value) {
    fov = value;
    recalculateProjectionMatrix();
}

// 0x5820b4 _ZN6glitch5scene16CCameraSceneNode6setMAGEf
// Stores +0x150 (mag), then recalculateProjectionMatrix().
void CCameraSceneNode::setMAG(float value) {
    mag = value;
    recalculateProjectionMatrix();
}

// 0x5820cc _ZN6glitch5scene16CCameraSceneNode17setFarToInifinityEb
// Stores the byte at +0x164, then recalculateProjectionMatrix().
void CCameraSceneNode::setFarToInifinity(bool value) {
    farToInfinity = value;
    recalculateProjectionMatrix();
}

// 0x5820e4 _ZN6glitch5scene16CCameraSceneNode6renderEPv
// PARTIAL, stub: the original reads the scene manager (+0x110), then its driver (manager +0x14); when the driver is
// set it calls the driver virtual at Hex-Rays +108 twice, first with state 2 and the projection matrix (+0x274, now
// frustum.projectionMatrix), then with state 0 and the view matrix (+0x1ec, frustum.viewMatrix). Missing: the
// manager's driver pointer and the driver interface (video::IVideoDriver is only forward-declared in this category),
// so the two setTransform calls cannot be written. This body does nothing.
void CCameraSceneNode::render(void* /*unused*/) {}

// 0x582138 _ZNK6glitch5scene16CCameraSceneNode14getViewFrustumEv
// Original returns this + 360 (+0x168), the SViewFrustum member.
const SViewFrustum* CCameraSceneNode::getViewFrustum() const {
    return &frustum;
}

// 0x5823ac _ZN6glitch5scene16CCameraSceneNode19setProjectionMatrixERKNS_4core8CMatrix4IfEEb
// Stores the flag byte at +0x134, copies the 65-byte matrix (16 floats and the identity flag) into +0x274, the
// frustum's projection matrix, and calls SViewFrustum::setTransformState(this + 0x168, 2).
void CCameraSceneNode::setProjectionMatrix(const core::CMatrix4<float>& matrix, bool isOrthogonal) {
    orthogonal = isOrthogonal;
    frustum.projectionMatrix = matrix;
    frustum.setTransformState(2);
}

// 0x582904 _ZN6glitch5scene16CCameraSceneNode18recalculateViewAreaEv
// Stores ISceneNode::getAbsolutePosition() in the frustum's first three words (this + 0x168) and returns
// SViewFrustum::setFrom(frustum, matrix at this + 0x2b8), the frustum's view-projection matrix.
bool CCameraSceneNode::recalculateViewArea() {
    frustum.cameraPosition = getAbsolutePosition();
    return frustum.setFrom(frustum.viewProjectionMatrix);
}

// 0x583280 _ZN6glitch5scene16CCameraSceneNode19recalculateMatricesEv
// The view direction (target - absolute position) and the up vector are each normalised when their squared length is
// non-zero (a reciprocal square root, as the listing does). When the two are parallel, |dot| within 1e-6 of 1, the
// up vector's x is moved by 0.5. The look-at matrix for (absolute position, target, up) becomes the view matrix
// (+0x1ec), SViewFrustum::setTransformState(0) refreshes the dependent matrices, and recalculateViewArea() gives the
// result. The look-at target is the raw target (+0x138), not the normalised direction.
bool CCameraSceneNode::recalculateMatrices() {
    const core::vector3d<float> position = getAbsolutePosition();  // v20
    float dx = target.X - position.X;                               // v2
    float dy = target.Y - position.Y;                               // v3
    float dz = target.Z - position.Z;                               // v8
    core::vector3d<float> up = upVector;                            // v17..v19
    const float directionLengthSquared = ((dx * dx) + (dy * dy)) + (dz * dz);
    if (directionLengthSquared != 0.0f) {
        const float scale = static_cast<float>(1.0 / std::sqrt(directionLengthSquared));  // v9
        dx = dx * scale;
        dy = dy * scale;
        dz = dz * scale;
    }
    const float upLengthSquared = ((up.X * up.X) + (up.Y * up.Y)) + (up.Z * up.Z);
    if (upLengthSquared != 0.0f) {
        const float scale = static_cast<float>(1.0 / std::sqrt(upLengthSquared));  // v10
        up.X = scale * up.X;
        up.Y = scale * up.Y;
        up.Z = scale * up.Z;
    }
    const float dot = ((dx * up.X) + (dy * up.Y)) + (dz * up.Z);  // v11
    const float absDot = std::fabs(dot);
    constexpr float kRoundingError = 1e-6f;  // 0.000001
    if (absDot + kRoundingError >= 1.0f && absDot - kRoundingError <= 1.0f) {
        up.X = up.X + 0.5f;
    }
    frustum.viewMatrix = core::buildCameraLookAtMatrix<float>(position, target, up);  // memcpy(this + 492, ..., 0x41)
    frustum.setTransformState(0);
    return recalculateViewArea();
}

// 0x583534 _ZN6glitch5scene16CCameraSceneNode20onRegisterSceneNodeEv
// Recalculates the matrices, then, when this camera is the scene manager's active camera (manager +0xe4), registers it
// with the manager through the manager's virtual at Hex-Rays +0x24 (listing +0x30, registerNodeForRendering) with an
// empty material, no user data, pass 0, no position and the sort order 0x7FFFFFFF. Returns 1.
// The manager (+0x110) is dereferenced without a null test in the original; a camera that is not attached to a
// manager skips the registration here instead. CSceneManager::registerNodeForRendering (0x58f348) is partial.
int CCameraSceneNode::onRegisterSceneNode() {
    recalculateMatrices();
    if (sceneManager != nullptr && sceneManager->activeCamera == this) {
        const boost::intrusive_ptr<video::CMaterial> noMaterial;
        sceneManager->registerNodeForRendering(this, noMaterial, nullptr, 0, nullptr, 0x7FFFFFFF);
    }
    return 1;
}

// 0x58364c _ZN6glitch5scene16CCameraSceneNode25recalculateProjectionMatrixEv
// Orthographic (virtual isOrthogonal(), listing +0x15c): height 2 * mag (+0x150), width height * aspect (+0x158),
// near (+0x15c) and far (+0x160). Otherwise the perspective matrix from fov (+0x154), aspect, near and, unless the
// far-to-infinity byte (+0x164) is set, far. The matrix is copied into +0x274 (the frustum's projection matrix) and
// SViewFrustum::setTransformState(2) is called. The pseudocode returns the result of that call (an int that is a
// leftover of the callee), so this is void.
void CCameraSceneNode::recalculateProjectionMatrix() {
    if (isOrthogonal()) {
        const float height = mag + mag;  // v3
        frustum.projectionMatrix =
            core::buildProjectionMatrixOrtho<float>(height * aspectRatio, height, nearValue, farValue);
    } else if (farToInfinity) {
        frustum.projectionMatrix =
            core::buildProjectionMatrixPerspectiveFovInfinity<float>(fov, aspectRatio, nearValue);
    } else {
        frustum.projectionMatrix =
            core::buildProjectionMatrixPerspectiveFov<float>(fov, aspectRatio, nearValue, farValue);
    }
    frustum.setTransformState(2);
}

}  // namespace glitch::scene
