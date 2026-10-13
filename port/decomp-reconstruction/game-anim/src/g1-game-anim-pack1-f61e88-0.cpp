// Batch g1-game-anim-pack1-f61e88-0: the destructors, constructors and Update of v2GamepadController, the
// destructors and constructors of v2HudController, and the destructors, constructors and Update of v2NetController.
//
// The v2Controller base (dh2/v2Controller.hpp) has no standalone constructor symbol in the original: each derived
// constructor writes the v2Controller fields inline. The C++ base constructor v2Controller(v2Controllable*) is still
// named in the initializer lists below, and nothing defines it yet, so the constructors are partial for that reason.
// The field stores themselves are written here, as the original does them.

#include <cstdio>
#include <cstdlib>

#include "dh2/InputManager.hpp"
#include "dh2/v2GamepadController.hpp"
#include "dh2/v2HudController.hpp"
#include "dh2/v2NetController.hpp"

// Engine assert level, defined in core-platform/src/memory.cpp (declared the same way by the other game-anim batches).
extern int gAssertLevel;

namespace {

// The ASSERT(controllable) that every controller constructor runs (v2Controller.h:68). gAssertLevel 2 stops the
// program, because the original stores 0 to address 0; gAssertLevel 1 prints the message and continues.
void AssertControllable(v2Controllable* controllable) {
    if (controllable) {
        return;
    }
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "controllable",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Controllers\\/v2Controller.h", 68);
    }
}

// The v2Controller fields the original derived constructors store inline: +0x04 m_controllable, +0x08 m_blocked = 0,
// +0x09 unk_009 = 0, +0x0a unk_00a = 0 and +0x0c m_character = 0.
void InitControllerFields(v2Controller& self, v2Controllable* controllable) {
    self.m_controllable = controllable;
    self.m_blocked = false;
    self.unk_009 = false;
    self.unk_00a = false;
    self.m_character = nullptr;
}

}  // namespace

namespace v2gamepad_detail {

// The threshold every flagged button group of v2GamepadController::Update compares its value with:
// (lo + hi + 1.0f) * 0.5f in single precision, where lo and hi are that group's two calibration floats.
float ButtonThreshold(float lo, float hi) {
    return (lo + hi + 1.0f) * 0.5f;
}

// Result of one button group of Update. kReleased raises the group's event with state 0, kPressed with state 1.
enum class ButtonEvent { kNone, kReleased, kPressed };

// The edge rule each group applies. Below the threshold, state 0 is raised only when the group's pressed flag (+0x10
// of the group) is set. At or above the threshold, state 1 is raised only when the flag is clear. Update does not
// write the flag in these groups, so the caller stores nothing from this result.
ButtonEvent ButtonEdge(float value, float threshold, bool flag) {
    if (value < threshold) {
        return flag ? ButtonEvent::kReleased : ButtonEvent::kNone;
    }
    return flag ? ButtonEvent::kNone : ButtonEvent::kPressed;
}

}  // namespace v2gamepad_detail

// ---------------------------------------------------------------------------------------------------------------------
// v2GamepadController

// 0x406920 _ZN19v2GamepadControllerD2Ev (base-object destructor)
// 0x406924 _ZN19v2GamepadControllerD1Ev (complete-object destructor)
// Both are a single BX LR in the original (asm verified for D1). The implicit base destructor is empty too. The two
// symbols share this body, so it is defined once here.
v2GamepadController::~v2GamepadController() {}

// 0x40695c _ZN19v2GamepadControllerD0Ev (deleting destructor)
// skipped: compiler-generated. The original calls D1 (0x406924) and then CustomFree(this) (asm verified). The C++
// delete expression runs the destructor and then the deallocation, so no body is written here.

// 0x406978 _ZN19v2GamepadControllerC1EP14v2Controllablei (complete-object constructor)
// 0x406a64 _ZN19v2GamepadControllerC2EP14v2Controllablei (base-object constructor), same body, defined once here.
// partial: waits on the v2Controller(v2Controllable*) base constructor (see the file comment). The body does what the
// original does: the v2Controller field stores, the ASSERT(controllable) when the controllable is null, m_padIndex at
// +0x10, and unk_014 at +0x14..+0x1c zeroed. The vptr writes are compiler-managed.
v2GamepadController::v2GamepadController(v2Controllable* controllable, int padIndex)
    : v2Controller(controllable)
{
    InitControllerFields(*this, controllable);
    AssertControllable(controllable);
    m_padIndex = padIndex;
    unk_014.X = 0.0f;
    unk_014.Y = 0.0f;
    unk_014.Z = 0.0f;
}

// 0x406c2c _ZN19v2GamepadController6UpdateEv
// partial: the pad lookup is rebuilt, the rest of the frame is not. The remainder of Update is missing:
//   - GamepadTweaker::UpdateButtonMappings on Application's m_gamepadTweaker (+0x68 of Singleton<Application>::s_inst,
//     0x99f794). The instance accessor is declared as ApplicationGetInstance() (dh2/global/ctrl_camera.hpp), and the
//     member is in Application.hpp. dh2/GamepadTweaker.hpp does not compile yet: it includes
//     glitch-debugger/CTweakable.hpp, which is not in the tree. The call waits on that header.
//   - every stick, button and zoom read. They come from the Gamepad object (fields at +0x0c .. +0x614). dh2/Gamepad.hpp
//     is absent, so the layout is not declared. The stick vectors, the map-camera move and the zoom follow the reads.
//   - the 26 flagged button groups and their EventManager raises (type 7, and type 8 for the zoom), which use the
//     rule in v2gamepad_detail::ButtonEdge and the threshold in v2gamepad_detail::ButtonThreshold.
//   - the Cmd_* calls that follow from those reads, and the CameraBase and Character state of the stick vector.
// Until those land, Update returns after the lookup.
void v2GamepadController::Update() {
    InputManager* input = InputManager::GetInstance();
    if (m_padIndex >= input->GetNumGamepads()) {
        return;
    }
    // +0x10 m_padIndex: a negative index selects the first connected pad, any other index selects that pad.
    Gamepad* pad = (m_padIndex < 0) ? input->GetFirstConnectedGamepad() : input->GetGamepad(m_padIndex);
    if (!pad) {
        return;
    }
    // Missing: the rest of the frame, listed above. pad is not dereferenced yet.
}

// ---------------------------------------------------------------------------------------------------------------------
// v2HudController

// 0x4088ec _ZN15v2HudControllerD2Ev (base-object destructor)
// 0x4088f0 _ZN15v2HudControllerD1Ev (complete-object destructor)
// One BX LR each in the original (asm verified for D1). Same body, defined once here.
v2HudController::~v2HudController() {}

// 0x408914 _ZN15v2HudControllerD0Ev (deleting destructor)
// skipped: compiler-generated. The original calls D1 (0x4088f0) and then CustomFree(this) (asm verified). The C++
// delete expression covers it, so no body is written here.

// 0x408930 _ZN15v2HudControllerC1EP14v2Controllable (complete-object constructor)
// 0x408a04 _ZN15v2HudControllerC2EP14v2Controllable (base-object constructor), same body, defined once here.
// partial: waits on the v2Controller(v2Controllable*) base constructor. v2HudController adds no members, so the body is
// the v2Controller field stores and the ASSERT(controllable).
v2HudController::v2HudController(v2Controllable* controllable)
    : v2Controller(controllable)
{
    InitControllerFields(*this, controllable);
    AssertControllable(controllable);
}

// ---------------------------------------------------------------------------------------------------------------------
// v2NetController

// 0x4091d0 _ZN15v2NetControllerD2Ev (base-object destructor)
// 0x4091d4 _ZN15v2NetControllerD1Ev (complete-object destructor)
// One BX LR each in the original (asm verified for D1). Same body, defined once here.
v2NetController::~v2NetController() {}

// 0x4091d8 _ZN15v2NetControllerD0Ev (deleting destructor)
// skipped: compiler-generated. The original calls D1 (0x4091d4) and then CustomFree(this) (asm verified). The C++
// delete expression covers it, so no body is written here.

// 0x4091f4 _ZN15v2NetControllerC1EP14v2Controllable (complete-object constructor)
// 0x4092c8 _ZN15v2NetControllerC2EP14v2Controllable (base-object constructor), same body, defined once here.
// partial: waits on the v2Controller(v2Controllable*) base constructor, as the other controller constructors do.
// v2NetController adds no members, so the body is the v2Controller field stores and the ASSERT(controllable).
v2NetController::v2NetController(v2Controllable* controllable)
    : v2Controller(controllable)
{
    InitControllerFields(*this, controllable);
    AssertControllable(controllable);
}

// 0x40939c _ZN15v2NetController6UpdateEv
// partial: not rebuilt, so Update does nothing for now. The original, in order:
//   1. GetOnline() (symbol _Z9GetOnlinev) gates the body on the COnline byte at +0x05. No accessor is declared in the
//      tree, so this waits on it.
//   2. CMessaging: IsMessageReceived("CMsgControllerAction") and GetMessageFromQueue. The message fields are not
//      declared: the action byte at +0x50, the target network id (uint16) at +0x52, the sender byte at +0x54, and the
//      consumed flag written at +0x3c. No header for CMsgControllerAction exists.
//   3. The sender byte is compared with the controller's Character field at +0x108 (int32, not declared). Otherwise the
//      object is looked up by network id (ObjectManager::GetObjectHandleByNetworkId; dh2/ObjectManager.hpp is absent),
//      and a Character found there must have a PlayerManager record whose byte +0x66c is 1.
//   4. The action switch calls v2Controllable slots: 0 Ctrl_Attack(target), 1 Ctrl_BeginSkill(id), 2 Ctrl_EndSkill(id),
//      3 Character::SG_SetCurrentFaerie(id, -1) then Ctrl_BeginCast(true), 4 Ctrl_EndCast(true), 5 Ctrl_UseOOI(target,
//      when the object is found). Any other action runs the ASSERT at v2NetController.cpp:103.
// The switch is written as code once the message and object layouts are declared.
void v2NetController::Update() {}
