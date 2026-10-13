// Batch g1-game-world-Level-7c7f5d-0: 20 Level methods (loading entry points and load-step flags, the
// object module id, the config and referee queries, the savegame flag, and the scene-manager forwarders).
// Declarations come from dh2/Level.hpp. A PARTIAL body names the owner header it waits on.

#include <cstdio>
#include <cstdlib>

#include "core-platform/memory.hpp"  // gAssertLevel
#include "dh2/Level.hpp"

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
// PARTIAL: waits on dh2/LevelSavegame.hpp. The original stores the flag at LevelSavegame +0x39 when
// m_levelSavegame (+0xec) is set. That store is left out until the LevelSavegame layout is declared.
void Level::SG_BlockLevel(bool blocked) {
    if (m_levelSavegame) {
        (void)blocked;
    }
}

// 0x3ef248 Level::Draw2D() const
void Level::Draw2D() const {
}

// 0x3ef24c Level::hasActiveDialog() const
// PARTIAL: waits on dh2/MenuMessageManager.hpp. The original returns whether the static DialogMsg queue of
// MenuMessageManager<DialogMsg,1> is non-empty: its deque begin word at 0x99fb8c differs from the finish word
// at 0x99fb9c. Placeholder false until that queue is declared.
bool Level::hasActiveDialog() const {
    return false;
}

// 0x3ef278 Level::SetObjectModuleId(int)
void Level::SetObjectModuleId(int id) {
    m_objectModuleId = id;  // +0x18c
}

// 0x3ef280 Level::UpdateLightSet(bool, int)
void Level::UpdateLightSet(bool, int) {
}

// 0x3ef3fc Level::CleanUpAllSkills()
// PARTIAL: waits on dh2/ObjectManager.hpp. The original walks the ObjectManager list at +0x60 (node +0 next,
// +8 element) and calls Character::UnLoadScriptProcess(element, &it, true) for each element, with the iterator
// it holding the address &Character::s_concurrentAI. The iterator is not modelled as a std::map iterator yet.
void Level::CleanUpAllSkills() {
}

// 0x3ef460 Level::UnlockAllObjects()
// PARTIAL: waits on dh2/ObjectManager.hpp (the same list at ObjectManager +0x60 as CleanUpAllSkills). For each
// element whose ObjectBase::m_lockCount (+0x29) is non-zero, the count is cleared.
void Level::UnlockAllObjects() {
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
// PARTIAL: waits on dh2/ObjectManager.hpp for the search. The original scans the ObjectManager list at +0x60 for
// the first element whose name (const char* at element +0x44) starts with "referee" (strncmp over 7 bytes), then
// caches it in m_mpReferee (+0x154). Only the cached read is reconstructed here, so a miss returns null.
GameObject* Level::GetMPReferee() const {
    GameObject* referee = m_mpReferee;  // +0x154
    return referee;
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
// PARTIAL: waits on dh2/ObjectManager.hpp for the scan. The original walks ObjectManager's object tree (a
// libstdc++-style _Rb_tree header at +0x0c, in-order from the leftmost node at +0x14) and returns the first object
// whose node flag (+0x2c) is set, whose m_goId (ObjectBase +0xf4) is 13, whose m_enabled (+0x8a) is set, and whose
// +0x374 word equals m_entryPointId (+0x110). Null until the scan is reconstructed.
GameObject* Level::GetSpawnPoint() {
    return nullptr;
}
