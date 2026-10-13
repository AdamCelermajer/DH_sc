// Batch g1-game-app-GSTest-c7a9f5-0 (GSTest, part 1 of 1).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// Declarations are in dh2/GSTest.hpp. The vptr stores the original constructors and destructors make
// are the compiler's, so no body here writes them.
#include "dh2/GSTest.hpp"

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
// PARTIAL: the body stops after the InputManager lookup. The rest of the original is not rebuilt yet:
//  1. The trigger gate reads Gamepad fields (+0x18 trigger value, +0x20 and +0x24 range bounds,
//     +0x28 flag byte). dh2/Gamepad.hpp does not exist yet, so the gate cannot be written.
//  2. The test run uses sfc::script::lua::Arguments and sfc::script::lua::Binder, which have no
//     headers in this tree. The run also flushes Application::m_luaManager and loads "data/scripts/test".
// The first two calls below are the original's lookups, so their side effects (the lazy creation of the
// input manager) happen as in the original.
void GSTest::Update(StateMachine* machine, double dt) {
    InputManager* input = InputManager::GetInstance();
    Gamepad* pad = input->GetFirstConnectedGamepad();
    (void)machine;
    (void)dt;
    (void)pad;
}
