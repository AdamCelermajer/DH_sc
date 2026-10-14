// Batch g1-game-app-GSConsole-adb367-0 (GSConsole, part 1 of 1: all 11 addresses of the class).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py). Member
// offsets in comments are the original 32-bit ones (dh2/GSConsole.hpp, dh2/Application.hpp).
//
// Status of each address is in game-app/ledger.d/g1-game-app-GSConsole-adb367-0.csv. In short:
//   reconstructed  the constructors, the D1/D2 destructors, Draw and Toggle.
//   partial        Ctor, Dtor and Draw2D. Each PARTIAL comment names the missing piece:
//                  Console::open_ (private) and the IDevice +0x18 object (not modelled).
//   skipped        the deleting destructor D0, which the compiler emits.
//
// The globals the original reads are fields of Singleton<Application>::s_inst (0x99f72c), reached through
// ApplicationGetInstance() (dh2/global/ctrl_camera.hpp):
//   dword_99F73C  = Application::m_device      (+0x10)
//   dword_99F744  = Application::m_stateMachine (+0x18)
//   dword_99F774  = Application::m_console     (+0x48)
#include <cstdio>
#include <cstdlib>

#include "core-platform/memory.hpp"         // gAssertLevel
#include "core-platform/w7_console_01.hpp"  // Console: Toggle() is public, open_ (+0x04) is private
#include "dh2/Application.hpp"
#include "dh2/GSConsole.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/global/ctrl_camera.hpp"       // ApplicationGetInstance()

namespace {
// Source path of the original's assert, as the string table holds it (GSConsole.cpp, line 26).
constexpr const char kGSConsoleCppPath[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\States\\GSConsole.cpp";
}  // namespace

// The static instance that Toggle pushes and pops: GSConsole::s_inst (.bss, 4 bytes, vptr only).
GSConsole GSConsole::s_inst;

// 0x383298 GSConsole::GSConsole()
// Complete-object constructor (C1). The body is empty: the compiler stores the GSConsole vptr (off_963D18,
// the vtable at _ZTV9GSConsole +0x08) and the StateBase base adds nothing.
GSConsole::GSConsole() {}

// 0x383274 GSConsole::GSConsole()
// Base-object constructor (C2). Same body as the C1 above; one C++ constructor covers both addresses.

// 0x3832c0 GSConsole::~GSConsole()
// Complete-object destructor (D1), the virtual slot +0x00. The body is empty (asm: BX LR) and the empty
// StateBase destructor needs no call.
GSConsole::~GSConsole() {}

// 0x3832bc GSConsole::~GSConsole()
// Base-object destructor (D2). Same empty body as the D1 above.

// 0x383394 GSConsole::~GSConsole()
// Deleting destructor D0, the virtual slot +0x04: calls D1 (0x3832c0), then CustomFree(this). The compiler
// emits it from the virtual destructor, so no body is written here.

// 0x3832c4 GSConsole::Draw2D(StateMachine const*)
// PARTIAL. Asm 0x3832c4: R3 = Singleton<Application>::s_inst + (dword_99F73C - 0x99F72C) = Application::m_device (+0x10),
// then R3 = [R3+0x18], then R0 = R3 (the receiver), R3 = [R3] (its vptr), LR = PC, PC = [R3+0xC]. So the object at
// IDevice +0x18 is called through vtable slot +0x0c with NO other argument (the StateMachine in r1 is not moved into a
// parameter register), and its int result is returned. The call has exactly one caller in the binary (xrefs_to
// 0x3832c4: GSConsole::Draw, the Draw2D slot), so the object's type cannot be inferred from another site either; the
// class of IDevice +0x18 and its slot +0x0c are the concrete missing item (glitch-core-io/os/device.hpp declares the
// word as void* unk_18 and its comment says the role is not identified). The body is therefore empty; the original's
// int result is dropped by the void StateBase::Draw2D slot. Adding the receiver type to IDevice is engine-glitch's
// edit, not this batch's.
void GSConsole::Draw2D(const StateMachine*) {}

// 0x3832fc GSConsole::Draw(StateMachine const*)
// The original moves the machine into r0 and tail-calls StateMachine::RecurseDraw() (asm: MOV R0,R1; B).
void GSConsole::Draw(const StateMachine* machine) {
    machine->RecurseDraw();
}

// 0x383304 GSConsole::Update(StateMachine *,double)
// Tail-calls Console::Update(dt) when Application::m_console (+0x48, the global dword_99F774) is set, and returns 0 otherwise.
void GSConsole::Update(StateMachine*, double dt) {
    Console* console = ApplicationGetInstance().m_console;
    if (console == nullptr) {
        return;
    }
    console->Update(dt);
}

// 0x38332c GSConsole::Dtor(StateMachine const*)
// Closes the console: calls Console::Toggle() when Application::m_console (+0x48) is set and its open flag (Console +0x04) is set.
void GSConsole::Dtor(const StateMachine*) {
    Console* console = ApplicationGetInstance().m_console;
    if (console == nullptr) {
        return;
    }
    if (console->open_) {
        console->Toggle();
    }
}

// 0x383360 GSConsole::Ctor(StateMachine const*)
// Opens the console: calls Console::Toggle() when Application::m_console is set and its open flag (Console +0x04) is clear.
void GSConsole::Ctor(const StateMachine*) {
    Console* console = ApplicationGetInstance().m_console;
    if (console == nullptr) {
        return;
    }
    if (!console->open_) {
        console->Toggle();
    }
}

// 0x3833e4 GSConsole::Toggle(void)
// The original reads sm = Application::m_stateMachine (+0x18, the global dword_99F744). When sm is null it reports
// ASSERT(sm) at level 1 (fprintf at line 26) and at level 2 stores to address 0, and then it dereferences sm. Those
// paths all end in a crash, so this rebuild aborts. Otherwise the top of the state stack decides: when the stack is
// not empty and its top state (the last StateInfo, m_stack.back().state) is &GSConsole::s_inst, the state is popped
// (PopState(nullptr) pops the top), and when it is not, the state is pushed with owned = false.
void GSConsole::Toggle() {
    StateMachine* machine = ApplicationGetInstance().m_stateMachine;
    if (machine == nullptr) {
        if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "sm", kGSConsoleCppPath, 26);
        }
        // Level 2 writes to address 0, levels 0 and 1 dereference the null machine: the original crashes here.
        std::abort();
    }
    const bool onTop = !machine->m_stack.empty() && machine->m_stack.back().state == &GSConsole::s_inst;
    if (onTop) {
        machine->PopState(nullptr);
    } else {
        machine->PushState(&GSConsole::s_inst, false);
    }
}
