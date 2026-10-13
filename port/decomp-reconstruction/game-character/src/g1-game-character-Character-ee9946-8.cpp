// Batch g1-game-character-Character-ee9946-8 (part 9 of 20 of Character): the faery accessors and level changes,
// the immunity-checked attack roll, the scrolling combat text and sound helpers, the result application and the
// combat result calculation. Declarations come from dh2/Character.hpp and dh2/global/CombatFormulas.hpp.
// PARTIAL bodies name what is missing in a PARTIAL comment; the ledger rows in
// game-character/ledger.d/g1-game-character-Character-ee9946-8.csv repeat it.

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "core-platform/memory.hpp"            // gAssertLevel
#include "dh2/global/runtime_support.hpp"      // PushProfilingContext, PopProfilingContext
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Random.hpp"
#include "dh2/global/CombatFormulas.hpp"       // s_attacker, s_defender, s_element, s_offHanded, s_isSkill, CF_SetCombatants
#include "dh2/global/cf_calc.hpp"              // CF__Calc* rolls, CombatDamage, MissOrDodgeResult
#include "dh2/global/charai_ai.hpp"            // ApplicationGetPyDataConstants (dword_99F758)
#include "pydata-accessors/w7_pydata12.hpp"    // PyDataConstants::getConstant

namespace {

constexpr const char kCharacterFaeryFile[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Faery.cpp";
constexpr const char kCharacterFormulasFile[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Formulas.cpp";

// The original ASSERT(expr) macro, called on its failing branch. gAssertLevel 2 stops the process (the original stores
// 0 to address 0), 1 prints the message to stderr and execution continues, 0 is silent.
void ReportAssertFailure(const char* expr, const char* file, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, file, line);
    }
}

// The attack code reads one debug switch at several points: load the switch file, then read the switch. The value is
// discarded at each of these call sites, so only the read itself is kept.
void ReadTraceSwitch(const char* name) {
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch(name);
}

// A debug switch whose value the original tests: load the switch file, then read the switch.
bool ReadSwitch(const char* name) {
    DebugSwitches::GetInstance()->load();
    return DebugSwitches::GetInstance()->GetSwitch(name);
}

// Character::AttackResult.m_resultFlags (+0x18, the byte at +0x18 and bit 0 of +0x19 as bit 8). Set by the CF__Calc*
// rolls in _F_CalculateResult and read by F_ApplyResult. Each name is the state F_ApplyResult selects with the bit.
constexpr std::uint32_t kResultMissed = 0x001;   // CF__CalcMissOrDodge, missed
constexpr std::uint32_t kResultDodged = 0x002;   // CF__CalcMissOrDodge, dodged: dodging state
constexpr std::uint32_t kResultBlocked = 0x004;  // CF__CalcBlock: blocking state
constexpr std::uint32_t kResultCritical = 0x008; // CF__CalcCrit: critical hit
constexpr std::uint32_t kResultInjure = 0x010;   // CF__CalcHurt: injure state
constexpr std::uint32_t kResultScare = 0x020;    // CF__CalcFear: scare state
constexpr std::uint32_t kResultStun = 0x040;     // CF__CalcStun: stun state
constexpr std::uint32_t kResultPush = 0x080;     // CF__CalcPush: knock-back state
constexpr std::uint32_t kResultSlow = 0x100;     // CF__CalcSlow: slow debuff (bit 0 of +0x19)

// Character::AttackResult.m_attackFlags (+0x1c), the request word of _F_CalculateResult.
constexpr std::uint32_t kAttackOffHand = 0x04000000;  // CF_SetCombatants offHanded
constexpr std::uint32_t kAttackSkill = 0x08000000;    // CF_SetCombatants isSkill
constexpr std::uint32_t kAttackDot = 0x20000000;      // damage over time: the "anim_sct_dot" text, no combat AI reaction

void SetResultFlag(Character::AttackResult& result, std::uint32_t flag, bool on) {
    if (on) {
        result.m_resultFlags |= flag;
    } else {
        result.m_resultFlags &= ~flag;
    }
}

// One roll of the attack code: Random::GetRandom(100, ...). The original calls the IPA clone at 0x3af6d8 (IDA name
// _ZN6Random9GetRandomEib.clone.1_1), which ignores the second argument register and draws from the ordinary stream
// (Random::s_seed, debug counter 0), so the stream is fixed to false here.
int RollPercent() {
    return static_cast<int>(Random::GetRandom(100, false));
}

}  // namespace

// 0x3ae60c Character::GetFaeryCharacter(void)
// PARTIAL: the original returns the word at +0x420 of Character, which is CharAI::unk_58 (the faery character object:
// ChangeFaery sets its model through the same pointer). That member is private in dh2/CharAI.hpp and cannot be read here.
// Layout request: a public accessor (or a public member) for the faery pointer in CharAI. Until then this returns nullptr.
Character* Character::GetFaeryCharacter() {
    return nullptr;
}

// 0x3ae614 Character::GetFaeryCharacter(void) const
// PARTIAL: as the non-const overload above (the CharAI::unk_58 pointer is private in dh2/CharAI.hpp).
const Character* Character::GetFaeryCharacter() const {
    return nullptr;
}

// 0x3ae61c Character::IncFaeryLevel(unsigned int)
// PARTIAL: the faery_charged trophy is not awarded. The original unlocks it through TrophyManager::s_instance when
// PlayerManager::IsLocalPlayer(dword_99F76C, this) is true and the name is found in Arrays::TrophyTable. dh2/PlayerManager.hpp
// does not compile beside dh2/Character.hpp (its net-transport StreamBuffer clashes with the pydata IStreamBase), so the
// local-player test is not written. The level change, the skill update and the every-faery-leveled check are reproduced.
void Character::IncFaeryLevel(unsigned int faery) {
    const int difficulty = SG_GetGameDifficulty();
    if (static_cast<unsigned int>(SG_GetFaerieCount(difficulty)) <= faery) {
        ReportAssertFailure("faeryIdx < SG_GetFaerieCount(diff)", kCharacterFaeryFile, 169);
    }
    const int level = SG_GetFaerieLevel(faery, difficulty);
    SG_SetFaerieLevel(faery, level + 1, difficulty);
    CharAI::UpdateAllSkills();

    // Every faery at level 1 or above (the loop returns on the first level 0) unlocks nothing more.
    for (unsigned int i = 0; static_cast<unsigned int>(SG_GetFaerieCount(difficulty)) > i; ++i) {
        if (SG_GetFaerieLevel(i, -1) == 0) {
            return;
        }
    }

    // PARTIAL: the faery_charged trophy unlock (see the comment above) belongs here, after the loop.
}

// 0x3ae7d4 Character::SetFaeryState(unsigned int, int)
// PARTIAL: the first-faery tutorial branch is not reproduced. It runs when state is 1, faery is not 0, the online flag
// of COnline (+0x05, isOnlineGame_, no getter in net-session-online/c_online.hpp) is clear, the difficulty is 0 and
// the byte at SavegameManager +0x2b is set (no member declared for it). It starts the cinematic_Tuto_faery script
// (ScriptManager::GetIDFromName and StartScript are not declared), clears the byte and calls saveSettings. The state
// change below is complete.
void Character::SetFaeryState(unsigned int faery, int state) {
    const int difficulty = SG_GetGameDifficulty();
    if (static_cast<unsigned int>(SG_GetFaerieCount(difficulty)) <= faery) {
        ReportAssertFailure("faeryIdx < SG_GetFaerieCount(diff)", kCharacterFaeryFile, 135);
    }
    SG_SetFaerieState(faery, state, difficulty);
}

// 0x3ae94c Character::IncAllFaeriesLevel(void)
void Character::IncAllFaeriesLevel() {
    const int difficulty = SG_GetGameDifficulty();
    for (unsigned int i = 0; i < static_cast<unsigned int>(SG_GetFaerieCount(difficulty)); ++i) {
        SetFaeryState(i, 1);
        IncFaeryLevel(i);
    }
}

// 0x3ae99c Character::ChangeFaery(unsigned int)
// PARTIAL: the faery's visual model change and animation set (on the faery character from GetFaeryCharacter, which is
// partial) are not written. The ASSERT, the current faery and the skill update are reproduced.
void Character::ChangeFaery(unsigned int faery) {
    const int difficulty = SG_GetGameDifficulty();
    if (static_cast<unsigned int>(SG_GetFaerieCount(difficulty)) <= faery) {
        ReportAssertFailure("faeryIdx < SG_GetFaerieCount(diff)", kCharacterFaeryFile, 107);
    }
    SG_SetCurrentFaerie(faery, difficulty);
    CharAI::UpdateAllSkills();
}

// 0x3aea9c Character::IsCastingPossible(void)
bool Character::IsCastingPossible() {
    if (!CTRLIsAllowed()) {
        return false;
    }
    return CharAI::AI_IsSpellUsable();
}

// 0x3aeac0 Character::GetCharFaery(int) const
// PARTIAL: the two pydata rows are read through raw listings. The FaeryListTable row (12-byte stride: ListSize at +4,
// List at +8) and the FaeryTable row (36-byte stride: Type at +32) have no declared struct layout in pydata-accessors
// (w7_arrays27.hpp declares only the static members and size). Layout request: a FaeryListTable row struct with
// ListSize and List, and a FaeryTable row struct with Type. Until then the lookup returns nullptr. The two ASSERTs
// on the count are reproduced.
const void* Character::GetCharFaery(int faeryId) const {
    const int faeryTypeCount = ApplicationGetPyDataConstants()->getConstant("FaeryTypes", "COUNT");
    if (faeryId < 0 || faeryId >= faeryTypeCount) {
        ReportAssertFailure(
            "faeryId >= 0 && faeryId < Application::GetInstance()->GetConstantsMgr()->getConstant(\"FaeryTypes\", \"COUNT\")",
            kCharacterFaeryFile, 62);
    }
    return nullptr;
}

// 0x3af0c8 Character::F_ApplyScrollingCombatTextXP(Character*, int, int, int)
// PARTIAL: the floating text needs the StringManager instance (dword_99F760, no accessor declared) to format the
// GAMEPLAYMENUS_REWARD_XP string with the target, and the GameObject position floats at +0x14c and +0x158 (no names
// declared). Nothing is drawn yet. The style argument is FlashAnimManager::PlayScrollingCombatText's styleId and the
// fourth argument is its color; the fifth argument of the original's call list is never read.
void Character::F_ApplyScrollingCombatTextXP(Character* target, int styleId, int colorId, int unusedArg) {
    (void)target;
    (void)styleId;
    (void)colorId;
    (void)unusedArg;
}

// 0x3aef68 Character::F_ApplyScrollingCombatTextGold(Character*, int)
// PARTIAL: the original formats the ITEMS_GOLD string with the first character argument through the StringManager
// instance (dword_99F760, no accessor declared), and places it with the GameObject position floats at +0x14c and +0x158
// (no names declared). The gold argument is not read by the original. Nothing is drawn yet.
void Character::F_ApplyScrollingCombatTextGold(Character* target, int gold) {
    (void)target;
    (void)gold;
}

// 0x3af1fc Character::F_ApplyScrollingCombatText(Character::AttackResult const&, GameObject*, Character*)
// PARTIAL: a follower shows nothing and returns here. The rest (the damage value and its style, the color constant)
// needs the GameObject position floats at +0x14c and +0x158, which have no declared names.
void Character::F_ApplyScrollingCombatText(const AttackResult& result, GameObject* attacker, Character* target) {
    (void)result;
    (void)attacker;
    if (target->IsFollower()) {
        return;
    }
    // PARTIAL: FlashAnimManager::PlayScrollingCombatValue of the damage (anim_sct_normaldamage, anim_sct_dot,
    // anim_sct_critleft/right and friends, PlayerDamageColor or DamageColor) is not reproduced.
}

// 0x3af3b0 Character::_F_ResetResult(Character::AttackResult&)
// The original stores zeros, then overwrites the words it sets to -1 (the last store to each word wins).
void Character::_F_ResetResult(AttackResult& result) {
    result.m_damage = -1;
    result.unk_04 = -1;
    result.unk_08 = -1;
    result.unk_0c = -1;
    result.unk_10 = 0;
    result.unk_14 = 0;
    result.m_resultFlags = 0;
    result.m_attackFlags = 0;
    result.unk_20 = -1;
    result.unk_24 = -1;
}

// 0x3af77c Character::F_ApplyScrollingCombatText(Character::AttackResult const&, Character*, Character*)
// PARTIAL: as the GameObject overload above. Followers return at once. The damage and block/dodge/fear/slow texts need
// StringManager (dword_99F760, no accessor declared) and the GameObject position floats at +0x14c and +0x158 (no names).
void Character::F_ApplyScrollingCombatText(const AttackResult& result, Character* attacker, Character* target) {
    (void)result;
    (void)attacker;
    if (target->IsFollower()) {
        return;
    }
    // PARTIAL: the miss/block/dodge/critical/injure/stun/fear/slow/dot texts and the damage value are not reproduced.
}

// 0x3afd38 Character::F_ApplyCombatSound(Character::AttackResult const&, GameObject*, Character*)
// PARTIAL: the MP_MinimalRandoms switch is read and gates the sound choice, which is reproduced here. The sound itself
// needs VoxSoundManager::Play3D and VoxSoundManager::s_instance (not declared in vox-audio) and the CharSounds rows
// (GetCharSounds returns an untyped pointer; the row layout is not declared). Nothing is played yet.
void Character::F_ApplyCombatSound(const AttackResult& result, GameObject* attacker, Character* target) {
    (void)result;
    (void)attacker;
    (void)target;
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("MP_MinimalRandoms")) {
        return;
    }
    // PARTIAL: the death, injured and hit sound choice (IsDead on target selects the death rows; the damage rows are
    // gated on result damage > 0) and VoxSoundManager::Play3D are not reproduced.
}

// 0x3afee0 Character::F_ApplyCombatSound(Character::AttackResult const&, Character*, Character*)
// PARTIAL: as the GameObject overload. The Character overload plays through the attacker's CharSounds rows (the same
// untyped layout) and VoxSoundManager::Play3D, neither declared.
void Character::F_ApplyCombatSound(const AttackResult& result, Character* attacker, Character* target) {
    (void)result;
    (void)attacker;
    (void)target;
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("MP_MinimalRandoms")) {
        return;
    }
    // PARTIAL: the attacker/target sound choice and VoxSoundManager::Play3D are not reproduced.
}

// 0x3b01b8 Character::F_ApplyResult(Character::AttackResult const&, GameObject*, Character*, bool)
// PARTIAL, in order of the original: (1) the network notification (CMsgAttackResult sent through GetMessaging when
// COnline::isOnlineGame_ (+0x05, no getter) is set, the attacker's +0x110 is -1 and suppressNetMessage is false) is not
// reproduced. (2) The blood effect (VisualFXManager::PlayAnimFXSet on Singleton<VisualFXManager>::s_inst, no accessor) is
// not reproduced. (3) The HitFor call, which runs when the target's +0x110 is -1 (no name declared for that GameObject
// word), is not reproduced. The debug switch reads, the NoDamages and GOD gates, the state updates on the result, the
// injure state, the scrolling text, the sound and the OnCombatResults hook are reproduced.
void Character::F_ApplyResult(const AttackResult& result, GameObject* attacker, Character* target, bool suppressNetMessage) {
    (void)suppressNetMessage;
    // The original writes the dead-state clears back into the caller's result. The header takes the result const, so that
    // the overload does not become ambiguous with the Character overload for existing callers; the caller's object is
    // not const, so the write lands in the caller's result as in the original.
    AttackResult& writable = const_cast<AttackResult&>(result);

    // The NoDamages switch and the GOD switch (with the player test and the target's m_freeMana) skip the damage part.
    bool skipDamage = ReadSwitch("NoDamages");
    if (!skipDamage) {
        const bool godSwitch = ReadSwitch("GOD");
        skipDamage = (godSwitch && target->IsPlayer()) || target->m_freeMana != 0;
    }

    if (!skipDamage && result.m_damage > 0) {
        // PARTIAL: the blood effect (attack flag 0x200000), the store of 0 to CharStateMachine::m_deadAltAnim (+0x3f,
        // private in dh2/CharStateMachine.hpp: layout request for a setter) and HitFor(damage, attacker) when the target's
        // +0x110 word is -1 (no name declared) are not reproduced, see the comment above.
        if (target->IsDead()) {
            writable.m_resultFlags &= ~static_cast<std::uint32_t>(0x60);  // scare and stun (byte 0x9F mask)
            writable.m_resultFlags &= ~kResultSlow;                        // bit 0 of +0x19
        }
    }

    if (!target->IsDead() && (result.m_resultFlags & kResultInjure) != 0) {
        static_cast<CharStateMachine*>(target)->SM_SetInjureState(static_cast<void*>(attacker), false);
        ReadTraceSwitch("isTracingChar_Attack");
    }
    target->CancelSneaking();
    F_ApplyScrollingCombatText(result, attacker, target);
    F_ApplyCombatSound(result, attacker, target);
    // The target's CharAI is told the result (vtable slot 46 of CharAI, OnCombatResults). The hook takes void* and does
    // not write the result, so the const is cast away for the call only.
    static_cast<CharAI*>(target)->OnCombatResults(attacker, const_cast<AttackResult*>(&result));
}

// 0x3b061c Character::F_GOAttack(Character::AttackResult&, GameObject*, Character*, int, int)
// PARTIAL: the damage roll and the immunity timer read the Arrays::GameObjectDamager row (24-byte stride; the word at
// +4 is the damage type, +8 the immunity time, +12 and +16 the damage range, +20 the attack flags). The row has no
// declared struct layout (layout request: an Arrays::GameObjectDamager row struct with those four words), so only the
// asserts, the trace read and the reset of the result are reproduced.
void Character::F_GOAttack(AttackResult& result, GameObject* attacker, Character* target, int a, int b) {
    (void)a;
    (void)b;
    if (attacker == nullptr) {
        ReportAssertFailure("attacker", kCharacterFormulasFile, 665);
    }
    if (target == nullptr) {
        ReportAssertFailure("defender", kCharacterFormulasFile, 666);
    }
    ReadTraceSwitch("isTracingChar_Attack");
    _F_ResetResult(result);
}

// 0x3b10b4 Character::F_ApplyResult(Character::AttackResult const&, Character*, Character*, bool)
// PARTIAL, in order of the original: (1) the network notification (CMsgAttackResult and the GetOnline()+0x05 test, see
// F_ApplyResult above) is not reproduced. (2) The hit counter (unk_14d0 of the attacker, a 16-bit word, cleared on a
// hit) is reproduced. (3) The GOD option read through Singleton<Application>::s_inst.IsSavedOptionOn("GOD") has no
// accessor for the Application singleton, so it counts as off here; the NoDamages and GOD switches and the target's
// m_freeMana gate are reproduced. (4) The gold steal (attack flag 0x400000, ItemInventory::AddGold on the attacker),
// the difficulty scaling of the damage (PlayerManager word +1732 read through dword_99F76C, DesignSettingsTable floats
// at +52 and +56), the threat (CharAI::AI_AddAggro returns void in the header, the original tests its float result for
// the isTracingThreatChange switch), the blood effect, the camera shake (Application::GetCurrentLevel()+0x128 is not
// declared) and the PlayerStatManager counters (Singleton<PlayerStatManager>::s_inst, no accessor) are not reproduced.
void Character::F_ApplyResult(const AttackResult& result, Character* attacker, Character* target, bool suppressNetMessage) {
    (void)suppressNetMessage;

    // The attacker's hit counter (a 16-bit word at +0x14d0) restarts on a hit and counts up on a miss.
    attacker->unk_14d0 = (result.m_resultFlags & 3) != 0 ? 0 : static_cast<std::int16_t>(attacker->unk_14d0 + 1);

    // The NoDamages switch and the GOD switch (with the player test and the target's m_freeMana) skip the damage part.
    bool skipDamage = ReadSwitch("NoDamages");
    if (!skipDamage) {
        const bool godSwitch = ReadSwitch("GOD");
        // PARTIAL: || Application::IsSavedOptionOn("GOD") is not reproduced (no Application singleton accessor).
        skipDamage = (godSwitch && target->IsPlayer()) || target->m_freeMana != 0;
    }

    if (!skipDamage && result.m_damage > 0) {
        // PARTIAL: the store to CharStateMachine::m_deadAltAnim (private, layout request for a setter), the gold steal
        // (attack flag 0x400000), the damage scaling, the threat, the blood effect, HitFor (when the target's +0x110
        // word is -1), the camera shake and the state machine branches are not reproduced (see above).
        ReadTraceSwitch("isTracingChar_Attack");
    }

    // The attacker's hit counts drain into its own life and mana (attack words +0x10 and +0x14).
    attacker->RegenHP(result.unk_10);
    attacker->RegenMP(result.unk_14);
    target->CancelSneaking();
    F_ApplyScrollingCombatText(result, attacker, target);
    F_ApplyCombatSound(result, attacker, target);
    if ((result.m_attackFlags & kAttackDot) == 0) {
        // The const_cast only passes the result to the CharAI hook, which takes void*; the hook does not write it.
        AttackResult* const hookResult = const_cast<AttackResult*>(&result);
        static_cast<CharAI*>(attacker)->OnCombatResults(attacker, target, hookResult);
        static_cast<CharAI*>(target)->OnCombatResults(attacker, target, hookResult);
    }
}

// 0x3b2638 Character::_F_CalculateResult(Character::AttackResult&, Character*, Character*, int, int, int, int)
// The original is a chain of request bits, each rolling one d100 (Random::GetRandom(100)) and calling one CF__Calc*
// formula with the roll shifted to fixed point (<< 8), except where noted. The damage split at the end is the one the
// original writes into the result; the caller's a and b are stored in the words +0x1c, +0x20 and +0x24.
//   requests & 0x5: miss/dodge roll, mode 0; requests & 0xA: miss/dodge roll, mode 2 (the roll runs only when the mode 0
//   roll did not; the result is cleared when an attacker or defender is missing)
//   0x10 block; 0x20 critical mode 0, 0x40 critical mode 2; 0x80 injure mode 0, 0x100 injure mode 2; 0x200 push mode 0,
//   0x400 push mode 2; 0x800 stun mode 0, 0x1000 stun mode 2; 0x2000 fear mode 0, 0x4000 fear mode 2 (fear 2 only when
//   fear 0 did not run); 0x8000 slow mode 0, 0x10000 slow mode 2 (slow 2 only when slow 0 did not run)
//   0x20000 damage split mode 0, 0x40000 damage split mode 2, 0x80000 damage with the fixed damage (mode 3)
// The critical roll is reused by the injure roll when it ran (the original's v14), otherwise the injure roll draws its own.
void Character::_F_CalculateResult(AttackResult& result, Character* attacker, Character* target, int requests,
                                   int stored20, int element, int fixedDamage) {
    PushProfilingContext("Character::_F_CalculateResult");
    _F_ResetResult(result);
    result.m_attackFlags = static_cast<std::uint32_t>(requests);
    result.unk_20 = stored20;
    result.unk_24 = element;
    CF_SetCombatants(attacker, target, element, (requests & static_cast<int>(kAttackOffHand)) != 0,
                     (requests & static_cast<int>(kAttackSkill)) != 0);

    const bool haveCombatants = s_attacker != nullptr && s_defender != nullptr;

    // Miss and dodge. The mode-0 roll and the mode-2 roll are alternatives; both end the same way.
    if ((requests & 0x5) != 0) {
        const int roll = RollPercent();
        if (haveCombatants) {
            const MissOrDodgeResult r = CF__CalcMissOrDodge(s_attacker, s_defender, roll << 8, 0, s_element, s_offHanded);
            result.m_resultFlags = (result.m_resultFlags & ~0x3u) | (r.m_missed ? kResultMissed : 0u) |
                                   (r.m_dodged ? kResultDodged : 0u);
        } else {
            result.m_resultFlags &= ~0x3u;
        }
    } else if ((requests & 0xA) != 0) {
        const int roll = RollPercent();
        if (haveCombatants) {
            const MissOrDodgeResult r = CF__CalcMissOrDodge(s_attacker, s_defender, roll << 8, 2, s_element, s_offHanded);
            result.m_resultFlags = (result.m_resultFlags & ~0x3u) | (r.m_missed ? kResultMissed : 0u) |
                                   (r.m_dodged ? kResultDodged : 0u);
        } else {
            result.m_resultFlags &= ~0x3u;
        }
    }
    if ((result.m_resultFlags & 0x3) != 0) {
        PopProfilingContext("Character::_F_CalculateResult");
        return;
    }

    // Block.
    if ((requests & 0x10) != 0) {
        const int roll = RollPercent();
        const bool blocked = haveCombatants ? CF__CalcBlock(s_attacker, s_defender, roll << 8, 0) : false;
        SetResultFlag(result, kResultBlocked, blocked);
    }

    // Critical hit. The roll is kept for the injure roll below (v14).
    int critRoll = -1;
    if ((requests & 0x20) != 0 || (requests & 0x40) != 0) {
        const int mode = (requests & 0x20) != 0 ? 0 : 2;
        critRoll = RollPercent() << 8;
        const bool critical = haveCombatants ? CF__CalcCrit(s_attacker, s_defender, critRoll, mode) : false;
        SetResultFlag(result, kResultCritical, critical);
    }

    // Injure (hurt). Reuses the critical roll when one ran.
    if ((requests & 0x80) != 0 || (requests & 0x100) != 0) {
        const int mode = (requests & 0x80) != 0 ? 0 : 2;
        if (critRoll == -1) {
            critRoll = RollPercent() << 8;
        }
        const bool hurt = haveCombatants ? CF__CalcHurt(s_attacker, s_defender, critRoll, mode) : false;
        SetResultFlag(result, kResultInjure, hurt);
    }

    // Push (knock-back).
    if ((requests & 0x200) != 0 || (requests & 0x400) != 0) {
        const int mode = (requests & 0x200) != 0 ? 0 : 2;
        const int roll = RollPercent() << 8;
        const bool push = haveCombatants ? CF__CalcPush(s_attacker, s_defender, roll, mode) : false;
        SetResultFlag(result, kResultPush, push);
    }

    // Stun.
    if ((requests & 0x800) != 0 || (requests & 0x1000) != 0) {
        const int mode = (requests & 0x800) != 0 ? 0 : 2;
        const int roll = RollPercent() << 8;
        const bool stun = haveCombatants ? CF__CalcStun(s_attacker, s_defender, roll, mode) : false;
        SetResultFlag(result, kResultStun, stun);
    }

    // Fear. Fear 2 is only reached when fear 0 did not run.
    if ((requests & 0x2000) != 0) {
        const int roll = RollPercent() << 8;
        const bool fear = haveCombatants ? CF__CalcFear(s_attacker, s_defender, roll, 0) : false;
        SetResultFlag(result, kResultScare, fear);
    } else if ((requests & 0x4000) != 0) {
        const int roll = RollPercent() << 8;
        const bool fear = haveCombatants ? CF__CalcFear(s_attacker, s_defender, roll, 2) : false;
        SetResultFlag(result, kResultScare, fear);
    }

    // Slow. Slow 2 is only reached when slow 0 did not run.
    if ((requests & 0x8000) != 0) {
        const int roll = RollPercent() << 8;
        const bool slow = haveCombatants ? CF__CalcSlow(s_attacker, s_defender, roll, 0) : false;
        SetResultFlag(result, kResultSlow, slow);
    } else if ((requests & 0x10000) != 0) {
        const int roll = RollPercent() << 8;
        const bool slow = haveCombatants ? CF__CalcSlow(s_attacker, s_defender, roll, 2) : false;
        SetResultFlag(result, kResultSlow, slow);
    }

    ReadTraceSwitch("isTracingChar_Attack");

    // Damage. The three request bits write the damage split, the damage split (mode 2) or the fixed damage.
    if ((requests & 0x20000) != 0 || (requests & 0x40000) != 0) {
        const int mode = (requests & 0x20000) != 0 ? 0 : 2;
        CombatDamage damage{};
        if (haveCombatants) {
            damage = CF__CalcDamage(s_attacker, s_defender, 0, mode, s_element, s_offHanded, s_isSkill);
        }
        result.m_damage = damage.m_damage;
        result.unk_04 = damage.m_dot.m_element;
        result.unk_08 = damage.m_dot.m_gate;
        result.unk_0c = damage.m_dot.m_damage;
        result.unk_10 = damage.m_value132;
        result.unk_14 = damage.m_value133;
        if ((requests & 0x20000) != 0) {
            result.unk_24 = damage.m_element;
        }
    } else if ((requests & 0x80000) != 0) {
        if (haveCombatants) {
            result.m_damage = CF__CalcDamage(s_attacker, s_defender, fixedDamage, 3, s_element, s_offHanded, s_isSkill).m_damage;
        } else {
            result.m_damage = 0;
        }
    }

    PopProfilingContext("Character::_F_CalculateResult");
}

// 0x3b2e68 Character::F_DotAttack(Character::AttackResult&, Character*, Character*, int, int)
// The damage-over-time request: _F_CalculateResult with the request word 0x20080000 (0x20000000 DOT, 0x80000 fixed
// damage mode 3), the stored word -1, the caller's b as the element and the caller's a as the fixed damage.
void Character::F_DotAttack(AttackResult& result, Character* attacker, Character* target, int a, int b) {
    if (attacker == nullptr) {
        ReportAssertFailure("attacker", kCharacterFormulasFile, 649);
    }
    if (target == nullptr) {
        ReportAssertFailure("defender", kCharacterFormulasFile, 650);
    }
    ReadTraceSwitch("isTracingChar_Attack");
    _F_CalculateResult(result, attacker, target, 537395200, -1, b, a);
}
