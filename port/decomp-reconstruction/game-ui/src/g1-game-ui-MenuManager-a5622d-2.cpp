// MenuManager bodies, batch g1-game-ui-MenuManager-a5622d-2 (part 3 of 3 of MenuManager). Rebuilt from the Hex-Rays
// pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM offsets.
//
// Rebuilt in full: RegisterListener, PushMenu(const char*), and Init steps 4 (HUD caches), 5 (property names), 24
// (drag-and-drop pass) and 25 (the 174 native registrations of kMenuNatives).
// The tail of LoadMenu and LoadMainMenu (PostLoad, then the drag-and-drop pass for the menus added since) is kept.
//
// PARTIAL: the statements that need declarations the tree does not have are left out, and the rest is kept.
//   - dh2/MultiMenuManager.hpp is in the tree. m_multiMenuManager (+0xf4) is used for m_menuFX[0..3] (+0x134 .. +0x143),
//     LoadSWFFile and the vtable slots +0x34 (push), +0x38 (pop) and +0x40 (IsStateInStack). The constructor allocates it.
//   - Menu classes with no header in the tree yet (each waits on its dh2/<Class>.hpp): MenuMainMenu,
//     MenuCharacterSelect, MenuEnterName, MenuMultiplayerLobbyMulti, MenuCharMenu_InvMain, MenuCharMenu_Map, MenuWorldMap,
//     MenuDebug, MenuDebugHUD, MenuMerchant, MenuConfirm, MenuHUDConfirm, MenuLeaderboard.
//   - RenderFX (dh2/RenderFX.hpp is a placeholder): GetFlashRoot, FindCharacters, GetContext, Find, GetRoot's
//     display callback, RegisterDisplayCallback, SetInputBehavior, SetEventListener, InvokeASCallback and the
//     Initialize parameter block.
//   - gameswf: register_log_callback, as_value, smart_ptr_proxy and weak_ptr proxies, the character name (a tu_string
//     at character +0x44).
//   - IEvent (the type and payload accessors), TouchScreenBase, OnlineSingleton<OnlineGameState>, Level::m_isInGameView.
//   - Binary globals with no declaration in the tree: Singleton<Application>::s_inst, byte_99F7D7, Width_Screen,
//     HTC_DEVICES, NO_IGP, s_SWFfileToLoad and GameSWFUtils::CACHE_CHARS_*. The four globals used here are Application
//     members (s_inst at 0x99f72c): dword_99F73C = +0x10 m_device, dword_99F740 = +0x14 m_eventManager,
//     dword_99F74C = +0x20 m_touchScreen, dword_99F778 = +0x4c m_savegameManager (Application.hpp offsets).

#include "dh2/MenuManager.hpp"

#include <cstddef>
#include <string>
#include <vector>

#include "dh2/CharProperties.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/FlashAnimManager.hpp"
#include "dh2/HUDControls.hpp"
#include "dh2/InfoHUDManager.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/Application.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/global/menu_manager_events_load.hpp"
#include "dh2/global/native_app.hpp"
#include "dh2/global/native_app_bindings.hpp"
#include "dh2/global/native_app_hud_level.hpp"
#include "dh2/global/native_gameswf_menus.hpp"
#include "dh2/global/native_get.hpp"
#include "dh2/global/native_online_gc.hpp"
#include "dh2/global/native_player_actions.hpp"
#include "dh2/global/native_player_info_options.hpp"
#include "dh2/global/native_sound_menu.hpp"
#include "dh2/global/native_stats_inventory_merchant.hpp"

// These three declarations repeat the prototypes of dh2/global/runtime_support.hpp (PushProfilingContext 0x3136b4,
// PopProfilingContext 0x3136b8) and of core-platform/w7_global_rem.hpp (NatvieResumeAllSounds 0x43acb4). Those headers
// are not included here: they pull in core-platform's glitch core, which clashes with the glitch-shared core that
// FlashAnimManager.hpp and HUDControls.hpp need in the same unit. The signatures are identical.
void PushProfilingContext(const char* name);
void PopProfilingContext(const char* name);
int NatvieResumeAllSounds(const gameswf::fn_call& call);

namespace gameswf {
// The native registration gameswf offers the Flash script (lib-gameswf, 0x76ef5c, signature from the binary's symbol).
// The vendored gameswf r1714 does not define it, so the integrator must provide it.
void register_as_native_function(const char* name, void (*fn)(const fn_call&));
}  // namespace gameswf

namespace {

using NativeFn = void (*)(const gameswf::fn_call&);

struct NativeBinding {
    const char* name;  // the name the Flash script calls: the binary's string, misspellings included
    NativeFn fn;       // the handler
};

// The natives Init registers in step 25, in the original order. NatvieResumeAllSounds is declared with an int return
// in core-platform (its return is the leftover r0 of the last call), so it is taken through a void-returning pointer,
// as the gameswf registration expects.
const NativeBinding kMenuNatives[] = {
    {"NativeGetSkillDetails", NativeGetSkillDetails},
    {"NativeEquipSkill", NativeEquipSkill},
    {"NativeGetPlayerInfos", NativeGetPlayerInfos},
    {"NativeGetPlayerRanking", NativeGetPlayerRanking},
    {"NativeGetPlayerStats", NativeGetPlayerStats},
    {"NativeGetSaveSlotDetails", NativeGetSaveSlotDetails},
    {"NativeEraseSaveSlot", NativeEraseSaveSlot},
    {"NativeFindFreeSaveSlot", NativeFindFreeSaveSlot},
    {"NativeGetSaveIDsListSize", NativeGetSaveIDsListSize},
    {"NativeGetSaveIDsList", NativeGetSaveIDsList},
    {"NativeSetSaveSlotIDToMainMenu", NativeSetSaveSlotIDToMainMenu},
    {"NativeInvAutoEquipSlot", NativeInvAutoEquipSlot},
    {"NativeInvGetHasTwoHandedWeapon", NativeInvGetHasTwoHandedWeapon},
    {"NativeInvGetHasOffHandWeapon", NativeInvGetHasOffHandWeapon},
    {"NativeInvGetEquipedItem", NativeInvGetEquipedItem},
    {"NativeGetMultiplayerGameRooms", NativeGetMultiplayerGameRooms},
    {"NativeGetMultiplayerRoomInfo", NativeGetMultiplayerRoomInfo},
    {"NativeGetMultiplayerQuestInfo", NativeGetMultiplayerQuestInfo},
    {"NativeConnectToServer", NativeConnectToServer},
    {"NativeSendReady", NativeSendReady},
    {"NativeCreateMultiplayerLobby", NativeCreateMultiplayerLobby},
    {"NativeOnlineSanityCheck", NativeOnlineSanityCheck},
    {"NativeConectionType", NativeConectionType},
    {"NativeGetNextStatusMessage", NativeGetNextStatusMessage},
    {"NativeGetNextOnlineStatusMessage", NativeGetNextOnlineStatusMessage},
    {"NativeStatsReset", NativeStatsReset},
    {"NativeStatsRemoveAssign", NativeStatsRemoveAssign},
    {"NativeInvTransmuteItem", NativeInvTransmuteItem},
    {"NativeInvGetItemsListForSlot", NativeInvGetItemsListForSlot},
    {"NativeInvGetItemDetails", NativeInvGetItemDetails},
    {"NativeInvEquipItem", NativeInvEquipItem},
    {"NativeInvUnequipItem", NativeInvUnequipItem},
    {"NativeHUDSetActiveFaery", NativeHUDSetActiveFaery},
    {"NativeHUDGetIsFaeryUnlocked", NativeHUDGetIsFaeryUnlocked},
    {"NativeHUDGetActiveFaery", NativeHUDGetActiveFaery},
    {"NativeSkillsGetSkillPointsLeft", NativeSkillsGetSkillPointsLeft},
    {"NativeSkillsTrainSkill", NativeSkillsTrainSkill},
    {"NativeStatsAssignPoint", NativeStatsAssignPoint},
    {"NativeInvGetPlayerGold", NativeInvGetPlayerGold},
    {"NativeSkillGetEquipedSkillsIDs", NativeSkillGetEquipedSkillsIDs},
    {"NativeLockCharacter", NativeLockCharacter},
    {"NativeUnlockCharacter", NativeUnlockCharacter},
    {"NativeGetNextTutorialMessage", NativeGetNextTutorialMessage},
    {"NativeGetCharMenuTutorialMessage", NativeGetCharMenuTutorialMessage},
    {"NativeSkipCharMenuTutorialMessage", NativeSkipCharMenuTutorialMessage},
    {"NativeGetNextDialogMessage", NativeGetNextDialogMessage},
    {"NativeGetNextQuestMessage", NativeGetNextQuestMessage},
    {"NativeGetNextAchievementMessage", NativeGetNextAchievementMessage},
    {"NativeSkipAchievementMessage", NativeSkipAchievementMessage},
    {"NativeOnHUDOpenEvent", NativeOnHUDOpenEvent},
    {"NativeSetLevelPause", NativeSetLevelPause},
    {"NativeGetQuestCount", NativeGetQuestCount},
    {"NativeGetQuestIDsInRange", NativeGetQuestIDsInRange},
    {"NativeGetQuestDetails", NativeGetQuestDetails},
    {"NativeSetCurrentQuest", NativeSetCurrentQuest},
    {"NativeGetLoadingProgress", NativeGetLoadingProgress},
    {"NativeGetLoadingTipStrID", NativeGetLoadingTipStrID},
    {"NativeGetStringFromID", NativeGetStringFromID},
    {"NativeGetStringFromSymbol", NativeGetStringFromSymbol},
    {"NativeEnterOptionMenu", NativeEnterOptionMenu},
    {"NativeOptionFX", NativeOptionFX},
    {"NativeOptionMusic", NativeOptionMusic},
    {"NativeGetOptionParameters", NativeGetOptionParameters},
    {"NativeSetOptions", NativeSetOptions},
    {"NativeGetMapIcons", NativeGetMapIcons},
    {"NativeLoadSettings", NativeLoadSettings},
    {"NativeSaveSettings", NativeSaveSettings},
    {"NativeCreateSaveSlot", NativeCreateSaveSlot},
    {"NativeAssignSaveSlotToPlayer", NativeAssignSaveSlotToPlayer},
    {"NativeStartGame", NativeStartGame},
    {"NativeGetStringNumPotions", NativeGetStringNumPotions},
    {"NativeGetNumPotions", NativeGetNumPotions},
    {"NativeUsePotion", NativeUsePotion},
    {"NativePushMenu", NativePushMenu},
    {"NativePopMenu", NativePopMenu},
    {"NativePopAllMenus", NativePopAllMenus},
    {"NativePopAllAbove", NativePopAllAbove},
    {"NativeReviveAllPlayers", NativeReviveAllPlayers},
    {"NativeEndLoading", NativeEndLoading},
    {"NativeMustWaitForHost", NativeMustWaitForHost},
    {"NativeIsMultiplayerLoadCompleted", NativeIsMultiplayerLoadCompleted},
    {"NativeHUDInteract", NativeHUDInteract},
    {"NativeHUDSpell", NativeHUDSpell},
    {"NativeHUDSkill", NativeHUDSkill},
    {"NativePushState", NativePushState},
    {"NativeGoToMainMenu", NativeGoToMainMenu},
    {"NativePlaySoundFX", NativePlaySoundFX},
    {"NativePlayMusic", NativePlayMusic},
    {"NativePlayPreviousMusic", NativePlayPreviousMusic},
    {"NativeStopMusic", NativeStopMusic},
    {"NativePauseMusic", NativePauseMusic},
    {"NativePauseAllSounds", NativePauseAllSounds},
    {"NatvieResumeAllSounds", reinterpret_cast<NativeFn>(NatvieResumeAllSounds)},
    {"NativeSkipScript", NativeSkipScript},
    {"NativeStopMessage", NativeStopMessage},
    {"NativeStartMultiplayerGame", NativeStartMultiplayerGame},
    {"NativeGetParsedString", NativeGetParsedString},
    {"NativeSelectClassSpec", NativeSelectClassSpec},
    {"NativeGetPossibleClassSpec", NativeGetPossibleClassSpec},
    {"NativeGetMonsterHUDHP", NativeGetMonsterHUDHP},
    {"NativeDebugConsole", NativeDebugConsole},
    {"NativeBackToHud", NativeBackToHud},
    {"NativeAwayFromHud", NativeAwayFromHud},
    {"NativeGLLive", NativeGLLive},
    {"NativeLaunchIGP", NativeLaunchIGP},
    {"NativeLaunchTwitter", NativeLaunchTwitter},
    {"NativeSwapEquipment", NativeSwapEquipment},
    {"NativeMerchantGetItemsListForSlot", NativeMerchantGetItemsListForSlot},
    {"NativeMerchantBuyItem", NativeMerchantBuyItem},
    {"NativeMerchantSellItem", NativeMerchantSellItem},
    {"NativeGoToZone", NativeGoToZone},
    {"NativeGetWorldMapLocations", NativeGetWorldMapLocations},
    {"NativeInvDropItem", NativeInvDropItem},
    {"NativeGetBuffs", NativeGetBuffs},
    {"NativeLockZoom", NativeLockZoom},
    {"NativeUnlockZoom", NativeUnlockZoom},
    {"NativeShowMinimap", NativeShowMinimap},
    {"NativeHideMinimap", NativeHideMinimap},
    {"NativeShowMinimapLegend", NativeShowMinimapLegend},
    {"NativeResetMapZoom", NativeResetMapZoom},
    {"getCursorState", NativeGetCursorState},
    {"NativePinchZoom", NativePinchZoom},
    {"NativeResetZoom", NativeResetZoom},
    {"NativeShowStatusBar", NativeShowStatusBar},
    {"NativeMultiplayerBack", NativeMultiplayerBack},
    {"NativeTouchToMove", NativeTouchToMove},
    {"NativeIPodControl", NativeIPodControl},
    {"NativeGLXPlayerLoggedIn", NativeGLXPlayerLoggedIn},
    {"NativeLoginInfos", NativeLoginInfos},
    {"NativeGLXPlayerSignin", NativeGLXPlayerSignin},
    {"NativeSendRoomSearchRequest", NativeSendRoomSearchRequest},
    {"NativeOpenKeyboard", NativeOpenKeyboard},
    {"NativeGetKeyboardLastString", NativeGetKeyboardLastString},
    {"NativeCloseKeyboard", NativeCloseKeyboard},
    {"NativeChangeRolloverInputBehavior", NativeChangeRolloverInputBehavior},
    {"NativeWiFiCheck", NativeWiFiCheck},
    {"NativeBluetoothCheck", NativeBluetoothCheck},
    {"NativeIsMultiplayerGame", NativeIsMultiplayerGame},
    {"NativeRefreshHudManager", NativeRefreshHudManager},
    {"NativeKickPlayerOut", NativeKickPlayerOut},
    {"NativeIsMultiplayerHost", NativeIsMultiplayerHost},
    {"NativeIsGLLiveMultiplayer", NativeIsGLLiveMultiplayer},
    {"NativeSetCurrentDifficulty", NativeSetCurrentDifficulty},
    {"NativeShowLeaderboard", NativeShowLeaderboard},
    {"NativeEnableGameCenter", NativeEnableGameCenter},
    {"NativeGCAchievements", NativeGCAchievements},
    {"NativeGCInvite", NativeGCInvite},
    {"NativeGCLeaderboards", NativeGCLeaderboards},
    {"NativeGCAutoMatch", NativeGCAutoMatch},
    {"NativeStartFromGCInvite", NativeStartFromGCInvite},
    {"NativeInvitePlayer", NativeInvitePlayer},
    {"NativeSendGLInvite", NativeSendGLInvite},
    {"NativeAddFriend", NativeAddFriend},
    {"NativeUseIpodPlayer", NativeUseIpodPlayer},
    {"NativeIPodSetShuffle", NativeIPodSetShuffle},
    {"NativeIPodSetPlaylist", NativeIPodSetPlaylist},
    {"NativeGetIPodPlaylistName", NativeGetIPodPlaylistName},
    {"NativeGetIPodPlaylistCount", NativeGetIPodPlaylistCount},
    {"NativeUpdateOrientation", NativeUpdateOrientation},
    {"NativeEnableBluetooth", NativeEnableBluetooth},
    {"NativeGetCreditMovement", NativeGetCreditMovement},
    {"NativeSaveGame", NativeSaveGame},
    {"NativeReloadSkills", NativeReloadSkills},
    {"NativeIsKorean", NativeIsKorean},
    {"NativeIsOnlineMode", NativeIsOnlineMode},
    {"NativeSetMultitouch", NativeSetMultitouch},
    {"NativeIsMultiplayerEnabled", NativeIsMultiplayerEnabled},
    {"NativeScreenIsBlack", NativeScreenIsBlack},
    {"NativeIsBluetooth", NativeIsBluetooth},
    {"NativeIsJapaneseVersion", NativeIsJapaneseVersion},
    {"NativeHasPushNotification", NativeHasPushNotification},
    {"NativeDoWeHaveInternet", NativeDoWeHaveInternet},
    {"NativeIsBluetoothWifiEnabled", NativeIsBluetoothWifiEnabled},
    {"NativeBeginCustom", NativeBeginCustom},
};

// The HUD bring-up that Init step 4 and LoadMenu(3) both run, in the original order: the InfoHUDManager and the
// HUDControls caches are pointed at the HUD movie and rebuilt, then the HUD movie is scanned for FlashAnim clips.
// (The original's InfoHUDManager +0x57c and HUDControls +0x658 are both set to the HUD root, which is multi->m_menuFX[3].)
void InitHudCaches(MenuFX* hudRoot) {
    InfoHUDManager* hud = InfoHUDManager::GetInstance();
    hud->m_menu = hudRoot;
    hud->initCachedChars();
    HUDControls* controls = HUDControls::GetInstance();
    controls->m_menu = hudRoot;
    controls->initCachedChars();
    FlashAnimManager::GetInstance()->ScanForAnims(hudRoot);
}

// The drag-and-drop pass of LoadMenu and LoadMainMenu: for each menu from index first on, its drag and drop is registered
// with no filter and then it is hidden (the original re-reads the list bounds on every pass).
void RegisterDragAndHideFrom(const std::vector<MenuBase*>& menus, std::size_t first) {
    for (std::size_t i = first; i < menus.size(); ++i) {
        MenuBase* menu = menus[i];
        menu->RegisterDragAndDrops(nullptr);
        menu->Hide();
    }
}

}  // namespace

// 0x42efb8 _ZN11MenuManager8PostLoadEv
// PARTIAL: the whole body is left out. The original walks the four multi->m_menuFX movies, takes the "menu_" movies of each
// (RenderFX::FindCharacters), creates a MenuBase for every name that is not registered yet (operator new(0xc4), the
// m_deleteOnUnload flag at +0x7d set, then RegisterMenu on the singleton), and registers the "flush_text" characters with
// RenderFX::RegisterDisplayCallback. None of those are declared in the tree yet.
void MenuManager::PostLoad() {}

// 0x42f304 _ZN11MenuManager4InitEv
// One step per call: the step counter m_initStep (+0xbc) selects the case, and the counter advances after each step.
// Step 26 returns true at once without advancing, which marks the init done.
bool MenuManager::Init() {
    switch (m_initStep) {
        case 0:
        case 2:
        case 3:
            // PARTIAL: the SWF load of this step is left out. It clears multi->m_menuFX companion words, checks the file with
            // a virtual call on the object at dword_99F73C+52, picks the file from s_SWFfileToLoad by Width_Screen,
            // HTC_DEVICES, NO_IGP and the language, calls MultiMenuManager::LoadSWFFile, and sets the input behaviour
            // (RenderFX::SetInputBehavior with 132, or 4 for step 3). None of those are declared in the tree.
            break;
        case 1:
            // PARTIAL: clears two MultiMenuManager words at +0x138 and +0x148. MultiMenuManager is not in the tree.
            break;
        case 4:
            InitHudCaches(GetHUDRoot());
            break;
        case 5:
            CharProperties::LoadPropNames("PyData/character_properties_pystructnames.bin");
            break;
        case 7:
            // PARTIAL: MenuMainMenu::GetInstance(this) left out. Waits on dh2/MenuMainMenu.hpp.
            break;
        case 9:
            // PARTIAL: MenuCharacterSelect::GetInstance(this) left out. Waits on dh2/MenuCharacterSelect.hpp.
            break;
        case 11:
            // PARTIAL: MenuCharMenu_InvMain, MenuCharMenu_Map and MenuWorldMap GetInstance calls left out.
            // Waits on dh2/MenuCharMenu_InvMain.hpp, dh2/MenuCharMenu_Map.hpp and dh2/MenuWorldMap.hpp.
            break;
        case 13:
            // PARTIAL: MenuEnterName::GetInstance(this) left out. Waits on dh2/MenuEnterName.hpp.
            break;
        case 14:
            // PARTIAL: MenuMultiplayerLobbyMulti::GetInstance(this) left out. Waits on dh2/MenuMultiplayerLobbyMulti.hpp.
            break;
        case 17:
            // PARTIAL: MenuDebug::GetInstance and MenuDebugHUD::GetInstance left out. Waits on dh2/MenuDebug.hpp and dh2/MenuDebugHUD.hpp.
            break;
        case 18:
            // PARTIAL: MenuMerchant::GetInstance(this) left out. Waits on dh2/MenuMerchant.hpp.
            break;
        case 19:
            // PARTIAL: MenuConfirm::GetInstance(this) left out. Waits on dh2/MenuConfirm.hpp.
            break;
        case 20:
            // PARTIAL: MenuHUDConfirm::GetInstance(this) left out. Waits on dh2/MenuHUDConfirm.hpp.
            break;
        case 23:
            PostLoad();
            CharProperties::UnLoadPropNames();
            // PARTIAL: RenderFX::SetEventListener(GetHUDRoot(), <EventListener subobject at +0x04>) and the loop over
            // "preload_font.preload_font1" .. "preload_font6" (RenderFX::GetContext, RenderFX::Find, and the four-argument
            // GameSWFUtils::PreloadGlyph with a cache object first) are left out. The header declares PreloadGlyph with
            // three arguments, and RenderFX has no Find or GetContext.
            break;
        case 24: {
            // Drag-and-drop pass over the menus that exist when the step starts (the original captures the list bounds once).
            const std::size_t count = m_menus.size();
            for (std::size_t i = 0; i < count; ++i) {
                MenuBase* menu = m_menus[i];
                menu->RegisterDragAndDrops(nullptr);
                menu->Hide();
            }
            break;
        }
        case 25:
            for (const NativeBinding& binding : kMenuNatives) {
                gameswf::register_as_native_function(binding.name, binding.fn);
            }
            break;
        case 26:
            // PARTIAL: EventManager::Attach(dword_99F740, 5, this, 10), (..., 4, this, 10) and (..., 7, this, 10) are left out.
            // dword_99F740 is Application +0x14 (m_eventManager), but no Application instance accessor is declared in the tree.
            // The step still reports done.
            return true;
        default:
            break;
    }
    ++m_initStep;
    return false;
}

// 0x430ad4 _ZN11MenuManager7onEventEPK6IEventPK12EventManager
// PARTIAL: the prologue, the two debug-switch gates, the event bookkeeping, the cursor read and the epilogue are rebuilt.
// The event dispatch in the middle is left out: the event type and payload (IEvent::GetType and the fields at +0x08..+0x14
// of the event), the key events (the button name comes from MenuManagerButtonName, its as_value and the
// RenderFX::InvokeASCallback "onInputEvent" call are not declared), the touch paths that set m_currentEvent and return
// consumed, and the RenderFX::UpdateCursor loop with the online and level checks (Level +0x198, GetOnline, OnlineGameState).
// Consequently consumed is only what consumeEvent already set, and the menus do not see the event yet.
bool MenuManager::onEvent(const IEvent* event, const EventManager* manager) {
    static_cast<void>(manager);
    PushProfilingContext("MenuManager::onEvent");
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("IsDeactivatingFlashMenus")) {
        PopProfilingContext("MenuManager::onEvent");
        return false;
    }
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("IsDeactivatingFlashMenusUpdate")) {
        PopProfilingContext("MenuManager::onEvent");
        return false;
    }
    m_eventConsumed = 0;
    m_currentEvent = event;
    if (event == nullptr) {
        PopProfilingContext("MenuManager::onEvent");
        return false;
    }

    // Cursor position for the dispatch: the raw position, with z = 0 (float[3] in the original).
    int cursorX = 0;
    int cursorY = 0;
    GetRawCurrentEventPosition(cursorX, cursorY);
    const float cursor[3] = {static_cast<float>(cursorX), static_cast<float>(cursorY), 0.0f};
    static_cast<void>(cursor);  // PARTIAL: passed to RenderFX::UpdateCursor by the dispatch that is left out

    m_currentEvent = nullptr;
    PopProfilingContext("MenuManager::onEvent");
    return m_eventConsumed != 0;
}

// 0x431750 _ZN11MenuManager16RegisterListenerEP8MenuBase
void MenuManager::RegisterListener(MenuBase* menu) {
    m_listeners[menu] = true;
}

// 0x4317e8 _ZN11MenuManager8PushMenuEP8MenuBase
// PARTIAL: the TouchScreenBase::ProcessEvents(dword_99F74C) call of the push path is omitted (dh2/TouchScreenBase.hpp is
// absent). The rest is as in the original. When the menu is not valid (IsValidMenu, vtable +0x3c) or its state is already
// in a movie stack (MultiMenuManager::IsStateInStack, vtable +0x40), nothing is pushed. Otherwise the touch state is reset
// (Application::ResetTouch), the menu is pushed on the MultiMenuManager (vtable +0x34), and the isTracingMenuManager switch
// is read. The listener step then runs on both paths: the menu is registered when its MenuFX (+0x04) is the HUD root.
// A null menu returns early.
void MenuManager::PushMenu(MenuBase* menu) {
    if (menu == nullptr) {
        return;
    }
    if (menu->IsValidMenu() && !m_multiMenuManager->IsStateInStack(menu)) {
        ApplicationGetInstance().ResetTouch();
        // PARTIAL: TouchScreenBase::ProcessEvents(m_touchScreen) omitted, see above.
        m_multiMenuManager->PushMenu(menu);
        (void)DebugSwitches::GetInstance()->GetSwitch("isTracingMenuManager");
    }
    if (menu->m_menu == GetHUDRoot()) {
        RegisterListener(menu);
    }
}

// 0x431924 _ZN11MenuManager8PushMenuEPKc
void MenuManager::PushMenu(const char* name) {
    MenuBase* menu = GetMenuByName(name);
    if (menu != nullptr) {
        PushMenu(menu);
    }
}

// 0x4319cc _ZN11MenuManagerC2Ev (complete-object copy)
// 0x431c38 _ZN11MenuManagerC1Ev (base-object copy). The two bodies match, so one definition serves both.
// The base classes and members that are constructed in place (IEventReceiver and EventListener vptrs, m_debugCachedCharacter
// at +0x20, m_menus at +0x64, m_listeners at +0x70, the m_menuFXDeque at +0xcc) are handled by the C++ constructors.
// PARTIAL: gameswf::register_log_callback(GameSWFUtils::gameswf_logger) and RenderFX::Initialize (the 512 x 512 texture
// size, GameSWFUtils::SwfTextureLoader and the device record at dword_99F73C+16) are left out. The gameswf callback
// type does not match the header's int return, and RenderFX has no Initialize. The MultiMenuManager is allocated after the
// three DebugSwitches are set, as in the original (operator new(0x164, 8) and its constructor).
MenuManager::MenuManager() {
    unk_50 = 0;
    unk_54 = 0;
    unk_58 = 0;
    unk_5c = 0;
    unk_60 = 0;
    m_resetListeners = false;
    m_eventConsumed = 0;
    unk_b0 = 0;
    m_initStep = 0;
    unk_c0 = 0;
    unk_c4 = true;
    unk_f8 = 0;
    unk_fc = 0;
    unk_100 = 0;
    unk_104 = 0;
    m_actionIconType = -1;
    unk_10c = 0;
    unk_110 = false;

    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->SetSwitch("IsDeactivatingFlashMenus", false);
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->SetSwitch("IsDeactivatingFlashMenusUpdate", false);
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->SetSwitch("IsDeactivatingFlashMenusRender", false);

    m_multiMenuManager = new MultiMenuManager();
}

// 0x431ea4 _ZN11MenuManager8LoadMenuEi
// PARTIAL: the menu load is left out. The original picks the SWF for index from s_SWFfileToLoad (by Width_Screen,
// HTC_DEVICES and NO_IGP) and calls MultiMenuManager::LoadSWFFile, sets the input behaviour (RenderFX::SetInputBehavior
// with 132), calls Update(1, false) on multi->m_menuFX[index], and runs the registration chain for MenuDebug, MenuDebugHUD,
// MainMenu, Merchant, CharacterSelect, EnterName, MultiplayerLobbyMulti, CharMenu_InvMain, CharMenu_Map and WorldMap, each
// with a release of its gameswf weak_ptr proxy first. Only the HUD bring-up for index 3 (minus RenderFX::SetEventListener)
// and the tail are kept. For index > 3 the original calls through a null MenuFX, so that path is not rebuilt.
void MenuManager::LoadMenu(int index) {
    if (index == 3) {
        // PARTIAL: RenderFX::SetInputBehavior(m_menuFX[3], 4) and RenderFX::SetEventListener(GetHUDRoot(), this+4) left out.
        InitHudCaches(GetHUDRoot());
    }
    const std::size_t before = m_menus.size();
    PostLoad();
    RegisterDragAndHideFrom(m_menus, before);
}

// 0x4324dc _ZN11MenuManager12LoadMainMenuEv
// PARTIAL: the SWF load and the registration chain are left out. The original loads the file that
// MenuManagerMainMenuSwf picks for Width_Screen, HTC_DEVICES, NO_IGP and the language (MultiMenuManager::LoadSWFFile with
// index 2), sets the root's input behaviour (RenderFX::SetInputBehavior, 132), and registers the MenuDebug, MainMenu,
// CharacterSelect, EnterName, MultiplayerLobbyMulti and Leaderboard menus. Kept: the root movie's Update(1, false) and the
// tail (PostLoad, then the drag-and-drop pass for the menus added since).
void MenuManager::LoadMainMenu() {
    GetRoot()->Update(1, false);
    const std::size_t before = m_menus.size();
    PostLoad();
    RegisterDragAndHideFrom(m_menus, before);
}
