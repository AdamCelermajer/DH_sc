// Test for batch g1-game-app-GSViewer-081209-0. Covers the constructor (the three node words are cleared), Ctor's
// camera creation (the arguments decoded from the Hex-Rays float constants: id -1, position (0, -500, 0), target
// (0, 0, 100), farToInfinity false) and Dtor (the three words are cleared in order).
//
// The camera is the canonical glitch::scene::CCameraSceneNode from the glitch-scene-collada-gui library, which the test
// links. Its constructor runs for real, and the checks read the values it stored through the public getters. The only
// stand-ins left are the StateBase members that GSViewer's vtable and destructor refer to.
// Exit status 0 means every check passed.
#include <cstdio>

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

}  // namespace

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
        Check(view.m_camera != nullptr, "Ctor stores the camera in the camera word");
        if (view.m_camera != nullptr) {
            Check(view.m_camera->getID() == -1, "camera id is -1");
            const glitch::core::vector3d<float>& position = view.m_camera->getPosition();
            Check(position.X == 0.0f && position.Y == -500.0f && position.Z == 0.0f,
                  "camera position is (0, -500, 0)");
            const glitch::core::vector3d<float>& target = view.m_camera->getTarget();
            Check(target.X == 0.0f && target.Y == 0.0f && target.Z == 100.0f, "camera target is (0, 0, 100)");
            Check(!view.m_camera->getFarToInifinity(), "camera is not far to infinity");
        }
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
