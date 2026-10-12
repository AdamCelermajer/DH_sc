#pragma once
// B154: ranged-skill monsters (the Bogwitch, and every actor whose authored AI script has the same shape).
//
// Original evidence (IDA libDungeonHunter2.so, the authored scripts and pydata tables; coordination/v21.6/B154-opus-report.md):
//  - CharAI::_UpdateTarget 0x3cb908: for a character whose Character::CanRangeAttack 0x3a4d3c is true (cached
//    RangeProjectileID != -1) the AI raises OnTargetInCloseRange (16) when the target is nearer than RangeMinDistance
//    (AI_IsInCloseRange 0x3d63d8), OnTargetInRangedRange (15) inside [RangeMinDistance, RangeMaxDistance]
//    (AI_IsInRange 0x3d6604), else OnTargetOutOfRange (14). The melee event (17) is never raised for such actors.
//  - The authored AI script (data/scripts/ai/<script>.luac, plain Lua text) answers those events. This module reads the
//    facts it needs from that text (parse_ranged_skill_ai_script_v1) instead of keying behaviour by an actor name.
//  - DoSkill(n) -> Character::_DoSkill 0x3b8bd8 -> CharAI::AI_UseSkill/AI_BeginSkill 0x3d86bc: the skill is usable when the
//    actor is not already using a skill/casting and the skill script's check passes (witch_ranged_00: always true, no
//    cooldown); SM_SetSkillState 0x3c6670 enters state 6 with the SkillTable row's Anim sequence.
//  - The skill script (data/scripts/skills/<script>.luac): OnPreSkill searches enemies FrontalFirst within RangeMaxDistance
//    and looks at the top one; at the clip's do_skill event OnSkill looks at it and SpawnSkillProjectile(RangeProjectileID)
//    (CharAISkillScript::SpawnProjectile 0x3da510 -> ProjectileManager::Spawn 0x3e701c -> Projectile::SetInfo 0x3e58cc:
//    origin = owner target position, destination = origin + look * 1000, no target lock). The projectile's hit callback
//    runs the script's delayed callback: SnS_Level = skill level, ApplyPropClass(CLASS_ID), SkillCombatRoll(target)
//    (_SkillCombatRoll 0x3b9fbc -> F_SkillAttack/F_ApplyResult), here CombatSession::apply_source_result.
//  - Skill level: GetCurrentSpellInfo 0x3b6e30 is only reached from OnSkillUpdate, which CharAI::UpdateAllSkills runs for
//    the local player only (Character::InitFinal, PlayerManager::_ManageCharacters); a monster keeps the script's initial 0.
//  - Projectile motion: ProjectileTable Velocity is a Box2D speed (x100 game units per second, the same conversion as
//    runtime_session_projectile_v1), Timer is its lifetime in ms; DisappearOnHit and FriendlyFire filter the contact.
#include "../../asset_catalog.hpp"
#include "../../combat_session.hpp"
#include "../spawn/actor_profile_derivation_v1.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../../game-data/properties.hpp"

#include <array>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::enemy_ai {

// Facts read from the authored AI script text. Every field is required for the ranged-skill behaviour; an AI
// script that does not have this shape (for example monster.luac) is rejected and keeps its existing owner.
struct RangedSkillAiScriptV1 {
    std::int32_t close_range_skill = -1;      // DoSkill(n) in the OnTargetInCloseRange callback
    bool close_range_stops = false;           // Stop() before DoSkill
    bool close_range_looks_at_target = false; // LookAtTarget() before DoSkill
    bool ranged_range_approaches = false;     // OnTargetInRangedRange: MoveTo(GetTarget()) while Idle without a path
    bool out_of_range_clears_target = false;  // OnTargetOutOfRange: Stop(); ClearAggro; ClearTarget()
    bool enemy_spotted_searches_player = false; // OnEnemySpotted: Do_SearchVictims (Player filter) when no target
    bool target_died_clears_target = false;   // OnTargetDied: Stop(); ClearTarget(); stops the teleport timer
    // OnEnemySpotted starts a repeating timer whose callback teleports relative to the actor's look vector.
    struct Teleport {
        bool present = false;
        double delay_ms = 0;
        bool repeat = false;
        std::string fx_name;    // GetPyOID("AnimatedEffectTable", <name>) played at the old and new position
        std::string sound_name; // PlaySound(<name>)
        // Relative MoveTo/WarpTo candidates in script order: {right, forward, up} game units.
        std::vector<std::array<float, 3>> candidates;
    } teleport;
    bool died_stops_timer = false;            // OnDied stops the teleport timer
    std::string spotted_music, died_music;    // PlayMusic(<name>, ...) in OnEnemySpotted / OnDied
    std::int32_t spotted_music_fade_ms = 0, died_music_fade_ms = 0;
};
bool parse_ranged_skill_ai_script_v1(const std::string& text, RangedSkillAiScriptV1& output, std::string& error);

// Facts read from the authored skill script text (data/scripts/skills/<script>.luac).
struct RangedSkillScriptV1 {
    std::string class_token;       // CLASS_ID = GetPyOID("ClassTable", <token>)
    std::string range_field;       // RANGE = FromFixed(GetProp(GetPyStruct("CharacterProperties", <field>)))
    std::string projectile_field;  // PROJECTILE_ID = GetProp(GetPyStruct("CharacterProperties", <field>))
    bool pre_frontal_enemy_search = false; // OnPreSkill: Enemy filter, FrontalFirst, TargetListSearch(RANGE), LookAt
    bool skill_spawns_projectile = false;  // OnSkill: LookAt(target); SpawnSkillProjectile(PROJECTILE_ID)
    bool delayed_combat_roll = false;      // delayed: SetProp(SnS_Level); ApplyPropClass(CLASS_ID); SkillCombatRoll(target)
    bool delayed_check_character = false;  // delayed_check: target:IsCharacter()
};
bool parse_ranged_skill_script_v1(const std::string& text, RangedSkillScriptV1& output, std::string& error);

// One ProjectileTable row (data/pydata/projectiles_pyarray.bin, field order of projectiles_pystructnames.bin).
struct SourceProjectileRowV1 {
    std::string name, model_uri;
    bool dedicated = false, disappear_on_hit = false, friendly_fire = false, laser = false, magic = false,
         stay_on_floor = false, target_lock = false;
    std::int32_t expire_fx = -1, expire_sound = -1, impact_fx = -1, impact_sound = -1, object_impact_fx = -1,
                 object_impact_sound = -1, model = -1, time_between_hit_ms = 0, timer_ms = 0;
    float max_distance = 0, max_rotation = 0, velocity = 0, velocity_damping = 0;
};
bool load_source_projectile_table_v1(dh2::data::Bytes records, dh2::data::Bytes names, dh2::data::Bytes schema,
                                     dh2::data::Bytes dictionary_values, std::vector<SourceProjectileRowV1>& output,
                                     std::string& error);

// Everything the runtime needs for one CharacterTable row, assembled from the tables and both scripts.
struct RangedSkillMonsterDefinitionV1 {
    std::string profile, ai_script, skill_script, skill_name;
    RangedSkillAiScriptV1 ai;
    RangedSkillScriptV1 skill;
    std::int32_t skill_table_id = -1, animation_sequence_id = -1, element = -1;
    std::uint32_t skill_flags = 0;
    float range_min = 0, range_max = 0;   // CanRangeAttack min/max (raw >> 8)
    float pre_search_range = 0;           // the skill script's RANGE field (FromFixed)
    std::int32_t projectile_row = -1;
    SourceProjectileRowV1 projectile;
    std::int32_t class_id = -1;           // ClassTables row of the skill's CLASS_ID
};

// Sources read through AssetCatalog content paths. Missing optional sources make the profile unsupported
// (reason filled), never a startup failure.
struct RangedSkillMonsterSourcesV1 {
    const AssetCatalog* assets = nullptr;
    const spawn::ProfileDerivationTablesV1* tables = nullptr;
    dh2::data::SkillTables::Borrow skills;
    const std::vector<SourceProjectileRowV1>* projectiles = nullptr;
    const dh2::data::ClassTables* classes = nullptr;
};
// supported=false with a reason for every actor that is not a ranged-skill monster (or whose data is missing).
bool derive_ranged_skill_monster_v1(const RangedSkillMonsterSourcesV1&, const std::string& profile,
                                    bool& supported, RangedSkillMonsterDefinitionV1& output, std::string& reason);

// Incoming-only session profile for a ranged-skill monster: targetable shared damage receiver with its own AnimTable
// Injured/Died poses (Idle initial, retained source clock). Its outgoing attack is the skill/projectile owner below,
// not the session's melee damage-marker attack (her Attack clips carry do_skill, a projectile/skill event).
bool derive_ranged_skill_monster_policy_v1(const spawn::ProfileDerivationTablesV1&, const std::string& profile,
                                           CombatSessionProfile& output, std::string& error);

// Pure helpers (unit tested).
enum class RangedRangeEventV1 : std::int32_t { out_of_range = 14, ranged_range = 15, close_range = 16 };
RangedRangeEventV1 ranged_range_event_v1(float distance_squared, float range_min, float range_max);
// GameObject::GetLookAtVec of a port heading (same convention as the skill target query): {sin h, -cos h, 0}.
std::array<float, 3> source_look_vector_v1(float heading);
// Character::_MoveTo/_WarpTo relative branch: position + x*cross(Z,look) + y*look + z*Z.
std::array<float, 3> relative_point_v1(const std::array<float, 3>& position, float heading, const std::array<float, 3>& offset);
// Skill formula sheet: original defaults, SnS_Level = level << 8, then the ClassTables row (mode-1 reads from the
// live resolved sheet with the same SnS_Level), as the player skill coordinator builds it.
bool ranged_skill_formula_sheet_v1(const dh2::data::CharacterTable&, const dh2::data::ClassTables&,
                                   const dh2::data::PropertyRules&, const dh2::data::PropertySheet& live,
                                   std::int32_t class_id, std::int32_t skill_level,
                                   dh2::data::PropertySheet& output, std::string& error);

// Projectile record advanced by advance_ranged_projectile_v1 (game units, ms).
struct RangedProjectileV1 {
    std::uint64_t id = 0;
    ActorId owner = invalid_actor_id;
    std::array<float, 3> origin{}, position{}, forward{};
    float speed = 0;
    std::int64_t remaining_ms = 0, age_ms = 0;
    std::string model_uri; // ProjectileDict model (Projectile::SetInfo SetVisualObject); presentation only
    bool live = true;
};
// One frame: move by speed*100*dt along forward, damp, count the timer. Returns expired.
bool advance_ranged_projectile_v1(RangedProjectileV1&, const SourceProjectileRowV1&, std::int32_t dt_ms);
// XY contact of the projectile point with a character body circle.
bool ranged_projectile_contact_v1(const RangedProjectileV1&, const std::array<float, 3>& body_centre, float body_radius);

struct RangedSkillMonsterServicesV1 {
    std::function<bool(CombatSession&, const ActorState& owner, const ActorState& target, bool& visible, std::string&)> can_see;
    std::function<bool(CombatSession&, ActorState& owner, ActorState& target, std::string&)> approach;
    std::function<bool(CombatSession&, ActorState& owner, std::string&)> stop;
    // Source MoveTo(point) + HasPath(): true when a route to `goal` exists (the route is released again).
    std::function<bool(CombatSession&, ActorState& owner, const std::array<float, 3>& goal, bool& found, std::string&)> path_exists;
    std::function<bool(CombatSession&, ActorState& owner, const std::array<float, 3>& goal, std::string&)> warp;
    std::function<float(ActorId)> body_radius;
    // Presentation (optional): AnimatedEffectTable set at a point, sound/music by sounds-table name.
    std::function<bool(std::int32_t set, const std::array<float, 3>& at, std::string&)> play_fx;
    std::function<std::int32_t(const std::string& name)> fx_id;
    std::function<bool(const std::string& name, std::string&)> play_sound;
    std::function<bool(std::int32_t sound_row, std::string&)> play_sound_id; // ProjectileTable ImpactSFX/ExpireSFX rows
    std::function<bool(const std::string& name, std::int32_t fade_ms, std::string&)> play_music;
    // Source target search population order (player first, then the active population) with visibility facts.
    std::function<bool(std::vector<ActorId>& order, std::vector<std::pair<ActorId, bool>>& visible_interactive, std::string&)> search_population;
    const dh2::data::CharacterTable* characters = nullptr;
    const dh2::data::ClassTables* classes = nullptr;
    const dh2::data::PropertyRules* property_rules = nullptr;
};

struct RangedSkillMonsterReportV1 {
    std::size_t spotted = 0, casts = 0, cast_rejected = 0, projectiles = 0, hits = 0, misses_expired = 0,
                teleports = 0, teleports_without_path = 0, approaches = 0, target_losses = 0, interrupted_casts = 0;
};

// Per-actor decision owner installed on the CombatSession decision provider. CombatSession stays the sole animation
// and damage owner; this owns only the AI target/timer/cast/projectile records of the bound actors.
class RuntimeRangedSkillMonsterV1 final {
public:
    RuntimeRangedSkillMonsterV1(RangedSkillMonsterServicesV1, std::map<ActorId, RangedSkillMonsterDefinitionV1>);
    RuntimeRangedSkillMonsterV1(const RuntimeRangedSkillMonsterV1&) = delete;
    RuntimeRangedSkillMonsterV1& operator=(const RuntimeRangedSkillMonsterV1&) = delete;
    // ai=false (global controller block, CharAI::Update returns early) still advances projectiles in flight.
    bool update(CombatSession&, double dt, bool ai, std::string& error);
    bool owns(ActorId id) const noexcept { return definitions_.count(id) != 0; }
    // The SkillTable row of the actor's current DoSkill occurrence (physical Skill Focus reads its AnimIsMoving byte).
    bool active_cast(ActorId id, std::uint64_t generation, std::int32_t& skill_table_id) const noexcept;
    const RangedSkillMonsterReportV1& report() const noexcept { return report_; }
    std::vector<RangedProjectileV1> projectiles() const;
    // Diagnostic log lines produced since the last call.
    std::vector<std::string> take_log();
private:
    struct Cast {
        bool active = false, used = false;
        std::uint64_t generation = 0;
        ActorId target = invalid_actor_id; // skill script local `target` (kept across casts like the Lua local)
    };
    struct ActorRuntime {
        ActorId target = invalid_actor_id;
        bool spotted = false, timer_active = false, dead_seen = false, approaching = false;
        double timer_ms = 0;
        Cast cast;
        ActorId skill_target = invalid_actor_id;
    };
    bool tick_actor(CombatSession&, ActorId, const RangedSkillMonsterDefinitionV1&, ActorRuntime&, double dt, std::string&);
    bool begin_skill(CombatSession&, ActorId, const RangedSkillMonsterDefinitionV1&, ActorRuntime&, std::string&);
    bool skill_event(CombatSession&, ActorId, const RangedSkillMonsterDefinitionV1&, std::uint64_t generation,
                     const std::string& name, std::string&);
    bool teleport(CombatSession&, ActorId, const RangedSkillMonsterDefinitionV1&, ActorRuntime&, std::string&);
    bool update_projectiles(CombatSession&, std::int32_t dt_ms, std::string&);
    bool look_at(CombatSession&, ActorId owner, ActorId target, std::string&);
    bool pre_search(CombatSession&, ActorId, const RangedSkillMonsterDefinitionV1&, ActorId& selected, std::string&);
    void log(std::string line);

    RangedSkillMonsterServicesV1 services_;
    std::map<ActorId, RangedSkillMonsterDefinitionV1> definitions_;
    std::map<ActorId, ActorRuntime> actors_;
    std::vector<RangedProjectileV1> projectiles_;
    std::map<std::uint64_t, ActorId> projectile_owner_;
    std::weak_ptr<const void> lease_;
    bool lease_known_ = false;
    std::uint64_t next_generation_ = 1, next_projectile_ = 1;
    double fraction_ms_ = 0;
    std::vector<std::string> log_;
    RangedSkillMonsterReportV1 report_;
};

} // namespace dh::foundation::enemy_ai
