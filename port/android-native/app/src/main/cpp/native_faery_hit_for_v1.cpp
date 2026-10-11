#include "native_faery_hit_for_v1.hpp"

#include <stdexcept>

namespace dh2::native::faery_hit_for_v1 {
namespace {

character_kill_npc_provider_v1::Bindings kill_bindings(
    Runtime* runtime, const Bindings& bindings) {
    const auto* services = bindings.services;
    character_kill_npc_provider_v1::Services kill_services{};
    kill_services.context = runtime;
    kill_services.preflight = Runtime::kill_preflight;
    kill_services.player_branch = Runtime::kill_player_branch;
    kill_services.raise_event2 = Runtime::kill_event2;
    if (services) kill_services.general_tail = services->kill.general_tail;
    return {
        bindings.record ? bindings.record->character.identity : 0,
        nullptr,
        bindings.properties,
        services ? services->classification_state : nullptr,
        services ? services->classification_services : nullptr,
        kill_services,
        bindings.record ? &bindings.record->serialized_dead_1449 : nullptr,
        bindings.record ? &bindings.record->kill_source_killer_144c : nullptr,
    };
}

bool canonical_record(const Bindings& bindings) noexcept {
    const auto* record = bindings.record;
    if (!record || !bindings.properties || !bindings.services) return false;
    const auto identity = record->character.identity;
    return identity && record->constructor.identity == identity &&
           record->game_object.identity == identity && record->object_registered &&
           record->character_listed && record->source_properties_ready() &&
           record->serialized_constructor_fields_ready &&
           record->hit_for_constructor_fields_ready &&
           bindings.services->classification_state &&
           bindings.services->classification_state->character == identity &&
           bindings.properties->resolved &&
           dh2_property_validate(bindings.properties) == 0 &&
           record->source_remote_culling_ready &&
           record->source_remote_culling.identity == identity;
}

} // namespace

Runtime::Runtime(Bindings bindings)
    : bindings_(bindings), kill_(kill_bindings(this, bindings)) {
    if (!canonical_record(bindings_))
        throw std::invalid_argument("Faery HitFor requires its canonical factory Record and live source owners");
    if (!bindings_.services->capture_facts || !bindings_.services->after_health ||
        !bindings_.services->kill.preflight ||
        !bindings_.services->kill.player_branch ||
        !bindings_.services->kill.raise_event2 ||
        !bindings_.services->kill.general_tail.preflight ||
        !bindings_.services->kill.general_tail.drop_loot ||
        !bindings_.services->kill.general_tail.killer_credit ||
        !bindings_.services->kill.general_tail.resolve_xp_killer ||
        !bindings_.services->kill.general_tail.distribute_xp ||
        !bindings_.services->kill.general_tail.virtual_54 ||
        !bindings_.services->kill.general_tail.read_character_14e4 ||
        !bindings_.services->kill.general_tail.objective_tail)
        throw std::invalid_argument("Faery HitFor or Character Kill services are incomplete");
}

bool Runtime::kill_preflight(void* raw, std::uintptr_t character,
                            std::uintptr_t killer, std::uint32_t force,
                            std::string& error) {
    auto* self = static_cast<Runtime*>(raw);
    if (!self || !canonical_record(self->bindings_) ||
        character != self->bindings_.record->character.identity || force != 0) {
        error = "Faery Character::Ctrl_Kill preflight crossed its canonical Record";
        return false;
    }
    const auto& services = self->bindings_.services->kill;
    if (!services.preflight(services.context, character, killer, force, error)) {
        if (error.empty()) error = "Faery Character::Ctrl_Kill source preflight failed";
        return false;
    }
    error.clear();
    return true;
}

bool Runtime::kill_player_branch(void* raw, std::uintptr_t character,
                               std::uintptr_t killer, std::uint32_t force,
                               std::string& error) {
    auto* self = static_cast<Runtime*>(raw);
    if (!self || !canonical_record(self->bindings_) ||
        character != self->bindings_.record->character.identity) {
        error = "Faery Character::Kill player branch crossed its canonical Record";
        return false;
    }
    const auto& services = self->bindings_.services->kill;
    if (!services.player_branch(services.context, character, killer, force, error)) {
        if (error.empty()) error = "Faery Character::Kill player continuation failed";
        return false;
    }
    error.clear();
    return true;
}

bool Runtime::kill_event2(void* raw, std::uintptr_t character,
                         std::uintptr_t killer, std::string& error) {
    auto* self = static_cast<Runtime*>(raw);
    if (!self || !canonical_record(self->bindings_) ||
        character != self->bindings_.record->character.identity) {
        error = "Faery Character::RaiseEvent(2) crossed its canonical Record";
        return false;
    }
    const auto& services = self->bindings_.services->kill;
    if (!services.raise_event2(services.context, character, killer, error)) {
        if (error.empty()) error = "Faery Character::RaiseEvent(2,killer) failed";
        return false;
    }
    error.clear();
    return true;
}

int Runtime::hit_for(void* context,
                     const character_apply_result::Request* request,
                     character_apply_result::Reply* reply,
                     std::string& error) {
    if (busy_) {
        error = "Faery Character::HitFor reentered";
        return 1;
    }
    if (context != this || !request || !reply ||
        request->operation != character_apply_result::Operation::hit_for ||
        !canonical_record(bindings_) ||
        request->subject != bindings_.record->character.identity ||
        request->defender != bindings_.record->character.identity ||
        request->peer != request->attacker || !request->attacker) {
        error = "Faery Character::HitFor request crossed its bound Record or operation";
        return 1;
    }

    busy_ = true;
    struct Release { bool& value; ~Release() { value = false; } } release{busy_};
    auto& record = *bindings_.record;
    auto& properties = *bindings_.properties;
    snapshot_ = {};
    error.clear();

    // HitFor starts with Character::IsDead and returns immediately when the
    // canonical byte is set. Do not capture application/session facts, query
    // ObjectBase culling, or reach the post-health callbacks on that path.
    if (record.serialized_dead_1449) {
        bindings_.services->classification_state->dead =
            record.serialized_dead_1449;
        snapshot_.health.before = properties.resolved[36];
        snapshot_.health.after = properties.resolved[36];
        snapshot_.health.low_health_armed = record.hit_for_low_health_armed_1448;
        snapshot_.health.lifecycle_write = -1;
        snapshot_.health.skipped_dead = 1;
        *reply = {};
        return 0;
    }

    Facts facts{};
    if (!bindings_.services->capture_facts(bindings_.services->context,
            record, request->attacker, facts, error)) {
        if (error.empty()) error = "Faery Character::HitFor source facts are unavailable";
        return 1;
    }
    snapshot_.facts_captured = 1;
    facts.flags &= ~(data::health_dead | data::health_remote);
    if (record.serialized_dead_1449) facts.flags |= data::health_dead;
    bindings_.services->classification_state->dead =
        record.serialized_dead_1449;

    object_update_culling::RemoteResult remote{};
    if (object_update_culling::is_remotely_updated(
            &record.source_remote_culling, &remote) !=
        object_update_culling::Status::complete) {
        error = "Faery Character::HitFor lost its canonical ObjectBase remote fields";
        return 1;
    }
    snapshot_.remote_query_completed = 1;
    if (remote.raw) facts.flags |= data::health_remote;

    const data::HealthRequest health_request{
        &properties, request->word0, facts.flags, facts.session_state,
        record.hit_for_low_health_armed_1448};
    if (dh2_health_hit(&snapshot_.health, &health_request)) {
        error = "Faery Character::HitFor health kernel rejected canonical property state";
        return 1;
    }
    record.hit_for_low_health_armed_1448 =
        static_cast<std::uint8_t>(snapshot_.health.low_health_armed);

    if (snapshot_.health.kill_requested && !record.serialized_dead_1449) {
        snapshot_.kill_attempted = 1;
        auto& classification = *bindings_.services->classification_state;
        classification.dead = record.serialized_dead_1449;
        player_kill_continuation_v1::Result result{};
        const auto status = kill_.caller().kill(request->attacker, 0, &result, error);
        snapshot_.kill = kill_.snapshot();
        snapshot_.kill.ctrl_result = result;
        if (status != player_kill_continuation_v1::Status::complete) {
            if (error.empty()) error = "Faery Character::Ctrl_Kill -> Character::Kill failed";
            return 1;
        }
        snapshot_.kill_completed = 1;
        if (record.serialized_dead_1449 != 1 || properties.resolved[36] != 0) {
            error = "Faery Character Kill returned without its canonical dead byte and zero HP";
            return 1;
        }
        classification.dead = record.serialized_dead_1449;
    }

    if (snapshot_.health.lifecycle_write != -1) {
        snapshot_.lifecycle_write_attempted = 1;
        record.hit_for_lifecycle_11c = snapshot_.health.lifecycle_write;
        snapshot_.lifecycle_write_completed = 1;
    }

    snapshot_.tail_attempted = 1;
    if (!bindings_.services->after_health(bindings_.services->context,
            record, request->attacker, request->word0, snapshot_.health, error)) {
        if (error.empty()) error = "Faery Character::HitFor post-health source tail failed";
        return 1;
    }
    snapshot_.tail_completed = 1;
    *reply = {};
    error.clear();
    return 0;
}

int hit_for(void* raw, const character_apply_result::Request* request,
            character_apply_result::Reply* reply, std::string& error) {
    if (!raw) {
        error = "Faery Character::HitFor has no retained per-Record Runtime";
        return 1;
    }
    return static_cast<Runtime*>(raw)->hit_for(raw, request, reply, error);
}

} // namespace dh2::native::faery_hit_for_v1
