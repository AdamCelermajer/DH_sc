// MenuBase bodies, batch g1-game-ui-MenuBase-8948cc-0 (part 1 of 5 of MenuBase).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original
// 32-bit ARM offsets. Partial functions say in their comment which part is missing.
// LC_API_STRCMP_0 is read as strcmp (0 when the strings are equal): the FS_SetPlayerClass search loop
// continues while it is nonzero, which only fits strcmp.

#include <cmath>
#include <cstdint>
#include <cstring>

#include "gameswf/gameswf.h"
#include "dh2/Character.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"
#include "dh2/v2Controller.hpp"
#include "dh2/v2Controllable.hpp"
#include "pydata-accessors/arrays.hpp"
#include "pydata-accessors/member_names.hpp"

// 0x41b3d0 _ZN8MenuBase11OnFSCommandEPKcS1_
// Virtual dispatch to MyFSCommand (vtable +0x38). The original tail-calls it and returns its bool;
// this void override drops that result.
void MenuBase::OnFSCommand(const char* command, const char* args) {
    MyFSCommand(command, args);
}

// 0x41b3e4 _ZN8MenuBase6CreateEv
void MenuBase::Create() {}

// 0x41b3e8 _ZN8MenuBase8GotFocusEv
void MenuBase::GotFocus() {}

// 0x41b3ec _ZN8MenuBase9LostFocusEv
void MenuBase::LostFocus() {}

// 0x41b3f0 _ZNK8MenuBase11IsValidMenuEv
bool MenuBase::IsValidMenu() const {
    return m_registered;  // +0x7c
}

// 0x41b3f8 _ZN8MenuBase14SetIsLocalisedEb
void MenuBase::SetIsLocalised(bool localised) {
    m_isLocalised = localised;  // +0x75
}

// 0x41b400 _ZN8MenuBase12CacheStringsEv
void MenuBase::CacheStrings() {}

// 0x41f3f4 _ZNK8MenuBase9IsVisibleEv
bool MenuBase::IsVisible() const {
    return m_visible;  // +0x74
}

// 0x41f3fc _ZN8MenuBase14CanHandleEventERN8RenderFX5EventE
bool MenuBase::CanHandleEvent(RenderFX::Event&) {
    return true;
}

// 0x41f404 _ZN8MenuBase16FS_LaunchTwitterEPKcS1_Pv
// The original does nothing and returns true.
bool MenuBase::FS_LaunchTwitter(const char*, const char*, void*) {
    return true;
}

// 0x41f40c _ZN8MenuBase11FS_ExitGameEPKcS1_Pv
// PARTIAL: the original calls vtable slot +0x30 of the object at Singleton<Application>::s_inst + 0x10
// (the glitch::IDevice* m_device) with the args string, then returns true. The slot is not declared in
// glitch-core-io/os/device.hpp and the Application singleton is not declared in this tree, so the call is
// omitted. Waits on the device vtable slot and on a declaration of Singleton<Application>::s_inst.
bool MenuBase::FS_ExitGame(const char*, const char*, void*) {
    return true;
}

// 0x41f444 _ZN8MenuBase16FS_PauseGameplayEPKcS1_Pv
// The original returns false and does nothing else.
bool MenuBase::FS_PauseGameplay(const char*, const char*, void*) {
    return false;
}

// 0x41f44c _ZN8MenuBase17FS_ResumeGameplayEPKcS1_Pv
// The original returns false and does nothing else.
bool MenuBase::FS_ResumeGameplay(const char*, const char*, void*) {
    return false;
}

// 0x41f454 _ZN8MenuBase14RemoveDeadZoneERKN7gameswf4rectE
// The original scans m_deadZones (+0xb8 begin, +0xbc end, 16-byte gameswf::rect) for the first rect whose m_x_max
// (float at +4) is within 0.0001 of zone.m_x_max, shifts the following rects down one slot and pops the end. That
// is vector::erase on the first match. gameswf::rect is defined by the vendored gameswf.h (line 652), which compiles
// in this TU with the engine include paths.
void MenuBase::RemoveDeadZone(const gameswf::rect& zone) {
    for (auto it = m_deadZones.begin(); it != m_deadZones.end(); ++it) {
        if (std::fabs(it->m_x_max - zone.m_x_max) < 0.0001f) {
            m_deadZones.erase(it);
            return;
        }
    }
}

// 0x41f4f8 _ZN8MenuBase16ClearSlideEventsEv
// Frees each non-null slide-event object with CustomFree (not delete, as in the original), nulls the slot, then
// empties the vector. The original sets the end pointer to the begin pointer, which is clear().
void MenuBase::ClearSlideEvents() {
    for (SlideEventCharacter*& slot : m_slideEvents) {  // +0x60 begin .. +0x64 end
        if (slot) {
            CustomFree(slot);
            slot = nullptr;
        }
    }
    m_slideEvents.clear();
}

// 0x41f578 _ZN8MenuBase13FS_LoadLevel2EPKcS1_Pv
// PARTIAL: the name search over Arrays::LevelList is rebuilt; the LoadLevel call is not. The original calls
// menu->MyFSCommand("LoadLevel", element) with element = the LevelList entry's word at +0x20 (listing stride 72)
// and returns true. The +0x20 word is Structs::LevelDeclaration::m_w8 (structs_records.hpp:963, a string buffer, stride
// 72 in the file). The raw table is Arrays::LevelList::members in w7_arrays_00.hpp, which conflicts with
// member_names.hpp in this file, so only the name table is usable here. The count is taken from NameTable::size; the
// original reads the separate Arrays::LevelList::size global.
bool MenuBase::FS_LoadLevel2(const char*, const char* args, void* menu) {
    if (!args || !*args) {
        return false;
    }
    const std::uint32_t count = Arrays::LevelList::size;
    if (count == 0) {
        return false;
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        if (std::strcmp(args, Arrays::LevelList::m_memberNames.names[i]) == 0) {
            // Each LevelList row is 72 bytes; the word at +0x20 is the level string the original passes on.
            const std::uint8_t* row = Arrays::LevelList::members + 72 * i;
            const char* level = *reinterpret_cast<const char* const*>(row + 32);
            static_cast<MenuBase*>(menu)->MyFSCommand("LoadLevel", level);
            return true;
        }
    }
    return false;
}

// 0x41f640 _ZN8MenuBase17FS_SetPlayerClassEPKcS1_Pv
// The count is Arrays::CharacterTable::size, and the search runs over its member-name table. The result is stored in
// the menu at +0x98 (unk_98 in MenuBase.hpp): the CharacterTable row index, or -1 when there is no match. The
// function always returns true.
bool MenuBase::FS_SetPlayerClass(const char*, const char* args, void* menu) {
    int index = -1;
    const std::uint32_t count = Arrays::CharacterTable::size;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (std::strcmp(args, Arrays::CharacterTable::m_memberNames.names[i]) == 0) {
            index = i;
            break;
        }
    }
    static_cast<MenuBase*>(menu)->unk_98 = static_cast<uint32_t>(index);  // +0x98
    return true;
}

// 0x41ff08 _ZNK8MenuBase19DBG_Draw2DDeadZonesEv
// PARTIAL: not rebuilt. The original draws each m_deadZones rect (+0xb8..+0xbc) as a 2D quad: the rect is
// converted to ints in the order (x_min, y_min, x_max, y_max), the four vertex colours are the bytes ff 00 00 64
// repeated, and the call goes through the driver at device +0x10, vtable slot +0x34, with a zero clip rect and a
// null clip pointer. The material is set up by glitch::video::C2DDriver::set2DTexture and released afterwards.
// Blocked by: the gameswf::rect definition (see RemoveDeadZone), the device +0x10 type (glitch::IDevice has no
// such member declared), the vtable slot, and the set2DTexture signature.
void MenuBase::DBG_Draw2DDeadZones() const {}

// Walks the local players for FS_LockCharacter and FS_UnlockCharacter. The original loops i < GetNumLocalPlayers(false)
// on the PlayerManager global (dword_99F76C), takes GetLocalPlayer(i, false)->unk_660 (the Character*), and for a
// non-null Character sets the byte at controller +0x08 (v2Controller::m_blocked) to the flag.
static void SetLocalCharactersBlocked(bool blocked) {
    const int count = dword_99F76C->GetNumLocalPlayers(false);
    for (int i = 0; i < count; ++i) {
        Character* character = dword_99F76C->GetLocalPlayer(i, false)->unk_660;
        if (character) {
            character->m_controller->m_blocked = blocked;
        }
    }
}

// 0x4202a8 _ZN8MenuBase18FS_UnlockCharacterEPKcS1_Pv
// Any name other than "All" clears the block on each local player's character. "All" clears the global
// v2Controller::s_blocked.
bool MenuBase::FS_UnlockCharacter(const char*, const char* args, void*) {
    if (std::strcmp(args, "All") != 0) {
        SetLocalCharactersBlocked(false);
        return true;
    }
    v2Controller::s_blocked = false;
    return true;
}

// 0x420344 _ZN8MenuBase16FS_LockCharacterEPKcS1_Pv
// Mirror of FS_UnlockCharacter: any name other than "All" sets the block on each local player's character, "All"
// sets v2Controller::s_blocked.
bool MenuBase::FS_LockCharacter(const char*, const char* args, void*) {
    if (std::strcmp(args, "All") != 0) {
        SetLocalCharactersBlocked(true);
        return true;
    }
    v2Controller::s_blocked = true;
    return true;
}
