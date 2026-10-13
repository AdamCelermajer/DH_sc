// Batch g1-game-world-Level-7c7f5d-3: 11 Level entries (part 4 of 4): the load-step helpers _LoadLightSet, _FlushMeshes,
// _LoadBatchMap and _LoadProcess, the per-frame Update, the destructor (D1 and D2 share one body, D0 is skipped), the co-op
// camera functions CanMoveTowards and UpdateCameraZoom, and GetRimLightFile. Declarations come from dh2/Level.hpp.
// A PARTIAL body names what it waits on. A "SEAM" comment marks one step of the original that is left out, with its owner.
//
// The COnline online flag (COnline::isOnlineGame_ at +0x05) is protected and has no accessor, so every test on it is a SEAM.
// PFWorld and HUDControls cannot share this translation unit with core-platform/memory.hpp (the glitch stand-ins conflict),
// so their calls are SEAMs too.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "core-platform/device.hpp"               // Device::IsFixedPipeline (static)
#include "core-platform/memory.hpp"              // gAssertLevel
#include "dh2/global/runtime_support.hpp"        // PushProfilingContext, PopProfilingContext (before Character.hpp: see below)
#include "dh2/Application.hpp"
// Character.hpp renames MemoryHintState (net-transport stand-in), so it follows runtime_support.hpp and comes before the rest.
#include "dh2/Character.hpp"
#include "dh2/CameraBase.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/AnimSetManager.hpp"
#include "dh2/CameraOverview.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameEventManager.hpp"
#include "dh2/InfoHUDManager.hpp"
#include "dh2/Level.hpp"
#include "dh2/LevelConfig.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/LuaManager.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuDebug.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Random.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/SpawnGroupManager.hpp"
#include "dh2/VisualFXManager.hpp"
#include "dh2/batch/BatchNodeCompiler.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"      // dword_99F76C, the PlayerManager instance
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764, the ObjectManager instance
#include "dh2/global/ctrl_camera.hpp"            // ApplicationGetInstance()
#include "dh2/global/runtime_support.hpp"        // PushProfilingContext, PopProfilingContext (included above)
#include "dh2/global/net_singletons.hpp"         // GetOnlineGameFlag (COnline +0x05)
#include "pydata-accessors/arrays.hpp"           // Arrays::DesignSettingsTable
#include "pydata-accessors/structs_records.hpp"  // Structs::DesignSettings

namespace {

// Source file named in the original's assert messages for the m_levelConfig check (Level.h, line 476).
constexpr const char* kLevelHeaderFile =
    "..\\..\\project_vs2005\\Game/..\\..\\sources/Game/Level/Level.h";

// The original ASSERT: level 2 faults (it stores to address 0), level 1 prints to stderr. std::abort() ends the process
// the way the level-2 fault does.
void LevelAssertFailed(const char* expression, const char* file, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, file, line);
    }
}

// The m_levelConfig != 0 check that the original inlines in front of each read of the config. The original goes on to
// dereference null, which faults; here the process ends after the assert.
LevelConfig* RequireLevelConfig(const Level* level) {
    if (!level->m_levelConfig) {
        LevelAssertFailed("m_levelConfig != 0", kLevelHeaderFile, 476);
        std::abort();
    }
    return level->m_levelConfig;
}

// DebugSwitches::load() on the singleton, then GetSwitch(name). The original addresses DebugSwitches::s_inst directly,
// which is private in dh2/DebugSwitches.hpp; GetInstance() is the same singleton.
bool ReadDebugSwitch(const char* name) {
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    return switches->GetSwitch(name);
}

// The discarded "isTracingLevel_Loading" read that most _LoadProcess arms make before their work. GetInstance() then
// GetSwitch(), with no load() and no use of the result, as in the original.
void TraceLoadingStep() {
    DebugSwitches::GetInstance()->GetSwitch("isTracingLevel_Loading");
}

}  // namespace

// 0x3f459c Level::_LoadLightSet()
// Loads the light-set file named by the level config, retrying until LoadFile reports a result. The fixed-pipeline build
// reads m_fixedLightSet, the programmable build reads m_lightSet. The original's "data != finish" test on each string is
// !empty(). The string offsets it reads are the header's members (+0x2b8 and +0x2d0) at STLport's +0x10 and +0x14.
void Level::_LoadLightSet() {
    if (Device::IsFixedPipeline()) {
        LevelConfig* config = RequireLevelConfig(this);
        if (!config->m_fixedLightSet.empty()) {
            int loaded = 0;
            do {
                config = RequireLevelConfig(this);
                const std::string file(config->m_fixedLightSet.c_str());
                loaded = LoadFile(file, "Module");
            } while (!loaded);
        }
    } else {
        LevelConfig* config = RequireLevelConfig(this);
        if (!config->m_lightSet.empty()) {
            int loaded = 0;
            do {
                config = RequireLevelConfig(this);
                const std::string file(config->m_lightSet.c_str());
                loaded = LoadFile(file, "Module");
            } while (!loaded);
        }
    }
}

// 0x3f55a4 Level::_FlushMeshes()
// PARTIAL: the walk is not reconstructed, so the function does nothing yet. For each node of ObjectManager::m_objectMap
// (+0x0c) whose ObjectListItem flag (node +0x2c) is set, the original takes the object handle, converts it to a
// GameObject and, when the GameObject's m_visualObject (+0x2d8) is set, calls VisualObject::FlushMeshes and collects the
// returned name into a std::set. It then releases the removable buffer of the glitch collada CResFile of each name
// (CResFileManager::Inst->get(name, 0) and releaseRemovableBuffer(1)). Waits on dh2/ObjectListItem.hpp (not written: the
// flag, the handle and the object pointer are not declared) and on the CResFileManager and CResFile accessors, which the
// glitch-scene-collada-gui headers do not declare.
void Level::_FlushMeshes() {
}

// 0x3f5ea0 Level::_LoadBatchMap()
// PARTIAL: the IsDisablingCompiledBatching gate is reconstructed. With the gate clear and a batch compiler present, the
// original walks the GameObject* list at BatchNodeCompiler +0x10 .. +0x14. For each object whose m_visualObject (+0x2d8)
// has a scene node at VisualObject +0x08, it calls BatchNodeCompiler::_MapMeshNode(object, node). The walk is left out.
// dh2/batch/BatchNodeCompiler.hpp declares the list as m_gameObjects (+0x10), so the list is no longer the blocker. The
// blocker is VisualObject::m_node, which is a RootSceneNode* and needs a conversion to glitch::scene::ISceneNode* before
// _MapMeshNode.
void Level::_LoadBatchMap() {
    if (ReadDebugSwitch("IsDisablingCompiledBatching")) {
        return;
    }
    if (!m_batchNodeCompiler) {
        return;
    }
    // SEAM: the object walk over m_gameObjects (BatchNodeCompiler +0x10) and _MapMeshNode. The VisualObject scene node
    // (m_node, a RootSceneNode*) needs a conversion to glitch::scene::ISceneNode* first.
}

// 0x3f6990 Level::_LoadProcess()
// PARTIAL. One loading step per call. m_loadStep (+0x130) selects the arm, and 38 means loaded. The arms follow the switch
// of the original, with the case labels in their original order. An arm that calls a declared Level or manager method does
// so. Any other call is a SEAM comment that names its owner. The discarded "isTracingLevel_Loading" reads are kept.
//
// Known gaps beyond the SEAM comments:
//   - The cheat block at the start (handle_cheats_inGame) and the step-name display (DBG_GetLoadingStepName, _DEBUG_OUT)
//     are left out.
//   - Arm 0x24 has no step increment on the offline path in the original (the BEQ over the online block goes straight to
//     the common tail, and the assembly agrees). Step 0x24 is the progress-100 state, so something outside this function
//     advances it. This is reproduced as written and flagged for the integrator.
//   - The native license check at the end (USE_NATIVE_DRM_GAME) is not reconstructed.
void Level::_LoadProcess() {
    switch (m_loadStep) {
        case 0:
            // SEAM: g_bigI = 0; g_bigV = 0 (undeclared globals).
            ApplicationGetInstance().CleanGlitch();
            // SEAM: ShowMemoryStats("Before Level Loading") (not declared).
            // SEAM: VoxSoundManager::StopAllSounds(500) (dh2 has no VoxSoundManager header); byte_99F7E0 = 1 (undeclared).
            m_loadWorkDone = 0;
            m_loadWorkTotal = 0;
            m_loadFileCount = 0;
            m_isLoaded = false;
            ++m_loadStep;
            break;

        case 2:
            TraceLoadingStep();
            MenuManager::GetInstance()->UnloadMenu(2);
            MenuManager::GetInstance()->UnloadMenu(1);
            MenuManager::GetInstance()->UnloadMenu(3);
            ++m_loadStep;
            break;

        case 3:
            TraceLoadingStep();
            ++m_loadStep;
            break;

        case 4:
            TraceLoadingStep();
            // SEAM: VisualFXManager::BuildLibraries on the Singleton<VisualFXManager>::s_inst (Singleton is not declared).
            ++m_loadStep;
            break;

        case 5:
            TraceLoadingStep();
            // SEAM: PhysicalWorld::load(-2000, -2000, 2000, 2000) on dword_99F770 (undeclared global).
            ++m_loadStep;
            break;

        case 6:
            TraceLoadingStep();
            dword_99F76C->unk_6c9 = 1;
            dword_99F76C->Update();
            m_gameEventManager = new GameEventManager();
            m_gameEventManager->Load();
            ++m_loadStep;
            break;

        case 7: {
            Random::s_seed = m_seed;
            Random::s_syncedSeed = m_syncedSeed;
            if (m_isRandomLevel) {
                // SEAM: the random-level branch. The seed is the synced one when GetOnline()->isOnlineGame_ (no accessor),
                // GenerateRandomLevel fills a StreamBuffer and AssignSteamToLoadDataFile wraps it on success. On failure the
                // original writes 'x' to the first character of m_levelName and sets it to the part before the first '.'
                // followed by "_BACKUP.mlx". GenerateRandomLevel returns 0 in this tree (see its batch notes).
            }
            TraceLoadingStep();
            m_loadWorkTotal = 500;
            int loaded = 0;
            do {
                loaded = LoadFile(m_levelName, "Level");
            } while (!loaded);
            ++m_loadStep;
            ++m_loadFileCount;
            break;
        }

        case 8: {
            TraceLoadingStep();
            // SEAM: when m_levelConfig is null, the LevelConfig.xml load from the dword_99F7F8, dword_99F7FC and dword_99F814
            // globals (undeclared) and ObjectManager::Spawn("LevelConfig") with SetLevelConfig. The Spawn signature in the
            // header does not match the call.
            // The original reads this+0x38 here with no assert, so a null config is not an error at this step. The fallback
            // above is a SEAM, so a null config stays null and the fog-colour test is skipped instead of aborting.
            LevelConfig* config = m_levelConfig;
            if (config && !config->m_dfogColors.empty()) {
                // SEAM: ObjectManager::InitModulesFogColor(dword_99F764, m_dfogColors, m_dfogChangeMinDist, &Application).
                // The header declares two arguments, the original passes four.
            }
            _LoadScripts();
            ++m_loadStep;
            break;
        }

        case 9:
            TraceLoadingStep();
            _LoadLightSet();
            ++m_loadStep;
            break;

        case 10:
            TraceLoadingStep();
            m_loadWorkTotal = static_cast<int>(dword_99F764->m_objectMap.size());
            // The original repeats InitPost until it returns non-zero. The header types InitPost void, so the first call
            // is made and the repeat is left out.
            dword_99F764->InitPost();
            ++m_loadStep;
            break;

        case 11:
            TraceLoadingStep();
            // SEAM: Singleton<PFWorld>::s_inst.PostLoad() (the singleton is not declared).
            ++m_loadStep;
            break;

        case 12:
            MenuManager::GetInstance()->LoadMenu(1);
            ++m_loadStep;
            break;

        case 13:
            TraceLoadingStep();
            ScriptManager::s_inst.InitCommands();
            ++m_loadStep;
            break;

        case 14:
            TraceLoadingStep();
            _LoadPlayer();
            // SEAM: when GetOnline()->isOnlineGame_ (no accessor) and the local player has no character: CMatching::Get(),
            // DisconnectAll on the connection manager and GoToMainMenu(3).
            ++m_loadStep;
            break;

        case 15:
            TraceLoadingStep();
            // SEAM: VoxSoundManager::SetLevelRouting(VoxSoundManager::s_instance, m_levelListIndex) (no header).
            ++m_loadStep;
            break;

        case 16:
            TraceLoadingStep();
            ++m_loadStep;
            break;

        case 17:
            TraceLoadingStep();
            _LoadFinalInit();
            ++m_loadStep;
            break;

        case 18:
            TraceLoadingStep();
            _LoadCharStates();
            ++m_loadStep;
            break;

        case 19:
            TraceLoadingStep();
            ++m_loadStep;
            break;

        case 20:
            TraceLoadingStep();
            _LoadCamera();
            ++m_loadStep;
            break;

        case 21:
            TraceLoadingStep();
            _LoadBatchInit();
            ++m_loadStep;
            break;

        case 22:
            TraceLoadingStep();
            _LoadBatchList();
            ++m_loadStep;
            break;

        case 23:
            TraceLoadingStep();
            _LoadBatchMap();
            ++m_loadStep;
            break;

        case 24:
            TraceLoadingStep();
            _LoadBatching();
            ++m_loadStep;
            break;

        case 25:
            TraceLoadingStep();
            ApplicationGetInstance().CleanGlitch();
            ++m_loadStep;
            break;

        case 26:
            MenuManager::GetInstance()->LoadMenu(3);
            // SEAM: five DebugCachedCharacter::RefreshCache(nullptr, nullptr, nullptr) calls on the DialogMsg,
            // AchievementMsg, StatusMsg and OnlineStatusMsg node caches. The nullptr arguments are ambiguous between the
            // two overloads, and the static caches are not declared in a header this batch includes.
            dword_99F76C->PostInitCharacters();
            // SEAM: RenderFX::InvokeASCallback(MenuManager::GetHUDRoot(), "menu_HUD_0", "completeRefresh", nullptr, 0)
            // (no declaration of InvokeASCallback found in the tree).
            ++m_loadStep;
            break;

        case 27:
        case 28:
            ++m_loadStep;
            break;

        case 29:
            TraceLoadingStep();
            // SEAM: ItemManager::PreCache and ProjectileManager::PreCache on their singletons (not declared).
            ++m_loadStep;
            break;

        case 30:
            TraceLoadingStep();
            // SEAM: VisualFXManager::PreCacheLibraries on the Singleton<VisualFXManager>::s_inst (not declared).
            ++m_loadStep;
            break;

        case 31:
            m_gameEventManager->Compile();
            ++m_loadStep;
            break;

        case 32:
            // SEAM: the ambient light from m_levelConfig->m_ambientColor (+0x1cc, alpha 1.0) on the scene manager at
            // dword_99F73C + 28 (undeclared global); the byte_9A276C flag around the update; CharAI::IncUpdateQueue on the
            // ObjectManager::Update result; PhysicalWorld::update on dword_99F770.
            dword_99F764->Update(1.0f);
            // The camera update (vtable slot +0x10, CameraLevel::Update) runs last, after the physics update.
            m_camera->Update();
            ++m_loadStep;
            break;

        case 33:
            TraceLoadingStep();
            if (m_resume) {
                m_levelSavegame->Load();
                // SEAM: the online branch: when the game is online and this client hosts, the local player's character
                // (PlayerInfo +0x660, not declared) is placed on the floor (PFWorld::GetFloorHeightAt) and every local
                // player is repositioned with GameObject::SetPosition. Needs COnline and PlayerInfo accessors.
            }
            // SEAM: Character::SG_SetUseSpawnPoint(local player character, false). Needs PlayerInfo +0x660 and Character.hpp.
            // +0xf3 is cleared here. dh2/Level.hpp names it m_cameFromOtherLevel, and this step only clears it.
            m_cameFromOtherLevel = false;
            ++m_loadStep;
            break;

        case 34:
            TraceLoadingStep();
            // SEAM: the local player's character (PlayerInfo +0x660, not declared) with Character::SG_SetLevelId and
            // CameraTarget::SetTarget(character, 0), and the CMatching transition message (online only, COnline has no accessor).
            m_camera->SetActive();
            // SEAM: the scene-manager call at dword_99F73C + 96 (undeclared global).
            m_camera->Update();
            // SEAM: the per-character flag reached through Character +0x378 and +0x08, for each local player (Character not
            // included); the scene-node positions for the spawn offset; the camera and light values copied from
            // m_levelConfig into the scene manager; and RENDERING_DisableAllLighting.
            dword_99F764->HandleNoRoomObjects();
            UpdateFog(nullptr);
            UpdateLightSet(true, 4);
            UpdateMaterial(false, true);
            // SEAM: the checkpoint block: when online (no accessor) or the savegame checkpoint does not validate, or
            // m_hasPreviousLevel-named flag +0xf5 is set, the spawn point (GetSpawnPoint, which is a placeholder) or the
            // local player's position is saved with CheckpointSave, and the flag is cleared.
            ++m_loadStep;
            break;

        case 36:
            TraceLoadingStep();
            // The online wait. The original leaves m_loadStep unchanged here (see the notes above).
            // SEAM: when GetOnline()->isOnlineGame_ (no accessor): the local player's loading state, PlayerManager::
            // AllLoadingDone and AllClientsReadyToRoll, ObjectManager::NetworkInitLevel, the Update call with dt 1.0,
            // and PlayerSavegame::SG_TryQuestSync for the host.
            break;

        case 37: {
            TraceLoadingStep();
            MenuManager* menus = MenuManager::GetInstance();
            MenuBase* loading = menus->GetMenuByName("menu_Loading");
            // The original calls IsVisible on this result with no null test. A missing menu faults there. This tests for null.
            if (loading != nullptr && loading->IsVisible()) {
                menus->PopMenu(loading);
            }
            menus->GetMenuByName("menu_HUD_0");
            // SEAM: ShowMemoryStats("After Level Loading") (not declared).
            if (ReadDebugSwitch("IsDisplayLoadingStepName")) {
                MenuDebug::GetInstance()->SetText(nullptr);
            }
            // SEAM: clear the flag at character +0x3d8 + 8 for each local player (Character and PlayerInfo are not declared).
            // SEAM: when online (no accessor) and the object manager is not initialised (m_initialized, +0x1ac): NetworkInitLevel.
            PlaceFaeryAndFollowers(nullptr);
            ++m_loadStep;
            break;
        }

        default:
            m_loadStep = (m_loadStep <= 37) ? m_loadStep + 1 : 38;
            break;
    }

    if (m_loadStep == 38) {
        m_loadProgressPercent = 100;
        // SEAM: if (USE_NATIVE_DRM_GAME) ALicenseCheck_ValidateLicense(1). The native license check is not reconstructed.
        MenuManager* menus = MenuManager::GetInstance();
        menus->PushMenu(menus->GetMenuByName("menu_FadeFromBlackScreen"));
    } else {
        m_loadWorkDone = std::min(m_loadWorkDone, m_loadWorkTotal);
        int percent;
        if (m_loadStep == 36) {
            percent = 100;
        } else {
            percent = 100 * m_loadStep / 38;
            if (percent > 99) {
                percent = 100;
            }
        }
        m_loadProgressPercent = percent;
    }

    MenuBase* loading = MenuManager::GetInstance()->GetMenuByName("menu_Loading");
    if (loading) {
        // SEAM: the onProgress callback on the loading menu's RenderFX and its gameswf character, after
        // gameswf::weak_ptr<gameswf::character>::check_proxy (no declaration of InvokeASCallback found).
    }
}

// 0x3f82d8 Level::Update(bool)
// PARTIAL. Runs one loading step until m_loadStep reaches 38, unless forceUpdate is set. After that the frame update runs.
// The early returns, the script start and the game-event update are reconstructed. The scene, sound, physics, AI, camera
// and menu work in between is a SEAM.
void Level::Update(bool forceUpdate) {
    PushProfilingContext("Level::Update");
    if (m_loadStep != 38 && !forceUpdate) {
        _LoadProcess();
        PopProfilingContext("Level::Update");
        return;
    }
    // SEAM: MenuDebugHUD::GetInstance() and the reset of its pending-text map (the MenuDebugHUD fields it reads are not declared).
    if (ReadDebugSwitch("IsUsingToggleDisplayMode")) {
        PopProfilingContext("Level::Update");
        return;
    }
    if (ReadDebugSwitch("IsLevelUpdatePaused")) {
        PopProfilingContext("Level::Update");
        return;
    }
    if (m_startScriptId != -1 && ApplicationGetInstance().IsCurrentlyInGameView()) {
        ScriptManager::s_inst.StartScript(m_startScriptId, -1, false);
    }
    // The savegame update runs on the local player's character (PlayerInfo +0x660, PlayerInfo::unk_660), when there is one.
    if (Character* localCharacter = dword_99F76C->GetLocalPlayer(0, true)->unk_660) {
        localCharacter->SG_Update(false);
    }
    if (!m_isLoaded) {
        dword_99F76C->Update();
    }
    if (dword_99F76C->m_globalDeathState == 0) {
        ScriptManager::s_inst.ExecuteAllScripts();
    }
    m_gameEventManager->Update();
    // SEAM: the ambient light on the scene manager (m_levelConfig->m_ambientColor, +0x1cc). The scene manager is glitch::IDevice
    // +0x1c, and the IDevice data members are not declared. SEAM: PhysicalWorld::update on dword_99F770 (global not declared).
    SpawnGroupManager::GetInstance()->update(0.0);
    // SEAM: CharAI::HandleGroups. In the decompiled code its receiver is the return value of SpawnGroupManager::update, and the
    // method is not static in dh2/CharAI.hpp, so there is no object to call it on.
    dword_99F764->Update(1.0f);
    // SEAM: CharAI::IncUpdateQueue (non-static; the receiver is not recovered).
    UpdateCameraZoom();
    VisualFXManager::s_inst.Update();
    if (m_cameraOverview != nullptr && m_cameraOverview == CameraBase::s_activeCamera) {
        m_cameraOverview->Update();
    }
    m_camera->Update();
    // SEAM: the scene-node update through the camera's node (vtable +0xb8 on the object at +0x08), the node position read
    // and the SceneManager::UpdateFog distance (the scene node layout is not declared).
    // SEAM: the RENDERING_DisableAllLighting and RENDERING_DisableAllSpecular switches written to the scene manager flags
    // (undeclared globals). SEAM: the VoxSoundManager music and ambience block on the first update (m_musicSoundId,
    // m_ambienceSoundId), not declared.
    m_isLoaded = true;
    UpdateListener();
    UpdateDynamicFog();
    if (ReadDebugSwitch("IsAutoLoadingLevels")) {
        // SEAM: the debug auto-load countdown and the random level pick over Arrays::LevelList, using Random and
        // LC_API_STRCMP_0 (not declared), then Application::LoadLevel with the picked level.
    }
    PopProfilingContext("Level::Update");
}

// 0x3f90b0 _ZN5LevelD2Ev and 0x3f92c4 _ZN5LevelD1Ev (Level::~Level()). Both addresses hold the same body.
// The symbol table names them this way; dh2/Level.hpp's comment names them the other way round.
// PARTIAL. The original tears the level down in this order: skills and objects, AI aggro and groups, the cached character
// OIDs, the message queues, scripts and HUD, items and projectiles, the visual FX libraries, the game-event manager, the
// two batch compilers, the object manager, the zoom handler's camera, the scene manager's camera, physics, the PF world,
// the anim set manager, the base EventManager, the savegame, the Lua buffered files, the level name and the Lua script.
// The C++ member and base destructors that run after the body cover the last steps: m_levelName, then m_script, then
// the EventManager base. The steps in SEAM comments are not made.
Level::~Level() {
    CleanUpAllSkills();
    UnlockAllObjects();
    // SEAM: CharAI::ClearAllAggro and CharAI::ClearGroupInfo on the value returned by UnlockAllObjects (the header types
    // that call void and the decompiled chain does not match).
    // SEAM: the cached character OID tree dword_9A293C and dword_9A2930 (undeclared statics), and Character::ClearConcurrentAI
    // (dh2/Character.hpp conflict).
    // SEAM: MenuMessageManager<T, N>::FlushEnqueuedMessages for StatusMsg, TutorialMsg, DialogMsg (twice) and OnlineStatusMsg.
    ScriptManager::s_inst.Flush();
    InfoHUDManager::GetInstance()->Flush();
    // SEAM: HUDControls::GetInstance()->Flush(). dh2/HUDControls.hpp cannot share this translation unit with
    // core-platform/memory.hpp (glitch::IEventReceiver is defined by both stand-in headers).
    // SEAM: ItemManager::Flush and ProjectileManager::Flush. Both are non-static members and their headers give no instance accessor.
    VisualFXManager::s_inst.FlushLibraries();
    delete m_gameEventManager;
    m_gameEventManager = nullptr;
    delete m_batchNodeCompiler;
    m_batchNodeCompiler = nullptr;
    delete m_batchNodeCompiler2;
    m_batchNodeCompiler2 = nullptr;
    dword_99F764->Flush();
    // SEAM: ZoomHandler::setCamera(nullptr) on Application::m_zoomHandler (+0x50). dh2/ZoomHandler.hpp conflicts with memory.hpp.
    // SEAM: the scene manager's camera reset (virtual slot +104) and CSceneManager::setActiveCamera(nullptr) on the scene
    // manager, which is glitch::IDevice +0x1c (IDevice data members not declared).
    // SEAM: PhysicalWorld::clear on dword_99F770 (global not declared), and PFWorld::Flush (PFWorld conflicts with memory.hpp).
    AnimSetManager::s_inst.Flush();
    EventManager::Flush();
    if (m_levelSavegame) {
        delete m_levelSavegame;
        m_levelSavegame = nullptr;
    }
    ApplicationGetInstance().CleanGlitch();
    ApplicationGetInstance().m_luaManager->FlushBufferedFiles();  // dword_99F768 is Application +0x3c (m_luaManager)
}

// 0x3f94f4 Level::CanMoveTowards(Point3D<float> const&, Point3D<float> const&)
// PARTIAL. The original answers yes at once for a single player character, for an online game, and for a zero-length
// direction. Otherwise it measures the angle between the direction and the vector from the camera node's position to the
// point, and checks each local player's screen position against the co-op limits. Reconstructed here: the single-character
// test and the zero-length test. The online test is COnline::isOnlineGame_ (protected, no accessor), and it comes first, so
// nothing after it can be decided. The angle test needs the camera node's position getter (CameraBase::m_node, vtable +0xa0,
// not named). The limits are Structs::DesignSettings m_w6 (bottom, +0x18), m_w7 (sides, +0x1c) and m_w8 (top, +0x20). The
// result returned for that path is a placeholder true.
bool Level::CanMoveTowards(const Point3D<float>& from, const Point3D<float>& direction) {
    (void)from;
    if (dword_99F76C->m_numCharacters == 1) {
        return true;
    }
    const float lengthSquared = direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z;
    if (std::fabs(lengthSquared) < 0.0001) {
        return true;
    }
    // SEAM: GetOnline()->isOnlineGame_ (online: true) and the angle test against the camera target, and the co-op screen-limit
    // test. Placeholder: true.
    return true;
}

// 0x3f9870 Level::UpdateCameraZoom()
// PARTIAL. The co-op zoom sets the camera zoom ceiling (CameraLevel::m_zoomTarget, +0x8c) toward the value that keeps the
// local players' screen positions inside the co-op limits (Structs::DesignSettings m_w6, m_w7 and m_w8, with the zoom step in
// m_w3 and m_w4). Reconstructed: the early exit for a single player character. The rest waits on the online flag
// (COnline::isOnlineGame_, protected with no accessor), which is tested first. The per-player screen scan is declared
// (PlayerManager::GetPlayer, Character::IsDead, CameraBase::GetScreenCoord) but is not written behind that gate.
void Level::UpdateCameraZoom() {
    if (dword_99F76C->m_numCharacters == 1) {
        return;
    }
    if (GetOnlineGameFlag()) {  // COnline +0x05 (GetOnline()->isOnlineGame_)
        return;
    }
    const auto* settings = reinterpret_cast<const Structs::DesignSettings*>(Arrays::DesignSettingsTable::members);
    // Level_Players.cpp:36..38 asserts. gAssertLevel 2 traps in the original; only the message is reproduced here.
    if (settings->m_w7 < 0.0f && gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "settings.CoopCamLimits_Sides >= 0.0f",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Level\\Level_Players.cpp", 36);
    }
    if (settings->m_w8 < 0.0f && gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "settings.CoopCamLimits_Top >= 0.0f",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Level\\Level_Players.cpp", 37);
    }
    if (settings->m_w6 < 0.0f && gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "settings.CoopCamLimits_Bottom >= 0.0f",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Level\\Level_Players.cpp", 38);
    }
    // Lowest headroom over the living local players (the original starts at FLT_MAX).
    float headroom = 3.4028e38f;
    for (int i = 0; i != 4; ++i) {
        Character* character = dword_99F76C->GetPlayer(i, false)->unk_660;
        if (character && !character->IsDead()) {
            Point2D<float> screen;
            CameraBase::GetScreenCoord(character->m_position, screen);
            if (settings->m_w7 - std::fabs(screen.X) < headroom) {
                headroom = settings->m_w7 - std::fabs(screen.X);
            }
            if (headroom > settings->m_w8 - screen.Y) {
                headroom = settings->m_w8 - screen.Y;
            }
            if (headroom > screen.Y + settings->m_w6) {
                headroom = screen.Y + settings->m_w6;
            }
        }
    }
    const float current = settings->m_w3;
    const float delta = std::fabs(current - headroom);
    if (current > headroom) {
        m_camera->m_zoomTarget -= delta * settings->m_w4;
    } else if (current < headroom) {
        m_camera->m_zoomTarget += delta * settings->m_w4;
    }
}

// 0x474260 Level::GetRimLightFile()
// Copies the rim-light file name of the level config. The original's string copy from the [c_str, finish) range is the
// std::string copy of m_rimLightFile.
std::string Level::GetRimLightFile() {
    return RequireLevelConfig(this)->m_rimLightFile;
}
