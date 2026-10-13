// Test for batch g1-game-world-GameObject-5d2019-5.
//
// Covers the state that the non-virtual methods of this unit touch: GetTargetPosition (target node and visibility),
// SetDestination, IsAtDestination (waypoint list and distance rule) and SetRotation. The object is zero-filled and never
// constructed, as in the other GameObject tests. The waypoint list is constructed in place on PFObject::m_path.
//
// UpdateRotation is not covered here. It calls GetRotationSpeed() through the vtable, and a zero-filled object has no
// vtable to dispatch through. Its engine calls (ApplicationGetInstance, Application::GetDt and VisualObject::SyncRotation)
// are stubbed only so that the unit links. The Lua bindings in the unit are partial and are not called.

#include <cstdio>
#include <cstring>
#include <new>

#include "dh2/Application.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ctrl_camera.hpp"

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                             \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

alignas(GameObject) unsigned char g_storage[sizeof(GameObject)];
alignas(Application) unsigned char g_appStorage[sizeof(Application)];

GameObject* Storage() {
    return reinterpret_cast<GameObject*>(g_storage);
}

using PathList = decltype(PFObject::m_path);

}  // namespace

// Stubs for the engine calls that UpdateRotation makes. The test never calls UpdateRotation.
Application& ApplicationGetInstance() {
    return *reinterpret_cast<Application*>(g_appStorage);
}

int Application::GetDt() {
    return 0;
}

void VisualObject::SyncRotation() {}

int main() {
    // GetTargetPosition: the target node's position only when there is a node and the object is visible.
    std::memset(g_storage, 0, sizeof g_storage);
    GameObject* go = Storage();
    go->m_position = Point3D<float>{1.0f, 2.0f, 3.0f};
    go->m_targetNodePos = Point3D<float>{7.0f, 8.0f, 9.0f};
    go->m_visible = true;

    EXPECT(&go->GetTargetPosition() == &go->m_position);  // no target node
    int nodeMarker = 0;
    go->m_targetNode = reinterpret_cast<glitch::scene::ISceneNode*>(&nodeMarker);  // never dereferenced
    EXPECT(&go->GetTargetPosition() == &go->m_targetNodePos);  // node and visible
    go->m_visible = false;
    EXPECT(&go->GetTargetPosition() == &go->m_position);  // node but hidden
    go->m_visible = true;
    go->m_targetNode = nullptr;
    EXPECT(&go->GetTargetPosition() == &go->m_position);  // visible but no node

    // SetDestination: copies the three components.
    go->SetDestination(Point3D<float>{3.0f, 4.0f, 5.0f});
    EXPECT(go->m_destination.X == 3.0f && go->m_destination.Y == 4.0f && go->m_destination.Z == 5.0f);

    // IsAtDestination with an empty waypoint list: squared XY distance from m_position to m_destination below 6400.
    std::memset(g_storage, 0, sizeof g_storage);
    go = Storage();
    new (&go->m_pfObject.m_path) PathList();  // the list is constructed in place
    go->m_position = Point3D<float>{0.0f, 0.0f, 0.0f};
    go->m_destination = Point3D<float>{30.0f, 40.0f, 0.0f};  // squared distance 2500
    EXPECT(go->IsAtDestination());
    go->m_destination = Point3D<float>{80.0f, 0.0f, 0.0f};  // squared distance 6400, not below it
    EXPECT(!go->IsAtDestination());
    go->m_destination = Point3D<float>{100.0f, 0.0f, 0.0f};
    EXPECT(!go->IsAtDestination());
    go->m_destination = Point3D<float>{0.0f, 0.0f, 1000.0f};  // height is ignored
    EXPECT(go->IsAtDestination());

    // IsAtDestination with a non-empty waypoint list: the target is the path end, not m_destination.
    const PFGInnerEdge* edge = reinterpret_cast<const PFGInnerEdge*>(&nodeMarker);  // stored, never dereferenced
    go->m_pfObject.m_path.push_back(edge);
    go->m_pfObject.m_pathEnd = Point3D<float>{10.0f, 0.0f, 0.0f};
    go->m_destination = Point3D<float>{500.0f, 0.0f, 0.0f};  // far away, but the path end is near
    EXPECT(go->IsAtDestination());
    go->m_pfObject.m_pathEnd = Point3D<float>{200.0f, 0.0f, 0.0f};
    go->m_destination = Point3D<float>{0.0f, 0.0f, 0.0f};  // near, but the path end is far
    EXPECT(!go->IsAtDestination());
    go->m_pfObject.m_path.clear();  // empty again: m_destination decides
    EXPECT(go->IsAtDestination());
    go->m_pfObject.m_path.~PathList();

    // SetRotation: m_rotation takes the three components and m_targetYaw takes Z. No visual is attached.
    std::memset(g_storage, 0, sizeof g_storage);
    go = Storage();
    go->SetRotation(Point3D<float>{0.1f, 0.2f, 0.3f});
    EXPECT(go->m_rotation.X == 0.1f && go->m_rotation.Y == 0.2f && go->m_rotation.Z == 0.3f);
    EXPECT(go->m_targetYaw == 0.3f);

    if (g_failures == 0) {
        std::printf("GameObject-5d2019-5: all checks passed\n");
        return 0;
    }
    std::printf("GameObject-5d2019-5: %d failures\n", g_failures);
    return 1;
}
