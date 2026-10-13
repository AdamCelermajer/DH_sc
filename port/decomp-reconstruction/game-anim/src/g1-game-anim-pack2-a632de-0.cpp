// v2PS3MoveController functions of libDungeonHunter2.so (game-anim batch g1-game-anim-pack2-a632de-0).
// Rebuilt from the Hex-Rays pseudocode in recovered/native/hexrays-all and checked against the ARM assembly
// (tools/decomp_asm.py --address 0x409bd8 lists every call; the Gamepad, MenuManager and EvGamepad accesses are
// read from it). Class layout and declarations: game/include/dh2/v2PS3MoveController.hpp.
//
// Status. The destructors and the constructors are rebuilt. The v2Controller base constructor they call is declared
// but not defined in the tree yet (the original writes the base inline). Update is partial. Its control flow is
// complete, but six reads need types the tree does not declare: the Gamepad control table, the cursor position in
// MenuManager, PFWorld::TranslateScreenToWorld, the player id in PlayerInfo, the EvGamepad event and the object list
// for the target search. Those reads are the PARTIAL seams in the anonymous namespace below. Until they are filled,
// every control reads as released with no latch, so Update issues no command.

#include "dh2/v2PS3MoveController.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

#include "core-platform/deps/game_types.hpp"  // Point2D, Point3D
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/InputManager.hpp"
#include "dh2/ObjectSearcher.hpp"

// Engine assert level, defined in core-platform/src/memory.cpp. Declared here as AnimController does, so the game
// sources do not pull in the engine memory header.
extern int gAssertLevel;

namespace {

// The pad controls Update reads, named by the call they drive. Each control is a float value, two float bounds
// (the press level is (bound1 + bound0 + 1.0f) * 0.5f) and, for most, a latch byte. The 32-bit Gamepad offsets are
// listed on each enumerator and in the Gamepad layout request.
enum PadControl : int {
    kPadPrimary,       // value +0x3cc, bounds +0x3d0 +0x3d4, no latch: cursor aim, target search, Cmd_HeadTo, Cmd_Attack / Cmd_UseOOI
    kPadMoveToClick,   // value +0x3d8, bounds +0x3e0 +0x3e4, latch +0x3e8: Cmd_Stop on release unless IsMovingToClick
    kPadAttack,        // value +0x34c, bounds +0x350 +0x354, no latch: Cmd_Attack(nullptr) while held
    kPadSkill0,        // value +0x338, bounds +0x340 +0x344, latch +0x348: skill slot 0, and EvGamepad action 2
    kPadSkill1,        // value +0x398, bounds +0x3a0 +0x3a4, latch +0x3a8: skill slot 1, and EvGamepad action 3
    kPadSkill2,        // value +0x378, bounds +0x380 +0x384, latch +0x388: skill slot 2, and EvGamepad action 1
    kPadEventAction0,  // value +0x358, bounds +0x360 +0x364, latch +0x368: EvGamepad action 0 only
    kPadMadness0,      // value +0x438, bounds +0x440 +0x444, latch +0x448: move madness, slot 0
    kPadMadness1,      // value +0x458, bounds +0x460 +0x464, latch +0x468: move madness, slot 1
    kPadMadness2,      // value +0x478, bounds +0x480 +0x484, latch +0x488: move madness, slot 2
    kPadPotion,        // value +0x498, bounds +0x4a0 +0x4a4, latch +0x4a8: Cmd_UsePotion under IsUsingMoveMadness
    kPadCast,          // value +0x3b8, bounds +0x3c0 +0x3c4, latch +0x3c8: Cmd_BeginCast(false) / Cmd_EndCast(false)
    kPadOpenIGM,       // value +0x3f8, bounds +0x400 +0x404, latch +0x408: Cmd_OpenIGM on release while latched
};

// Skill slot id of "no skill" (Character::SG_GetSkillInSlot returns -1, which the original compares as unsigned).
constexpr unsigned int kNoSkill = 0xFFFFFFFFu;

// PARTIAL seam (Gamepad). The control table is not declared (InputManager.hpp forward-declares Gamepad only).
// Reports the released state, so no control is pressed.
float PadValue(const Gamepad* pad, PadControl control) {
    (void)pad;
    (void)control;
    return 0.0f;
}

// PARTIAL seam (Gamepad): the first bound of a control (the lower of its two bound fields).
float PadBound0(const Gamepad* pad, PadControl control) {
    (void)pad;
    (void)control;
    return 0.0f;
}

// PARTIAL seam (Gamepad): the second bound of a control (the higher of its two bound fields).
float PadBound1(const Gamepad* pad, PadControl control) {
    (void)pad;
    (void)control;
    return 0.0f;
}

// PARTIAL seam (Gamepad): the latch byte, set while the control is held. Controls without a latch report false.
bool PadLatched(const Gamepad* pad, PadControl control) {
    (void)pad;
    (void)control;
    return false;
}

// The press level of every control, in the order the original evaluates it: ((bound1 + bound0) + 1.0f) * 0.5f.
float PadPressLevel(const Gamepad* pad, PadControl control) {
    return (PadBound1(pad, control) + PadBound0(pad, control) + 1.0f) * 0.5f;
}

// PARTIAL seam: the cursor position. The original reads Application::m_menuManager (Application +0x54, the global
// dword_99F780) and copies its words at MenuManager +0x50 and +0x54 into a Point2D<float>. MenuManager.hpp declares
// those words as uint32 (unk_50, unk_54), so the copy is not made here. Reports the origin.
Point2D<float> CursorScreenPosition() {
    return Point2D<float>{0.0f, 0.0f};
}

// PARTIAL seam: PFWorld::TranslateScreenToWorld(Point2D<float> const&, Point3D<float>&) -> bool, called on
// Singleton<PFWorld>::s_inst. PFWorld is not declared in the tree. Reports failure, so `out` is not written.
bool TranslateScreenToWorld(const Point2D<float>& screen, Point3D<float>& out) {
    (void)screen;
    (void)out;
    return false;
}

// PARTIAL seam: the player id, the int at +0x668 of the record PlayerManager::GetPlayerByCharacter returns for the
// character (the global dword_99F76C; PlayerInfo is not declared). Reports player 0.
int PlayerIdForCharacter(const Character* character) {
    (void)character;
    return 0;
}

// PARTIAL seam: raises EvGamepad (vtable _ZTV9EvGamepad, base IEvent) on the global EventManager (dword_99F740) with
// the fields the original fills: +0x04 type 7, +0x08 action, +0x0c pressed, +0x10 player id, +0x14 1.0f. EvGamepad is
// not declared in the tree, so the event is dropped.
void RaiseGamepadEvent(int action, int pressed, int playerId) {
    (void)action;
    (void)pressed;
    (void)playerId;
}

// PARTIAL seam: the object list the target search walks. The original builds an ObjectSearcher::RoomObjectList over
// the list at ObjectManager +0x80 (the global dword_99F764). ObjectManager and the list constructor are not declared.
// Reports no list, so the search is skipped and produces no hit.
ObjectSearcher::IObjectList* RoomSearchList() {
    return nullptr;
}

// DebugSwitches::load(), then GetSwitch(name). The original passes &DebugSwitches::s_inst (private in the header) to
// load() and GetSwitch(). GetInstance() returns that object, but it also loads the file on first use, which the load()
// call here repeats. The repeat only costs time.
bool DebugSwitchOn(const char* name) {
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    const std::string key(name);
    return switches->GetSwitch(key);
}

// One skill slot held by a pad control (slots 0, 1 and 2 share this shape). Release (value < press level) ends the
// skill when it was latched. Press starts it when it was not.
void SkillSlotHold(v2Controller& controller, const Gamepad* pad, PadControl control, unsigned int skill) {
    if (PadValue(pad, control) < PadPressLevel(pad, control)) {
        if (PadLatched(pad, control)) controller.Cmd_EndSkill(skill);
    } else if (!PadLatched(pad, control)) {
        controller.Cmd_BeginSkill(skill);
    }
}

// One move-madness slot. Press (value >= press level) starts the skill when it was not latched. Release ends it when
// it was latched. The comparison order differs from SkillSlotHold, as in the original.
void MadnessSlotHold(v2Controller& controller, const Gamepad* pad, PadControl control, unsigned int skill) {
    if (PadValue(pad, control) >= PadPressLevel(pad, control)) {
        if (!PadLatched(pad, control)) controller.Cmd_BeginSkill(skill);
    } else if (PadLatched(pad, control)) {
        controller.Cmd_EndSkill(skill);
    }
}

// An EvGamepad for a control (actions 2, 0 and 3). Release (value < press level) raises pressed = 0 when latched.
// Press raises pressed = 1 when it was not latched.
void RaiseOnChange(const Gamepad* pad, PadControl control, int action, int playerId) {
    if (PadValue(pad, control) < PadPressLevel(pad, control)) {
        if (PadLatched(pad, control)) RaiseGamepadEvent(action, 0, playerId);
    } else if (!PadLatched(pad, control)) {
        RaiseGamepadEvent(action, 1, playerId);
    }
}

}  // namespace

// 0x4098ac v2PS3MoveController::~v2PS3MoveController() (D2, base-object destructor)
// 0x4098b0 v2PS3MoveController::~v2PS3MoveController() (D1, complete-object destructor), same body.
// Both are empty in the original (4 bytes, bx lr). The v2Controller base destructor is empty too, so the call is not made.
// The vptr writes are the compiler's.
v2PS3MoveController::~v2PS3MoveController() {}

// 0x4098b4 v2PS3MoveController::~v2PS3MoveController() (D0, deleting destructor)
// skipped: compiler-generated deleting destructor. The original body is the D1 call (0x4098b0) and then CustomFree(this)
// (28 bytes in the symbol table). The C++ compiler emits it for the virtual destructor declared in
// dh2/v2PS3MoveController.hpp, so no body is written here.

// 0x4098d0 v2PS3MoveController::v2PS3MoveController(v2Controllable*, int) (C1, complete-object constructor)
// 0x4099bc v2PS3MoveController::v2PS3MoveController(v2Controllable*, int) (C2, base-object constructor), same body.
// partial: the original writes the v2Controller base inline (it has no standalone symbol). The base constructor
// v2Controller(v2Controllable*) is declared in dh2/v2Controller.hpp but nothing defines it yet, so the base is
// initialised through it here. The base stores (vptr, +0x04 m_controllable, +0x08 m_blocked, +0x09, +0x0a and
// +0x0c m_character) belong to that constructor.
// This body does: the null-controllable assert, the gamepad index at +0x10, and the three zeroed words at +0x14..+0x1c.
v2PS3MoveController::v2PS3MoveController(v2Controllable* controllable, int gamepadIndex)
    : v2Controller(controllable) {
    if (controllable == nullptr) {
        // The assert names v2Controller.h:68 because the original inlines the v2Controller constructor, where this
        // check belongs. It moves there once that constructor is defined, so it is not printed twice.
        if (gAssertLevel == 2) {
            // The original stores 0 to address 0 to force a crash.
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "controllable",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Controllers\\/v2Controller.h", 68);
        }
    }
    m_gamepadIndex = gamepadIndex;  // +0x10
    unk_14 = 0;                     // +0x14
    unk_18 = 0;                     // +0x18
    unk_1c = 0;                     // +0x1c
}

// 0x409bd8 v2PS3MoveController::Update() (vtable slot 2, overrides v2Controller::Update)
// partial: the control flow is complete. The pad reads, the cursor, the PFWorld screen translation, the player id, the
// EvGamepad event and the target-search list are PARTIAL seams (see the anonymous namespace above). Until they are
// filled, every control reads as released with no latch, so no command is issued.
void v2PS3MoveController::Update() {
    InputManager* input = InputManager::GetInstance();
    if (m_gamepadIndex >= input->GetNumGamepads()) return;  // +0x10
    Gamepad* pad = (m_gamepadIndex < 0) ? input->GetFirstConnectedGamepad() : input->GetGamepad(m_gamepadIndex);
    if (pad == nullptr) return;

    // Primary control. While held: with a pending spot target, store the translated cursor point on the character
    // (+0x14b0 and +0x14bc). Otherwise head to the cursor point, search for a target and act on the nearest hit.
    if (PadValue(pad, kPadPrimary) >= PadPressLevel(pad, kPadPrimary)) {
        Character* character = m_character;  // +0x0c, the original's v18
        Point3D<float> pos{0.0f, 0.0f, 0.0f};  // v42..v44 are zeroed before the translation
        const Point2D<float> screen = CursorScreenPosition();
        if (character != nullptr && character->m_spotTargetPending != 0) {  // Character +0x14c8
            if (TranslateScreenToWorld(screen, pos)) {
                character->m_spotTargetPos = pos;       // Character +0x14b0
                character->m_spotTargetValidPos = pos;  // Character +0x14bc
            }
        } else {
            if (TranslateScreenToWorld(screen, pos)) Cmd_HeadTo(pos);
            ObjectSearcher::TargetList targets(character, 1, 0, 1);
            targets.m_filterMask = 89;  // +0x34, overrides the constructor argument
            targets.m_arg3 = 1;         // +0x38
            while (!targets.m_results.empty()) targets.m_results.pop();
            // The sorter is closest. The constructor's sortType argument (1) installs
            // ObjectSearcher::TargetSorter::_sortClosest, which the original stores at +0x28 at this point.
            ObjectSearcher::IObjectList* objects = RoomSearchList();
            // maxAngle is the float 0x40C90FDB (2 * pi, 6.2831855f), the bits the original loads with MOVW/MOVT.
            // 6.2832f is a different float (0x40C90FF9), so it is not used.
            if (objects != nullptr) targets.Search(pos, 0.0f, 6.2831855f, *objects);
            if (!targets.m_results.empty()) {
                GameObject* target = targets.m_results.top().object;
                if (target != nullptr && target->IsInteractive(character)) {
                    if (character->AI_CanAttack(target)) {
                        Cmd_Attack(nullptr);
                    } else if (!character->SM_IsInteracting()) {
                        Cmd_UseOOI(nullptr);
                    }
                }
            }
        }
    }

    // Move-to-click control. On release, stop unless the IsMovingToClick debug switch is set.
    if (PadValue(pad, kPadMoveToClick) < PadPressLevel(pad, kPadMoveToClick)) {
        if (PadLatched(pad, kPadMoveToClick) && !DebugSwitchOn("IsMovingToClick")) Cmd_Stop();
    }

    // Attack control. Attack while held (no latch).
    if (PadValue(pad, kPadAttack) >= PadPressLevel(pad, kPadAttack)) Cmd_Attack(nullptr);

    // Skill slots, then move madness when the IsUsingMoveMadness debug switch is set.
    Character* character = m_character;  // the original's v6
    if (character != nullptr) {
        // All three slots are read before any of them is used, as in the original.
        const unsigned int skill0 = static_cast<unsigned int>(character->SG_GetSkillInSlot(0));
        const unsigned int skill1 = static_cast<unsigned int>(character->SG_GetSkillInSlot(1));
        const unsigned int skill2 = static_cast<unsigned int>(character->SG_GetSkillInSlot(2));
        if (skill0 != kNoSkill) SkillSlotHold(*this, pad, kPadSkill0, skill0);
        if (skill1 != kNoSkill) SkillSlotHold(*this, pad, kPadSkill1, skill1);
        if (skill2 != kNoSkill) SkillSlotHold(*this, pad, kPadSkill2, skill2);

        if (m_character != nullptr && DebugSwitchOn("IsUsingMoveMadness")) {
            Character* madnessCharacter = m_character;  // the original re-reads +0x0c here
            const unsigned int madness0 = static_cast<unsigned int>(madnessCharacter->SG_GetSkillInSlot(0));
            const unsigned int madness1 = static_cast<unsigned int>(madnessCharacter->SG_GetSkillInSlot(1));
            const unsigned int madness2 = static_cast<unsigned int>(madnessCharacter->SG_GetSkillInSlot(2));
            if (madness0 != kNoSkill) MadnessSlotHold(*this, pad, kPadMadness0, madness0);
            if (madness1 != kNoSkill) MadnessSlotHold(*this, pad, kPadMadness1, madness1);
            if (madness2 != kNoSkill) MadnessSlotHold(*this, pad, kPadMadness2, madness2);
        }
    }

    // Potion control. On press, use a potion when IsUsingMoveMadness is set. The control must not be latched.
    if (PadValue(pad, kPadPotion) >= PadPressLevel(pad, kPadPotion) && !PadLatched(pad, kPadPotion)) {
        if (DebugSwitchOn("IsUsingMoveMadness")) Cmd_UsePotion();
    }

    // Cast control. Press begins the cast (argument false), release ends it.
    if (PadValue(pad, kPadCast) >= PadPressLevel(pad, kPadCast)) {
        if (!PadLatched(pad, kPadCast)) Cmd_BeginCast(false);
    } else if (PadLatched(pad, kPadCast)) {
        Cmd_EndCast(false);
    }

    // Open-IGM control. Opens the menu on release while latched.
    if (PadValue(pad, kPadOpenIGM) < PadPressLevel(pad, kPadOpenIGM) && PadLatched(pad, kPadOpenIGM)) {
        Cmd_OpenIGM();
    }

    // EvGamepad events for the skill and action controls, in the original's order: action 2, action 0, action 1,
    // action 3. The player id comes from the character's player record (0 when there is no character).
    const int playerId = (m_character != nullptr) ? PlayerIdForCharacter(m_character) : 0;
    RaiseOnChange(pad, kPadSkill0, 2, playerId);
    RaiseOnChange(pad, kPadEventAction0, 0, playerId);
    // The action-1 event uses the opposite comparison order from RaiseOnChange, as in the original.
    if (PadValue(pad, kPadSkill2) >= PadPressLevel(pad, kPadSkill2)) {
        if (!PadLatched(pad, kPadSkill2)) RaiseGamepadEvent(1, 1, playerId);
    } else if (PadLatched(pad, kPadSkill2)) {
        RaiseGamepadEvent(1, 0, playerId);
    }
    // The original returns after this event, so nothing follows it.
    RaiseOnChange(pad, kPadSkill1, 3, playerId);
}
