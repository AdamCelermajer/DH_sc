#include "runtime_timed_buff_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

// Property type word as the native resolver reads it: -1 is the runtime-only kind 16.
unsigned property_type(const dh2::data::PropertyRules& rules, unsigned p) {
    return rules.types[p] == -1 ? 16u : static_cast<std::uint32_t>(rules.types[p]);
}

bool same_owner(const std::weak_ptr<const void>& a, const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}
}

bool resolve_timed_buffs_v1(const dh2::data::PropertyRules& rules, dh2::data::PropertyState& state,
                            const std::vector<const TimedBuffInstanceV1*>& buffs,
                            const std::set<unsigned>& previous_touched, std::set<unsigned>& touched,
                            bool& changed, std::string& error) {
    touched.clear();
    changed = false;
    // Native BuffOwner::refresh: one group per class id (ascending), the group keeps its instances in order.
    std::vector<std::vector<const std::int32_t*>> group_sheets;
    std::vector<dh2::data::PropertyBuffGroup> groups;
    std::int32_t group_class = -2;
    for (const auto* buff : buffs) {
        if (!buff) return fail(error, "Timed buff list holds a null instance");
        if (buff->class_id != group_class) {
            if (!group_sheets.empty() && buff->class_id < group_class)
                return fail(error, "Timed buffs must be ordered by ascending class id");
            group_class = buff->class_id;
            group_sheets.emplace_back();
        }
        group_sheets.back().push_back(buff->sheet.data());
    }
    for (const auto& sheets : group_sheets)
        groups.push_back({sheets.data(), static_cast<std::uint32_t>(sheets.size())});
    // A cell is buff-visible when a buff sheet moves it off its default and its type is a base-resolved kind.
    for (const auto* buff : buffs)
        for (unsigned p = 0; p < buff->sheet.size(); ++p)
            if ((property_type(rules, p) & 7) && buff->sheet[p] != rules.defaults[p]) touched.insert(p);
    std::set<unsigned> cells = touched;
    cells.insert(previous_touched.begin(), previous_touched.end());
    auto next = state;
    auto view = dh2::data::property_view(rules, next);
    view.groups = groups.empty() ? nullptr : groups.data();
    view.group_count = static_cast<std::uint32_t>(groups.size());
    if (dh2_property_validate(&view)) return fail(error, "Timed buff property view is invalid");
    for (const unsigned p : cells) {
        if (!(property_type(rules, p) & 7)) continue;
        std::int32_t value = 0;
        if (dh2_property_resolve(&view, static_cast<std::int32_t>(p), &value))
            return fail(error, "Timed buff property resolution failed");
        if (next.resolved[p] != state.resolved[p]) changed = true;
    }
    if (changed) state = next;
    error.clear();
    return true;
}

std::vector<const TimedBuffInstanceV1*> RuntimeTimedBuffOwnerV1::ordered(const ActorBuffs& buffs) const {
    std::vector<const TimedBuffInstanceV1*> result;
    for (const auto& instance : buffs.instances) result.push_back(instance.get());
    std::stable_sort(result.begin(), result.end(), [](const auto* a, const auto* b) { return a->class_id < b->class_id; });
    return result;
}

bool RuntimeTimedBuffOwnerV1::bind_lease(CombatSession& session) {
    const auto lease = session.actor_binding_lease();
    if (lease.expired()) return false;
    if (lease_bound_ && (lease_.expired() || !same_owner(lease_, lease))) {
        // A replaced CombatSession owns different actors and a different FX manager: nothing to publish or drop.
        actors_.clear();
        dirty_.clear();
    }
    lease_ = lease;
    lease_bound_ = true;
    return true;
}

void RuntimeTimedBuffOwnerV1::release_fx(TimedBuffInstanceV1& instance) {
    if (!instance.fx_identity) return;
    std::string ignored;
    if (bindings_.fx.drop) bindings_.fx.drop(instance.fx_identity, ignored);
    instance.fx_identity = 0;
}

bool RuntimeTimedBuffOwnerV1::publish(CombatSession& session, ActorId actor, ActorBuffs& buffs,
                                      TimedBuffEventV1::Kind kind, const std::string& class_name,
                                      const std::string& source, std::uint32_t strength, double duration_ms,
                                      bool& changed, std::string& error) {
    changed = false;
    auto* world = session.world();
    const auto* live = world ? world->combat_properties(actor) : nullptr;
    const auto* traits = world ? world->traits(actor) : nullptr;
    if (!live || !traits) return fail(error, "Timed buff publication requires the live actor sheet and traits");
    auto next = *live;
    std::set<unsigned> touched;
    if (!resolve_timed_buffs_v1(*bindings_.rules, next.sheets, ordered(buffs), buffs.touched, touched, changed, error))
        return false;
    TimedBuffEventV1 event;
    event.kind = kind;
    event.actor = actor;
    event.class_name = class_name;
    event.source = source;
    event.strength = strength;
    event.duration_ms = duration_ms;
    if (changed) {
        for (const unsigned p : touched) if (next.sheets.resolved[p] != live->sheets.resolved[p])
            event.changed_cells.push_back({p, {live->sheets.resolved[p], next.sheets.resolved[p]}});
        for (const unsigned p : buffs.touched) if (!touched.count(p) && next.sheets.resolved[p] != live->sheets.resolved[p])
            event.changed_cells.push_back({p, {live->sheets.resolved[p], next.sheets.resolved[p]}});
        if (!world->update_combat_properties(actor, std::move(next), *traits, error)) return false;
        dirty_.insert(actor);
    }
    buffs.touched = touched;
    if (changed || kind != TimedBuffEventV1::Kind::reasserted) events_.push_back(std::move(event));
    error.clear();
    return true;
}

bool RuntimeTimedBuffOwnerV1::remove_timed_buff(CombatSession& session, ActorId actor,
                                                const std::string& class_name, std::string& error) {
    error.clear();
    if (!bindings_.classes || !bindings_.rules) return fail(error, "Timed buff owner has no class/property tables");
    if (!bind_lease(session)) return fail(error, "Timed buff removal requires a live CombatSession binding");
    const auto found = actors_.find(actor);
    if (found == actors_.end()) return true;
    auto& buffs = found->second;
    bool removed = false;
    for (auto it = buffs.instances.begin(); it != buffs.instances.end();) {
        if ((*it)->class_name == class_name) {
            release_fx(**it);
            it = buffs.instances.erase(it);
            removed = true;
        } else ++it;
    }
    if (!removed) return true;
    bool changed = false;
    if (!publish(session, actor, buffs, TimedBuffEventV1::Kind::removed, class_name, std::string(), 0, 0.0, changed, error))
        return false;
    if (buffs.instances.empty()) actors_.erase(found);
    return true;
}

bool RuntimeTimedBuffOwnerV1::apply_timed_buff(CombatSession& session, ActorId actor, const TimedBuffSpecV1& spec,
                                               std::string& error) {
    error.clear();
    if (!bindings_.classes || !bindings_.rules) return fail(error, "Timed buff owner has no class/property tables");
    if (!bind_lease(session)) return fail(error, "Timed buff application requires a live CombatSession binding");
    if (!(spec.duration_ms > 0.0) || !std::isfinite(spec.duration_ms))
        return fail(error, "Timed buff duration must be finite and positive");
    const auto* state = session.actor(actor);
    const auto* live = session.world() ? session.world()->combat_properties(actor) : nullptr;
    if (!state || !live) return fail(error, "Timed buff target is not an actor with a property sheet in this CombatSession");
    if (!state->alive()) return fail(error, "Timed buff target is dead");
    const auto& names = bindings_.classes->names;
    const auto class_it = std::find(names.begin(), names.end(), spec.class_name);
    if (class_it == names.end() || bindings_.classes->rows.size() != names.size())
        return fail(error, "Timed buff class is absent from the authored ClassTables: " + spec.class_name);
    constexpr unsigned kSnsLevel = 172; // CharacterProperties.SnS_Level (type 8)

    auto instance = std::make_unique<TimedBuffInstanceV1>();
    instance->class_id = static_cast<std::int32_t>(class_it - names.begin());
    instance->class_name = spec.class_name;
    instance->fx_set_name = spec.fx_set_name;
    instance->source = spec.source;
    instance->strength = spec.strength;
    instance->total_ms = instance->remaining_ms = spec.duration_ms;
    // CreateBuff: free sheet = defaults with the strength in SnS_Level; ApplyBuff: the class reads the owner's
    // resolved sheet, where the Lua SetProp(SnS_Level) has just put the same value.
    instance->sheet = bindings_.rules->defaults;
    const std::int32_t level_raw = static_cast<std::int32_t>(spec.strength << 8);
    instance->sheet[kSnsLevel] = level_raw;
    auto owner_view = live->sheets.resolved;
    owner_view[kSnsLevel] = level_raw;
    if (!dh2::data::apply_class(*bindings_.classes, instance->class_id, instance->sheet, error, &owner_view)) {
        if (error.empty()) error = "Timed buff class evaluation failed: " + spec.class_name;
        return false;
    }

    auto& buffs = actors_[actor];
    const bool replaced = std::any_of(buffs.instances.begin(), buffs.instances.end(),
        [&](const auto& item) { return item->class_name == spec.class_name; });
    for (auto it = buffs.instances.begin(); it != buffs.instances.end();) {
        if ((*it)->class_name == spec.class_name) {
            release_fx(**it);
            it = buffs.instances.erase(it);
        } else ++it;
    }
    std::string fx_error;
    if (!spec.fx_set_name.empty() && bindings_.fx.play) {
        std::uintptr_t identity = 0;
        if (bindings_.fx.play(spec.fx_set_name, actor, identity, fx_error)) instance->fx_identity = identity;
    }
    const auto fx_note = fx_error.empty() ? std::string() : "fx=" + fx_error;
    buffs.instances.push_back(std::move(instance));
    bool changed = false;
    if (!publish(session, actor, buffs, replaced ? TimedBuffEventV1::Kind::replaced : TimedBuffEventV1::Kind::applied,
                 spec.class_name, spec.source, spec.strength, spec.duration_ms, changed, error)) {
        release_fx(*buffs.instances.back());
        buffs.instances.pop_back();
        if (buffs.instances.empty()) actors_.erase(actor);
        return false;
    }
    if (!fx_note.empty() && !events_.empty()) events_.back().detail = fx_note;
    return true;
}

bool RuntimeTimedBuffOwnerV1::advance(CombatSession& session, double dt_seconds, std::string& error) {
    error.clear();
    if (!std::isfinite(dt_seconds) || dt_seconds < 0.0) return fail(error, "Timed buff clock needs a finite nonnegative dt");
    if (!bindings_.classes || !bindings_.rules) return true;
    if (!bind_lease(session)) return true;
    const double dt_ms = dt_seconds * 1000.0;
    for (auto it = actors_.begin(); it != actors_.end();) {
        const ActorId actor = it->first;
        auto& buffs = it->second;
        const auto* state = session.actor(actor);
        if (!state || !state->alive() || !session.world() || !session.world()->combat_properties(actor)) {
            // The actor is gone or dead: its owner released the buffs; there is no sheet left to publish.
            for (auto& instance : buffs.instances) {
                release_fx(*instance);
                TimedBuffEventV1 event;
                event.kind = TimedBuffEventV1::Kind::dropped;
                event.actor = actor;
                event.class_name = instance->class_name;
                event.source = instance->source;
                events_.push_back(std::move(event));
            }
            it = actors_.erase(it);
            continue;
        }
        std::vector<std::unique_ptr<TimedBuffInstanceV1>> expired;
        for (auto inst = buffs.instances.begin(); inst != buffs.instances.end();) {
            (*inst)->remaining_ms -= dt_ms;
            if ((*inst)->remaining_ms <= 0.0) {
                expired.push_back(std::move(*inst));
                inst = buffs.instances.erase(inst);
            } else ++inst;
        }
        for (auto& dead : expired) release_fx(*dead);
        bool changed = false;
        if (!expired.empty()) {
            if (!publish(session, actor, buffs, TimedBuffEventV1::Kind::expired, expired.back()->class_name,
                         expired.back()->source, expired.back()->strength, expired.back()->total_ms, changed, error))
                return false;
        } else if (!publish(session, actor, buffs, TimedBuffEventV1::Kind::reasserted, std::string(), std::string(),
                            0, 0.0, changed, error)) {
            return false;
        }
        if (buffs.instances.empty()) it = actors_.erase(it);
        else ++it;
    }
    return true;
}

bool RuntimeTimedBuffOwnerV1::take_sheet_dirty(ActorId actor) {
    return dirty_.erase(actor) != 0;
}

std::vector<TimedBuffEventV1> RuntimeTimedBuffOwnerV1::take_events() {
    std::vector<TimedBuffEventV1> result;
    result.swap(events_);
    return result;
}

std::size_t RuntimeTimedBuffOwnerV1::active_count(ActorId actor) const {
    const auto found = actors_.find(actor);
    return found == actors_.end() ? 0 : found->second.instances.size();
}

bool RuntimeTimedBuffOwnerV1::active(ActorId actor, const std::string& class_name) const {
    const auto found = actors_.find(actor);
    return found != actors_.end() && std::any_of(found->second.instances.begin(), found->second.instances.end(),
        [&](const auto& item) { return item->class_name == class_name; });
}

double RuntimeTimedBuffOwnerV1::remaining_ms(ActorId actor, const std::string& class_name) const {
    const auto found = actors_.find(actor);
    if (found == actors_.end()) return 0.0;
    for (const auto& item : found->second.instances) if (item->class_name == class_name) return item->remaining_ms;
    return 0.0;
}

} // namespace dh::foundation::generic_skills
