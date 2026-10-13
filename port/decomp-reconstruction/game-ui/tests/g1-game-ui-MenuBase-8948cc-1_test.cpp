// Checks the MenuBase FSCommand handlers of batch g1-game-ui-MenuBase-8948cc-1 on the paths that run without the
// engine: the null and empty argument checks, the "undefined" level name, and the return values the original
// produces on paths that do not depend on a missing call. The handlers that touch the menu or the engine singletons
// are not called here.
#include <cstdio>

#include "dh2/MenuBase.hpp"

// Link-only definition: FS_LoadLevel calls MenuFX::PopAll on the menu, and the menu is not built here.
void MenuFX::PopAll() {}

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
    // Handlers whose return value is fixed on every path (the menu is not dereferenced).
    Check(MenuBase::FS_PopAllMenu("", "", nullptr), "FS_PopAllMenu returns true");
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
