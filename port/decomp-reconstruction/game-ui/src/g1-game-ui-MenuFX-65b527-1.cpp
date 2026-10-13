// MenuFX bodies, batch g1-game-ui-MenuFX-65b527-1 (part 2 of 2 of MenuFX).
// Constructors and destructors, Unload, the menu stack (PushMenu, SwitchMenu, PopMenu, PopAll, SetMenu),
// state registration, the per-frame Update and the event dispatch. Rebuilt from the Hex-Rays pseudocode of
// libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM offsets.
//
// Not declared anywhere in this tree yet, so the statements that call them are left out and marked PARTIAL:
//   - RenderFX (dh2/RenderFX.hpp is a placeholder): SetEventListener, SetContext, PlayAnim, FindCharacters,
//     ResetFocus, SetFocus, Find, Controller::Reset, the flags word (+0xf8), the focus (+0x68), the context
//     character (+0x40), and the RenderFX::Event layout (first word is the target character).
//   - gameswf: weak_ptr<character> (MenuFX::State +0x48/+0x4c and +0x50/+0x54), the flag bytes of
//     gameswf::character (+0x9b, +0xea), smart_ptr_proxy liveness, gameswf::array<State*>::reserve.
// A reserve call that grows m_states or m_stack is left out, and the element stores after it are kept as in
// the original, so the integrator must wire gameswf::array before these paths run.

#include "dh2/MenuFX.hpp"

// 0x7a86e4 _ZN6MenuFXC1Ev
// 0x7a8750 _ZN6MenuFXC2Ev (same body: one C++ constructor serves both)
// PARTIAL: the original's last statement is RenderFX::SetEventListener(this, <EventListener subobject at
// +0x100>), which is not declared in dh2/RenderFX.hpp, so it is left out. The vptrs (MenuFX at +0x00 and
// EventListener at +0x100) come from the C++ base constructors, as in the original.
MenuFX::MenuFX() {
    m_states = StateArray{};  // +0x104 data, +0x108 size, +0x10c capacity, +0x110 static flag: all zero
    m_stack = StateArray{};   // +0x114 data, +0x118 size, +0x11c capacity, +0x120 static flag: all zero
}

// 0x7abc68 _ZN6MenuFXD1Ev (base-object destructor; its body is the same as 0x7abd48)
// 0x7abd48 _ZN6MenuFXD2Ev (complete-object destructor, same body)
// PARTIAL: the two gameswf::array<State*>::reserve(0) calls that free the buffers are left out (not declared
// in this tree). The original also zeroes the slots below index 0 when a size is negative (asm loc_7ABD04 and
// loc_7ABCE0); that branch is not rebuilt because it writes before the buffer.
// RenderFX::~RenderFX runs after this body (implicit base destructor), as in the original.
MenuFX::~MenuFX() {
    m_stack.m_size = 0;   // +0x118
    // PARTIAL: gameswf::array<State*>::reserve(&m_stack, 0)
    m_states.m_size = 0;  // +0x108
    // PARTIAL: gameswf::array<State*>::reserve(&m_states, 0)
}

// 0x7abbbc _ZN6MenuFX6UnloadEv
// PARTIAL: RenderFX::Controller::Reset on the four controllers at +0x58, +0x80, +0xa8 and +0xd0 is left out
// (RenderFX::Controller is not declared in dh2/RenderFX.hpp). The loops that zero slots below index 0 for a
// negative size are not rebuilt, as in the destructor.
void MenuFX::Unload() {
    RenderFX::Unload();
    m_states.m_size = 0;  // +0x108 (set after the states loop in the original)
    m_stack.m_size = 0;   // +0x118
}

// 0x7ac444 _ZN6MenuFX15SetFocusDefaultEv
// PARTIAL: the whole body is left out. The original finds the "btn" characters under the context character
// (+0x40) with RenderFX::FindCharacters(ctx, "btn", 3); with none it calls RenderFX::ResetFocus(0), otherwise
// RenderFX::SetFocus(first button, 0). None of the three RenderFX methods is declared in dh2/RenderFX.hpp.
void MenuFX::SetFocusDefault() {}

// 0x7ad88c _ZN6MenuFX6UpdateEib
// PARTIAL: rebuilt are the base update and the top state's Update(dt). Not rebuilt:
//  - the states below the top, from the second-from-top down: a state whose character weak pointer has expired
//    (proxy byte +4 clear) releases the proxy and clears both pointers; otherwise, when the character's flag
//    byte +0x9b is set, UpdateBackground(dt) runs. Needs gameswf::weak_ptr and the gameswf::character layout.
//  - the registered states with m_status == 2 (hiding): the same weak-pointer release, the call through
//    MEMORY[0x464C4617] that decides when the hide has finished, and the clears of the flag byte +0x9b. The
//    pseudocode also writes HIBYTE(dword_98), and Hex-Rays warns that its writes to const memory may be wrong,
//    so this part waits on a check against the assembly.
void MenuFX::Update(int dt, bool a) {
    RenderFX::Update(dt, a);
    if (m_stack.m_size > 0) {
        m_stack.m_data[m_stack.m_size - 1]->Update(dt);  // top state, vtable +0x20
    }
}

// 0x7addb0 _ZN6MenuFX15GetStateHandlerEPN7gameswf9characterE
// PARTIAL: the empty-stack result and the null-target result are exact (both return GetCurrentState()). For a
// target the original walks the stack from the top. For each state it compares the state's character (+0x4c,
// valid only while the weak proxy at +0x48 is alive) with the target, then with the target's parent chain
// (the gameswf::character weak pointer at +0x3c and cached parent at +0x40). That walk needs the gameswf
// layouts, so for a non-null target this port returns the current state.
MenuFX::State* MenuFX::GetStateHandler(gameswf::character* target) {
    (void)target;
    return GetCurrentState();
}

// 0x7adee0 _ZN6MenuFX14CanHandleEventERN8RenderFX5EventE
// PARTIAL: the original asks GetStateHandler(first word of the event, the target character) for its state and
// returns that state's CanHandleEvent. RenderFX::Event is incomplete in dh2/RenderFX.hpp, so the target is not
// read and the current state answers instead. The empty-stack result (true) is exact.
bool MenuFX::CanHandleEvent(RenderFX::Event& e) {
    if (m_stack.m_size <= 0) {
        return true;
    }
    return GetCurrentState()->CanHandleEvent(e);
}

// 0x7adf20 _ZN6MenuFX7OnEventERN8RenderFX5EventE
// PARTIAL: as CanHandleEvent. The original dispatches to GetStateHandler(event target)->OnEvent(e); the event
// target is not declared, so the current state receives the event.
void MenuFX::OnEvent(RenderFX::Event& e) {
    if (m_stack.m_size > 0) {
        GetCurrentState()->OnEvent(e);
    }
}

// 0x7adf50 _ZN6MenuFX13RegisterStateEPNS_5StateEPKc
// PARTIAL: left out are RenderFX::Find(movieName), the gameswf weak pointer assignment into the state (+0x48,
// +0x4c) and the clear of the found character's flag byte +0x9b (RenderFX::Find and the weak pointer are not
// declared). The reserve that grows m_states is left out too. The menu back-pointer, the array stores and the
// Create() call are kept as in the original.
void MenuFX::RegisterState(State* state, const char* movieName) {
    state->m_menu = this;                        // +0x04
    const int32_t index = m_states.m_size;       // +0x108
    const int32_t newSize = index + 1;
    // PARTIAL: if (index != -1 && newSize > m_states.m_capacity) gameswf::array<State*>::reserve(&m_states, newSize + (newSize >> 1))
    m_states.m_data[index] = nullptr;
    if (movieName == nullptr) {
        movieName = state->m_name;               // +0x08, the state's own name
    }
    m_states.m_size = newSize;
    m_states.m_data[index] = state;
    // PARTIAL: RenderFX::Find(movieName) and the weak pointer and flag steps described above
    state->Create();                             // vtable +0x08
}

// 0x7adff0 _ZN6MenuFX14RegisterStatesEPPNS_5StateEi
// PARTIAL: the first loop is rebuilt (reserve omitted, as in RegisterState). Left out are the second loop, which
// finds each menu with RenderFX::Find(name) and, when found, sets its weak pointer, clears its flag byte +0x9b
// and calls Create() (when not found it logs "menu %s not found"), and the tail, which runs
// RenderFX::FindCharacters(ctx, "btn", 0), calls the character's vtable +0x08 with 2 for each button and sets
// its flag byte +0xea when that returns nonzero.
void MenuFX::RegisterStates(State** states, int count) {
    for (int i = 0; i < count; ++i) {
        states[i]->m_menu = this;                // +0x04
        const int32_t index = m_states.m_size;   // +0x108
        const int32_t newSize = index + 1;
        // PARTIAL: reserve as in RegisterState
        m_states.m_data[index] = nullptr;
        m_states.m_size = newSize;
        m_states.m_data[index] = states[i];
    }
    // PARTIAL: second loop (RenderFX::Find per state, weak pointer, flag, Create or log) and the FindCharacters tail
}

// 0x7ae284 _ZN6MenuFX7SetMenuEPKcb
// PARTIAL: the transition follows the original's order, but its RenderFX and gameswf calls are left out because
// they are not declared in this tree: PlayAnim ("hide", "focus_out", "focus_in", "show"), SetContext, SetFocus
// and ResetFocus; the saved-focus weak pointers (+0x50/+0x54 of the old top, from RenderFX focus +0x68); the bits
// 0x01, 0x08 and 0x40 of the RenderFX flags word (+0xf8); and the status values that the animation results set
// (2 hiding, 4 focus out). The reserve that grows m_stack is left out.
// Empty-stack SwitchMenu: the original decrements the size to -1 and stores at data[-1]; this port skips the
// decrement and pushes at index 0 instead.
void MenuFX::SetMenu(const char* name, bool push) {
    State* next = GetState(name);
    if (next == nullptr) {
        return;
    }
    if (m_stack.m_size > 0) {
        State* top = m_stack.m_data[m_stack.m_size - 1];
        top->LostFocus();                        // vtable +0x18
        if (!push) {
            top->Hide();                         // vtable +0x10
        }
        // PARTIAL: flag-dependent hide or focus_out animation of the old top, and its saved focus
    }
    if (!push && m_stack.m_size > 0) {
        m_stack.m_size -= 1;                     // SwitchMenu replaces the top
    }
    const int32_t index = m_stack.m_size;        // +0x118
    // PARTIAL: if (index != -1 && index + 1 > m_stack.m_capacity) gameswf::array<State*>::reserve(&m_stack, ...)
    m_stack.m_data[index] = nullptr;
    m_stack.m_size = index + 1;
    m_stack.m_data[index] = next;
    // PARTIAL: focus restore and the show or focus_in animation, and RenderFX::SetContext
    next->Show();                                // vtable +0x0c
    next->GotFocus();                            // vtable +0x14
    next->m_status = 1;                          // +0x58, shown
}

// 0x7ae69c _ZN6MenuFX10SwitchMenuEPKc
void MenuFX::SwitchMenu(const char* name) {
    SetMenu(name, false);
}

// 0x7ae6a4 _ZN6MenuFX8PushMenuEPKc
void MenuFX::PushMenu(const char* name) {
    SetMenu(name, true);
}

// 0x7abb20 _ZN6MenuFX6PopAllEv
// PARTIAL: the final RenderFX::SetContext(this, root character at root +0x10) is left out (SetContext and
// gameswf::root are not declared in this tree). The loop that hides every state from the top down is rebuilt.
void MenuFX::PopAll() {
    while (m_stack.m_size > 0) {
        m_stack.m_data[m_stack.m_size - 1]->Hide();         // vtable +0x10
        m_stack.m_data[m_stack.m_size - 1]->m_status = 2;   // +0x58, hiding
        m_stack.m_size -= 1;                                // shrink; reserve is only needed to grow
    }
}

// 0x7ae808 _ZN6MenuFX7PopMenuEv
// PARTIAL: the flag-dependent steps are left out (RenderFX flags word +0xf8, bits 0x40, 0x08 and 0x01): the
// "hide" PlayAnim of the old top's character when bit 0x40 is clear, the saved-focus release when bit 0x08 is
// set, the RenderFX::SetContext call that runs before the stack shrinks, the focus_in or show animation and the
// saved-focus restore of the new top, and the weak-pointer releases. The reserve that shrinks m_stack is left
// out. The steps every path makes are kept: LostFocus, Hide and status 2 for the popped state; GotFocus and
// status 3 for the new top.
// Empty stack: the original reads stack[-1]; this port returns.
void MenuFX::PopMenu() {
    if (m_stack.m_size <= 0) {
        return;
    }
    State* top = m_stack.m_data[m_stack.m_size - 1];
    top->LostFocus();                            // vtable +0x18
    top->Hide();                                 // vtable +0x10
    top->m_status = 2;                           // +0x58, hiding
    if (m_stack.m_size == 1) {
        m_stack.m_size = 0;
        return;
    }
    m_stack.m_size -= 1;
    State* below = m_stack.m_data[m_stack.m_size - 1];
    below->GotFocus();                           // vtable +0x14
    below->m_status = 3;                         // +0x58, focus back
}
