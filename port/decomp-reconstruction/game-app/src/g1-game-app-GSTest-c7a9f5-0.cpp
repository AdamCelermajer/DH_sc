// Batch g1-game-app-GSTest-c7a9f5-0 (GSTest, part 1 of 1).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// Declarations are in dh2/GSTest.hpp. The vptr stores the original constructors and destructors make
// are the compiler's, so no body here writes them.
#include "dh2/GSTest.hpp"

#include "dh2/Gamepad.hpp"
#include "dh2/InputManager.hpp"

// 0x386a5c _ZN6GSTestC2Ev
// 0x386a80 _ZN6GSTestC1Ev
// Both constructors have the same body, so one C++ constructor covers the complete-object and the
// base-object variants. The original body is only the GSTest vptr store (asm: ADD #8, STR [R0]).
// StateBase has no constructor body of its own.
GSTest::GSTest() {}

// 0x386aa4 _ZN6GSTestD2Ev
// 0x386aa8 _ZN6GSTestD1Ev
// Both destructors are empty (asm: BX LR). The StateBase destructor the implicit base call reaches is
// empty as well (0x38324c is BX LR), so the base call adds nothing. One C++ destructor covers both.
GSTest::~GSTest() {}

// 0x386aac _ZN6GSTest4CtorEPK12StateMachine
// Empty override of the StateBase Ctor slot (asm: BX LR).
void GSTest::Ctor(const StateMachine* /*machine*/) {}

// 0x386ab0 _ZN6GSTest4DtorEPK12StateMachine
// Empty override of the StateBase Dtor slot (asm: BX LR).
void GSTest::Dtor(const StateMachine* /*machine*/) {}

// 0x386adc _ZN6GSTest4DrawEPK12StateMachine
// Empty override of the StateBase Draw slot (asm: BX LR).
void GSTest::Draw(const StateMachine* /*machine*/) {}

// 0x386b4c _ZN6GSTestD0Ev
// Not rebuilt: the compiler-generated deleting destructor. It calls the complete-object destructor and
// then CustomFree(this) (asm: BL D1, BL CustomFree). A C++ delete expression on a GSTest produces it.

// 0x386f90 _ZN6GSTest6UpdateEP12StateMachined
// PARTIAL. The trigger gate is written. The test run that the gate starts is not:
//  1. Gate (written): the first connected pad's channel 0 (Gamepad::m_buttons[0], the threshold group at pad +0x18 to
//     +0x28) must have previous (+0x18) >= (previous_maximum (+0x24) + previous_minimum (+0x20) + 1.0) * 0.5, and the
//     flag previous_above_threshold (+0x28) must be clear. The original skips the run when the flag is set.
//  2. Test run (missing): Application::m_luaManager FlushBufferedFiles(); LuaScript on the stack with skipBindings
//     false; puts of the banner; Binder::bindFunction("TestFunc", TestUD::TestFunc, user) on the script's own binder;
//     Load("data/scripts/test"); an Arguments list with TestUD, TestUD2 and TestUD2 user data and the script pointer;
//     Call("TestTestTest"); then the destructors. LuaScript keeps its Binder and Instance as opaque uint8_t[12] members
//     (no typed sfc::script::lua::Binder, layout request), so the binding cannot be made on the script's binder here.
//     The run also needs the TestUD vtable identity of the user pointer (off_963EA0 and off_963EB8), which is not
//     established. None of the run is written, because a run without its binding would not match the original.
void GSTest::Update(StateMachine* machine, double dt) {
    InputManager* input = InputManager::GetInstance();
    Gamepad* pad = input->GetFirstConnectedGamepad();
    (void)machine;
    (void)dt;
    if (pad == nullptr) {
        return;
    }
    const Gamepad::Channel& trigger = pad->m_buttons[0];
    const float sum = trigger.previous_maximum + trigger.previous_minimum;
    const float threshold = (sum + 1.0f) * 0.5f;
    if (!(trigger.previous >= threshold) || trigger.previous_above_threshold) {
        return;
    }
    // PARTIAL (2): the test run is not written, see above.
}
