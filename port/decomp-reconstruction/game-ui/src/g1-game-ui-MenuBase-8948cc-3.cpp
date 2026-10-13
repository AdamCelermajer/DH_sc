// MenuBase bodies, batch g1-game-ui-MenuBase-8948cc-3 (part 4 of 5 of MenuBase): the FSCommand handlers FS_AssignPoint
// through FS_StopMusic, the destructor, SetVisible, the slide and dead-zone registration, ProcessLocalization, the hit
// test and the two event hooks PostOnEvent and OnEvent. Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so.
// Offsets in comments are the original 32-bit ARM offsets. Partial functions say in their comment which part is missing.
//
// Facts shared by the bodies:
//   - A handler is (command, args, menu). static_cast<MenuBase*>(menu)->m_menu (State +0x04) is the owning MenuFX.
//   - The Application singleton is ApplicationGetInstance() (dh2/global/ctrl_camera.hpp). The Hex-Rays globals are its
//     fields: dword_99F76C = m_playerManager (+0x40), dword_99F778 = m_savegameManager (+0x4c).
//   - PlayerManager::GetLocalPlayerCharacter() (0x330794) has the body GetLocalPlayer(this, 0, 1)->character_660, which
//     is the local Character every handler here reads. The handlers call it.
//   - The "isTracingMenuBase" switch is read at each trace point only for the DebugSwitches file load, since every
//     use drops the result. TraceMenuBaseSwitch below performs that load and lookup.
//   - The menu's context is a gameswf::weak_ptr<character> pair: a proxy at +0x48 (MenuFX::State::m_characterProxy) and
//     the cached character at +0x4c (m_character). The alive test and the release need the weak_proxy layout and
//     gameswf::free_internal, which the game tree does not declare (see the MenuFX State notes). Where the original tests
//     or releases the proxy, this file takes the context as alive and marks the function PARTIAL.
//   - RenderFX::SetMember, IsAnimOver, FindCharacters, SetContext, GetSearchIndex, SearchIndex::Find, FormatHTML and
//     SetFocus, the RenderFX::Event fields, VoxSoundManager and nativeOpenBrowser are not declared in this tree. Their
//     calls are left out and marked PARTIAL.
//   - LC_API_STRCMP_0 is strcmp (0 when equal), LC_API_ATOI_0 is atoi.
//   - The State base releases its two proxies (+0x50, then +0x48) after ~MenuBase. That is MenuFX::State::~State.
//   - Character.hpp and PlayerManager.hpp cannot be included in one translation unit. Character.hpp reaches pydata's
//     deps/stream_reader.hpp, which defines IStreamBase, and PlayerManager.hpp reaches net-transport's deps/i_stream_base.hpp,
//     which defines it again; StreamBuffer then fails to compile. This file includes PlayerManager.hpp for the local
//     character pointer only. Every Character member call is therefore PARTIAL (see the notes on each function).

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core-platform/memory.hpp"
#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/DragAndDrop.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "gameswf/gameswf.h"

namespace {

// The "isTracingMenuBase" switch. The original calls DebugSwitches::load on its singleton and then reads the switch at
// every trace point, dropping the result. GetInstance() returns the same singleton and loads the file on first use, so
// the first call here may load the file once more. The lookup has no other effect.
void TraceMenuBaseSwitch() {
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    static_cast<void>(switches->GetSwitch("isTracingMenuBase"));
}

}  // namespace

// 0x4219b8 _ZN8MenuBase14FS_AssignPointEPKcS1_Pv
// PARTIAL: the original adds one point to the local Character's stat named by args: "Str" (IncStatStr), "Dex"
// (IncStatDex), "End" (IncStatEnd) or "Eng" (IncStatNrg). Character.hpp cannot be included in this file: it conflicts
// with PlayerManager.hpp over IStreamBase (see the notes above), so the increments are not made. The return value
// follows the original: false without a local Character, and false for any name other than the four stats.
bool MenuBase::FS_AssignPoint(const char*, const char* args, void*) {
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    if (character == nullptr || args == nullptr) {
        return false;
    }
    const bool isStat = std::strcmp(args, "Str") == 0 || std::strcmp(args, "Dex") == 0 ||
                        std::strcmp(args, "End") == 0 || std::strcmp(args, "Eng") == 0;
    return isStat;  // PARTIAL: the Character stat increment is not made (see above).
}

// 0x421aac _ZN8MenuBase16FS_GetNumPotionsEPKcS1_Pv
// PARTIAL: the original reads the local Character's potion count (ItemInventory base at Character +0x37c) and publishes
// it as the number _root.retval of the menu. Character.hpp cannot be included here (IStreamBase conflict with
// PlayerManager.hpp), and RenderFX::SetMember(as_value) is not declared, so nothing is read or published. Returns true
// when a local Character exists, as in the original.
bool MenuBase::FS_GetNumPotions(const char*, const char*, void*) {
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    return character != nullptr;  // PARTIAL: the count is not read or published (see above).
}

// 0x421b58 _ZN8MenuBase12FS_GetOptionEPKcS1_Pv
// PARTIAL: the original reads the savegame option named args + 7 (the 7 leading characters are skipped; the original
// does not check the length of args) and publishes it as _root.retval. RenderFX::SetMember is not declared, so the
// value is read and not published. A null args returns false. Otherwise returns true as in the original.
bool MenuBase::FS_GetOption(const char*, const char* args, void*) {
    if (args == nullptr) {
        return false;
    }
    const int value = ApplicationGetInstance().m_savegameManager->getOption(args + 7);
    static_cast<void>(value);  // PARTIAL: _root.retval not set (see above).
    return true;
}

// 0x421ce0 _ZN8MenuBase11FS_IncSkillEPKcS1_Pv
// PARTIAL: the original adds one level to the local Character's skill numbered atoi(args) (Character::IncSkill(skill,
// false)), then publishes that skill's new level as text in _root.retval. Character.hpp cannot be included here (see
// FS_AssignPoint), so the skill is not raised and the level is not read. RenderFX::SetMember is not declared either. The
// text buffer the publish would use is 12 bytes: the original's 4-byte sprintf buffer overflows for four-digit levels.
// A null or empty args returns false. Otherwise returns true as in the original.
bool MenuBase::FS_IncSkill(const char*, const char* args, void*) {
    if (args == nullptr || *args == '\0') {
        return false;
    }
    Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayerCharacter();
    static_cast<void>(character);  // PARTIAL: IncSkill, SG_GetSkillLevel and the _root.retval publish are not made.
    return true;
}

// 0x421fe0 _ZN8MenuBase6UpdateEv
// PARTIAL: the original first releases the context when its proxy is dead (see GetCurrentMenuContext). It then calls
// RenderFX::IsAnimOver(m_menu, context) and, when the animation is over, increments m_animOverCount (+0x78).
// RenderFX::IsAnimOver is not declared in this tree, so the counter is not advanced.
void MenuBase::Update() {}

// 0x42204c _ZNK8MenuBase21GetCurrentMenuContextEv
// PARTIAL: the original returns the cached context m_character while its proxy (+0x48) is alive. When the proxy is dead
// it releases the proxy, clears m_characterProxy and m_character, and returns null. That branch needs the weak_proxy
// layout, which is not declared, so a dead context is returned as is.
gameswf::character* MenuBase::GetCurrentMenuContext() const {
    return m_character;
}

// 0x4220a0 _ZN8MenuBase12FS_StartGameEPKcS1_Pv
// PARTIAL: only the first step is rebuilt. Unless args is "false", the original closes the menu stack (MenuFX::PopAll on
// m_menu). The rest is not rebuilt: it reads the last save slot (SavegameManager +0x08, m_lastSlot, private with no
// accessor; -1 becomes 0), deletes an existing save in that slot, builds a PlayerSavegame for it (mode 1), reads the
// menu's string at +0x94 and the MenuCharacterSelect class id (+0x164), sets the save date, writes the save, and
// calls Application::LoadLevel with the level name from Arrays::LevelList. Some PlayerSavegame fields are written at
// offsets derived from the static difficulty level, which the pseudocode does not resolve. Returns true as in the original.
bool MenuBase::FS_StartGame(const char*, const char* args, void* menu) {
    if (args == nullptr || std::strcmp(args, "false") != 0) {
        static_cast<MenuBase*>(menu)->m_menu->PopAll();
    }
    return true;  // PARTIAL: the save and level-load steps are not made (see above).
}

// 0x422280 _ZN8MenuBase16FS_SetPlayerNameEPKcS1_Pv
// Sets the player name (m_playerName, +0x80). A null args sets "Bobb", a 4-character literal: the assembly passes its
// end pointer as aBobb+4. Returns true.
bool MenuBase::FS_SetPlayerName(const char*, const char* args, void* menu) {
    MenuBase* self = static_cast<MenuBase*>(menu);
    self->m_playerName.assign(args != nullptr ? args : "Bobb");
    return true;
}

// 0x4222d8 _ZN8MenuBase16FS_SetDifficultyEPKcS1_Pv
// Sets the difficulty name (m_difficultyName, +0x9c) to args. The original takes strlen(args) without a null test; a
// null args is treated as the empty string here. Returns true.
bool MenuBase::FS_SetDifficulty(const char*, const char* args, void* menu) {
    static_cast<MenuBase*>(menu)->m_difficultyName.assign(args != nullptr ? args : "");
    return true;
}

// 0x422304 _ZN8MenuBase12FS_StopMusicEPKcS1_Pv
// Stops the music unless the main menu is on the stack with its context. The main menu's context is taken as alive
// (see GetCurrentMenuContext). VoxSoundManager::StopMusic(VoxSoundManager::s_instance, 0) is not called: VoxSoundManager
// is not declared in this tree. Returns false when the main menu's context is on the stack, true otherwise.
bool MenuBase::FS_StopMusic(const char*, const char*, void*) {
    MenuBase* mainMenu = MenuManager::GetInstance()->GetMenuByName("menu_MainMenu");
    if (mainMenu != nullptr && mainMenu->m_character != nullptr) {
        MenuFX::State* handler = mainMenu->m_menu->GetStateHandler(mainMenu->m_character);
        if (handler != nullptr && mainMenu->m_menu->IsStateInStack(handler)) {
            return false;
        }
    }
    return true;  // PARTIAL: VoxSoundManager::StopMusic not called (see above).
}

// 0x4223bc _ZN8MenuBase10SetVisibleEb
// Sets the visible flag (m_visible, +0x74) when the menu is valid (virtual IsValidMenu, slot +0x3c).
// PARTIAL: before the flag is set, the original writes the same value to the flag byte at +0x9b of the context character.
// The gameswf::character layout is not declared, so that write is not made, and a dead context is not released.
void MenuBase::SetVisible(bool visible) {
    if (IsValidMenu()) {
        m_visible = visible;
    }
}

// 0x422620 _ZN8MenuBase18RegisterSlideEventEv
// PARTIAL: the original clears the slide events, then, for each character that RenderFX::FindCharacters(m_menu, context,
// "slide_", 0) returns, appends a new SlideEventCharacter (44 bytes) to m_slideEvents. FindCharacters is not declared in
// this tree, so no slide event is registered. The context is taken as alive (see GetCurrentMenuContext).
void MenuBase::RegisterSlideEvent() {
    ClearSlideEvents();
}

// 0x422824 _ZN8MenuBaseD1Ev
// Complete-object destructor. Clears the slide events, then destroys the DragAndDrop (its destructor, then CustomFree).
// The members that follow are released implicitly in reverse declaration order, as in the original: m_deadZones (+0xb8),
// m_difficultyName (+0x9c), m_playerName (+0x80), the list at +0x6c, and the m_slideEvents storage at +0x60.
// PARTIAL: the original's last step releases the two weak_ptr proxies of MenuFX::State (+0x50, then +0x48), which the
// State destructor performs in this tree and which is partial there (MenuFX batch 0).
MenuBase::~MenuBase() {
    ClearSlideEvents();
    if (m_dragAndDrop != nullptr) {
        m_dragAndDrop->~DragAndDrop();
        CustomFree(m_dragAndDrop);
        m_dragAndDrop = nullptr;
    }
}

// 0x422958 _ZN8MenuBaseD0Ev
// Compiler-generated deleting destructor: runs ~MenuBase and frees the object with CustomFree. The C++ delete expression
// produces it, so it is not written here.

// 0x422974 _ZN8MenuBaseD2Ev
// Second emitted copy of the 0x422824 body (identical bytes, the base-object variant). One C++ destructor covers both.
// Same omission as 0x422824: the proxy releases in MenuFX::State::~State. Not a separate function in C++.

// 0x422d10 _ZN8MenuBase19ProcessLocalizationEv
// PARTIAL: not rebuilt. With a context (see GetCurrentMenuContext), the original sets the context on m_menu
// (RenderFX::SetContext). Then, for the string sheets 19, 20 and 28, it looks up each localized string under the menu's
// name (m_name, +0x08) in three forms: "<name>.btn_<str>.text", "<name>.<str>.text" and "<name>.<str>". It finds the
// character with RenderFX::GetSearchIndex and SearchIndex::Find. Each found character whose virtual slot +0x08 returns
// true for argument 32 gets RenderFX::FormatHTML(m_menu, character, "%s", string, 1). It sets m_isLocalised (+0x75) at
// the end. RenderFX::SetContext, GetSearchIndex, SearchIndex::Find and FormatHTML are not declared in this tree, and the
// gameswf::character virtual at +0x08 is not named, so nothing is done here. The return value of the original is ignored.
void MenuBase::ProcessLocalization() {}

// 0x423098 _ZNK8MenuBase14HasHitDeadZoneEii
// True when the point (x, y) lies inside a dead zone (xmin <= x <= xmax and ymin <= y <= ymax, the rect's float order).
// The trace point runs only on a hit.
bool MenuBase::HasHitDeadZone(int x, int y) const {
    const float fx = static_cast<float>(x);
    const float fy = static_cast<float>(y);
    for (const gameswf::rect& zone : m_deadZones) {
        if (fx < zone.m_x_min || fx > zone.m_x_max || fy < zone.m_y_min || fy > zone.m_y_max) {
            continue;
        }
        TraceMenuBaseSwitch();
        return true;
    }
    return false;
}

// 0x4231b4 _ZN8MenuBase11PostOnEventERN8RenderFX5EventE
// Consumes the current event when it lies on a dead zone. Nothing happens while no event is being handled
// (MenuManager::m_currentEvent, +0xb4, is null). The raw event position comes from MenuManager.
void MenuBase::PostOnEvent(RenderFX::Event&) {
    MenuManager* manager = MenuManager::GetInstance();
    if (manager->m_currentEvent == nullptr) {
        return;
    }
    int x = 0;
    int y = 0;
    manager->GetRawCurrentEventPosition(x, y);
    if (HasHitDeadZone(x, y)) {
        MenuManager::GetInstance()->consumeEvent();
    }
}

// 0x423214 _ZN8MenuBase7OnEventERN8RenderFX5EventE
// The drag-and-drop table gets the event first. Every path of the original then ends with PostOnEvent, which this does.
// PARTIAL: the original also, when m_isRolloverEventEnabled is set and the event is a rollover of a "btn" character that
// the event target's virtual slot +0x170 accepts, sets the focus on that character (RenderFX::Event::GetCharacterPath and
// RenderFX::SetFocus). It also opens the Twitter page with nativeOpenBrowser when the event kind is byte_6 and the event
// name contains "btn_MENU_TWITTER". The RenderFX::Event fields, the byte_6 and byte_8 kinds, SetFocus and nativeOpenBrowser
// are not declared in this tree, so those branches are not made.
void MenuBase::OnEvent(RenderFX::Event& e) {
    if (m_dragAndDrop != nullptr) {
        m_dragAndDrop->OnEvent(e);
    }
    PostOnEvent(e);
}

// 0x42331c _ZN8MenuBase17RegisterDeadZonesEv
// Registers the dead zones of the menu once. Its flag (+0xb4) is set before the zones are read, as in the original.
// PARTIAL: the original then takes RenderFX::FindCharacters(m_menu, context, "deadzone_", 0) and, for each character it
// returns, appends GameSWFUtils::GetAbsoluteBoundingRect(character) (xmin, xmax, ymin, ymax) to m_deadZones. FindCharacters
// is not declared in this tree, so no zone is added. The context is taken as alive (see GetCurrentMenuContext).
void MenuBase::RegisterDeadZones() {
    if (m_deadZonesRegistered || m_character == nullptr) {
        return;
    }
    m_deadZonesRegistered = true;
    TraceMenuBaseSwitch();
}
