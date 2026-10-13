// Batch g1-game-items-save-PlayerStatManager-d6e093-0 (owner PlayerStatManager, part 1 of 2).
// Reconstructed from the Hex-Rays pseudocode and the assembly of libDungeonHunter2.so. Declarations: dh2/PlayerStatManager.hpp.
// The stat values are PlayerInfo's seven NetStructInt blocks (dh2/PlayerInfo.hpp). Players come from the PlayerManager global.
//
// Notes:
//  * IncreaseStat is an empty body in this build (asm: BX LR). DecreaseStat, IncrementStat and DecrementStat forward to it
//    with their amount changed, so no stat changes. They are written as the forwards the asm shows.
//  * Hex-Rays drops the arguments of the forwards (it shows IncreaseStat() with no arguments). The argument values come
//    from the assembly: DecreaseStat negates the amount (RSB R2, R2, #0), IncrementStat passes 1, DecrementStat passes -1,
//    ResetStatValue passes the value 0 and the flag false (stack).
//  * GetStatStr is static. Its call sites pass the string return slot and the stat, with no this (asm: R0 = sret, R1 = stat).
//  * The ranking buffer is 32 bytes in the binary (symbol size): four (value, playerId) pairs.
//  * The deleting destructor (0x3795ac) is compiler generated. See the ledger.

#include "dh2/PlayerStatManager.hpp"

#include <cstdint>
#include <cstdio>
#include <string>

#include "core-platform/memory.hpp"          // gAssertLevel
#include "dh2/MenuManager.hpp"               // MenuManager::GetInstance and GetHUDRoot (MenuFX derives from RenderFX)
#include "dh2/PlayerInfo.hpp"                // the seven stat blocks, unk_548 .. unk_638
#include "dh2/PlayerManager.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager global
#include "gameswf/gameswf_value.h"           // gameswf::as_value (12 bytes, the binary's layout)
// gameswf/gameswf_root.h (gameswf::root::get_root_movie) and gameswf/gameswf_character.h do not compile in this host build
// (the vendored gameswf GC instantiation in base/tu_gc_singlethreaded_refcount.h), so the Invoke overloads stop before the
// root movie. See their notes.

namespace {

// PSTAT_COUNT (the assert text "statType < PSTAT_COUNT") and MAX_PLAYERS ("playerId < MAX_PLAYERS", tested as a3 > 3).
constexpr int kStatCount = 7;
constexpr int kMaxPlayerIndex = 3;

// The stat blocks, in stat order. Stat s is the value (block + 0x20) of the NetStructInt at +0x548 + 40 * s, which is
// the word the original reads at [40 * stat + 1384] (+0x568 + 40 * s). The seven blocks are contiguous with a 0x28 stride.
NetStructInt<16u> PlayerInfo::* const kStatMembers[kStatCount] = {
    &PlayerInfo::unk_548,  // +0x548 stat 0 (killer)
    &PlayerInfo::unk_570,  // +0x570 stat 1 (striker)
    &PlayerInfo::unk_598,  // +0x598 stat 2 (controller)
    &PlayerInfo::unk_5c0,  // +0x5c0 stat 3 (defender)
    &PlayerInfo::unk_5e8,  // +0x5e8 stat 4 (looter)
    &PlayerInfo::unk_610,  // +0x610 stat 5 (support)
    &PlayerInfo::unk_638,  // +0x638 stat 6 (xp)
};

}  // namespace

// 0x379084 _ZN17PlayerStatManagerC2Ev
// 0x3790a8 _ZN17PlayerStatManagerC1Ev
// Both constructors have the same body: the vptr is set to the vtable address point (vtable + 8). The compiler writes
// the vptr, and there is nothing else to do.
PlayerStatManager::PlayerStatManager() {}

// 0x3790cc _ZN17PlayerStatManagerD2Ev
// 0x3790d0 _ZN17PlayerStatManagerD1Ev
// Both destructors are a bare BX LR. There are no members to destroy.
PlayerStatManager::~PlayerStatManager() {}

// 0x3795ac _ZN17PlayerStatManagerD0Ev (deleting destructor)
// Skipped: compiler generated. delete on the virtual destructor runs ~PlayerStatManager() and then releases the object
// (the original calls CustomFree). Nothing is defined here.

// 0x3790d4 _ZN17PlayerStatManager5ResetEv
// Empty (asm: BX LR).
void PlayerStatManager::Reset() {}

// 0x3790d8 _ZN17PlayerStatManager6UpdateEv
// Empty (asm: BX LR).
void PlayerStatManager::Update() {}

// 0x3790dc _ZN17PlayerStatManager17ApplyPlayersBonusEv
// Empty (asm: BX LR).
void PlayerStatManager::ApplyPlayersBonus() {}

// 0x3790e0 _ZN17PlayerStatManager12IncreaseStatE9EStatTypeii
// Empty (asm: BX LR). The stat is not changed in this build.
void PlayerStatManager::IncreaseStat(EStatType /*stat*/, int /*amount*/, int /*playerId*/) {}

// 0x3790e4 _ZN17PlayerStatManager12DecreaseStatE9EStatTypeii
// asm: RSB R2, R2, #0, then B IncreaseStat. The amount is negated (wrapping, as RSB does) and the call is forwarded.
void PlayerStatManager::DecreaseStat(EStatType stat, int amount, int playerId) {
    IncreaseStat(stat, static_cast<int>(0u - static_cast<unsigned int>(amount)), playerId);
}

// 0x3790ec _ZN17PlayerStatManager13IncrementStatE9EStatTypei
// asm: MOV R3, R2; MOV R2, #1; B IncreaseStat. The player id moves to the fourth argument and the amount is 1.
void PlayerStatManager::IncrementStat(EStatType stat, int playerId) {
    IncreaseStat(stat, 1, playerId);
}

// 0x3790f8 _ZN17PlayerStatManager13DecrementStatE9EStatTypei
// asm: MOV R3, R2; MOV R2, #0xFFFFFFFF; B IncreaseStat. The amount is -1.
void PlayerStatManager::DecrementStat(EStatType stat, int playerId) {
    IncreaseStat(stat, -1, playerId);
}

// 0x379104 _ZN17PlayerStatManager12SetStatValueE9EStatTypeiib
// Empty (asm: BX LR).
void PlayerStatManager::SetStatValue(EStatType /*stat*/, int /*value*/, int /*playerId*/, bool /*flag*/) {}

// 0x379108 _ZN17PlayerStatManager14ResetStatValueE9EStatTypei
// asm: SetStatValue(stat, 0, playerId, false). The value is 0 and the flag (the fifth argument, on the stack) is 0.
void PlayerStatManager::ResetStatValue(EStatType stat, int playerId) {
    SetStatValue(stat, 0, playerId, false);
}

// 0x37912c _ZN17PlayerStatManager18ResetAllStatValuesEi
// ResetStatValue for stats 0 to 6 (asm loop, counter from 0 to 7). Hex-Rays declares the result as int and returns the R0
// that the last call leaves. The header declares void, and no caller uses the value.
void PlayerStatManager::ResetAllStatValues(int playerId) {
    for (int stat = 0; stat != kStatCount; ++stat) {
        ResetStatValue(static_cast<EStatType>(stat), playerId);
    }
}

// 0x379568 _ZNK17PlayerStatManager6InvokeERKSs
// PARTIAL. Reproduced: the HUD root (MenuManager::GetInstance, then GetHUDRoot).
// Not reproduced: the callback. The original calls InvokeASCallback(root movie, name, no arguments, count 0) on the HUD root,
// with the root movie from RenderFX::GetRoot and gameswf::root::get_root_movie. gameswf::root is declared in
// gameswf/gameswf_root.h, which does not compile in this host build, so the root movie cannot be named. 0 is returned in
// place of the callback's result.
int PlayerStatManager::Invoke(const std::string& /*name*/) const {
    MenuManager::GetInstance()->GetHUDRoot();  // the HUD root; the callback is not made (partial, see above)
    return 0;                                  // partial: see above
}

// 0x3795c8 _ZN17PlayerStatManager10GetStatStrE9EStatType
// Static (see the header). The seven names of the stats; any other value gives "".
std::string PlayerStatManager::GetStatStr(EStatType stat) {
    switch (stat) {
        case STAT_KILLER:
            return "killer";
        case STAT_STRIKER:
            return "striker";
        case STAT_CONTROLLER:
            return "controller";
        case STAT_DEFENDER:
            return "defender";
        case STAT_LOOTER:
            return "looter";
        case STAT_SUPPORT:
            return "support";
        case STAT_XP:
            return "xp";
        default:
            return "";
    }
}

// 0x3796c4 _ZNK17PlayerStatManager6InvokeE9EStatTypeii
// PARTIAL. Reproduced: the player record, the HUD root, and the four arguments of the onRankChange callback, in this order:
//   [0] the player's unk_67c (+0x67c, name not recovered), as a number,
//   [1] CNetPlayerInfo::IsRemote, as a boolean,
//   [2] GetStatStr(stat), as a string,
//   [3] the rank index, as a number.
// The arguments are released in reverse order when the array goes out of scope, as the original's drop_refs calls do.
// Not reproduced:
//   1. The callback. The original calls InvokeASCallback(root movie, "onRankChange", args, 4) on the HUD root. The root movie
//      comes from gameswf::root::get_root_movie, which cannot be named in this host build (see the include note above).
//   2. The return value. Hex-Rays returns the R0 that gameswf::as_value::drop_refs leaves, which is not a value of this
//      function. The header keeps int for this overload (UpdateRanking returns the result), so 0 is returned.
int PlayerStatManager::Invoke(EStatType stat, int playerId, int rankIndex) const {
    PlayerInfo* player = dword_99F76C->GetPlayerByInternalID(playerId, false);
    MenuManager::GetInstance()->GetHUDRoot();  // the HUD root; the callback is not made (partial, see above)
    gameswf::as_value args[4];
    args[0].set_double(player->unk_67c);
    args[1].set_bool(player->IsRemote());
    const std::string name = GetStatStr(stat);
    args[2].set_string(name.c_str());
    args[3].set_double(rankIndex);
    return 0;  // partial: see above
}

// 0x379824 _ZNK17PlayerStatManager12GetStatValueE9EStatTypei
// PARTIAL. Reproduced: the value of stat 0 to 6 for a player, read from the player's stat block (PlayerInfo +0x548 + 40 * stat,
// through the NetStruct value at block + 0x20), and the level-1 assert messages for stat > 6 and playerId > 3.
// Not reproduced:
//   1. When an assert fails and gAssertLevel is 2, the original stores 0 to address 0 (dword_0), a deliberate fault.
//   2. A stat outside 0 to 6 (the assert case). The original reads the word at 40 * stat + 1384 of the player record, which is
//      not a stat block. This build returns 0 for those stats.
int PlayerStatManager::GetStatValue(EStatType stat, int playerId) const {
    const int statIndex = static_cast<int>(stat);  // the original compares signed
    if (statIndex > kStatCount - 1) {
        if (gAssertLevel == 2) {
            // partial: the original stores 0 to address 0 here (dword_0), a deliberate fault. Not reproduced.
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "statType < PSTAT_COUNT",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\PlayerManager\\PlayerStatManager.cpp", 223);
        }
    }
    if (playerId > kMaxPlayerIndex) {
        if (gAssertLevel == 2) {
            // partial: as above, the deliberate fault is not reproduced.
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "playerId < MAX_PLAYERS",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\PlayerManager\\PlayerStatManager.cpp", 224);
        }
    }
    PlayerInfo* player = dword_99F76C->GetPlayerByInternalID(playerId, false);
    if (statIndex < 0 || statIndex >= kStatCount) {
        return 0;  // partial: see above
    }
    return (player->*kStatMembers[statIndex]).GetValue();
}

// 0x379ac4 _ZNK17PlayerStatManager10GetRankingE9EStatTypeb
// PARTIAL. Reproduced: the buffer of (value, playerId) pairs for the players (value = GetStatValue(stat, i), playerId = i) and,
// when sort is set, the insertion sort of the pairs by value, largest first. Ties keep their player order (the comparator
// PlayerStatManager::_StatComp is a.first > b.first, and the insertion is stable). The buffer is returned as ints.
// Not reproduced: the introsort and final insertion pass (STL, more than 16 pairs). The buffer holds four pairs, so that
// branch cannot run with MAX_PLAYERS players. For more than four players the original writes past the buffer; that is not
// reproduced, and the loops below are written as the original has them.
int* PlayerStatManager::GetRanking(EStatType stat, bool sort) const {
    // The original's guard zeroes the buffer on the first call. A zero-initialised static is the same.
    static int stat_values[8] = {};  // four pairs: stat_values[2 * i] is the value, stat_values[2 * i + 1] the player
    if (sort) {
        const int numPlayers = dword_99F76C->GetNumPlayers();
        if (numPlayers > 0) {
            for (int i = 0; i != numPlayers; ++i) {
                stat_values[2 * i] = GetStatValue(stat, i);
                stat_values[2 * i + 1] = i;
            }
        }
        if (numPlayers != 0) {
            // __insertion_sort: the pairs after the first are inserted into the sorted prefix (__linear_insert).
            for (int k = 1; k < numPlayers; ++k) {
                const int value = stat_values[2 * k];
                const int playerId = stat_values[2 * k + 1];
                if (value <= stat_values[0]) {
                    // Not above the first pair: walk back while the value is above the pair before the hole.
                    int hole = k;
                    while (value > stat_values[2 * (hole - 1)]) {
                        stat_values[2 * hole] = stat_values[2 * (hole - 1)];
                        stat_values[2 * hole + 1] = stat_values[2 * (hole - 1) + 1];
                        --hole;
                    }
                    stat_values[2 * hole] = value;
                    stat_values[2 * hole + 1] = playerId;
                } else {
                    // Above the first pair: every pair before k moves up one slot and the value goes to the front.
                    for (int j = k; j > 0; --j) {
                        stat_values[2 * j] = stat_values[2 * (j - 1)];
                        stat_values[2 * j + 1] = stat_values[2 * (j - 1) + 1];
                    }
                    stat_values[0] = value;
                    stat_values[1] = playerId;
                }
            }
        }
    }
    return stat_values;
}
