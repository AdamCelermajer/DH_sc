#include "native_faery_combat_v1.hpp"

#include <cmath>
#include <cstring>

namespace dh2::native::faery_combat_v1 {
namespace {
const char* apply_status(character_apply_result::Status status) {
    switch (status) {
    case character_apply_result::Status::complete: return "complete";
    case character_apply_result::Status::invalid_argument: return "invalid argument";
    case character_apply_result::Status::service_unavailable: return "service unavailable";
    case character_apply_result::Status::service_failed: return "service failed";
    case character_apply_result::Status::unsupported_network: return "network path unsupported";
    case character_apply_result::Status::unsupported_player: return "Player path unsupported";
    case character_apply_result::Status::unsupported_gold: return "gold inventory path unsupported";
    case character_apply_result::Status::unsupported_coop_scaling: return "co-op scaling unsupported";
    case character_apply_result::Status::unsupported_threat_value: return "non-finite threat unsupported";
    case character_apply_result::Status::invalid_provider_result: return "provider returned invalid result";
    }
    return "unknown status";
}
}

Runtime::Runtime(Bindings bindings) : bindings_(bindings) {
    if (!bindings_.record || !bindings_.resolved || !bindings_.random ||
        !bindings_.debug_globals || !bindings_.debug_services)
        return;
    character_ = bindings_.record->character.identity;
    if (!character_) return;
    dot_actor_ = {character_, bindings_.resolved};
    dot_storage_ = {&dot_actor_, 1, &combat_context_, bindings_.random,
                    bindings_.debug_globals, bindings_.debug_services};
}

Status Runtime::attach(player_ai_timer_events_v1::Bindings& timer, std::string& error) {
    if (!bindings_.record || !character_ ||
        bindings_.record->character.identity != character_ ||
        !bindings_.record->source_remote_culling_ready ||
        bindings_.record->source_remote_culling.identity != character_ ||
        !bindings_.resolved || !bindings_.property_view ||
        bindings_.property_view->resolved != bindings_.resolved ||
        !bindings_.record->canonical_ai() || !bindings_.source_char_type ||
        bindings_.source_char_type != &bindings_.record->canonical_ai()->word_38 ||
        !bindings_.source_character_name ||
        !bindings_.online || !bindings_.saved_options || !bindings_.random ||
        !bindings_.debug_globals || !bindings_.debug_services ||
        !bindings_.apply_services.invoke ||
        dot_storage_.actors != &dot_actor_ || dot_storage_.actor_count != 1) {
        error = "Faery DoT combat bridge has no canonical same-Character backing";
        return Status::invalid_argument;
    }
    if (timer.dot_attack || timer.dot_storage || timer.apply.invoke) {
        error = "Faery DoT combat bridge cannot replace an existing timer provider";
        return Status::invalid_argument;
    }
    timer.dot_attack = &dot_runtime_;
    timer.dot_storage = &dot_storage_;
    timer.apply = {this, apply_result};
    attached_ = true;
    error.clear();
    return Status::complete;
}

std::int32_t Runtime::dot_attack(void* raw,
                                const character_dot_tick::AttackRequest* request) {
    auto& self = *static_cast<Runtime*>(raw);
    if (!request || !request->result || self.busy_ ||
        !self.attached_ || request->attacker != self.character_ ||
        request->defender != self.character_) return 1;
    const character_dot_attack::Arguments arguments{
        request->attacker, request->defender, request->amount, request->element};
    self.busy_ = true;
    struct Guard { bool& value; ~Guard() { value = false; } } guard{self.busy_};
    return self.dot_runtime_.attack(&self.dot_storage_, &arguments, request->result,
                                    &self.dot_result_) ==
        character_dot_attack::Status::complete ? 0 : 1;
}

std::int32_t Runtime::regen_service(void* raw,
    const character_regeneration::Request* request,
    character_regeneration::Reply* reply) {
    auto& self = *static_cast<Runtime*>(raw);
    if (!request || !reply || !self.bindings_.record ||
        self.bindings_.record->character.identity != self.character_ ||
        !self.bindings_.property_view ||
        self.bindings_.property_view->resolved != self.bindings_.resolved)
        return 1;
    *reply = {};
    if (request->operation == character_regeneration::Operation::read_property) {
        if (request->subject != reinterpret_cast<std::uintptr_t>(self.bindings_.property_view) ||
            request->sheet != reinterpret_cast<std::uintptr_t>(self.bindings_.resolved) ||
            request->property >= 224) return 1;
        reply->word = static_cast<std::uint32_t>(self.bindings_.resolved[request->property]);
        return 0;
    }
    if (request->operation == character_regeneration::Operation::add_property) {
        std::int32_t amount = 0;
        std::memcpy(&amount, &request->amount, sizeof(amount));
        if (request->subject != reinterpret_cast<std::uintptr_t>(self.bindings_.property_view) ||
            request->property >= 224 ||
            dh2_property_add(self.bindings_.property_view,
                             static_cast<std::int32_t>(request->property), amount))
            return 1;
        return 0;
    }

    if (!self.bindings_.apply_services.invoke) return 1;
    character_apply_result::Operation operation{};
    character_apply_result::Request forwarded{};
    switch (request->operation) {
    case character_regeneration::Operation::debug_load:
        operation = character_apply_result::Operation::debug_load;
        forwarded.subject = request->subject;
        break;
    case character_regeneration::Operation::string_construct:
        operation = character_apply_result::Operation::string_construct;
        forwarded.text = request->text;
        break;
    case character_regeneration::Operation::debug_query:
        operation = character_apply_result::Operation::debug_query;
        forwarded.subject = request->subject;
        forwarded.identity = request->sheet;
        break;
    case character_regeneration::Operation::string_destroy:
        operation = character_apply_result::Operation::string_destroy;
        forwarded.subject = request->subject;
        break;
    default:
        return 1;
    }
    forwarded.operation = operation;
    character_apply_result::Reply result{};
    if (self.bindings_.apply_services.invoke(self.bindings_.apply_services.context,
                                              &forwarded, &result)) return 1;
    reply->word = result.word;
    reply->identity = result.identity;
    return 0;
}

std::int32_t Runtime::apply_service(void* raw,
    const character_apply_result::Request* request,
    character_apply_result::Reply* reply) {
    auto& self = *static_cast<Runtime*>(raw);
    if (!request || !reply || !self.bindings_.record ||
        self.bindings_.record->character.identity != self.character_ ||
        !self.bindings_.property_view ||
        self.bindings_.property_view->resolved != self.bindings_.resolved)
        return 1;
    *reply = {};
    const auto& record = *self.bindings_.record;
    switch (request->operation) {
    case character_apply_result::Operation::online_mode:
        if (request->subject) return 1;
        reply->word = *self.bindings_.online;
        return 0;
    case character_apply_result::Operation::saved_option: {
        if (request->subject != self.bindings_.saved_options->identity ||
            !request->text) return 1;
        bool enabled = false;
        if (data::savegame_options_v1::is_saved_option_on(
                self.bindings_.saved_options, request->text, &enabled) !=
            data::savegame_options_v1::Status::complete) return 1;
        reply->word = enabled ? 1u : 0u;
        return 0;
    }
    case character_apply_result::Operation::coop_player_count: {
        if (request->subject || !self.bindings_.player_count) return 1;
        std::int32_t count = 0;
        if (self.bindings_.player_count(self.bindings_.player_count_context,
                                        &count)) return 1;
        std::memcpy(&reply->word, &count, sizeof(count));
        return 0;
    }
    case character_apply_result::Operation::is_player: {
        if (request->subject != self.character_ || !record.canonical_ai() ||
            self.bindings_.source_char_type != &record.canonical_ai()->word_38 ||
            !self.bindings_.source_character_name) return 1;
        const auto type = *self.bindings_.source_char_type;
        reply->word = type ? static_cast<std::uint32_t>(type == 1u)
                           : static_cast<std::uint32_t>(
                               std::strstr(self.bindings_.source_character_name,
                                           "PlayerCharacter") != nullptr);
        return 0;
    }
    case character_apply_result::Operation::is_dead:
        if (request->subject != self.character_ ||
            !record.serialized_constructor_fields_ready) return 1;
        reply->word = record.serialized_dead_1449;
        return 0;
    case character_apply_result::Operation::effective_threat: {
        if (request->subject != self.character_ || !self.bindings_.resolved)
            return 1;
        const float per_damage = static_cast<float>(self.bindings_.resolved[204]) *
                                 0.00390625f;
        std::memcpy(&reply->word, &per_damage, sizeof(reply->word));
        return std::isfinite(per_damage) ? 0 : 1;
    }
    case character_apply_result::Operation::regen_hp:
    case character_apply_result::Operation::regen_mp: {
        if (request->subject != self.character_ || !self.bindings_.debug_globals ||
            !self.bindings_.debug_services) return 1;
        const character_regeneration::State state{
            self.character_, reinterpret_cast<std::uintptr_t>(self.bindings_.property_view),
            reinterpret_cast<std::uintptr_t>(self.bindings_.resolved)};
        const auto* debug = self.bindings_.debug_globals->singleton;
        const character_regeneration::Globals globals{
            debug ? debug->identity() : 0};
        const character_regeneration::Services services{&self, regen_service};
        auto& result = request->operation == character_apply_result::Operation::regen_hp
            ? self.regen_hp_result_ : self.regen_mp_result_;
        std::uint32_t amount = 0;
        std::memcpy(&amount, &request->word0, sizeof(amount));
        const auto status = request->operation == character_apply_result::Operation::regen_hp
            ? character_regeneration::regen_hp(&state, &globals, amount, &services, &result)
            : character_regeneration::regen_mp(&state, &globals, amount, &services, &result);
        return status == character_regeneration::Status::complete ? 0 : 1;
    }
    case character_apply_result::Operation::hit_for:
        if (self.bindings_.hit_for) {
            if (!self.bindings_.hit_for_context) return 1;
            self.last_service_error_.clear();
            try {
                if (self.bindings_.hit_for(self.bindings_.hit_for_context,
                                           request, reply,
                                           self.last_service_error_)) return 1;
            } catch (...) {
                self.last_service_error_ = "HitFor provider threw an exception";
                return 1;
            }
            return 0;
        }
        break;
    default:
        break;
    }
    // Classification, PlayerManager, CharAI/AddAggro, HitFor, FSM skill
    // cancellation, scrolling text and audio require app-owned live providers.
    // Forward them unchanged; a missing provider fails at its source phase.
    return self.bindings_.apply_services.invoke
        ? self.bindings_.apply_services.invoke(self.bindings_.apply_services.context,
                                                request, reply)
        : 1;
}

int Runtime::apply_result(void* raw, const character_dot_tick::ApplyRequest* request,
                          std::string& error) {
    auto& self = *static_cast<Runtime*>(raw);
    if (!request || !request->result || self.busy_ || !self.attached_ ||
        request->attacker != self.character_ ||
        request->defender != self.character_ ||
        !self.bindings_.record ||
        self.bindings_.record->character.identity != self.character_ ||
        !self.bindings_.record->source_remote_culling_ready ||
        self.bindings_.record->source_remote_culling.identity != self.character_) {
        error = "Faery F_ApplyResult identity or reentry check failed";
        return 1;
    }
    auto& record = *self.bindings_.record;
    std::int32_t remote_word = 0;
    static_assert(sizeof(remote_word) == sizeof(record.source_remote_culling.remote_word_110));
    std::memcpy(&remote_word, &record.source_remote_culling.remote_word_110,
                sizeof(remote_word));
    self.actor_ = {self.character_, record.target_change_marker_14d0,
                   record.source_byte_14f0, record.source_push_death_53b,
                   remote_word};
    const character_apply_result::Arguments arguments{
        &self.actor_, &self.actor_, request->mode};
    self.last_service_error_.clear();
    self.busy_ = true;
    struct Guard { bool& value; ~Guard() { value = false; } } guard{self.busy_};
    struct WriteBack {
        character_runtime_factory_v1::Record& record;
        character_apply_result::Actor& actor;
        ~WriteBack() {
            record.target_change_marker_14d0 = actor.combo_14d0;
            record.source_byte_14f0 = actor.invulnerable_14f0;
            record.source_push_death_53b = actor.push_death_53b;
        }
    } write_back{record, self.actor_};
    const character_apply_result::Services services{&self, apply_service};
    const auto status = character_apply_result::execute(
        &arguments, &self.bindings_.apply_globals, &services,
        request->result, &self.apply_result_);
    if (status != character_apply_result::Status::complete) {
        error = std::string("Faery F_ApplyResult: ") + apply_status(status) +
                " at operation " + std::to_string(self.apply_result_.last_operation);
        if (!self.last_service_error_.empty())
            error += ": " + self.last_service_error_;
        if (self.apply_result_.unsupported_pc)
            error += " (source PC " + std::to_string(self.apply_result_.unsupported_pc) + ")";
        return 1;
    }
    error.clear();
    return 0;
}

} // namespace dh2::native::faery_combat_v1
