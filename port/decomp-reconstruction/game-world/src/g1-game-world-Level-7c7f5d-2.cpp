// Batch g1-game-world-Level-7c7f5d-2: 20 Level entries (part 3 of 4). Declarations come from dh2/Level.hpp.
// The two Level constructor addresses share one body. A PARTIAL body names the owner header it waits on or the seam it leaves.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "core-platform/device.hpp"  // Device::IsHighPerformance (static)
#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/Application.hpp"        // ApplicationGetInstance (GetCurrentLevel)
#include "dh2/Character.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/CameraOverview.hpp"
#include "dh2/Level.hpp"
#include "dh2/LevelConfig.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/Module.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/batch/BatchNodeCompiler.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"      // dword_99F76C (PlayerManager*)
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764 (ObjectManager*)
#include "dh2/rnd/Block.hpp"
#include "dh2/rnd/RandomGenerator.hpp"
#include "dh2/rnd/Tile.hpp"
#include "pydata-accessors/member_names.hpp"              // Arrays::GetMemberIDByString (SetLevelConfig)

// 0x3f07b0 _ZN5Level18SG_SaveLocalPlayerEb
// PARTIAL: the local player's character is read as the original does, PlayerManager::GetLocalPlayer(dword_99F76C, 0, 1)
// ->character_660 (PlayerInfo +0x660, declared as unk_660), and passed to SG_SavePlayer(Character*, levelChange). That
// overload (0x3efa54) is an empty body in another batch, so nothing is saved yet.
void Level::SG_SaveLocalPlayer(bool levelChange) {
    Character* player = dword_99F76C->GetLocalPlayer(0, true)->unk_660;  // PlayerInfo +0x660
    SG_SavePlayer(player, levelChange);
}

// 0x3f07f8 _ZN5Level19GenerateRandomLevelER12StreamBufferj
// The previous generator is destroyed, a new one is built, LoadRuleFile gets the level name (+0x10c), the levels are
// generated with the seed, written to the stream, and the generator is destroyed again. The result is the count that
// Generate returns. operator new(400) and CustomFree become new and delete on the host.
int Level::GenerateRandomLevel(StreamBuffer& out, uint32_t seed) {
    if (m_randomGenerator) {
        delete m_randomGenerator;
        m_randomGenerator = nullptr;
    }
    m_randomGenerator = new rnd::RandomGenerator();
    m_randomGenerator->LoadRuleFile(m_levelName.c_str());
    int count = m_randomGenerator->Generate(static_cast<int>(seed));
    m_randomGenerator->SaveModularLevelToStream(out);
    delete m_randomGenerator;
    m_randomGenerator = nullptr;
    return count;
}

// 0x3f0898 _ZN5Level22PlaceFaeryAndFollowersEP9Character
// PARTIAL: waits on the Character API. The filter that the original applies to each ObjectManager::m_characters entry
// (IsFaerie and IsFollower) needs dh2/Character.hpp, but including it in this translation unit pulls in a second
// IStreamBase definition (pydata-accessors deps/stream_reader.hpp against net-transport deps/i_stream_base.hpp). That
// conflict is not this batch's to fix, so the filter is a seam and no entry is touched. Also not rebuilt: (1) a null player
// argument. The original then takes the hosting or local player's character (PlayerInfo +0x660, dh2/PlayerInfo.hpp absent)
// and tests COnline::isOnlineGame_ (+0x05, private with no getter). (2) The GameObject::SetPosition call. The pseudocode
// shows only the object, so its arguments are not recovered. (3) The faery bookkeeping: each local player's character gets
// the faery at Character +0x420, the AI master is swapped (CharAI subobject at Character +0x3c8), the faery id is changed,
// and the saved master (Character +0x418) is restored. Character.hpp types +0x418 as a float range, not a pointer.
void Level::PlaceFaeryAndFollowers(Character* player) {
    for (Character* character : dword_99F764->m_characters) {
        // SEAM: the faerie/follower test on character and the placement described above are not reconstructed.
        (void)character;
        (void)player;
    }
}

// 0x3f0ae0 _ZN5Level14UpdateListenerEv
// PARTIAL: placeholder. Two blockers remain. (1) VoxSoundManager is not declared anywhere in the tree (no SetListenerPos),
// so the listener is not updated. (2) The listener row is Arrays::Listeners (stride 28, the fields read at +0x04 to +0x18),
// read once through a function-local guard cache. pydata-accessors gives it only a stride, not a typed table. The reads the
// function makes are declared now: the local player's target position and look-at (PlayerInfo unk_660, GameObject
// GetTargetPosition and GetLookAtVec), the camera vectors (CameraBase::GetCameraLookAtVec and GetCameraUpVec), and the
// screen centre (GetWorldPosFromScreenPos).
void Level::UpdateListener() {
}

// 0x3f1008 _ZN5Level11_LoadCameraEv
// PARTIAL: the two allocations are reconstructed, in the original's order: CameraOverview (m_cameraOverview, +0x12c) and
// CameraLevel (m_camera, +0x128), each through its constructor. The rest is not made. (1) The camera file, animation set and
// camera node names are the LevelConfig string data words at +0x260, +0x278 and +0x290, which lie in the header's m_cameraFile,
// m_cameraAnimset and m_cameraName strings. (2) The animation set row is found in Arrays::CamAnimSetTable, which is declared
// only as a raw byte table, so the row (and the clip id at row +0x10) cannot be typed. (3) The sky box and the scene manager
// are read from the device (+0x1c), which is not declared. The field of view and aspect (CameraBase::SetData with LevelConfig
// +0x294 and +0x298), SetActive, PlayAnim, CameraTarget::SetTarget, ZoomHandler::setCamera and the zoom fields are omitted with them.
void Level::_LoadCamera() {
    m_cameraOverview = new CameraOverview();  // +0x12c, operator new(40) in the original
    m_camera = new CameraLevel();             // +0x128, operator new(168) in the original
    // SEAM: the camera load and the rest of the set-up described above are not made.
}

// 0x3f1394 _ZNK5Level15GetMPGameScriptEv
// PARTIAL: the original returns the int at LevelConfig +0x314 (788). In LevelConfig.hpp, +0x314 lies inside m_mpScript (a
// std::string at +0x300), so the field is not identified, and 0 is returned until the LevelConfig layout is reconciled. The
// m_levelConfig assert is reproduced by GetLevelConfig(), which the original repeats inline with the same message and abort.
int Level::GetMPGameScript() const {
    LevelConfig* config = GetLevelConfig();
    (void)config;
    return 0;
}

// 0x3f1430 _ZN5Level9EnableFogEPN6glitch5scene10ISceneNodeE
// PARTIAL: the fog start (LevelConfig +0x1d8, m_fogStart), end (+0x1dc, m_fogEnd) and colour (+0x1e0, m_fogColor) are declared.
// The original then calls SceneManager::EnableFog(start, end, &colour, node) on the scene manager, which it reads from the
// device's +0x1c field (glitch::IDevice does not declare it). That call is left out, so nothing is changed here.
void Level::EnableFog(glitch::scene::ISceneNode* node) {
    LevelConfig* config = GetLevelConfig();  // the same m_levelConfig assert as the original
    (void)config;
    (void)node;
}

// 0x3f150c _ZN5Level14SetLevelConfigEP11LevelConfig
// The config is stored (+0x38) and checked with the m_levelConfig assert. The music id (+0x11c) comes from the music name,
// the ambience id (+0x124) from the ambiant music name and the id at +0x120 from the safezone music name, each through
// Arrays::GetMemberIDByString<Arrays::Sounds>. The original reads the names' data words, which are the string members
// (the header's music, safezone_music and ambiant_music strings), and an empty ambient or safezone name leaves its id
// unchanged. The original reads the names of a null config after its assert and faults. Here a null config returns after the assert.
void Level::SetLevelConfig(LevelConfig* config) {
    m_levelConfig = config;                   // +0x38
    LevelConfig* checked = GetLevelConfig();  // the m_levelConfig assert (Level.h:476)
    if (!checked) {
        return;
    }
    m_musicSoundId = Arrays::GetMemberIDByString<Arrays::Sounds>(checked->m_music.c_str());  // +0x11c
    if (!checked->m_ambiantMusic.empty()) {
        m_ambienceSoundId = Arrays::GetMemberIDByString<Arrays::Sounds>(checked->m_ambiantMusic.c_str());  // +0x124
    }
    if (!checked->m_safezoneMusic.empty()) {
        unk_120 = Arrays::GetMemberIDByString<Arrays::Sounds>(checked->m_safezoneMusic.c_str());  // +0x120
    }
}

// 0x3f16c0 _ZN5Level16UpdateDynamicFogEv
// PARTIAL: the guard is reconstructed: the config's dynamic fog colour list (+0x204, m_dfogColors) must be non-empty. The
// original then takes the absolute position of the current level's camera scene node (Application's current Level, its
// m_camera (+0x128), that camera's glitch camera node at CameraBase +0x08), passes it to
// ObjectManager::GetDynamicFogColorAtPos with the blend radius (+0x214), stores the colour at the config's +0x1ec
// (m_clearColor), and sets the global material fog parameter through the device's material parameter manager (+0x1c, +0x14).
// The scene node's getAbsolutePosition needs glitch-scene-collada-gui/scene/ISceneNode.hpp, which conflicts with
// glitch-shared/glitch_core.hpp in this translation unit, and the material chain is not declared. Both are left out (SEAM).
void Level::UpdateDynamicFog() {
    if (!m_levelConfig || m_levelConfig->m_dfogColors.empty()) {
        return;
    }
    // SEAM: camera node position, GetDynamicFogColorAtPos, the store to m_clearColor (+0x1ec) and the material parameter set are not made.
}

// 0x3f2304 _ZN5Level9UpdateFogEPN6glitch5scene10ISceneNodeE
// PARTIAL: the choice reads LevelConfig +0x1dc (m_fogEnd): non-zero calls EnableFog(node), zero calls DisableFog(node). Both
// original branches first read the "isTracingLevel" debug switch, and that read is kept. Its result is unused, as in the
// original. The choice is complete; EnableFog and DisableFog are partial (the scene manager is not declared).
void Level::UpdateFog(glitch::scene::ISceneNode* node) {
    if (!m_levelConfig) {
        return;
    }
    bool fogEnabled = m_levelConfig->m_fogEnd != 0;  // +0x1dc, tested before the debug switch is read
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingLevel"));
    if (fogEnabled) {
        EnableFog(node);
    } else {
        DisableFog(node);
    }
}

// 0x3f240c _ZN5Level14_LoadBatchInitEv
// PARTIAL: the original first calls a virtual method of the scene manager (its glitch::scene::CSceneManager base, vtable
// +0x60, slot 24, with arguments 0 and 0). That slot has no name in the tree, and the scene manager is the device's +0x1c
// field, which is not declared, so the call is left out (SEAM). The switch test, the destruction of the old compiler and
// the allocation are reconstructed. The class has no constructor symbol (batch::BatchNodeCompiler writes its fields inline in
// the original), so value-initialising new matches the original's init.
void Level::_LoadBatchInit() {
    // SEAM: the scene manager's slot +0x60 call with arguments (0, 0) is not made.
    DebugSwitches::GetInstance()->load();
    if (!DebugSwitches::GetInstance()->GetSwitch(std::string("IsDisablingCompiledBatching"))) {
        delete m_batchNodeCompiler;
        m_batchNodeCompiler = new batch::BatchNodeCompiler();
    }
}

// 0x3f254c _ZNK5Level4DrawEv
// PARTIAL: the floor-box switch is read and its result is discarded, as in the original. The path-node block is a seam. It
// runs PFWorld::DBG_Draw at the local character's m_position (PlayerInfo +0x660, GameObject +0x160), then draws the look-at
// and perpendicular lines through glitch::IVideoDriver::draw3DLine (vtable +0x1c, glitch-video video_driver.hpp). PFWorld cannot
// share this translation unit with core-platform/memory.hpp (the glitch stand-ins conflict), and the video driver is
// glitch::IDevice +0x10, whose data members are not declared. The perpendicular uses Vec3f_K (declared) and dword_99F87C and
// dword_99F880 (not declared; Vec3f_K.hpp gives them as Y 0 and Z 1).
void Level::Draw() const {
    if (m_loadStep != 38) {
        return;
    }
    dword_99F764->Draw3D();
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch(std::string("isDisplayingFloorBoxes"));
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch(std::string("isDisplayingPathNodes"))) {
        // SEAM: the path-node drawing described above is not reconstructed.
    }
}

// 0x3f2ae4 _ZN5Level13_LoadBatchingEv
// PARTIAL. The IsDisablingCompiledBatching gate and the BUFFER SIZE print of each branch are reconstructed from LevelConfig
// (high-performance devices read +0x2a0..+0x2a8, the others +0x2ac..+0x2b4). Device::IsHighPerformance is static and takes no
// argument, so the clearRenderLists() result is not a Device pointer and nothing is passed to it. clearRenderLists itself is a
// void call on the scene manager, which is glitch::IDevice +0x1c. Those IDevice data members are not declared, so that call,
// the scene manager flag at +0x43c (SceneManager::unk_43c), the batch sizes written into the scene manager's batch object
// (+0x1c, fields +0x22c and +0x230, each times 1.02), BatchNodeCompiler::Compile and the tail after it are not made.
void Level::_LoadBatching() {
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch(std::string("IsDisablingCompiledBatching"))) {
        return;
    }
    LevelConfig* config = GetLevelConfig();  // the original reads this+0x38 directly; a null config faults there too
    if (Device::IsHighPerformance()) {
        std::printf("*** BUFFER SIZE: %d %d %d\n", config->m_progVertexBufferSize, config->m_progIndexBufferSize,
                    static_cast<int>(config->m_progQuantize));
        // SEAM: the sizes and BatchNodeCompiler::Compile(m_batchNodeCompiler, m_progQuantize), see above.
    } else {
        std::printf("*** BUFFER SIZE: %d %d %d\n", config->m_fixVertexBufferSize, config->m_fixIndexBufferSize,
                    static_cast<int>(config->m_fixQuantize));
        // SEAM: the sizes and BatchNodeCompiler::Compile(m_batchNodeCompiler, m_fixQuantize), see above.
    }
}

// 0x3f2d38 _ZN5Level14_LoadBatchListEv
// PARTIAL. The original walks ObjectManager::m_objectMap (+0x0c, a std::map<int, ObjectListItem>) in tree order and pushes
// each object that passes the filters onto the batch compiler list (BatchNodeCompiler::m_gameObjects). The filters: a null
// object or one whose ObjectBase::m_isUnderlay (+0x83) is set is skipped; "Module" and "Decor" are always pushed; "Player"
// types, and any object whose name contains "Player", are never pushed; "Character" is pushed when MeetCondition holds and
// it is not a faerie; the container, door, trap and AnimatedDecor types are pushed when MeetCondition holds. The walk needs
// the map node's object handle and flag, which are in ObjectListItem. dh2/ObjectListItem.hpp is absent (ObjectManager.hpp
// forward-declares it only), so the walk is not written. Only the IsDisablingCompiledBatching gate is reconstructed.
void Level::_LoadBatchList() {
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch(std::string("IsDisablingCompiledBatching"))) {
        return;
    }
}

// 0x3f3128 _ZN5LevelC1EPKcijjjbbii
// 0x3f34c0 _ZN5LevelC2EPKcijjjbbii
// The two addresses are the C1 and C2 copies of one body. The pseudocode differs only in how the decompiler types the
// savegame slot word (this+0x114), so one definition serves both.
// PARTIAL, two seams:
//   1. The online block is skipped. It needs GetOnline()->isOnlineGame_ (+0x05), which COnline keeps protected with no getter
//      (net-session-online/c_online.hpp), and OnlineSingleton<OnlineGameState>, CMatching and CMatchingGLLive, which are not
//      declared in the tree. So m_resume is not cleared for a joining non-host player when the game is online.
//   2. The Arrays::LevelList scan is skipped. Its rows need w7_arrays_00.hpp, which cannot share a translation unit with
//      member_names.hpp. So m_levelListIndex, m_levelListSubId and m_isRandomLevel keep their defaults, and the savegame
//      creation below does not run for a level that is in the list.
// Everything else is rebuilt: the field defaults, the LuaScript data directory and its two loads, Module::s_moduleId, and the
// savegame flags that depend on the list index.
Level::Level(const char* name, int entryPointId, uint32_t savegameSlot, uint32_t seed, uint32_t syncedSeed,
             bool resume, bool useSpawnPoint, int fromSubId, int difficulty)
    : EventManager(), m_script(false), m_levelName(name) {
    m_loadProgressPercent = 0;                 // +0x30
    m_levelConfig = nullptr;                   // +0x38
    m_levelListIndex = -1;                     // +0x3c
    m_levelListSubId = -1;                     // +0x40
    m_seed = seed;                             // +0xdc
    m_syncedSeed = syncedSeed;                 // +0xe0
    m_listenerIndex = 1;                       // +0xe4
    m_isRandomLevel = false;                   // +0xe8
    m_levelSavegame = nullptr;                 // +0xec
    m_levelChange = false;                     // +0xf0
    m_resume = resume;                         // +0xf1
    m_useSpawnPoint = useSpawnPoint;           // +0xf2
    m_cameFromOtherLevel = false;              // +0xf3
    m_loadingCheckpoint = false;               // +0xf4
    m_hasPreviousLevel = false;                // +0xf5
    m_entryPointId = entryPointId;             // +0x110
    m_savegameSlot = savegameSlot;             // +0x114
    m_difficulty = difficulty;                 // +0x118
    m_musicSoundId = -1;                       // +0x11c
    unk_120 = -1;                              // +0x120
    m_ambienceSoundId = -1;                    // +0x124
    m_camera = nullptr;                        // +0x128
    m_cameraOverview = nullptr;                // +0x12c
    m_loadStep = 0;                            // +0x130
    m_loadWorkDone = 0;                        // +0x134
    m_loadWorkTotal = 0;                       // +0x138
    m_loadFileCount = 0;                       // +0x13c
    m_loadFileData = nullptr;                  // +0x140
    m_isLoaded = false;                        // +0x144
    m_inTransition = false;                    // +0x145
    m_startScriptId = -1;                      // +0x148
    m_randomGenerator = nullptr;               // +0x14c
    unk_150 = nullptr;                         // +0x150
    m_mpReferee = nullptr;                     // +0x154
    m_batchNodeCompiler = nullptr;             // +0x158
    m_batchNodeCompiler2 = nullptr;            // +0x15c
    m_moduleOffset.X = 0.0f;                   // +0x160
    m_moduleOffset.Y = 0.0f;                   // +0x164
    m_moduleOffset.Z = 0.0f;                   // +0x168
    unk_16c = false;                           // +0x16c
    m_objectModuleId = -1;                     // +0x18c
    m_gameEventManager = nullptr;              // +0x194
    m_isInGameView = false;                    // +0x198
    m_cameraBaseOffset.X = 0.0f;               // +0x19c
    m_cameraBaseOffset.Y = 0.0f;               // +0x1a0
    m_cameraBaseOffset.Z = 0.0f;               // +0x1a4
    unk_1a8 = false;                           // +0x1a8

    ++s_dbgLevelLoadCount;
    m_script.m_scriptDirectory = "data/scripts/";  // LuaScript +0x68 (Level +0xac)
    m_script.Load("level/combat_formulas");
    m_script.Load("level/death_scripts");

    Module::s_moduleId = 0;

    // SEAM: the Arrays::LevelList scan (sets m_levelListIndex, m_levelListSubId and m_isRandomLevel from the row whose lowercased
    // name is a substring of m_levelName; rows are Structs::LevelDeclaration with the name at m_w8, subId at m_w4, isRandom at
    // m_w5). It needs Arrays::LevelList::members and size from pydata-accessors/w7_arrays_00.hpp, which conflicts with
    // pydata-accessors/member_names.hpp (included for SetLevelConfig's Arrays::GetMemberIDByString) in one translation unit.

    if (m_levelListIndex != -1) {
        if (fromSubId != -1) {
            m_hasPreviousLevel = true;
            if (!(fromSubId | m_levelListSubId) || (fromSubId != m_levelListSubId && m_levelListSubId)) {
                m_cameFromOtherLevel = true;
            }
        }
        m_levelSavegame = new LevelSavegame(this, m_savegameSlot, m_levelListSubId, m_levelListIndex, m_difficulty, false);
    }
}

// 0x3f38a0 _ZN5Level15_LoadScriptFileERSs
// Reconstructed. The original searches the file name backwards for ".pyscript" (the offset of 9 is its length). When the
// name is longer than 8 characters and the search finds it, it turns every backslash into a slash and then loads the two
// companion names: scripts through ScriptManager::LoadScriptFile and names through ScriptManager::LoadScriptFileNames,
// each with append false. Here rfind plays the role of the reversed std::search.
// The two names are not a plain prefix split. The original copies the file name twice:
//   scripts: copy of file, replace(pos, size - pos, "_pyscripts.bin", 14). That replaces [pos, end).
//   names:   copy of file, replace(pos, count, "_pyscriptnames.bin", 18), where count is read from the scripts copy after its
//            replace (size - pos == 14). So only 14 characters are replaced at pos, and a tail beyond pos + 14 is kept.
// For the shipped names, which end exactly in ".pyscript", this is <prefix>_pyscripts.bin and <prefix>_pyscriptnames.bin.
void Level::_LoadScriptFile(std::string& file) {
    if (file.size() <= 8) {
        return;
    }
    std::size_t pos = file.rfind(".pyscript");
    if (pos == std::string::npos) {
        return;
    }
    for (char& c : file) {
        if (c == '\\') {
            c = '/';
        }
    }
    std::string scripts = file;
    scripts.replace(pos, scripts.size() - pos, "_pyscripts.bin", 14);
    std::string names = file;
    names.replace(pos, scripts.size() - pos, "_pyscriptnames.bin", 18);
    ScriptManager::s_inst.LoadScriptFile(scripts.c_str(), false);
    ScriptManager::s_inst.LoadScriptFileNames(names.c_str(), false);
}

// 0x3f3a58 _ZN5Level12_LoadScriptsEv
// Reconstructed. The last step passes the level config's script file name (LevelConfig +0x150, m_scriptFile; the pseudocode's
// (std::string*)config + 14 with 24-byte strings) to _LoadScriptFile by reference, so _LoadScriptFile rewrites the config's
// string in place, as the original does. A null config asserts in GetLevelConfig; the original then passes null + 0x150 and
// faults, which std::abort reproduces here.
void Level::_LoadScripts() {
    ScriptManager::s_inst.UnLoadAllScripts();
    ScriptManager::s_inst.LoadScriptFile("data/pydata/scripts_pyscripts.bin", true);
    ScriptManager::s_inst.LoadScriptFileNames("data/pydata/scripts_pyscriptnames.bin", true);
    LevelConfig* config = GetLevelConfig();  // the same m_levelConfig assert as the original
    if (!config) {
        std::abort();
    }
    _LoadScriptFile(config->m_scriptFile);
}

// 0x3f3b40 _ZN5Level8LoadFileERKSsS1_
// PARTIAL: the original is an incremental TinyXML reader. Each call either advances the open file's element cursor
// (m_loadFileData, walked by _LoadFromXML and NextSiblingElement) or opens the next candidate: the four folder prefixes and
// four fragments to remove (their strings are not recovered), an uncompiled-data check through
// Application::IsUsingUncompiledData, and a LoadFileData (80 bytes) that wraps a StreamBuffer. None of that is reconstructed.
// The function returns 1 ("finished") so that the callers' do-while loops do not repeat. No module file is loaded.
int Level::LoadFile(const std::string& file, const std::string& tag) {
    (void)file;
    (void)tag;
    return 1;
}

// 0x3f4154 _ZN5Level8LoadTileEPN3rnd4TileEi
// PARTIAL. The original starts with an unconditional ASSERT(false) at Level.cpp line 3269. That is reproduced: level 2
// stops with a null store (here std::abort), and level 1 prints the assert text. Reconstructed: the grid position (the tile's
// m_x, m_y and m_z, scaled by the block's unit and block sizes; the y term is negated), the module id, the mvx and mgx paths
// (gameplay/visual folder plus block name), the two module loads, the zeroed module offset, and the recursion over the
// children. Skipped: the mgp and mvp module loads (see the SEAM comment below).
int Level::LoadTile(rnd::Tile* tile, int moduleId) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Level\\Level.cpp", 3269);
    }
    rnd::Block* block = tile->m_block;
    // The original sets the grid position before SetObjectModuleId. X scales the cell column by the block width, with half a
    // cell for each extra cell; Y does the same with the cell row and the unit height, negated (the original flips the sign bit).
    m_moduleOffset.X = (static_cast<float>(tile->m_x) + static_cast<float>(block->m_blockWidth - 1) * 0.5f) * block->m_unitWidth;
    m_moduleOffset.Y = (static_cast<float>(tile->m_y) + static_cast<float>(block->m_blockHeight - 1) * 0.5f) * -block->m_unitHeight;
    m_moduleOffset.Z = tile->m_z;
    SetObjectModuleId(moduleId);

    std::string mvxPath = block->m_gameplay + "/mvx/" + block->m_name + ".mvx";
    std::string mgxPath = block->m_gameplay + "/mgx/" + block->m_name + ".mgx";
    // SEAM: the mgp and mvp loads (original: gameplay + "/mgp/" + range at Tile +0x60..+0x64, and gameplay + "/mvp/" + range at
    // +0x78..+0x7c). Those words lie inside the ListElem copy's m_gameplay (ListElem +0x1c) and m_visual (+0x34) string
    // objects, so they are string internals that the host std::string does not expose. Not reconstructed.

    int loaded = 0;
    do {
        loaded = LoadFile(mvxPath, std::string("Module"));
    } while (!loaded);
    loaded = 0;
    do {
        loaded = LoadFile(mgxPath, std::string("Module"));
    } while (!loaded);

    unk_16c = false;                 // +0x16c
    m_moduleOffset.X = 0.0f;         // +0x160
    m_moduleOffset.Y = 0.0f;         // +0x164
    m_moduleOffset.Z = 0.0f;         // +0x168

    int id = moduleId;
    for (int i = 0; i < tile->m_childCount; ++i) {
        id = LoadTile(tile->m_children[i], id + 1);
    }
    return id;
}
