// Batch g1-game-app-Application-b291be-2 (Application, part 3 of 4).
// Rebuilt from the Hex-Rays pseudocode. Member offsets are the original 32-bit ones, recorded in
// dh2/Application.hpp. Evidence for the globals: the ELF symbol table places dword_99F72C..dword_99F780
// inside Singleton<Application>::s_inst (0x99f72c, 272 bytes), so dword_99F744 is s_inst +0x18 (m_stateMachine),
// dword_99F76C is s_inst +0x40 (m_playerManager), and so on. The member is used in its place.
//
// Most bodies here are PARTIAL: each one's note says which part of the original is not written and why.
#include "dh2/Application.hpp"

#include <new>

#include "core-platform/debug_out.hpp"
#include "core-platform/memory.hpp"
#include "core-platform/perf_counters.hpp"
#include "core-platform/touch_screen_base.hpp"
#include "dh2/DataReloaderManager.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/EventManager.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/IEventReceiver.hpp"
#include "dh2/InputManager.hpp"
#include "dh2/Level.hpp"
#include "dh2/LuaManager.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/StringManager.hpp"
#include "net-transport/update_job_thread.hpp"

// File-scope job threads #2..#4 of the original (m_updateJobThread0 is the member at +0x04). Their
// definitions belong to the globals batch; the constructor below stores into them.
extern updateJob_thread* m_updateJobThread1;
extern updateJob_thread* m_updateJobThread2;
extern updateJob_thread* m_updateJobThread3;
extern updateJob_thread* m_updateJobThread4;
// ELF .bss global set to 1 by the constructor. Its width is not established (the store is 32-bit).
extern int FirstTime;

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
// PARTIAL: only the no-gamepad exit is written. The rest reads the first connected Gamepad: about 40 controls,
// each a value float, a minimum and a maximum (the threshold is (min + max + 1.0) * 0.5) and a latch byte at
// the control's byte offset +16 (the latch is written by the event consumer, not here). It raises EvGamepad
// events (vtable 0x95b470: type 7, action id, pressed flag, player 0, 1.0f) and EvKeyboard events (vtable
// 0x95b440) on m_eventManager, toggles the console when the console button is pressed (m_console non-null), and
// closes the character menu on the menu button. Waits on a Gamepad layout header (dh2/Gamepad.hpp is not declared)
// and on the EvGamepad and EvKeyboard event headers (dh2/EvGamepad.hpp, dh2/EvKeyboard.hpp are not declared).
// The original keeps a function-static last-time value for the auto-repeat of the stick buttons.
void Application::_CheckGamepad() {
    InputManager* input = InputManager::GetInstance();
    Gamepad* pad = input->GetFirstConnectedGamepad();
    if (pad == nullptr) {
        return;
    }
    // PARTIAL: the rest of the body is not written, see above.
}

// 0x322924 Application::Pause()
// PARTIAL: VoxSoundManager::s_instance, IsMusicPlaying and PauseAllSounds are not declared, so no sound is paused
// and musicPlaying is taken as false (the m_resumeMusic latch stays clear). m_pauseTime (+0xa0) should take the
// device time from the IDevice timer (device +0x10, timer at device +0x20, virtual getRealTime at slot +0x0c);
// the timer interface is not declared, so m_pauseTime keeps its value. The menu_Ingame lookup result belongs in
// GSFlashMenu::s_inst +0x0c (not declared) and is only looked up here. The SavegameManager::saveSettings call is
// gated on a byte at SavegameManager +0x28 whose name is not established, so it is not written.
void Application::Pause() {
    const bool musicPlaying = false;  // PARTIAL, see above
    m_resumeMusic = false;

    m_pauseMenuPushed = false;

    Level* level = GetCurrentLevel();
    if (level != nullptr && level->m_loadStep == 38 && level->m_isLoaded) {
        level->SG_SavePlayer(0, false);
        if (level->m_isInGameView) {
            if (!MenuBase::s_igmOpened) {
                MenuBase* fadeToBlack = MenuManager::GetInstance()->GetMenuByName("menu_FadeToBlackScreen");
                if (!fadeToBlack->IsVisible()) {
                    MenuManager::GetInstance()->PopMenu("menu_FadeFromBlackScreen");
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

    m_paused = true;
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
// PARTIAL: the original forwards to UnderlaySceneNode::AddUnderlayNode(m_underlayNode, node) when the underlay exists.
// That member function is not declared (UnderlaySceneNode has no header), so nothing is forwarded.
void Application::AddUnderlayNode(glitch::scene::ISceneNode* node) {
    if (m_underlayNode == nullptr) {
        return;
    }
    (void)node;
    // PARTIAL: forward to UnderlaySceneNode::AddUnderlayNode, see above.
}

// 0x328f40 Application::ResetTouch()
// PARTIAL: the original asks m_touchScreen (vtable slot +0x4c) for the list of touch slots that are still down,
// calls touchEnded(Point2D(-1, -1), slot) for each entry (vtable slot +0x28), then frees the list nodes. The slot
// +0x4c accessor is not declared in core-platform/touch_screen_base.hpp, so no touch is ended here.
void Application::ResetTouch() {
    if (m_touchScreen == nullptr) {
        return;
    }
    // PARTIAL: see above.
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
// PARTIAL: written: the unk_ab (+0xab) reset and the statub bar hide. Not written: the GameCenter flag, the temp_* staging
// globals, the savegame and seed setup (PlayerSavegame difficulty slot, DebugSwitches "DontUsePlayerSeed", timer seed),
// the online start-game message (CMessaging / CMatching and Arrays::LevelList), the save of the finished level
// (Level::SG_SaveLocalPlayer, Level::QuickSave, Level::ResetIsLoaded, and the GSLevel::LoadLevel switch), and the
// PlayerStatManager reset and PlayerManager::Update tail. None of those types or globals is declared in the tree yet.
void Application::LoadLevel(const char* name, int entryPointId, uint32_t savegameSlot, bool resume, bool useSpawnPoint,
                            int difficulty, bool isRemotelyTriggered, uint32_t seed, uint32_t syncedSeed) {
    unk_ab = false;
    Level* level = GetCurrentLevel();
    ShowStatubBar(false);
    (void)level;
    (void)name;
    (void)entryPointId;
    (void)savegameSlot;
    (void)resume;
    (void)useSpawnPoint;
    (void)difficulty;
    (void)isRemotelyTriggered;
    (void)seed;
    (void)syncedSeed;
    // PARTIAL: the rest of the body is not written, see above.
}

// 0x32c1f4 Application::GoToMainMenu(GoToMainMenuEvent)
// PARTIAL: written: the level save (with m_inTransition set, +0x145), the menu reset, the switch to the main menu
// state, the hi-score send, the removal of all players, the statub bar, and the event latch (unk_ab, m_goToMainMenuEvent).
// Not written: the else branch of the level save makes a virtual call on the MenuManager at +0xf4 with argument 1
// (its slot is not declared); the two multiplayer globals (Is_In_Multiplayer_IGM, Is_In_Multiplayer_Mode) are not
// declared; the GSFlashMenu field writes (+0x0c menu pointer, +0x29 flag) are not declared; TrophyManager::
// UnlockTrophiesGLLive is not declared; and the online sign-out block (CSignIn, CMatching, COnline, OnlineSingleton,
// GCSingleton) is not declared.
void Application::GoToMainMenu(GoToMainMenuEvent event) {
    Level* level = GetCurrentLevel();
    if (level != nullptr && level->m_loadStep == 38) {
        // PARTIAL: the else branch of the original (virtual call on MenuManager +0xf4) is not written; both branches
        // below set m_inTransition and quick-save the level.
        level->m_inTransition = true;
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
    // PARTIAL: TrophyManager::UnlockTrophiesGLLive is not declared, not called.
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
// PARTIAL: written: the paused exit, the unk_7a clear, the touch event processing, the delta update, the update and
// the draw. Not written: the online packet receive and the online update block (GetOnline, COnline, COnlineImpl and
// OnlineSingleton are not declared), the device tick gate that returns early when the frame time exceeds 2000 ms
// (device timer not declared), the reload timer for DataReloaderManager::checkFiles (its 4000 ms period is kept in a
// function static), the PerfCounters FPS update (Singleton<PerfCounters> not declared), the online send, and the frame
// pacing based on Device::IsHighPerformance (static averages in function statics, not written).
void Application::Update() {
    if (m_paused) {
        return;
    }
    unk_7a = false;
    // PARTIAL: the online and device-timer steps are not written, see above.
    if (m_touchScreen != nullptr) {
        m_touchScreen->ProcessEvents();
    }
    ComputeDt();
    _Update(m_dt);
    _Draw();
}

// 0x32d79c Application::Application()
// 0x32d980 Application::Application()
// The two addresses are the complete-object and base-object variants of one constructor; one definition serves both.
// PARTIAL: the member stores from the constructor are written. Not written: the FirstTime global (its width is not
// established in the tree), and the job threads' pointer stores are declared extern above (their definitions belong to
// the globals batch). The std::string members (+0xbc, +0xd4) are empty by default construction, which is what the
// original's zero stores produce. The members at +0x54 (m_menuManager) and +0x58..+0x68 (tweakers) are not set here,
// as in the original.
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
