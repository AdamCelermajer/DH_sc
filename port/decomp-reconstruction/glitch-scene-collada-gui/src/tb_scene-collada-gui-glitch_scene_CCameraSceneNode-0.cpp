#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace glitch::scene {

// 0x581f50 _ZNK6glitch5scene16CCameraSceneNode19getProjectionMatrixEv
// Original returns this + 628 (+0x274), the projection matrix.
const core::CMatrix4<float>& CCameraSceneNode::getProjectionMatrix() const {
    return projectionMatrix;
}

// 0x581f58 _ZNK6glitch5scene16CCameraSceneNode13getViewMatrixEv
// Original returns this + 492 (+0x1ec), the view matrix.
const core::CMatrix4<float>& CCameraSceneNode::getViewMatrix() const {
    return viewMatrix;
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
// PARTIAL: stores the value at +0x15c. The original then calls the virtual at vptr+0x158
// (byte 344); that slot's name is not confirmed (see the ledger note), so the call is omitted.
void CCameraSceneNode::setNearValue(float value) {
    nearValue = value;
}

// 0x58206c _ZN6glitch5scene16CCameraSceneNode11setFarValueEf
// PARTIAL: stores +0x160. The vptr+0x158 call is omitted (see setNearValue).
void CCameraSceneNode::setFarValue(float value) {
    farValue = value;
}

// 0x582084 _ZN6glitch5scene16CCameraSceneNode14setAspectRatioEf
// PARTIAL: stores +0x158. The vptr+0x158 call is omitted (see setNearValue).
void CCameraSceneNode::setAspectRatio(float value) {
    aspectRatio = value;
}

// 0x58209c _ZN6glitch5scene16CCameraSceneNode6setFOVEf
// PARTIAL: stores +0x154. The vptr+0x158 call is omitted (see setNearValue).
void CCameraSceneNode::setFOV(float value) {
    fov = value;
}

// 0x5820b4 _ZN6glitch5scene16CCameraSceneNode6setMAGEf
// PARTIAL: stores +0x150. The vptr+0x158 call is omitted (see setNearValue).
void CCameraSceneNode::setMAG(float value) {
    mag = value;
}

// 0x5820cc _ZN6glitch5scene16CCameraSceneNode17setFarToInifinityEb
// PARTIAL: stores the byte at +0x164. The vptr+0x158 call is omitted (see setNearValue).
void CCameraSceneNode::setFarToInifinity(bool value) {
    farToInfinity = value;
}

// 0x5820e4 _ZN6glitch5scene16CCameraSceneNode6renderEPv
// PARTIAL, stub: the original reads the scene manager pointer at +0x110 (inherited from
// ISceneNode, not mapped), then the driver at manager+0x14, and calls two driver virtuals at
// byte 108 (state 2 with the projection matrix, then state 0 with the view matrix). None of
// that is reconstructed, so this body does nothing.
void CCameraSceneNode::render(void* /*unused*/) {}

// 0x582138 _ZNK6glitch5scene16CCameraSceneNode14getViewFrustumEv
// PARTIAL, stub: the original returns this + 360 (+0x168), the SViewFrustum member. The type
// is not declared in this category, so this returns null for now.
SViewFrustum* CCameraSceneNode::getViewFrustum() const {
    return nullptr;
}

// 0x5823ac _ZN6glitch5scene16CCameraSceneNode19setProjectionMatrixERKNS_4core8CMatrix4IfEEb
// Stores the flag byte at +0x134, then copies the 65-byte matrix (16 floats and the identity
// flag) into +0x274. Copy assignment covers the same members.
// PARTIAL: the original then calls glitch::scene::SViewFrustum::setTransformState(this+0x168, 2).
// SViewFrustum is not declared in this category, so that call is omitted.
void CCameraSceneNode::setProjectionMatrix(const core::CMatrix4<float>& matrix, bool isOrthogonal) {
    orthogonal = isOrthogonal ? 1 : 0;
    projectionMatrix = matrix;
}

}  // namespace glitch::scene
