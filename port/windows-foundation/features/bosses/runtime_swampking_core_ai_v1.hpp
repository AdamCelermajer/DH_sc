#pragma once
// B155: the Mud Lord (CharacterTable SwampKing, AI row 63 "SwampKing", authored AI script swampking_core.luac).
//
// Evidence (coordination/v21.7/B155-B156-report.md):
//  - The AI script data/scripts/ai/swampking_core.luac is plain Lua text. This module reads its constants and DoSkill
//    arguments from that text (parse_swampking_core_ai_script_v1) and translates its callbacks one to one, the same way
//    runtime_ranged_skill_monster_v1 (B154) and monster_decisions translate witch.luac / monster.luac. Every actor whose
//    AI row script has this shape gets the owner; no actor name is keyed.
//  - DoSkill(n) indexes the actor's SkillTree list (Character::_DoSkill 0x3b8bd8): SwampKingSkills =
//    [106 SwampKingAttack1 swamp_king_attack_1, 107 SwampKingAttack2 swamp_king_attack_2,
//     108 SwampKingAttack3 swampking_atk_spit, 109 SwampKingAttack4 swamp_king_attack_4]. Each SkillTable row's Anim root
//    (665..668) is played in Skill6 (SM_SetSkillState 0x3c6670); every attack clip carries do_skill.
//  - Engine semantics used (IDA, libDungeonHunter2.so): Lua OnEndOfAnim is raised by CharAnimator events 34/35
//    (CharAI::RaiseAIEvent 0x3cbb34) at the end of any sequence; PlayAnim (Character::_PlayAnim 0x3ba388) forces state 14
//    (CSAnim) and returns to Idle at the clip end only when its third argument is true (CSAnim::OnEvent 0x3c1a14);
//    SetIsTargetable (0x3b7a64) toggles Character+1045, read by IsInteractive (player targeting); SetActorPosition from an
//    AI script is GameObject::_SetPosition 0x3928f0 (teleport, no state change); range events 14/15/16 are raised on every
//    CharAI update while the target is interactive and in sight (CharAI::_UpdateTarget 0x3cb908, CanRangeAttack ->
//    no melee event); OnEnemySpotted comes from _UpdateAggro 0x3cf3f0 (no target, ViewRadius, about every 500 ms);
//    StartTimerCB/StopTimerCB (_commons) are CharTimers slots advanced in Character::Update even when CharAI is
//    blocked; AI_IsSkillUsable 0x3d8358 rejects DoSkill while already in Skill/Cast/Dead/Scared/Stunned/KnockedBack/
//    Spawn/Limbus (accepted from Anim14).
#include "runtime_boss_encounter_plan_v1.hpp"
#include "../enemy_ai/runtime_ranged_skill_monster_v1.hpp"

#include <array>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::bosses {

using dh2::bosses::runtime_v1::CloseAttackRulesV1;

// Facts read from the authored AI script text. All are required.
struct SwampkingCoreAiScriptV1 {
    double attack_delay_ms = 0;          // ATTACK_DELAY: repeating isAttackReady timer
    double goto_dive_delay_ms = 0;       // GOTODIVE_DELAY: repeating GoToDivePattern timer
    double dive_retry_ms = 0;            // GoToDivePattern: StartTimerCB(500, false, GoToDivePattern) while not Idle
    double in_dive_pattern_delay_ms = 0; // INDIVEPATTERN_DELAY: repeating SetSwampkingInDiveState timer
    double move_delay_ms = 0;            // OnEndOfAnim(dive): StartTimerCB(500, false, MoveToPosition)
    double emerge_delay_ms = 0;          // DIVE_DELAY: StartTimerCB(DIVE_DELAY, false, SetSwampkingInEmergeState)
    double retarget_delay_ms = 0;        // StartTimerCB(3000, false, TargetAgain)
    double decision_delay_ms = 0;        // OnEndOfAnim(emerge): StartTimerCB(500, false, DiveAttackDecision)
    double skill2_delay_ms = 0;          // OnEndOfAnim(intimidate): StartTimerCB(500, false, function() DoSkill(2) end)
    int phase2_pct = 0, weak_pct = 0, dives_before_reset = 0;
    CloseAttackRulesV1 close;
    int spit_skill = -1, emerge_skill = -1, intimidate_skill = -1;
    std::vector<std::string> spawn_points; // sk_spawn_points (first = phase-2 / weak spawn point)
    std::string dive_anim, intimidate_anim; // GetPyOID("AnimDict", ...)
    std::string dive_sound;
    std::vector<std::string> intimidate_sounds; // GetRand(1,3) selects one
    std::string spotted_music, died_stop_music, died_music;
    std::int32_t spotted_music_fade_ms = 0, died_stop_fade_ms = 0, died_music_fade_ms = 0;
};
bool parse_swampking_core_ai_script_v1(const std::string& text, SwampkingCoreAiScriptV1& output, std::string& error);

enum class BossSkillShapeV1 : std::uint8_t {
    single_target,   // swamp_king_attack_1/2: Pre HasTarget/LookAt, re-search FrontalFirst when dist > RANGE; Skill: roll target
    target_list_aoe, // swamp_king_attack_4: Pre sound + FrontalFirst list within RANGE; Skill: roll every listed enemy, FX, sound
    projectile_aoe,  // swampking_atk_spit: Skill spawns RangeProjectileID; delayed: roll target, timer trap, AOE_RANGE roll
};
// Facts read from an authored skill script text (data/scripts/skills/<script>.luac).
struct BossSkillScriptV1 {
    BossSkillShapeV1 shape = BossSkillShapeV1::single_target;
    float range = 0;                  // RANGE literal (or the CharacterProperties field below)
    std::string range_field;          // RANGE = FromFixed(GetProp(GetPyStruct("CharacterProperties", <field>)))
    std::string class_token, aoe_class_token;
    float aoe_range = 0;
    std::string projectile_field;     // PROJECTILE_ID = GetProp(GetPyStruct("CharacterProperties", <field>))
    std::string pre_sound, skill_fx, skill_sound;
    std::string timer_trap, timer_trap_damager;
};
bool parse_boss_skill_script_v1(const std::string& text, BossSkillScriptV1& output, std::string& error);

struct BossSkillDefinitionV1 {
    int slot = -1;
    std::int32_t skill_table_id = -1, animation_sequence_id = -1, element = -1, class_id = -1, aoe_class_id = -1;
    std::uint32_t skill_flags = 0;
    std::string skill_name, skill_script;
    BossSkillScriptV1 script;
    float range = 0;                  // resolved pre-search range
    std::optional<enemy_ai::SourceProjectileRowV1> projectile;
};

struct SwampkingCoreDefinitionV1 {
    std::string profile, ai_script;
    SwampkingCoreAiScriptV1 ai;
    std::vector<BossSkillDefinitionV1> skills; // indexed by DoSkill slot
    float range_min = 0, range_max = 0;        // CanRangeAttack min/max (raw >> 8)
    std::int32_t dive_anim_id = -1, intimidate_anim_id = -1; // AnimDict ids
    std::string dive_anim_path, intimidate_anim_path;
};

struct SwampkingCoreSourcesV1 {
    const AssetCatalog* assets = nullptr; // scripts (data/scripts/...)
    const spawn::ProfileDerivationTablesV1* tables = nullptr;
    dh2::data::SkillTables::Borrow skills;
    const std::vector<enemy_ai::SourceProjectileRowV1>* projectiles = nullptr;
    const dh2::data::ClassTables* classes = nullptr;
};
// supported=false with a reason for every row that is not a swampking_core actor (or whose sources are missing).
bool derive_swampking_core_v1(const SwampkingCoreSourcesV1&, const std::string& profile, bool& supported,
                              SwampkingCoreDefinitionV1& output, std::string& reason);

// State names under which the SkillTree Anim roots join the actor's derived bank (append_sequence_state_v1) and the
// clip names under which the PlayAnim AnimDict clips join its visual (CombatSessionProfile::sourceAnimationClips).
std::string swampking_core_skill_state_v1(int slot);
std::string swampking_core_playanim_clip_v1(std::int32_t anim_dict_id);
bool append_swampking_core_states_v1(const spawn::ProfileDerivationTablesV1&, const SwampkingCoreDefinitionV1&,
                                     OriginalMeleeActor&, std::string& error);

struct SwampkingCoreServicesV1 {
    std::function<bool(CombatSession&, const ActorState& owner, const ActorState& target, bool& visible, std::string&)> can_see;
    std::function<bool(const std::string& waypoint, std::array<float, 3>& position, bool& found, std::string&)> waypoint;
    std::function<bool(ActorId, const std::array<float, 3>& goal, std::string&)> warp; // SetActorPosition owner
    // Scripted PlayAnim clip on the retained actor (CombatSession::play_actor_clip) and its authored duration.
    std::function<bool(ActorId, const std::string& clip, const std::string& path, std::int32_t& duration_ms, std::string&)> play_clip;
    std::function<bool(ActorId, std::string&)> put_idle;      // SM_SetIdleState
    std::function<float(ActorId)> body_radius;
    std::function<std::int32_t(std::int32_t low, std::int32_t high)> random; // GetRand(low, high), inclusive
    std::function<bool(std::int32_t set, const std::array<float, 3>& at, std::string&)> play_fx;
    std::function<std::int32_t(const std::string& name)> fx_id;
    std::function<bool(const std::string& name, std::string&)> play_sound;
    std::function<bool(std::int32_t sound_row, std::string&)> play_sound_id;
    std::function<bool(const std::string& name, std::int32_t fade_ms, std::string&)> play_music;
    std::function<bool(const std::string& name, std::int32_t fade_ms, std::string&)> stop_music;
    std::function<bool(std::vector<ActorId>& order, std::vector<std::pair<ActorId, bool>>& visible_interactive, std::string&)> search_population;
    // SummonTimerTrap(TimerTraps row name, GameObjectDamager name, position) (optional; logged when absent).
    std::function<bool(const std::string& trap, const std::string& damager, ActorId owner, const std::array<float, 3>& at, std::string&)> summon_timer_trap;
    const dh2::data::CharacterTable* characters = nullptr;
    const dh2::data::ClassTables* classes = nullptr;
    const dh2::data::PropertyRules* property_rules = nullptr;
};

struct SwampkingCoreReportV1 {
    std::size_t spotted = 0, casts = 0, cast_rejected = 0, hits = 0, projectiles = 0, projectile_hits = 0, aoe_hits = 0,
                dives = 0, emerges = 0, relocations = 0, intimidates = 0, phase2 = 0, weak = 0, traps = 0, deaths = 0;
};

class RuntimeSwampkingCoreAiV1 final {
public:
    RuntimeSwampkingCoreAiV1(SwampkingCoreServicesV1, std::map<ActorId, SwampkingCoreDefinitionV1>);
    RuntimeSwampkingCoreAiV1(const RuntimeSwampkingCoreAiV1&) = delete;
    RuntimeSwampkingCoreAiV1& operator=(const RuntimeSwampkingCoreAiV1&) = delete;
    // ai=false: CharAI::Update is blocked (controller lock); CharTimers and projectiles still advance.
    bool update(CombatSession&, double dt, bool ai, std::string& error);
    bool owns(ActorId id) const noexcept { return definitions_.count(id) != 0; }
    // SetIsTargetable(false) (submerged): the player's targeting admission excludes the actor.
    bool untargetable(ActorId id) const noexcept;
    // True once after any SetIsTargetable change (the host refreshes the Session permission).
    bool take_targetability_changed() noexcept { const bool changed = targetability_changed_; targetability_changed_ = false; return changed; }
    bool active_cast(ActorId id, std::uint64_t generation, std::int32_t& skill_table_id) const noexcept;
    std::vector<enemy_ai::RangedProjectileV1> projectiles() const;
    const SwampkingCoreReportV1& report() const noexcept { return report_; }
    std::vector<std::string> take_log();

    // Script-visible state (tests and diagnostics).
    enum class ScriptState : std::uint8_t { idle = 0, intimidate = 1, dive = 2, emerge = 3 };
    struct ActorView { ScriptState state = ScriptState::idle; int phase = 1; bool allowed = false, attack_ready = false,
                       weak = false, can_dive_now = false, targetable = true; int dive_counter = 0; std::size_t timers = 0; };
    bool view(ActorId, ActorView&) const;

private:
    enum class TimerAction : std::uint8_t { attack_ready, go_to_dive, dive_state, move_to_position, emerge, target_again,
                                            dive_decision, skill2 };
    struct Timer { int id = 0; double remaining_ms = 0, period_ms = 0; bool repeat = false, fresh = false, callback = true; TimerAction action{}; };
    struct Cast { bool active = false, used = false; std::uint64_t generation = 0; int slot = -1;
                  ActorId target = invalid_actor_id; std::vector<ActorId> list; };
    struct Clip { bool active = false; double remaining_ms = 0; bool return_to_idle = false; };
    struct ActorRuntime {
        ScriptState sk = ScriptState::idle;
        int phase = 1;
        bool allowed = false, attack_flag = false, first_melee = true, dive_anim = false, emerge_anim = false, weak = false,
             music_started = false, can_dive_now = false, targetable = true, dead_seen = false, in_anim_state = false, awake = false;
        int intimidate_flag = 0, dive_counter = 0;
        ActorId target = invalid_actor_id;
        std::optional<int> timer_attack, timer_diving, timer_divepattern, emerge_timer, dive_timer, skill2_timer;
        std::vector<Timer> timers;
        int next_timer = 1;
        double spot_ms = 0, range_log_ms = 0;
        Clip clip;
        Cast cast;
        // Lua skill-script locals keep their value between casts (target / g_target).
        std::map<int, ActorId> skill_local_target;
    };
    bool tick_actor(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, double dt, bool ai, std::string&);
    bool run_timers(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, double dt, std::string&);
    bool fire(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, TimerAction, std::string&);
    int start_timer(ActorRuntime&, double ms, bool repeat, TimerAction);
    void stop_timer(ActorRuntime&, std::optional<int>& handle);
    bool on_end_of_anim(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, std::string&);
    bool do_skill(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, int slot, std::string&);
    bool skill_event(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, std::uint64_t generation, const std::string&, std::string&);
    bool roll(CombatSession&, ActorId owner, ActorId target, const BossSkillDefinitionV1&, std::int32_t class_id,
              std::uint64_t generation, std::uint32_t event_index, std::string&);
    bool play_anim(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, bool dive, std::string&);
    bool dive_state(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, std::string&);
    bool idle_state(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, std::string&);
    bool set_position(CombatSession&, ActorId, const std::string& waypoint, std::string&);
    bool search_efficient_target(CombatSession&, ActorId, const SwampkingCoreDefinitionV1&, ActorRuntime&, std::string&);
    bool look_at(CombatSession&, ActorId owner, ActorId target, std::string&);
    bool frontal_search(CombatSession&, ActorId, float range, std::vector<ActorId>& list, std::string&);
    bool update_projectiles(CombatSession&, std::int32_t dt_ms, std::string&);
    void cleanup(ActorRuntime&);
    void log(std::string line);

    SwampkingCoreServicesV1 services_;
    std::map<ActorId, SwampkingCoreDefinitionV1> definitions_;
    std::map<ActorId, ActorRuntime> actors_;
    struct Projectile { enemy_ai::RangedProjectileV1 motion; int slot = -1; std::uint64_t generation = 0; };
    std::vector<Projectile> projectiles_;
    std::weak_ptr<const void> lease_;
    bool lease_known_ = false;
    std::uint64_t next_generation_ = 1, next_projectile_ = 1;
    double fraction_ms_ = 0;
    std::vector<std::string> log_;
    SwampkingCoreReportV1 report_;
    bool targetability_changed_ = false;
};

} // namespace dh::foundation::bosses
