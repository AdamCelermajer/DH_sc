// Batch g1-game-world-Level-7c7f5d-0: 20 Level methods (loading entry points and load-step flags, the
// object module id, the config and referee queries, the savegame flag, and the scene-manager forwarders).
// Declarations come from dh2/Level.hpp. A PARTIAL body names the owner header it waits on.

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/Character.hpp"
#include "dh2/DialogMsg.hpp"
#include "dh2/Level.hpp"
#include "dh2/LevelSavegame.hpp"
#include "dh2/MenuMessageManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764 (ObjectManager*)

// 0x3ef200 Level::_SpawnLoadingThread()
void Level::_SpawnLoadingThread() {
}

// 0x3ef204 Level::_CheckLoadProcess()
void Level::_CheckLoadProcess() {
}

// 0x3ef208 Level::_ThreadLoadingProcess()
void Level::_ThreadLoadingProcess() {
}

// 0x3ef20c Level::LoadScriptFile()
bool Level::LoadScriptFile() {
    return true;
}

// 0x3ef214 Level::_LoadResumeBatching()
void Level::_LoadResumeBatching() {
}

// 0x3ef218 Level::Load()
void Level::Load() {
    m_loadStep = 0;  // +0x130
}

// 0x3ef224 Level::ResetIsLoaded()
void Level::ResetIsLoaded() {
    m_isLoaded = false;  // +0x144
    m_loadStep = 0;      // +0x130
}

// 0x3ef234 Level::UpdateMaterial(bool, bool)
void Level::UpdateMaterial(bool, bool) {
}

// 0x3ef238 Level::SG_BlockLevel(bool)
// The original stores the flag at LevelSavegame +0x39 (LevelSavegame::m_blocked) when m_levelSavegame (+0xec) is set.
void Level::SG_BlockLevel(bool blocked) {
    if (m_levelSavegame) {
        m_levelSavegame->m_blocked = blocked;  // LevelSavegame +0x39
    }
}

// 0x3ef248 Level::Draw2D() const
void Level::Draw2D() const {
}

// 0x3ef24c Level::hasActiveDialog() const
// The original compares the start and finish words of queue 0 of MenuMessageManager<DialogMsg,1>::s_inst (0x99fb8c
// and 0x99fb9c, the deque's +0x04 and +0x14). Those words are the deques' current-element pointers, so the test is
// the deque's non-empty state.
bool Level::hasActiveDialog() const {
    return !MenuMessageManager<DialogMsg, 1>::s_inst.m_queues[0].empty();
}

// 0x3ef278 Level::SetObjectModuleId(int)
void Level::SetObjectModuleId(int id) {
    m_objectModuleId = id;  // +0x18c
}

// 0x3ef280 Level::UpdateLightSet(bool, int)
void Level::UpdateLightSet(bool, int) {
}

// 0x3ef3fc Level::CleanUpAllSkills()
// PARTIAL: the walk is in place: ObjectManager m_characters (+0x60, std::list<Character*>) in list order. For each element
// the original calls Character::UnLoadScriptProcess(element, it, true), where the iterator it is the value &Character::s_concurrentAI
// (the map object's own address, stored in v5). A std::map iterator cannot be made from that address on the host, so the call is
// left out (SEAM below). Its result is unused.
void Level::CleanUpAllSkills() {
    for (Character* character : dword_99F764->m_characters) {
        // SEAM: Character::UnLoadScriptProcess(character, <iterator at &Character::s_concurrentAI>, true) is not called.
        (void)character;
    }
}

// 0x3ef460 Level::UnlockAllObjects()
// For each element of ObjectManager m_characters (+0x60) whose ObjectBase::m_lockCount (+0x29) is non-zero, the count is cleared.
void Level::UnlockAllObjects() {
    for (Character* character : dword_99F764->m_characters) {
        if (character->m_lockCount) {
            character->m_lockCount = 0;  // ObjectBase +0x29
        }
    }
}

// 0x3ef4a8 Level::GetLevelConfig() const
// The original assert at level 2 stores 0 to address 0, which faults. std::abort() ends the process the same way.
// The level-1 message goes to stderr (the original's &__sF + 2, the third standard stream).
LevelConfig* Level::GetLevelConfig() const {
    LevelConfig* result = m_levelConfig;  // +0x38
    if (!result) {
        if (gAssertLevel == 2) {
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "m_levelConfig != 0",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources/Game/Level/Level.h", 476);
        }
    }
    return result;
}

// 0x3ef540 Level::GetMPReferee() const
// The cached m_mpReferee (+0x154) is returned when set. Otherwise the original scans ObjectManager m_characters (+0x60)
// for the first element whose name starts with "referee" (strncmp over 7 bytes), caches it and returns it. The name is
// ObjectBase::m_name (std::string at +0x30). The original reads its data pointer at element +0x44, which is the string's
// data word (+0x14 of the string), so the host m_name.c_str() is the same name.
GameObject* Level::GetMPReferee() const {
    if (!m_mpReferee) {  // +0x154
        for (Character* character : dword_99F764->m_characters) {
            if (character && std::strncmp(character->m_name.c_str(), "referee", 7) == 0) {
                m_mpReferee = character;
                return character;
            }
        }
    }
    return m_mpReferee;
}

// 0x3ef5cc Level::DisableFog(glitch::scene::ISceneNode*)
// PARTIAL: waits on dh2/SceneManager.hpp. The original tail-calls SceneManager::DisableFog(sm, node), where sm is
// read from the Application singleton (the object at 0x99f72c): Application::m_device (+0x10), then its +0x1c word.
// glitch::IDevice does not declare that +0x1c field yet, so the call is not made.
void Level::DisableFog(glitch::scene::ISceneNode* node) {
    (void)node;
}

// 0x3ef5f0 Level::UpdateLightSet(int, std::vector<bool>&, glitch::scene::ISceneNode*)
// PARTIAL: the same forwarder as DisableFog. The original tail-calls SceneManager::UpdateLightSet(sm, index,
// visited, node) with the scene manager found as above. Waits on dh2/SceneManager.hpp and the IDevice +0x1c field.
void Level::UpdateLightSet(int index, std::vector<bool>& visited, glitch::scene::ISceneNode* node) {
    (void)index;
    (void)visited;
    (void)node;
}

// 0x3ef614 Level::GetSpawnPoint()
// PARTIAL: waits on dh2/ObjectListItem.hpp (absent). The original walks ObjectManager's object map (m_objectMap, the
// tree header at +0x0c, in-order from the leftmost node at +0x14) and returns the first object whose node flag (+0x2c)
// is set, whose object handle resolves to a GameObject with m_goId (ObjectBase +0xf4) 13, m_enabled (+0x8a) set, and
// m_entryPointId (GameObject +0x374) equal to Level::m_entryPointId (+0x110). ObjectManager::m_objectMap is typed with
// ObjectListItem, which is not declared, so the node flag and handle cannot be read. Null until it is declared.
GameObject* Level::GetSpawnPoint() {
    return nullptr;
}
