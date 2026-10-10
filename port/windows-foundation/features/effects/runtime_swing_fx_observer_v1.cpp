#include "runtime_swing_fx_observer_v1.hpp"

#include "../../../engine-audio/audio_animation_swoosh_v38.hpp"
#include "../../../level-world/character_animation_step_fx_v2.hpp"
#include "../../../level-world/canonical_point3d_globals_v1.hpp"
#include "../../playable_actor_world.hpp"

#include <algorithm>
#include <exception>

namespace dh::foundation::effects {
namespace {
bool same_lease(const std::shared_ptr<const void>& a,
                const std::shared_ptr<const void>& b) noexcept {
    return a && b && a.get() == b.get();
}
bool same_owner(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.owner_before(b) && !b.owner_before(a);
}

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

struct CallContext {
    CombatSession* session{};
    const dh2::data::ItemRecord164* main_item{};
    const dh2::data::ItemRecord164* off_item{};
    dh2::data::EffectsTables::Borrow* effects{};
    dh2::fx::CharacterMeshFxOwnerV4* manager{};
    const RuntimeSwingFxEnabledResolverV1* enabled{};
    ActorId actor = invalid_actor_id;
    std::string* error{};
    std::vector<std::int32_t>* played_sets{};
};

int equipped(void* raw, std::int32_t kind, std::uintptr_t& item) {
    const auto& c = *static_cast<CallContext*>(raw);
    const auto* result = kind == 1 ? c.main_item : kind == 2 ? c.off_item : nullptr;
    item = reinterpret_cast<std::uintptr_t>(result);
    return kind == 1 || kind == 2 ? 0 : -1;
}

int item_effects(void*, std::uintptr_t raw, std::int32_t& sound, std::int32_t& fx) {
    sound = fx = -1;
    const auto* item = reinterpret_cast<const dh2::data::ItemRecord164*>(raw);
    if (!item) return 0;
    // Source Item::SwooshSoundFX and Item::SwooshFX are ItemRecord words 5/6.
    sound = item->words[5];
    fx = item->words[6];
    return 0;
}

bool actor_position(CallContext& c, float output[3]) {
    const auto* state = c.session->actor(c.actor);
    if (!state) return fail(*c.error, "AnimTable FX actor left its CombatSession");
    std::array<float,3> selected{};
    if (!c.enabled || !runtime_swing_fx_target_position_v1(
            *state, c.actor, *c.enabled, selected, *c.error)) return false;
    std::copy(selected.begin(), selected.end(), output);
    return true;
}

int play_item_fx(void* raw, std::int32_t set, bool anchored) {
    auto& c = *static_cast<CallContext*>(raw);
    if (!c.effects || !*c.effects || set < 0 ||
        static_cast<std::size_t>(set) >= c.effects->sets().size())
        return fail(*c.error, "ItemRecord SwooshFX is outside the original EffectsTables set rows") ? 0 : -1;
    float position[3]{};
    const float* rotation = nullptr;
    std::uintptr_t anchor = static_cast<std::uintptr_t>(c.actor);
    if (!anchored) {
        if (!actor_position(c, position)) return -1;
        const auto* state = c.session->actor(c.actor);
        if (!state) return fail(*c.error, "AnimTable Swoosh actor left its CombatSession") ? 0 : -1;
        rotation = state->transform.rotation.data();
        anchor = 0;
    } else {
        const auto& origin = dh2::world::canonical_vec3_origin_v1();
        std::copy(origin.begin(), origin.end(), position);
    }
    if (!c.manager->play_set(set, position, rotation, anchor, nullptr, *c.error)) return -1;
    // The source manager accepted this exact ItemRecord SwooshFX set row.
    // Diagnostics are presentation-only and never feed back into gameplay.
    c.played_sets->push_back(set);
    return 0;
}

int play_sound_owned_by_audio_first(void*, std::int32_t) {
    // The composed RuntimeSessionAudioV1 observer has already run the exact
    // AnimTable/Swoosh sound path. Replaying it here would duplicate audio.
    return 0;
}

int step_owner(void* raw, std::uintptr_t& owner) {
    const auto& c = *static_cast<CallContext*>(raw);
    owner = static_cast<std::uintptr_t>(c.actor);
    return owner ? 0 : -1;
}

int step_swoosh_gate(void* raw, const dh2::data::AnimationStep& step, bool& gate) {
    auto& c = *static_cast<CallContext*>(raw);
    dh2::character::AnimationSwooshServicesV4 services{};
    services.context = &c;
    services.equipped = equipped;
    services.effects = item_effects;
    services.play_sound = play_sound_owned_by_audio_first;
    services.play_fx = play_item_fx;
    bool fallback_sound = false;
    std::string error;
    if (dh2::audio::audio_animation_swoosh_v38(step.anchor_fx, services,
            fallback_sound, gate, error) != 0) {
        if (c.error) *c.error = std::move(error);
        return -1;
    }
    return 0;
}

int step_position(void* raw, std::uintptr_t owner, float output[3]) {
    auto& c = *static_cast<CallContext*>(raw);
    return owner == static_cast<std::uintptr_t>(c.actor) && actor_position(c, output) ? 0 : -1;
}

int step_rotation(void* raw, std::uintptr_t owner, float output[3]) {
    auto& c = *static_cast<CallContext*>(raw);
    if (owner != static_cast<std::uintptr_t>(c.actor))
        return fail(*c.error, "AnimTable FX rotation owner differs from same-session ActorId") ? 0 : -1;
    const auto* state = c.session->actor(c.actor);
    if (!state) return fail(*c.error, "AnimTable FX actor left its CombatSession") ? 0 : -1;
    std::copy(state->transform.rotation.begin(), state->transform.rotation.end(), output);
    c.error->clear();
    return 0;
}

int step_play(void* raw, std::int32_t set, const float* position,
              const float* rotation, std::uintptr_t anchor) {
    auto& c = *static_cast<CallContext*>(raw);
    if (!c.effects || !*c.effects || set < 0 ||
        static_cast<std::size_t>(set) >= c.effects->sets().size())
        return fail(*c.error, "AnimTable step FX is outside the original EffectsTables set rows") ? 0 : -1;
    if (!c.manager->play_set(set, position, rotation, anchor, nullptr, *c.error)) return -1;
    c.played_sets->push_back(set);
    return 0;
}
}

RuntimeSwingFxObserverV1::RuntimeSwingFxObserverV1(CombatSession& session,
    const dh2::data::AnimationTables& animations, const dh2::data::ItemTable& items,
    dh2::data::EffectsTables::Borrow effects,
    dh2::fx::CharacterMeshFxOwnerV4& manager,
    std::function<void(const RuntimeSwingFxDiagnosticV1&)> diagnostic,
    RuntimeSwingFxEnabledResolverV1 enabled)
    : session_(session), animations_(animations), items_(items),
      effects_(std::move(effects)), manager_(manager), diagnostic_(std::move(diagnostic)),
      enabled_(std::move(enabled)) {}

CombatSessionStepObserver RuntimeSwingFxObserverV1::step_entry_observer() {
    return [this](const CombatSessionStepEntry& entry) { dispatch(entry); };
}

void RuntimeSwingFxObserverV1::report(const RuntimeSwingFxDiagnosticV1& item) const noexcept {
    if (diagnostic_) try { diagnostic_(item); } catch (...) {}
}

void RuntimeSwingFxObserverV1::dispatch(const CombatSessionStepEntry& entry) {
    RuntimeSwingFxDiagnosticV1 d;
    d.actor = entry.actor; d.occurrence = entry.occurrence;
    d.update_serial = entry.update_serial; d.sequence_id = entry.sequence_id; d.step = entry.step;
    auto finish = [&](bool dispatched, std::string detail = {}) {
        d.dispatched = dispatched; d.detail = std::move(detail); report(d);
    };
    try {
        auto event_lease = entry.binding_lease.lock();
        auto active_lease = session_.actor_binding_lease().lock();
        if (!same_lease(event_lease, active_lease) || !entry.actor ||
            !entry.occurrence || !entry.update_serial) {
            finish(false, "Required current same-session actor binding lease and step occurrence"); return;
        }
        if (!same_owner(entry.binding_lease, active_lease_)) {
            delivered_.clear();
            active_lease_ = entry.binding_lease;
        }
        const OccurrenceKey key{reinterpret_cast<std::uintptr_t>(event_lease.get()),
                                entry.actor, entry.occurrence};
        if (!delivered_.insert(key).second) {
            finish(false, "Duplicate exact actor/step occurrence suppressed"); return;
        }
        if (entry.sequence_id < 0 ||
            static_cast<std::uint64_t>(entry.sequence_id) >= animations_.sequences.size()) {
            finish(false, "Retained sequence ID is outside original AnimationTables"); return;
        }
        const auto& sequence = animations_.sequences[static_cast<std::size_t>(entry.sequence_id)];
        if (entry.step >= sequence.steps.size()) {
            finish(false, "Retained step is outside its original AnimTable sequence"); return;
        }
        const auto& step = sequence.steps[entry.step];
        if (!effects_ || !session_.actor(entry.actor) || !session_.world() ||
            !session_.world()->traits(entry.actor)) {
            finish(false, "Required original EffectsTables and same-session actor traits"); return;
        }
        const auto* state = session_.actor(entry.actor);
        const auto* traits = session_.world()->traits(entry.actor);
        const dh2::data::ItemRecord164* off_item = nullptr;
        for (const auto& equipped_ref : state->equipment) {
            if (equipped_ref.slot != "off_hand") continue;
            if (off_item) {
                finish(false, "Ambiguous same-session off-hand equipment binding"); return;
            }
            const auto row = dh2::data::item_id(items_, equipped_ref.definition_id);
            const auto* item = dh2::data::item(items_, row);
            if (!item) {
                finish(false, "Same-session off-hand ItemTable row is unavailable"); return;
            }
            off_item = &item->record;
        }
        CallContext context{&session_, traits->main_item ? &*traits->main_item : nullptr,
            off_item, &effects_, &manager_, &enabled_, entry.actor, nullptr, &d.source_sets};
        std::string error;
        context.error = &error;
        dh2::character::AnimationStepFxServicesV2 services{};
        services.context = &context;
        services.owner = step_owner;
        services.swoosh_fx_gate = step_swoosh_gate;
        services.target_position = step_position;
        services.rotation = step_rotation;
        services.play = step_play;
        if (dh2::character::character_animation_step_fx_v2(step, services, error) != 0) {
            finish(false, std::move(error)); return;
        }
        finish(true);
    } catch (const std::exception& e) {
        finish(false, e.what());
    } catch (...) {
        finish(false, "Unknown exception in original AnimTable step FX observer");
    }
}

} // namespace dh::foundation::effects
