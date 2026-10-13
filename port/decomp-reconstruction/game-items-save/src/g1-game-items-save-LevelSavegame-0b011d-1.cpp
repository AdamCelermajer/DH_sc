// Batch g1-game-items-save-LevelSavegame-0b011d-1 (owner LevelSavegame, part 2 of 2: ValidateCheckpoint).
// Reconstructed from the Hex-Rays pseudocode and the assembly of libDungeonHunter2.so (0x463288, 604 bytes per the
// symbol index).
//
// Notes:
//  * Hex-Rays types the first int argument as LevelSavegame*. The assembly passes it in r1 as the level id to
//    GetCheckpointFilename and GetFilename (both take unsigned int levelId), so it is written as levelId.
//  * The fallback name is name + ".bak". The clone of std::operator+ that the original calls appends that literal
//    (checked in its disassembly, _ZStplIcSt11char_traitsIcESaIcEESbIT_T0_T1_ERKS6_PKS3_.clone.1).
//  * PARTIAL. Two inputs are not declared in the tree, so each has a stub below:
//      - the online flag, COnline +0x05 (isOnlineGame_), which is private with no accessor;
//      - the savefile existence check, vtable +0xb0 of the file system at IDevice +0x34, which IDevice does not declare.
//    Both stubs make the function return false at once. The rest follows the original's control flow.
//  * PlayerManager::m_hostChanged (+0x719) is read as the multiplayer test while the local player is hosting. It keeps
//    the name the owner header gives it.

#include <string>

#include "dh2/LevelSavegame.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Savegame.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"
#include "dh2/global/net_singletons.hpp"

namespace {

// PARTIAL stub for *(GetOnline() + 5), the COnline flag isOnlineGame_ (private, no getter; layout request). It reads as
// offline, so ValidateCheckpoint takes the single-player checkpoint name.
bool OnlineGameFlag(const COnline* online) {
    (void)online;
    return false;
}

// PARTIAL stub for the savefile existence check. The original calls vtable +0xb0 of the file system at IDevice +0x34
// with the checkpoint name and with name + ".bak". That slot is FileSystemBase::doesSavefileExists (0x34e150) in the
// FileSystemWin32 vtable. IDevice does not declare +0x34 (layout request), so no file is reported as present.
bool DeviceDoesSavefileExists(const char* name) {
    (void)name;
    return false;
}

}  // namespace

// 0x463288 _ZN13LevelSavegame18ValidateCheckpointEiii
// PARTIAL: the online flag and the savefile existence check are stubs (see the anonymous namespace above).
bool LevelSavegame::ValidateCheckpoint(int levelId, int fieldA, int fieldB) {
    std::string name;

    // Multiplayer test: online, then hosting. A hosting local player uses the PlayerManager flag at +0x719. Any other
    // online case is multiplayer. Offline is single-player.
    bool multiplayer = false;
    if (OnlineGameFlag(GetOnline())) {
        if (dword_99F76C->IsLocalPlayerHosting()) {
            multiplayer = dword_99F76C->m_hostChanged;  // +0x719
        } else {
            multiplayer = true;
        }
    }
    GetCheckpointFilename(static_cast<unsigned int>(levelId), m_difficulty, multiplayer, name);  // m_difficulty +0x0c

    // Use the checkpoint file when it exists. Otherwise use its ".bak" copy. With neither, nothing changes.
    if (DeviceDoesSavefileExists(name.c_str())) {
        m_savegame->m_filename = name;
    } else {
        const std::string backup = name + ".bak";
        if (!DeviceDoesSavefileExists(backup.c_str())) {
            return false;
        }
        m_savegame->m_filename = backup;
    }
    m_savegame->_cacheFile(nullptr);
    m_savegame->load("INFO", &LevelSavegame::__LoadLevelInfo, &LevelSavegame::__SaveLevelInfo, this);

    // The result compares the file index read from the INFO section (+0x2c) with fieldB. Then the level's own file name
    // replaces the checkpoint name in the savegame.
    const bool matches = m_loadedFileIndex == fieldB;
    GetFilename(static_cast<unsigned int>(levelId), fieldA, fieldB, m_difficulty, name);
    m_savegame->m_filename = name;
    m_savegame->_cacheFile(nullptr);
    return matches;
}
