// MenuBase bodies, batch g1-game-ui-MenuBase-8948cc-1 (part 2 of 5 of MenuBase): the Flash FSCommand
// handlers from FS_PopAllMenu through FS_ResetSaveFile. Rebuilt from the Hex-Rays pseudocode of
// libDungeonHunter2.so and checked against the ARM assembly where the pseudocode mistypes a call.
// Offsets in comments are the original 32-bit ARM offsets.
//
// Facts shared by the handlers:
//   - A handler is (command, args, menu). MenuBase::MyFSCommand passes the MenuBase as menu, so
//     static_cast<MenuBase*>(menu)->m_menu (State +0x04, the owning MenuFX) is the RenderFX the original uses.
//   - The Hex-Rays globals dword_99F758, dword_99F760, dword_99F778 and dword_99F780 are fields of the
//     Application singleton (Singleton<Application>::s_inst at 0x99f72c): +0x2c PyDataConstants,
//     +0x34 StringManager, +0x4c SavegameManager, +0x54 MenuManager (MenuManager::GetInstance returns that field).
//   - LC_API_ATOI_0 is atoi (its body at 0x30e094 is a bare atoi call) and LC_API_STRCMP_0 is strcmp.
//   - The singleton is reached through ApplicationGetInstance() (dh2/global/ctrl_camera.hpp). The managers these
//     handlers use (SavegameManager, ScriptManager, MultiMenuManager, DragAndDrop, Dragable, PlayerSavegame,
//     LevelSavegame) are declared in their own headers. Calls on what is still not declared (StringManager,
//     PyDataConstants, gameswf::as_value, the RenderFX methods Find, PlayAnim, SetMember and InvokeASCallback) are
//     left out and marked PARTIAL, and the function returns what the original returns on that path. Nothing is guessed.

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "dh2/Application.hpp"
#include "dh2/DragAndDrop.hpp"
#include "dh2/Dragable.hpp"
#include "dh2/Level.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/global/ctrl_camera.hpp"

// 0x4203e4 _ZN8MenuBase13FS_PopAllMenuEPKcS1_Pv
// The original calls the virtual at vtable +0x38 of MenuManager::GetInstance()->m_multiMenuManager (+0xf4 of
// MenuManager) with argument 1, which is MultiMenuManager::PopMenu(bool repeat) with repeat = true. Returns true.
bool MenuBase::FS_PopAllMenu(const char*, const char*, void*) {
    MenuManager::GetInstance()->m_multiMenuManager->PopMenu(true);
    return true;
}

// 0x42040c _ZN8MenuBase14FS_SetBtnImageEPKcS1_Pv
// PARTIAL: args is "<movie>|<frame>". The original copies the movie name (the bytes before '|') into a 100-byte
// buffer, calls RenderFX::Find on the menu's m_menu with it, and when that finds a character returns
// GameSWFUtils::GotoFrame(found, frame), where frame is the text after '|'. RenderFX::Find and GameSWFUtils are not
// declared in the tree, so the lookup is left out and the function returns false. A null args returns false here;
// the original calls strchr on it.
bool MenuBase::FS_SetBtnImage(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    const char* bar = std::strchr(args, '|');
    if (!bar) {
        return false;
    }
    return false;  // PARTIAL: Find and GotoFrame not called (see above).
}

// 0x4204bc _ZN8MenuBase14FS_LoadOptionsEPKcS1_Pv
// PARTIAL: loadSettings(false) on the SavegameManager (+0x4c of the Application singleton), then
// setLanguage(getLanguage()) on it. Both calls are made. The original returns setLanguage's int result, but
// dh2/SavegameManager.hpp declares setLanguage void (its binary returns int; see that header), so the result is not
// known and the function returns false.
bool MenuBase::FS_LoadOptions(const char*, const char*, void*) {
    SavegameManager* savegame = ApplicationGetInstance().m_savegameManager;
    savegame->loadSettings(false);
    savegame->setLanguage(savegame->getLanguage());
    return false;  // PARTIAL: the original returns setLanguage's int result (see above).
}

// 0x4204fc _ZN8MenuBase14FS_SaveOptionsEPKcS1_Pv
// saveSettings(), then setLanguage(getLanguage()) on the SavegameManager. The original ignores setLanguage's result
// and returns true.
bool MenuBase::FS_SaveOptions(const char*, const char*, void*) {
    SavegameManager* savegame = ApplicationGetInstance().m_savegameManager;
    savegame->saveSettings();
    savegame->setLanguage(savegame->getLanguage());
    return true;
}

// 0x42053c _ZN8MenuBase14FS_SetSaveSlotEPKcS1_Pv
// A null args returns false. Otherwise the original stores atoi(args) as an int at +0x08 of the SavegameManager
// (m_lastSlot) and returns true.
bool MenuBase::FS_SetSaveSlot(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    ApplicationGetInstance().m_savegameManager->m_lastSlot = std::atoi(args);
    return true;
}

// 0x42057c _ZN8MenuBase14FS_SetLanguageEPKcS1_Pv
// PARTIAL: a null or empty args returns false. Otherwise the original calls setLanguage(atoi(args)) on the
// SavegameManager and tests its int result: zero returns false, nonzero calls ResetFonts on MenuManager::GetInstance()
// and returns true. The call is made. dh2/SavegameManager.hpp declares setLanguage void, so the result is not known and
// the ResetFonts branch is left out; the function returns false.
bool MenuBase::FS_SetLanguage(const char*, const char* args, void*) {
    if (!args || !*args) {
        return false;
    }
    ApplicationGetInstance().m_savegameManager->setLanguage(std::atoi(args));
    return false;  // PARTIAL: the ResetFonts branch needs setLanguage's int result (see above).
}

// 0x4205e0 _ZN8MenuBase12FS_SetOptionEPKcS1_Pv
// A null args returns false. Otherwise the original calls SavegameManager::setOption(args + 7, 0), where the 7 bytes
// skipped are a fixed prefix of the option name, and returns true.
bool MenuBase::FS_SetOption(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    ApplicationGetInstance().m_savegameManager->setOption(args + 7, 0);
    return true;
}

// 0x420624 _ZN8MenuBase15FS_ToggleOptionEPKcS1_Pv
// PARTIAL: the flow is rebuilt; the calls are not. With name = args + 7 (the same 7-byte prefix as FS_SetOption):
//   SavegameManager::toggleOption(name); first = isOptionToggled(name); second = isOptionToggled(name);
//   RenderFX::PlayAnim(m_menu, name, second ? "true" : "false", 0);
//   RenderFX::SetMember(m_menu, "_root", "retval", value), where value is a 12-byte gameswf::as_value with byte 0 = 0,
//     byte 1 = 1 and the bool first at +4;
//   GameSWFUtils::PlayAnim(m_menu, name, "onToggleOption"); gameswf::as_value::drop_refs(value).
// The as_value layout is not declared, and the RenderFX and GameSWFUtils methods are missing. Those calls are left
// out. A null args returns false (the original tests it first). Otherwise returns true.
bool MenuBase::FS_ToggleOption(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    return true;  // PARTIAL: toggle, PlayAnim, SetMember and GameSWFUtils::PlayAnim not called (see above).
}

// 0x420728 _ZN8MenuBase12FS_DecOptionEPKcS1_Pv
// PARTIAL: the language step is rebuilt in the comments below; the calls are not. The original:
//   lang = getLanguage() - 1; if (lang < 0) lang = 7; setLanguage(lang); MenuManager::GetInstance()->ResetFonts();
//   root = menu manager's GetRoot();
//   text = StringManager::getString(getConstant("StrID", "GLOBAL_LANGUAGE_00") + lang);
//   value.set_string(text); RenderFX::InvokeASCallback(root, "_root.menu_language", "currentLanguageText", &value, 1);
//   value.drop_refs(); then returns true.
// The SavegameManager, PyDataConstants, StringManager, RenderFX and gameswf::as_value declarations are missing, so
// all calls are left out. Returns true as in the original.
bool MenuBase::FS_DecOption(const char*, const char*, void*) {
    return true;  // PARTIAL: language change and text update not performed (see above).
}

// 0x420804 _ZN8MenuBase12FS_IncOptionEPKcS1_Pv
// PARTIAL: the same flow as FS_DecOption with lang = getLanguage() + 1, and lang becomes 0 when it exceeds 7. The
// calls are left out for the same reason. Returns true as in the original.
bool MenuBase::FS_IncOption(const char*, const char*, void*) {
    return true;  // PARTIAL: language change and text update not performed (see above).
}

// 0x4208e4 _ZN8MenuBase25FS_ResetDraggablePositionEPKcS1_Pv
// The menu's DragAndDrop (+0x5c, MenuBase::m_dragAndDrop) must be set, or the function returns false. The original then
// calls DragAndDrop::GetDragable(args); a null result returns false, and otherwise Dragable::ResetPosition() runs and
// the function returns true.
bool MenuBase::FS_ResetDraggablePosition(const char*, const char* args, void* menu) {
    DragAndDrop* dragAndDrop = static_cast<MenuBase*>(menu)->m_dragAndDrop;
    if (!dragAndDrop) {
        return false;
    }
    Dragable* dragable = dragAndDrop->GetDragable(args);
    if (!dragable) {
        return false;
    }
    dragable->ResetPosition();  // the int result is ignored, as in the original
    return true;
}

// 0x420914 _ZN8MenuBase12FS_GetStringEPKcS1_Pv
// PARTIAL: a null args returns false. Otherwise the original looks up StringManager::getString(atoi(args)) on the
// Application's m_stringManager (+0x34), and when the string exists calls RenderFX::SetMember(m_menu, "_root",
// "retval", string). It returns true either way. Both calls are left out because StringManager.hpp and the
// three-string SetMember overload are not declared in the tree.
bool MenuBase::FS_GetString(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    return true;  // PARTIAL: lookup and SetMember not called (see above).
}

// 0x420988 _ZN8MenuBase16FS_GetPlayerNameEPKcS1_Pv
// PARTIAL: the original sets "_root.retval" on m_menu to the player name, or to "" when args is empty or the name is
// empty or null, and returns true in all cases. The name comes from PlayerSavegame::SG_GetPlayerName(atoi(args)),
// which takes the slot in r0 (checked in the assembly). PlayerSavegame.hpp and the SetMember overload are not in the
// tree, so the calls are left out. Returns true.
bool MenuBase::FS_GetPlayerName(const char*, const char*, void*) {
    return true;  // PARTIAL: SetMember with the name or "" not called (see above).
}

// 0x420a1c _ZN8MenuBase13FS_GetString2EPKcS1_Pv
// PARTIAL: a null args returns false. Otherwise the original calls StringManager::getStringFromSymbol(args) and,
// when the string is found, RenderFX::SetMember(m_menu, "_root", "retval", string). Returns true either way. Both
// calls are left out because StringManager.hpp and the SetMember overload are not in the tree.
bool MenuBase::FS_GetString2(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    return true;  // PARTIAL: getStringFromSymbol and SetMember not called (see above).
}

// 0x420b48 _ZN8MenuBase13FS_EndLoadingEPKcS1_Pv
// The current level comes from Application::GetCurrentLevel() on the singleton. When the level exists and its m_loadStep
// (+0x130, Level::m_loadStep) is 36 (0x24), the step is advanced to 37. The function always returns false.
bool MenuBase::FS_EndLoading(const char*, const char*, void*) {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level && level->m_loadStep == 36) {
        ++level->m_loadStep;
    }
    return false;
}

// 0x420b8c _ZN8MenuBase15FS_LoadWorldMapEPKcS1_Pv
// Application::LoadWorldMap(-1) on the singleton. The original passes (GSLevelMap*)0xFFFFFFFF; dh2/Application.hpp
// declares the parameter int, and -1 is the same value. Returns true.
bool MenuBase::FS_LoadWorldMap(const char*, const char*, void*) {
    ApplicationGetInstance().LoadWorldMap(-1);
    return true;
}

// 0x420bb8 _ZN8MenuBase12FS_LoadLevelEPKcS1_Pv
// A null args returns false here; the original calls strcmp on it. "undefined" returns false, as the original does.
// Otherwise the original calls MenuFX::PopAll() on m_menu, then Application::LoadLevel(args, 0, 0, false, true, 0, false,
// 0, 0) on the singleton, and returns true. The original passes a null pointer in the second (entry point) argument;
// dh2/Application.hpp declares that argument int, and the value 0 is the same.
bool MenuBase::FS_LoadLevel(const char*, const char* args, void* menu) {
    if (!args) {
        return false;
    }
    if (std::strcmp(args, "undefined") == 0) {
        return false;
    }
    static_cast<MenuBase*>(menu)->m_menu->PopAll();
    ApplicationGetInstance().LoadLevel(args, 0, 0, false, true, 0, false, 0, 0);
    return true;
}

// 0x420c3c _ZN8MenuBase15FS_GoToMainMenuEPKcS1_Pv
// Application::GoToMainMenu(0) on the singleton. The event value 0 is passed as is; the GoToMainMenuEvent values are
// not recovered (dh2/Application.hpp). Returns true.
bool MenuBase::FS_GoToMainMenu(const char*, const char*, void*) {
    ApplicationGetInstance().GoToMainMenu(static_cast<GoToMainMenuEvent>(0));
    return true;
}

// 0x420c68 _ZN8MenuBase13FS_SkipScriptEPKcS1_Pv
// ScriptManager::SkipScript(-2, false) on the static instance ScriptManager::s_inst. Returns true.
bool MenuBase::FS_SkipScript(const char*, const char*, void*) {
    ScriptManager::s_inst.SkipScript(-2, false);
    return true;
}

// 0x420c98 _ZN8MenuBase16FS_ResetSaveFileEPKcS1_Pv
// The slot is atoi(args) when args is non-empty, otherwise the SavegameManager's signed m_lastSlot (+0x08). When the slot
// is not negative, the original calls PlayerSavegame::SG_Delete(slot) and then LevelSavegame::DeleteAllLevelSave(slot).
// The assembly (0x420c98) sets only r0 for both calls, so the slot is the only argument. Returns true in all cases.
bool MenuBase::FS_ResetSaveFile(const char*, const char* args, void*) {
    std::int32_t slot;
    if (args != nullptr && *args != '\0') {
        slot = std::atoi(args);
    } else {
        slot = ApplicationGetInstance().m_savegameManager->m_lastSlot;
    }
    if (slot >= 0) {
        PlayerSavegame::SG_Delete(static_cast<std::uint32_t>(slot));
        LevelSavegame::DeleteAllLevelSave(static_cast<std::uint32_t>(slot));
    }
    return true;
}
