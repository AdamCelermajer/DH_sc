// Batch g1-game-app-pack5-057b13-0 (game-app). Ten script command classes, 20 ELF functions: the IsBlocking and
// Execute pair of Script_AwardEndGameTrophies, Script_AwardTrophy, Script_BlockSaveGame, Script_CONSOLE,
// Script_ChangeLevel, Script_CloseDoor, Script_DeactivateTriggerPlate, Script_DoTutorial, Script_DropLoot and
// Script_EmptyImpl.
//
// Rebuilt from the Hex-Rays pseudocode. The IsBlocking bodies and Script_EmptyImpl::Execute were checked against the
// assembly (tools/decomp_asm.py). Status per function is in the ledger rows.
//
// The Execute bodies marked PARTIAL share three gaps, each named again at its place below:
//   (a) the command's argument record, ScriptCmdImpl::m_args (void*). No type for the record is declared in the tree,
//       so the names, ids and flags the original reads from it cannot be named.
//   (b) the local player's character, PlayerManager::GetLocalPlayer(...)->character_660. PlayerInfo has no header
//       (dh2/PlayerInfo.hpp is absent), so its character_660 member cannot be read.
//   (c) the online flag *(GetOnline() + 5). COnline::isOnlineGame_ is private with no getter. The globals
//       lastOpenMenuID (ELF 0x9f63fc) and m_updateJobThread2 are not declared.
//   (d) dh2/Level.hpp and dh2/Character.hpp cannot be included in one translation unit: each pulls in an IStreamBase
//       with a different definition. Script_BlockSaveGame needs Level, the trophy helper needs Character, so the level
//       block of Script_BlockSaveGame is left out. The Arrays declarations below repeat the ones in
//       pydata-accessors/member_names.hpp, which cannot share a unit with Character either.
#include "dh2/Script_AwardEndGameTrophies.hpp"
#include "dh2/Script_AwardTrophy.hpp"
#include "dh2/Script_BlockSaveGame.hpp"
#include "dh2/Script_CONSOLE.hpp"
#include "dh2/Script_ChangeLevel.hpp"
#include "dh2/Script_CloseDoor.hpp"
#include "dh2/Script_DeactivateTriggerPlate.hpp"
#include "dh2/Script_DoTutorial.hpp"
#include "dh2/Script_DropLoot.hpp"
#include "dh2/Script_EmptyImpl.hpp"

#include <string>

#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Door.hpp"
#include "dh2/Quest.hpp"
#include "dh2/TrophyManager.hpp"

// Declarations of Arrays::TrophyTable and Arrays::GetMemberIDByString from pydata-accessors/member_names.hpp. The
// definitions stay there. The header itself is not included because it cannot share a translation unit with
// dh2/Character.hpp (both define pydata types, e.g. Structs::AIProps).
namespace Arrays {
struct TrophyTable;
template <class T>
int GetMemberIDByString(const char* name);
}  // namespace Arrays

namespace {

// DebugSwitches::load() on the singleton, then GetSwitch(name). The original passes &DebugSwitches::s_inst, which is
// private in dh2/DebugSwitches.hpp. DebugSwitches::GetInstance() calls load() on that same singleton and returns it
// (0x330740), so one GetInstance() call makes the same two steps. The switch value is returned to the caller.
bool ReadScriptSwitch(const char* name) {
    return DebugSwitches::GetInstance()->GetSwitch(std::string(name));
}

// The "isTracingScriptCmd" probe that the original runs at the start of several Execute bodies. The value is discarded.
void TraceScriptCmd() {
    (void)ReadScriptSwitch("isTracingScriptCmd");
}

// The unlocks of Script_AwardEndGameTrophies::Execute for one character, in the original's order. The character lookup
// is the part that is missing (see Script_AwardEndGameTrophies::Execute), so nothing calls this yet. It is complete
// otherwise: the class trophies, the quest completion walk and the three stat checks.
[[maybe_unused]] void UnlockEndGameTrophies(Character* character) {
    TrophyManager* trophies = TrophyManager::s_instance;
    const auto unlock = [trophies](const char* name) {
        trophies->UnlockTrophy(Arrays::GetMemberIDByString<Arrays::TrophyTable>(name));
    };

    // Class trophies. The original tests the class in an if chain and repeats the getter in every test.
    if (character->SG_GetPlayerClass() == 263) {
        unlock("quest_warriorwin");
    } else if (character->SG_GetPlayerClass() == 264) {
        unlock("quest_warriorwin");
        unlock("quest_berserkerwin");
    } else if (character->SG_GetPlayerClass() == 265) {
        unlock("quest_warriorwin");
        unlock("quest_paladinwin");
    } else if (character->SG_GetPlayerClass() == 325) {
        unlock("quest_roguewin");
    } else if (character->SG_GetPlayerClass() == 326) {
        unlock("quest_roguewin");
        unlock("quest_archerwin");
    } else if (character->SG_GetPlayerClass() == 327) {
        unlock("quest_roguewin");
        unlock("quest_assassinwin");
    } else if (character->SG_GetPlayerClass() == 290) {
        unlock("quest_magewin");
    } else if (character->SG_GetPlayerClass() == 291) {
        unlock("quest_magewin");
        unlock("quest_illusionistwin");
    } else if (character->SG_GetPlayerClass() == 292) {
        unlock("quest_magewin");
        unlock("quest_necromancerwin");
    }

    // Quest completion. allDone (v11) stays set while every quest except index 22 has state above 12. sideQuestDone
    // (v9) is set when a quest above state 12 is a secondary quest. The walk stops early once a secondary quest is done
    // and a main quest is not.
    const int numQuests = character->SG_GetNumQuests(-1);
    bool allDone = true;
    bool sideQuestDone = false;
    if (numQuests != 0) {
        int index = 0;
        bool stoppedEarly = false;
        for (;;) {
            if (character->SG_GetQuestByID(index, -1)->m_state <= 12 && index != 22) {
                allDone = false;
            }
            if (character->SG_GetQuestByID(index, -1)->m_state > 12) {
                Quest* quest = character->SG_GetQuestByID(index, -1);
                if (quest->IsSecondary()) {
                    sideQuestDone = true;
                }
            }
            ++index;
            if (!allDone && sideQuestDone) {
                stoppedEarly = true;
                break;
            }
            if (index == numQuests) {
                break;
            }
        }
        if (!stoppedEarly) {
            if (allDone) {
                unlock("quest_allcomplete");
            }
            if (!sideQuestDone) {
                unlock("completed_no_sidequests");
            }
        }
    } else {
        unlock("quest_allcomplete");
        unlock("completed_no_sidequests");
    }

    // Stat checks. The character's CharProperties subobject is at +0x560 and the calls read it through the base class.
    // Property ids 25, 148 and 218 are not named in the tree.
    if (character->PROPS_GetInt(25, false) == 0) {
        unlock("completed_never_died");
    }
    const int prop148 = character->PROPS_GetInt(148, false);
    if (prop148 == 2 * (character->SG_GetPlayerLevel() - 1)) {
        unlock("completed_never_increase_stats");
    }
    const int prop218 = character->PROPS_GetInt(218, false);
    if (prop218 == 0) {
        unlock("completed_never_open_chests");
    }
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------------
// Script_AwardEndGameTrophies
// ---------------------------------------------------------------------------------------------------------------------

// 0x455918 Script_AwardEndGameTrophies::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_AwardEndGameTrophies::IsBlocking() const {
    return false;
}

// 0x459b04 Script_AwardEndGameTrophies::Execute(bool, int)
// PARTIAL. The original starts with PlayerManager::GetLocalPlayer(dword_99F76C, 0, true)->character_660, and every unlock
// is for that character. Gap (b): PlayerInfo is not declared (dh2/PlayerInfo.hpp is absent), so the character cannot be
// read. The unlock logic is complete in UnlockEndGameTrophies above. When the lookup exists, Execute passes the character
// to that helper. Until then Execute does nothing. Waits on dh2/PlayerInfo.hpp (PlayerInfo +0x660 Character* character_660).
void Script_AwardEndGameTrophies::Execute(bool /*skipping*/, int /*module*/) {
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_AwardTrophy
// ---------------------------------------------------------------------------------------------------------------------

// 0x455920 Script_AwardTrophy::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_AwardTrophy::IsBlocking() const {
    return false;
}

// 0x4593c4 Script_AwardTrophy::Execute(bool, int)
// PARTIAL. The original reads the argument record from m_args (+0x0c). When the record is present it unlocks the trophy
// whose id is the int at record +0x08, through TrophyManager::s_instance->UnlockTrophy(id). Gap (a): the record type is
// not declared (m_args is void*). Layout request: Script_AwardTrophy argument record +0x08 int trophy id. Until then
// Execute does nothing.
void Script_AwardTrophy::Execute(bool /*skipping*/, int /*module*/) {
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_BlockSaveGame
// ---------------------------------------------------------------------------------------------------------------------

// 0x455930 Script_BlockSaveGame::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_BlockSaveGame::IsBlocking() const {
    return false;
}

// 0x45c8c0 Script_BlockSaveGame::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd probe only. Not reproduced, in the original's order:
//   - the local character's save block. The original reads PlayerManager::GetLocalPlayer(dword_99F76C, 0, true)->character_660
//     and, when it is non-null, calls Character::SG_Block(character, true). Gap (b): PlayerInfo is not declared. Waits on
//     dh2/PlayerInfo.hpp.
//   - the current level's save block, Level::SG_BlockLevel(true) on ApplicationGetInstance().GetCurrentLevel() when that is
//     non-null. Gap (d): dh2/Level.hpp cannot share this translation unit with dh2/Character.hpp, which the trophy helper
//     above needs. The block is one call; add it when the two headers can be included together.
void Script_BlockSaveGame::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_CONSOLE
// ---------------------------------------------------------------------------------------------------------------------

// 0x455648 Script_CONSOLE::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_CONSOLE::IsBlocking() const {
    return false;
}

// 0x460e68 Script_CONSOLE::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd probe and the DisplayScriptConsoleAsDialog switch. When the switch is set and
// the record's text is non-null, the original queues a bottom-bubble dialog: DialogMsg("Console Text", text,
// PyDataConstants::getConstant("DialogStyles", "BottomBubbleAvatarLeft"), -1), enqueued with
// MenuMessageManager<DialogMsg,1>::EnqueueMessage(msg, 1). Not reproduced:
//   (a) the text, at argument record +0x0c (char*). Gap (a): the record type is not declared. Layout request: Script_CONSOLE
//       argument record +0x0c char* console text.
//   (b) the enqueue. The listing passes no singleton for the receiver, and dh2/MenuMessageManager.hpp declares
//       EnqueueMessage(message, queue, flag) with a third bool the listing does not pass. Neither is settled here.
void Script_CONSOLE::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
    const bool displayAsDialog = ReadScriptSwitch("DisplayScriptConsoleAsDialog");
    if (displayAsDialog) {
        // Text and dialog not reproduced, see above.
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_ChangeLevel
// ---------------------------------------------------------------------------------------------------------------------

// 0x4558fc Script_ChangeLevel::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_ChangeLevel::IsBlocking() const {
    return false;
}

// 0x45cba0 Script_ChangeLevel::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd probe only. Not reproduced, and each gap is named:
//   - the local player's character, read first for the difficulty and for every later Character call (SG_GetGameDifficulty,
//     SG_SetLevelId, SG_Save, SG_GetSlot). Gap (b): PlayerInfo is not declared.
//   - the arguments: the entry point name (record +0x08, char*) and the level name (record +0x10, char*). Gap (a): the record
//     type is not declared.
//   - Level::SG_SaveAllPlayer(false) on the current level and the difficulty copied from Level +0x118 (m_difficulty). They run
//     after the character read above, so they are left out with it.
//   - the online test *(GetOnline() + 5) and the hosting player's character (PlayerInfo +0x660). Gap (c): COnline::isOnlineGame_
//     is private with no getter.
//   - the level list search and the level name lookup. Arrays::LevelList is declared twice in the tree, once in
//     pydata-accessors/member_names.hpp (m_memberNames) and once in pydata-accessors/w7_arrays_00.hpp (members, size). Which one
//     is right is not settled.
//   - the second argument of Application::LoadLevel. The listing passes the record's +0x08 value as a const char*, but the
//     header declares the parameter as int entryPointId. Not settled here.
void Script_ChangeLevel::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_CloseDoor
// ---------------------------------------------------------------------------------------------------------------------

// 0x455884 Script_CloseDoor::IsBlocking() const
// Asm: loads m_door (+0x10) and m_instant (+0x14). Returns 0 when m_door is null or m_instant is clear. Otherwise returns
// 1 when m_door->m_state (+0x3a8) is 1 or 3. In Door::DoorStates these are DOOR_OPENED and DOOR_OPENING, so the command
// blocks while the door is open or opening.
bool Script_CloseDoor::IsBlocking() const {
    if (m_door == nullptr || !m_instant) {
        return false;
    }
    return m_door->m_state == Door::DOOR_OPENED || m_door->m_state == Door::DOOR_OPENING;
}

// 0x45d650 Script_CloseDoor::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd probe. Not reproduced: the lookup and the close. The original reads the door
// name (argument record +0x0c, char*) and the instant flag (+0x10, bool) from m_args. Gap (a): the record type is not
// declared. Layout request: Script_CloseDoor argument record +0x0c char* door name, +0x10 bool instant. The original then
// calls ObjectManager::GetObjectByName(name, module, false, nullptr) on the object manager (Application +0x38, the global
// dword_99F764) and ObjectHandle::GetObject(false). When the object's m_goId is 2 it stores the object in m_door, copies
// the instant flag to m_instant and calls Door::Close(skipping) on it. Otherwise m_door is cleared. Until the record exists
// m_door stays null and the command never blocks.
void Script_CloseDoor::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_DeactivateTriggerPlate
// ---------------------------------------------------------------------------------------------------------------------

// 0x4558d4 Script_DeactivateTriggerPlate::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_DeactivateTriggerPlate::IsBlocking() const {
    return false;
}

// 0x45d0e0 Script_DeactivateTriggerPlate::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd probe. Not reproduced: the lookup and the switch-off. The original reads the
// object name (argument record +0x0c, char*). Gap (a): the record type is not declared. Layout request: Script_DeactivateTriggerPlate
// argument record +0x0c char* object name. It then resolves the name with ObjectManager::GetObjectByName(name, module, false,
// nullptr) and ObjectHandle::GetObject(false). When the object's m_goId is 18 it stores the object in m_plate and writes 0 to the
// byte at +0x779 of the object. Otherwise m_plate is cleared. Gap (d): the byte at +0x779 is not declared in ObjectBase.hpp or
// GameObject.hpp. Layout request: ObjectBase +0x779 uint8_t, cleared for a type-18 object (its name is not recovered).
void Script_DeactivateTriggerPlate::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_DoTutorial
// ---------------------------------------------------------------------------------------------------------------------

// 0x455970 Script_DoTutorial::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_DoTutorial::IsBlocking() const {
    return false;
}

// 0x460950 Script_DoTutorial::Execute(bool, int)
// PARTIAL. Not reproduced. The original reads the tutorial script name (argument record +0x0c, char*) and the tutorial index
// (+0x10, int). Gap (a): the record type is not declared. Layout request: Script_DoTutorial argument record +0x0c char* script
// name, +0x10 int tutorial index. It then runs the tutorial only when all of these hold, in this order:
//   - the local character is set (PlayerManager::GetLocalPlayer(dword_99F76C, 0, false)->character_660). Gap (b): PlayerInfo is
//     not declared.
//   - Character::SG_GetGameDifficulty() is 0.
//   - SavegameManager m_tutorials[index] (+0x29) is set. The savegame manager is Application +0x4c (dword_99F778).
//   - the online flag *(GetOnline() + 5) is clear. Gap (c): COnline::isOnlineGame_ is private with no getter.
// Then it starts the script ScriptManager::GetIDFromName(name, true) when that is not -1 (StartScript(id, -1, true)), clears
// m_tutorials[index], and calls SavegameManager::saveSettings() when lastOpenMenuID is 17, otherwise
// updateJob_thread::Start2(m_updateJobThread2). Gap (c): lastOpenMenuID (ELF 0x9f63fc) is not declared. The job thread
// header dh2/updateJob_thread.hpp is absent and m_updateJobThread2 is not declared.
void Script_DoTutorial::Execute(bool /*skipping*/, int /*module*/) {
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_DropLoot
// ---------------------------------------------------------------------------------------------------------------------

// 0x4557e8 Script_DropLoot::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_DropLoot::IsBlocking() const {
    return false;
}

// 0x45e374 Script_DropLoot::Execute(bool, int)
// PARTIAL. Reproduced: the isTracingScriptCmd probe. Not reproduced: the loot drop. The original reads two object names from
// the argument record: the character (+0x0c) and the loot source (+0x14), both char*. Gap (a): the record type is not declared.
// Layout request: Script_DropLoot argument record +0x0c char* character name, +0x14 char* loot source name. It resolves each
// name with ObjectManager::GetObjectByName(name, module, false, nullptr) and ObjectHandle::GetObject(false). When the character
// handle resolves it calls Character::DropLoot(source) with the source as GameObject*, which is null when the second name does
// not resolve.
void Script_DropLoot::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
}

// ---------------------------------------------------------------------------------------------------------------------
// Script_EmptyImpl
// ---------------------------------------------------------------------------------------------------------------------

// 0x455630 Script_EmptyImpl::IsBlocking() const
// Asm: MOV R0,#0; BX LR. Never blocks.
bool Script_EmptyImpl::IsBlocking() const {
    return false;
}

// 0x45c560 Script_EmptyImpl::Execute(bool, int)
// Asm: DebugSwitches::load() on the singleton, then GetSwitch("isTracingScriptCmd") with the value discarded, then the
// std::string temporary is destroyed. No other effect.
void Script_EmptyImpl::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmd();
}
