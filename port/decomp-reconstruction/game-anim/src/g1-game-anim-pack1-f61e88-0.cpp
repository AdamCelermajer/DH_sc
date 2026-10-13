// Batch g1-game-anim-pack1-f61e88-0: the destructors, constructors and Update of v2GamepadController, the
// destructors and constructors of v2HudController, and the destructors, constructors and Update of v2NetController.
//
// The v2Controller base constructor (the field stores and the ASSERT(controllable)) is v2Controller::v2Controller in
// game-anim/src/r2a-game-anim-pack2-aa7d76-0.cpp. The derived constructors below name it as their base.

#include "dh2/InputManager.hpp"
#include "dh2/v2GamepadController.hpp"
#include "dh2/v2HudController.hpp"
#include "dh2/v2NetController.hpp"

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
// The v2Controller base (field stores and ASSERT(controllable)) runs first, in v2Controller::v2Controller. Then m_padIndex
// at +0x10 and unk_014 at +0x14..+0x1c are set. The vptr writes are compiler-managed.
v2GamepadController::v2GamepadController(v2Controllable* controllable, int padIndex)
    : v2Controller(controllable)
{
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
// v2HudController adds no members, so the body is the v2Controller base constructor alone.
v2HudController::v2HudController(v2Controllable* controllable)
    : v2Controller(controllable)
{
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
// v2NetController adds no members, so the body is the v2Controller base constructor alone.
v2NetController::v2NetController(v2Controllable* controllable)
    : v2Controller(controllable)
{
}

// 0x40939c _ZN15v2NetController6UpdateEv
// partial: not rebuilt, so Update does nothing for now. The original, in order:
//   1. GetOnline() gates the body on the COnline byte at +0x05. COnline::isOnlineGame_ is private with no accessor in
//      net-session-online/c_online.hpp (layout request: a public getter), and Application::IsOnlineGame (0x320668) is a
//      stub, so this gate is blocked.
//   2. GetMessaging(): IsMessageReceived("CMsgControllerAction") and GetMessageFromQueue. CMsgControllerAction in
//      net-session-online/c_msg_fixed.hpp keeps its payload opaque, so the action byte at +0x50, the target network id
//      (uint16) at +0x52 and the sender byte at +0x54 are not declared (layout request). The consumed flag written at
//      +0x3c is not declared in c_message.hpp either (layout request).
//   3. The sender byte is compared with Character unk_108 (+0x108, declared on ObjectBase). Otherwise the object is looked
//      up by network id (ObjectManager::GetObjectHandleByNetworkId, declared), and a Character found there that IsPlayer()
//      must have a PlayerManager record whose unk_66c (PlayerInfo +0x66c) is 1, otherwise Update returns early.
//   4. The action switch calls v2Controllable slots: 0 Ctrl_Attack(target), 1 Ctrl_BeginSkill(id), 2 Ctrl_EndSkill(id),
//      3 Character::SG_SetCurrentFaerie(id, -1) then Ctrl_BeginCast(true), 4 Ctrl_EndCast(true), 5 Ctrl_UseOOI(target,
//      when the object is found). Any other action runs the ASSERT at v2NetController.cpp:103.
// The switch is written as code once the online gate and the CMsgControllerAction payload fields are declared.
void v2NetController::Update() {}
