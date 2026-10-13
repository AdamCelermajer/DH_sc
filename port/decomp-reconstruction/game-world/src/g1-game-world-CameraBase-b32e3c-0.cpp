// Batch g1-game-world-CameraBase-b32e3c-0: CameraBase constructors, destructors, activation hooks, camera vector
// getters, SetData, world and screen projection, the center offset, and the static member s_activeCamera.
// Declarations come from dh2/CameraBase.hpp. A PARTIAL body names the callee or layout it waits on.

#include <cmath>

#include "dh2/CameraBase.hpp"
#include "dh2/global/Vec3f_Origin.hpp"  // Vec3f_Origin (0x99f854), the zero Point3D<float> global
#include "glitch-scene-collada-gui/scene_nodes.hpp"

// Static member. ELF symbol _ZN10CameraBase14s_activeCameraE, 4 bytes in .bss.
CameraBase* CameraBase::s_activeCamera = nullptr;

// 0x40e718 CameraBase::Activated()
void CameraBase::Activated() {
}

// 0x40e71c CameraBase::Deactivated()
void CameraBase::Deactivated() {
}

// 0x40ea50 CameraBase::Update()
void CameraBase::Update() {
}

// 0x40e720 CameraBase::CameraBase()   (base-object constructor, C2)
// 0x40e750 CameraBase::CameraBase()   (complete-object constructor, C1; same body)
// The vptr word at +0x00 is written by the compiler.
CameraBase::CameraBase() : m_node(nullptr), m_camera(nullptr) {  // +0x04, +0x08
}

// 0x40e814 CameraBase::~CameraBase()   (complete-object destructor, D1)
// 0x40e780 CameraBase::~CameraBase()   (base-object destructor, D2; same body)
// The original drops m_node (+0x04) and m_camera (+0x08) through the IReferenceCounted virtual base of each glitch
// object, and nulls both words. ISceneNode derives from glitch::IReferenceCounted (scene/ISceneNode.hpp), so drop()
// is called on each node.
CameraBase::~CameraBase() {
    if (s_activeCamera == this) {
        s_activeCamera = nullptr;
    }
    if (m_node) {
        m_node->drop();
        m_node = nullptr;
    }
    if (m_camera) {
        m_camera->drop();
        m_camera = nullptr;
    }
}

// 0x40e8a8 CameraBase::GetCameraLookAtVec() const
Point3D<float> CameraBase::GetCameraLookAtVec() const {
    if (m_camera) {
        // Vtable call +0x38 on the camera node (ISceneNode::getAbsoluteTransformation; the camera vtable's listing
        // +0x44, not overridden by CCameraSceneNode), then words 4, 5 and 6 of the 4x4 matrix: row 1 of the absolute
        // transform.
        const float* m = m_camera->getAbsoluteTransformation().M;
        return Point3D<float>{m[4], m[5], m[6]};
    }
    return Vec3f_Origin;
}

// 0x40e928 CameraBase::GetCameraUpVec() const
Point3D<float> CameraBase::GetCameraUpVec() const {
    if (m_camera) {
        // Same call as GetCameraLookAtVec. Words 8, 9 and 10 of the matrix: row 2 of the absolute transform.
        const float* m = m_camera->getAbsoluteTransformation().M;
        return Point3D<float>{m[8], m[9], m[10]};
    }
    return Vec3f_Origin;
}

// 0x40e9a8 CameraBase::SetData(float,float,float,float,bool)
// With m_camera set, the original calls, in this order: the camera's vtable slot +0x13c with a2 (setFOV, listing
// +0x148), slot +0x138 with a3 (setAspectRatio, listing +0x144), slot +0x130 with a4 (setNearValue, listing +0x13c),
// slot +0x134 with a5 (setFarValue, listing +0x140), and slot +0x114 with the vector (0, 0, 1) (setUpVector, listing
// +0x120). The call offset is the listing offset minus the 0x0c vtable header (tools/decomp_vtable.py
// CCameraSceneNode). The bool argument is not read by the original.
void CameraBase::SetData(float fov, float aspect, float nearValue, float farValue, bool unused) {
    (void)unused;
    if (!m_camera) {
        return;
    }
    m_camera->setFOV(fov);
    m_camera->setAspectRatio(aspect);
    m_camera->setNearValue(nearValue);
    m_camera->setFarValue(farValue);
    m_camera->setUpVector(glitch::core::vector3d<float>{0.0f, 0.0f, 1.0f});
}

// 0x40f2f0 CameraBase::GetWorldCoord(Point2D<int> const&,Point3D<float>&,float)
// PARTIAL: the view ray is not made. The original reads the device at dword_99F73C, the scene manager at device +0x1c
// and its collision manager at scene manager +0x2c. It calls the collision manager's vtable +0x14 with the screen point
// and a null camera to get a line3d (start and end). It then intersects that line with the plane normal (0, 0, 1) and
// D = -z through plane3d::getIntersectionWithLimitedLine. The device's +0x1c member is not declared (see SetActive),
// and no collision manager interface declares the ray-from-screen call, so the function returns false.
bool CameraBase::GetWorldCoord(const Point2D<int>& screen, Point3D<float>& world, float z) {
    (void)screen;
    (void)world;
    (void)z;
    return false;
}

// 0x40f394 CameraBase::GetWorldCoord(Point2D<float> const&,Point3D<float>&,float)
// PARTIAL: needs the viewport size. The original reads the video driver at device +0x10, then the record at
// *(driver +0xcc) - 4, and takes its width (+0x0c) and height (+0x10) as ints. It maps the screen x and y in -1..1 to
// pixels as (v + 1) * 0.5 * size, truncated to int, and calls the Point2D<int> overload with z. The driver layout is
// not declared, so the function returns false.
bool CameraBase::GetWorldCoord(const Point2D<float>& screen, Point3D<float>& world, float z) {
    (void)screen;
    (void)world;
    (void)z;
    return false;
}

// 0x40f45c CameraBase::SetActive()
// PARTIAL: the scene manager call is not made. The original makes this the active camera (Deactivated() on the
// previous one comes first), then calls glitch::scene::CSceneManager::setActiveCamera(scene manager, m_camera). The
// scene manager is at device +0x1c (device = dword_99F73C). CSceneManager::setActiveCamera is declared
// (scene/CSceneManager.hpp), but glitch::IDevice (glitch-core-io/os/device.hpp) declares no member at +0x1c.
// Layout request: "glitch::IDevice +0x1c CSceneManager* scene manager (SetActive 0x40f45c, DBG_DrawCircle 0x524b30)".
void CameraBase::SetActive() {
    if (s_activeCamera != this) {
        if (s_activeCamera) {
            s_activeCamera->Deactivated();
        }
        s_activeCamera = this;
        // Not made: CSceneManager::setActiveCamera(scene manager, m_camera).
        s_activeCamera->Activated();
    }
}

// 0x40f4dc CameraBase::GetCenterOffset(Point3D<float>&,float) const
// PARTIAL: the centring correction is not made. Without a camera node the original returns false and leaves the offset
// alone. Otherwise it writes offset = (m[4], m[5], 0) from the camera's absolute transform. When m[4]^2 + m[5]^2 is at
// least 0.0001 it scales the offset by a factor built from: the camera's absolute Z minus z (ISceneNode::getAbsolutePosition
// on the camera); the angle between the view vector (m[4], m[5], m[6]) and -Vec3f_K, where Vec3f_K = (0, 0, 1); and
// tan() of that angle plus and minus half of the field of view, read twice from the camera's vtable slot +0x128
// (call offset; listing +0x134, ICameraSceneNode::getFOV). The scaling needs Point3D<float>::angle and
// Point3D<float>::normalize, which core-platform/deps/game_types.hpp does not declare, and Vec3f_K, which has no
// declaration. The scaling is left out.
bool CameraBase::GetCenterOffset(Point3D<float>& offset, float z) const {
    (void)z;
    if (!m_camera) {
        return false;
    }
    const float* m = m_camera->getAbsoluteTransformation().M;
    offset.X = m[4];
    offset.Y = m[5];
    offset.Z = 0.0f;
    if (std::fabs(m[4] * m[4] + m[5] * m[5]) >= 0.0001f) {
        // PARTIAL: the scaling of the offset is not made (see above).
    }
    return true;
}

// 0x40f714 CameraBase::GetScreenCoord(Point3D<float> const&,Point2D<float>&)
// PARTIAL: the projection is not made. The original gets the view matrix (video driver getTransform(0)) and the
// projection matrix (getTransform(2)) from the driver at device +0x10. It multiplies them as projection * view and
// applies the 4x4 result to (x, y, z, 1). The screen point becomes (x / w, y / w). IVideoDriver::getTransform is
// declared (glitch-video/video_driver.hpp), but glitch::IDevice declares no member at +0x10.
// Layout request: "glitch::IDevice +0x10 IVideoDriver* driver (GetScreenCoord 0x40f714, GetWorldCoord 0x40f394, DBG_DrawPath 0x524828)".
void CameraBase::GetScreenCoord(const Point3D<float>& world, Point2D<float>& screen) {
    (void)world;
    (void)screen;
}

// 0x40f6f8 CameraBase::~CameraBase()   (deleting destructor, D0): not defined here. The compiler emits it for
// `delete` on a CameraBase*; its body is D1 followed by CustomFree(this).
