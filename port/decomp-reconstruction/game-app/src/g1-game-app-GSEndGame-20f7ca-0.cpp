// Batch g1-game-app-GSEndGame-20f7ca-0 (GSEndGame, part 1 of 1).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// The C1/C2 constructor pair and the D1/D2 destructor pair have identical bodies, so each pair has one
// definition here. The scalar deleting destructor (D0) is compiler-generated and is not written.
#include "dh2/GSEndGame.hpp"

#include "dh2/Application.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "net-session-online/c_online.hpp"

// 0x38358c GSEndGame::GSEndGame()
// 0x3835b8 GSEndGame::GSEndGame()  (C1 variant, same body)
// Asm (0x38358c): stores 0 to +0x04, then the vtable address point (_ZTV9GSEndGame + 8) to +0x00. The compiler
// writes the vptr from the class; StateBase has no constructor body of its own.
GSEndGame::GSEndGame() : m_phase(0) {}

// GSEndGame::s_inst (ELF 0x9a245c, 8 bytes): the state object Script_EndGame switches to. Constructed by the static initializer through GSEndGame().
GSEndGame GSEndGame::s_inst;

// 0x3835e4 GSEndGame::~GSEndGame()  (D2 variant)
// 0x3835e8 GSEndGame::~GSEndGame()  (D1 variant, same body)
// Both are a bare return (asm: BX LR). The base StateBase::~StateBase (0x38324c) is also a bare return, so the
// base destructor call that the compiler inserts changes nothing.
GSEndGame::~GSEndGame() {}

// 0x3835ec GSEndGame::Ctor(StateMachine const*)
// Asm: MOV R3,#0; STR R3,[R0,#4]; BX LR. Sets m_phase (+0x04) to 0. The original leaves r0 (this) as it was, so
// the decompiled int return is this. StateBase::Ctor is void in the slot contract, so that value is not reproduced.
void GSEndGame::Ctor(const StateMachine* /*machine*/) {
    m_phase = 0;
}

// 0x3835f8 GSEndGame::Dtor(StateMachine const*)
// Asm: BX LR. Empty.
void GSEndGame::Dtor(const StateMachine* /*machine*/) {}

// 0x3835fc GSEndGame::~GSEndGame()  (D0, scalar deleting destructor)
// Not written: compiler-generated from the virtual destructor. Asm: BL D1 (0x3835e8), then BL CustomFree(this).
// C++ delete covers it, the same convention as DataReloaderManager.

// 0x383618 GSEndGame::Draw(StateMachine const*)
// Asm: BL MenuManager::GetInstance; MOV R1,#0; B MenuManager::Draw(bool).
void GSEndGame::Draw(const StateMachine* /*machine*/) {
    MenuManager::GetInstance()->Draw(false);
}

// 0x383fbc GSEndGame::Update(StateMachine*, double)
// PARTIAL. The phase dispatch, the About menu checks, the MainMenu hand-over, the state switch and the final
// MenuManager::Update are reconstructed, and so is MultiMenuManager::PopMenu(true) (redo: dh2/MultiMenuManager.hpp exists).
// Two parts are not, each marked at its place in the body:
//   (a) phase 0: the test of COnline +0x05 (isOnlineGame_) and the online clean-up. COnline keeps that flag private
//       with no getter in net-session-online/c_online.hpp. CMatching::Destroy is not declared in
//       net-session-online/c_matching_base.hpp, and the CMatching vtable slot +0x3c is not identified.
//   (b) phase 2: the message queue flushes and drains. Singleton<MenuMessageManager<T,N>>::s_inst is not instantiated in
//       game-app/src/Globals.cpp for these four instances, and MenuMessageManager<DialogMsg,1>::FlushEnqueuedMessages
//       (the clone with queue 0) has no definition in the tree.
// Update does not use its StateMachine argument or dt; the original reads only m_phase, the menu objects and Application.
void GSEndGame::Update(StateMachine* /*machine*/, double /*dt*/) {
    if (m_phase == 1) {
        // Phase 1 waits for the About menu to close. The original calls IsVisible on the result of GetMenuByName
        // without testing it for null, and so does this.
        MenuBase* aboutMenu = MenuManager::GetInstance()->GetMenuByName("menu_About");
        if (aboutMenu->IsVisible()) {
            MenuManager::GetInstance()->Update(false);
            return;
        }
        // About is hidden: the original falls into the phase-2 steps below and leaves m_phase at 1.
    } else if (m_phase != 2) {
        if (m_phase == 0) {
            // PARTIAL (a): the original tests GetOnline()->isOnlineGame_ (COnline +0x05). That flag is private and
            // has no getter in dh2 or net-session-online (layout request: COnline +0x05 bool getter). When it is set,
            // the original runs the online clean-up: CMatching::Get()->vtable +0x3c, CMatching::Get(),
            // CMatching::SetMatchingProvider(1), CMatching::Get() and CMatching::Destroy(), COnline::SetIsOnlineGame(false),
            // and OnlineSingleton<OnlineGameState>::GetInstance()->+0x28 = 0. That branch is not run here.
            // GetOnline() is still called, because the original's call creates the COnline singleton on first use.
            COnline::GetInstance();
            MenuManager::GetInstance()->PushMenu("menu_About");
            ++m_phase;  // +0x04
        }
        MenuManager::GetInstance()->Update(false);
        return;
    }

    // Phase 2, also reached from phase 1 when the About menu is hidden.
    // PARTIAL (b): the original runs MenuMessageManager<DialogMsg,1>::FlushEnqueuedMessages(0), drains the AchievementMsg
    // queue, flushes DialogMsg again, then drains the StatusMsg and OnlineStatusMsg queues. Each drain is
    // "while (!empty) pop_front()" on the std::deque at +0x04 of the static MenuMessageManager object (m_queues[0] of
    // dh2/MenuMessageManager.hpp; ELF 0x99fbe0 AchievementMsg, 0x99fa8c StatusMsg, 0x99fb30 OnlineStatusMsg). Left out
    // here: those instances are not instantiated in game-app/src/Globals.cpp, and the flush has no definition.
    // The MultiMenuManager pop follows the drains in the original.
    MultiMenuManager* multiMenus = MenuManager::GetInstance()->m_multiMenuManager;
    multiMenus->PopMenu(true);                      // vtable +0x38 on MenuManager +0xf4; true keeps popping
    MenuBase* mainMenu = MenuManager::GetInstance()->GetMenuByName("menu_MainMenu");
    GSFlashMenu::s_inst.m_pendingMenu = mainMenu;   // GSFlashMenu +0x0c (ELF dword_9A247C)
    GSFlashMenu::s_inst.m_loadMainMenu = true;      // GSFlashMenu +0x29 (ELF byte_9A2499)
    Application& application = ApplicationGetInstance();
    application.m_stateMachine->SwitchState(&GSFlashMenu::s_inst, false);  // Application +0x18
    application.m_playerManager->RemoveAllPlayers();                        // Application +0x40
    MenuManager::GetInstance()->Update(false);
}
