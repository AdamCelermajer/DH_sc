// Test for batch g1-game-world-CameraTarget-255099-0.
//
// The CameraTarget translation unit is linked with stubs for the engine code it calls: the CameraBase
// constructor, destructor and virtuals, GameObject::GetCameraAnchorPosition, Application::GetDt and
// ApplicationGetInstance, and the ObjectManager lookup with its ObjectHandle conversions. The stubs read
// a few globals set here. The two globals the unit reads (Vec3f_Origin and the ObjectManager instance)
// are defined here as well. The camera-node seams in the unit are neutral, so the node position is never
// observed. The checks cover the transition countdown, the target bookkeeping, the offset and ghost
// arithmetic, and the damping arithmetic against the origin that the neutral seam reports.

#include <cmath>
#include <cstdio>

#include "dh2/Application.hpp"
#include "dh2/CameraTarget.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/global/Vec3f_Origin.hpp"
#include "dh2/global/ctrl_camera.hpp"

namespace {

int g_failures = 0;
int g_dt = 0;                                  // Application::GetDt result, ms
int g_baseUpdates = 0;                         // CameraBase::Update calls
Point3D<float> g_centerOffset{0.0f, 0.0f, 0.0f};
Point3D<float> g_anchor{10.0f, 20.0f, 30.0f};  // every GameObject's camera anchor in these checks
bool g_lookupFound = false;
GameObject* g_lookupObject = nullptr;

// Storage whose address stands for objects and nodes. The stubs never read through it.
char g_fakeManager[16];
char g_fakeBase[16];
char g_fakeNode[16];
char g_objA[16];
char g_objB[16];
char g_objC[16];
alignas(Application) unsigned char g_appStorage[sizeof(Application)];

#define EXPECT(cond)                                                    \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::printf("FAIL line %d: %s\n", __LINE__, #cond);         \
            ++g_failures;                                               \
        }                                                               \
    } while (0)

bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

bool sameVec(const Point3D<float>& a, const Point3D<float>& b) {
    return near(a.X, b.X) && near(a.Y, b.Y) && near(a.Z, b.Z);
}

GameObject* asObject(char* storage) { return reinterpret_cast<GameObject*>(storage); }

glitch::scene::ISceneNode* fakeNode() { return reinterpret_cast<glitch::scene::ISceneNode*>(g_fakeNode); }

// Exposes the protected fields the checks need.
class TestCamera : public CameraTarget {
public:
    GameObject* target() const { return m_target; }
    const Point3D<float>& from() const { return m_transitionFrom; }
    int totalMs() const { return m_transitionTotalMs; }
    int remainingMs() const { return m_transitionRemainingMs; }
    bool offsetEnabled() const { return m_offsetEnabled; }
    bool dampingEnabled() const { return m_dampingEnabled; }
    float dampingRatio() const { return m_dampingRatio; }
    const Point3D<float>& velocity() const { return m_dampingVelocity; }
    const Point3D<float>& ghost() const { return m_ghostOffset; }
    void setNode(glitch::scene::ISceneNode* node) { m_node = node; }
    void setGhost(const Point3D<float>& offset) { m_ghostOffset = offset; }
    void setOffsetEnabled(bool on) { m_offsetEnabled = on; }
};

void testDefaults() {
    TestCamera cam;
    const Point3D<float> zero{0.0f, 0.0f, 0.0f};
    EXPECT(cam.dampingEnabled());
    EXPECT(!cam.offsetEnabled());
    EXPECT(near(cam.dampingRatio(), 0.7f));
    EXPECT(sameVec(cam.velocity(), zero));
    EXPECT(cam.remainingMs() == 0 && cam.totalMs() == 0);
    EXPECT(cam.target() == nullptr);
    EXPECT(sameVec(cam.ghost(), zero));
}

void testGhostAndOffset() {
    TestCamera cam;
    cam.setGhost(Point3D<float>{1.0f, 2.0f, 3.0f});
    Point3D<float> p{10.0f, 20.0f, 30.0f};
    EXPECT(cam.HandleGhostCam(p));
    EXPECT(sameVec(p, Point3D<float>{11.0f, 22.0f, 33.0f}));

    // Offset disabled by default: the position is left alone and the call reports false.
    Point3D<float> q{10.0f, 20.0f, 30.0f};
    EXPECT(!cam.HandleOffset(q));
    EXPECT(sameVec(q, Point3D<float>{10.0f, 20.0f, 30.0f}));

    cam.setOffsetEnabled(true);
    g_centerOffset = Point3D<float>{5.0f, 6.0f, 7.0f};
    EXPECT(cam.HandleOffset(q));
    EXPECT(sameVec(q, Point3D<float>{15.0f, 26.0f, 37.0f}));
}

void testDampingSetters() {
    TestCamera cam;
    cam.EnableDamping(false);
    EXPECT(!cam.dampingEnabled());
    cam.SetDampingRatio(0.25f);
    EXPECT(near(cam.dampingRatio(), 0.25f));
    cam.EnableDamping(true);
    EXPECT(cam.dampingEnabled());
    EXPECT(sameVec(cam.velocity(), Point3D<float>{0.0f, 0.0f, 0.0f}));
}

void testSetTargetBookkeeping() {
    TestCamera cam;
    const Point3D<float> zero{0.0f, 0.0f, 0.0f};
    cam.setGhost(Point3D<float>{1.0f, 1.0f, 1.0f});

    // Zero transition: no blend, the countdown stays at zero, the ghost offset is cleared.
    cam.SetTarget(asObject(g_objA), 0);
    EXPECT(cam.target() == asObject(g_objA));
    EXPECT(cam.remainingMs() == 0 && cam.totalMs() == 0);
    EXPECT(sameVec(cam.from(), zero));
    EXPECT(sameVec(cam.ghost(), zero));

    // Timed transition: the blend starts from the old target's anchor and the countdown is set to the time.
    cam.SetTarget(asObject(g_objB), 500);
    EXPECT(cam.target() == asObject(g_objB));
    EXPECT(cam.totalMs() == 500 && cam.remainingMs() == 500);
    EXPECT(sameVec(cam.from(), g_anchor));

    // A null object is ignored.
    cam.SetTarget(static_cast<GameObject*>(nullptr), 100);
    EXPECT(cam.target() == asObject(g_objB));
    EXPECT(cam.remainingMs() == 500);
}

void testSetTargetByName() {
    TestCamera cam;
    dword_99F764 = reinterpret_cast<ObjectManager*>(g_fakeManager);

    g_lookupFound = false;
    cam.SetTarget("missing", 100);
    EXPECT(cam.target() == nullptr);
    EXPECT(cam.remainingMs() == 0);

    g_lookupFound = true;
    g_lookupObject = asObject(g_objC);
    cam.SetTarget("found", 0);
    EXPECT(cam.target() == asObject(g_objC));

    g_lookupFound = false;
    g_lookupObject = nullptr;
}

void testTransitionCountdown() {
    TestCamera cam;
    cam.setNode(fakeNode());
    cam.SetTarget(asObject(g_objA), 500);  // no old target: blend from the origin, 500 ms

    g_dt = 200;
    EXPECT(cam.HandleTransition());
    EXPECT(cam.remainingMs() == 300);

    g_dt = 300;
    EXPECT(cam.HandleTransition());  // reaches zero: the node snaps to the anchor
    EXPECT(cam.remainingMs() == 0);

    g_dt = 300;
    EXPECT(cam.HandleTransition());  // past zero, still the snap branch
    EXPECT(cam.remainingMs() == -300);

    EXPECT(!cam.HandleTransition());  // idle once the countdown is negative
    EXPECT(cam.remainingMs() == -300);

    // Without a node the transition does not run and the countdown is left alone.
    cam.setNode(nullptr);
    cam.SetTarget(asObject(g_objB), 100);
    EXPECT(!cam.HandleTransition());
    EXPECT(cam.remainingMs() == 100);
}

void testUpdateTransitionGate() {
    TestCamera cam;
    cam.setNode(fakeNode());
    const int before = g_baseUpdates;

    // A running transition returns from Update before CameraBase::Update is called.
    cam.SetTarget(asObject(g_objA), 0);  // countdown at zero: the next HandleTransition snaps
    g_dt = 16;
    cam.Update();
    EXPECT(g_baseUpdates == before);

    // Idle transition: the target path runs and CameraBase::Update is called once.
    cam.Update();
    EXPECT(g_baseUpdates == before + 1);

    // No node: the target path is skipped, but CameraBase::Update still runs.
    TestCamera bare;
    bare.Update();
    EXPECT(g_baseUpdates == before + 2);
}

void testDamping() {
    TestCamera cam;
    Point3D<float> p{10.0f, 0.0f, 0.0f};

    // Damping on but no node: false, position unchanged.
    EXPECT(!cam.HandleDamping(p));
    EXPECT(sameVec(p, Point3D<float>{10.0f, 0.0f, 0.0f}));

    cam.setNode(fakeNode());
    cam.EnableDamping(false);
    EXPECT(!cam.HandleDamping(p));

    // Damping on with ratio 1 and a 1000 ms frame. The neutral node seam reports the origin, so the
    // velocity becomes (10 + 0 - 0) * 1 = 10 and the position becomes 1.0 s * 10 + 0 = 10.
    cam.EnableDamping(true);
    cam.SetDampingRatio(1.0f);
    g_dt = 1000;
    EXPECT(cam.HandleDamping(p));
    EXPECT(near(cam.velocity().X, 10.0f));
    EXPECT(sameVec(p, Point3D<float>{10.0f, 0.0f, 0.0f}));
}

}  // namespace

// Stubs for the engine functions the unit calls, and the two globals it reads.

Point3D<float> Vec3f_Origin{0.0f, 0.0f, 0.0f};
static ObjectManager* g_test_dword_99F764 = nullptr;
ObjectManager*& dword_99F764 = g_test_dword_99F764;  // link-1: the header declares a reference to the Application member (game-app/src/link1_ApplicationWords.cpp)

Application& ApplicationGetInstance() { return *reinterpret_cast<Application*>(g_appStorage); }
int Application::GetDt() { return g_dt; }

CameraBase::CameraBase() : m_node(nullptr), m_camera(nullptr) {}
CameraBase::~CameraBase() {}
void CameraBase::Activated() {}
void CameraBase::Deactivated() {}
void CameraBase::Update() { ++g_baseUpdates; }
bool CameraBase::GetCenterOffset(Point3D<float>& offset, float) const {
    offset = g_centerOffset;
    return true;
}

const Point3D<float>& GameObject::GetCameraAnchorPosition() const { return g_anchor; }

ObjectHandle::ObjectHandle() : m_id(0), m_cached(nullptr), m_generation(0) {}
ObjectBase* ObjectHandle::GetObject(bool) {
    return g_lookupFound ? reinterpret_cast<ObjectBase*>(g_fakeBase) : nullptr;
}
ObjectHandle::operator GameObject*() { return g_lookupObject; }
ObjectHandle ObjectManager::GetObjectByName(char const*, int, bool, char const*) {
    ObjectHandle handle;
    return handle;
}

int main() {
    testDefaults();
    testGhostAndOffset();
    testDampingSetters();
    testSetTargetBookkeeping();
    testSetTargetByName();
    testTransitionCountdown();
    testUpdateTransitionGate();
    testDamping();
    if (g_failures == 0) {
        std::printf("CameraTarget tests passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
