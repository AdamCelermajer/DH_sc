// Checks the MenuBase FSCommand handlers of batch g1-game-ui-MenuBase-8948cc-1 on the paths that run without the
// engine: the null and empty argument checks, the "undefined" level name, and the return values the original
// produces on paths that do not depend on a missing call. The handlers that reach the engine singletons run against
// the link-only stubs below, which record nothing; the calls themselves are checked by
// r2a-game-ui-MenuBase-8948cc-0_test.cpp. FS_PopAllMenu is not called here: it makes a virtual call on a
// MultiMenuManager that this test cannot build.
#include <cstdint>
#include <cstdio>

#include "dh2/Application.hpp"
#include "dh2/DragAndDrop.hpp"
#include "dh2/Dragable.hpp"
#include "dh2/Level.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/global/ctrl_camera.hpp"

// Link-only definition: FS_LoadLevel calls MenuFX::PopAll on the menu, and the menu is not built here.
void MenuFX::PopAll() {}

// ---- Fake singleton storage. These handlers read only the SavegameManager pointer and its m_lastSlot.
namespace {
alignas(Application) unsigned char g_appStorage[sizeof(Application)];
alignas(SavegameManager) unsigned char g_saveStorage[sizeof(SavegameManager)];
}  // namespace

Application& ApplicationGetInstance() {
    return *reinterpret_cast<Application*>(g_appStorage);
}

// ---- Link-only stubs for the singleton calls the handlers make. They do nothing.
MenuManager* MenuManager::GetInstance() {
    return nullptr;
}
void SavegameManager::loadSettings(bool) {}
void SavegameManager::saveSettings() {}
int SavegameManager::getLanguage() const {
    return 0;
}
void SavegameManager::setLanguage(int) {}
void SavegameManager::setOption(const char*, int) {}
void Application::LoadLevel(const char*, int, std::uint32_t, bool, bool, int, bool, std::uint32_t, std::uint32_t) {}
void Application::LoadWorldMap(int) {}
void Application::GoToMainMenu(GoToMainMenuEvent) {}
Level* Application::GetCurrentLevel() const {
    return nullptr;
}
Dragable* DragAndDrop::GetDragable(const char*) {
    return nullptr;
}
int Dragable::ResetPosition() {
    return 0;
}
int PlayerSavegame::SG_Delete(std::uint32_t) {
    return 0;
}
int LevelSavegame::DeleteAllLevelSave(unsigned int) {
    return 0;
}
void ScriptManager::SkipScript(int, bool) {}
ScriptManager::ScriptManager() {}
ScriptManager::~ScriptManager() {}
ScriptManager ScriptManager::s_inst;

namespace {

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

}  // namespace

int main() {
    // No save slot is set, so FS_ResetSaveFile with no argument takes the m_lastSlot path with -1 and calls nothing.
    ApplicationGetInstance().m_savegameManager = reinterpret_cast<SavegameManager*>(g_saveStorage);
    reinterpret_cast<SavegameManager*>(g_saveStorage)->m_lastSlot = -1;

    // Handlers whose return value is fixed on every path (the menu is not dereferenced).
    Check(MenuBase::FS_SaveOptions("", "", nullptr), "FS_SaveOptions returns true");
    Check(MenuBase::FS_LoadWorldMap("", "", nullptr), "FS_LoadWorldMap returns true");
    Check(MenuBase::FS_GoToMainMenu("", "", nullptr), "FS_GoToMainMenu returns true");
    Check(MenuBase::FS_SkipScript("", "", nullptr), "FS_SkipScript returns true");
    Check(MenuBase::FS_IncOption("", "", nullptr), "FS_IncOption returns true");
    Check(MenuBase::FS_DecOption("", "", nullptr), "FS_DecOption returns true");
    Check(MenuBase::FS_ResetSaveFile("", "", nullptr), "FS_ResetSaveFile returns true");
    Check(MenuBase::FS_GetPlayerName("", "", nullptr), "FS_GetPlayerName returns true");
    Check(!MenuBase::FS_EndLoading("", "", nullptr), "FS_EndLoading returns false");

    // Null and empty argument checks.
    Check(!MenuBase::FS_SetSaveSlot("", nullptr, nullptr), "FS_SetSaveSlot(null) returns false");
    Check(MenuBase::FS_SetSaveSlot("", "3", nullptr), "FS_SetSaveSlot(3) returns true");
    Check(!MenuBase::FS_SetOption("", nullptr, nullptr), "FS_SetOption(null) returns false");
    Check(MenuBase::FS_SetOption("", "option_Sound", nullptr), "FS_SetOption(option_Sound) returns true");
    Check(!MenuBase::FS_ToggleOption("", nullptr, nullptr), "FS_ToggleOption(null) returns false");
    Check(MenuBase::FS_ToggleOption("", "option_Sound", nullptr), "FS_ToggleOption(option_Sound) returns true");
    Check(!MenuBase::FS_GetString("", nullptr, nullptr), "FS_GetString(null) returns false");
    Check(MenuBase::FS_GetString("", "1", nullptr), "FS_GetString(1) returns true");
    Check(!MenuBase::FS_GetString2("", nullptr, nullptr), "FS_GetString2(null) returns false");
    Check(MenuBase::FS_GetString2("", "KEY", nullptr), "FS_GetString2(KEY) returns true");
    Check(!MenuBase::FS_SetLanguage("", nullptr, nullptr), "FS_SetLanguage(null) returns false");
    Check(!MenuBase::FS_SetLanguage("", "", nullptr), "FS_SetLanguage(empty) returns false");

    // FS_SetBtnImage: no '|' separator means no lookup, so the result is false.
    Check(!MenuBase::FS_SetBtnImage("", nullptr, nullptr), "FS_SetBtnImage(null) returns false");
    Check(!MenuBase::FS_SetBtnImage("", "noSeparator", nullptr), "FS_SetBtnImage(no '|') returns false");

    // FS_LoadLevel: "undefined" is the no-op name and returns false before any call on the menu.
    Check(!MenuBase::FS_LoadLevel("", nullptr, nullptr), "FS_LoadLevel(null) returns false");
    Check(!MenuBase::FS_LoadLevel("", "undefined", nullptr), "FS_LoadLevel(undefined) returns false");

    if (g_failures == 0) {
        std::printf("MenuBase batch g1-game-ui-MenuBase-8948cc-1: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
