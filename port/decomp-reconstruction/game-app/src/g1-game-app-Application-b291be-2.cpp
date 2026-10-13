// Batch g1-game-app-Application-b291be-2 (Application, part 3 of 4).
// Rebuilt from the Hex-Rays pseudocode. Member offsets are the original 32-bit ones, recorded in
// dh2/Application.hpp. Evidence for the globals: the ELF symbol table places dword_99F72C..dword_99F780
// inside Singleton<Application>::s_inst (0x99f72c, 272 bytes), so dword_99F744 is s_inst +0x18 (m_stateMachine),
// dword_99F76C is s_inst +0x40 (m_playerManager), and so on. The member is used in its place.
//
// Most bodies here are PARTIAL: each one's note says which part of the original is not written and why.
#include "dh2/Application.hpp"

#include <list>
#include <new>

#include "core-platform/debug_out.hpp"
#include "core-platform/device.hpp"
#include "core-platform/memory.hpp"
#include "core-platform/perf_counters.hpp"
#include "core-platform/touch_screen_base.hpp"
#include "dh2/DataReloaderManager.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/EventManager.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/GSLevel.hpp"
#include "dh2/IEventReceiver.hpp"
#include "dh2/InputManager.hpp"
#include "dh2/Level.hpp"
#include "dh2/LuaManager.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/MultiMenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/Point2D.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/PlayerStatManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/global/Globals.hpp"
#include "dh2/global/net_singletons.hpp"
#include "net-session-online/c_online.hpp"
#include "net-transport/update_job_thread.hpp"

// The file-scope job threads #2..#4 (m_updateJobThread1..4) and the first-run flag FirstTime are declared in
// dh2/global/Globals.hpp (definitions in game-app/src/Globals.cpp). m_updateJobThread0 is the member at +0x04.
// FirstTime is a bool: the constructor stores it with a byte store (STRB, asm 0x32d980).

namespace {

// Reads a debug switch the way the original does: DebugSwitches::load(), then GetSwitch.
bool ReadDebugSwitch(const char* name) {
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    return switches->GetSwitch(name);
}

// Writes a debug switch the way the original does: DebugSwitches::load(), then SetSwitch.
void WriteDebugSwitch(const char* name, bool value) {
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    switches->SetSwitch(name, value);
}

}  // namespace

// 0x321164 Application::_CheckGamepad()
// PARTIAL: only the no-gamepad exit is written. The rest reads the first connected Gamepad. Its control table is now
// declared (dh2/Gamepad.hpp, Gamepad::m_buttons[k]): the value is Channel::previous (+0x18+0x20k), the minimum
// previous_minimum (+0x20+0x20k), the maximum previous_maximum (+0x24+0x20k) and the pressed latch
// previous_above_threshold (+0x28+0x20k), with threshold (min + max + 1.0) * 0.5. The body is still not written
// because every branch raises an event: EvKeyboard (vtable 0x95b440, forward-declared only in dh2/v2EmuController.hpp)
// and EvGamepad (vtable 0x95b470, no header) are not declared, so the raises cannot be built without a stand-in.
// It also toggles the console when m_console is set and closes the character menu on the menu button. The original
// keeps a function-static last-time value (Application::_CheckGamepad()::last_time, .bss) for the stick auto-repeat.
void Application::_CheckGamepad() {
    InputManager* input = InputManager::GetInstance();
    Gamepad* pad = input->GetFirstConnectedGamepad();
    if (pad == nullptr) {
        return;
    }
    // PARTIAL: the rest of the body is not written, see above.
}

// 0x322924 Application::Pause()
// PARTIAL: the music and sound pause is not written. VoxSoundManager (s_instance, IsMusicPlaying, PauseAllSounds) has
// no declaration in the tree, so musicPlaying is taken as false and the m_resumeMusic latch never sets. m_pauseTime
// (+0xa0) should take the device timer read (IDevice +0x20, virtual getRealTime at slot +0x0c); IDevice declares no
// timer, so m_pauseTime keeps its value. The menu_Ingame lookup result belongs in GSFlashMenu::s_inst +0x0c, which is
// not declared, so the lookup is made and discarded.
// The menu lookups use m_menuManager (dword_99F780 is s_inst +0x54). The save-settings gate is the byte at
// SavegameManager +0x28, which is SavegameManager::m_optionsLoaded (set at the end of a full loadSettings).
void Application::Pause() {
    const bool musicPlaying = false;  // PARTIAL: VoxSoundManager is not declared, see above
    m_resumeMusic = false;            // +0xa6
    m_pauseMenuPushed = false;        // +0xa5

    Level* level = GetCurrentLevel();
    if (level != nullptr && level->m_loadStep == 38 && level->m_isLoaded) {
        level->SG_SavePlayer(0, false);
        if (level->m_isInGameView) {
            if (!MenuBase::s_igmOpened) {
                MenuBase* fadeToBlack = m_menuManager->GetMenuByName("menu_FadeToBlackScreen");
                if (!fadeToBlack->IsVisible()) {
                    m_menuManager->PopMenu("menu_FadeFromBlackScreen");
                    m_pauseMenuPushed = true;
                    // PARTIAL: the result is stored in GSFlashMenu::s_inst +0x0c (not declared).
                    (void)MenuManager::GetInstance()->GetMenuByName("menu_Ingame");
                    m_stateMachine->PushState(&GSFlashMenu::s_inst, false);
                }
            }
        }
        if (musicPlaying && MenuBase::s_igmOpened) {
            m_resumeMusic = true;
        }
    }

    if (m_savegameManager != nullptr && m_savegameManager->m_optionsLoaded) {  // byte +0x28
        m_savegameManager->saveSettings();
    }

    m_paused = true;  // +0xa4
    if (m_stateMachine != nullptr) {
        m_stateMachine->Sleep();
    }
    if (!IsInIGPOrGameCenter()) {
        SetOrientation(m_orientation, false);
    }
}

// 0x322afc Application::InitPS3(glitch::IDevice*)
// PARTIAL: the original first stores 1 to address 0 (LOBYTE(dword_0) = 1). The decompiler left the target
// unnamed and no symbol is known for it, so that store is not written. The menu manager is created as in the
// original (operator new(0x114), then MenuManager::MenuManager).
void Application::InitPS3(glitch::IDevice* device) {
    (void)device;
    m_menuManager = new MenuManager();
}

// 0x323ae0 Application::Init(glitch::IDevice*)
// PARTIAL: the device is stored at +0x10, its reference count is raised and this becomes its event receiver. The
// rest configures the driver (device +0x10 virtual calls at slots +0xa0 and +0xa8 with flag arguments; the driver
// render-state slot +0x0c..) and the collada managers (glitch::collada::CResFileManager::Inst flags at +41 and +43,
// CAnimationStreamingManager::Instance +24 = 512000, then checkMemoryUsage). Those glitch interfaces are not
// declared in the tree, so the tail is not written.
void Application::Init(glitch::IDevice* device) {
    m_device = device;
    // PARTIAL: reference count raise and setEventReceiver are not written; the IDevice header is not included.
}

// 0x323bc4 Application::Quit()
// PARTIAL: written up to the member teardown in the original order. Not written: the InputManager::Destroy and
// VoxSoundManager::DeleteInstance calls (VoxSoundManager statics not declared), Savegame::FlushJobs, the
// CleanGlitch call (partial in game-app part 1), the device release (IDevice vtable slots not declared), the
// PropertyMap::DestroyPropertyMaps call, the underlay node release (UnderlaySceneNode not declared), the
// OnlineSingleton / GCSingleton / COnlineImpl teardown, TrophyManager::DeleteInstance and nativeExit.
// The members whose classes have no header in the tree yet (PyDataArrays, PyDataConstants, AccelerometerBase,
// PhysicalWorld, Console, the unk_1c owner) are not deleted here; their deletes are missing.
// The PlayerManager light-tweaker delete loop covers m_playerLightTweakers[0..3] at +0x58..+0x64.
void Application::Quit() {
    if (m_savegameManager != nullptr) {
        m_savegameManager->saveSettings();
    }

    Level* level = GetCurrentLevel();
    if (level != nullptr && level->m_loadStep == 38) {
        level->QuickSave(false);
        // PARTIAL: the local player (PlayerManager::GetLocalPlayer(0, 1) +0x660 Character*) is not declared here,
        // so Character::SG_Save is not called.
    }

    // PARTIAL: StateMachine::__clear (original call on m_stateMachine, +0x18) is private in dh2/StateMachine.hpp;
    // needs a public accessor (API request). Not called here.
    // PARTIAL: delete m_zoomHandler (+0x50) is not written: dh2/ZoomHandler.hpp pulls glitch-scene-collada-gui
    // event stand-ins that conflict with glitch-shared glitch_core.hpp in this translation unit.
    delete m_objectManager;
    m_objectManager = nullptr;
    delete m_luaManager;
    m_luaManager = nullptr;
    delete m_playerManager;
    m_playerManager = nullptr;
    delete m_stateMachine;
    m_stateMachine = nullptr;
    delete m_eventManager;
    m_eventManager = nullptr;
    delete m_stringManager;
    m_stringManager = nullptr;
    delete m_dataReloaderManager;
    m_dataReloaderManager = nullptr;
    delete m_touchScreen;
    m_touchScreen = nullptr;
    delete m_savegameManager;
    m_savegameManager = nullptr;
    // PARTIAL: the four m_playerLightTweakers (+0x58..+0x64) and m_gamepadTweaker (+0x68) are not deleted:
    // dh2/PlayerLightTweaker.hpp and dh2/GamepadTweaker.hpp need glitch-debugger/CTweakable.hpp, which has no header
    // in the tree yet. The original deletes them here, between savegame and the menu manager.
    // m_gamepadTweaker (+0x68) is not deleted: dh2/GamepadTweaker.hpp needs glitch-debugger/CTweakable.hpp,
    // which has no header in the tree yet.
    delete m_menuManager;
    m_menuManager = nullptr;
}

// 0x3241b8 Application::OnInterruptResume()
// PARTIAL: the original allocates an 8-byte block (CustomAlloc(8)), constructs GS_InterruptLoading in it, and pushes
// its StateBase subobject (block +4) on the state machine (dword_99F744 = Singleton<Application>::s_inst +0x18 =
// m_stateMachine). The push is not written: dh2/GS_InterruptLoading.hpp cannot be included in this translation unit,
// because its glitch-core-io intrusive_ptr header brings a second glitch_core.hpp that conflicts with the one
// core-platform/touch_screen_base.hpp needs here. The state push belongs in a separate translation unit or after
// those two glitch headers are unified.
void Application::OnInterruptResume() {
    _DEBUG_OUT("\n\n Application::OnInterruptResume \n\n");
}

// 0x324210 Application::OnInterruptPause()
// PARTIAL: nativeGetisplaying and nativePauseMusicBG are not declared, so the music pause is not written. The
// original sets Musicplayerpause (extern bool, defined by the GS_InterruptLoading batch) when the music was playing.
void Application::OnInterruptPause() {
    _DEBUG_OUT("\n\n Application::OnInterruptPause \n");
}

// 0x3261fc Application::RemoveUnderlayNode(glitch::scene::ISceneNode*)
// PARTIAL: the original removes the node from the pointer vector of m_underlayNode (the vector's begin is at
// UnderlaySceneNode +0x130 and its end at +0x134; a linear search, then memmove to close the gap). UnderlaySceneNode
// has no header in the tree yet. Layout request: "UnderlaySceneNode +0x130 ISceneNode* vector begin, +0x134 end (4-byte
// pointers, erase keeps order)".
void Application::RemoveUnderlayNode(glitch::scene::ISceneNode* node) {
    if (m_underlayNode == nullptr) {
        return;
    }
    (void)node;
    // PARTIAL: vector search and erase are not written, see above.
}

// 0x3270c8 Application::AddUnderlayNode(glitch::scene::ISceneNode*)
// PARTIAL: the original forwards to UnderlaySceneNode::AddUnderlayNode(m_underlayNode, node) when the underlay exists
// (declared in dh2/UnderlaySceneNode.hpp). That header cannot be included in this translation unit: it brings glitch
// scene types that redefine the glitch_core types of core-platform/deps/glitch_core.hpp. So nothing is forwarded.
// The original returns the callee's R0 (the callee returns void), which the void header does not carry.
void Application::AddUnderlayNode(glitch::scene::ISceneNode* node) {
    if (m_underlayNode == nullptr) {
        return;
    }
    (void)node;
    // PARTIAL: forward to UnderlaySceneNode::AddUnderlayNode, see above.
}

// 0x328f40 Application::ResetTouch()
// Ends every touch that is still down. The touch screen's list of active touch ids (getTouchIDList, vtable slot +0x4c)
// is walked in order, and touchEnded (vtable slot +0x28) is called for each id with the point (-1, -1). The list is then
// freed (the std::list destructor). getTouchIDList is virtual in the original, but core-platform declares it non-virtual;
// no class in the tree overrides it, so the direct call reaches TouchScreenBase::getTouchIDList.
void Application::ResetTouch() {
    if (m_touchScreen == nullptr) {
        return;
    }
    const std::list<long> touchIds = m_touchScreen->getTouchIDList();  // +0x4c
    for (const long slot : touchIds) {
        const Point2D<short> endPoint{-1, -1};
        m_touchScreen->touchEnded(endPoint, slot);  // vtable +0x28
    }
}

// 0x328fe0 Application::Resume()
// PARTIAL: written: the no-device exit, the HUD refresh, the pause flag clear, the state machine wake-up and the
// touch reset. Not written: the sound resume (VoxSoundManager statics not declared, ResumeAllSounds / ResumeMusic
// when the in-game menu is open), the online reconnect check (GetOnline and OnlineSingleton not declared; sets
// m_goToMainMenuPending when the packet clock is past 0x15f90 ms or Wi-Fi is off), the global isDevicePause, and the
// device timer reads that set m_lastTickTime (+0x70) and the pause-time reset.
void Application::Resume() {
    if (m_device == nullptr) {
        return;
    }
    // PARTIAL: HUDControls::hasInstance is called in the original with the Application pointer as its argument; the
    // declared signature is checked in the compile step.
    m_paused = false;
    if (m_stateMachine != nullptr) {
        m_stateMachine->WakeUp();
    }
    ResetTouch();
    // PARTIAL: m_lastTickTime is not written, see above.
    ComputeDt();
}

// 0x329310 Application::UnRegisterForIrrlichtEvents(glitch::IEventReceiver*)
// The original walks m_eventReceivers and frees every node that holds the receiver; std::list::remove does the same.
void Application::UnRegisterForIrrlichtEvents(glitch::IEventReceiver* receiver) {
    if (receiver == nullptr) {
        return;
    }
    m_eventReceivers.remove(receiver);
}

// 0x32a9c8 Application::onEvent(glitch::SEvent const&)
// PARTIAL: written: the dispatch to every registered receiver (IEventReceiver::onEvent, vtable slot +0x08) and the OR of
// their results. Not written: the event-type split. Type 1 (mouse) goes to a switch that raises EvMouseButton (vtable
// 0x95b4b8), EvMouseMove (0x95b488) and EvMouseWheel (0x95b4a0) events; type 2 (keyboard) raises EvKeyboard (0x95b440)
// events, toggles the console on the console key when m_console is set, checks the DebugSwitches
// "IsUsingToggleDisplayMode" and "IsUsingCharacterShowRoom", and steps the view scale at +0x9c by 0.1 on keys 43 and 45.
// These event classes and the SEvent fields have no headers yet. The original passes a decompiler-reused value to
// GSConsole::Toggle; the console object is not identified.
bool Application::onEvent(const glitch::SEvent& event) {
    (void)event;
    // PARTIAL: the receiver dispatch is not written: the glitch::IEventReceiver stand-in in
    // glitch-scene-collada-gui/deps/event_stand_ins.hpp declares no onEvent (vtable slot +0x08 of the original), and
    // including the dh2 receiver header there conflicts with glitch-shared in this translation unit. The event-type
    // branches are not written either.
    return false;
}

// 0x32ade8 Application::_Draw()
// PARTIAL: written: the not-initialised branch, which only draws the state machine. Not written: the 3D pass (driver
// and scene-manager virtual calls, camera selection, glitch bounding-box debug draw), the 2D pass (StateMachine::Draw2D,
// MenuManager::DBG_Draw, PerfCounters::Draw) and the driver begin and end calls. The glitch video and scene interfaces
// are not declared. The original's second condition, the InterLoad global, is not declared either, so this branch is
// taken whenever IsInitFinish is false (IsInitFinish is itself partial).
void Application::_Draw() {
    if (!IsInitFinish()) {
        if (m_stateMachine != nullptr) {
            m_stateMachine->Draw();
        }
        return;
    }
    // PARTIAL: the drawing branch is not written, see above.
}

// 0x32bdc8 Application::LoadLevel(...)
// PARTIAL. Written: the unk_ab (+0xab) reset and the statub bar hide; then the original's two paths.
//   A running level (its m_loadStep must be 38, else the function returns): when the level is not in its game view,
//   MultiMenuManager::PopMenu(true) runs (the m_menuManager +0xf4 object, vtable +0x38). Then m_inTransition (+0x145)
//   is set, SendGLHiScore runs, m_levelChange (+0xf0) is set, SG_SaveLocalPlayer(true), QuickSave(true) and ResetIsLoaded
//   run. The switch argument fromSubId is the level's m_levelListSubId (+0x40), read as a full word.
//   No level: the player manager's unk_71b (+0x71b) is set, and fromSubId is -1.
// Then the switch: GSLevel::LoadLevel with the arguments, PlayerStatManager::s_inst.Reset(), PlayerManager unk_6c9 = 0
// and PlayerManager::Update.
// Not written: (a) the savegame setup, which the original runs before the switch when isRemotelyTriggered is false.
// It needs the PlayerSavegame difficulty seed, DebugSwitches "DontUsePlayerSeed", the device timer for the seed
// (IDevice declares no timer) and the online start-game message (CMessaging and CMatching are not declared). The
// switch below takes the caller's seed and syncedSeed in that case. (b) The GameCenter flag (GCSingleton<GameCenter>
// +0x14, not declared) and the temp_* staging globals (not declared).
void Application::LoadLevel(const char* name, int entryPointId, uint32_t savegameSlot, bool resume, bool useSpawnPoint,
                            int difficulty, bool isRemotelyTriggered, uint32_t seed, uint32_t syncedSeed) {
    // PARTIAL: the GameCenter flag and the temp_* staging globals are not written, see above.
    unk_ab = false;
    ShowStatubBar(false);

    int fromSubId = -1;
    Level* level = GetCurrentLevel();
    if (level != nullptr) {
        if (level->m_loadStep != 38) {
            return;
        }
        if (!level->m_isInGameView) {
            m_menuManager->m_multiMenuManager->PopMenu(true);  // +0xf4 MultiMenuManager, vtable +0x38
        }
        level->m_inTransition = true;                 // +0x145
        SendGLHiScore();
        fromSubId = level->m_levelListSubId;          // +0x40
        level->m_levelChange = true;                  // +0xf0
        level->SG_SaveLocalPlayer(true);
        level->QuickSave(true);
        level->ResetIsLoaded();
    } else {
        m_playerManager->unk_71b = true;  // +0x71b
    }

    // PARTIAL: when isRemotelyTriggered is false the original runs the savegame setup here, see above. It is not written.
    (void)isRemotelyTriggered;

    GSLevel::LoadLevel(name, entryPointId, savegameSlot, seed, syncedSeed, resume, useSpawnPoint, fromSubId, difficulty);
    PlayerStatManager::s_inst.Reset();
    m_playerManager->unk_6c9 = 0;  // +0x6c9
    m_playerManager->Update();
}

// 0x32c1f4 Application::GoToMainMenu(GoToMainMenuEvent)
// PARTIAL: written: the level save (the else branch calls MultiMenuManager::PopMenu(true) at vtable +0x38, then
// m_inTransition (+0x145) is set and the level quick-saved), the menu reset, the switch to the main menu state, the
// hi-score send, the TrophyManager unlock, the removal of all players, the statub bar, and the event latch (unk_ab,
// m_goToMainMenuEvent). Not written: the two multiplayer globals (Is_In_Multiplayer_IGM, Is_In_Multiplayer_Mode, not
// declared, cleared first in the original); the GSFlashMenu field writes (+0x0c menu pointer, +0x29 flag: the menu lookup
// is made and discarded); and the online sign-out block, gated on the online byte (CSignIn, CMatching and
// GCSingleton<GameCenter> are not declared; COnline::SetIsOnlineGame and the OnlineSingleton state reset are declared
// but sit behind that block).
void Application::GoToMainMenu(GoToMainMenuEvent event) {
    Level* level = GetCurrentLevel();
    if (level != nullptr && level->m_loadStep == 38) {
        if (!level->m_isInGameView) {
            m_menuManager->m_multiMenuManager->PopMenu(true);  // +0xf4 MultiMenuManager, vtable +0x38
        }
        level->m_inTransition = true;  // +0x145
        level->QuickSave(false);
        level->SG_SaveLocalPlayer(false);
    }

    MenuManager* menus = MenuManager::GetInstance();
    menus->Reset();
    _DEBUG_OUT("*************************GoToMainMenu::1111111");
    MenuBase* mainMenu = menus->GetMenuByName("menu_MainMenu");
    // PARTIAL: mainMenu belongs in GSFlashMenu::s_inst +0x0c, and the flag at +0x29 is set to 1; not declared.
    (void)mainMenu;
    m_stateMachine->SwitchState(&GSFlashMenu::s_inst, false);
    _DEBUG_OUT("*************************GoToMainMenu::222222");
    SendGLHiScore();
    TrophyManager::s_instance->UnlockTrophiesGLLive();
    _DEBUG_OUT("*************************GoToMainMenu::3333333");
    if (m_playerManager != nullptr) {
        m_playerManager->RemoveAllPlayers();
    }
    // PARTIAL: the online sign-out block is not written, see above.
    _DEBUG_OUT("*************************GoToMainMenu::4444444444");
    ShowStatubBar(false);
    unk_ab = true;
    m_goToMainMenuEvent = event;
    _DEBUG_OUT("*************************GoToMainMenu::55555555555555");
}

// 0x32c438 Application::_Update(int)
// PARTIAL: written: the UseClickToMove switch clear, the pending connection-loss main-menu switch, the event manager
// and state machine updates, the touch update, the gamepad poll, the one-shot object dump and the quest dumps, and
// the frame counter. Not written: CXPlayerManager::Update (singleton not declared), the cheat switches block (the
// handle_cheats_inGame and lastOpenMenuID globals, SavegameManager::toggleOption), the scene-manager update of the
// root scene node and the IsLevelPaused / IsLevelLoaded gate around it, the update-job thread starts (Savegame::UpdateJobs),
// CharAI::StepResetAvailableStep, PlayerManager::Update, VoxSoundManager::UpdateIPodAsync, the accelerometer and the
// InputManager update (virtual slot +0x30 on the input manager), and the one-shot glitch traces (TraceAllLoaded*,
// TraceGlitchHierarchyOnce, TraceVisibleNodeOnce), which call glitch debug functions that are not declared. The
// DumpActiveQuestInformationOnce trace needs PyDataConstants::getConstant, also not declared; its switch is cleared and
// the trace skipped.
void Application::_Update(int dtMs) {
    // PARTIAL: CXPlayerManager::Update is not written, see above.
    WriteDebugSwitch("UseClickToMove", false);
    if (m_goToMainMenuPending) {
        m_goToMainMenuPending = false;
        GoToMainMenu(static_cast<GoToMainMenuEvent>(3));
    }
    // PARTIAL: the cheat block and the scene update are not written, see above.

    // PARTIAL: CharAI::StepResetAvailableStep and PlayerManager::Update are not written, see above.
    if (m_eventManager != nullptr) {
        m_eventManager->Update(dtMs);
    }
    if (m_stateMachine != nullptr) {
        m_stateMachine->Update(dtMs);
    }
    // PARTIAL: VoxSoundManager::UpdateIPodAsync, the accelerometer and the input manager update are not written.
    if (m_touchScreen != nullptr) {
        m_touchScreen->update(dtMs);
    }
    _CheckGamepad();

    // PARTIAL: TraceAllLoadedResfileInfoOnce, TraceAllLoadedTextureInfoOnce, TraceGlitchHierarchyOnce and
    // TraceVisibleNodeOnce are not written, see above.
    if (ReadDebugSwitch("TraceRoomObjects")) {
        m_objectManager->DBG_DumpRoomObjects();
        WriteDebugSwitch("TraceRoomObjects", false);
    }
    if (ReadDebugSwitch("DumpQuestInformationOnce")) {
        WriteDebugSwitch("DumpQuestInformationOnce", false);
        Level* level = GetCurrentLevel();
        if (level != nullptr) {
            level->SG_DBG_TraceDetailedQuestInformation(-1);
        }
    }
    if (ReadDebugSwitch("DumpActiveQuestInformationOnce")) {
        WriteDebugSwitch("DumpActiveQuestInformationOnce", false);
        // PARTIAL: the trace needs PyDataConstants::getConstant("v2QuestState", "Active"), not declared.
    }

    ++m_frameCount;
}

// 0x32ccc4 Application::Update()
// PARTIAL: the device-timer steps are not written. The original reads the device timer (IDevice +0x20 object, virtual
// getRealTime at slot +0x0c). It returns through ComputeDt when more than 2000 ms have passed since m_lastTickTime (+0x70),
// runs the DataReloaderManager::checkFiles timer (4000 ms, the function static reloadtimer in .bss) and stores the timer
// into m_lastTickTime. IDevice declares no timer, so that gate, the reload timer and the store are missing, and every
// frame runs the steps below. Also not written: the online update block (COnlineImpl::Update is not declared, and the
// byte that gates the block is private in COnline, see IsOnlineGame). The frame pacing calls the device's vtable slot
// +0x10 with no argument, which IDevice does not declare; those calls are marked below.
void Application::Update() {
    if (m_paused) {
        return;
    }
    if (IsOnlineGame()) {  // *(GetOnline() + 5)
        GetOnline()->ReceivePackets();
    }
    unk_7a = false;
    // PARTIAL: the device-timer steps and the online update block, see above.
    if (m_touchScreen != nullptr) {
        m_touchScreen->ProcessEvents();
    }
    ComputeDt();
    // The original converts +0x8c as an unsigned int for both the FPS value and the counter update.
    const float frameTimeMs = static_cast<float>(static_cast<unsigned int>(m_dt));
    const float fps = 1000.0f / frameTimeMs;
    Singleton<PerfCounters>::s_inst.Update(frameTimeMs);
    _Update(m_dt);
    _Draw();
    Singleton<PerfCounters>::s_inst.SetCounterValue("FPS", fps, 0.0f, 60.0f);
    if (IsOnlineGame()) {
        GetOnline()->SendPackets();
    }

    // Frame pacing, high-performance devices only. The three averages are function statics in the original (.bss, zero).
    if (!Device::IsHighPerformance()) {
        return;
    }
    static int avgFrameRate = 0;
    static int avgFrameCount = 0;
    static int avgSleepTime = 0;
    ++avgFrameCount;
    avgFrameRate += m_dt;
    if (avgFrameCount == 10) {
        const int v13 = avgFrameRate / 10 - avgSleepTime;
        int v14 = 0;
        if (v13 > 15) {
            if (v13 > 32) {
                v14 = v13 > 49 ? 5 : 50 - v13;
            } else {
                v14 = 33 - v13;
            }
        } else {
            v14 = 16 - v13;
        }
        avgFrameRate = 0;
        avgFrameCount = 0;
        avgSleepTime = v14 > 4 ? v14 : 5;
        // PARTIAL: the original calls the device's vtable slot +0x10 here (no argument), not declared in IDevice.
        return;
    }
    // PARTIAL: when avgSleepTime is above 0 the original calls the same device slot here, not declared in IDevice.
}

// 0x32d79c Application::Application()
// 0x32d980 Application::Application()
// The two addresses are the complete-object and base-object variants of one constructor; one definition serves both.
// Every store of the original is written, in its order: the vptr is the compiler's, FirstTime is the bool of
// dh2/global/Globals.hpp (STRB), and the job threads are created in the order of the original (CustomAlloc(0x10), then
// updateJob_thread(n), then the pointer store). The std::string members (+0xbc, +0xd4) are constructed empty. The original
// allocates a 16-byte buffer for each and writes a terminating zero, which is the same empty string. The std::list
// sentinel at +0x08 is the default construction of m_eventReceivers. Not set, as in the original: m_menuManager (+0x54),
// the tweakers (+0x58..+0x68), m_lastTickTime (+0x70), m_orientation (+0x80), m_dt..m_timeScale (+0x8c..+0x98),
// m_underlayNode (+0xb8), unk_ec (+0xec) and the pad bytes. They are zero, because Application is constructed in the
// zeroed .bss storage of Singleton<Application>::s_inst.
Application::Application() {
    unk_6c = 30;
    m_dtFactor = 1.0f;
    m_pauseTime = -1;
    m_lastTime = 0;
    m_frameCount = 0;

    // Pointer members +0x10..+0x50 are cleared; m_device is the one at +0x10.
    m_device = nullptr;
    m_eventManager = nullptr;
    m_stateMachine = nullptr;
    unk_1c = nullptr;
    m_touchScreen = nullptr;
    m_accelerometer = nullptr;
    m_dataReloaderManager = nullptr;
    m_pyDataConstants = nullptr;
    m_pyDataArrays = nullptr;
    m_stringManager = nullptr;
    m_objectManager = nullptr;
    m_luaManager = nullptr;
    m_playerManager = nullptr;
    m_physicalWorld = nullptr;
    m_console = nullptr;
    m_savegameManager = nullptr;
    m_zoomHandler = nullptr;

    unk_78 = false;
    unk_79 = false;
    unk_7a = false;
    m_paused = false;
    m_pauseMenuPushed = false;
    m_resumeMusic = false;
    unk_a7 = false;
    unk_a8 = false;
    m_sceneUpdated = false;
    m_goToMainMenuPending = false;
    unk_ab = true;
    unk_ad = false;
    unk_b4 = true;
    unk_ed = false;
    unk_ee = false;
    unk_10e = false;
    unk_10f = true;

    // The std::string members (+0xbc m_uncompiledData, +0xd4 unk_d4) are constructed empty by the compiler.

    FirstTime = 1;
    m_updateJobThread0 = new (CustomAlloc(0x10)) updateJob_thread(1);
    m_updateJobThread1 = new (CustomAlloc(0x10)) updateJob_thread(2);
    m_updateJobThread2 = new (CustomAlloc(0x10)) updateJob_thread(3);
    m_updateJobThread3 = new (CustomAlloc(0x10)) updateJob_thread(4);
    m_updateJobThread4 = new (CustomAlloc(0x10)) updateJob_thread(5);
}
