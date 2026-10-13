// Batch g1-game-character-Character-ee9946-14 (Character, part 15 of 20): two Lua bindings (_MoveTo, _WarpTo) and
// eighteen savegame accessors (SG_*). Declarations are in dh2/Character.hpp, and the PlayerSavegame members come from
// dh2/PlayerSavegame.hpp.
//
// The SG_* accessors are thin wrappers over m_savegame (the PlayerSavegame pointer at Character +0x14e8). Without a
// savegame each one does nothing, or returns -1, false or null. With one, it reads or writes one PlayerSavegame member
// (offsets in the comments). Two points:
//   - SG_SetLevelId and SG_SetLevelEntryPoint take a difficulty. -1 means the static PlayerSavegame::m_difficultyLevel
//     (the difficulty the game runs at). SG_SetUseSpawnPoint always uses the static difficulty.
//   - SG_GetPlayerName returns the word at PlayerSavegame +0x2c (asm 0x3bb7e8: LDR R0, [R0,#0x2C]). That word is the
//     text pointer of the name, which sits 20 bytes into the 24-byte m_name at +0x18. PlayerSavegame::SG_GetPlayerName
//     (slot, 0x468470) reads the same word with strlen and copies the characters. The reconstruction returns
//     m_name.c_str(), the host string's text.
//
// Status: the 18 SG_* accessors are reconstructed. _MoveTo and _WarpTo are partial. Their argument checks use the
// sfc::script::lua::Arguments and Value accessors, and those headers do not exist in this tree.

#include "dh2/Character.hpp"
#include "dh2/PlayerSavegame.hpp"

// 0x3bb728 _ZN9Character10SG_GetSlotEv
// Returns PlayerSavegame +0x04 (m_slot), or -1 without a savegame.
int Character::SG_GetSlot() {
    if (m_savegame) return static_cast<int>(m_savegame->m_slot);
    return -1;
}

// 0x3bb740 _ZN9Character10SG_SetSlotEj
// Stores the slot in PlayerSavegame +0x04 (m_slot).
void Character::SG_SetSlot(unsigned int slot) {
    if (m_savegame) m_savegame->m_slot = slot;
}

// 0x3bb754 _ZN9Character12SG_SetPlayerEPS_
// Writes the owning Character to three places in the savegame: the quest log that is the volatile one (+0x174, the
// m_owner word at the end of m_volatileQuestLog), m_player (+0x10), and the quest log (+0x114, the m_owner word at the
// end of m_questLog). The order is the one in the Hex-Rays listing.
void Character::SG_SetPlayer(Character* player) {
    if (m_savegame) {
        m_savegame->m_volatileQuestLog.m_owner = player;
        m_savegame->m_player = player;
        m_savegame->m_questLog.m_owner = player;
    }
}

// 0x3bb770 _ZN9Character8SG_BlockEb
// Stores the flag in PlayerSavegame +0x0c (m_saveBlocked). SG_Save skips the write while it is set.
void Character::SG_Block(bool blocked) {
    if (m_savegame) m_savegame->m_saveBlocked = blocked;
}

// 0x3bb784 _ZN9Character12SG_IsBlockedEv
// Returns PlayerSavegame +0x0c (m_saveBlocked), or false without a savegame.
bool Character::SG_IsBlocked() {
    if (m_savegame) return m_savegame->m_saveBlocked;
    return false;
}

// 0x3bb798 _ZN9Character19SG_SetUseSpawnPointEb
// Stores the flag in the byte of PlayerSavegame +0x4c for the static difficulty (m_useSpawnPoint[difficulty], which is
// at +0x4c + difficulty). The Hex-Rays listing returns the constant 6812 in this branch. The void signature drops it.
void Character::SG_SetUseSpawnPoint(bool use) {
    if (m_savegame) m_savegame->m_useSpawnPoint[PlayerSavegame::m_difficultyLevel] = use;
}

// 0x3bb7d0 _ZN9Character16SG_RedoQuestSyncEv
// Clears PlayerSavegame +0x14 (m_questSynced), so the next quest sync is redone.
void Character::SG_RedoQuestSync() {
    if (m_savegame) m_savegame->m_questSynced = false;
}

// 0x3bb7e8 _ZNK9Character16SG_GetPlayerNameEv
// Returns the player name text, which the original reads from the word at PlayerSavegame +0x2c (see the file comment).
// Returns null without a savegame.
const char* Character::SG_GetPlayerName() const {
    if (m_savegame) return m_savegame->m_name.c_str();
    return nullptr;
}

// 0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv
// Returns PlayerSavegame +0x34 (m_playerClass), or -1 without a savegame.
int Character::SG_GetPlayerClass() const {
    if (m_savegame) return m_savegame->m_playerClass;
    return -1;
}

// 0x3bb814 _ZN9Character17SG_SetPlayerClassEi
// Stores the class index in PlayerSavegame +0x34 (m_playerClass).
void Character::SG_SetPlayerClass(int playerClass) {
    if (m_savegame) m_savegame->m_playerClass = playerClass;
}

// 0x3bb828 _ZN9Character17SG_GetPlayerLevelEv
// Returns PlayerSavegame +0x30 (m_playerLevel), or -1 without a savegame.
int Character::SG_GetPlayerLevel() {
    if (m_savegame) return m_savegame->m_playerLevel;
    return -1;
}

// 0x3bb840 _ZN9Character17SG_SetPlayerLevelEi
// Stores the level in PlayerSavegame +0x30 (m_playerLevel).
void Character::SG_SetPlayerLevel(int playerLevel) {
    if (m_savegame) m_savegame->m_playerLevel = playerLevel;
}

// 0x3bb854 _ZN9Character13SG_SetLevelIdEii
// Stores levelId in the int at PlayerSavegame +0x50 + 4 * difficulty. difficulty -1 means the static difficulty
// (Hex-Rays: "a3 == -1" selects PlayerSavegame::m_difficultyLevel + 20 in 4-byte units). The array is unk_50 in the
// PlayerSavegame header, whose comment says these functions never write it: a layout request asks for a rename.
void Character::SG_SetLevelId(int levelId, int difficulty) {
    if (m_savegame) {
        const int d = (difficulty == -1) ? PlayerSavegame::m_difficultyLevel : difficulty;
        m_savegame->unk_50[d] = levelId;
    }
}

// 0x3bb89c _ZN9Character21SG_SetLevelEntryPointEii
// Stores entry in PlayerSavegame +0x40 + 4 * difficulty (m_levelEntryPoint[difficulty]). difficulty -1 means the static
// difficulty, as in SG_SetLevelId.
void Character::SG_SetLevelEntryPoint(int entryPoint, int difficulty) {
    if (m_savegame) {
        const int d = (difficulty == -1) ? PlayerSavegame::m_difficultyLevel : difficulty;
        m_savegame->m_levelEntryPoint[d] = entryPoint;
    }
}

// 0x3bb8e4 _ZN9Character20SG_GetGameDifficultyEv
// Returns the static difficulty (PlayerSavegame::m_difficultyLevel) when there is a savegame, or -1 without one.
int Character::SG_GetGameDifficulty() {
    if (m_savegame) return PlayerSavegame::m_difficultyLevel;
    return -1;
}

// 0x3bb918 _ZN9Character28SG_GetGameDifficultyUnlockedEv
// Returns PlayerSavegame +0x3c (m_unlockedDifficulty), or -1 without a savegame.
int Character::SG_GetGameDifficultyUnlocked() {
    if (m_savegame) return m_savegame->m_unlockedDifficulty;
    return -1;
}

// 0x3bb930 _ZN9Character28SG_SetGameDifficultyUnlockedEi
// Raises PlayerSavegame +0x3c (m_unlockedDifficulty) to d when d is higher. It never lowers it.
void Character::SG_SetGameDifficultyUnlocked(int d) {
    if (m_savegame && d > m_savegame->m_unlockedDifficulty) m_savegame->m_unlockedDifficulty = d;
}

// 0x3bb950 _ZN9Character20SG_SetGameDifficultyEi
// Sets the static difficulty (PlayerSavegame::m_difficultyLevel) to d, but only when there is a savegame and d is not
// above its unlocked difficulty (+0x3c).
void Character::SG_SetGameDifficulty(int d) {
    if (m_savegame && d <= m_savegame->m_unlockedDifficulty) PlayerSavegame::m_difficultyLevel = d;
}

// 0x3bada8 _ZN9Character7_MoveToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. The binding takes the argument list (args), a ReturnValues that it does not use, and the
// Character (userData, the self of the controller). Dispatch from the Hex-Rays listing, with the count test checked
// against the asm:
//   - One argument whose Lua tag is 2 (light userdata) or 7 (userdata): Cmd_MoveTo(self->m_controller, target), where
//     target = getUserData(args[0]). This is v2Controller::Cmd_MoveTo(GameObject*) (0x405540).
//   - One argument of any other tag, or two arguments: nothing happens (count 2 returns at once).
//   - Three or more arguments, where args[0], args[1] or args[2] has tag 3 (number): if there is a fourth argument with
//     tag 1 (boolean) and it is true, the point is self->m_position (+0x160) moved in the look-at frame, using
//     GameObject::GetLookAtVec(self) and the constants dword_99F87C, dword_99F880 and Vec3f_K. The exact combination
//     is not recovered (the Hex-Rays look vector is split across variables that do not match its layout). Otherwise the
//     point is (getNumber(args[0]), getNumber(args[1]), getNumber(args[2])). Then Cmd_MoveTo(self->m_controller, &point)
//     (v2Controller::Cmd_MoveTo(const Point3D&), 0x4054e4).
// Missing: sfc::script::lua::Arguments (operator[] and the argument count), Value (the type tag at +4, getNumber,
// getBool, getUserData). None of these has a header in this tree (the game-app category owns the wrapper). The tags
// are the vendor-lua LUA_T* values (1 boolean, 2 light userdata, 3 number, 7 userdata). The bindings stay empty until
// those headers exist. The controller offset is self +0x378 (v2Controllable::m_controller).
void Character::_MoveTo([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}

// 0x3bb268 _ZN9Character7_WarpToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// PARTIAL. Not written. Same dispatch as _MoveTo (see its comment), with these differences from the asm:
//   - One-argument case: Cmd_WarpTo(self->m_controller, &target->m_position), where target = getUserData(args[0]). The asm
//     at 0x3bb6f8 passes the GameObject's m_position (+0x160) after the getUserData call (ADD R1, R0, #0x160). The
//     Hex-Rays listing drops that argument and the result of getUserData.
//   - Point case: Cmd_WarpTo(self->m_controller, &point). The asm at 0x3bb39c passes the stack copy of the point. The
//     Hex-Rays listing drops that argument too.
//   - The look-at offset is the same frame as _MoveTo (Hex-Rays only, not checked against the asm).
// v2Controller::Cmd_WarpTo(const Point3D&) (0x405318) is the only WarpTo overload, and it covers both calls.
// Missing: the Arguments and Value accessors, as for _MoveTo. The bindings stay empty until those headers exist.
void Character::_WarpTo([[maybe_unused]] const sfc::script::lua::Arguments& args,
                        [[maybe_unused]] sfc::script::lua::ReturnValues& ret, [[maybe_unused]] void* userData) {}
