// MenuBase bodies, batch g1-game-ui-MenuBase-8948cc-2 (part 3 of 5 of MenuBase): the Flash FSCommand handlers from
// FS_IsSaveSlotValid through FS_GetPlayerClass2, and MenuBase::IsTopMost. Rebuilt from the Hex-Rays pseudocode of
// libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM offsets. Partial functions say in their comment
// which part is missing.
//
// Facts shared by the handlers:
//   - A handler is (command, args, menu). The menu is the MenuBase, so static_cast<MenuBase*>(menu)->m_menu (State +0x04,
//     the owning MenuFX) is the RenderFX that the original uses for _root.retval.
//   - The Application singleton (Singleton<Application>::s_inst, 0x99f72c) is reached through ApplicationGetInstance()
//     (dh2/global/ctrl_camera.hpp; the integrator defines it). Its fields used here are the Hex-Rays globals:
//     dword_99F744 = m_stateMachine (+0x18), dword_99F76C = m_playerManager (+0x40), dword_99F778 = m_savegameManager
//     (+0x4c). dword_9A247C is GSFlashMenu::s_inst.m_pendingMenu (+0x0c).
//   - LC_API_ATOI_0 is atoi and LC_API_STRCMP_0 is strcmp, as in batches 0 and 1.
//   - RenderFX::SetMember (the _root.retval publish) and RenderFX::SetFocus are not declared in this tree. A handler that
//     publishes a value computes it, keeps it in a local, and stops at the publish. Those are PARTIAL.
//   - SavegameManager::m_lastSlot (+0x08) is private with no accessor, so no handler here can read the current save slot.
//   - Header conflict: dh2/PlayerManager.hpp and dh2/Character.hpp (through CharProperties.hpp) each bring a different
//     IStreamBase, and pydata-accessors/member_names.hpp conflicts with both (Structs::AIProps, IStreamBase). The
//     handlers that reach a local Character therefore cannot be compiled in this file with the sound-name table. They
//     are PARTIAL. The integrator resolves the pydata and stream conflicts, then they can be completed here.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "dh2/Application.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/global/menu_base_fs.hpp"
#include "pydata-accessors/member_names.hpp"

namespace {

// Index of `name` in the Sounds name table, or -1. The original compares with strcmp in table order and returns the
// first match. Its count is Arrays::Sounds::size (w7_arrays_00.hpp). That header declares a second struct named
// Arrays::Sounds, so it cannot be included next to member_names.hpp. The NameTable size is used instead, as batch 0
// does for CharacterTable.
int FindSoundIndex(const char* name) {
    const pydata::NameTable& table = Arrays::Sounds::m_memberNames;
    for (int i = 0; i < table.size; ++i) {
        if (std::strcmp(name, table.names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

}  // namespace

// 0x420cfc _ZN8MenuBase18FS_IsSaveSlotValidEPKcS1_Pv
// PARTIAL: the original reads the slot from args (atoi), tests PlayerSavegame::SG_Exists on it, and stores the boolean
// as _root.retval of the menu's RenderFX through RenderFX::SetMember(as_value), which is not declared. The boolean is
// computed here and not published. A null args or slot -1 gives false, as in the original. Returns true.
bool MenuBase::FS_IsSaveSlotValid(const char*, const char* args, void*) {
    bool valid = false;
    if (args != nullptr) {
        const int slot = std::atoi(args);
        if (slot != -1) {
            valid = PlayerSavegame::SG_Exists(static_cast<std::uint32_t>(slot)) != 0;
        }
    }
    static_cast<void>(valid);  // PARTIAL: _root.retval not set (see above).
    return true;
}

// 0x420d7c _ZN8MenuBase14FS_GetSaveSlotEPKcS1_Pv
// PARTIAL: the original reads the current save slot (SavegameManager +0x08) and sets _root.retval to it when that slot
// exists. Otherwise it sets _root.retval to the first existing slot among 1 to 4, or to "" when none exists. The
// current slot is private (SavegameManager::m_lastSlot) and RenderFX::SetMember is not declared, so nothing is read or
// set. Returns true as in the original.
bool MenuBase::FS_GetSaveSlot(const char*, const char*, void*) {
    return true;  // PARTIAL: see above.
}

// 0x420e64 _ZN8MenuBase14FS_ContinueGameEPKcS1_Pv
// PARTIAL: the original closes the menu stack (MenuFX::PopAll on the menu's MenuFX) and then reads the current save
// slot. With slot -1 it runs the "StartGame" command on the menu (virtual MyFSCommand, vtable +0x38). Otherwise it
// constructs the PlayerSavegame of that slot, calls Application::LoadLevel with values read from that save, and
// destroys the save. The slot is private (SavegameManager::m_lastSlot), and the mapping of the LoadLevel arguments
// from PlayerSavegame is not confirmed, so only the PopAll is made. Returns true as in the original.
bool MenuBase::FS_ContinueGame(const char*, const char*, void* menu) {
    static_cast<MenuBase*>(menu)->m_menu->PopAll();
    return true;  // PARTIAL: the slot branch is not made (see above).
}

// 0x420fac _ZN8MenuBase15FS_ReturnToGameEPKcS1_Pv
// PARTIAL: when a save slot is set (SavegameManager::m_lastSlot, private), the original constructs that slot's
// PlayerSavegame, pops the menu stack (MenuFX::PopAll), calls Application::LoadLevel, and destroys the save. None of
// that is made here, since the slot cannot be read. Returns true as in the original.
bool MenuBase::FS_ReturnToGame(const char*, const char*, void*) {
    return true;  // PARTIAL: see above.
}

// 0x4210a8 _ZN8MenuBase13FS_PlayMusicEPKcS1_Pv
// PARTIAL: args names a cue of the Sounds table. The original finds its index by strcmp over the table names and then
// calls VoxSoundManager::PlayMusic(s_instance, index, 1, 0, 2000). VoxSoundManager is not declared in this tree, so
// that call is left out. Returns true when the name is found and false otherwise, including null or empty args.
bool MenuBase::FS_PlayMusic(const char*, const char* args, void*) {
    if (args == nullptr || *args == '\0') {
        return false;
    }
    const int index = FindSoundIndex(args);
    if (index == -1) {
        return false;
    }
    static_cast<void>(index);  // PARTIAL: VoxSoundManager::PlayMusic not called (see above).
    return true;
}

// 0x421164 _ZN8MenuBase13FS_PlaySoundFXEPKcS1_Pv
// PARTIAL: the same name search as FS_PlayMusic. The original then calls VoxSoundManager::PlayMenu(s_instance, index,
// 0, 0, 0), which is not declared in this tree, so that call is left out. Returns true when the name is found.
bool MenuBase::FS_PlaySoundFX(const char*, const char* args, void*) {
    if (args == nullptr || *args == '\0') {
        return false;
    }
    const int index = FindSoundIndex(args);
    if (index == -1) {
        return false;
    }
    static_cast<void>(index);  // PARTIAL: VoxSoundManager::PlayMenu not called (see above).
    return true;
}

// 0x42121c _ZN8MenuBase12FS_SetFocusEPKcS1_Pv
// PARTIAL: the original calls RenderFX::SetFocus(menu->m_menu, args, 0). RenderFX (placeholder header) declares no
// SetFocus, so the call is left out. Returns true as in the original.
bool MenuBase::FS_SetFocus(const char*, const char*, void*) {
    return true;  // PARTIAL: RenderFX::SetFocus not called (see above).
}

// 0x421234 _ZN8MenuBase12FS_PushStateEPKcS1_Pv
// The original opens the menu named args as a state. For "menu_CharacterMenu" and "menu_Ingame" it first needs
// GSLevel::s_gsLevel on top of the state machine's stack and returns false otherwise. Then it stores
// MenuManager::GetMenuByName(args) in GSFlashMenu::s_inst.m_pendingMenu (dword_9A247C), pushes &GSFlashMenu::s_inst
// on Application::m_stateMachine (not owned), and returns true.
// PARTIAL: the stack top needs StateMachine::m_stack, which is private with no accessor, so for those two names the
// check is not made and the function returns false. Other names are rebuilt in full. A null args returns false here;
// the original calls strcmp on it.
bool MenuBase::FS_PushState(const char*, const char* args, void*) {
    if (args == nullptr) {
        return false;
    }
    const bool guarded = std::strcmp("menu_CharacterMenu", args) == 0 || std::strcmp("menu_Ingame", args) == 0;
    if (guarded) {
        return false;  // PARTIAL: the stack top check is not possible (see above).
    }
    GSFlashMenu::s_inst.m_pendingMenu = MenuManager::GetInstance()->GetMenuByName(args);
    ApplicationGetInstance().m_stateMachine->PushState(&GSFlashMenu::s_inst, false);
    return true;
}

// 0x4212f8 _ZN8MenuBase13FS_SwitchMenuEPKcS1_Pv
// PARTIAL: the original first calls the virtual at vtable +0x38 of MenuManager::m_multiMenuManager (+0xf4) with
// argument 0, then pushes args with MenuManager::PushMenu. The MultiMenuManager class is not declared (its header is
// absent), so only the PushMenu runs. Returns true as in the original.
bool MenuBase::FS_SwitchMenu(const char*, const char* args, void*) {
    MenuManager::GetInstance()->PushMenu(args);
    return true;  // PARTIAL: the MultiMenuManager call is missing (see above).
}

// 0x421334 _ZN8MenuBase13FS_PushMenuEPKcS1_Pv
bool MenuBase::FS_PushMenu(const char*, const char* args, void*) {
    MenuManager::GetInstance()->PushMenu(args);
    return true;
}

// 0x421350 _ZN8MenuBase15FS_PopAllAboveEPKcS1_Pv
// PARTIAL: the original looks up the state named args (default "menu_CharacterMenu") and, while that state is not the
// top and the stack is not empty, pops the top with MultiMenuManager vtable +0x38 (no argument). It returns whether the
// target is the top. The pop is not declared (MultiMenuManager header absent), so no pop is made here. The result
// matches the original when no pop is needed, that is when the target is missing or already the top.
bool MenuBase::FS_PopAllAbove(const char*, const char* args, void* menu) {
    MenuFX* fx = static_cast<MenuBase*>(menu)->m_menu;
    const char* name = (args != nullptr) ? args : "menu_CharacterMenu";
    MenuFX::State* target = fx->GetState(name);
    return target == fx->GetCurrentState();  // PARTIAL: the pop loop is not run (see above).
}

// 0x4213f0 _ZN8MenuBase9IsTopMostEv
// True when this menu is the top state of its MenuFX (the original compares this with GetCurrentState of +0x04).
bool MenuBase::IsTopMost() {
    return this == m_menu->GetCurrentState();
}

// 0x421410 _ZN8MenuBase11FS_PopMenuEPKcS1_Pv
// An empty args pops the menu itself: the original passes a3 + 8, which is the menu's name (State +0x08, m_name).
// The original reads args without a null test, so a null args is treated here as empty.
bool MenuBase::FS_PopMenu(const char*, const char* args, void* menu) {
    if (args == nullptr || *args == '\0') {
        args = static_cast<MenuBase*>(menu)->m_name;
    }
    MenuManager::GetInstance()->PopMenu(args);
    return true;
}

// 0x42143c _ZN8MenuBase16FS_AutoEquipSlotEPKcS1_Pv
// PARTIAL: the original reads the local player's Character (PlayerManager::GetLocalPlayer(0, true)->character_660) and
// then unequips and re-equips equipment slots with the Character's virtuals: UnEquipItemFromSlot (+0x144) and
// EquipSlotAuto (+0x140). "ALL" runs over every equipment slot, from the last down to 0 for the re-equip. Any other
// args is parsed with atoi and handled as one slot. Neither the PlayerManager nor the Character header can be included
// in this file (see the header comment), so nothing is equipped. A null or empty args returns false, as in the
// original, and so does every other call here.
bool MenuBase::FS_AutoEquipSlot(const char*, const char* args, void*) {
    if (args == nullptr || *args == '\0') {
        return false;
    }
    return false;  // PARTIAL: the character path (see above).
}

// 0x421554 _ZN8MenuBase21FS_GetHasOffHandWeaponEPKcS1_Pv
// PARTIAL: the original returns true when the local player has a Character, and sets _root.retval to that Character's
// ItemInventory::HasOffHandWeapon through RenderFX::SetMember. The Character cannot be reached here (see the header
// comment), so this returns false, which is the original's result when there is no Character.
bool MenuBase::FS_GetHasOffHandWeapon(const char*, const char*, void*) {
    return false;  // PARTIAL: see above.
}

// 0x4215ec _ZN8MenuBase21FS_GetHasMainHandWeaponEPKcS1_Pv
// PARTIAL: as FS_GetHasOffHandWeapon, with ItemInventory::HasMainHandWeapon. Returns false (see above).
bool MenuBase::FS_GetHasMainHandWeapon(const char*, const char*, void*) {
    return false;  // PARTIAL: see above.
}

// 0x421684 _ZN8MenuBase21FS_GetHasTwoHandWeaponEPKcS1_Pv
// PARTIAL: as FS_GetHasOffHandWeapon, with ItemInventory::HasTwoHander(false). Returns false (see above).
bool MenuBase::FS_GetHasTwoHandWeapon(const char*, const char*, void*) {
    return false;  // PARTIAL: see above.
}

// 0x421720 _ZN8MenuBase16FS_GetPlayerClassEPKcS1_Pv
// PARTIAL: the original reads the local player's Character and its class (Character::SG_GetPlayerClass). A null
// Character or class -1 sets _root.retval to "". Otherwise it sets _root.retval to
// StringManager::getString(playerClassAsStrID(class)) when that is not null. The original also parses args with atoi
// and drops the result. The Character cannot be reached here (see the header comment), so no value is set. Returns true
// as in the original, which returns 1 on every path.
bool MenuBase::FS_GetPlayerClass(const char*, const char*, void*) {
    return true;  // PARTIAL: see above.
}

// 0x421808 _ZN8MenuBase17FS_GetCharPropertyEPKcS1_Pv
// PARTIAL: args is a property name. The original looks it up with CharProperties::GetPropIDFromName and reads the
// local player's CharProperties (the base at +0x560) with PROPS_GetInt(id, false). It sets the decimal text of the
// value as _root.retval through RenderFX::SetMember. The Character cannot be reached here (see the header comment), so
// nothing is read. The original formats the text into an 8-byte buffer with sprintf, which overflows for values of
// eight or more characters; a rebuild should use a buffer of at least 12 bytes, which fits any int. Returns true as in
// the original, which returns 1 on every path.
bool MenuBase::FS_GetCharProperty(const char*, const char*, void*) {
    return true;  // PARTIAL: see above.
}

// 0x421980 _ZN8MenuBase17FS_GetPlayerClass2EPKcS1_Pv
// PARTIAL: the original sets _root.retval to playerClassAsStr() through RenderFX::SetMember, which is not declared, so
// the name is read and not published. Returns true as in the original.
bool MenuBase::FS_GetPlayerClass2(const char*, const char*, void*) {
    const char* className = playerClassAsStr();
    static_cast<void>(className);  // PARTIAL: _root.retval not set (see above).
    return true;
}
