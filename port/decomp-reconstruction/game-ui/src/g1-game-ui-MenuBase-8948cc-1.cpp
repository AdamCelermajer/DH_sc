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
//   - Not declared in this tree yet: Singleton<Application>::s_inst, ScriptManager::s_inst, SavegameManager,
//     StringManager, PyDataConstants, MultiMenuManager (PopMenu is virtual slot +0x38), DragAndDrop, Dragable,
//     PlayerSavegame, LevelSavegame, GameSWFUtils, gameswf::as_value, and the RenderFX methods Find, PlayAnim,
//     SetMember and InvokeASCallback. Each call on those is left out and marked PARTIAL, and the function returns
//     what the original returns on that path. Nothing is guessed.

#include <cstring>

#include "dh2/Level.hpp"
#include "dh2/MenuBase.hpp"

// 0x4203e4 _ZN8MenuBase13FS_PopAllMenuEPKcS1_Pv
// PARTIAL: the original calls the virtual at vtable +0x38 of MenuManager::GetInstance()->m_multiMenuManager
// (+0xf4 of MenuManager) with argument 1, which is MultiMenuManager::PopMenu(true). MultiMenuManager.hpp is not
// in the tree, so the call is left out. Returns true as in the original.
bool MenuBase::FS_PopAllMenu(const char*, const char*, void*) {
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
// setLanguage(getLanguage()) on it. The original returns setLanguage's result, and that return type is not declared
// yet. SavegameManager.hpp is not in the tree and the singleton is not declared, so the calls are left out and the
// function returns false.
bool MenuBase::FS_LoadOptions(const char*, const char*, void*) {
    return false;  // PARTIAL: the return value is setLanguage's, see above.
}

// 0x4204fc _ZN8MenuBase14FS_SaveOptionsEPKcS1_Pv
// PARTIAL: saveSettings() and then setLanguage(getLanguage()) on the SavegameManager. Left out for the same reason as
// FS_LoadOptions. Returns true as in the original.
bool MenuBase::FS_SaveOptions(const char*, const char*, void*) {
    return true;  // PARTIAL: saveSettings and setLanguage not called (see above).
}

// 0x42053c _ZN8MenuBase14FS_SetSaveSlotEPKcS1_Pv
// PARTIAL: a null args returns false. Otherwise the original stores atoi(args) as an int at +0x08 of the
// SavegameManager. The SavegameManager layout is not declared yet, so the store is left out. Returns true.
bool MenuBase::FS_SetSaveSlot(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    return true;  // PARTIAL: the store at SavegameManager +0x08 is not made (see above).
}

// 0x42057c _ZN8MenuBase14FS_SetLanguageEPKcS1_Pv
// PARTIAL: a null or empty args returns false. Otherwise the original calls setLanguage(atoi(args)) on the
// SavegameManager. A zero result returns false. A nonzero result calls ResetFonts on MenuManager::GetInstance() and
// returns true. The setLanguage call is left out because SavegameManager.hpp is not in the tree, so the function
// returns false after the checks.
bool MenuBase::FS_SetLanguage(const char*, const char* args, void*) {
    if (!args || !*args) {
        return false;
    }
    return false;  // PARTIAL: setLanguage not called, so the ResetFonts branch is not known (see above).
}

// 0x4205e0 _ZN8MenuBase12FS_SetOptionEPKcS1_Pv
// PARTIAL: a null args returns false. Otherwise the original calls SavegameManager::setOption(args + 7, 0), where the
// 7 bytes skipped are a fixed prefix of the option name, and returns true. The setOption call is left out because
// SavegameManager.hpp is not in the tree.
bool MenuBase::FS_SetOption(const char*, const char* args, void*) {
    if (!args) {
        return false;
    }
    return true;  // PARTIAL: setOption(args + 7, 0) not called (see above).
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
// PARTIAL: the menu's DragAndDrop (+0x5c, MenuBase::m_dragAndDrop) is checked first, and a null one returns false, as
// in the original. Otherwise the original calls DragAndDrop::GetDragable(args), and if that is not null it calls
// Dragable::ResetPosition() and returns true. DragAndDrop.hpp and Dragable.hpp are not in the tree, so the lookup is
// left out and the function returns false when the DragAndDrop is present.
bool MenuBase::FS_ResetDraggablePosition(const char*, const char*, void* menu) {
    if (!static_cast<MenuBase*>(menu)->m_dragAndDrop) {
        return false;
    }
    return false;  // PARTIAL: GetDragable and ResetPosition not called (see above).
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
// PARTIAL: the original takes the current level from Singleton<Application>::s_inst.GetCurrentLevel(). The singleton
// is not declared in the tree, so the level is left as null here and nothing changes. The rest is rebuilt: when the
// level exists and its m_loadStep (+0x130) is 36 (0x24), the step is advanced to 37. The function always returns
// false.
bool MenuBase::FS_EndLoading(const char*, const char*, void*) {
    Level* level = nullptr;  // PARTIAL: Application::GetCurrentLevel() on the singleton (see above).
    if (level && level->m_loadStep == 36) {
        ++level->m_loadStep;
    }
    return false;
}

// 0x420b8c _ZN8MenuBase15FS_LoadWorldMapEPKcS1_Pv
// PARTIAL: the original calls Application::LoadWorldMap(-1) on Singleton<Application>::s_inst. The singleton is not
// declared in the tree, so the call is left out. Returns true as in the original.
bool MenuBase::FS_LoadWorldMap(const char*, const char*, void*) {
    return true;  // PARTIAL: LoadWorldMap(-1) not called (see above).
}

// 0x420bb8 _ZN8MenuBase12FS_LoadLevelEPKcS1_Pv
// PARTIAL: when args is not "undefined" the original calls MenuFX::PopAll() on m_menu, then
// Application::LoadLevel(args, 0, 0, false, true, 0, false, 0, 0) on Singleton<Application>::s_inst (the arguments
// were read from the assembly). The singleton is not declared in the tree, so that call is left out and the function
// returns true. For "undefined" it returns false, as the original does. A null args returns false here; the original
// calls strcmp on it.
bool MenuBase::FS_LoadLevel(const char*, const char* args, void* menu) {
    if (!args) {
        return false;
    }
    if (std::strcmp(args, "undefined") == 0) {
        return false;
    }
    static_cast<MenuBase*>(menu)->m_menu->PopAll();
    return true;  // PARTIAL: Application::LoadLevel not called (see above).
}

// 0x420c3c _ZN8MenuBase15FS_GoToMainMenuEPKcS1_Pv
// PARTIAL: the original calls Application::GoToMainMenu(0) on Singleton<Application>::s_inst. The singleton is not
// declared in the tree, so the call is left out. Returns true as in the original.
bool MenuBase::FS_GoToMainMenu(const char*, const char*, void*) {
    return true;  // PARTIAL: GoToMainMenu(0) not called (see above).
}

// 0x420c68 _ZN8MenuBase13FS_SkipScriptEPKcS1_Pv
// PARTIAL: the original calls ScriptManager::SkipScript(-2, false) on ScriptManager::s_inst. The static instance is
// not declared in ScriptManager.hpp, so the call is left out. Returns true as in the original.
bool MenuBase::FS_SkipScript(const char*, const char*, void*) {
    return true;  // PARTIAL: SkipScript(-2, false) not called (see above).
}

// 0x420c98 _ZN8MenuBase16FS_ResetSaveFileEPKcS1_Pv
// PARTIAL: the slot is atoi(args) when args is non-empty, otherwise the SavegameManager's signed int at +0x08. When the
// slot is not negative, the original calls PlayerSavegame::SG_Delete(slot) and LevelSavegame::DeleteAllLevelSave(slot).
// Both get the slot in r0 and no other argument in the assembly, so they are read as static. SavegameManager, PlayerSavegame.hpp and
// LevelSavegame.hpp are not in the tree, so these calls are left out. Returns true in all cases.
bool MenuBase::FS_ResetSaveFile(const char*, const char*, void*) {
    return true;  // PARTIAL: slot read and SG_Delete / DeleteAllLevelSave not called (see above).
}
