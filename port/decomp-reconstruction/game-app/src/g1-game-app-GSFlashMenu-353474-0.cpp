// Batch g1-game-app-GSFlashMenu-353474-0 (GSFlashMenu, part 1 of 1).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// The C1/C2 constructor pair and the D1/D2 destructor pair have identical bodies, so each pair has one definition
// here. The scalar deleting destructor (D0) is compiler-generated and is not written.
#include "dh2/GSFlashMenu.hpp"

#include <string>

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/global/ctrl_camera.hpp"

// The one GSFlashMenu object that GSInit and GSEndGame switch to. It is defined with its class, as GSConsole and
// GSKeyboard define theirs. Its constructor is 0x3841b4.
GSFlashMenu GSFlashMenu::s_inst;

namespace {

// Trace probe at the start of Ctor and Dtor (asm of 0x3846fc and 0x3845f8 is the same sequence): DebugSwitches::load(),
// then GetSwitch("isTracingGSFlashMenu") and the switch value is discarded. The asm passes the 20-byte literal to
// std::string::_M_range_initialize; the Hex-Rays listing prints it as "" because it drops the second bound.
// The original calls load() on DebugSwitches::s_inst. s_inst is private in dh2/DebugSwitches.hpp, so GetInstance()
// supplies that same object. GetInstance also loads on first use, and the repeated load only costs time (the same
// convention as DebugSwitchOn in game-anim).
void TraceGSFlashMenuSwitch() {
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    const std::string name("isTracingGSFlashMenu");
    (void)switches->GetSwitch(name);
}

}  // namespace

// 0x3841a8 GSFlashMenu::Draw(StateMachine const*)
// Asm: LDR R0,[R0,#4]; MOV R1,#0; B MenuManager::Draw(bool). A tail call on m_manager with hudOnly = false.
void GSFlashMenu::Draw(const StateMachine* /*machine*/) {
    m_manager->Draw(false);
}

// 0x3841b4 GSFlashMenu::GSFlashMenu()  (C1 variant)
// 0x38421c GSFlashMenu::GSFlashMenu()  (C2 variant, same body; both are 72 bytes)
// PARTIAL. Reproduced: the three menu pointers are cleared (+0x04, +0x08, +0x0c), the string at +0x10 is set to the
// empty string on its inline buffer (+0x20 and +0x24 point at unk_10, and the NUL is stored at the pointer that
// +0x20 holds after the call), and m_popped and m_loadMainMenu are cleared. The vptr is written by the compiler.
// Not reproduced: the call std::priv::_String_base::_M_allocate_block(this+0x10, 16). It is STL runtime, out of scope,
// and it can change the finish and end pointers. The asm re-reads +0x20 after the call, so the NUL goes to the
// pointer the call left there. This body assumes that pointer is still unk_10, and that is unverified. unk_24 keeps
// its pre-call value.
GSFlashMenu::GSFlashMenu() {
    m_manager = nullptr;                               // +0x04
    m_menu = nullptr;                                  // +0x08
    m_pendingMenu = nullptr;                           // +0x0c
    unk_20 = reinterpret_cast<char*>(unk_10);          // +0x20 string finish, the inline buffer
    unk_24 = reinterpret_cast<char*>(unk_10);          // +0x24 string end, before _M_allocate_block
    *unk_20 = '\0';                                    // empty string
    m_popped = false;                                  // +0x28
    m_loadMainMenu = false;                            // +0x29
}

// 0x3842ec GSFlashMenu::~GSFlashMenu()  (D0, scalar deleting destructor)
// Not written: compiler-generated from the virtual destructor. Asm: BL D1 (0x384284), then BL CustomFree(this).
// C++ delete covers it, the same convention as GSEndGame and GSInit.

// 0x384284 GSFlashMenu::~GSFlashMenu()  (D1 variant)
// 0x3842b8 GSFlashMenu::~GSFlashMenu()  (D2 variant, same body)
// PARTIAL. Asm (D1): the vptr is stored with post-increment, so the string sits at this+0x10, and then
// std::priv::_String_base::_M_deallocate_block runs on it. That STL call is not reproduced. For the inline buffer the
// constructor sets up, nothing is freed, and no code in this batch grows the string. The base destructor
// StateBase::~StateBase runs after this body, as the compiler inserts it.
GSFlashMenu::~GSFlashMenu() {}

// 0x384460 GSFlashMenu::Update(StateMachine*, double)
// PARTIAL. Reproduced: the menu swap at the top of the body. When m_pendingMenu is set and differs from m_menu, the
// original pops the old menu, reads m_pendingMenu again, clears it, stores the value in m_menu and pushes it.
// Not reproduced:
//   (a) the pop and the push. Both are virtual calls on m_manager->m_multiMenuManager: the vtable slot +0x3c,
//       PopMenu(const char*, bool), gets m_menu->m_name and false, and slot +0x34, PushMenu(MenuBase*), gets the new
//       menu. dh2/MultiMenuManager.hpp is absent. The reconstruction keeps the stores and omits the calls.
//   (b) everything after the swap. The original tests whether this state is the top of the StateMachine stack. That
//       test reads StateMachine::m_stack (begin +0x0c, end +0x10), which is private, and no accessor is declared.
//       When this state is on top and m_menu is null or hidden, the pop path runs: the trace probe, m_popped = true, then
//       ApplicationGetInstance().m_stateMachine->PopState(nullptr). Otherwise the body runs MenuManager::Update(false)
//       and GetOnline(), then StateMachine::RecurseUpdate(dt) when the COnline byte at +0x05 (isOnlineGame_, private
//       with no getter) is set. Without the top check, none of that can be decided here.
void GSFlashMenu::Update(StateMachine* /*machine*/, double /*dt*/) {
    if (m_pendingMenu != nullptr && m_pendingMenu != m_menu) {
        // PARTIAL (a): when m_menu is set, m_manager->m_multiMenuManager->PopMenu(m_menu->m_name, false) runs here.
        m_menu = m_pendingMenu;
        m_pendingMenu = nullptr;
        // PARTIAL (a): then m_manager->m_multiMenuManager->PushMenu(m_menu) runs here.
    }
    // PARTIAL (b): the rest of the body is not reproduced, see above.
}

// 0x3845f8 GSFlashMenu::Dtor(StateMachine const*)
// PARTIAL. Reproduced: the trace probe; the current level's flag at +0x198 (Level::m_isInGameView) is set when there is a
// level; m_menu is cleared; m_popped is set. Not reproduced: when m_menu is visible, the original first calls
// m_manager->m_multiMenuManager->PopMenu(m_menu->m_name, false) (vtable +0x3c). dh2/MultiMenuManager.hpp is absent.
// The MenuBase* the original returns is not reproduced: StateBase::Dtor is void.
void GSFlashMenu::Dtor(const StateMachine* /*machine*/) {
    TraceGSFlashMenuSwitch();
    if (Level* level = ApplicationGetInstance().GetCurrentLevel()) {
        level->m_isInGameView = true;                  // +0x198
    }
    if (m_menu != nullptr && m_menu->IsVisible()) {
        // PARTIAL: m_manager->m_multiMenuManager->PopMenu(m_menu->m_name, false) runs here.
    }
    m_menu = nullptr;                                  // +0x08
    m_popped = true;                                   // +0x28
}

// 0x3846fc GSFlashMenu::Ctor(StateMachine const*)
// PARTIAL. Reproduced: the trace probe; m_manager = MenuManager::GetInstance(); when m_loadMainMenu is set, the main
// menu is loaded on the manager (LoadMainMenu; its return value is unused, and GetInstance ignores the argument the
// original passes), m_pendingMenu = GetMenuByName("menu_MainMenu") and m_loadMainMenu is cleared; m_popped is cleared.
// Not reproduced: when m_menu is set, the original calls m_manager->m_multiMenuManager->PushMenu(m_menu), vtable +0x34
// (asm: r1 holds m_menu at the call). dh2/MultiMenuManager.hpp is absent. The int return of the original is not
// reproduced: StateBase::Ctor is void.
void GSFlashMenu::Ctor(const StateMachine* /*machine*/) {
    TraceGSFlashMenuSwitch();
    MenuManager* manager = MenuManager::GetInstance();
    m_manager = manager;                               // +0x04
    if (m_loadMainMenu) {
        manager->LoadMainMenu();
        m_pendingMenu = MenuManager::GetInstance()->GetMenuByName("menu_MainMenu");  // +0x0c
        m_loadMainMenu = false;                        // +0x29
    }
    if (m_menu != nullptr) {
        // PARTIAL: m_manager->m_multiMenuManager->PushMenu(m_menu) runs here.
    }
    m_popped = false;                                  // +0x28
}
