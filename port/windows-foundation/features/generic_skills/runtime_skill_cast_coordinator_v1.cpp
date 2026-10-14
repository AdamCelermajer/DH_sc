#include "runtime_skill_cast_coordinator_v1.hpp"
#include "runtime_skill_use_profile_registry_v1.hpp"
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
[[maybe_unused]] void request_faery_pre_sound(const RuntimeSkillFaeryPreSoundSinkV1& sink, CombatSession& session,
                             ActorId caster, bool hotty, std::size_t target_count,
                             const faery_menu::FaeryAbilityV1* ability = nullptr) {
    if (!sink) return;
    RuntimeSkillFaeryPreSoundV1 request;
    request.caster = caster;
    request.target_count = target_count;
    if (const auto* actor = session.actor(caster))
        request.position = {actor->transform.position[0], actor->transform.position[1], actor->transform.position[2]};
    // v21.8: registry-keyed labels for strike Faeries; Celest keeps its own table.
    request.labels = ability ? faery_menu::faery_strike_pre_sound_labels_v1(*ability, target_count)
                             : faery_menu::faery_pre_sound_labels_v1(hotty, target_count);
    sink(request);
}

// v21.8: post-hit PlaySound after the source target loop (Rocky, Wetty, Windy).
// Delivered with hit_phase=true so the host can keep it separate from the 3D Pre cue.
void request_faery_hit_sound(const RuntimeSkillFaeryPreSoundSinkV1& sink, CombatSession& session,
                             ActorId caster, const faery_menu::FaeryAbilityV1& ability,
                             std::size_t target_count) {
    auto labels = faery_menu::faery_strike_hit_sound_labels_v1(ability);
    if (!sink || labels.empty()) return;
    RuntimeSkillFaeryPreSoundV1 request;
    request.caster = caster;
    request.target_count = target_count;
    request.hit_phase = true;
    if (const auto* actor = session.actor(caster))
        request.position = {actor->transform.position[0], actor->transform.position[1], actor->transform.position[2]};
    request.labels = std::move(labels);
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
    services.focused=std::move(existing.focused);
    existing.focused={}; // downstream retained callbacks cannot retain Focus stack captures
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
                            int table_id, const RuntimeSkillUseProfileV1*& profile,
                            std::string& token, const RuntimeSkillUseProfileV1* profile_override) {
    profile = nullptr;
    if (!tables || table_id < 0 || std::size_t(table_id) >= tables.skills().size() ||
        std::size_t(table_id) >= tables.skill_names().size()) return false;
    const auto& skill = tables.skills()[std::size_t(table_id)];
    const auto& name = tables.skill_names()[std::size_t(table_id)];
    // Admission is by the exact SkillTable (name, script) pair of a row in
    // kRuntimeSkillUseProfilesV1, transcribed from the original skill Lua.
    profile = find_skill_use_profile_registered_v1(name, skill.script);
    if (!profile) return false;
    // v21.8 ES-API test seam: an authored variant of the SAME admitted (name, script) row (presentation arms, probe values).
    // Hosts leave RuntimeSkillCastRequestV1::profile_override null.
    if (profile_override && profile_override->skill_name == name && profile_override->script == skill.script)
        profile = profile_override;
    token = profile->class_token;
    return true;
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
    // v21.8: dispatch from the faery ability registry keyed by the authored script.
    // Strike Faeries (Hotty, Rocky, Wetty, Windy) share the Hotty-shaped arm.
    const auto* entry = faery_menu::find_faery_ability_by_script_v1(row.script);
    if (!entry || tables.faery_names()[static_cast<std::size_t>(row_id)] != std::string(entry->faery_name)) {
        error = "Current source Faery script is not in the faery ability registry; no substitution is permitted";
        return false;
    }
    script = entry->kind == faery_menu::FaeryAbilityKindV1::celest_lightning
        ? RuntimeSkillFaeryScriptV1::celest
        : RuntimeSkillFaeryScriptV1::hotty;
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
    // B097: the source SetProp(SnS_Level) lands on the owner's resolved sheet,
    // which mode-1 ApplyPropClass reads as the type-1 source (see mana_v1).
    auto live_with_rank = live;
    live_with_rank[skill_level] = output[skill_level];
    if (!dh2::data::apply_class(classes, static_cast<std::int32_t>(found - classes.names.begin()),
                               output, error, &live_with_rank)) {
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
    // CSInjured::OnInit3c8850 likewise registers50005->6 unguarded; the
    // session transition owner runs outgoing Injured Blur and retires its pose.
    if (actor->action == CharacterAction::casting)
        return fail(error, "Source skill is already casting; a new skill cast is rejected");
    // NativeHUDSpell/AI_BeginSpell3d81c0 uses SM_SetCastState3c6394
    // event50006. CSInjured registers only50005->Skill6, not Cast7.
    if (actor->action == CharacterAction::hurt &&
        request.dispatch == RuntimeSkillCastDispatchV1::native_hud_spell)
        return fail(error, "CSInjured has no native Cast7 event50006 admission");
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
        // v21.8: the strike arm serves Hotty, Rocky, Wetty and Windy; name the receipt by the selected Faery's registry entry.
        const auto* strikeAbility = faery_menu::find_faery_ability_by_script_v1(
            arm->tables->faeries()[static_cast<std::size_t>(selected_faery_row)].script);
        const std::string strikeScript = strikeAbility ? std::string(strikeAbility->script) : std::string("faerie_hotty");
        const std::string strikeName = strikeAbility ? std::string(strikeAbility->faery_name) : std::string("Hotty");
        active.receipt.skill_name = strikeScript;
        active.receipt.source_script = strikeScript;
        active.receipt.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
        active.source_state = 7;
        active.source_focus_flags = 0x6301u;
        active.receipt.detail = strikeName + " OnPre completed; waiting for its distinct source Cast-state Use owner";
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
        // v21.8: the receipt names the selected registry Faery script (Hotty, Rocky, Wetty or Windy).
        if (active.hotty_prepared.ability)
            active.receipt.skill_name = std::string(active.hotty_prepared.ability->script);
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
                                active.hotty_prepared.character_targets.size(),
                                active.hotty_prepared.ability);
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
    const RuntimeSkillUseProfileV1* profile = nullptr;
    if (!source_supported_skill(request.skills, visual.skill_table_id, profile, class_token, request.profile_override)) {
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
    if (profile->buff && !request.timed_buffs) {
        output.actor = request.actor;
        output.source_slot = request.source_slot;
        output.saved_skill_row = saved_row;
        output.class_skill_position = position;
        output.skill_table_id = visual.skill_table_id;
        output.skill_name = visual.source_skill_name;
        output.source_script = visual.source_script;
        output.phase = RuntimeSkillCastPhaseV1::rejected;
        output.detail = "Skill creates a timed buff but no timed-buff owner is bound to this request";
        error.clear();
        return true;
    }
    if (profile->requires_bow && !request.main_hand_is_bow) {
        output.actor = request.actor;
        output.source_slot = request.source_slot;
        output.saved_skill_row = saved_row;
        output.class_skill_position = position;
        output.skill_table_id = visual.skill_table_id;
        output.skill_name = visual.source_skill_name;
        output.source_script = visual.source_script;
        output.phase = RuntimeSkillCastPhaseV1::rejected;
        output.detail = "OnSkillCheck_ HasBow() is false: the main hand holds no bow";
        error.clear();
        return true;
    }
    const auto& skill_row = request.skills.skills()[std::size_t(visual.skill_table_id)];
    // Native _SkillCombatRoll (IDA 0x3b9fbc; audit docs/ida-fidelity-audit-2026-10-11/character3.md F2):
    // mask 0x800000 needs the equipped-hand facts before any hit is formed.
    const bool paired_hand = (skill_row.scalar.words[7] & 0x00800000u) != 0;
    if (paired_hand && !request.hand_facts_known)
        return fail(error, "Paired-hand skill result requires the equipped main/off-hand facts");

    // Borrow the live population enrollment order. Source Pre consumes it
    // after outgoing Blur, using the then-current heading and typed objects.
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
    // DeathClaw.luac OnPreSkill_: SetSkillCooldown(COOLDOWN_BASE) is a Lua constant, not the class SnS_Cooldown.
    if (profile->fixed_cooldown_ms > 0.0f) cooldown_ms = static_cast<double>(profile->fixed_cooldown_ms);
    // Only the authored OnPre SetSkillCooldown owners start a source timer.
    if (profile->sets_cooldown && cooldown_ms > 0.0) {
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

    active.receipt.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
    active.source_state = 6;
    active.source_focus_flags = 0x6341u;
    active.binding_lease = session.actor_binding_lease();
    active.character = request.character;
    active.characters = request.characters;
    active.classes = request.classes;
    active.property_rules = request.property_rules;
    const std::string use_class_token = profile->shield_variant &&
        current->facts.shield ? "Skill_Warrior_Charge_ShieldBonus" : class_token;
    active.source_class_id = static_cast<int>(std::find(request.classes->names.begin(),
        request.classes->names.end(), use_class_token) - request.classes->names.begin());
    if (active.source_class_id < 0 ||
        std::size_t(active.source_class_id) >= request.classes->names.size()) {
        return fail(error, "Authored skill CLASS_ID token is unavailable in source ClassTables");
    }
    active.row_mask = skill_row.scalar.words[7];
    active.paired_hand = paired_hand;
    active.has_main_hand = request.has_main_hand_weapon;
    active.has_off_hand = request.has_off_hand_weapon;
    active.element = static_cast<std::int32_t>(skill_row.scalar.words[5]);
    active.category = current->facts.main_damage_class;
    active.use_profile = profile;
    active.timed_buffs = request.timed_buffs;
    active.source_post_clears_target = profile->post_clear_target;
    active.class_token = class_token;
    active.external_owner = request.external_owner;
    active.population_order = order;
    active.target_facts = request.target_facts;
    active.non_character_attackable_objects_absent = request.non_character_attackable_objects_absent;
    active.target_sort = profile->use_sort;
    active.target_range = profile->use_range_from_temp_prop1
        ? static_cast<float>(projected_range) : profile->use_range;
    active.target_cone_radians = profile->use_cone_radians;
    active.cooldown_ms = profile->sets_cooldown ? cooldown_ms : 0.0;
    active.pre_range = profile->pre_range_from_temp_prop1 ? static_cast<float>(projected_range) : profile->pre_range;
    active.knockback_great = request.knockback_great;
    if (request.object_targets) active.object_targets = *request.object_targets;
    // An admitted skill departs the swing (CSAttack::OnBlur/interrupt), so a
    // failed sequence start must not restore Attack with no attack owner.
    active.previous_action = (actor->action == CharacterAction::attacking || actor->action == CharacterAction::hurt)
        ? CharacterAction::idle : actor->action;

    // AI_IsSkillUsable/OnSkillCheck HasMana precedes any source departure.
    if(!prepare_skill_cast_mana_v1(*request.character,session,request.actor,
        *request.characters,request.skills,*request.classes,*request.property_rules,
        position,class_token,request.mana_policy,active.receipt.mana,error,true))return false;
    if(active.receipt.mana.status==SkillManaPrepareStatusV1::insufficient_mana){
        active.receipt.phase=RuntimeSkillCastPhaseV1::rejected;
        active.receipt.detail="Authored OnSkillCheck_ HasMana returned false; no departure or UseMana prefix ran";
        output=active.receipt;active_[request.actor]=std::move(active);error.clear();return true;
    }

    const auto generation = active.receipt.generation;
    auto services = source_sequence_services(*this, session, request.actor,
        generation, 6, request.downstream_animation_services);
    const CombatSessionSourceSequencePolicy source_policy{6, 0x6341u, generation};

    if (source_selection.state.empty() ||
        std::none_of(request.visual_plan->sequences.begin(), request.visual_plan->sequences.end(),
            [&](const OriginalCombatSequencePlan& sequence) {
                return sequence.id == visual.animation_sequence_id;
            })) {
        return fail(error, "Skill cast requires its actual SkillTable animation sequence in the retained source bank");
    }
    active_[request.actor]=std::move(active);
    const auto downstream_focused=std::move(services.focused);
    services.focused=[&,generation,downstream_focused,profile](ActorId focused_actor,std::string& error){
        if(focused_actor!=request.actor)return fail(error,"Source Focus Pre reached a foreign actor");
        auto& active=active_.at(request.actor);
        if(active.receipt.generation!=generation)return fail(error,"Source Focus Pre occurrence is stale");
        actor->action=CharacterAction::casting;
        SkillActorTargetQueryV1 target;
        target.caster = request.actor;
        target.status = SkillActorTargetQueryStatusV1::no_actor_target;
        // pre_range == 0: the authored OnPre has no target query (GroundSlam).
        if (active.pre_range > 0.0f && !query_source_character_targets_v1(session, request.actor,
                order, request.target_facts, request.non_character_attackable_objects_absent,
                active.pre_range, profile->pre_cone_radians,
                profile->pre_sort, target, error, request.object_targets)) return false;
        if(target.status==SkillActorTargetQueryStatusV1::non_character_target_unknown)
            return fail(error,"Source Focus Pre target query lacks its typed AttackableOnly owner");

        if (profile->pre_look_at &&
            (target.status == SkillActorTargetQueryStatusV1::selected_character ||
             target.status == SkillActorTargetQueryStatusV1::selected_object) &&
            !look_at_bashdown_target_v1(session, target, error)) return false;

        active.target_query=target;
        active.receipt.target_order=request.object_targets?target.ordered_targets:target.ordered_character_targets;
        // v21.8 berserker: prince_berseeker_berserk.luac OnPreSkill_ self HP cost, after the target query and LookAt and
        // before UseMana: `if HPPercentCurr > 10 then SetProp(HP, health - health/10)`. The Lua order is kept; a later
        // UseMana failure after this write is reported as a partial failure like the other OnPre prefixes.
        if (profile->self_hp_cost) {
            float health = 0.0f, maximum = 0.0f;
            auto* live_world = session.world();
            if (!live_world || !live_world->current_health(request.actor, health, maximum) || !(maximum > 0.0f))
                return fail(error, "Source OnPre HP cost requires the caster's live HP sheet");
            if (health / maximum * 100.0f > profile->self_hp_cost->gate_percent) {
                std::string hp_error;
                if (!live_world->set_current_health(request.actor, health - health / profile->self_hp_cost->divisor, hp_error))
                    return fail(error, ("Source OnPre HP cost SetProp(HP): " + hp_error).c_str());
            }
        }
        // v21.8 ES-API: OnPre `PopTargetList` + `target:PlayFX(...)` for every popped Character (unconditional).
        if (profile->pre_target_fx_name)
            for (const auto victim : target.ordered_character_targets)
                active.receipt.presentation_cues.push_back({SkillPresentationCueV1::Kind::target_fx,
                                                            profile->pre_target_fx_name, victim});
        const bool mana_ok = prepare_skill_cast_mana_v1(*request.character, session, request.actor,
                *request.characters, request.skills, *request.classes,
                *request.property_rules, position, class_token, request.mana_policy,
                active.receipt.mana, error);
        if (!mana_ok) {
            if (active.receipt.mana.mana_spent ||
                active.receipt.mana.status == SkillManaPrepareStatusV1::provider_failure_after_mana_commit) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Reached source UseMana prefix committed MP before a later provider failed";
                output = active.receipt;
            }
            return false;
        }
        if (active.receipt.mana.status == SkillManaPrepareStatusV1::insufficient_mana) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::rejected;
            active.receipt.detail = "Authored OnSkillCheck_ HasMana returned false; no UseMana prefix ran";
            output = active.receipt;
            return fail(error,"Source UseMana no longer has the admitted mana after departure");
        }
        if (active.receipt.mana.status != SkillManaPrepareStatusV1::mana_committed &&
            active.receipt.mana.status != SkillManaPrepareStatusV1::mana_bypass) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Source OnPre UseMana did not reach a completed result";
            output = active.receipt;
            return false;
        }
        if (active.cooldown_ms > 0.0) {
            const auto ready = elapsed_ms_ + active.cooldown_ms;
            if (!std::isfinite(ready)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Source UseMana committed but skill timer overflowed";
                output = active.receipt;
                return fail(error, "Source skill cooldown timer overflowed after UseMana");
            }
            skill_ready_at_ms_[{request.actor, visual.skill_table_id}] = ready;
            skill_cooldown_total_ms_[{request.actor, visual.skill_table_id}] = active.cooldown_ms;
        }

        // prince_rogue_quickness.luac OnPreSkill_: RemoveBuff(BUFF_ID, buff) then UseMana. The two touch different owners (buff
        // list vs MP), so their relative order is not observable; the removal is the same-class replacement of the previous cast.
        if (profile->buff && active.timed_buffs) {
            std::string buff_error;
            if (!active.timed_buffs->remove_timed_buff(session, request.actor, profile->buff->buff_class, buff_error)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Source OnPre RemoveBuff failed after the UseMana prefix: " + buff_error;
                output = active.receipt;
                return fail(error, active.receipt.detail.c_str());
            }
        }
        if (profile->external_owner) {
            // v21.8 mage: the skill's authored OnPreSkill_ body after UseMana (PlayFX, SpawnSkillProjectile, counters).
            if (!active.external_owner || !active.external_owner->handles(*profile)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Authored skill Pre needs its host-supplied external owner";
                output = active.receipt;
                return fail(error, "Skill profile requires an external Pre/Use owner that the host did not supply");
            }
            RuntimeSkillExternalContextV1 external;
            external.caster = request.actor;
            external.generation = generation;
            external.profile = profile;
            external.skill_name = active.receipt.skill_name;
            external.rank = active.receipt.mana.skill.saved_rank;
            external.row_mask = active.row_mask;
            external.element = active.element;
            external.category = active.category;
            external.characters = request.characters;
            external.classes = request.classes;
            external.rules = request.property_rules;
            external.ordered_character_targets = &active.target_query.ordered_character_targets;
            external.population_order = &active.population_order;
            external.target_facts = &active.target_facts;
            external.non_character_attackable_objects_absent = active.non_character_attackable_objects_absent;
            if (!active.external_owner->pre(session, external, error)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Authored skill OnPre external owner failed after the UseMana prefix";
                output = active.receipt;
                return false;
            }
        }
        return !downstream_focused||downstream_focused(focused_actor,error);
    };
    if (!session.play_actor_source_sequence(request.actor,*request.visual_plan,
        *request.sequence_policies,source_selection,std::move(services),source_policy,error)) {
        auto& failed=active_.at(request.actor);
        failed.receipt.phase=RuntimeSkillCastPhaseV1::partial_failure;
        failed.receipt.detail=failed.receipt.mana.mana_spent
            ? "Source Focus Pre failed after its reached UseMana prefix"
            : "Source sequence/Focus Pre failed before a UseMana debit; reached departure prefix retained";
        output=failed.receipt;return false;
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
        active.receipt.detail = "Celest source state7 do_spell completed its ordered one-roll-per-Character result loop";
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
        if (active.hotty_prepared.ability)
            request_faery_hit_sound(faery_pre_sound_sink_, session, active.hotty_prepared.caster,
                                    *active.hotty_prepared.ability,
                                    active.hotty_targets.character_targets_in_source_order.size());
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

    if (!active.use_profile)
        return fail(error, "Skill Use has no admitted source use profile");
    const auto& profile = *active.use_profile;
    SkillActorTargetQueryV1 use_targets = active.target_query;
    if (profile.use_targets == SkillUseTargetsV1::requery_at_use) {
        if (!query_source_character_targets_v1(session, actor_id,
                active.population_order, active.target_facts,
                active.non_character_attackable_objects_absent,
                active.target_range, active.target_cone_radians, active.target_sort,
                use_targets, error,
                active.object_targets ? &*active.object_targets : nullptr)) {
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
        use_targets.status != SkillActorTargetQueryStatusV1::selected_object &&
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
    if (profile.use_targets == SkillUseTargetsV1::pre_selected_single) {
        if (use_targets.status == SkillActorTargetQueryStatusV1::selected_character) {
            use_targets.ordered_character_targets.assign(1, use_targets.selected);
            use_targets.ordered_targets.assign(1, use_targets.selected);
            use_targets.ordered_object_targets.clear();
        } else if (use_targets.status == SkillActorTargetQueryStatusV1::selected_object) {
            use_targets.ordered_character_targets.clear();
            use_targets.ordered_targets.assign(1, use_targets.selected);
            use_targets.ordered_object_targets.assign(1, use_targets.selected);
        } else {
            use_targets.ordered_character_targets.clear();
            use_targets.ordered_targets.clear();
            use_targets.ordered_object_targets.clear();
        }
    }
    // v21.8 es-runtime: per-volley Use. The volley is chosen by the Uses already delivered for this actor and skill
    // (Lua skill_inc): its ClassTable row replaces the row class, and a top-only volley keeps the first target.
    const auto volley_key = std::make_pair(actor_id, active.receipt.skill_table_id);
    const RuntimeSkillVolleyV1* volley = profile.volley_count ? select_volley_v1(profile, volley_cursor_[volley_key]) : nullptr;
    if (volley && volley->top_target_only) {
        if (use_targets.ordered_character_targets.size() > 1) use_targets.ordered_character_targets.resize(1);
        if (use_targets.ordered_targets.size() > 1) use_targets.ordered_targets.resize(1);
    }
    active.receipt.target_order = active.object_targets ? use_targets.ordered_targets : use_targets.ordered_character_targets;
    const auto* use_class = active.class_token.c_str();
    if (profile.shield_variant && current->facts.shield)
        use_class = "Skill_Warrior_Charge_ShieldBonus";
    if (volley) use_class = volley->class_token;
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
    if (profile.use_look_at &&
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

    if (profile.buff) {
        // prince_rogue_quickness.luac OnSkill_: SetProp(SnS_Level); CreateBuff(BUFF_ID, bufftime, false, slvl, BUFF_FX);
        // ApplyBuff. bufftime is the projected class SnS_Duration (milliseconds), or TIME_BASE + TIME_INC * level when 0.
        if (!active.timed_buffs) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Timed-buff Use lost its buff owner after the committed OnPre prefix";
            output = active.receipt;
            return fail(error, "Timed-buff Use requires its buff owner");
        }
        const std::uint32_t rank = active.receipt.mana.skill.saved_rank;
        // CharacterProperties.SnS_Duration (176) unless the authored Lua reads another duration cell (PoisonSting: Dot_Duration 125).
        double duration_ms = original_signed256(active.formula_sheet[std::size_t(profile.buff->duration_prop_index)]);
        if (!(duration_ms > 0.0) || !std::isfinite(duration_ms))
            duration_ms = double(profile.buff->fallback_base_ms) + double(profile.buff->fallback_inc_ms) * double(rank);
        // Paladin auras: CreateBuff(BUFF_ID, BUFF_TIME, ...) with the authored constant, not the projected duration.
        if (profile.buff->fixed_duration_ms > 0.0f) duration_ms = double(profile.buff->fixed_duration_ms);
        TimedBuffSpecV1 spec;
        spec.class_name = profile.buff->buff_class;
        spec.fx_set_name = profile.buff->fx_set ? profile.buff->fx_set : "";
        spec.strength = rank;
        spec.duration_ms = duration_ms;
        spec.source = active.receipt.skill_name;
        std::string buff_error;
        if (!active.timed_buffs->apply_timed_buff(session, actor_id, spec, buff_error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Source CreateBuff/ApplyBuff failed after the committed OnPre prefix: " + buff_error;
            output = active.receipt;
            return fail(error, active.receipt.detail.c_str());
        }
        active.receipt.timed_buff_class = spec.class_name;
        active.receipt.timed_buff_duration_ms = duration_ms;
    }

    if (profile.external_owner) {
        // v21.8 mage: authored OnSkill_ body executed by the host-supplied owner (skill projectile / timer arms).
        if (!active.external_owner || !active.external_owner->handles(profile)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Authored skill Use needs its host-supplied external owner";
            output = active.receipt;
            return fail(error, "Skill profile requires an external Pre/Use owner that the host did not supply");
        }
        RuntimeSkillExternalContextV1 external;
        external.caster = actor_id;
        external.generation = generation;
        external.profile = &profile;
        external.skill_name = active.receipt.skill_name;
        external.rank = active.receipt.mana.skill.saved_rank;
        external.row_mask = active.row_mask;
        external.element = active.element;
        external.category = current->facts.main_damage_class;
        external.formula_sheet = &active.formula_sheet;
        external.class_projection = [&](const std::string& token, dh2::data::PropertySheet& output, std::string& projection_error) {
            double ignored_cooldown = 0.0, ignored_range = 0.0, ignored_growth = 0.0;
            return source_skill_class_projection(*active.characters, *active.classes, *active.property_rules,
                current->sheets.resolved, token, active.receipt.mana.skill.saved_rank, output,
                ignored_cooldown, ignored_range, ignored_growth, projection_error);
        };
        external.characters = active.characters;
        external.classes = active.classes;
        external.rules = active.property_rules;
        external.ordered_character_targets = &use_targets.ordered_character_targets;
        external.population_order = &active.population_order;
        external.target_facts = &active.target_facts;
        external.non_character_attackable_objects_absent = active.non_character_attackable_objects_absent;
        active.receipt.applied_results.clear();
        active.receipt.target_hit_fx_name.clear();
        active.receipt.target_hit_fx_targets.clear();
        if (!active.external_owner->use(session, external, error)) {
            active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
            active.receipt.detail = "Authored skill OnSkill_ external owner failed after the committed OnPre prefix";
            output = active.receipt;
            return false;
        }
        active.use_delivered = true;
        active.receipt.phase = RuntimeSkillCastPhaseV1::use_applied;
        active.receipt.detail = "Authored source skill Use ran its external owner (skill projectile / timer arms)";
        output = active.receipt;
        error.clear();
        return true;
    }

    // A repeated marker volley appends to the cast's result list; the first Use starts it.
    if (!active.repeat_use) {
        active.receipt.applied_results.clear();
        active.receipt.target_hit_fx_name.clear();
        active.receipt.target_hit_fx_targets.clear();
    }
    // v21.8 ES-API presentation, in authored order: PlaySound at Use, PlayFX on the caster, then the per-target PlayFX
    // that the Lua runs for every popped Character (not gated by damage; rolls_combat does not gate it).
    if (profile.use_sound_name)
        active.receipt.presentation_cues.push_back({SkillPresentationCueV1::Kind::sound, profile.use_sound_name, invalid_actor_id});
    if (profile.self_fx_name)
        active.receipt.presentation_cues.push_back({SkillPresentationCueV1::Kind::self_fx, profile.self_fx_name, actor_id});
    if (profile.target_fx_each_name) {
        for (const auto victim : use_targets.ordered_character_targets) {
            if (profile.target_fx_each_non_player_only) {
                const auto* traits = session.world()->traits(victim);
                if (!traits || traits->is_player) continue;
            }
            active.receipt.presentation_cues.push_back({SkillPresentationCueV1::Kind::target_fx, profile.target_fx_each_name, victim});
        }
    }
    // v21.8 berserker: prince_berseeker_warcry.luac OnSkill_ over the OnPre list (PopTargetList, no re-search). Per Character:
    // GetRand(1, faces) == stun_face gives SetStunState(duration, true), any other face SetScareState(duration, true). The draw
    // also happens for bosses (the Lua draws only for non-bosses; the boss refusal is inside the status owner), so the RNG
    // sequence differs from the original, not the gameplay outcome. Sounds follow the loop, fear then stun.
    if (profile.status_arm) {
        const auto& arm = *profile.status_arm;
        double duration_ms = original_signed256(active.formula_sheet[std::size_t(176)]); // SnS_Duration (ms)
        if (!(duration_ms > 0.0) || !std::isfinite(duration_ms)) duration_ms = double(arm.fallback_ms);
        bool scared_any = false, stunned_any = false;
        for (const auto victim : use_targets.ordered_character_targets) {
            std::uint32_t draw = 0;
            std::string rng_error;
            if (!session.world()->random_uniform(arm.faces, draw, rng_error)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Source Warcry GetRand failed after committed OnPre: " + rng_error;
                output = active.receipt;
                return fail(error, active.receipt.detail.c_str());
            }
            const bool stun = draw + 1u == arm.stun_face;
            bool applied = false;
            std::string status_error;
            if (!session.script_status_actor(victim, stun, duration_ms, applied, status_error)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Source Warcry SetStunState/SetScareState failed after committed OnPre: " + status_error;
                output = active.receipt;
                return fail(error, active.receipt.detail.c_str());
            }
            if (applied) (stun ? stunned_any : scared_any) = true;
        }
        if (scared_any && arm.scare_sound)
            active.receipt.presentation_cues.push_back({SkillPresentationCueV1::Kind::sound, arm.scare_sound, invalid_actor_id});
        if (stunned_any && arm.stun_sound)
            active.receipt.presentation_cues.push_back({SkillPresentationCueV1::Kind::sound, arm.stun_sound, invalid_actor_id});
    }

    std::uint32_t event_index = active.next_event_index;
    // Legacy Character-only callers retain their exact target order.
    const auto& ordered = active.object_targets ? use_targets.ordered_targets : use_targets.ordered_character_targets;
    for (const auto target : ordered) {
        // v21.8 skills-knight: rows with rolls_combat=false (auras, Warcry) run their Use without any SkillCombatRoll.
        if (!profile.rolls_combat) break;
        if (session.world()->find_object(target)) {
            // Authored loops that guard SkillCombatRoll with IsCharacter() pop the object and skip it.
            if (!profile.rolls_objects) continue;
            // _SkillCombatRoll3b9fbc: non-Character type8 -> vt152 Interact,
            // returns false; no Character result or combat RNG is fabricated.
            if (!active.object_targets || !active.object_targets->interact ||
                !active.object_targets->interact(session, target, actor_id, error)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Source object SkillCombatRoll Interact failed after committed OnPre";
                output = active.receipt;
                return false;
            }
            active.receipt.delivered_object_interactions.push_back(target);
            continue;
        }
        // Native _SkillCombatRoll: a dual-wield row (mask 0x800000) attacks with the
        // main hand when HasMainHandWeapon, then with the off hand (mask | 0x4000000)
        // when HasOffHandWeapon. Other rows make one F_SkillAttack with the row mask.
        std::vector<std::uint32_t> hand_masks;
        if (!active.paired_hand) hand_masks.push_back(active.row_mask);
        else {
            if (active.has_main_hand) hand_masks.push_back(active.row_mask);
            if (active.has_off_hand) hand_masks.push_back(active.row_mask | 0x04000000u);
        }
        for (const auto hand_mask : hand_masks) {
            CombatSessionSourceHit hit;
            hit.attacker = actor_id;
            hit.target = target;
            hit.binding_lease = session.actor_binding_lease();
            hit.generation = generation;
            hit.event_index = event_index++;
            hit.mask = hand_mask | 0x08000000u; // Native SkillCombatRoll SkillAttack bit.
            hit.source_id = active.receipt.skill_name;
            hit.marker_name = "do_skill";
            hit.category = current->facts.main_damage_class;
            hit.element = active.element;
            hit.direct_amount = 0;
            hit.attacker_formula_sheet = &active.formula_sheet;
            // v21.8 w2-necromancer: the pre-hit original state is the admission state of the SetKnockBackState request.
            const std::int32_t state_before_hit = session.original_actor_state(target);
            DamageEvent result;
            if (!session.apply_source_result(hit, result, error)) {
                active.receipt.phase = RuntimeSkillCastPhaseV1::partial_failure;
                active.receipt.detail = "Source SkillCombatRoll loop failed after its applied ordered hit prefix";
                output = active.receipt;
                return false;
            }
            // Lua: if(character) and (dmg > 0) then target:PlayFX(...). dmg is the SkillCombatRoll return (requested damage).
            if (profile.target_hit_fx_name && result.requested_damage > 0.0f) {
                active.receipt.target_hit_fx_name = profile.target_hit_fx_name;
                if (std::find(active.receipt.target_hit_fx_targets.begin(), active.receipt.target_hit_fx_targets.end(),
                              target) == active.receipt.target_hit_fx_targets.end())
                    active.receipt.target_hit_fx_targets.push_back(target);
            }
            // v21.8 w2-necromancer: DeathClaw `if(character) and (dmg > 0) then target:SetKnockBackState(true)`. The Lua
            // return value is not checked, so a sink failure is recorded as a diagnostic and the Use continues.
            if (profile.knockback_great_on_character_hit && active.knockback_great && *active.knockback_great &&
                result.requested_damage > 0.0f) {
                RuntimeSkillKnockBackRequestV1 knockback;
                knockback.attacker = actor_id;
                knockback.target = target;
                knockback.generation = generation;
                knockback.event_index = hit.event_index;
                knockback.source_id = active.receipt.skill_name;
                knockback.target_state_before_hit = state_before_hit;
                std::string knockback_error;
                if (!(*active.knockback_great)(session, knockback, knockback_error))
                    active.receipt.knockback_diagnostics.push_back(knockback_error);
                else
                    active.receipt.knockback_targets.push_back(target);
            }
            active.receipt.applied_results.push_back(std::move(result));
        }
    }
    active.next_event_index = event_index;
    if (volley) ++volley_cursor_[volley_key];  // the Lua skill_inc advances once the OnSkill_ volley has run
    ++active.use_volleys;
    active.use_delivered = true;
    active.receipt.phase = RuntimeSkillCastPhaseV1::use_applied;
    active.receipt.detail = !active.receipt.delivered_object_interactions.empty()
        ? "Authored source skill Use delivered typed object Interact and its ordered Character result subset"
        : active.receipt.applied_results.empty()
        ? "Authored source skill Use completed with an empty target list"
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
    auto& cast = it->second;
    const bool duplicate_marker = cast.last_use_key_valid &&
        cast.last_use_wall_ms == event.wall_timestamp_ms && cast.last_use_slot == event.slot &&
        cast.last_use_lag_ms == event.lag_ms;
    const auto previous_phase = cast.receipt.phase;
    bool volley = false;
    if (previous_phase == RuntimeSkillCastPhaseV1::use_applied ||
        previous_phase == RuntimeSkillCastPhaseV1::completed) {
        // A later distinct marker of a still-running generic skill sequence is another OnSkill_ call.
        // Faery casts, completed casts and exact duplicate callbacks stay single-shot.
        if (faery_cast || duplicate_marker || previous_phase != RuntimeSkillCastPhaseV1::use_applied ||
            !cast.use_profile || !cast.use_delivered)
            return apply_use_v1(session, actor_id, generation, output, error);
        volley = true;
    }
    const auto* actor = session.actor(actor_id);
    if (!actor || !actor->alive() || actor->action == CharacterAction::hurt ||
        actor->action == CharacterAction::dead)
        return fail(error, "Retained source Use event reached an unavailable or interrupted cast owner");
    cast.last_use_key_valid = true;
    cast.last_use_wall_ms = event.wall_timestamp_ms;
    cast.last_use_slot = event.slot;
    cast.last_use_lag_ms = event.lag_ms;
    if (volley) {
        cast.receipt.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
        cast.repeat_use = true;
    }
    const bool applied = apply_use_v1(session, actor_id, generation, output, error);
    if (volley && !applied && cast.receipt.phase == RuntimeSkillCastPhaseV1::prepared_pending_use)
        cast.receipt.phase = previous_phase;
    return applied;
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
