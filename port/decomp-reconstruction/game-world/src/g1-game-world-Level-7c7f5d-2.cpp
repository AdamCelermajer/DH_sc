// Batch g1-game-world-Level-7c7f5d-2: 20 Level entries (part 3 of 4). Declarations come from dh2/Level.hpp.
// The two Level constructor addresses share one body. A PARTIAL body names the owner header it waits on or the seam it leaves.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/Level.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/batch/BatchNodeCompiler.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"      // dword_99F76C (PlayerManager*)
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764 (ObjectManager*)
#include "dh2/rnd/Block.hpp"
#include "dh2/rnd/RandomGenerator.hpp"
#include "dh2/rnd/Tile.hpp"

// 0x3f07b0 _ZN5Level18SG_SaveLocalPlayerEb
// PARTIAL: waits on dh2/PlayerInfo.hpp (absent). The original saves PlayerManager::GetLocalPlayer(dword_99F76C, 0, 1)
// ->character_660 (PlayerInfo +0x660) through SG_SavePlayer(character, levelChange). PlayerInfo is only forward-declared,
// so the character cannot be read and nothing is saved here.
void Level::SG_SaveLocalPlayer(bool levelChange) {
    (void)levelChange;
}

// 0x3f07f8 _ZN5Level19GenerateRandomLevelER12StreamBufferj
// PARTIAL: rnd::RandomGenerator::Generate returns an int in the original, and the function returns that value. Its header
// (dh2/rnd/RandomGenerator.hpp) declares Generate as void, so the count is not available and 0 is returned. The other steps
// match the original: the previous generator is destroyed, a new one is built, LoadRuleFile gets the level name (+0x10c),
// the levels are generated, written to the stream, and the generator is destroyed again. operator new(400) and CustomFree
// become new and delete on the host.
int Level::GenerateRandomLevel(StreamBuffer& out, uint32_t seed) {
    if (m_randomGenerator) {
        delete m_randomGenerator;
        m_randomGenerator = nullptr;
    }
    m_randomGenerator = new rnd::RandomGenerator();
    m_randomGenerator->LoadRuleFile(m_levelName.c_str());
    m_randomGenerator->Generate(static_cast<int>(seed));
    m_randomGenerator->SaveModularLevelToStream(out);
    delete m_randomGenerator;
    m_randomGenerator = nullptr;
    return 0;
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
// PARTIAL: the listener row is Arrays::Listeners (stride 28), read once through a function-local cache (a guard
// variable). Its rows have no typed declaration in pydata-accessors. The listener position and orientation come from the
// local player (PlayerInfo +0x660, dh2/PlayerInfo.hpp absent), from the camera (CameraBase::GetCameraLookAtVec and
// GetCameraUpVec, declared), or from the screen centre (GetWorldPosFromScreenPos, declared, Width_Screen and Height_Screen,
// declared in dh2/global/MenuManagerGlobals.hpp). The VoxSoundManager::SetListenerPos call is not made, so the listener is
// not updated.
void Level::UpdateListener() {
}

// 0x3f1008 _ZN5Level11_LoadCameraEv
// PARTIAL: waits on dh2/CameraLevel.hpp, dh2/CameraOverview.hpp and dh2/LevelConfig.hpp (none written yet). The original
// allocates CameraOverview (operator new(40)) and CameraLevel (operator new(168)) into m_cameraOverview (+0x12c) and m_camera
// (+0x128). It then loads the camera and its animation set from the LevelConfig strings at +0x260, +0x278 and +0x290,
// sets the field of view and aspect from LevelConfig ints at +0x294 and +0x298, makes the camera active, plays the animation,
// targets the local player, sets the zoom handler's camera, and adds the sky box from the LevelConfig name at +0x248.
// Only the configured-null asserts are present in the pseudocode. Nothing is created here.
void Level::_LoadCamera() {
}

// 0x3f1394 _ZNK5Level15GetMPGameScriptEv
// PARTIAL: waits on dh2/LevelConfig.hpp. The original returns the LevelConfig int at +0x314 (788). The m_levelConfig assert
// is reproduced by GetLevelConfig(), which the original repeats inline with the same message and the same abort.
int Level::GetMPGameScript() const {
    LevelConfig* config = GetLevelConfig();
    (void)config;
    return 0;
}

// 0x3f1430 _ZN5Level9EnableFogEPN6glitch5scene10ISceneNodeE
// PARTIAL: waits on dh2/LevelConfig.hpp (fog start and end floats at +0x1d8 and +0x1dc, fog position at +0x1e0) and on the
// scene manager. The original reads that from the IDevice +0x1c field (glitch::IDevice does not declare it). Then
// SceneManager::EnableFog(start, end, position, node) is called. Nothing is changed here.
void Level::EnableFog(glitch::scene::ISceneNode* node) {
    LevelConfig* config = GetLevelConfig();  // the same m_levelConfig assert as the original
    (void)config;
    (void)node;
}

// 0x3f150c _ZN5Level14SetLevelConfigEP11LevelConfig
// PARTIAL: stores m_levelConfig (+0x38), as the original does first. The three sound ids (+0x11c music, +0x120, +0x124
// ambience) come from LevelConfig name strings (config fields around +0x17c, +0x1c0 and +0x190) through
// Arrays::GetMemberIDByString<Arrays::Sounds>. That template is not declared (pydata-accessors has only the non-template
// GetMemberIDByString), so the ids are left unchanged. dh2/LevelConfig.hpp is also absent.
void Level::SetLevelConfig(LevelConfig* config) {
    m_levelConfig = config;  // +0x38
}

// 0x3f16c0 _ZN5Level16UpdateDynamicFogEv
// PARTIAL: waits on dh2/LevelConfig.hpp. The original checks two LevelConfig words (+0x204 and +0x208) and, when they
// differ, takes the dynamic fog colour at the current camera-target position from ObjectManager::GetDynamicFogColorAtPos
// (declared to return the colour by value, the pseudocode passes an output pointer), with the radius at +0x214. It then
// sets the global material fog colour and writes the result into the scene manager's material slot. The material parameter
// setter is not declared, so this update is not made.
void Level::UpdateDynamicFog() {
}

// 0x3f2304 _ZN5Level9UpdateFogEPN6glitch5scene10ISceneNodeE
// PARTIAL: the choice between EnableFog(node) and DisableFog(node) reads LevelConfig +0x1dc (476), and dh2/LevelConfig.hpp
// is not written yet. Both original branches first read the "isTracingLevel" debug switch, so that read is reproduced
// here. The result is unused, as in the original.
void Level::UpdateFog(glitch::scene::ISceneNode* node) {
    if (!m_levelConfig) {
        return;
    }
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingLevel"));
    (void)node;
}

// 0x3f240c _ZN5Level14_LoadBatchInitEv
// PARTIAL: the original first calls a virtual method of the scene manager (its glitch::scene::CSceneManager base, vtable
// +0x60, slot 24, with arguments 0 and 0). That slot has no name in the tree, so the call is not made. The BatchNodeCompiler
// constructor is not declared (dh2/batch/BatchNodeCompiler.hpp declares only the destructor), so the new object is default
// constructed and the original's init of its vtable and list sentinels is not reproduced. The switch test, the destruction
// of the old compiler, and the allocation are reconstructed.
void Level::_LoadBatchInit() {
    DebugSwitches::GetInstance()->load();
    if (!DebugSwitches::GetInstance()->GetSwitch(std::string("IsDisablingCompiledBatching"))) {
        delete m_batchNodeCompiler;
        m_batchNodeCompiler = new batch::BatchNodeCompiler();
    }
}

// 0x3f254c _ZNK5Level4DrawEv
// PARTIAL: the floor-box switch is read and its result is discarded, as in the original. The path-node drawing waits on
// the local player's character (PlayerInfo +0x660, dh2/PlayerInfo.hpp absent), on the physics world's debug draw
// (Singleton<PFWorld>::s_inst is not declared), and on the scene draw slot of the IDraw interface (vtable +0x1c, not named).
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
// PARTIAL: waits on dh2/LevelConfig.hpp (the batch buffer sizes at +0x2a0 .. +0x2b4, with a 1.02 margin on each size) and
// on the scene manager. The original sets the scene manager flag at +0x43c around the batch compile and reads the
// clearRenderLists() result as a Device pointer (Device::IsHighPerformance), but the header declares clearRenderLists as void.
// The buffer-size writes, the flag writes and BatchNodeCompiler::Compile are not made. The IsDisablingCompiledBatching gate
// is reconstructed.
void Level::_LoadBatching() {
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch(std::string("IsDisablingCompiledBatching"))) {
        return;
    }
}

// 0x3f2d38 _ZN5Level14_LoadBatchListEv
// PARTIAL: the original walks the ObjectManager object tree (a tree header at +0x0c, see the conflict below) and adds
// each object that passes the name filters ("Module", "Decor", "Player", "Character", the container, door, trap and
// AnimatedDecor types) to the batch compiler list. The GameObject fields used (the type name at +0x5c, the disabled flag
// at +0x83, and the object's string at +0x44) and the list push are not declared. Also, dh2/ObjectManager.hpp declares
// m_objects as a std::list at +0x04, which conflicts with the walked tree at +0x0c. That conflict is not resolved. Only the
// IsDisablingCompiledBatching gate is reconstructed.
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
// PARTIAL, three seams:
//   1. The Arrays::LevelList scan (72-byte rows, with name at +0x20, subId at +0x10 and isRandom at +0x14) has no typed row
//      in pydata-accessors. m_levelListIndex, m_levelListSubId and m_isRandomLevel therefore stay at their defaults, and the
//      LevelSavegame creation below never runs.
//   2. The online check (GetOnline()->isOnlineGame_ at +0x05, OnlineSingleton<OnlineGameState>, CMatching and
//      CMatchingGLLive are not declared) is skipped, so m_resume is not cleared for a joining non-host player.
//   3. Module::s_moduleId = 0 waits on dh2/Module.hpp (absent).
// Everything else (the field defaults, the LuaScript data directory and its two loads, and the savegame flags) is rebuilt.
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

    // SEAM 1: the Arrays::LevelList scan (sets m_levelListIndex, m_levelListSubId, m_isRandomLevel) is not reconstructed.
    // SEAM 2: the online check (may clear m_resume) is not reconstructed.
    // SEAM 3: the original also sets Module::s_moduleId = 0. dh2/Module.hpp is absent, so that store is not made.

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
// PARTIAL: waits on dh2/LevelConfig.hpp. The original's last step passes the LevelConfig std::string at +0x150 (the
// pseudocode's (std::string*)config + 14, 24 bytes each) to _LoadScriptFile, which rewrites it. That call is not made.
// The global script loads and the m_levelConfig assert are reconstructed.
void Level::_LoadScripts() {
    ScriptManager::s_inst.UnLoadAllScripts();
    ScriptManager::s_inst.LoadScriptFile("data/pydata/scripts_pyscripts.bin", true);
    ScriptManager::s_inst.LoadScriptFileNames("data/pydata/scripts_pyscriptnames.bin", true);
    LevelConfig* config = GetLevelConfig();  // the same m_levelConfig assert as the original
    (void)config;
    // SEAM: _LoadScriptFile(config's name string at +0x150) is not called.
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
// stops with a null store (here std::abort), and level 1 prints the assert text. The tile's grid position (Tile +0x84,
// +0x88 and +0x8c) and the two child name ranges (Tile +0x60..+0x64 and +0x78..+0x7c) are not declared in
// dh2/rnd/Tile.hpp. So the m_moduleOffset update and the mgp and mvp module loads are skipped. Reconstructed: the module
// id, the mvx and mgx paths (gameplay/visual folder plus block name, from Block +0x1c and +0x04), the two module loads,
// the zeroed module offset, and the recursion over the children (m_childCount and m_children).
int Level::LoadTile(rnd::Tile* tile, int moduleId) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false",
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Level\\Level.cpp", 3269);
    }
    SetObjectModuleId(moduleId);

    rnd::Block* block = tile->m_block;
    std::string mvxPath = block->m_gameplay + "/mvx/" + block->m_name + ".mvx";
    std::string mgxPath = block->m_gameplay + "/mgx/" + block->m_name + ".mgx";
    // SEAM: the grid position and the mgp and mvp paths are not reconstructed (see above).

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
