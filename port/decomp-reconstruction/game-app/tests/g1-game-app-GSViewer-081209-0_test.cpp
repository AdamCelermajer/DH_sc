// Test for batch g1-game-app-GSViewer-081209-0. Covers the constructor (the three node words are cleared), Ctor's
// camera creation (the arguments decoded from the Hex-Rays float constants: id -1, position (0, -500, 0), target
// (0, 0, 100), farToInfinity false) and Dtor (the three words are cleared in order).
//
// The engine is not linked. The camera chain is stood in as follows, against the current scene_nodes.hpp:
//   - CCameraSceneNode's constructor forwards to ICameraSceneNode's four-argument constructor (rotation zero, scale
//     one, as the engine's constructor does), and records its own four arguments for the checks.
//   - ICameraSceneNode's constructor and the out-of-line virtuals of the camera vtable are defined here. The vtable of
//     CCameraSceneNode is emitted in this translation unit (setProjectionMatrix is its key function), so every slot
//     it lists needs a definition. None of those slots is called by GSViewer's code exercised here; each one aborts
//     if it is ever reached, so a call would fail loudly instead of passing silently.
// Exit status 0 means every check passed.
#include <cstdio>
#include <cstdlib>

#include "dh2/GSViewer.hpp"
#include "glitch-scene-collada-gui/scene_nodes.hpp"

namespace {

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// Values recorded by the stub camera constructor.
int g_cameraCount = 0;
int g_cameraId = 0;
glitch::core::vector3d<float> g_cameraPosition{};
glitch::core::vector3d<float> g_cameraTarget{};
bool g_cameraFarToInfinity = true;

// Called by the stand-in camera virtuals, which this test never reaches.
[[noreturn]] void NotCalled(const char* name) {
    std::fprintf(stderr, "camera stand-in: %s was called, which this test does not expect\n", name);
    std::abort();
}

}  // namespace

namespace glitch::scene {

// Stand-in for the engine's view frustum constructor, which the camera's frustum member needs (not linked here).
SViewFrustum::SViewFrustum() {}

// Stand-in for the engine's ICameraSceneNode constructor (not linked in this test).
ICameraSceneNode::ICameraSceneNode(int, const core::vector3d<float>&, const core::vector3d<float>&,
                                   const core::vector3d<float>&) {}

// Stand-in for the engine's camera constructor. Forwards to the base as the engine does and records the arguments.
CCameraSceneNode::CCameraSceneNode(int id, const core::vector3d<float>& position, const core::vector3d<float>& target,
                                   bool farToInfinityValue)
    : ICameraSceneNode(id, position, core::vector3d<float>{}, core::vector3d<float>{1.0f, 1.0f, 1.0f}) {
    ++g_cameraCount;
    g_cameraId = id;
    g_cameraPosition = position;
    g_cameraTarget = target;
    g_cameraFarToInfinity = farToInfinityValue;
}

// Camera virtuals the vtable needs. Each one aborts if reached (see NotCalled).
void CCameraSceneNode::setProjectionMatrix(const core::CMatrix4<float>&, bool) { NotCalled("setProjectionMatrix"); }
const core::CMatrix4<float>& CCameraSceneNode::getProjectionMatrix() const { NotCalled("getProjectionMatrix"); }
const core::CMatrix4<float>& CCameraSceneNode::getViewMatrix() const { NotCalled("getViewMatrix"); }
bool CCameraSceneNode::onEvent(const glitch::SEvent&) { NotCalled("onEvent"); }
void CCameraSceneNode::setTarget(const core::vector3d<float>&) { NotCalled("setTarget"); }
const core::vector3d<float>& CCameraSceneNode::getTarget() const { NotCalled("getTarget"); }
void CCameraSceneNode::setUpVector(const core::vector3d<float>&) { NotCalled("setUpVector"); }
const core::vector3d<float>& CCameraSceneNode::getUpVector() const { NotCalled("getUpVector"); }
float CCameraSceneNode::getNearValue() const { NotCalled("getNearValue"); }
float CCameraSceneNode::getFarValue() const { NotCalled("getFarValue"); }
float CCameraSceneNode::getAspectRatio() const { NotCalled("getAspectRatio"); }
float CCameraSceneNode::getFOV() const { NotCalled("getFOV"); }
bool CCameraSceneNode::getFarToInifinity() { NotCalled("getFarToInifinity"); }
void CCameraSceneNode::setNearValue(float) { NotCalled("setNearValue"); }
void CCameraSceneNode::setFarValue(float) { NotCalled("setFarValue"); }
void CCameraSceneNode::setAspectRatio(float) { NotCalled("setAspectRatio"); }
void CCameraSceneNode::setFOV(float) { NotCalled("setFOV"); }
void CCameraSceneNode::setFarToInifinity(bool) { NotCalled("setFarToInifinity"); }
const SViewFrustum* CCameraSceneNode::getViewFrustum() const { NotCalled("getViewFrustum"); }
void CCameraSceneNode::setInputReceiverEnabled(bool) { NotCalled("setInputReceiverEnabled"); }
bool CCameraSceneNode::isInputReceiverEnabled() const { NotCalled("isInputReceiverEnabled"); }
void CCameraSceneNode::setMAG(float) { NotCalled("setMAG"); }
void CCameraSceneNode::recalculateProjectionMatrix() { NotCalled("recalculateProjectionMatrix"); }
ISceneNode* ICameraSceneNode::getTargetNode() const { NotCalled("getTargetNode const"); }
ISceneNode* ICameraSceneNode::getTargetNode() { NotCalled("getTargetNode"); }
bool ICameraSceneNode::isOrthogonal() const { NotCalled("isOrthogonal"); }

}  // namespace glitch::scene

// Empty stand-ins for the StateBase members that GSViewer's vtable and destructor refer to.
StateBase::~StateBase() {}
void StateBase::Ctor(const StateMachine*) {}
void StateBase::Dtor(const StateMachine*) {}
void StateBase::Pause(const StateMachine*) {}
void StateBase::Resume(const StateMachine*) {}
void StateBase::Update(StateMachine*, double) {}
void StateBase::Draw(const StateMachine*) {}
void StateBase::Draw2D(const StateMachine*) {}
void StateBase::Sleep(const StateMachine*) {}
void StateBase::WakeUp(const StateMachine*) {}

int main() {
    {
        // Constructor: all three node words are null.
        GSViewer view;
        Check(view.m_camera == nullptr, "constructor clears the camera word");
        Check(view.m_target == nullptr, "constructor clears the target word");
        Check(view.m_light == nullptr, "constructor clears the light word");

        // Ctor: the camera is created once, with the decoded arguments.
        view.Ctor(nullptr);
        Check(g_cameraCount == 1, "Ctor creates one camera");
        Check(view.m_camera != nullptr, "Ctor stores the camera in the camera word");
        Check(g_cameraId == -1, "camera id is -1");
        Check(g_cameraPosition.X == 0.0f && g_cameraPosition.Y == -500.0f && g_cameraPosition.Z == 0.0f,
              "camera position is (0, -500, 0)");
        Check(g_cameraTarget.X == 0.0f && g_cameraTarget.Y == 0.0f && g_cameraTarget.Z == 100.0f,
              "camera target is (0, 0, 100)");
        Check(!g_cameraFarToInfinity, "camera is not far to infinity");
        Check(view.m_target == nullptr && view.m_light == nullptr,
              "Ctor does not create the target or light yet (partial)");

        // Dtor: the three words are cleared. They are set to stand-in addresses that Dtor does not dereference.
        int standIn[3] = {0, 0, 0};
        view.m_camera = reinterpret_cast<glitch::scene::CCameraSceneNode*>(&standIn[0]);
        view.m_target = reinterpret_cast<glitch::scene::CEmptySceneNode*>(&standIn[1]);
        view.m_light = reinterpret_cast<glitch::scene::CLightSceneNode*>(&standIn[2]);
        view.Dtor(nullptr);
        Check(view.m_camera == nullptr, "Dtor clears the camera word");
        Check(view.m_target == nullptr, "Dtor clears the target word");
        Check(view.m_light == nullptr, "Dtor clears the light word");
    }

    if (g_failures == 0) {
        std::printf("g1-game-app-GSViewer-081209-0 test: all checks passed\n");
        return 0;
    }
    std::printf("g1-game-app-GSViewer-081209-0 test: %d check(s) failed\n", g_failures);
    return 1;
}
