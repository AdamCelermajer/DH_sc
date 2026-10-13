// Batch g1-game-world-Level-7c7f5d-1: 20 Level methods (player save and lock queries, restart, the object-list scans
// of final init and skinning reset, character AI states, player spawn, the XML object loader, the load-data stream,
// the checkpoint load and save, quick save and unload). Declarations come from dh2/Level.hpp. A PARTIAL body names what
// it waits on; the notes on each function say which part is left out.
//
// dh2/Character.hpp is deliberately not included. It brings the pydata-accessors IStreamBase, which cannot share a
// translation unit with the net-transport StreamBuffer that Level.hpp embeds (both define class IStreamBase). Every
// call on a Character is therefore omitted, and the functions that make one are PARTIAL with the call named in the note.

#include <cstdio>
#include <cstdlib>

#include "core-platform/memory.hpp"               // gAssertLevel, CustomFree
#include "dh2/Application.hpp"
#include "dh2/GameEventManager.hpp"
#include "dh2/Level.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Random.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"       // dword_99F76C, the PlayerManager instance
#include "dh2/global/ObjectManagerInstance.hpp"   // dword_99F764, the ObjectManager instance
#include "dh2/global/ctrl_camera.hpp"             // ApplicationGetInstance()
#include "glitch-core-io/os/logging.hpp"          // glitch::os::Timer::getRealTime

namespace {

// Source file named in the original's assert messages (Level.cpp of the VS2005 project).
constexpr const char* kLevelSourceFile =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Level\\Level.cpp";

// The original ASSERT: level 2 faults (it stores to address 0), level 1 prints to stderr (the third standard stream,
// &__sF + 2). std::abort() ends the process the way the level-2 fault does.
void LevelAssertFailed(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kLevelSourceFile, line);
    }
}

// PARTIAL helper. The original reads PlayerManager::GetPlayer(friendlyId, true)->character_660 (PlayerInfo +0x660, the
// Character* of the record). dh2/PlayerInfo.hpp is not written yet (PlayerManager.hpp keeps only a pad for the record),
// so this returns null. When PlayerInfo is declared, replace the body with the member and the callers are complete.
Character* PlayerCharacterByFriendlyId(int friendlyId) {
    (void)friendlyId;
    return nullptr;
}

}  // namespace

// 0x3ef728 Level::SG_DBG_TraceDetailedQuestInformation(int) const
// PARTIAL: body not reconstructed. The original calls Character::SG_DBG_TraceDetailedQuestInformation(stream, questState,
// -1) for every player character (the stream is &_sF + 1, stdout). Two blockers: Character.hpp cannot share this
// translation unit (IStreamBase conflict, see the file comment), and that declaration takes __sFILE*, which is not the
// FILE* of this host (FILE is _iobuf here). Nothing is traced until both are resolved.
void Level::SG_DBG_TraceDetailedQuestInformation(int questState) const {
    (void)questState;
}

// 0x3ef7ac Level::SG_IsMapLocSeen(int) const
// PARTIAL: body not reconstructed. The original is true when any player character reports the location seen, testing
// characters in player order until one does (Character::SG_IsMapLocSeen, 0x3bbd30). That call needs Character.hpp (see
// the file comment), so the placeholder is false.
bool Level::SG_IsMapLocSeen(int location) const {
    (void)location;
    return false;
}

// 0x3ef850 Level::SG_IsMapLocLocked(int) const
// PARTIAL: body not reconstructed. The original is true when every player character reports the location locked (true
// with no players), through Character::SG_IsMapLocLocked(location, -1). That call needs Character.hpp. The placeholder
// is true, the value the original gives with no players.
bool Level::SG_IsMapLocLocked(int location) const {
    (void)location;
    return true;
}

// 0x3ef8f8 Level::SG_IsLevelLocked(int) const
// PARTIAL: body not reconstructed, the same as SG_IsMapLocLocked but through Character::SG_IsLevelLocked(level, -1).
// The placeholder is true.
bool Level::SG_IsLevelLocked(int level) const {
    (void)level;
    return true;
}

// 0x3ef9a0 Level::Restart()
// Application::LoadLevel with this level's own arguments: the name (the original reads the string's c_str at +0x10c),
// the entry point (+0x110), the savegame slot (+0x114), resume false, use spawn point true, and zero difficulty, remote
// flag, seed and synced seed.
void Level::Restart() {
    ApplicationGetInstance().LoadLevel(m_levelName.c_str(), m_entryPointId, m_savegameSlot, false, true, 0, false, 0, 0);
}

// 0x3efa54 Level::SG_SavePlayer(Character*,bool)
// PARTIAL: body not reconstructed. The original returns unless the character IsPlayer (vtable +0x28). For a player it
// reads SG_IsBlocked, lifts the block for a level change, sets the player level from GetLevel, stamps the save date,
// sets the entry point to m_entryPointId (+0x110) with -1, calls SG_Save and puts the block flag back. Every call
// is on Character, which needs Character.hpp (see the file comment).
void Level::SG_SavePlayer(Character* player, bool levelChange) {
    (void)player;
    (void)levelChange;
}

// 0x3efaec Level::SG_SavePlayer(int,bool)
// PARTIAL: the two asserts and the range check are reconstructed. The character is found through
// PlayerCharacterByFriendlyId, which waits on dh2/PlayerInfo.hpp, so no player is saved until that is declared. The
// save itself (the Character overload, 0x3efa54) is also partial.
void Level::SG_SavePlayer(int playerIndex, bool levelChange) {
    if (playerIndex < 0) {
        LevelAssertFailed("playerID >= 0", 2865);
    }
    if (playerIndex >= dword_99F76C->m_numCharacters) {
        LevelAssertFailed("playerID < pMgr->GetNumPlayerCharacters()", 2866);
    }
    if (playerIndex >= 0 && playerIndex < dword_99F76C->m_numCharacters) {
        SG_SavePlayer(PlayerCharacterByFriendlyId(playerIndex), levelChange);
    }
}

// 0x3efc40 Level::SG_SaveAllPlayer(bool)
// Saves every player index in turn, re-reading the player count each time as the original does.
void Level::SG_SaveAllPlayer(bool levelChange) {
    for (int i = 0; i < dword_99F76C->m_numCharacters; ++i) {
        SG_SavePlayer(i, levelChange);
    }
}

// 0x3efca4 Level::_LoadFinalInit()
// PARTIAL: body not reconstructed. The original walks dword_99F764's object map (std::map<int, ObjectListItem> at
// ObjectManager +0x0c) in key order. For each node whose ObjectListItem flag (node +0x2c) is set, it resolves the
// object handle (ObjectBase::GetHandle, then the ObjectHandle conversion to GameObject*) and calls
// GameObject::InitFinal() (vtable +0x58) when the object exists. Waits on dh2/ObjectListItem.hpp, which is not written.
void Level::_LoadFinalInit() {
}

// 0x3efd74 Level::ResetSkinning()
// PARTIAL: body not reconstructed. The original walks the same object map as _LoadFinalInit. For each node with the
// ObjectListItem flag (+0x2c) set, it takes the GameObject and, when its m_visualObject (+0x2d8) is set, calls
// VisualObject::ResetSkinning, then ApplyShadowMaterial and ApplyXrayMaterial on it. Waits on dh2/ObjectListItem.hpp.
void Level::ResetSkinning() {
}

// 0x3eff98 Level::_LoadCharStates()
// PARTIAL: body not reconstructed. The original walks ObjectManager::m_characters (+0x60) in list order. A character
// whose pre-set AI state (Character::GetPreSetAIState) is 0 (Limbus) or 17 (PreSpawn) goes to the pre-spawn state
// (CharStateMachine::SM_SetPreSpawnState); any other state goes to idle (SM_SetIdleState(false)). Every call is on
// Character, which needs Character.hpp (see the file comment).
void Level::_LoadCharStates() {
}

// 0x3f0018 Level::_LoadPlayer()
// PARTIAL: body not reconstructed. For each local player with a character (GetLocalPlayer(i, 1)->character_660, which
// waits on dh2/PlayerInfo.hpp), the original scans the object map in key order for the first object whose ObjectListItem
// flag (node +0x2c) is set, whose m_goId (ObjectBase +0xf4) is 13 (spawn point), that is enabled (+0x8a) and whose
// GameObject +0x374 equals m_entryPointId. When found and PlayerManager::m_initialSetupDone (+0x6d0) is clear, it calls
// SpawnPoint::PlaceObject(spawn, character), Character::SetInitialPosition(m_position) and
// Character::SG_SetLevelEntryPoint(m_entryPointId, -1). Blocked on dh2/PlayerInfo.hpp, dh2/ObjectListItem.hpp,
// dh2/SpawnPoint.hpp (none written yet), Character.hpp (see the file comment), and GameObject +0x374, which
// GameObject.hpp does not declare.
void Level::_LoadPlayer() {
}

// 0x3f01c8 Level::_LoadFromXML(TiXmlElement*)
// PARTIAL: the null-element assert is reproduced. The rest waits on two declarations: TiXmlElement::Attribute, which the
// tinyxml-and-c-libs TiXmlElement does not have (it keeps only Value()), for the "gametype" test; and an int return from
// ObjectManager::LoadFromXML(element, nullptr, m_moduleOffset, m_objectModuleId), which 0x34b868 returns as int and
// ObjectManager.hpp declares void. The original returns strcmp's result (0 for gametype "Player") or the load's result.
// The placeholder returns false.
bool Level::_LoadFromXML(TiXmlElement* element) {
    if (!element) {
        LevelAssertFailed("element", 2270);
    }
    return false;
}

// Level::LoadFileData constructor. It is the body of 0x3f02ac below, which allocates it (operator new(80) in the
// original) and stores it in m_loadFileData. The vtable pointer is set by the compiler. The StreamBuffer is built from
// the IStreamBase, since the StreamBuffer copy constructor is deleted in net-transport.
Level::LoadFileData::LoadFileData(IStreamBase* source)
    : m_stream(source),
      m_document(nullptr),
      m_root(nullptr),
      m_currentModule(nullptr),
      m_currentElement(nullptr),
      unk_48(false) {}

// 0x3f02ac Level::AssignSteamToLoadDataFile(StreamBuffer&)
void Level::AssignSteamToLoadDataFile(StreamBuffer& stream) {
    m_loadFileData = new LoadFileData(&stream);  // +0x140
}

// 0x3f0310 Level::LoadFileData::~LoadFileData() (D1)
// The original sets the vtable and destroys the StreamBuffer at +0x08. The member destructor does the second step.
Level::LoadFileData::~LoadFileData() {
}

// The deleting destructor (D0) of LoadFileData is compiler-generated from ~LoadFileData above, so it has no body here.
// The original's tail after the D1 body is CustomFree(this); this build frees through the default operator delete.

// 0x3f0428 Level::LoadCheckpoint()
// PARTIAL: the local player's checkpoint reload is omitted. The savegame part is reconstructed: the level savegame loads
// its checkpoint file (slot +0x114, sub id +0x40, index +0x3c), then the game event manager re-initialises. The original
// then takes GetLocalPlayer(0, 1)->character_660 (GetLocalPlayerCharacter, 0x330794), calls SG_LoadCheckpoint(16) and
// SG_Update(true) on it. Those calls need Character.hpp (see the file comment).
void Level::LoadCheckpoint() {
    m_loadingCheckpoint = true;  // +0xf4
    if (m_levelSavegame) {       // +0xec
        m_levelSavegame->LoadCheckPoint(static_cast<int>(m_savegameSlot), m_levelListSubId, m_levelListIndex);
        m_gameEventManager->ReInit();  // +0x194
    }
    m_loadingCheckpoint = false;
}

// 0x3f04b4 Level::CheckpointSave(Point3D<float> const&,bool)
// PARTIAL: body not reconstructed. The original, for the local player (GetLocalPlayerCharacter) at load step 34 or later
// with a savegame and a living character, writes m_savedPoint (+0x1468) from the position, writes m_revivePosition
// (+0x1474) from the position when usePosition is set or from the character's position (+0x160) otherwise, calls
// SG_SaveCheckpoint, and then has the savegame write its checkpoint file (SaveCheckPoint with slot +0x114, sub id +0x40,
// index +0x3c). The character accesses need Character.hpp (see the file comment).
void Level::CheckpointSave(const Point3D<float>& position, bool usePosition) {
    (void)position;
    (void)usePosition;
}

// 0x3f059c Level::QuickSave(bool)
// PARTIAL: body not reconstructed. The original, at load step 38 with a savegame and a living local player, copies the
// player's position (+0x160) to m_savedPoint (+0x1468) and saves the level savegame. The savegame's blocked flag
// (LevelSavegame +0x39) is cleared while Save runs for a level change, then restored. The save is also gated on the
// online flag: GetOnline()->+0x05 (COnline::isOnlineGame_, private with no getter in c_online.hpp), or hosting with
// PlayerManager +0x719 (m_hostChanged) clear. The player accesses need Character.hpp (see the file comment).
void Level::QuickSave(bool levelChange) {
    (void)levelChange;
}

// 0x3f0690 Level::Unload()
// PARTIAL: the camera deletes and VoxSoundManager::StopAllSounds(500) are left out. The original deletes m_camera (+0x128)
// and m_cameraOverview (+0x12c) through their virtual destructors and nulls both, and calls StopAllSounds(500) on
// VoxSoundManager::s_instance. dh2/CameraLevel.hpp, dh2/CameraOverview.hpp and dh2/VoxSoundManager.hpp are not written.
// The QuickSave step is the partial body above. The rest follows the original, including PlayerManager::Update() (its
// result is discarded) and the seed reset from glitch::os::Timer::getRealTime().
void Level::Unload() {
    QuickSave(m_levelChange);  // +0xf0
    MenuManager* menus = MenuManager::GetInstance();
    MenuBase* loading = menus->GetMenuByName("menu_Loading");
    menus->PushMenu(loading);
    // PARTIAL: delete m_camera and m_cameraOverview and null them (see above).
    SG_SaveAllPlayer(m_levelChange);
    // PARTIAL: VoxSoundManager::StopAllSounds(500) (see above).
    if (loading->IsVisible()) {
        menus->PopMenu(loading);
    }
    m_loadStep = 0;  // +0x130
    dword_99F764->NetworkUnInitLevel();
    dword_99F76C->unk_6c9 = 0;  // PlayerManager +0x6c9
    dword_99F76C->Update();
    Random::s_seed = glitch::os::Timer::getRealTime();
    Random::s_syncedSeed = 0;
}
