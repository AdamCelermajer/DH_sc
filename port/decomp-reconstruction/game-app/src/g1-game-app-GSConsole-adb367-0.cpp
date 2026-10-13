// Batch g1-game-app-GSConsole-adb367-0 (GSConsole, part 1 of 1: all 11 addresses of the class).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py). Member
// offsets in comments are the original 32-bit ones (dh2/GSConsole.hpp, dh2/Application.hpp).
//
// Status of each address is in game-app/ledger.d/g1-game-app-GSConsole-adb367-0.csv. In short:
//   reconstructed  the constructors, the D1/D2 destructors and Draw.
//   partial        Ctor, Dtor, Update, Draw2D and Toggle. Each PARTIAL comment names the missing piece:
//                  Console::open_ (private), Console::Update(double) (not declared), the IDevice +0x18
//                  object (not modelled), and the top of StateMachine::m_stack (private).
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
// PARTIAL. The original loads Singleton<Application>::s_inst.m_device (+0x10), then the object at IDevice +0x18,
// and calls that object's vtable slot +0x0c with (object, machine), returning the callee's int. The IDevice
// +0x18 object is not modelled (glitch-core-io/os/device.hpp says so), so the call is missing and the return
// value is dropped. Layout request: glitch::IDevice +0x18 object, its class and slot +0x0c.
void GSConsole::Draw2D(const StateMachine*) {}

// 0x3832fc GSConsole::Draw(StateMachine const*)
// The original moves the machine into r0 and tail-calls StateMachine::RecurseDraw() (asm: MOV R0,R1; B).
void GSConsole::Draw(const StateMachine* machine) {
    machine->RecurseDraw();
}

// 0x383304 GSConsole::Update(StateMachine *,double)
// PARTIAL. The original tail-calls Console::Update(console, dt) when Application::m_console (+0x48, the global
// dword_99F774) is set, and returns 0 otherwise. The null check is written. Console::Update(double) is not
// declared in core-platform/include/core-platform/w7_console_01.hpp (its queue row is core-platform 330270),
// so the call is missing. Layout request: declare Console::Update(double) on the core-platform header.
void GSConsole::Update(StateMachine*, double dt) {
    Console* console = ApplicationGetInstance().m_console;
    if (console == nullptr) {
        return;
    }
    // Missing: console->Update(dt);
    (void)dt;
}

// 0x38332c GSConsole::Dtor(StateMachine const*)
// PARTIAL. The original calls Console::Toggle() when Application::m_console (+0x48) is set and the console's
// open flag (Console +0x04, a bool, asm: LDRB [console,#4]) is set. Console::open_ is private in
// core-platform/include/core-platform/w7_console_01.hpp, so the flag cannot be read and the toggle is missing.
// Layout request: a public accessor for Console +0x04 (open_), e.g. IsOpen() const.
void GSConsole::Dtor(const StateMachine*) {
    Console* console = ApplicationGetInstance().m_console;
    if (console == nullptr) {
        return;
    }
    // Missing: if (console->open_) console->Toggle();
}

// 0x383360 GSConsole::Ctor(StateMachine const*)
// PARTIAL. The mirror of Dtor: the original calls Console::Toggle() when m_console is set and the console's
// open flag (Console +0x04) is clear. The flag is private (see Dtor), so the toggle is missing.
void GSConsole::Ctor(const StateMachine*) {
    Console* console = ApplicationGetInstance().m_console;
    if (console == nullptr) {
        return;
    }
    // Missing: if (!console->open_) console->Toggle();
}

// 0x3833e4 GSConsole::Toggle(void)
// PARTIAL. The original reads sm = Application::m_stateMachine (+0x18, the global dword_99F744). When sm is null
// it reports ASSERT(sm) at level 1 (fprintf at line 26) and at level 2 stores to address 0, and then it
// dereferences sm (+0x10) on the next line. Those paths all end in a crash, so this rebuild aborts. That part
// is written in full. The branch that follows is missing: it needs the top of the state stack, and
// StateMachine::m_stack (begin +0x0c, end +0x10, 8-byte StateInfo) is private in dh2/StateMachine.hpp. The
// original pops (PopState(sm, nullptr)) when the stack is not empty and its top state is &GSConsole::s_inst,
// and pushes (PushState(sm, &GSConsole::s_inst, false)) otherwise. Layout request: a public top-state accessor
// on StateMachine (nullptr when empty).
void GSConsole::Toggle() {
    StateMachine* machine = ApplicationGetInstance().m_stateMachine;
    if (machine == nullptr) {
        if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "sm", kGSConsoleCppPath, 26);
        }
        // Level 2 writes to address 0, levels 0 and 1 dereference the null machine: the original crashes here.
        std::abort();
    }
    // Missing: the push/pop decision on machine->m_stack (see the comment above).
}
