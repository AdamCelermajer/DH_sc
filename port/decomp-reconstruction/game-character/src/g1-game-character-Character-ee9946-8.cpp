// Batch g1-game-character-Character-ee9946-8 (part 9 of 20 of Character): the faery accessors and level changes,
// the immunity-checked attack roll, the scrolling combat text and sound helpers, the result application and the
// combat result calculation. Declarations come from dh2/Character.hpp and dh2/global/CombatFormulas.hpp.
// PARTIAL bodies name what is missing in a PARTIAL comment; the ledger rows in
// game-character/ledger.d/g1-game-character-Character-ee9946-8.csv repeat it.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core-platform/memory.hpp"            // gAssertLevel
#include "dh2/global/runtime_support.hpp"      // PushProfilingContext, PopProfilingContext
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Random.hpp"
#include "dh2/global/CombatFormulas.hpp"       // s_attacker, s_defender, s_element, s_offHanded, s_isSkill, CF_SetCombatants
#include "dh2/global/cf_calc.hpp"              // CF__Calc* rolls, CombatDamage, MissOrDodgeResult
#include "dh2/global/charai_ai.hpp"            // ApplicationGetPyDataConstants (dword_99F758)
#include "pydata-accessors/w7_pydata12.hpp"    // PyDataConstants::getConstant
#include "pydata-accessors/structs_records.hpp"  // Structs::FaeryList, Structs::Faery
#include "pydata-accessors/w7_arrays27.hpp"    // Arrays::GameObjectDamager::members (raw 24-byte rows)
#include "dh2/Singleton.hpp"                   // Singleton<VisualFXManager>::s_inst
#include "dh2/VisualFXManager.hpp"             // PlayAnimFXSet
#include "pydata-accessors/w7_arrays27.hpp"      // Arrays::FaeryListTable, Arrays::FaeryTable

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

// One 32-bit word of row `index` of Arrays::GameObjectDamager, read at its file layout: the rows are 24 bytes and the word
// at byte offset `offset` (4 .. 20) is decoded field by field, never cast to a host struct.
std::int32_t DamagerRowWord(int index, int offset) {
    std::int32_t value = 0;
    std::memcpy(&value, Arrays::GameObjectDamager::members + 24 * index + offset, sizeof(value));
    return value;
}

}  // namespace

// 0x3ae60c Character::GetFaeryCharacter(void)
// Returns the word at +0x420 of Character, which is CharAI::unk_58 (CharAI +0x58). ChangeFaery calls the Character members
// GetCharModelName and the CharAnimator base on this pointer, so it is the faery Character, cast here to Character*.
Character* Character::GetFaeryCharacter() {
    return static_cast<Character*>(CharAI::unk_58);
}

// 0x3ae614 Character::GetFaeryCharacter(void) const
const Character* Character::GetFaeryCharacter() const {
    return static_cast<const Character*>(CharAI::unk_58);
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
// PARTIAL: the first-faery tutorial branch is not reproduced. It runs when state is 1, faery is not 0, COnline::isOnlineGame_
// (+0x05) is clear, the difficulty is 0 and the SavegameManager tutorial byte m_tutorials[2] (+0x2b) is set. It then starts
// cinematic_Tuto_faery (ScriptManager::s_inst, GetIDFromName and StartScript are declared), clears the byte and calls
// saveSettings. The blocker is isOnlineGame_: it is a protected member of COnline (net-session-online/c_online.hpp) with no
// getter. The state change below is complete.
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
// Sets the current faery and updates the skills. Then, when there is a faery character (GetFaeryCharacter), its visual
// is set from its own model name (no xref, forced) and its CharAnimator adds the animation set to the render object.
void Character::ChangeFaery(unsigned int faery) {
    const int difficulty = SG_GetGameDifficulty();
    if (static_cast<unsigned int>(SG_GetFaerieCount(difficulty)) <= faery) {
        ReportAssertFailure("faeryIdx < SG_GetFaerieCount(diff)", kCharacterFaeryFile, 107);
    }
    SG_SetCurrentFaerie(faery, difficulty);
    CharAI::UpdateAllSkills();
    Character* faeryCharacter = GetFaeryCharacter();
    if (faeryCharacter) {
        faeryCharacter->GameObject::SetVisualObject(faeryCharacter->GetCharModelName(), nullptr, true);
        faeryCharacter->CharAnimator::ANIM_AddSetToRenderObject();
    }
}

// 0x3aea9c Character::IsCastingPossible(void)
bool Character::IsCastingPossible() {
    if (!CTRLIsAllowed()) {
        return false;
    }
    return CharAI::AI_IsSpellUsable();
}

// 0x3aeac0 Character::GetCharFaery(int) const
// The faery list row (GetCharFaeryList) gives ListSize (m_w1, must equal the FaeryTypes COUNT) and the List (m_w2, an
// int32 array). List[faeryId] is the index of the Arrays::FaeryTable row, whose Type (m_w8) must equal faeryId. The three
// ASSERTs are reproduced. The List index is read without a bound check, as in the original; the first ASSERT limits
// faeryId to the COUNT.
const void* Character::GetCharFaery(int faeryId) const {
    const int faeryTypeCount = ApplicationGetPyDataConstants()->getConstant("FaeryTypes", "COUNT");
    if (faeryId < 0 || faeryId >= faeryTypeCount) {
        ReportAssertFailure(
            "faeryId >= 0 && faeryId < Application::GetInstance()->GetConstantsMgr()->getConstant(\"FaeryTypes\", \"COUNT\")",
            kCharacterFaeryFile, 62);
    }
    const Structs::FaeryList* faeryList = static_cast<const Structs::FaeryList*>(GetCharFaeryList());
    if (static_cast<int>(faeryList->m_w1) != faeryTypeCount) {
        ReportAssertFailure(
            "charFaeryList.ListSize == Application::GetInstance()->GetConstantsMgr()->getConstant(\"FaeryTypes\", \"COUNT\")",
            kCharacterFaeryFile, 63);
    }
    const int faeryIndex = static_cast<const std::int32_t*>(faeryList->m_w2)[faeryId];
    const Structs::Faery* faery = reinterpret_cast<const Structs::Faery*>(Arrays::FaeryTable::members) + faeryIndex;
    if (faery->m_w8 != faeryId) {
        ReportAssertFailure("Arrays::FaeryTable::members[charFaeryList.List[faeryId]].Type == faeryId", kCharacterFaeryFile, 64);
    }
    return faery;
}

// 0x3af0c8 Character::F_ApplyScrollingCombatTextXP(Character*, int, int, int)
// PARTIAL: nothing is drawn. The original formats the GAMEPLAYMENUS_REWARD_XP string (StringManager, dword_99F760) with
// the xp amount and places it above the target (the z extent of the local AABB, m_relAabb[5] - m_relAabb[2], is added to the
// target position). The amount is the first explicit argument, which dh2/Character.hpp declares as Character* target (rename
// candidate: int amount), so it cannot be passed to StringManager::parse until that is retyped. The text call
// (FlashAnimManager::PlayScrollingCombatText) takes a glitch::core::vector3d<float>, and dh2/FlashAnimManager.hpp cannot be
// included beside this file's Character chain (see F_ApplyScrollingCombatText below). styleId and colorId are the styleId
// and color of that call; the fifth argument is never read.
void Character::F_ApplyScrollingCombatTextXP(Character* target, int styleId, int colorId, int unusedArg) {
    (void)target;
    (void)styleId;
    (void)colorId;
    (void)unusedArg;
}

// 0x3aef68 Character::F_ApplyScrollingCombatTextGold(Character*, int)
// PARTIAL: nothing is drawn. The original formats the ITEMS_GOLD string (StringManager, dword_99F760) with the gold amount,
// which is the first explicit argument (declared Character* target in dh2/Character.hpp; rename candidate: int amount), and
// places it above the target with the anim_reward_gold style and the QR_Gold colour. The amount cannot be passed to
// StringManager::parse until that declaration is retyped, and the text call needs glitch::core::vector3d (see
// F_ApplyScrollingCombatText below). The second argument is never read.
void Character::F_ApplyScrollingCombatTextGold(Character* target, int gold) {
    (void)target;
    (void)gold;
}

// 0x3af1fc Character::F_ApplyScrollingCombatText(Character::AttackResult const&, GameObject*, Character*)
// PARTIAL: a follower shows nothing and returns here. A positive damage would show its value (damage >> 8) with the
// anim_sct_normaldamage style at the target's text anchor (GetTargetPosition, z raised by m_relAabb[5] - m_relAabb[2],
// GameObject +0x158 and +0x14c), in PlayerDamageColor for a player and DamageColor otherwise. Nothing is drawn:
// FlashAnimManager::PlayScrollingCombatValue takes a glitch::core::vector3d<float>, and dh2/FlashAnimManager.hpp
// (glitch-shared/glitch_core.hpp) cannot be included in this translation unit, because the Character chain already defines
// glitch::core::vector3d from core-platform/deps/glitch_core.hpp (through Billboard.hpp and SimpleTypeProperty.hpp).
void Character::F_ApplyScrollingCombatText(const AttackResult& result, GameObject* attacker, Character* target) {
    (void)result;
    (void)attacker;
    if (target->IsFollower()) {
        return;
    }
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
// PARTIAL: followers return at once. Nothing is drawn. The original chooses one text from the result flags: miss (bit 0 of
// +0x18) and dodge (bit 1) show their text and return; block (bit 2) shows its text and goes on; fear (bit 5) or else slow
// (bit 8 of +0x19) show their text when the attacker's sheet value (PROPS 187 or 143 for fear, 189 or 146 for slow, chosen
// by bit 16 of m_attackFlags) is non-zero after >> 8. Then a positive damage shows its value (damage >> 8) with the style of
// the attack flags (dot, off-hand, critical) and the colour for the target (player or not, critical or not). The drawing
// calls take a glitch::core::vector3d<float>, which clashes with the one in the Character chain (see the GameObject overload
// above), so the text is not drawn.
void Character::F_ApplyScrollingCombatText(const AttackResult& result, Character* attacker, Character* target) {
    (void)result;
    (void)attacker;
    if (target->IsFollower()) {
        return;
    }
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
// PARTIAL, in order of the original: (1) the network notification is not written. It runs when COnline::isOnlineGame_
// (+0x05) is set, suppressNetMessage is false and the attacker's m_netOwnerId (+0x110) is -1: CMsgAttackResult of the
// attacker and defender unk_108 ids through GetMessaging. isOnlineGame_ is protected in net-session-online/c_online.hpp
// with no getter, and CMsgAttackResult's header (net-session-online/deps/character_attack_result.hpp) declares a
// namespace Character stand-in that clashes with dh2/Character.hpp. The NoDamages and GOD gates, the blood effect
// (attack flag 0x200000, VisualFXManager::PlayAnimFXSet on the target position), the store of false to
// CharStateMachine::m_deadAltAnim, HitFor when the target's m_netOwnerId is -1, the dead-state clears, the injure state,
// CancelSneaking, the scrolling text, the sound and the CharAI OnCombatResults hook are reproduced. The Hex-Rays return
// value is a leftover of the hook; declared void.
void Character::F_ApplyResult(const AttackResult& result, GameObject* attacker, Character* target, bool suppressNetMessage) {
    (void)suppressNetMessage;
    // The original writes the dead-state clears back into the caller's result. The header takes the result const, so that
    // the overload does not become ambiguous with the Character overload for existing callers; the caller's object is
    // not const, so the write lands in the caller's result as in the original.
    AttackResult& writable = const_cast<AttackResult&>(result);
    // PARTIAL: the network notification (see above) is not written.

    // The NoDamages switch and the GOD switch (with the player test and the target's m_freeMana) skip the damage part.
    bool skipDamage = ReadSwitch("NoDamages");
    if (!skipDamage) {
        const bool godSwitch = ReadSwitch("GOD");
        skipDamage = (godSwitch && target->IsPlayer()) || target->m_freeMana != 0;
    }

    if (!skipDamage && result.m_damage > 0) {
        if ((result.m_attackFlags & 0x00200000u) != 0) {
            // unk_24 == -1 selects the target's blood effect; otherwise the attack's own effect table row (+124).
            const int fxBlood = result.unk_24 == -1 ? target->GetFXBlood() : result.unk_24 + 124;
            Singleton<VisualFXManager>::s_inst.PlayAnimFXSet(fxBlood, target->GetTargetPosition(), nullptr, nullptr);
        }
        // The original clears m_deadAltAnim (CharStateMachine +0x3f, Character +0x53b) before the HitFor test.
        target->CharStateMachine::m_deadAltAnim = false;
        if (target->m_netOwnerId == -1) {
            target->HitFor(static_cast<unsigned int>(result.m_damage), attacker);
        }
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
// PARTIAL. The asserts and the trace read, the roll and the immunity gate, the injure flag and the result words are
// reproduced. The damage row is Arrays::GameObjectDamager row a, read at its 32-bit file layout (24-byte rows: +4 damage
// type, +8 immunity time, +12 and +16 the damage range maximum and minimum, +20 the attack flags; DamagerRowWord).
// The roll is minimum + Random(maximum + 1 - minimum) in fixed point; the original passes the damage type as the second
// argument of that three-argument call, and the stream the clone draws from is not verified (this body draws from the
// ordinary stream, as RollPercent does). While the target's immunity timer for the damage type runs the damage is zero,
// otherwise the timer is started with the row's immunity time. The injure flag (0x10) is set when the damage is positive
// and the target's property 134 is below b << 8. The result's unk_24 word is the row's +20 word. The Hex-Rays return
// value is a leftover; declared void.
void Character::F_GOAttack(AttackResult& result, GameObject* attacker, Character* target, int a, int b) {
    if (attacker == nullptr) {
        ReportAssertFailure("attacker", kCharacterFormulasFile, 665);
    }
    if (target == nullptr) {
        ReportAssertFailure("defender", kCharacterFormulasFile, 666);
    }
    ReadTraceSwitch("isTracingChar_Attack");

    const std::int32_t damageType = DamagerRowWord(a, 4);
    const std::int32_t immunityTime = DamagerRowWord(a, 8);
    const std::int32_t rangeMax = DamagerRowWord(a, 12);
    const std::int32_t rangeMin = DamagerRowWord(a, 16);
    const std::int32_t attackWord = DamagerRowWord(a, 20);

    const std::uint32_t roll = Random::GetRandom(rangeMax + 1 - rangeMin, false);
    const bool immunityApplies = immunityTime != 0 && damageType >= 0;
    bool immune = false;
    if (immunityApplies) {
        TimerUtil* immunityTimer = target->GetImmunityTimer(damageType);
        if (immunityTimer->GetRemainingTime() != 0) {
            immune = true;
        } else {
            immunityTimer->Start(static_cast<unsigned int>(immunityTime));
        }
    }

    std::int32_t damage = 0;
    if (!immune) {
        damage = (static_cast<std::int32_t>(roll) + rangeMin) * 256;
    }
    _F_ResetResult(result);
    if (damage > 0) {
        const int defence = target->CharProperties::_GetProperty(target->CharProperties::m_props, 134);
        if (defence < b * 256) {
            result.m_resultFlags |= kResultInjure;
        }
    }
    result.m_damage = damage;
    result.unk_24 = attackWord;
}
// 0x3b10b4 Character::F_ApplyResult(Character::AttackResult const&, Character*, Character*, bool)
// PARTIAL, in order of the original: (1) the network notification (CMsgAttackResult and the GetOnline()+0x05 test, see
// F_ApplyResult above) is not reproduced. (2) The hit counter (unk_14d0 of the attacker, a 16-bit word, cleared on a
// hit) is reproduced. (3) The GOD option read through Singleton<Application>::s_inst.IsSavedOptionOn("GOD") is not
// written here, so it counts as off; the NoDamages and GOD switches and the target's m_freeMana gate are reproduced.
// (4) The gold steal (attack flag 0x400000, ItemInventory::AddGold on the attacker, declared), the difficulty scaling of
// the damage (PlayerManager word +1732 read through dword_99F76C = Application +0x40, needs dh2/PlayerManager.hpp, which
// does not compile beside Character.hpp), the threat (CharAI::AI_AddAggro now returns the float the original tests for
// the isTracingThreatChange switch; not yet read here), the blood effect, the camera shake (Application::GetCurrentLevel
// is declared; the CameraLevel fields are not) and the PlayerStatManager counters are not reproduced. The store to
// CharStateMachine::m_deadAltAnim (public now) and HitFor on the target's m_netOwnerId are not yet written in this overload.
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
        // PARTIAL: the store to CharStateMachine::m_deadAltAnim (public now, not yet written in this overload), the gold steal
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
