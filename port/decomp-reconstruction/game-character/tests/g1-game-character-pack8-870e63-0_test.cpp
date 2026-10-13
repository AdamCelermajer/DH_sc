// Test for batch g1-game-character-pack8-870e63-0.
//
// Covers the CharMenuTutorialMsg constructors, the CharPropTextField constructor defaults and its Update null path, the
// charproptext_internal::UpdatePropText mode dispatch, and the flag, probe and registration logic of CSSpawn (OnEvent, OnBlur,
// OnFocus, OnInit) and CSStunned (OnBlur, OnUpdate, OnInit). CSStunned::OnFocus is not run: it calls the IsPlayer virtual on the
// Character, and the test has no Character vtable.
//
// The engine functions the batch links to are stubs in this file. They record their calls and return the values the test sets.
// Characters, controllers, visual and physical objects, the CharAnim row and the other engine objects are zero-filled storage;
// the code under test reads and writes their named data members only. The state classes are constructed (they have no data).

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "dh2/CSSpawn.hpp"
#include "dh2/CSStunned.hpp"
#include "dh2/CharMenuTutorialMsg.hpp"
#include "dh2/CharPropTextField.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/charai_ai.hpp"
#include "dh2/v2Controller.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_pydata12.hpp"

namespace charproptext_internal {
void UpdatePropText(RenderFX* renderFx, Character* localCharacter, int propId, int kind, gameswf::character* target);
}  // namespace charproptext_internal

namespace {

int g_failures = 0;

void Expect(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// Zero-filled storage for the engine objects. None of them is constructed.
alignas(16) unsigned char g_charStorage[sizeof(Character)];
alignas(16) unsigned char g_ctrlStorage[sizeof(v2Controller)];
alignas(16) unsigned char g_visStorage[sizeof(VisualObject)];
alignas(16) unsigned char g_physStorage[sizeof(PhysicalObject)];
alignas(16) unsigned char g_renderStorage[sizeof(RenderFX)];
alignas(16) unsigned char g_switchStorage[sizeof(DebugSwitches)];
alignas(16) unsigned char g_animRow[sizeof(Structs::CharAnim)];
alignas(16) unsigned char g_pydataStorage[64];
char g_dummyName[1];
char g_dummyTarget[1];

// Call log and return values set by each scenario.
std::vector<std::string> g_switchProbes;
int g_loads = 0;
int g_initPhysical = 0;
int g_cancelSneaking = 0;
int g_setIdle = 0;
bool g_setIdleArg = true;
int g_pin = 0;
int g_unpin = 0;
int g_aiSetTarget = 0;
const void* g_aiTargetArg = nullptr;
bool g_aiForceArg = true;
int g_aiSync = 0;
int g_animSet = 0;
int g_animArg = -1;
int g_fadeCalls = 0;
float g_fadeSeconds = -1.0f;
int g_stance = 7;        // GetAnimStance return
int g_constant = 0;      // PyDataConstants::getConstant return
int g_propValue = 0;     // CharProperties::PROPS_GetIntWithBonus return
int g_gotoCalls = 0;
int g_gotoFrame = -1;
std::vector<std::string> g_htmlFormats;
std::vector<int> g_htmlValues;

struct RegCall {
    int state;
    int event;
    int next;
    int handler;  // 0 null, 1 CSM_Injured, 2 CSM_Interrupted, 3 other
};
std::vector<RegCall> g_regs;

int HandlerCode(bool (Character::*h)(int, void*, int, int&)) {
    if (h == nullptr) {
        return 0;
    }
    if (h == &Character::CSM_Injured) {
        return 1;
    }
    if (h == &Character::CSM_Interrupted) {
        return 2;
    }
    return 3;
}

void ResetLog() {
    g_switchProbes.clear();
    g_loads = 0;
    g_initPhysical = 0;
    g_cancelSneaking = 0;
    g_setIdle = 0;
    g_pin = 0;
    g_unpin = 0;
    g_aiSetTarget = 0;
    g_aiTargetArg = nullptr;
    g_aiForceArg = true;
    g_aiSync = 0;
    g_animSet = 0;
    g_animArg = -1;
    g_fadeCalls = 0;
    g_fadeSeconds = -1.0f;
    g_gotoCalls = 0;
    g_gotoFrame = -1;
    g_htmlFormats.clear();
    g_htmlValues.clear();
    g_regs.clear();
}

// A zero-filled Character with its controller, visual and physical objects attached.
Character* MakeCharacter(unsigned int flags) {
    std::memset(g_charStorage, 0, sizeof(g_charStorage));
    std::memset(g_ctrlStorage, 0, sizeof(g_ctrlStorage));
    auto* c = reinterpret_cast<Character*>(g_charStorage);
    c->m_controller = reinterpret_cast<v2Controller*>(g_ctrlStorage);
    c->m_visualObject = reinterpret_cast<VisualObject*>(g_visStorage);
    c->m_physicalObject = reinterpret_cast<PhysicalObject*>(g_physStorage);
    c->unk_1440 = 2.5f;
    c->m_stateFlags = flags;
    c->m_statusFlags = 0;
    return c;
}

Structs::CharAnim* AnimRow() {
    std::memset(g_animRow, 0, sizeof(g_animRow));
    return reinterpret_cast<Structs::CharAnim*>(g_animRow);
}

char kInteractive[] = "is_interactive";
char kOther[] = "something_else";

}  // namespace

// ---- stubs for the engine functions the batch links to ----------------------------------------------------------------

CharState::~CharState() {}

DebugSwitches* DebugSwitches::GetInstance() { return reinterpret_cast<DebugSwitches*>(g_switchStorage); }
void DebugSwitches::load() { ++g_loads; }
bool DebugSwitches::GetSwitch(const std::string& name) {
    g_switchProbes.push_back(name);
    return false;
}

void Character::InitPhysicalObject() { ++g_initPhysical; }
void Character::CancelSneaking() { ++g_cancelSneaking; }
const Structs::CharAnim* Character::GetCharAnimTable() const {
    return reinterpret_cast<const Structs::CharAnim*>(g_animRow);
}
int Character::GetAnimStance() const { return g_stance; }
bool Character::CSM_Injured(int, void*, int, int&) { return false; }
bool Character::CSM_Interrupted(int, void*, int, int&) { return false; }

void CharStateMachine::SM_RegisterEvent(int stateId, int eventId, int nextState,
                                        bool (Character::*handler)(int, void*, int, int&)) {
    g_regs.push_back(RegCall{stateId, eventId, nextState, HandlerCode(handler)});
}
int CharStateMachine::SM_SetIdleState(bool flag) {
    ++g_setIdle;
    g_setIdleArg = flag;
    return 0;
}

void CharAnimator::ANIM_Set(int animId) {
    ++g_animSet;
    g_animArg = animId;
}

void CharAI::AI_SetTarget(GameObject* target, bool force) {
    ++g_aiSetTarget;
    g_aiTargetArg = target;
    g_aiForceArg = force;
}
void CharAI::AI_SyncLastTarget() { ++g_aiSync; }

void VisualObject::StartFadeIn(float seconds) {
    ++g_fadeCalls;
    g_fadeSeconds = seconds;
}

void PhysicalObject::pin() { ++g_pin; }
void PhysicalObject::unpin() { ++g_unpin; }

PyDataConstants* ApplicationGetPyDataConstants() { return reinterpret_cast<PyDataConstants*>(g_pydataStorage); }
int PyDataConstants::getConstant(const char*, const char*) const { return g_constant; }

int CharProperties::PROPS_GetIntWithBonus(int, bool) const { return g_propValue; }

void RenderFX::FormatHTML(gameswf::character*, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int value = va_arg(args, int);
    va_end(args);
    g_htmlFormats.push_back(format);
    g_htmlValues.push_back(value);
}

int GameSWFUtils::GotoFrame(gameswf::character*, int frame) {
    ++g_gotoCalls;
    g_gotoFrame = frame;
    return 0;
}

// ---- checks -------------------------------------------------------------------------------------------------------------

namespace {

void TestCharMenuTutorialMsg() {
    std::string frame = "frame_a";
    std::string menu = "menu_b";
    CharMenuTutorialMsg msg(42, frame, menu);
    Expect(msg.m_stringId == 42, "CharMenuTutorialMsg ctor sets the id");
    Expect(msg.m_frame == "frame_a", "CharMenuTutorialMsg ctor copies the frame");
    Expect(msg.m_menu == "menu_b", "CharMenuTutorialMsg ctor copies the menu");

    CharMenuTutorialMsg copy(msg);
    Expect(copy.m_stringId == 42 && copy.m_frame == "frame_a" && copy.m_menu == "menu_b",
           "CharMenuTutorialMsg copy ctor copies all three fields");
    msg.m_frame = "changed";
    Expect(copy.m_frame == "frame_a", "CharMenuTutorialMsg copy does not follow later changes to the source");
}

void TestCharPropTextFieldDefaults() {
    auto* source = reinterpret_cast<gameswf::character*>(g_dummyName);
    auto* target = reinterpret_cast<gameswf::character*>(g_dummyTarget);

    CharPropTextField fallback(source, nullptr);
    Expect(fallback.m_propId == -1, "CharPropTextField ctor leaves the property id at -1 (name parse partial)");
    Expect(fallback.m_kind == '_', "CharPropTextField ctor starts the mode at '_'");
    Expect(fallback.m_target == source, "CharPropTextField target defaults to the source when null");

    CharPropTextField explicitTarget(source, target);
    Expect(explicitTarget.m_target == target, "CharPropTextField keeps an explicit target");

    // Update on the partial path: the local character is not fetched, so the dispatch returns before any output.
    ResetLog();
    auto* renderFx = reinterpret_cast<RenderFX*>(g_renderStorage);
    std::memset(g_renderStorage, 0, sizeof(g_renderStorage));
    fallback.Update(renderFx);
    Expect(g_htmlValues.empty() && g_gotoCalls == 0, "CharPropTextField::Update does nothing on the partial path");
}

void TestUpdatePropTextDispatch() {
    auto* renderFx = reinterpret_cast<RenderFX*>(g_renderStorage);
    auto* target = reinterpret_cast<gameswf::character*>(g_dummyTarget);
    Character* local = MakeCharacter(0);

    ResetLog();
    g_propValue = 5;
    charproptext_internal::UpdatePropText(renderFx, local, 10, '_', target);
    Expect(g_htmlFormats.size() == 1 && g_htmlFormats[0] == "%d" && g_htmlValues[0] == 5, "mode '_' prints the value with %d");

    ResetLog();
    g_propValue = 50;
    charproptext_internal::UpdatePropText(renderFx, local, 10, 'p', target);
    Expect(g_htmlFormats.size() == 1 && g_htmlFormats[0] == "%d%%" && g_htmlValues[0] == 50, "mode 'p' prints the value with %d%%");

    ResetLog();
    g_propValue = 3;
    charproptext_internal::UpdatePropText(renderFx, local, 10, 'i', target);
    Expect(g_gotoCalls == 1 && g_gotoFrame == 3 && g_htmlValues.empty(), "mode 'i' goes to the frame with the value");

    ResetLog();
    g_propValue = -1;
    charproptext_internal::UpdatePropText(renderFx, local, 10, 'i', target);
    Expect(g_gotoCalls == 0, "mode 'i' with value -1 does not move the frame");

    ResetLog();
    g_propValue = 7;
    charproptext_internal::UpdatePropText(renderFx, local, 10, 'b', target);
    Expect(g_htmlFormats.size() == 1 && g_htmlValues[0] == 7, "mode 'b' still runs the text step (colour step is partial)");

    ResetLog();
    g_propValue = 1500;
    charproptext_internal::UpdatePropText(renderFx, local, 10, '_', target);
    Expect(g_htmlValues.empty(), "value above 1000 takes the partial text path and prints nothing");

    ResetLog();
    charproptext_internal::UpdatePropText(renderFx, local, 10, 'x', target);
    Expect(g_htmlValues.empty() && g_gotoCalls == 0, "unknown mode returns without output");

    ResetLog();
    g_propValue = 5;
    charproptext_internal::UpdatePropText(renderFx, local, -1, '_', target);
    Expect(g_htmlValues.empty(), "property id -1 returns without output");

    ResetLog();
    charproptext_internal::UpdatePropText(renderFx, nullptr, 10, '_', target);
    Expect(g_htmlValues.empty(), "null local character returns without output");
}

void TestCSSpawnEventsAndBlur() {
    CSSpawn spawn;

    // Event 40 with "is_interactive" sets the interactive bit and runs InitPhysicalObject.
    Character* c = MakeCharacter(0);
    ResetLog();
    spawn.OnEvent(1, c, c, 40, kInteractive);
    Expect((c->m_stateFlags & 0x2000u) != 0 && g_initPhysical == 1, "CSSpawn event 40 sets 0x2000 and runs InitPhysicalObject");

    // Other data or other event ids leave the character alone.
    c = MakeCharacter(0);
    ResetLog();
    spawn.OnEvent(1, c, c, 40, kOther);
    spawn.OnEvent(1, c, c, 41, kInteractive);
    Expect(c->m_stateFlags == 0 && g_initPhysical == 0, "CSSpawn ignores other data and other events");

    // OnBlur runs InitPhysicalObject only when the interactive bit is clear.
    c = MakeCharacter(0);
    ResetLog();
    spawn.OnBlur(1, c, c, 2);
    Expect(g_initPhysical == 1 && !g_switchProbes.empty() && g_switchProbes[0] == "isTracingCharState",
           "CSSpawn::OnBlur probes the switch and runs InitPhysicalObject when clear");

    c = MakeCharacter(0x2000u);
    ResetLog();
    spawn.OnBlur(1, c, c, 2);
    Expect(g_initPhysical == 0, "CSSpawn::OnBlur skips InitPhysicalObject when the interactive bit is set");
}

void TestCSSpawnFocus() {
    CSSpawn spawn;
    Structs::CharAnim* row = AnimRow();
    row->m_w32 = 42;
    row->m_w35 = 43;

    // Focus from another state: flags become 0x241, the interactive bit is dropped, no stance (constant 0).
    Character* c = MakeCharacter(0x2000u);
    ResetLog();
    g_constant = 0;
    spawn.OnFocus(1, c, c, 3, 9, nullptr);
    Expect(c->m_stateFlags == 0x241u, "CSSpawn::OnFocus sets 0x241 and drops the interactive bit from state 3");
    Expect(g_animSet == 1 && g_animArg == 42, "CSSpawn::OnFocus sets the CharAnim m_w32 animation");
    Expect(g_aiSetTarget == 1 && g_aiTargetArg == nullptr && g_aiForceArg == false && g_aiSync == 1,
           "CSSpawn::OnFocus clears the AI target and syncs the last target");
    Expect(g_cancelSneaking == 1, "CSSpawn::OnFocus cancels sneaking");
    Expect(g_fadeCalls == 1 && g_fadeSeconds == 2.5f, "CSSpawn::OnFocus fades in the visual with unk_1440");
    Expect(g_switchProbes.size() == 2 && g_switchProbes[0] == "isTracingCharState" && g_switchProbes[1] == "isTracingCSSpawn",
           "CSSpawn::OnFocus probes the two switches in order");

    // Focus from PreSpawn (17) keeps the interactive bit.
    c = MakeCharacter(0x2000u);
    ResetLog();
    spawn.OnFocus(1, c, c, 17, 9, nullptr);
    Expect(c->m_stateFlags == 0x2241u, "CSSpawn::OnFocus from state 17 keeps the interactive bit");

    // Stance: the constant's bit 0 adds the character's stance to the animation.
    c = MakeCharacter(0);
    ResetLog();
    g_constant = 1;
    spawn.OnFocus(1, c, c, 3, 9, nullptr);
    Expect(g_animArg == 42 + 7, "CSSpawn::OnFocus adds GetAnimStance when the stance bit is set");
    g_constant = 0;

    // No visual object: no fade.
    c = MakeCharacter(0);
    c->m_visualObject = nullptr;
    ResetLog();
    spawn.OnFocus(1, c, c, 3, 9, nullptr);
    Expect(g_fadeCalls == 0, "CSSpawn::OnFocus skips the fade without a visual object");
}

void TestCSSpawnInit() {
    CSSpawn spawn;
    Character* c = MakeCharacter(0);
    ResetLog();
    spawn.OnInit(17, c, c);
    const std::vector<RegCall> expected = {
        {17, 34, 3, 0}, {17, 50008, 12, 0}, {17, 50010, 11, 1}, {17, 50011, 10, 2}, {17, 50012, 9, 2},
    };
    bool same = g_regs.size() == expected.size();
    for (std::size_t i = 0; same && i < expected.size(); ++i) {
        same = g_regs[i].state == expected[i].state && g_regs[i].event == expected[i].event &&
               g_regs[i].next == expected[i].next && g_regs[i].handler == expected[i].handler;
    }
    Expect(same, "CSSpawn::OnInit registers the five events in order");
}

void TestCSStunned() {
    CSStunned stun;
    Character* c = MakeCharacter(0);

    // OnBlur: clears the controller block, pins the physical object, probes the switch.
    c->m_controller->m_blocked = true;
    ResetLog();
    stun.OnBlur(1, c, c, 2);
    Expect(!c->m_controller->m_blocked && g_pin == 1 && g_unpin == 0, "CSStunned::OnBlur unblocks the controller and pins");
    Expect(!g_switchProbes.empty() && g_switchProbes[0] == "isTracingCharState", "CSStunned::OnBlur probes the switch");

    // OnUpdate: with the stunned status bit clear, the stun ends (idle state, flag 0). With it set, nothing happens.
    c = MakeCharacter(0);
    c->m_statusFlags = 0;
    ResetLog();
    stun.OnUpdate(1, c, c);
    Expect(g_setIdle == 1 && g_setIdleArg == false, "CSStunned::OnUpdate sets the idle state when not stunned");
    Expect(!g_switchProbes.empty() && g_switchProbes[0] == "isTracingCSStunned", "CSStunned::OnUpdate probes isTracingCSStunned");

    c = MakeCharacter(0);
    c->m_statusFlags = 2;
    ResetLog();
    stun.OnUpdate(1, c, c);
    Expect(g_setIdle == 0 && g_switchProbes.empty(), "CSStunned::OnUpdate does nothing while stunned");

    // OnInit: three events.
    c = MakeCharacter(0);
    ResetLog();
    stun.OnInit(20, c, c);
    const std::vector<RegCall> expected = {{20, 50008, 12, 0}, {20, 50010, 11, 1}, {20, 50011, 10, 2}};
    bool same = g_regs.size() == expected.size();
    for (std::size_t i = 0; same && i < expected.size(); ++i) {
        same = g_regs[i].state == expected[i].state && g_regs[i].event == expected[i].event &&
               g_regs[i].next == expected[i].next && g_regs[i].handler == expected[i].handler;
    }
    Expect(same, "CSStunned::OnInit registers the three events in order");
}

}  // namespace

int main() {
    TestCharMenuTutorialMsg();
    TestCharPropTextFieldDefaults();
    TestUpdatePropTextDispatch();
    TestCSSpawnEventsAndBlur();
    TestCSSpawnFocus();
    TestCSSpawnInit();
    TestCSStunned();
    if (g_failures == 0) {
        std::printf("all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
