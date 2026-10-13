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
//   - RenderFX::SetMember (dh2/RenderFX.hpp) publishes _root.retval. A boolean is published as a gameswf::as_value built
//     with set_bool, and a text as a const char*.
//   - The local Character is ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter(), which is
//     GetLocalPlayer(0, true)->character_660 (PlayerManager.hpp, Character.hpp, PlayerInfo.hpp).
//   - MenuManager::m_multiMenuManager (+0xf4) is a MultiMenuManager (dh2/MultiMenuManager.hpp). Its virtual PopMenu(bool)
//     is vtable slot +0x38.
//   - VoxSoundManager::PlayMenu is not declared in vox-audio/include/vox-audio/vox/sound_manager.hpp, so the sound
//     handlers stay PARTIAL.
//   - SavegameManager::m_lastSlot (+0x08) is a public member of dh2/SavegameManager.hpp, read directly where a handler needs the slot.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "dh2/Application.hpp"
#include "dh2/CharProperties.hpp"
#include "dh2/Character.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/GSLevel.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/global/menu_base_fs.hpp"
#include "pydata-accessors/member_names.hpp"
#include "vendor-gameswf/as_value.hpp"

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
// The original closes the menu stack (MenuFX::PopAll on the menu's MenuFX), then reads the current save slot
// (SavegameManager::m_lastSlot). With slot -1 it runs the "StartGame" command on the menu (virtual MyFSCommand, vtable
// +0x38) with the handler's args. Otherwise it constructs the PlayerSavegame of that slot, calls Application::LoadLevel
// with values read from that save, and destroys the save.
// PARTIAL: the save-load branch is not made. Its LoadLevel name is the Arrays::LevelList element's +0x20 word
// (Structs::LevelDeclaration::m_w8, a string buffer), and the raw LevelList table cannot be declared in this file next
// to pydata-accessors member_names.hpp; the PlayerSavegame argument mapping is not confirmed either. Returns true.
bool MenuBase::FS_ContinueGame(const char*, const char* args, void* menu) {
    MenuBase* self = static_cast<MenuBase*>(menu);
    self->m_menu->PopAll();
    const std::int32_t slot = ApplicationGetInstance().m_savegameManager->m_lastSlot;
    if (slot == -1) {
        self->MyFSCommand("StartGame", args);
        return true;
    }
    return true;  // PARTIAL: the save-load branch is not made (see above).
}

// 0x420fac _ZN8MenuBase15FS_ReturnToGameEPKcS1_Pv
// The original reads the current save slot (SavegameManager::m_lastSlot). When it is set (not -1), the original constructs
// that slot's PlayerSavegame, pops the menu stack (MenuFX::PopAll), calls Application::LoadLevel, and destroys the save.
// PARTIAL: that branch is not made, for the LevelList name reason given for FS_ContinueGame. For slot -1 the original
// does nothing else and returns true, which is what this returns.
bool MenuBase::FS_ReturnToGame(const char*, const char*, void*) {
    const std::int32_t slot = ApplicationGetInstance().m_savegameManager->m_lastSlot;
    if (slot != -1) {
        // PARTIAL: the save-load branch (PlayerSavegame, MenuFX::PopAll, Application::LoadLevel) is not made (see above).
    }
    return true;
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
// The stack top is the last entry of StateMachine::m_stack (the original reads the end pointer minus 8, and the state
// pointer of that entry). The stack must not be empty and its top must be GSLevel::s_gsLevel. A null args returns false
// here; the original calls strcmp on it, which is undefined for a null pointer.
bool MenuBase::FS_PushState(const char*, const char* args, void*) {
    if (args == nullptr) {
        return false;
    }
    const bool guarded = std::strcmp("menu_CharacterMenu", args) == 0 || std::strcmp("menu_Ingame", args) == 0;
    StateMachine* machine = ApplicationGetInstance().m_stateMachine;
    if (guarded && (machine->m_stack.empty() || machine->m_stack.back().state != &GSLevel::s_gsLevel)) {
        return false;
    }
    GSFlashMenu::s_inst.m_pendingMenu = MenuManager::GetInstance()->GetMenuByName(args);
    machine->PushState(&GSFlashMenu::s_inst, false);
    return true;
}

// 0x4212f8 _ZN8MenuBase13FS_SwitchMenuEPKcS1_Pv
// The original first calls the virtual at vtable +0x38 of MenuManager::m_multiMenuManager (+0xf4) with argument 0, which
// is MultiMenuManager::PopMenu(false), then pushes args with MenuManager::PushMenu. Returns true.
bool MenuBase::FS_SwitchMenu(const char*, const char* args, void*) {
    MenuManager* instance = MenuManager::GetInstance();
    instance->m_multiMenuManager->PopMenu(false);
    instance->PushMenu(args);
    return true;
}

// 0x421334 _ZN8MenuBase13FS_PushMenuEPKcS1_Pv
bool MenuBase::FS_PushMenu(const char*, const char* args, void*) {
    MenuManager::GetInstance()->PushMenu(args);
    return true;
}

// 0x421350 _ZN8MenuBase15FS_PopAllAboveEPKcS1_Pv
// The original looks up the state named args (default "menu_CharacterMenu") in the menu's MenuFX. While that state is
// not the current one and the MenuFX stack (m_stack, count at +0x118) is not empty, it pops one page with
// MultiMenuManager::PopMenu(false): the virtual at vtable +0x38, whose argument r1 is set to 0 before the loop. It
// returns whether the target is the current state.
bool MenuBase::FS_PopAllAbove(const char*, const char* args, void* menu) {
    MenuManager* instance = MenuManager::GetInstance();
    MenuFX* fx = static_cast<MenuBase*>(menu)->m_menu;
    const char* name = (args != nullptr) ? args : "menu_CharacterMenu";
    MenuFX::State* target = fx->GetState(name);
    if (target != nullptr && target != fx->GetCurrentState()) {
        while (fx->m_stack.m_size > 0) {
            instance->m_multiMenuManager->PopMenu(false);
            if (target == fx->GetCurrentState()) {
                break;
            }
        }
    }
    return target == fx->GetCurrentState();
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
// The local player's Character is read (character_660). Without one the original returns 0 and publishes nothing. With
// one it publishes ItemInventory::HasMainHandWeapon (the inventory base at Character +0x37c) as the boolean _root.retval
// of the menu, and returns 1.
bool MenuBase::FS_GetHasMainHandWeapon(const char*, const char*, void* menu) {
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    if (character == nullptr) {
        return false;
    }
    gameswf::as_value retval;
    retval.set_bool(character->ItemInventory::HasMainHandWeapon());
    static_cast<MenuBase*>(menu)->m_menu->SetMember("_root", "retval", retval);
    return true;
}

// 0x421684 _ZN8MenuBase21FS_GetHasTwoHandWeaponEPKcS1_Pv
// As FS_GetHasMainHandWeapon, with ItemInventory::HasTwoHander(false) published as the boolean _root.retval.
bool MenuBase::FS_GetHasTwoHandWeapon(const char*, const char*, void* menu) {
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    if (character == nullptr) {
        return false;
    }
    gameswf::as_value retval;
    retval.set_bool(character->ItemInventory::HasTwoHander(false));
    static_cast<MenuBase*>(menu)->m_menu->SetMember("_root", "retval", retval);
    return true;
}

// 0x421720 _ZN8MenuBase16FS_GetPlayerClassEPKcS1_Pv
// The original reads the local player's Character and its class (Character::SG_GetPlayerClass). A null Character or a
// class of -1 publishes "" as _root.retval. Otherwise it publishes StringManager::getString(playerClassAsStrID(class))
// when that is not null. The original's atoi of args has no effect on the result and is kept as a call. Returns true,
// as the original does on every path.
bool MenuBase::FS_GetPlayerClass(const char*, const char* args, void* menu) {
    RenderFX* fx = static_cast<MenuBase*>(menu)->m_menu;
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    if (character == nullptr) {
        fx->SetMember("_root", "retval", "");
        return true;
    }
    if (args != nullptr) {
        static_cast<void>(std::atoi(args));
    }
    const int playerClass = character->SG_GetPlayerClass();
    if (playerClass == -1) {
        fx->SetMember("_root", "retval", "");
        return true;
    }
    const char* text = ApplicationGetInstance().m_stringManager->getString(playerClassAsStrID(playerClass));
    if (text != nullptr) {
        fx->SetMember("_root", "retval", text);
    }
    return true;
}

// 0x421808 _ZN8MenuBase17FS_GetCharPropertyEPKcS1_Pv
// args is a property name. When the local player has a Character, the original looks the name up with
// CharProperties::GetPropIDFromName (a static, called with args). When the id is not -1 it publishes the decimal text of
// PROPS_GetInt(id, false) on the CharProperties base of the Character (+0x560) as _root.retval. The text buffer is 12
// bytes; the original's 8-byte sprintf buffer overflows for eight or more characters. Returns true, as the original does.
bool MenuBase::FS_GetCharProperty(const char*, const char* args, void* menu) {
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    if (character != nullptr) {
        const int propId = CharProperties::GetPropIDFromName(args);
        if (propId != -1) {
            const int value = character->PROPS_GetInt(propId, false);
            char text[12];
            std::snprintf(text, sizeof(text), "%d", value);
            static_cast<MenuBase*>(menu)->m_menu->SetMember("_root", "retval", text);
        }
    }
    return true;
}

// 0x421980 _ZN8MenuBase17FS_GetPlayerClass2EPKcS1_Pv
// PARTIAL: the original sets _root.retval to playerClassAsStr() through RenderFX::SetMember, which is not declared, so
// the name is read and not published. Returns true as in the original.
bool MenuBase::FS_GetPlayerClass2(const char*, const char*, void*) {
    const char* className = playerClassAsStr();
    static_cast<void>(className);  // PARTIAL: _root.retval not set (see above).
    return true;
}
