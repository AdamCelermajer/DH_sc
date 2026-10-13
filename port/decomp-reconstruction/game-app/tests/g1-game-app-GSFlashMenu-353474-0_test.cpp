// Test for batch g1-game-app-GSFlashMenu-353474-0. Covers Draw, the Ctor and Dtor state changes, and the menu swap at the
// top of Update. The engine symbols the batch calls (MenuManager, MenuBase::IsVisible, DebugSwitches, Application and
// the StateBase virtuals) are stubbed here, so the test links without the engine. Exit status 0 means every check passed.
#include <cstdio>
#include <cstring>
#include <string>

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/global/ctrl_camera.hpp"

namespace {

int g_failures = 0;
int g_drawCount = 0;             // MenuManager::Draw calls
bool g_drawHudOnly = true;       // hudOnly passed to the last Draw
int g_loadMainMenuCount = 0;     // MenuManager::LoadMainMenu calls
int g_loadCount = 0;             // DebugSwitches::load calls
int g_switchCount = 0;           // DebugSwitches::GetSwitch calls
std::string g_lastSwitchName;    // name passed to the last GetSwitch
Level* g_currentLevel = nullptr; // what Application::GetCurrentLevel returns
int g_managerTag = 0;            // address stands in for the MenuManager singleton
int g_mainTag = 0;               // address stands in for the "menu_MainMenu" MenuBase
int g_aboutTag = 0;              // address stands in for a second MenuBase
int g_switchesTag = 0;           // address stands in for the DebugSwitches singleton
int g_appTag = 0;                // address stands in for the Application singleton

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

void ResetCounters() {
    g_drawCount = 0;
    g_drawHudOnly = true;
    g_loadMainMenuCount = 0;
    g_loadCount = 0;
    g_switchCount = 0;
    g_lastSwitchName.clear();
}

MenuManager* ManagerTag() {
    return reinterpret_cast<MenuManager*>(&g_managerTag);
}

MenuBase* MainMenuTag() {
    return reinterpret_cast<MenuBase*>(&g_mainTag);
}

MenuBase* AboutTag() {
    return reinterpret_cast<MenuBase*>(&g_aboutTag);
}

}  // namespace

// ---- stubs for the engine symbols the batch references ----

MenuManager* MenuManager::GetInstance() {
    return ManagerTag();
}

void MenuManager::LoadMainMenu() {
    ++g_loadMainMenuCount;
}

MenuBase* MenuManager::GetMenuByName(const char* name) {
    if (std::strcmp(name, "menu_MainMenu") == 0) {
        return MainMenuTag();
    }
    return nullptr;
}

void MenuManager::Draw(bool hudOnly) {
    ++g_drawCount;
    g_drawHudOnly = hudOnly;
}

bool MenuBase::IsVisible() const {
    return false;  // no menu in these checks is visible
}

DebugSwitches* DebugSwitches::GetInstance() {
    return reinterpret_cast<DebugSwitches*>(&g_switchesTag);
}

void DebugSwitches::load() {
    ++g_loadCount;
}

bool DebugSwitches::GetSwitch(const std::string& name) {
    ++g_switchCount;
    g_lastSwitchName = name;
    return false;
}

Application& ApplicationGetInstance() {
    return *reinterpret_cast<Application*>(&g_appTag);
}

Level* Application::GetCurrentLevel() const {
    return g_currentLevel;
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

int main() {
    // Draw: MenuManager::Draw(false) once, through m_manager.
    {
        GSFlashMenu menu;
        ResetCounters();
        menu.m_manager = ManagerTag();
        menu.Draw(nullptr);
        Check(g_drawCount == 1 && !g_drawHudOnly, "Draw calls MenuManager::Draw(false) once");
    }

    // Ctor with the main-menu flag: stores the manager, loads the main menu, sets the pending menu, clears the flag.
    {
        GSFlashMenu menu;
        ResetCounters();
        menu.m_loadMainMenu = true;
        menu.m_popped = true;
        menu.m_menu = nullptr;
        menu.Ctor(nullptr);
        Check(menu.m_manager == ManagerTag(), "Ctor stores MenuManager::GetInstance() in m_manager");
        Check(g_loadMainMenuCount == 1, "Ctor loads the main menu once when the flag is set");
        Check(menu.m_pendingMenu == MainMenuTag(), "Ctor sets m_pendingMenu to menu_MainMenu");
        Check(!menu.m_loadMainMenu, "Ctor clears m_loadMainMenu");
        Check(!menu.m_popped, "Ctor clears m_popped");
        Check(g_loadCount == 1 && g_switchCount == 1 && g_lastSwitchName == "isTracingGSFlashMenu",
              "Ctor runs the trace probe on isTracingGSFlashMenu");
    }

    // Ctor without the flag: no main-menu load, m_pendingMenu unchanged.
    {
        GSFlashMenu menu;
        ResetCounters();
        menu.m_loadMainMenu = false;
        menu.m_pendingMenu = AboutTag();
        menu.Ctor(nullptr);
        Check(g_loadMainMenuCount == 0, "Ctor without the flag does not load the main menu");
        Check(menu.m_pendingMenu == AboutTag(), "Ctor without the flag keeps m_pendingMenu");
    }

    // Dtor with a level: sets the level's in-game-view flag, clears m_menu, sets m_popped.
    {
        alignas(Level) unsigned char levelBytes[sizeof(Level)] = {};
        Level* level = reinterpret_cast<Level*>(levelBytes);
        g_currentLevel = level;
        GSFlashMenu menu;
        ResetCounters();
        menu.m_menu = nullptr;
        menu.m_popped = false;
        menu.Dtor(nullptr);
        Check(level->m_isInGameView, "Dtor sets the level's in-game-view flag");
        Check(menu.m_menu == nullptr && menu.m_popped, "Dtor clears m_menu and sets m_popped");
        Check(g_loadCount == 1 && g_lastSwitchName == "isTracingGSFlashMenu", "Dtor runs the trace probe");
        g_currentLevel = nullptr;
    }

    // Dtor without a level: m_popped is still set and nothing else changes.
    {
        g_currentLevel = nullptr;
        GSFlashMenu menu;
        ResetCounters();
        menu.m_menu = nullptr;
        menu.m_popped = false;
        menu.Dtor(nullptr);
        Check(menu.m_popped && menu.m_menu == nullptr, "Dtor without a level sets m_popped");
    }

    // Update: a pending menu that differs from m_menu (none set) is swapped in and m_pendingMenu is cleared.
    {
        GSFlashMenu menu;
        menu.m_menu = nullptr;
        menu.m_pendingMenu = AboutTag();
        menu.Update(nullptr, 0.0);
        Check(menu.m_menu == AboutTag(), "Update swaps the pending menu in");
        Check(menu.m_pendingMenu == nullptr, "Update clears m_pendingMenu after the swap");
    }

    // Update: pending equal to the current menu, or no pending menu, changes nothing.
    {
        GSFlashMenu menu;
        menu.m_menu = AboutTag();
        menu.m_pendingMenu = AboutTag();
        menu.Update(nullptr, 0.0);
        Check(menu.m_menu == AboutTag() && menu.m_pendingMenu == AboutTag(),
              "Update with pending equal to the current menu changes nothing");
        menu.m_pendingMenu = nullptr;
        menu.Update(nullptr, 0.0);
        Check(menu.m_menu == AboutTag() && menu.m_pendingMenu == nullptr,
              "Update without a pending menu changes nothing");
    }

    if (g_failures == 0) {
        std::printf("GSFlashMenu: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
