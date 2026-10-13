// v2EmuController functions of libDungeonHunter2.so (game-anim batch g1-game-anim-v2EmuController-c84e52-0).
// Covers the two constructors (0x4065ac, 0x4066cc), the three destructors (0x4064b8, 0x406528, 0x406544), the IEvent
// entry onEvent (0x406450) and the input handlers _onEvent(EvMouseButton) (0x405f48) and _onEvent(EvKeyboard) (0x4060c0).
// Class layout and declarations: game/include/dh2/v2EmuController.hpp. Every body was checked against its assembly
// (tools/decomp_asm.py).
//
// Status: the controller logic is rebuilt in v2EmuController_detail (the keyboard switch, the mouse command choice and
// the head-towards offset) and covered by tests/g1-game-anim-v2EmuController-c84e52-0_test.cpp. The entry points are
// partial until these headers exist:
//   dh2/IEvent.hpp         the event type id, read through the IEvent vtable slot +0x08 (0 selects the keyboard handler,
//                          2 the mouse handler)
//   dh2/EvMouseButton.hpp  fields +0x08 (int), +0x0c (byte), +0x0e (int16 screen x), +0x10 (int16 screen y)
//   dh2/EvKeyboard.hpp     fields +0x0c (int key id), +0x10 (byte pressed flag)
//   dh2/PFWorld.hpp        Singleton<PFWorld>::s_inst and PFWorld::TranslateScreenToWorld(Point2D<float> const&,
//                          Point3D<float>&) returning bool
//   an instance accessor for Singleton<Application>::s_inst. Its +0x14 is the EventManager that the receiver attaches
//                          to: the ELF global dword_99F740 is Application::m_eventManager (asm of the ctor and dtor).
//                          Attach and Detach wait on that accessor.
// Every entry point returns 0 on every path in the original, so the stubs below return false.

#include "dh2/v2EmuController.hpp"

#include <cstdio>
#include <cstdlib>

#include "dh2/Character.hpp"
#include "dh2/v2Controller.hpp"

// Engine assert level, defined in core-platform/src/memory.cpp. Declared here as the other game-anim sources do,
// because core-platform/memory.hpp redefines glitch::core types that the game headers already provide.
extern int gAssertLevel;

namespace v2EmuController_detail {

// Mouse command chosen from the key-state mask unk_018 (asm of 0x405f48). Bits 0 and 1 together win (MoveTo), then
// bit 0 (WarpTo), then bit 1 (LookAt). The original translates the screen point only on the branch that runs a command.
enum class MouseCommand { kNone, kMoveTo, kWarpTo, kLookAt };

MouseCommand MouseCommandForMask(unsigned int mask) {
    if ((mask & 3u) == 3u) {
        return MouseCommand::kMoveTo;
    }
    if ((mask & 1u) != 0) {
        return MouseCommand::kWarpTo;
    }
    if ((mask & 2u) != 0) {
        return MouseCommand::kLookAt;
    }
    return MouseCommand::kNone;
}

// Offset built from the key-state mask unk_014 for the head-towards keys 135..138 (asm: loc_406284 to loc_4062fc).
// Bit 0 sets x to -200, bit 1 adds 200 to x, bit 2 subtracts 200 from y and bit 3 adds 200 to y. z stays 0. The
// original does the same steps in single precision in this order; every value is exact.
Point3D<float> HeadOffsetForMask(unsigned int mask) {
    Point3D<float> pos{0.0f, 0.0f, 0.0f};
    if ((mask & 1u) != 0) {
        pos.X = -200.0f;
    }
    if ((mask & 2u) != 0) {
        pos.X += 200.0f;
    }
    if ((mask & 4u) != 0) {
        pos.Y -= 200.0f;
    }
    if ((mask & 8u) != 0) {
        pos.Y += 200.0f;
    }
    return pos;
}

// Keyboard switch of _onEvent(EvKeyboard) (0x4060c0). key is the event's key id (the original's jump table covers ids
// 49..146) and pressed is its flag. Ids that the switch does not list do nothing.
void RouteKey(v2EmuController& self, int key, bool pressed) {
    switch (key) {
        case 49:  // '1'..'4': potions
        case 50:
        case 51:
        case 52:
            if (pressed) {
                self.Cmd_UsePotion();
            }
            return;
        case 77:
            if (pressed) {
                self.Cmd_UseOOI(nullptr);
            }
            return;
        case 78:
            if (pressed) {
                self.Cmd_Attack(nullptr);
            }
            return;
        case 95:  // skill slots 0..2 of the controlled Character
        case 96:
        case 97: {
            Character* character = self.m_character;
            if (character == nullptr) {
                return;
            }
            const int skill = character->SG_GetSkillInSlot(key - 95);
            if (skill == -1) {
                return;
            }
            if (pressed) {
                self.Cmd_BeginSkill(static_cast<unsigned int>(skill));
            } else {
                self.Cmd_EndSkill(static_cast<unsigned int>(skill));
            }
            return;
        }
        case 98:
            if (pressed) {
                self.Cmd_BeginCast(false);
            } else {
                self.Cmd_EndCast(false);
            }
            return;
        case 135:  // head-towards keys: set or clear one bit group of unk_014, then head towards the offset
        case 136:
        case 137:
        case 138: {
            static const unsigned int kBits[4] = {0x9u, 0x6u, 0x5u, 0xAu};
            const unsigned int bits = kBits[key - 135];
            self.unk_014 = pressed ? (self.unk_014 | bits) : (self.unk_014 & ~bits);
            self.Cmd_HeadTowards(HeadOffsetForMask(self.unk_014));
            return;
        }
        case 145:  // unk_018 bit groups, no command
            self.unk_018 = pressed ? (self.unk_018 | 0x9u) : (self.unk_018 & ~0x9u);
            return;
        case 146:
            self.unk_018 = pressed ? (self.unk_018 | 0x12u) : (self.unk_018 & ~0x12u);
            return;
        default:
            return;
    }
}

}  // namespace v2EmuController_detail

// 0x4065ac _ZN15v2EmuControllerC1EP14v2Controllable (complete-object constructor)
// 0x4066cc _ZN15v2EmuControllerC2EP14v2Controllable (base-object constructor): same body, defined once here.
// The original inlines the v2Controller constructor. It writes the base fields (+0x04 controllable, +0x08..+0x0a
// flags, +0x0c character), then checks the controllable, then writes the derived vptrs, unk_014 and unk_018, then
// calls Attach(0, this+0x10, 0) and Attach(2, this+0x10, 0). The base fields come from v2Controller(controllable),
// which nothing defines yet. The assert is printed here, where the original prints it (v2Controller.h:68). When the
// base constructor gets its body, the assert moves there, as v2MixedController does, so it is not printed twice.
v2EmuController::v2EmuController(v2Controllable* controllable) : v2Controller(controllable) {
    if (controllable == nullptr) {
        if (gAssertLevel == 2) {
            // The original stores 0 to address 0 to force a crash.
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "controllable",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Controllers\\/v2Controller.h", 68);
        }
    }
    unk_014 = 0;  // +0x14 key-state mask, cleared
    unk_018 = 0;  // +0x18 key-state mask, cleared
    // PARTIAL: the original attaches this receiver with EventManager::Attach(0, this+0x10, 0) and
    // Attach(2, this+0x10, 0) on dword_99F740 (Singleton<Application>::s_inst +0x14). No header declares that
    // instance yet (see the file comment), so the receiver is not attached and no event reaches it.
}

// 0x4064b8 _ZN15v2EmuControllerD1Ev (complete-object destructor)
// 0x406544 _ZN15v2EmuControllerD2Ev (base-object destructor): same body, defined once here.
// The original sets the vptrs (the compiler does this), calls EventManager::Detach(0, this+0x10) and
// Detach(2, this+0x10) on dword_99F740, and returns. It does not call ~v2Controller, whose body is empty, so the
// implicit base call below changes nothing.
v2EmuController::~v2EmuController() {
    // PARTIAL: Detach(0, this+0x10) and Detach(2, this+0x10) wait on the Application instance accessor (see above).
}

// 0x406528 _ZN15v2EmuControllerD0Ev (deleting destructor) is compiler-generated: it calls D1 and then CustomFree(this).
// The C++ delete expression covers it, so no body is written here.

// 0x406450 _ZN15v2EmuController7onEventEPK6IEventPK12EventManager
// Asm verified: the event's type id comes from the IEvent vtable (slot +0x08, LDR PC,[R3,#8]). Type 0 tail-calls the
// keyboard handler (B _onEvent(EvKeyboard)), type 2 tail-calls the mouse handler (B _onEvent(EvMouseButton)), and any
// other type returns 0. Both handlers return 0 on every path, so this returns 0 on every path.
bool v2EmuController::onEvent(const IEvent* event, const EventManager* manager) {
    // PARTIAL: waits on dh2/IEvent.hpp. The type id is a virtual call through the vtable slot +0x08, which needs the
    // complete IEvent type, so the dispatch to the two handlers is not written. The stub returns the value the original
    // always returns.
    (void)event;
    (void)manager;
    return false;
}

// 0x405f48 _ZN15v2EmuController8_onEventEPK13EvMouseButtonPK12EventManager
// Asm verified. Acts only when the word at event+0x08 and the byte at event+0x0c are both 0. The screen point is the
// int16 at event+0x0e (x) and the int16 at event+0x10 (y), each converted to float. The command comes from unk_018
// through v2EmuController_detail::MouseCommandForMask: MoveTo, WarpTo or LookAt. The command runs only when
// PFWorld::TranslateScreenToWorld succeeds. Returns 0 on every path.
bool v2EmuController::_onEvent(const EvMouseButton* event, const EventManager* manager) {
    // PARTIAL: waits on dh2/EvMouseButton.hpp (fields +0x08, +0x0c, +0x0e, +0x10) and on dh2/PFWorld.hpp
    // (Singleton<PFWorld>::s_inst and TranslateScreenToWorld(Point2D<float> const&, Point3D<float>&) returning bool).
    // The command choice is v2EmuController_detail::MouseCommandForMask(unk_018), rebuilt and tested.
    (void)event;
    (void)manager;
    return false;
}

// 0x4060c0 _ZN15v2EmuController8_onEventEPK10EvKeyboardPK12EventManager
// Asm verified. The key id at event+0x0c is the switch value and the byte at event+0x10 is the pressed flag. The switch
// is v2EmuController_detail::RouteKey, rebuilt and tested. Returns 0 on every path.
bool v2EmuController::_onEvent(const EvKeyboard* event, const EventManager* manager) {
    // PARTIAL: waits on dh2/EvKeyboard.hpp (key id as int at +0x0c, pressed flag as bool at +0x10). Once it exists the
    // body is v2EmuController_detail::RouteKey(*this, <key id>, <pressed>) followed by return false.
    (void)event;
    (void)manager;
    return false;
}
