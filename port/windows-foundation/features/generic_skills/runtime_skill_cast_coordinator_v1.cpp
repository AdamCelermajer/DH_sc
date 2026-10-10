#include "runtime_skill_cast_coordinator_v1.hpp"
#include "../faery_menu/faery_cast_sound_v1.hpp"
#include "pc_cooldown_frame_v1.hpp"

#include "../../playable_actor_world.hpp"
#include "../../original_combat_properties.hpp"
#include "../../retained_animation_owner.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

// P15 FAERYSOUND (B050): original OnPreSkill_ PlaySound3D labels. Called after the
// UseMana/cooldown prefix is committed, for every cast (empty target list included).
void request_faery_pre_sound(const RuntimeSkillFaeryPreSoundSinkV1& sink, CombatSession& session,
                             ActorId caster, bool hotty, std::size_t target_count) {
    if (!sink) return;
    RuntimeSkillFaeryPreSoundV1 request;
    request.caster = caster;
    request.target_count = target_count;
    if (const auto* actor = session.actor(caster))
        request.position = {actor->transform.position[0], actor->transform.position[1], actor->transform.position[2]};
    request.labels = faery_menu::faery_pre_sound_labels_v1(hotty, target_count);
    sink(request);
}

#if !defined(DH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST)
void append_fx_diagnostic(std::string& detail, const char* branch,
                          const std::string& diagnostic) {
    if (diagnostic.empty()) return;
    if (!detail.empty()) detail += "; ";
    detail += branch;
    detail += " FX diagnostic: ";
    detail += diagnostic;
}

const char* hotty_effect_status_name(faery_menu::HottyEffectStatusV1 status) {
    switch (status) {
    case faery_menu::HottyEffectStatusV1::not_requested: return "not requested";
    case faery_menu::HottyEffectStatusV1::source_branch_skipped: return "source branch skipped";
    case faery_menu::HottyEffectStatusV1::source_instance_created: return "source instance created";
    case faery_menu::HottyEffectStatusV1::dispatch_failed: return "dispatch failed";
    }
    return "unknown status";
}
#endif

bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}

CombatSessionStateAnimationServices source_sequence_services(
    RuntimeSkillCastCoordinatorV1& coordinator, CombatSession& session,
    ActorId actor, std::uint64_t generation, std::int32_t source_state,
    CombatSessionStateAnimationServices existing) {
    const char* use_event = source_state == 7 ? "do_spell" : "do_skill";
    const auto event_state = source_state == 7
        ? RuntimeSkillSourceAnimStateV1::cast
        : RuntimeSkillSourceAnimStateV1::skill;
    CombatSessionStateAnimationServices services;
    services.event = [&coordinator, &session, actor, generation, event_state,
                      use_event, existing](ActorId event_actor,
                          const RetainedAnimationEvent& event,
                          std::string& error) mutable {
        if (event_actor != actor)
            return fail(error, "Skill source animation callback reached a foreign ActorId");
        if (event.name == use_event) {
            auto* active = coordinator.receipt(actor);
            if (!active || active->generation != generation)
                return fail(error, "Source skill Use callback belongs to a stale occurrence");
            if (!coordinator.apply_retained_use_event_v1(session, actor, generation,
                    event_state, event, *active, error))
                return false;
        }
        return !existing.event || existing.event(event_actor, event, error);
    };
    services.finished = [&coordinator, &session, actor, generation, existing](
        ActorId finished_actor, std::string& error) mutable {
        if (finished_actor != actor)
            return fail(error, "Skill source completion reached a foreign ActorId");
        if (existing.finished && !existing.finished(finished_actor, error)) return false;
        auto* active = coordinator.receipt(actor);
        if (!active || active->generation != generation)
            return fail(error, "Skill source completion belongs to a stale occurrence");
        return coordinator.complete_v1(session, actor, generation, *active, error);
    };
    services.departed = [&coordinator, &session, actor, generation, existing](
        ActorId departed_actor, std::int32_t from_state, std::int32_t to_state,
        std::string& error) mutable {
        if (departed_actor != actor)
            return fail(error, "Skill source departure reached a foreign ActorId");
        if (!coordinator.source_departed_v1(session, actor, generation,
                from_state, to_state, error)) return false;
        return !existing.departed || existing.departed(departed_actor,
                from_state, to_state, error);
    };
    services.checkpoint = [&coordinator, &session, existing](std::string& error) mutable {
        if (!coordinator.checkpoint_v1(session, error)) return false;
        return !existing.checkpoint || existing.checkpoint(error);
    };
    return services;
}

bool source_supported_skill(const dh2::data::SkillTables::Borrow& tables,
                            int table_id, RuntimeWarriorSkillBranchV1& branch,
                            std::string& token) {
    if (!tables || table_id < 0 || std::size_t(table_id) >= tables.skills().size() ||
        std::size_t(table_id) >= tables.skill_names().size()) return false;
    const auto& skill = tables.skills()[std::size_t(table_id)];
    const auto& name = tables.skill_names()[std::size_t(table_id)];
    // Exact SkillTable.Script and Lua CLASS_ID tokens, recovered from the
    // matching cached source files; no user-facing/class-name fallback.
    if (name == "BashDown" && skill.script == "prince_warrior_bashdown") {
        branch = RuntimeWarriorSkillBranchV1::bashdown;
        token = "Skill_Warrior_BashDown";
        return true;
    }
    if (name == "GroundSlam" && skill.script == "prince_warrior_ground_slam") {
        branch = RuntimeWarriorSkillBranchV1::ground_slam;
        token = "Skill_Warrior_GroundSlam";
        return true;
    }
    if (name == "Charge" && skill.script == "prince_warrior_charge") {
        branch = RuntimeWarriorSkillBranchV1::charge;
        token = "Skill_Warrior_Charge";
        return true;
    }
    if (name == "JumpKick" && skill.script == "prince_rogue_jump_kick") {
        branch = RuntimeWarriorSkillBranchV1::rogue_jump_kick;
        token = "Skill_Rogue_JumpKick";
        return true;
    }
    return false;
}

#if !defined(DH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST)
bool resolve_current_faery_script(
    const CharacterState& character, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow& tables,
    RuntimeSkillFaeryScriptV1& script, std::int32_t& slot,
    std::int32_t& row_id, std::string& error) {
    if (!character.source_faery_state_known || !tables || difficulty < 0 || difficulty >= 3 ||
        character.source_faery_list_id < 0 ||
        static_cast<std::size_t>(character.source_faery_list_id) >= tables.lists().size() ||
        static_cast<std::size_t>(character.source_faery_list_id) >= tables.list_names().size())
        return fail(error, "NativeHUDSpell requires the same source-known current FaeryList and difficulty");
    const auto& save = character.faery_by_difficulty[static_cast<std::size_t>(difficulty)];
    slot = save.current_faery;
    const auto& list = tables.lists()[static_cast<std::size_t>(character.source_faery_list_id)];
    if (list.size() != 5 || slot < 0 || slot >= 5)
        return fail(error, "NativeHUDSpell current Faery slot/list is outside the original five slots");
    row_id = list[static_cast<std::size_t>(slot)];
    if (row_id < 0 || static_cast<std::size_t>(row_id) >= tables.faeries().size() ||
        static_cast<std::size_t>(row_id) >= tables.faery_names().size())
        return fail(error, "NativeHUDSpell current FaeryList references an invalid source row");
    const auto& row = tables.faeries()[static_cast<std::size_t>(row_id)];
    if (row.scalar.words[8] != static_cast<std::uint32_t>(slot))
        return fail(error, "NativeHUDSpell source Faery row type does not match the current Save slot");
    if (row.script == "faerie_hotty") {
        if (tables.faery_names()[static_cast<std::size_t>(row_id)] != "Hotty")
            return fail(error, "Hotty gameplay script row differs from the source class FaeryList");
        script = RuntimeSkillFaeryScriptV1::hotty;
    } else if (row.script == "faerie_celest") {
        if (tables.faery_names()[static_cast<std::size_t>(row_id)] != "Celest")
            return fail(error, "Celest script row identity differs from the recovered Faery table");
        script = RuntimeSkillFaeryScriptV1::celest;
    } else {
        error = "Current source Faery script is unsupported; no Hotty/Celest substitution is permitted";
        return false;
    }
    error.clear();
    return true;
}
#endif

std::vector<ActorId> active_population_order(const ActorPopulation& population,
                                             const CombatSession& session,
                                             std::string& error) {
    const auto* world = session.world();
    if (!world) {
        error = "Active population has no registered same-Session Character world";
        return {};
    }
    std::vector<ActorId> ids;
    ids.reserve(population.actors().size());
    for (const auto& actor : population.actors()) {
        const auto stable = actor.definition.stableId;
        if (stable == 0 || stable > static_cast<std::uint64_t>(invalid_actor_id - 1)) {
            error = "Active ActorPopulation contains an invalid stable actor identity";
            return {};
        }
        const auto id = static_cast<ActorId>(stable);
        if (std::find(ids.begin(), ids.end(), id) != ids.end()) {
            error = "Active ActorPopulation contains a duplicate stable actor identity";
            return {};
        }
        // The population also owns render-only visuals. Source Character
        // searches enumerate registered Characters, so pass only the exact
        // ordered subset represented in this Session's combat world. Query
        // predicates still decide alive/faction/visibility/interaction.
        if (actor.enabled && world->find_actor(id)) ids.push_back(id);
    }
    error.clear();
    return ids;
}

bool source_field(const dh2::data::CharacterTable& table, const char* name,
                  std::size_t& index, std::string& error) {
    const auto found = std::find(table.fields.begin(), table.fields.end(), name);
    if (found == table.fields.end()) {
        error = std::string("Original CharacterProperties field is unavailable: ") + name;
        return false;
    }
    index = static_cast<std::size_t>(found - table.fields.begin());
    if (index >= dh2::data::PropertySheet{}.size()) {
        error = std::string("Original CharacterProperties field exceeds source sheet: ") + name;
        return false;
    }
    error.clear();
    return true;
}

bool source_skill_class_projection(
    const dh2::data::CharacterTable& characters,
    const dh2::data::ClassTables& classes,
    const dh2::data::PropertyRules& rules,
    const dh2::data::PropertySheet& live,
    const std::string& class_token, std::uint32_t rank,
    dh2::data::PropertySheet& output, double& cooldown_ms,
    double& source_range, double& source_range_growth,
    std::string& error) {
    std::size_t skill_level = 0, cooldown = 0, range = 0, range_growth = 0;
    if (!source_field(characters, "SnS_Level", skill_level, error) ||
        !source_field(characters, "SnS_Cooldown", cooldown, error) ||
        !source_field(characters, "TempProp1", range, error) ||
        !source_field(characters, "TempProp2", range_growth, error)) return false;
    if (rules.types[skill_level] != 8 || rules.types[cooldown] != 8 ||
        rules.types[range] != 8 || rules.types[range_growth] != 8 ||
        rules.defaults[skill_level] != 0)
        return fail(error, "Original skill source property fields do not retain their type-8 schema");
    if (rank > std::numeric_limits<std::uint16_t>::max())
        return fail(error, "Saved source skill rank exceeds the original u16 skill field");
    const auto found = std::find(classes.names.begin(), classes.names.end(), class_token);
    if (found == classes.names.end() || classes.rows.size() != classes.names.size())
        return fail(error, "Authored skill source CLASS_ID is missing from the actual ClassTables");
    output = rules.defaults;
    output[skill_level] = static_cast<std::int32_t>(std::uint32_t(rank) << 8);
    if (!dh2::data::apply_class(classes, static_cast<std::int32_t>(found - classes.names.begin()),
                               output, error, &live)) {
        if (error.empty()) error = "Original skill CalcManaCost class projection failed";
        return false;
    }
    cooldown_ms = original_signed256(output[cooldown]);
    if (!std::isfinite(cooldown_ms))
        return fail(error, "Original skill SnS_Cooldown is not finite");
    if (cooldown_ms < 0.0) cooldown_ms = 0.0; // Exact source Lua clamp.
    source_range = original_signed256(output[range]);
    source_range_growth = original_signed256(output[range_growth]);
    if (!std::isfinite(source_range) || !std::isfinite(source_range_growth))
        return fail(error, "Original skill source range properties are not finite");
    error.clear();
    return true;
}
} // namespace

bool RuntimeSkillCastCoordinatorV1::advance_after_session_update(
    CombatSession& session, double dt, std::string& error) {
    error.clear();
    const auto lease = session.actor_binding_lease();
    if (!session.world() || lease.expired() || !std::isfinite(dt) || dt < 0.0)
        return fail(error, "Skill timer clock requires a live Session and finite nonnegative update dt");
    if (timer_clock_bound_) {
        if (timer_binding_lease_.expired() || !same_owner(timer_binding_lease_, lease)) {
            skill_ready_at_ms_.clear();
            skill_cooldown_total_ms_.clear();
            timer_clock_bound_ = false;
            return fail(error, "Skill cooldown timer state belongs to a replaced CombatSession binding");
        }
        if (session.update_serial() != timer_update_serial_ + 1)
            return fail(error, "Skill cooldown clock must observe each successful Session update exactly once");
    }
    const double delta_ms = dt * 1000.0;
    if (!std::isfinite(delta_ms) || delta_ms > 86400000.0)
        return fail(error, "Skill cooldown clock dt exceeds source timer bounds");
    elapsed_ms_ += delta_ms;
    if (!std::isfinite(elapsed_ms_)) return fail(error, "Skill cooldown clock overflowed");
    timer_update_serial_ = session.update_serial();
    timer_binding_lease_ = lease;
    timer_clock_bound_ = true;
    for (auto it = skill_ready_at_ms_.begin(); it != skill_ready_at_ms_.end();) {
        if (elapsed_ms_ >= it->second) {
            skill_cooldown_total_ms_.erase(it->first);
            it = skill_ready_at_ms_.erase(it);
        } else ++it;
    }
    error.clear();
    return true;
}

bool RuntimeSkillCastCoordinatorV1::source_departed_v1(
    CombatSession& session, ActorId actor_id, std::uint64_t generation,
    std::int32_t from_state, std::int32_t to_state, std::string& error) {
    error.clear();
    auto found = active_.find(actor_id);
    if (found == active_.end() || found->second.receipt.generation != generation)
        return fail(error, "Accepted source departure belongs to a stale cast generation");
    auto& active = found->second;
    if (active.binding_lease.expired() ||
        !same_owner(active.binding_lease, session.actor_binding_lease()))
        return fail(error, "Accepted source departure belongs to a replaced CombatSession owner");
    if ((from_state != 6 && from_state != 7) || from_state != active.source_state ||
        to_state < 0 || to_state > 18)
        return fail(error, "Accepted source departure does not match the retained Skill6/Cast7 owner");
    if (active.source_post_delivered) return true;
    if (active.receipt.phase != RuntimeSkillCastPhaseV1::prepared_pending_use &&
        active.receipt.phase != RuntimeSkillCastPhaseV1::use_applied &&
        active.receipt.phase != RuntimeSkillCastPhaseV1::partial_failure)
        return fail(error, "Accepted source departure has no in-flight cast/Post occurrence");

    // Retire before running Post so reentrant or late markers cannot deliver Use.
    active.source_post_delivered = true;
    if (active.source_post_clears_target) {
        auto* actor = session.actor(actor_id);
        if (!actor) return fail(error, "Authored skill Post requires its same-Session actor target owner");
        actor->target_id = invalid_actor_id;
    }
    active.receipt.phase = RuntimeSkillCastPhaseV1::interrupted;
    active.receipt.detail = active.use_delivered
        ? "Accepted source state departure ran Post after the reached Use prefix"
        : "Accepted source state departure ran Post before Use; mana/cooldown prefix is retained";
    error.clear();
    return true;
}

bool RuntimeSkillCastCoordinatorV1::checkpoint_v1(
    CombatSession& session, std::string& error) const {
    error.clear();
    const auto lease = session.actor_binding_lease();
    if (!session.world() || lease.expired())
        return fail(error, "Skill checkpoint requires the live same-Session world and owner lease");
    if (timer_clock_bound_) {
        if (timer_binding_lease_.expired() || !same_owner(timer_binding_lease_, lease))
            return fail(error, "Skill cooldown clock belongs to a replaced Session and cannot be checkpointed");
        if (timer_update_serial_ != session.update_serial())
            return fail(error, "Skill checkpoint requires the cooldown clock at the current Session update serial");
    }
    for (const auto& timer : skill_ready_at_ms_) {
        if (elapsed_ms_ < timer.second)
            return fail(error, "Skill checkpoint is blocked by an unpersisted active source skill cooldown");
    }
    for (const auto& clock_entry : faery_cooldown_clocks_) {
        const auto* clock = clock_entry.second;
        if (!clock || !clock->has_binding_lease || clock->binding_lease.expired() ||
            !same_owner(clock->binding_lease, lease))
            return fail(error, "Faery checkpoint has no same-Session retained cooldown timer owner");
        if (clock->session_update_serial != session.update_serial())
            return fail(error, "Faery checkpoint requires its cooldown clock at the current Session update serial");
        for (const auto& timer : clock->spell_ready_at_ms) {
            if (clock->elapsed_ms < timer.second)
                return fail(error, "Faery checkpoint is blocked by an unpersisted active source spell cooldown");
        }
    }
    for (const auto& pair : active_) {
        const auto& active = pair.second;
        const auto phase = active.receipt.phase;
        if (phase == RuntimeSkillCastPhaseV1::prepared_pending_use ||
            phase == RuntimeSkillCastPhaseV1::use_applied ||
            (phase == RuntimeSkillCastPhaseV1::partial_failure && active.receipt.mana.mana_spent))
            return fail(error, "Skill checkpoint is blocked by a live or partial source cast occurrence");
        if ((phase == RuntimeSkillCastPhaseV1::completed ||
             phase == RuntimeSkillCastPhaseV1::interrupted) &&
            !active.source_post_delivered)
            return fail(error, "Skill checkpoint is blocked until the source Post owner has completed");
    }
    error.clear();
    return true;
}

bool RuntimeSkillCastCoordinatorV1::begin_skill_cast_v1(
    const RuntimeSkillCastRequestV1& request, CombatSession& session,
    RuntimeSkillCastReceiptV1& output, std::string& error) {
    output = {};
    error.clear();
    if (!request.character || !request.classes || !request.property_rules)
        return fail(error, "Skill cast requires the same source Character and class/property tables");
    if (!session.world() || session.actor_binding_lease().expired() ||
        !session.retained_actor_pose(request.actor))
        return fail(error, "Skill cast requires the existing live CombatSession actor and retained pose owner");
    auto* actor = session.actor(request.actor);
    if (!actor || actor != session.world()->find_actor(request.actor) || !actor->alive())
        return fail(error, "Skill cast owner is absent or dead in the same CombatSession");
    // Source CharAI::AI_IsSkillUsable (0x3d8358): reject only when a skill is
    // in use (unless Character+1312 & 0x8000) or casting. Attack is NOT a
    // rejection: CSAttack::OnInit registers 50005 -> state 6 unguarded, so a
    // skill interrupts the swing (its OnBlur runs in play_actor_source_sequence).
    // Hurt stays rejected until its source Injured admission is verified.
    if (actor->action == CharacterAction::casting)
        return fail(error, "Source skill is already casting; a new skill cast is rejected");
    if (actor->action == CharacterAction::hurt)
        return fail(error, "Hurt action rejects a new skill cast (source Injured admission unverified)");
    if (active_.count(request.actor) &&
        active_.at(request.actor).receipt.phase != RuntimeSkillCastPhaseV1::completed &&
        active_.at(request.actor).receipt.phase != RuntimeSkillCastPhaseV1::interrupted &&
        active_.at(request.actor).receipt.phase != RuntimeSkillCastPhaseV1::rejected &&
        active_.at(request.actor).receipt.phase != RuntimeSkillCastPhaseV1::partial_failure)
        return fail(error, "Same actor already has an unfinished source skill occurrence");

    if (request.dispatch == RuntimeSkillCastDispatchV1::native_hud_spell) {
#if defined(DH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST)
        return fail(error, "Hotty branch is excluded from the Warrior-only link fixture");
#else
        auto* arm = request.active_faery_spell;
        if (!arm || !arm->tables || !*arm->tables || !arm->policy || !arm->actor_order ||
            !arm->source_facts || !arm->targets || !arm->cooldown_clock)
            return fail(error, "NativeHUDSpell requires complete same-source Faery inputs");
        if (request.actor != session.player_id() || !arm->targets->query_complete ||
            arm->targets->scope != faery_menu::HottyTargetScopeV1::current_character_population)
            return fail(error, "NativeHUDSpell requires its current player and complete current Character target list");
        const auto prior_clock = faery_cooldown_clocks_.find(request.actor);
        if (prior_clock != faery_cooldown_clocks_.end() &&
            prior_clock->second != arm->cooldown_clock && prior_clock->second) {
            const auto& old_clock = *prior_clock->second;
            for (const auto& timer : old_clock.spell_ready_at_ms) {
                if (old_clock.elapsed_ms < timer.second)
                    return fail(error, "NativeHUDSpell cannot replace a same-actor clock with an unpersisted active cooldown");
            }
            if (old_clock.has_binding_lease &&
                !old_clock.binding_lease.expired() &&
                same_owner(old_clock.binding_lease, session.actor_binding_lease()))
                return fail(error, "NativeHUDSpell requires one retained cooldown clock per same-Session actor");
        }
        faery_cooldown_clocks_[request.actor] = arm->cooldown_clock;
        RuntimeSkillFaeryScriptV1 faery_script{};
        std::int32_t selected_faery_slot = -1, selected_faery_row = -1;
        if (!resolve_current_faery_script(*request.character, arm->difficulty,
                *arm->tables, faery_script, selected_faery_slot,
                selected_faery_row, error))
            return false;
        if (faery_script == RuntimeSkillFaeryScriptV1::celest) {
            ActiveCastV1 active;
            active.receipt.actor = request.actor;
            active.receipt.generation = next_generation_++;
            if (active.receipt.generation == 0) active.receipt.generation = next_generation_++;
            active.receipt.session_update_serial = session.update_serial();
            active.receipt.native_hud_spell = true;
            active.receipt.skill_name = "Celest";
            active.receipt.source_script = "faerie_celest";
            active.receipt.faery_slot = selected_faery_slot;
            active.receipt.faery_record_id = selected_faery_row;
            active.receipt.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
            active.source_state = 7;
            active.source_focus_flags = 0x6301u;
            active.receipt.detail = "Celest OnPre completed; waiting for retained state7 do_spell Use";
            active.binding_lease = session.actor_binding_lease();
            active.character = request.character;
            active.characters = request.characters;
            active.classes = request.classes;
            active.property_rules = request.property_rules;
            active.previous_action = actor->action;
            active.faery_script = faery_script;
            active.faery_tables = *arm->tables;
            active.faery_difficulty = arm->difficulty;
            active.hotty_policy = *arm->policy;
            active.hotty_actor_order = *arm->actor_order;
            active.hotty_source_facts = *arm->source_facts;
            active.hotty_targets = *arm->targets;
            active.hotty_cooldown_clock = arm->cooldown_clock;
            active.celest_effect_dispatch = arm->celest_effect_dispatch;
            if (!faery_menu::prepare_celest_spell_v1(session, *request.character,
                    active.faery_difficulty, active.faery_tables, *request.classes,
                    *request.property_rules, active.hotty_policy,
                    *active.hotty_cooldown_clock, active.celest_prepared, error,
                    &active.hotty_targets)) {
                if (active.celest_prepared.mana_debited) {
                    active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                    active.receipt.faery_slot = active.celest_prepared.faery_slot;
                    active.receipt.faery_record_id = active.celest_prepared.faery_record_id;
                    active.receipt.spell_type = active.celest_prepared.source_spell_type;
                    active.receipt.target_order = active.celest_prepared.character_targets;
                    active.receipt.detail = "Celest source UseMana committed before a later OnPre provider failure";
                    output = active.receipt;
                    active_[request.actor] = std::move(active);
                }
                return false;
            }
            active.receipt.faery_slot = active.celest_prepared.faery_slot;
            active.receipt.faery_record_id = active.celest_prepared.faery_record_id;
            active.receipt.spell_type = active.celest_prepared.source_spell_type;
            active.receipt.target_order = active.celest_prepared.character_targets;
            if (active.celest_prepared.status == faery_menu::CelestPrepareStatusV1::rejected) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::rejected;
                active.receipt.detail = "Celest authored Check/OnPre rejected the same-source request";
                output = active.receipt;
                active_[request.actor] = std::move(active);
                error.clear();
                return true;
            }
            if (!active.celest_prepared.mana_debited) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Celest OnPre returned pending combat without committed mana/cooldown";
                output = active.receipt;
                active_[request.actor] = std::move(active);
                return fail(error, "Celest OnPre did not reach its authored UseMana/cooldown prefix");
            }
            // P15 FAERYSOUND: Celest OnPreSkill_ tier label (+ StaticBallKilled when empty), after mana commit.
            request_faery_pre_sound(faery_pre_sound_sink_, session, request.actor, false,
                                    active.celest_prepared.character_targets.size());
            faery_menu::CelestEffectReceiptV1 pre_fx;
            if (!faery_menu::dispatch_celest_player_pre_best_effort_v1(
                    session, active.celest_prepared, *active.character,
                    *active.hotty_cooldown_clock, active.celest_effect_dispatch,
                    pre_fx, error)) {
                append_fx_diagnostic(active.receipt.detail, "Celest Player_Pre",
                    error.empty() ? "best-effort dispatch failed" : error);
            } else if (pre_fx.status == faery_menu::CelestEffectStatusV1::dispatch_failed ||
                       pre_fx.status == faery_menu::CelestEffectStatusV1::not_requested) {
                append_fx_diagnostic(active.receipt.detail, "Celest Player_Pre",
                    pre_fx.diagnostic.empty() ? "dispatcher unavailable" : pre_fx.diagnostic);
            }
            error.clear();
            actor->action = CharacterAction::casting;
            output = active.receipt;
            active_[request.actor] = std::move(active);
            output = active_.at(request.actor).receipt;
            const auto generation = active_.at(request.actor).receipt.generation;
            if (!request.visual_plan || !request.sequence_policies) {
                active_.at(request.actor).receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active_.at(request.actor).receipt.detail = "Celest OnPre committed but its exact current-slot source Cast bank is unavailable";
                output = active_.at(request.actor).receipt;
                return fail(error, "Celest Cast requires the actual current-slot CharAnimTable sequence bank");
            }
            auto services = source_sequence_services(*this, session, request.actor,
                generation, 7, request.downstream_animation_services);
            const CombatSessionSourceSequencePolicy source_policy{7, 0x6301u, generation};
            if (!session.play_actor_source_sequence(request.actor, *request.visual_plan,
                    *request.sequence_policies, request.selection, std::move(services),
                    source_policy, error)) {
                active_.at(request.actor).receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active_.at(request.actor).receipt.detail = "Celest OnPre committed but the retained state7 Cast sequence could not start";
                output = active_.at(request.actor).receipt;
                return false;
            }
            output = active_.at(request.actor).receipt;
            return true;
        }
        ActiveCastV1 active;
        active.receipt.actor = request.actor;
        active.receipt.generation = next_generation_++;
        if (active.receipt.generation == 0) active.receipt.generation = next_generation_++;
        active.receipt.session_update_serial = session.update_serial();
        active.receipt.native_hud_spell = true;
        active.receipt.skill_name = "faerie_hotty";
        active.receipt.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
        active.source_state = 7;
        active.source_focus_flags = 0x6301u;
        active.receipt.detail = "Hotty OnPre completed; waiting for its distinct source Cast-state Use owner";
        active.binding_lease = session.actor_binding_lease();
        active.character = request.character;
        active.characters = request.characters;
        active.classes = request.classes;
        active.property_rules = request.property_rules;
        active.previous_action = actor->action;
        active.is_hotty = true;
        active.faery_script = faery_script;
        active.faery_tables = *arm->tables;
        active.faery_difficulty = arm->difficulty;
        active.hotty_policy = *arm->policy;
        active.hotty_actor_order = *arm->actor_order;
        active.hotty_source_facts = *arm->source_facts;
        active.hotty_targets = *arm->targets;
        active.hotty_cooldown_clock = arm->cooldown_clock;
        active.hotty_effect_dispatch = arm->effect_dispatch;
        active.hotty_effects_factory = arm->effects_factory;
        active.hotty_effects_tables = arm->effects_tables;
        active.hotty_effect_ids = arm->effect_ids;
        if (!faery_menu::prepare_hotty_spell_v1(session, *request.character,
                active.faery_difficulty, active.faery_tables, *request.classes,
                *request.property_rules, active.hotty_policy, *active.hotty_cooldown_clock,
                active.hotty_prepared, error, &active.hotty_targets)) {
            if (active.hotty_prepared.mana_debited) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.faery_slot = active.hotty_prepared.faery_slot;
                active.receipt.faery_record_id = active.hotty_prepared.faery_record_id;
                active.receipt.spell_type = active.hotty_prepared.source_spell_type;
                active.receipt.detail = "Hotty source UseMana committed before a later OnPre provider failure";
                output = active.receipt;
                active_[request.actor] = std::move(active);
            }
            return false;
        }
        active.receipt.faery_slot = active.hotty_prepared.faery_slot;
        active.receipt.faery_record_id = active.hotty_prepared.faery_record_id;
        active.receipt.spell_type = active.hotty_prepared.source_spell_type;
        active.receipt.target_order = active.hotty_prepared.character_targets;
        if (active.hotty_prepared.status == faery_menu::HottyPrepareStatusV1::rejected) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::rejected;
            active.receipt.detail = "Hotty authored Check/OnPre rejected the same-source request";
            output = active.receipt;
            active_[request.actor] = std::move(active);
            error.clear();
            return true;
        }
        if (!active.hotty_prepared.mana_debited) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Hotty OnPre returned pending combat without a source mana/cooldown prefix";
            output = active.receipt;
            active_[request.actor] = std::move(active);
            return fail(error, "Hotty OnPre did not reach its authored UseMana/cooldown prefix");
        }
        // P15 FAERYSOUND: Hotty OnPreSkill_ sound (tier label, or mage staff fire when empty).
        request_faery_pre_sound(faery_pre_sound_sink_, session, request.actor, true,
                                active.hotty_prepared.character_targets.size());
        if (active.hotty_effect_dispatch) {
            std::string fx_error;
            if (!active.hotty_effect_dispatch->dispatch_player_pre(
                    session, active.hotty_prepared, *active.character,
                    *active.hotty_cooldown_clock, active.hotty_player_pre_status,
                    fx_error)) {
                append_fx_diagnostic(active.receipt.detail, "Player_Pre",
                    fx_error.empty() ? hotty_effect_status_name(active.hotty_player_pre_status) : fx_error);
            } else if (active.hotty_player_pre_status ==
                       faery_menu::HottyEffectStatusV1::dispatch_failed) {
                append_fx_diagnostic(active.receipt.detail, "Player_Pre",
                    hotty_effect_status_name(active.hotty_player_pre_status));
            }
        } else {
            active.hotty_player_pre_status = faery_menu::HottyEffectStatusV1::not_requested;
            append_fx_diagnostic(active.receipt.detail, "Player_Pre", "dispatcher unavailable");
        }
        actor->action = CharacterAction::casting;
        output = active.receipt;
        active_[request.actor] = std::move(active);
        const auto generation = active_.at(request.actor).receipt.generation;
        if (!request.visual_plan || !request.sequence_policies) {
            active_.at(request.actor).receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active_.at(request.actor).receipt.detail = "Hotty OnPre committed but its exact current-slot source Cast bank is unavailable";
            output = active_.at(request.actor).receipt;
            return fail(error, "Hotty Cast requires the actual current-slot CharAnimTable sequence bank");
        }
        auto services = source_sequence_services(*this, session, request.actor,
            generation, 7, request.downstream_animation_services);
        const CombatSessionSourceSequencePolicy source_policy{7, 0x6301u, generation};
        if (!session.play_actor_source_sequence(request.actor, *request.visual_plan,
                *request.sequence_policies, request.selection, std::move(services),
                source_policy, error)) {
            active_.at(request.actor).receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active_.at(request.actor).receipt.detail = "Hotty OnPre committed but the retained state7 Cast sequence could not start";
            output = active_.at(request.actor).receipt;
            return false;
        }
        output = active_.at(request.actor).receipt;
        error.clear();
        return true;
#endif
    }

    if (!request.characters || !request.skills || !request.population ||
        !request.visual_plan || !request.sequence_policies)
        return fail(error, "NativeHUDSkill requires the original tables, active population and authored animation bank");

    int position = -1;
    std::uint32_t saved_row = 0;
    if (!resolve_assigned_skill_position_v1(*request.character, request.skills,
            request.equipment_set, request.source_slot, position, saved_row, error)) return false;
    SkillVisualRequestV1 visual;
    if (!resolve_skill_visual_request_v1(*request.character, *request.characters,
            request.skills, position, visual, error)) return false;
    if (visual.saved_skill_row != static_cast<int>(saved_row) ||
        visual.skill_table_id < 0 || visual.skill_table_id >= static_cast<int>(request.skills.skills().size()))
        return fail(error, "Saved assignment no longer resolves to the same source SkillTable row");

    auto source_selection = request.selection;
    if (request.animation_bank) {
        RuntimeSkillAnimationSlotV1 resolved_slot;
        if (!resolve_runtime_skill_animation_slot_v1(*request.character,
                *request.characters, request.skills, request.equipment_set,
                request.source_slot, *request.animation_bank, resolved_slot, error)) return false;
        if (resolved_slot.skill.saved_skill_row != static_cast<int>(saved_row) ||
            resolved_slot.skill.class_skill_position != position ||
            resolved_slot.skill.skill_table_id != visual.skill_table_id)
            return fail(error, "Fresh animation-bank slot resolution disagrees with current saved skill identity");
        source_selection.state = resolved_slot.selection_state;
    }

    std::string class_token;
    RuntimeWarriorSkillBranchV1 branch = RuntimeWarriorSkillBranchV1::bashdown;
    if (!source_supported_skill(request.skills, visual.skill_table_id, branch, class_token)) {
        output.actor = request.actor;
        output.source_slot = request.source_slot;
        output.saved_skill_row = saved_row;
        output.class_skill_position = position;
        output.skill_table_id = visual.skill_table_id;
        output.skill_name = visual.source_skill_name;
        output.source_script = visual.source_script;
        output.phase = RuntimeSkillCastPhaseV1::rejected;
        output.detail = "No complete generic same-session Use arm is implemented for this authored source skill script";
        error.clear();
        return true;
    }
    const auto& skill_row = request.skills.skills()[std::size_t(visual.skill_table_id)];
    if (skill_row.scalar.words[7] & 0x00800000u)
        return fail(error, "Paired-hand skill result requests are not supported by this source owner");

    // Build the target order from the live, active population in enrollment
    // order. Reached object-only results remain unsupported; a Character hit
    // is never substituted from cursor distance or ActorId sorting.
    auto order = active_population_order(*request.population, session, error);
    if (!error.empty()) return false;
    const auto* current = session.world()->combat_properties(request.actor);
    if (!current) return fail(error, "Warrior skill requires the same live source property owner");
    dh2::data::PropertySheet class_projection{};
    double cooldown_ms = 0.0, projected_range = 0.0, projected_range_growth = 0.0;
    if (!source_skill_class_projection(*request.characters, *request.classes,
            *request.property_rules, current->sheets.resolved, class_token,
            visual.saved_rank, class_projection, cooldown_ms, projected_range,
            projected_range_growth, error)) return false;
    if (cooldown_ms > 0.0) {
        if (!timer_clock_bound_) {
            timer_clock_bound_ = true;
            timer_binding_lease_ = session.actor_binding_lease();
            timer_update_serial_ = session.update_serial();
            elapsed_ms_ = 0.0;
        }
        if (timer_binding_lease_.expired() ||
            !same_owner(timer_binding_lease_, session.actor_binding_lease()) ||
            timer_update_serial_ != session.update_serial())
            return fail(error, "Source skill cooldown clock has not observed the current CombatSession update");
        const auto timer = skill_ready_at_ms_.find({request.actor, visual.skill_table_id});
        if (timer != skill_ready_at_ms_.end() && elapsed_ms_ < timer->second) {
            output.actor = request.actor;
            output.source_slot = request.source_slot;
            output.saved_skill_row = saved_row;
            output.class_skill_position = position;
            output.skill_table_id = visual.skill_table_id;
            output.skill_name = visual.source_skill_name;
            output.source_script = visual.source_script;
            output.phase = RuntimeSkillCastPhaseV1::rejected;
            output.detail = "Authored HasSkillCooldown returned its active source timer";
            error.clear();
            return true;
        }
    }

    SkillActorTargetQueryV1 target;
    target.caster = request.actor;
    target.status = SkillActorTargetQueryStatusV1::no_actor_target;
    float pre_range = 0.0f;
    bool pre_search = true;
    if (branch == RuntimeWarriorSkillBranchV1::bashdown) pre_range = 160.0f;
    else if (branch == RuntimeWarriorSkillBranchV1::charge) pre_range = 300.0f;
    else if (branch == RuntimeWarriorSkillBranchV1::rogue_jump_kick) pre_range = 250.0f;
    else pre_search = false; // GroundSlam's authored OnPre has no target query.
    if (pre_search && !query_source_character_targets_v1(session, request.actor,
            order, request.target_facts, request.non_character_attackable_objects_absent,
            pre_range, 3.1415927410125732421875f,
            SkillTargetSortV1::frontal_first, target, error)) return false;
    if ((branch == RuntimeWarriorSkillBranchV1::bashdown ||
         branch == RuntimeWarriorSkillBranchV1::charge ||
         branch == RuntimeWarriorSkillBranchV1::rogue_jump_kick) &&
        target.status == SkillActorTargetQueryStatusV1::non_character_target_unknown) {
        output.actor = request.actor;
        output.source_slot = request.source_slot;
        output.saved_skill_row = saved_row;
        output.class_skill_position = position;
        output.skill_table_id = visual.skill_table_id;
        output.skill_name = visual.source_skill_name;
        output.source_script = visual.source_script;
        output.phase = RuntimeSkillCastPhaseV1::rejected;
        output.detail = "Character-only host scope cannot resolve an AttackableOnly source object candidate";
        error.clear();
        return true;
    }

    const auto old_heading = actor->transform.rotation[2];
    if ((branch == RuntimeWarriorSkillBranchV1::bashdown ||
         branch == RuntimeWarriorSkillBranchV1::charge ||
         branch == RuntimeWarriorSkillBranchV1::rogue_jump_kick) &&
        target.status == SkillActorTargetQueryStatusV1::selected_character &&
        !look_at_bashdown_target_v1(session, target, error)) return false;

    ActiveCastV1 active;
    active.receipt.actor = request.actor;
    active.receipt.generation = next_generation_++;
    if (active.receipt.generation == 0) active.receipt.generation = next_generation_++;
    active.receipt.session_update_serial = session.update_serial();
    active.receipt.equipment_set = request.equipment_set;
    active.receipt.source_slot = request.source_slot;
    active.receipt.saved_skill_row = saved_row;
    active.receipt.class_skill_position = position;
    active.receipt.skill_table_id = visual.skill_table_id;
    active.receipt.skill_name = visual.source_skill_name;
    active.receipt.source_script = visual.source_script;
    active.receipt.target_order = target.ordered_character_targets;
    active.receipt.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
    active.source_state = 6;
    active.source_focus_flags = 0x6341u;
    active.binding_lease = session.actor_binding_lease();
    active.character = request.character;
    active.characters = request.characters;
    active.classes = request.classes;
    active.property_rules = request.property_rules;
    const std::string use_class_token = branch == RuntimeWarriorSkillBranchV1::charge &&
        current->facts.shield ? "Skill_Warrior_Charge_ShieldBonus" : class_token;
    active.source_class_id = static_cast<int>(std::find(request.classes->names.begin(),
        request.classes->names.end(), use_class_token) - request.classes->names.begin());
    if (active.source_class_id < 0 ||
        std::size_t(active.source_class_id) >= request.classes->names.size()) {
        actor->transform.rotation[2] = old_heading;
        return fail(error, "Authored skill CLASS_ID token is unavailable in source ClassTables");
    }
    active.row_mask = skill_row.scalar.words[7];
    active.element = static_cast<std::int32_t>(skill_row.scalar.words[5]);
    active.category = current->facts.main_damage_class;
    active.warrior_branch = branch;
    active.source_post_clears_target =
        branch == RuntimeWarriorSkillBranchV1::bashdown ||
        branch == RuntimeWarriorSkillBranchV1::ground_slam ||
        branch == RuntimeWarriorSkillBranchV1::rogue_jump_kick;
    active.class_token = class_token;
    active.population_order = order;
    active.target_facts = request.target_facts;
    active.non_character_attackable_objects_absent = request.non_character_attackable_objects_absent;
    active.target_sort = branch == RuntimeWarriorSkillBranchV1::ground_slam
        ? SkillTargetSortV1::closest_first
        : branch == RuntimeWarriorSkillBranchV1::charge
            ? SkillTargetSortV1::source_order : SkillTargetSortV1::frontal_first;
    active.target_range = branch == RuntimeWarriorSkillBranchV1::bashdown ? 160.0f
        : branch == RuntimeWarriorSkillBranchV1::ground_slam
            ? static_cast<float>(projected_range)
            : branch == RuntimeWarriorSkillBranchV1::rogue_jump_kick ? 160.0f : 300.0f;
    active.target_cone_radians = branch == RuntimeWarriorSkillBranchV1::charge
        ? 2.09439510239319549f
        : branch == RuntimeWarriorSkillBranchV1::rogue_jump_kick
            ? 4.71238898038468986f : 3.1415927410125732421875f;
    active.cooldown_ms = cooldown_ms;
    active.target_query = target;
    // An admitted skill departs the swing (CSAttack::OnBlur/interrupt), so a
    // failed sequence start must not restore Attack with no attack owner.
    active.previous_action = actor->action == CharacterAction::attacking
        ? CharacterAction::idle : actor->action;

    const bool mana_ok = prepare_skill_cast_mana_v1(*request.character, session, request.actor,
            *request.characters, request.skills, *request.classes,
            *request.property_rules, position, class_token, request.mana_policy,
            active.receipt.mana, error);
    if (!mana_ok) {
        actor->transform.rotation[2] = old_heading;
        if (active.receipt.mana.mana_spent ||
            active.receipt.mana.status == SkillManaPrepareStatusV1::provider_failure_after_mana_commit) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Reached source UseMana prefix committed MP before a later provider failed";
            output = active.receipt;
            active_[request.actor] = std::move(active);
        }
        return false;
    }
    if (active.receipt.mana.status == SkillManaPrepareStatusV1::insufficient_mana) {
        actor->transform.rotation[2] = old_heading;
        active.receipt.phase = RuntimeSkillCastPhaseV1::rejected;
        active.receipt.detail = "Authored OnSkillCheck_ HasMana returned false; no UseMana prefix ran";
        output = active.receipt;
        active_[request.actor] = std::move(active);
        error.clear();
        return true;
    }
    if (active.receipt.mana.status != SkillManaPrepareStatusV1::mana_committed &&
        active.receipt.mana.status != SkillManaPrepareStatusV1::mana_bypass) {
        actor->transform.rotation[2] = old_heading;
        active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active.receipt.detail = "Source OnPre UseMana did not reach a completed result";
        output = active.receipt;
        active_[request.actor] = std::move(active);
        return false;
    }
    if (active.cooldown_ms > 0.0) {
        const auto ready = elapsed_ms_ + active.cooldown_ms;
        if (!std::isfinite(ready)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Source UseMana committed but skill timer overflowed";
            output = active.receipt;
            active_[request.actor] = std::move(active);
            return fail(error, "Source skill cooldown timer overflowed after UseMana");
        }
        skill_ready_at_ms_[{request.actor, visual.skill_table_id}] = ready;
        skill_cooldown_total_ms_[{request.actor, visual.skill_table_id}] = active.cooldown_ms;
    }

    const auto generation = active.receipt.generation;
    auto services = source_sequence_services(*this, session, request.actor,
        generation, 6, request.downstream_animation_services);
    const CombatSessionSourceSequencePolicy source_policy{6, 0x6341u, generation};

    actor->action = CharacterAction::casting;
    active_[request.actor] = std::move(active);
    const auto& retained = active_.at(request.actor);
    if (source_selection.state.empty() ||
        std::none_of(request.visual_plan->sequences.begin(), request.visual_plan->sequences.end(),
            [&](const OriginalCombatSequencePlan& sequence) {
                return sequence.id == visual.animation_sequence_id;
            })) {
        actor->action = retained.previous_action;
        active_.at(request.actor).receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active_.at(request.actor).receipt.detail = "Exact SkillTable animation root is not bound in the supplied source bank";
        output = active_.at(request.actor).receipt;
        return fail(error, "Skill cast requires its actual SkillTable animation sequence in the retained source bank");
    }
    if (!session.play_actor_source_sequence(request.actor, *request.visual_plan,
            *request.sequence_policies, source_selection, std::move(services),
            source_policy, error)) {
        actor->action = retained.previous_action;
        active_.at(request.actor).receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active_.at(request.actor).receipt.detail = "Source UseMana prefix committed but source sequence could not start";
        output = active_.at(request.actor).receipt;
        return false;
    }
    output = active_.at(request.actor).receipt;
    error.clear();
    return true;
}

bool RuntimeSkillCastCoordinatorV1::apply_use_v1(
    CombatSession& session, ActorId actor_id, std::uint64_t generation,
    RuntimeSkillCastReceiptV1& output, std::string& error) {
    error.clear();
    auto it = active_.find(actor_id);
    if (it == active_.end() || it->second.receipt.generation != generation)
        return fail(error, "Skill Use callback has no matching active cast occurrence");
    auto& active = it->second;
    if (active.receipt.phase == RuntimeSkillCastPhaseV1::use_applied ||
        active.receipt.phase == RuntimeSkillCastPhaseV1::completed) {
        output = active.receipt;
        return true;
    }
    if (active.receipt.phase != RuntimeSkillCastPhaseV1::prepared_pending_use ||
        active.binding_lease.expired() ||
        !same_owner(active.binding_lease, session.actor_binding_lease()))
        return fail(error, "Skill Use callback is stale or its source actor is not retained by this Session");
    if (active.faery_script == RuntimeSkillFaeryScriptV1::celest) {
#if defined(DH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST)
        return fail(error, "Celest branch is excluded from the Warrior link fixture");
#else
        if (!active.faery_tables || !active.hotty_cooldown_clock ||
            !active.character || !active.classes)
            return fail(error, "Celest Cast-state Use requires its retained source arm");
        faery_menu::CelestSourceUseV1 applied;
        if (!faery_menu::apply_celest_source_use_v1(session,
                active.celest_prepared, active.hotty_targets,
                *active.character, *active.hotty_cooldown_clock, active.faery_tables,
                *active.classes, active.celest_effect_dispatch, applied, error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Celest do_spell Use applied only its reached source roll prefix";
            if (!applied.partial_prefix_diagnostic.empty()) {
                active.receipt.detail += "; source prefix: ";
                active.receipt.detail += applied.partial_prefix_diagnostic;
            }
            for (const auto& roll : applied.rolls)
                active.receipt.applied_results.push_back(roll);
            active.receipt.celest_dead_target_calculations.insert(
                active.receipt.celest_dead_target_calculations.end(),
                applied.dead_target_calculations.begin(),
                applied.dead_target_calculations.end());
            for (const auto& fx : applied.target_effects) {
                if (fx.status == faery_menu::CelestEffectStatusV1::dispatch_failed ||
                    fx.status == faery_menu::CelestEffectStatusV1::not_requested)
                    append_fx_diagnostic(active.receipt.detail, "Celest target",
                        fx.diagnostic.empty() ? "dispatcher unavailable" : fx.diagnostic);
            }
            output = active.receipt;
            return false;
        }
        for (const auto& roll : applied.rolls)
            active.receipt.applied_results.push_back(roll);
        active.receipt.celest_dead_target_calculations.insert(
            active.receipt.celest_dead_target_calculations.end(),
            applied.dead_target_calculations.begin(),
            applied.dead_target_calculations.end());
        if (!applied.source_hits_applied || !applied.source_use_complete) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Celest do_spell Use did not complete its authored roll sequence";
            if (!applied.partial_prefix_diagnostic.empty()) {
                active.receipt.detail += "; source prefix: ";
                active.receipt.detail += applied.partial_prefix_diagnostic;
            }
            for (const auto& fx : applied.target_effects) {
                if (fx.status == faery_menu::CelestEffectStatusV1::dispatch_failed ||
                    fx.status == faery_menu::CelestEffectStatusV1::not_requested)
                    append_fx_diagnostic(active.receipt.detail, "Celest target",
                        fx.diagnostic.empty() ? "dispatcher unavailable" : fx.diagnostic);
            }
            output = active.receipt;
            return fail(error, "Celest do_spell did not complete the exact source roll loop");
        }
        for (const auto& fx : applied.target_effects) {
            if (fx.status == faery_menu::CelestEffectStatusV1::dispatch_failed ||
                fx.status == faery_menu::CelestEffectStatusV1::not_requested)
                append_fx_diagnostic(active.receipt.detail, "Celest target",
                    fx.diagnostic.empty() ? "dispatcher unavailable" : fx.diagnostic);
        }
        active.use_delivered = true;
        active.receipt.phase = RuntimeSkillCastPhaseV1::use_applied;
        std::string fx_diagnostics;
        const auto diagnostic_start = active.receipt.detail.find("FX diagnostic:");
        if (diagnostic_start != std::string::npos)
            fx_diagnostics = active.receipt.detail.substr(diagnostic_start);
        active.receipt.detail = "Celest source state7 do_spell completed its ordered two-roll result loop";
        if (!fx_diagnostics.empty()) {
            active.receipt.detail += "; ";
            active.receipt.detail += fx_diagnostics;
        }
        output = active.receipt;
        error.clear();
        return true;
#endif
    }
    if (active.is_hotty) {
#if defined(DH_RUNTIME_SKILL_CAST_WARRIOR_ONLY_TEST)
        return fail(error, "Hotty branch is excluded from the Warrior-only link fixture");
#else
        if (!active.faery_tables || !active.hotty_cooldown_clock ||
            !active.character || !active.classes)
            return fail(error, "Hotty Cast-state Use requires its retained source arm");
        faery_menu::HottySourceUseV1 applied;
        if (!faery_menu::apply_hotty_source_use_v1(session,
                active.hotty_prepared, active.hotty_targets,
                *active.character, *active.hotty_cooldown_clock, active.faery_tables,
                *active.classes, active.hotty_effect_dispatch, applied, error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Hotty do_spell Use applied only its reached source result prefix";
            for (const auto& hit : applied.combat.hits)
                active.receipt.applied_results.push_back(hit.receipt);
            output = active.receipt;
            return false;
        }
        const bool source_hits_complete = applied.combat.status ==
            faery_menu::HottyCharacterApplyStatusV1::source_ordered_character_hits_applied ||
            (active.hotty_targets.character_targets_in_source_order.empty() &&
             applied.combat.status == faery_menu::HottyCharacterApplyStatusV1::no_character_targets);
        if (!applied.source_hits_applied || !source_hits_complete) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Hotty do_spell Use did not apply complete source-ordered results";
            output = active.receipt;
            return fail(error, "Hotty do_spell did not return complete per-character result receipts");
        }
        for (const auto& hit : applied.combat.hits)
            active.receipt.applied_results.push_back(hit.receipt);
        for (const auto& fx : applied.target_effects) {
            if (fx.status == faery_menu::HottyEffectStatusV1::dispatch_failed ||
                fx.status == faery_menu::HottyEffectStatusV1::not_requested) {
                append_fx_diagnostic(active.receipt.detail, "target",
                    fx.diagnostic.empty() ? hotty_effect_status_name(fx.status) : fx.diagnostic);
            }
        }
        active.use_delivered = true;
        active.receipt.phase = RuntimeSkillCastPhaseV1::use_applied;
        // Keep retained FX diagnostics (Player_Pre / target FX) after the Use summary.
        std::string hotty_fx_diagnostics;
        const auto hotty_diagnostic_start = active.receipt.detail.find("FX diagnostic:");
        if (hotty_diagnostic_start != std::string::npos)
            hotty_fx_diagnostics = active.receipt.detail.substr(hotty_diagnostic_start);
        active.receipt.detail = "Hotty source state7 do_spell applied its ordered same-session result loop";
        if (!hotty_fx_diagnostics.empty()) {
            active.receipt.detail += "; ";
            active.receipt.detail += hotty_fx_diagnostics;
        }
        output = active.receipt;
        error.clear();
        return true;
#endif
    }
    if (!active.character || !active.classes || !active.property_rules ||
        active.receipt.actor == invalid_actor_id)
        return fail(error, "Skill Use source class/property borrowers have expired");
    auto* world = session.world();
    auto* actor = session.actor(actor_id);
    const auto* current = world ? world->combat_properties(actor_id) : nullptr;
    if (!world || !actor || !current || actor != world->find_actor(actor_id) || !actor->alive())
        return fail(error, "Skill Use requires the same live Session actor/property owner");
    if (!active.characters || !active.classes || !active.property_rules)
        return fail(error, "Skill Use lost its source Character/Class/PropertyRules borrower");

    SkillActorTargetQueryV1 use_targets = active.target_query;
    if (active.warrior_branch != RuntimeWarriorSkillBranchV1::bashdown) {
        const float range = active.warrior_branch == RuntimeWarriorSkillBranchV1::charge
            ? 300.0f : active.target_range;
        const auto sort = active.warrior_branch == RuntimeWarriorSkillBranchV1::ground_slam
            ? SkillTargetSortV1::closest_first
            : active.warrior_branch == RuntimeWarriorSkillBranchV1::charge
                ? SkillTargetSortV1::source_order : SkillTargetSortV1::frontal_first;
        if (!query_source_character_targets_v1(session, actor_id,
                active.population_order, active.target_facts,
                active.non_character_attackable_objects_absent,
                range, active.target_cone_radians, sort, use_targets, error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Source Use target query failed after the OnPre mana/cooldown prefix";
            output = active.receipt;
            return false;
        }
    }
    if (use_targets.status == SkillActorTargetQueryStatusV1::non_character_target_unknown) {
        active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active.receipt.detail = "Source Use reached an AttackableOnly non-Character candidate outside this host scope";
        output = active.receipt;
        return fail(error, "Character-only Session cannot resolve the source Use target-list object category");
    }
    if (use_targets.status != SkillActorTargetQueryStatusV1::selected_character &&
        use_targets.status != SkillActorTargetQueryStatusV1::no_actor_target) {
        active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active.receipt.detail = "Source Use target-list query returned an unsupported status after OnPre";
        output = active.receipt;
        return fail(error, "Source Use requires a complete Character subset target-list result");
    }

    // BashDown retains its OnPre-selected Lua-local target and does not search
    // again at Use. GroundSlam/Charge/JumpKick perform their authored Use-time
    // search; JumpKick uses source range160/angle270 with MaxAngleSearch.
    // A complete empty Character subset remains an accepted empty Lua loop.
    if (active.warrior_branch == RuntimeWarriorSkillBranchV1::bashdown) {
        if (use_targets.status == SkillActorTargetQueryStatusV1::selected_character) {
            use_targets.ordered_character_targets.assign(1, use_targets.selected);
        } else {
            use_targets.ordered_character_targets.clear();
        }
    }
    active.receipt.target_order = use_targets.ordered_character_targets;
    const auto* use_class = active.class_token.c_str();
    if (active.warrior_branch == RuntimeWarriorSkillBranchV1::charge && current->facts.shield)
        use_class = "Skill_Warrior_Charge_ShieldBonus";
    double ignored_cooldown = 0.0, ignored_range = 0.0, ignored_range_growth = 0.0;
    if (!source_skill_class_projection(*active.characters, *active.classes,
            *active.property_rules, current->sheets.resolved, use_class,
            active.receipt.mana.skill.saved_rank, active.formula_sheet,
            ignored_cooldown, ignored_range, ignored_range_growth, error)) {
        active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active.receipt.detail = "Source ApplyPropClass failed after committed OnPre prefix";
        output = active.receipt;
        return false;
    }
    if (active.warrior_branch == RuntimeWarriorSkillBranchV1::ground_slam &&
        use_targets.status == SkillActorTargetQueryStatusV1::selected_character &&
        !use_targets.ordered_character_targets.empty()) {
        use_targets.selected = use_targets.ordered_character_targets.front();
        if (!look_at_bashdown_target_v1(session, use_targets, error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "GroundSlam source LookAt failed after OnPre";
            output = active.receipt;
            return false;
        }
    }

    active.receipt.applied_results.clear();
    std::uint32_t event_index = 0;
    for (const auto target : use_targets.ordered_character_targets) {
        CombatSessionSourceHit hit;
        hit.attacker = actor_id;
        hit.target = target;
        hit.binding_lease = session.actor_binding_lease();
        hit.generation = generation;
        hit.event_index = event_index++;
        hit.mask = active.row_mask | 0x08000000u; // Native SkillCombatRoll SkillAttack bit.
        hit.source_id = active.receipt.skill_name;
        hit.marker_name = "do_skill";
        hit.category = current->facts.main_damage_class;
        hit.element = active.element;
        hit.direct_amount = 0;
        hit.attacker_formula_sheet = &active.formula_sheet;
        DamageEvent result;
        if (!session.apply_source_result(hit, result, error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Source SkillCombatRoll loop failed after its applied ordered hit prefix";
            output = active.receipt;
            return false;
        }
        active.receipt.applied_results.push_back(std::move(result));
    }
    active.use_delivered = true;
    active.receipt.phase = RuntimeSkillCastPhaseV1::use_applied;
    active.receipt.detail = active.receipt.applied_results.empty()
        ? "Authored source skill Use completed with an empty Character subset target list"
        : "Authored source skill Use applied the ordered Character SkillCombatRoll result loop";
    output = active.receipt;
    error.clear();
    return true;
}

bool RuntimeSkillCastCoordinatorV1::apply_retained_use_event_v1(
    CombatSession& session, ActorId actor_id, std::uint64_t generation,
    RuntimeSkillSourceAnimStateV1 source_state,
    const RetainedAnimationEvent& event,
    RuntimeSkillCastReceiptV1& output, std::string& error) {
    const auto it = active_.find(actor_id);
    if (it == active_.end() || it->second.receipt.generation != generation)
        return fail(error, "Retained source Use event has no matching cast occurrence");
    const bool faery_cast = it->second.is_hotty ||
        it->second.faery_script == RuntimeSkillFaeryScriptV1::celest;
    const auto required_state = faery_cast ? RuntimeSkillSourceAnimStateV1::cast
                                          : RuntimeSkillSourceAnimStateV1::skill;
    const char* required_event = faery_cast ? "do_spell" : "do_skill";
    if (source_state != required_state || event.name != required_event)
        return fail(error, "Retained source Use event does not match its authored state6/state7 event route");
    if (it->second.receipt.phase == RuntimeSkillCastPhaseV1::interrupted)
        return fail(error, "Retained source Use marker belongs to a retired interrupted generation");
    if (it->second.receipt.phase == RuntimeSkillCastPhaseV1::use_applied ||
        it->second.receipt.phase == RuntimeSkillCastPhaseV1::completed)
        return apply_use_v1(session, actor_id, generation, output, error);
    const auto* actor = session.actor(actor_id);
    if (!actor || !actor->alive() || actor->action == CharacterAction::hurt ||
        actor->action == CharacterAction::dead)
        return fail(error, "Retained source Use event reached an unavailable or interrupted cast owner");
    return apply_use_v1(session, actor_id, generation, output, error);
}

bool RuntimeSkillCastCoordinatorV1::complete_v1(
    CombatSession& session, ActorId actor_id, std::uint64_t generation,
    RuntimeSkillCastReceiptV1& output, std::string& error) {
    error.clear();
    auto it = active_.find(actor_id);
    if (it == active_.end() || it->second.receipt.generation != generation)
        return fail(error, "Skill Post callback has no matching cast occurrence");
    auto& active = it->second;
    if (active.binding_lease.expired() ||
        !same_owner(active.binding_lease, session.actor_binding_lease()))
        return fail(error, "Skill Post callback belongs to an expired CombatSession owner");
    if (active.receipt.phase == RuntimeSkillCastPhaseV1::interrupted)
        return fail(error, "Skill completion belongs to a retired interrupted generation");
    if (!active.use_delivered || active.receipt.phase != RuntimeSkillCastPhaseV1::use_applied) {
        active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
        active.receipt.detail = active.is_hotty
            ? "Hotty Cast sequence completed without its authored state7 do_spell Use callback"
            : active.faery_script == RuntimeSkillFaeryScriptV1::celest
                ? "Celest Cast sequence completed without its authored state7 do_spell Use callback"
                : "Skill sequence completed without its authored state6 do_skill Use callback";
        output = active.receipt;
        return fail(error, "Source Post cannot complete before its authored retained Use event");
    }
    auto* actor = session.actor(actor_id);
    if (!actor) return fail(error, "Source Post requires its same-Session actor owner");
    if (active.source_post_clears_target)
        actor->target_id = invalid_actor_id;
    if (actor->action == CharacterAction::casting)
        actor->action = active.previous_action;
    active.source_post_delivered = true;
    std::string preserved_fx_diagnostics;
    const auto diagnostic_start = active.receipt.detail.find("FX diagnostic:");
    if (diagnostic_start != std::string::npos)
        preserved_fx_diagnostics = active.receipt.detail.substr(diagnostic_start);
    active.receipt.phase = RuntimeSkillCastPhaseV1::completed;
    active.receipt.detail = active.is_hotty
        ? "Hotty retained state7 source Use and Cast sequence completed"
            : active.faery_script == RuntimeSkillFaeryScriptV1::celest
                ? "Celest retained state7 source Use and Cast sequence completed"
                : active.source_post_clears_target
                    ? "Retained state6 source Use/Post completed and authored ClearTarget was applied"
                    : "Retained state6 source Use/Skill sequence completed; conditional or owner-local Post remains unprojected";
    if (!preserved_fx_diagnostics.empty()) {
        active.receipt.detail += "; ";
        active.receipt.detail += preserved_fx_diagnostics;
    }
    output = active.receipt;
    error.clear();
    return true;
}

RuntimeSkillCastReceiptV1* RuntimeSkillCastCoordinatorV1::receipt(
    ActorId actor) noexcept {
    const auto found = active_.find(actor);
    return found == active_.end() ? nullptr : &found->second.receipt;
}

const RuntimeSkillCastReceiptV1* RuntimeSkillCastCoordinatorV1::receipt(
    ActorId actor) const noexcept {
    const auto found = active_.find(actor);
    return found == active_.end() ? nullptr : &found->second.receipt;
}

double RuntimeSkillCastCoordinatorV1::skill_cooldown_remaining_fraction_v1(
    ActorId actor, int skill_table_id) const noexcept {
    const auto ready = skill_ready_at_ms_.find({actor, skill_table_id});
    const auto total = skill_cooldown_total_ms_.find({actor, skill_table_id});
    if (ready == skill_ready_at_ms_.end() || total == skill_cooldown_total_ms_.end()) return 0.0;
    return pc_cooldown_remaining_fraction_v1(ready->second, elapsed_ms_, total->second);
}

} // namespace dh::foundation::generic_skills
