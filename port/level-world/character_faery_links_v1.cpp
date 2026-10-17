#include "character_faery_links_v1.hpp"

#include <algorithm>

namespace dh2::character_faery_links_v1 {
namespace {
namespace ctor = character_constructor_owner_v1;

bool lifecycle_ready(const factory::Record& record,
                    const ai::State& ai_owner) noexcept {
    const auto identity = record.constructor.identity;
    return identity && record.character.identity == identity &&
        record.game_object.identity == identity &&
        record.constructor.constructor_complete &&
        record.constructor.registered_states == ctor::all_registered_states &&
        record.object_registered && record.character_listed &&
        record.source_properties_ready() && record.init_post_complete() &&
        record.init_final_complete() &&
        record.components.find(ctor::Component::ai) == &ai_owner &&
        ai_owner.identity && ai_owner.owner_04 == identity;
}
} // namespace

Status Owner::register_character(factory::Record& record, ai::State& ai_owner,
                                 std::int32_t source_character_type, Role role,
                                 std::string& error) {
    error.clear();
    if (!record.constructor.identity || !ai_owner.identity) {
        error = "Faery links require a stable factory Character and CharAI owner";
        return Status::invalid_argument;
    }
    if (!lifecycle_ready(record, ai_owner)) {
        error = "Faery links require this Character's completed source lifecycle and canonical CharAI";
        return Status::source_character_incomplete;
    }
    const auto expected = role == Role::faery ? 3 : 1;
    if (source_character_type != expected) {
        error = role == Role::faery
            ? "Faery link owner is not source Character type 3"
            : "Faery link target is not source Player Character type 1";
        return Status::wrong_character_type;
    }
    if (find(record.constructor.identity)) {
        error = "Faery link Character identity is already registered";
        return Status::duplicate_identity;
    }
    try {
        entries_.push_back({&record, &ai_owner, role});
    } catch (...) {
        error = "Faery link Character registry allocation failed";
        return Status::provider_unavailable;
    }
    error.clear();
    return Status::complete;
}

Status Owner::unregister_character(std::uintptr_t identity,
                                   std::string& error) noexcept {
    error.clear();
    auto found = std::find_if(entries_.begin(), entries_.end(),
        [identity](const Entry& entry) {
            return entry.record && entry.record->constructor.identity == identity;
        });
    if (!identity || found == entries_.end()) {
        error = "Faery link Character identity is not registered";
        return Status::invalid_argument;
    }
    if (found->role == Role::faery) {
        for (auto& entry : entries_) {
            if (entry.role == Role::player && entry.record &&
                entry.record->faery_character_420 == identity)
                entry.record->faery_character_420 = 0;
        }
    } else if (found->record) {
        found->record->faery_character_420 = 0;
    }
    entries_.erase(found);
    return Status::complete;
}

Owner::Entry* Owner::find(std::uintptr_t identity) noexcept {
    const auto found = std::find_if(entries_.begin(), entries_.end(),
        [identity](const Entry& entry) {
            return entry.record &&
                entry.record->constructor.identity == identity;
        });
    return found == entries_.end() ? nullptr : &*found;
}

const Owner::Entry* Owner::find(std::uintptr_t identity) const noexcept {
    const auto found = std::find_if(entries_.begin(), entries_.end(),
        [identity](const Entry& entry) {
            return entry.record &&
                entry.record->constructor.identity == identity;
        });
    return found == entries_.end() ? nullptr : &*found;
}

placement::Services Owner::services(
        void* context,
        int (*delegate)(void*, const placement::Request&, placement::Reply*),
        MasterServices master)
        noexcept {
    delegate_context_ = context;
    delegate_ = delegate;
    master_ = master;
    return {this, &Owner::dispatch};
}

int Owner::dispatch(void* raw, const placement::Request& request,
                    placement::Reply* reply) {
    auto* self = static_cast<Owner*>(raw);
    if (!self || !reply) return 1;
    *reply = {};
    if (request.operation == placement::Operation::is_faery) {
        const auto* entry = self->find(request.subject);
        if (entry && entry->role == Role::faery) {
            reply->word = 1;
            return 0;
        }
    }
    if (request.operation == placement::Operation::set_player_faery) {
        auto* player = self->find(request.subject);
        const auto* faery = self->find(request.argument);
        if (!player || player->role != Role::player || !faery ||
            faery->role != Role::faery || !player->record || !faery->record)
            return 1;
        player->record->faery_character_420 = request.argument;
        return 0;
    }
    if (request.operation == placement::Operation::previous_ai_master) {
        const auto* faery = self->find(request.subject);
        if (!faery || faery->role != Role::faery || !faery->ai_owner)
            return 1;
        reply->identity = faery->ai_owner->master_50;
        return 0;
    }
    if (request.operation == placement::Operation::set_ai_master) {
        auto* faery = self->find(request.subject);
        if (!faery || faery->role != Role::faery || !faery->ai_owner)
            return 1;
        // The old master can be a Character outside this registry; the source
        // saves and restores it verbatim around ChangeFaery. AI_SetMaster
        // stores the pointer first. For non-null masters it then refreshes
        // byte+0x54 from IsMasterHostPlayer and byte+0x55 from squared target
        // distance versus the Faery AI row's squared view-distance.
        // Preflight callback availability so an unwired adapter cannot leave
        // a half-applied master change; reached provider failures below still
        // retain the exact source-side prefix.
        if (request.argument && (!self->master_.is_host_player ||
            !self->master_.target_position ||
            !self->master_.owner_view_distance)) return 1;
        faery->ai_owner->master_50 = request.argument;
        if (!request.argument) return 0;
        bool host_player = false;
        if (self->master_.is_host_player(self->master_.context,
                request.argument, &host_player) != 0) return 1;
        faery->ai_owner->byte_54 = host_player ? 0 : 1;
        placement::Vector3 master_position{}, faery_position{};
        if (self->master_.target_position(self->master_.context,
                request.argument, &master_position) != 0 ||
            self->master_.target_position(self->master_.context,
                request.subject, &faery_position) != 0) return 1;
        float view_distance = 0.0f;
        if (self->master_.owner_view_distance(self->master_.context,
                request.subject, &view_distance) != 0) return 1;
        const float dx = master_position.x - faery_position.x;
        const float dy = master_position.y - faery_position.y;
        const float dz = master_position.z - faery_position.z;
        faery->ai_owner->byte_55 =
            view_distance * view_distance > dx * dx + dy * dy + dz * dz;
        return 0;
    }
    return self->delegate_ &&
        self->delegate_(self->delegate_context_, request, reply) == 0 ? 0 : 1;
}

} // namespace dh2::character_faery_links_v1
