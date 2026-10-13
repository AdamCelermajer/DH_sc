// Test for batch g1-game-anim-AnimatedFX-6c1bfb-0: constructor defaults, the SetAnimFX field stores, speed,
// the loop countdown and end-of-loop flag/callback, and the material assert level. Returns non-zero when a
// check fails.
//
// The checks use no VisualObject (the paths that need one are not exercised here). The stubs below only exist
// so that the batch file links; none of them is reached by these checks.

#include <cstdio>

#include "core-platform/deps/game_types.hpp"
#include "dh2/AnimController.hpp"
#include "dh2/AnimatedFX.hpp"
#include "dh2/VisualObject.hpp"

// Global assert level read by AnimatedFX::ForceMaterial.
int gAssertLevel = 0;

// Link stubs for the non-virtual engine calls in the batch file (never reached here).
glitch::collada::ISceneNodeAnimator* AnimController::GetAnim(unsigned int) {
    return nullptr;
}

VisualObject::VisualObject(GameObject*, const std::string&, const std::string&) {}

void VisualObject::SetScaling(const Point3D<float>&) {}

// The stub constructor writes the vtable pointer, so the vtable must exist: these virtual functions make
// this translation unit the vtable's home (the destructor is the key function).
VisualObject::~VisualObject() {}
void VisualObject::Update() {}
void VisualObject::Draw() const {}

namespace {

int g_failures = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

int g_callbackCalls = 0;
AnimatedFX* g_callbackFx = nullptr;
void* g_callbackSetData = nullptr;

void RecordCallback(AnimatedFX* fx, void* setData) {
    ++g_callbackCalls;
    g_callbackFx = fx;
    g_callbackSetData = setData;
}

void TestConstructorDefaults() {
    AnimatedFX fx(42);
    CHECK(fx.unk_08 == 42);
    CHECK(fx.unk_1c == -1);
    CHECK(fx.m_speed == 1.0f);
    CHECK(fx.unk_24 == true);
    CHECK(fx.m_visual == nullptr);
    CHECK(fx.m_looping == false);
    CHECK(fx.m_loopCount == 0);
    CHECK(fx.m_setData == nullptr);
    CHECK(fx.m_endOfLoop == nullptr);
    CHECK(fx.unk_30 == false && fx.unk_31 == false && fx.unk_32 == false);
    CHECK(fx.unk_4c == false);
    CHECK(fx.GetAnimController() == nullptr);
    CHECK(fx.GetAnimator() == nullptr);
}

void TestSetAnimFXStoresFields() {
    AnimatedFX fx(1);
    unsigned char setData[4] = {0, 9, 9, 9};
    fx.SetAnimFX(true, false, true, 2.5f, 3, setData, &RecordCallback);
    CHECK(fx.unk_30 == true);
    CHECK(fx.unk_31 == false);
    CHECK(fx.unk_32 == true);
    CHECK(fx.m_speed == 2.5f);
    CHECK(fx.m_loopCount == 3);
    CHECK(fx.m_setData == setData);
    CHECK(fx.m_endOfLoop == &RecordCallback);
}

void TestSetSpeedWithoutVisual() {
    AnimatedFX fx(0);
    fx.SetSpeed(4.0f);
    CHECK(fx.m_speed == 4.0f);
}

void TestSetScaleWithoutVisual() {
    AnimatedFX fx(0);
    fx.unk_32 = true;
    fx.SetScale(Point3D<float>{1.0f, 2.0f, 3.0f});
    // Without a visual the original does not clear the scale flag either.
    CHECK(fx.unk_32 == true);
}

void TestSetLoopingStoresFlag() {
    AnimatedFX fx(0);
    fx.SetLooping(true);
    CHECK(fx.m_looping == true);
    fx.SetLooping(false);
    CHECK(fx.m_looping == false);
}

void TestLoopCountdown() {
    AnimatedFX fx(0);
    fx.SetAnimFX(false, false, false, 1.0f, 2, nullptr, nullptr);
    AnimatedFX::_CBLoop(nullptr, &fx);
    CHECK(fx.m_loopCount == 1);
    AnimatedFX::_CBLoop(nullptr, &fx);
    CHECK(fx.m_loopCount == 0);
}

void TestLoopEndFinishSetsFlag() {
    AnimatedFX fx(0);
    unsigned char setData[4] = {0, 0, 0, 0};
    fx.SetAnimFX(false, false, false, 1.0f, 0, setData, nullptr);
    fx.SetLooping(true);
    fx._HandleLoopEnd();
    CHECK(fx.m_looping == false);
    CHECK(setData[0] == 1);
    CHECK(fx.m_loopCount == 0);
}

void TestLoopEndCallback() {
    AnimatedFX fx(0);
    unsigned char setData[2] = {0, 0};
    g_callbackCalls = 0;
    g_callbackFx = nullptr;
    g_callbackSetData = nullptr;
    fx.SetAnimFX(false, false, false, 1.0f, 0, setData, &RecordCallback);
    fx._HandleLoopEnd();
    CHECK(g_callbackCalls == 1);
    CHECK(g_callbackFx == &fx);
    CHECK(g_callbackSetData == setData);
    CHECK(setData[0] == 1);
    CHECK(fx.m_endOfLoop == nullptr);
}

void TestLoopEndGateClosed() {
    AnimatedFX fx(0);
    unsigned char setData[1] = {0};
    fx.SetAnimFX(false, false, false, 0.0f, 0, setData, nullptr);
    fx._HandleLoopEnd();
    // Speed <= 0 and no callback: the original does nothing here.
    CHECK(setData[0] == 0);
    CHECK(fx.m_loopCount == 0);
}

void TestLoopEndNegativeCountIsEndless() {
    AnimatedFX fx(0);
    unsigned char setData[1] = {0};
    fx.SetAnimFX(false, false, false, 1.0f, -1, setData, nullptr);
    fx._HandleLoopEnd();
    CHECK(setData[0] == 0);
    CHECK(fx.m_loopCount == -1);
}

void TestForceMaterialLevelZeroIsSilent() {
    AnimatedFX fx(0);
    gAssertLevel = 0;
    fx.ForceMaterial(7);
    CHECK(gAssertLevel == 0);
}

}  // namespace

int main() {
    TestConstructorDefaults();
    TestSetAnimFXStoresFields();
    TestSetSpeedWithoutVisual();
    TestSetScaleWithoutVisual();
    TestSetLoopingStoresFlag();
    TestLoopCountdown();
    TestLoopEndFinishSetsFlag();
    TestLoopEndCallback();
    TestLoopEndGateClosed();
    TestLoopEndNegativeCountIsEndless();
    TestForceMaterialLevelZeroIsSilent();
    if (g_failures == 0) {
        std::printf("AnimatedFX batch g1-game-anim-AnimatedFX-6c1bfb-0: all checks passed\n");
        return 0;
    }
    std::printf("AnimatedFX batch g1-game-anim-AnimatedFX-6c1bfb-0: %d check(s) failed\n", g_failures);
    return 1;
}
