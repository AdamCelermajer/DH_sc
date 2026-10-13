// Batch g1-game-character-Character-ee9946-19: stat, regeneration, mana, level and experience code of Character
// (0x3bd600 .. 0x3bf828). Declarations come from dh2/Character.hpp.
//
// Status by function (the ledger rows in game-character/ledger.d/g1-game-character-Character-ee9946-19.csv repeat it):
//   reconstructed: IncStatEnd, IncStatDex, IncStatStr
//   partial:       ModifyCharProps, RegenMP, RegenHP, RegenTick, UseMana, LevelUp, _GiveXP, DistributeXP
//
// Header conflict: dh2/PlayerManager.hpp, dh2/PlayerInfo.hpp and dh2/Level.hpp include net-transport's stream_buffer.hpp,
// which redefines IStreamBase (pydata-accessors' deps/stream_reader.hpp is included by Character.hpp through CharProperties).
// The same conflict keeps dh2/global/IsMultiplayerGame.hpp's PlayerManager, Level +0x118 / +0x150, and the
// pydata-accessors member_names.hpp template Arrays::GetMemberIDByString out of this translation unit. The functions that
// need them are PARTIAL and say so.
//
// Property access follows dh2/CharProperties.hpp: PROPS_Get(id, false) reads the final sheet raw (8 fractional bits),
// PROPS_GetInt is that raw value >> 8, PROPS_Add and PROPS_Set take raw values, PROPS_AddInt and PROPS_SetInt shift their
// value left by 8. The original's CharProperties::_GetProperty(m_props, id) is PROPS_Get(id, false).
//
// Every ASSERT of this batch is in Character_Stats.cpp. gAssertLevel 1 prints the failed expression here. gAssertLevel 2
// stores 0 to address 0 in the original (a deliberate crash); that store is not reproduced, and each function that has one
// is marked PARTIAL.

#include <cstdio>

#include "core-platform/memory.hpp"          // gAssertLevel
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/global/charai_ai.hpp"          // ApplicationGetPyDataConstants
#include "dh2/global/ctrl_camera.hpp"        // ApplicationGetInstance
#include "pydata-accessors/w7_pydata12.hpp"  // PyDataConstants::getConstant

namespace {

// CharProperties ids of this batch. The numbers are the original's. Each name comes from the code that uses the id.
constexpr int kPropStatPoints = 148;            // free stat points: the Inc* functions spend one
constexpr int kPropStatStr = 149;               // IncStatStr adds one
constexpr int kPropStatDex = 150;               // IncStatDex adds one
constexpr int kPropStatEnd = 151;               // IncStatEnd adds one
constexpr int kPropXP = 33;                     // LevelUp, _GiveXP: current XP (raw)
constexpr int kPropXPNext = 34;                 // LevelUp, _GiveXP: XP threshold of the next level (raw)
constexpr int kPropLevel = 19;                  // LevelUp, _GiveXP: character level
constexpr int kPropHP = 36;                     // RegenHP: current HP (raw)
constexpr int kPropTotalHP = 38;                // RegenHP: total HP (raw)
constexpr int kPropRegenHPOutOfCombat = 39;     // RegenTick(false): HP regeneration amount
constexpr int kPropRegenHPInCombat = 40;        // RegenTick(true): HP regeneration amount
constexpr int kPropMP = 41;                     // RegenMP, UseMana: current MP (raw)
constexpr int kPropTotalMP = 43;                // RegenMP: total MP (raw)
constexpr int kPropRegenMPOutOfCombat = 44;     // RegenTick(false): MP regeneration amount
constexpr int kPropRegenMPInCombat = 45;        // RegenTick(true): MP regeneration amount

// The Character_Stats.cpp source path of the ASSERT strings, as the original spells it.
constexpr const char* kStatsSourceFile =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Stats.cpp";

// The DebugSwitches probe of the stat and XP paths: load the switch file, then read the named switch (the result is unused).
void ProbeSwitch(const char* name) {
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch(name);
}

// The original's ASSERT(expression) at Character_Stats.cpp:line. gAssertLevel 2 is the deliberate crash, not reproduced.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kStatsSourceFile, line);
    }
}

// The MaxLevel row of the character design for a difficulty (CharacterDesign constants, as the original picks them).
int MaxLevelForDifficulty(int difficulty) {
    if (difficulty == 1) {
        return ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "MaxLevelCHard");
    }
    if (difficulty == 2) {
        return ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "MaxLevelDVeryHard");
    }
    return ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "MaxLevelBNormal");
}

}  // namespace

// 0x3bd600 Character::IncStatEnd(void)
void Character::IncStatEnd() {
    if (PROPS_GetInt(kPropStatPoints, false) <= 0) {
        DebugSwitches::GetInstance()->load();
    } else {
        PROPS_AddInt(kPropStatPoints, -1);
        PROPS_AddInt(kPropStatEnd, 1);
        UpdateBaseProperties(m_charPropsId);
        DebugSwitches::GetInstance()->load();
    }
    DebugSwitches::GetInstance()->GetSwitch("isTracingChar_Stats");
}

// 0x3bd708 Character::IncStatDex(void)
void Character::IncStatDex() {
    if (PROPS_GetInt(kPropStatPoints, false) <= 0) {
        DebugSwitches::GetInstance()->load();
    } else {
        PROPS_AddInt(kPropStatPoints, -1);
        PROPS_AddInt(kPropStatDex, 1);
        UpdateBaseProperties(m_charPropsId);
        DebugSwitches::GetInstance()->load();
    }
    DebugSwitches::GetInstance()->GetSwitch("isTracingChar_Stats");
}

// 0x3bd810 Character::IncStatStr(void)
void Character::IncStatStr() {
    if (PROPS_GetInt(kPropStatPoints, false) <= 0) {
        DebugSwitches::GetInstance()->load();
    } else {
        PROPS_AddInt(kPropStatPoints, -1);
        PROPS_AddInt(kPropStatStr, 1);
        UpdateBaseProperties(m_charPropsId);
        DebugSwitches::GetInstance()->load();
    }
    DebugSwitches::GetInstance()->GetSwitch("isTracingChar_Stats");
}

// 0x3bda74 Character::ModifyCharProps(int,int)
// PARTIAL: the ASSERT amount >= 0 (Character_Stats.cpp:398) at gAssertLevel 2 is the deliberate null store, not reproduced.
// The IsPlayer test is the virtual call of vtable slot +0x28, which is Character::IsPlayer (dh2/Character.hpp).
void Character::ModifyCharProps(int propId, int amount) {
    if (!IsPlayer()) {
        return;
    }
    if (amount < 0) {
        ReportAssert("amount >= 0", 398);
    }
    PROPS_Add(propId, amount);
    UpdateBaseProperties(m_charPropsId);
    ProbeSwitch("isTracingChar_Stats");
}

// 0x3bdbb8 Character::RegenMP(int)
// PARTIAL: the original returns int. It returns the total MP (raw, prop 43) when nothing is regenerated, and the int of
// CharProperties::PROPS_Add otherwise. The header declares RegenMP and PROPS_Add void, so the return value is not made.
// No caller in the tree reads it. The regeneration itself is reproduced.
void Character::RegenMP(int amount) {
    const int current = PROPS_Get(kPropMP, false);
    const int total = PROPS_Get(kPropTotalMP, false);
    if (amount < 0) {
        amount = total;
    }
    if (amount + current > total) {
        amount = total - current;
    }
    if (amount > 0) {
        ProbeSwitch("isTracingChar_Stats");
        PROPS_Add(kPropMP, amount);
    }
}

// 0x3bdca4 Character::RegenHP(int)
// PARTIAL: the return value, see RegenMP (the total HP, raw, prop 38, or the int of PROPS_Add).
void Character::RegenHP(int amount) {
    const int current = PROPS_Get(kPropHP, false);
    const int total = PROPS_Get(kPropTotalHP, false);
    if (amount < 0) {
        amount = total;
    }
    if (amount + current > total) {
        amount = total - current;
    }
    if (amount > 0) {
        ProbeSwitch("isTracingChar_Stats");
        PROPS_Add(kPropHP, amount);
    }
}

// 0x3bdd90 Character::RegenTick(bool)
// PARTIAL: the original returns the int of RegenMP (see RegenMP), not reproduced. The argument is the in-combat flag:
// CharAI calls RegenTick(AI_IsInCombat()).
void Character::RegenTick(bool inCombat) {
    if (inCombat) {
        ProbeSwitch("isTracingChar_Stats");
        RegenHP(PROPS_Get(kPropRegenHPInCombat, false));
        RegenMP(PROPS_Get(kPropRegenMPInCombat, false));
    } else {
        ProbeSwitch("isTracingChar_Stats");
        RegenHP(PROPS_Get(kPropRegenHPOutOfCombat, false));
        RegenMP(PROPS_Get(kPropRegenMPOutOfCombat, false));
    }
}

// 0x3bdef4 Character::UseMana(int)
// PARTIAL: the original first reads COnline's online-game flag (GetOnline() + 5). When the flag is set and
// IsRemotelyUpdated() holds, the result is true. COnline is only forward-declared (dh2/global/net_singletons.hpp), so the
// online branch is not reproduced and every call runs the offline path. The ASSERT amount >= 0 (line 159) at gAssertLevel
// 2 is the deliberate null store, not reproduced.
bool Character::UseMana(int cost) {
    if (cost < 0) {
        ReportAssert("amount >= 0", 159);
    }
    if (ApplicationGetInstance().IsSavedOptionOn("GOD_MANA")) {
        return true;
    }
    DebugSwitches::GetInstance()->load();
    const bool godMana = DebugSwitches::GetInstance()->GetSwitch("GOD_MANA");
    if (godMana || m_freeMana != 0) {
        return true;
    }
    if (!HasMana(cost)) {
        return false;
    }
    PROPS_Add(kPropMP, -cost);
    ProbeSwitch("isTracingChar_Stats");
    return true;
}

// 0x3beb88 Character::LevelUp(int)
// PARTIAL: the player presentation of the level-up is only partly made (the original does it when IsPlayer()):
//   - not made: the HUD status message (StatusMsg deque, StringManager "MENU_LEVEL_UP") and the HUD callback
//     (MenuManager HUD root, DebugCachedCharacter cache, RenderFX::InvokeASCallback);
//   - not made: VisualFXManager::PlayAnimFXSet(135, this, 0), since Singleton<VisualFXManager>::s_inst has no accessor;
//   - not made: the tutorial cinematics at level 2 (COnline flag, ScriptManager tutorial flags) and the spec-time movie
//     callback at level 12 (RenderFX gameswf argument);
//   - not made: the epic trophy of a local player (PlayerManager::IsLocalPlayer and TrophyManager::UnlockTrophy with
//     Arrays::GetMemberIDByString<Arrays::TrophyTable>; PlayerManager cannot be included here, see the header note).
// Made: the level step, the XP reset, the regeneration, the savegame update, the PlayerLevelUp script start, and the
// leftover XP of the new level. The original's return value (PROPS_GetInt of the level, or the int of PROPS_AddInt) is
// void here.
void Character::LevelUp(int levels) {
    const int maxLevel = MaxLevelForDifficulty(SG_GetGameDifficultyUnlocked());
    if (maxLevel <= PROPS_GetInt(kPropLevel, false)) {
        return;
    }
    ProbeSwitch("isTracingChar_Stats");
    PROPS_AddInt(kPropLevel, 1);
    PROPS_SetInt(kPropXP, 0);
    UpdateBaseProperties(m_charPropsId);
    RegenHP(-1);
    RegenMP(-1);
    SG_SetPlayerLevel(PROPS_GetInt(kPropLevel, false));
    SG_Save();

    if (IsPlayer()) {
        // PARTIAL (see the function note): only the script start is made here.
        const int playerLevelUp = ScriptManager::s_inst.GetIDFromName("PlayerLevelUp", true);
        if (playerLevelUp != -1) {
            ScriptManager::s_inst.StartScript(playerLevelUp, -1, false);
        }
    }

    // LABEL_18 of the original: the leftover levels become XP of the new level, capped at the threshold minus one.
    if (maxLevel > PROPS_GetInt(kPropLevel, false)) {
        if (levels > (PROPS_Get(kPropXPNext, false) >> 8)) {
            levels = (PROPS_Get(kPropXPNext, false) >> 8) - 1;
        }
        PROPS_AddInt(kPropXP, levels);
    }
}

// 0x3bf498 Character::_GiveXP(int,bool)
// PARTIAL: (1) the original returns 0 when the current Level's unk_150 (+0x150) is non-null, and it compares the unlocked
// difficulty with the Level's m_difficulty (+0x118) to force the amount to 256 when it is lower. Both read dh2/Level.hpp,
// which conflicts with the pydata IStreamBase (see the header note). Neither check is made here. (2) The ASSERT
// amount >= 0 (Character_Stats.cpp:350) at gAssertLevel 2 is the deliberate null store, not reproduced. (3) The a3 branch,
// PlayerStatManager::IncreaseStat(6, xp >> 8, player's PlayerInfo +0x670), is not made: Singleton<PlayerStatManager>::s_inst
// has no accessor. The declaration returns bool now (was void): the body returns 1 or 0 and DistributeXP reads the result.
bool Character::_GiveXP(int xp, bool a3) {
    const int maxLevel = MaxLevelForDifficulty(SG_GetGameDifficultyUnlocked());
    if (maxLevel <= PROPS_GetInt(kPropLevel, false) || !IsPlayer() || IsRemotelyUpdated()) {
        return false;
    }
    if (xp < 0) {
        ReportAssert("amount >= 0", 350);
    }
    DebugSwitches::GetInstance()->load();
    const bool oneKillLevelUp = DebugSwitches::GetInstance()->GetSwitch("OneKillLevelUp");
    if (oneKillLevelUp) {
        xp = PROPS_Get(kPropXPNext, false) - PROPS_Get(kPropXP, false);
    }
    const int modifiedXP = PROPS_GetModifiedXP(xp);
    PROPS_Add(kPropXP, modifiedXP);
    ProbeSwitch("isTracingChar_Stats");
    if (PROPS_Get(kPropXP, false) >= PROPS_Get(kPropXPNext, false)) {
        LevelUp((PROPS_Get(kPropXP, false) - PROPS_Get(kPropXPNext, false)) >> 8);
        if (PROPS_Get(kPropXP, false) > PROPS_Get(kPropXPNext, false)) {
            PROPS_Set(kPropXP, PROPS_Get(kPropXPNext, false));
        }
    }
    (void)a3;  // PARTIAL: the PlayerStatManager counter of the a3 branch (see the note above) is not made.
    return true;
}

// 0x3bf828 Character::DistributeXP(Character*,Character*)
// The original's signature is (Character* this, Character* a2, Character* a3). this is the killer: Character::Kill
// (recovered/native/hexrays-all/chunk_008.c:2018) calls DistributeXP(killer, this, <unset>), so the killed character is a2,
// the header's first parameter. a3 is never read by the body, so the header's second parameter is unused here.
// PARTIAL: only the entry, the victim check and the debug-switch probes are made. The experience share over the local
// players is not made: the loop over PlayerManager::GetPlayer(i, true), the share radius and penalty (DesignSettingsTable
// rows 40 and 41, read as floats from the table's raw buffer), GetLevelScaledXP, _GiveXP on each rewarded player and the
// scrolling combat text. PlayerManager (dword_99F76C, m_numCharacters +0x6c4) and PlayerInfo (unk_660 +0x660, the
// player's Character*) cannot be included with Character.hpp (see the header note). The scrolling text would also need
// F_ApplyScrollingCombatTextXP, whose declared first parameter (Character* target) differs from the XP amount the original passes.
void Character::DistributeXP(Character* victim, Character* killer) {
    (void)killer;  // a3 of the original: unused
    if (victim == nullptr) {
        ReportAssert("killed != 0", 219);
        return;
    }
    ProbeSwitch("isTracingXPDistributionCst");
    ProbeSwitch("isTracingXPDistribution");
    ProbeSwitch("isTracingXPDistribution");
    // PARTIAL: the player loop of the original is not made (see the function note).
    ProbeSwitch("isTracingXPDistribution");  // LABEL_31
    ProbeSwitch("isTracingXPDistribution");  // LABEL_31
}
