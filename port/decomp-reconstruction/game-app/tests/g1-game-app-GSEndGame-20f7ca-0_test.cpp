// Test for batch g1-game-app-GSEndGame-20f7ca-0. Covers the phase counter of GSEndGame: the constructor, Ctor, Dtor,
// the Update phase dispatch, the hand-over to GSFlashMenu and Draw. The engine symbols the batch calls (MenuManager,
// MenuBase, COnline, StateBase, StateMachine, PlayerManager, the Application object and the GSFlashMenu::s_inst object)
// are stubbed here, so the test links without the engine. Exit status 0 means every check passed.
#include <cstdio>
#include <cstring>

#include "dh2/Application.hpp"
#include "dh2/GSEndGame.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "net-session-online/c_online.hpp"

namespace {

int g_failures = 0;
int g_pushCount = 0;          // MenuManager::PushMenu("menu_About")
int g_updateCount = 0;        // MenuManager::Update(hudOnly)
int g_lastUpdateArg = -1;     // hudOnly passed to the last Update
int g_drawCount = 0;          // MenuManager::Draw(hudOnly)
int g_switchCount = 0;        // StateMachine::SwitchState
StateBase* g_lastSwitchState = nullptr;
bool g_lastSwitchOwned = true;
int g_removeAllCount = 0;     // PlayerManager::RemoveAllPlayers
bool g_aboutVisible = false;
int g_aboutTag = 0;           // address stands in for the "menu_About" MenuBase
int g_mainTag = 0;            // address stands in for the "menu_MainMenu" MenuBase
int g_managerTag = 0;         // address stands in for the MenuManager singleton
int g_machineTag = 0;         // address stands in for the StateMachine the Application owns
int g_playerTag = 0;          // address stands in for the PlayerManager the Application owns

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

void ResetCounters() {
    g_pushCount = 0;
    g_updateCount = 0;
    g_lastUpdateArg = -1;
    g_drawCount = 0;
    g_switchCount = 0;
    g_lastSwitchState = nullptr;
    g_lastSwitchOwned = true;
    g_removeAllCount = 0;
}

void ResetPending() {
    GSFlashMenu::s_inst.m_pendingMenu = nullptr;
    GSFlashMenu::s_inst.m_loadMainMenu = false;
}

// The Application object the batch reaches through ApplicationGetInstance(). Only its two pointer members are used.
Application g_application;

}  // namespace

// ---- stubs for the engine symbols the batch references ----

Application::Application() {}
Application::~Application() {}
bool Application::onEvent(const glitch::SEvent&) {
    return false;
}

Application& ApplicationGetInstance() {
    return g_application;
}

MenuManager* MenuManager::GetInstance() {
    return reinterpret_cast<MenuManager*>(&g_managerTag);
}

void MenuManager::PushMenu(const char* name) {
    if (std::strcmp(name, "menu_About") == 0) {
        ++g_pushCount;
    }
}

MenuBase* MenuManager::GetMenuByName(const char* name) {
    if (std::strcmp(name, "menu_About") == 0) {
        return reinterpret_cast<MenuBase*>(&g_aboutTag);
    }
    if (std::strcmp(name, "menu_MainMenu") == 0) {
        return reinterpret_cast<MenuBase*>(&g_mainTag);
    }
    return nullptr;
}

void MenuManager::Update(bool hudOnly) {
    ++g_updateCount;
    g_lastUpdateArg = hudOnly ? 1 : 0;
}

void MenuManager::Draw(bool) {
    ++g_drawCount;
}

bool MenuBase::IsVisible() const {
    return reinterpret_cast<const MenuBase*>(&g_aboutTag) == this && g_aboutVisible;
}

COnline* COnline::GetInstance() {
    return nullptr;  // the test never dereferences it
}

void StateMachine::SwitchState(StateBase* state, bool owned) {
    ++g_switchCount;
    g_lastSwitchState = state;
    g_lastSwitchOwned = owned;
}

int PlayerManager::RemoveAllPlayers() {
    ++g_removeAllCount;
    return 0;
}

StateBase::~StateBase() {}
void StateBase::Ctor(const StateMachine*) {}
void StateBase::Dtor(const StateMachine*) {}
void StateBase::Update(StateMachine*, double) {}
void StateBase::Draw(const StateMachine*) {}
void StateBase::Pause(const StateMachine*) {}
void StateBase::Resume(const StateMachine*) {}
void StateBase::Draw2D(const StateMachine*) {}
void StateBase::Sleep(const StateMachine*) {}
void StateBase::WakeUp(const StateMachine*) {}

GSFlashMenu::GSFlashMenu() {}
GSFlashMenu::~GSFlashMenu() {}
void GSFlashMenu::Ctor(const StateMachine*) {}
void GSFlashMenu::Dtor(const StateMachine*) {}
void GSFlashMenu::Update(StateMachine*, double) {}
void GSFlashMenu::Draw(const StateMachine*) {}

GSFlashMenu GSFlashMenu::s_inst;

int main() {
    g_application.m_stateMachine = reinterpret_cast<StateMachine*>(&g_machineTag);
    g_application.m_playerManager = reinterpret_cast<PlayerManager*>(&g_playerTag);

    // Constructor, Ctor and Dtor.
    {
        GSEndGame endGame;
        Check(endGame.m_phase == 0, "constructor sets m_phase to 0");

        endGame.m_phase = 7;
        endGame.Ctor(nullptr);
        Check(endGame.m_phase == 0, "Ctor sets m_phase to 0");

        endGame.m_phase = 3;
        endGame.Dtor(nullptr);
        Check(endGame.m_phase == 3, "Dtor leaves m_phase alone");
    }

    // Phase 0: pushes menu_About, advances to phase 1 and ends with MenuManager::Update(false).
    {
        GSEndGame endGame;
        ResetCounters();
        ResetPending();
        endGame.m_phase = 0;
        endGame.Update(nullptr, 0.0);
        Check(endGame.m_phase == 1, "phase 0 advances to phase 1");
        Check(g_pushCount == 1, "phase 0 pushes menu_About once");
        Check(g_updateCount == 1 && g_lastUpdateArg == 0, "phase 0 ends with MenuManager::Update(false)");
        Check(g_switchCount == 0 && g_removeAllCount == 0, "phase 0 does not switch state or remove players");
        Check(GSFlashMenu::s_inst.m_pendingMenu == nullptr && !GSFlashMenu::s_inst.m_loadMainMenu,
              "phase 0 does not touch the main-menu hand-over");
    }

    // Phase 1 with About still visible: the phase holds and nothing else happens.
    {
        GSEndGame endGame;
        ResetCounters();
        ResetPending();
        endGame.m_phase = 1;
        g_aboutVisible = true;
        endGame.Update(nullptr, 0.0);
        Check(endGame.m_phase == 1, "phase 1 holds while About is visible");
        Check(g_pushCount == 0, "phase 1 does not push a menu");
        Check(g_updateCount == 1 && g_lastUpdateArg == 0, "phase 1 visible ends with MenuManager::Update(false)");
        Check(g_switchCount == 0 && g_removeAllCount == 0, "phase 1 visible does not switch state");
        Check(GSFlashMenu::s_inst.m_pendingMenu == nullptr && !GSFlashMenu::s_inst.m_loadMainMenu,
              "phase 1 visible leaves the main-menu hand-over alone");
        g_aboutVisible = false;
    }

    // Phase 1 with About hidden: falls into the phase-2 steps and keeps m_phase at 1.
    {
        GSEndGame endGame;
        ResetCounters();
        ResetPending();
        endGame.m_phase = 1;
        g_aboutVisible = false;
        endGame.Update(nullptr, 0.0);
        Check(endGame.m_phase == 1, "phase 1 with About hidden keeps m_phase at 1");
        Check(GSFlashMenu::s_inst.m_pendingMenu == reinterpret_cast<MenuBase*>(&g_mainTag),
              "phase 1 hidden sets m_pendingMenu to menu_MainMenu");
        Check(GSFlashMenu::s_inst.m_loadMainMenu, "phase 1 hidden sets m_loadMainMenu");
        Check(g_switchCount == 1 && g_lastSwitchState == &GSFlashMenu::s_inst && !g_lastSwitchOwned,
              "phase 1 hidden switches to GSFlashMenu::s_inst, not owned");
        Check(g_removeAllCount == 1, "phase 1 hidden removes all players");
        Check(g_updateCount == 1 && g_lastUpdateArg == 0, "phase 1 hidden ends with MenuManager::Update(false)");
    }

    // Phase 2: the same hand-over steps, m_phase unchanged.
    {
        GSEndGame endGame;
        ResetCounters();
        ResetPending();
        endGame.m_phase = 2;
        endGame.Update(nullptr, 0.0);
        Check(endGame.m_phase == 2, "phase 2 keeps m_phase at 2");
        Check(GSFlashMenu::s_inst.m_pendingMenu == reinterpret_cast<MenuBase*>(&g_mainTag),
              "phase 2 sets m_pendingMenu to menu_MainMenu");
        Check(GSFlashMenu::s_inst.m_loadMainMenu, "phase 2 sets m_loadMainMenu");
        Check(g_pushCount == 0, "phase 2 does not push a menu");
        Check(g_switchCount == 1 && g_lastSwitchState == &GSFlashMenu::s_inst && !g_lastSwitchOwned,
              "phase 2 switches to GSFlashMenu::s_inst, not owned");
        Check(g_removeAllCount == 1, "phase 2 removes all players");
        Check(g_updateCount == 1 && g_lastUpdateArg == 0, "phase 2 ends with MenuManager::Update(false)");
    }

    // Any other phase: only MenuManager::Update(false) runs.
    {
        GSEndGame endGame;
        ResetCounters();
        ResetPending();
        endGame.m_phase = 5;
        endGame.Update(nullptr, 0.0);
        Check(endGame.m_phase == 5, "other phases leave m_phase alone");
        Check(g_pushCount == 0, "other phases do not push a menu");
        Check(g_updateCount == 1 && g_lastUpdateArg == 0, "other phases end with MenuManager::Update(false)");
        Check(GSFlashMenu::s_inst.m_pendingMenu == nullptr && !GSFlashMenu::s_inst.m_loadMainMenu,
              "other phases leave the main-menu hand-over alone");
        Check(g_switchCount == 0 && g_removeAllCount == 0, "other phases do not switch state");
    }

    // Draw: MenuManager::Draw(false) once, no state change.
    {
        GSEndGame endGame;
        ResetCounters();
        endGame.m_phase = 1;
        endGame.Draw(nullptr);
        Check(g_drawCount == 1, "Draw calls MenuManager::Draw once");
        Check(endGame.m_phase == 1, "Draw leaves m_phase alone");
    }

    if (g_failures == 0) {
        std::printf("all GSEndGame checks passed\n");
        return 0;
    }
    std::printf("%d GSEndGame check(s) failed\n", g_failures);
    return 1;
}
