#include "character_faery_level_registry_v1.hpp"

#include <limits>
#include <utility>

namespace dh2::character_faery_level_registry_v1 {
namespace {
constexpr std::uint32_t kIterationGuard = 1'000'000;

bool invoke(placement::Services services, placement::Operation operation,
            placement::Reply& reply, placement::Identity subject = 0) {
    if (!services.invoke) return false;
    reply = {};
    try {
        return services.invoke(services.context,
            {operation, subject, 0, 0, nullptr, 0}, &reply) == 0;
    } catch (...) {
        return false;
    }
}
} // namespace

Owner::Owner(std::vector<Entry> entries, placement::Services level_services)
    : entries_(std::move(entries)), level_services_(level_services),
      runtime_({this, &Owner::dispatch}) {}

Entry* Owner::find(placement::Identity identity) noexcept {
    for (auto& entry : entries_)
        if (entry.character == identity) return &entry;
    return nullptr;
}

const Owner::Member* Owner::find_cursor(placement::Identity cursor) const noexcept {
    for (const auto& member : members_)
        if (member.cursor == cursor) return &member;
    return nullptr;
}

Status Owner::snapshot(Result& result, std::string& error) {
    members_.clear();
    list_end_ = 0;
    for (std::size_t i = 0; i < entries_.size(); ++i) {
        if (!entries_[i].character || !entries_[i].graph) {
            error = "Faery Level registry contains a null Character graph entry";
            return Status::invalid_argument;
        }
        for (std::size_t j = 0; j < i; ++j) {
            if (entries_[j].character == entries_[i].character) {
                error = "Faery Level registry contains duplicate Character identities";
                return Status::invalid_argument;
            }
        }
    }
    if (!level_services_.invoke) {
        error = "Faery Level placement providers are missing";
        return Status::invalid_argument;
    }

    placement::Reply reply;
    if (!invoke(level_services_, placement::Operation::list_begin, reply)) {
        error = "Faery Level character-list begin failed";
        return Status::list_service_failed;
    }
    auto cursor = reply.identity;
    if (!invoke(level_services_, placement::Operation::list_end, reply)) {
        error = "Faery Level character-list end failed";
        return Status::list_service_failed;
    }
    list_end_ = reply.identity;

    std::uint32_t iterations = 0;
    while (cursor != list_end_) {
        if (++iterations > kIterationGuard) {
            error = "Faery Level character list exceeded the source guard";
            return Status::list_service_failed;
        }
        Member member{};
        member.cursor = cursor;
        if (!invoke(level_services_, placement::Operation::list_value, reply,
                    cursor)) {
            error = "Faery Level character-list value failed";
            return Status::list_service_failed;
        }
        member.character = reply.identity;
        if (member.character) {
            if (!invoke(level_services_, placement::Operation::is_faery, reply,
                        member.character)) {
                error = "Faery classification provider failed";
                return Status::list_service_failed;
            }
            member.faery = reply.word != 0;
            if (!member.faery) {
                if (!invoke(level_services_, placement::Operation::is_follower,
                            reply, member.character)) {
                    error = "Follower classification provider failed";
                    return Status::list_service_failed;
                }
                member.follower = reply.word != 0;
            }
            if (member.faery || member.follower) {
                auto* entry = find(member.character);
                if (!entry || !entry->graph) {
                    error = "A classified Level actor has no canonical Faery graph entry";
                    return Status::missing_character_graph;
                }
                if (binding::validate_character_binding(*entry->graph,
                        member.faery, error) !=
                        binding::Status::complete) {
                    if (error.empty()) error = "A Level actor graph is incomplete";
                    return Status::graph_incomplete;
                }
                if (member.faery) ++result.faeries;
                else ++result.followers;
                ++result.classified_characters;
            }
        }
        if (!invoke(level_services_, placement::Operation::list_next, reply,
                    cursor)) {
            error = "Faery Level character-list next failed";
            return Status::list_service_failed;
        }
        member.next = reply.identity;
        for (const auto& previous : members_) {
            if (previous.cursor == member.cursor ||
                (member.next != list_end_ && previous.cursor == member.next)) {
                error = "Faery Level intrusive list contains a cycle or duplicate cursor";
                return Status::list_service_failed;
            }
        }
        members_.push_back(member);
        cursor = member.next;
    }
    return Status::complete;
}

Status Owner::preflight(Result* output, std::string& error) {
    error.clear();
    if (busy_ || !output) {
        error = "Faery Level registry is busy or has no result owner";
        return Status::invalid_argument;
    }
    *output = {};
    busy_ = true;
    struct Leave { bool& busy; ~Leave() { busy = false; } } leave{busy_};
    return snapshot(*output, error);
}

int Owner::dispatch(void* raw, const placement::Request& request,
                    placement::Reply* reply) {
    auto& self = *static_cast<Owner*>(raw);
    if (!reply) return 1;
    *reply = {};
    switch (request.operation) {
    case placement::Operation::list_begin:
        reply->identity = self.members_.empty() ? self.list_end_
                                                 : self.members_.front().cursor;
        return 0;
    case placement::Operation::list_end:
        reply->identity = self.list_end_;
        return 0;
    case placement::Operation::list_value: {
        const auto* member = self.find_cursor(request.subject);
        if (!member) return 1;
        reply->identity = member->character;
        return 0;
    }
    case placement::Operation::list_next: {
        const auto* member = self.find_cursor(request.subject);
        if (!member) return 1;
        reply->identity = member->next;
        return 0;
    }
    case placement::Operation::is_faery:
    case placement::Operation::is_follower: {
        auto* entry = self.find(request.subject);
        if (!entry || !entry->graph) return 1;
        const auto* member = static_cast<const Member*>(nullptr);
        for (const auto& candidate : self.members_)
            if (candidate.character == request.subject) { member = &candidate; break; }
        if (!member) return 1;
        if (request.operation == placement::Operation::is_faery) {
            reply->word = member->faery ? 1 : 0;
            return 0;
        }
        reply->word = member->follower ? 1 : 0;
        return 0;
    }
    case placement::Operation::set_position:
    case placement::Operation::force_update_position: {
        auto* entry = self.find(request.subject);
        if (!entry || !entry->graph || !entry->graph->position) return 1;
        auto& graph = *entry->graph;
        std::string error;
        if (request.operation == placement::Operation::set_position) {
            if (!request.vector) return 1;
            const game_object_position_owner_v1::Point3 point{
                request.vector->x, request.vector->y, request.vector->z};
            return game_object_position_owner_v1::set_position(
                graph.position, point, true, &graph.position_services, error) ==
                game_object_position_owner_v1::Status::complete ? 0 : 1;
        }
        return game_object_position_owner_v1::force_update_position(
            graph.position, &graph.position_services, error) ==
            game_object_position_owner_v1::Status::complete ? 0 : 1;
    }
    default:
        if (!self.level_services_.invoke) return 1;
        try {
            return self.level_services_.invoke(self.level_services_.context,
                                               request, reply);
        } catch (...) {
            return 1;
        }
    }
}

Status Owner::place(placement::Identity explicit_player_character,
                    Result* output, std::string& error) {
    error.clear();
    if (busy_ || !output) {
        error = "Faery Level registry is busy or has no result owner";
        return Status::invalid_argument;
    }
    *output = {};
    busy_ = true;
    struct Leave { bool& busy; ~Leave() { busy = false; } } leave{busy_};
    auto status = snapshot(*output, error);
    if (status != Status::complete) return status;

    // Every classified member has a complete, same-Character graph before the
    // first source mutation. Runtime consumes the captured real list cursors.
    const auto source_status = runtime_.place(explicit_player_character,
                                               &output->placement);
    if (source_status != placement::Status::complete) {
        error = "Source Faery placement failed at operation " +
            std::to_string(static_cast<unsigned>(output->placement.last_operation));
        return Status::placement_failed;
    }
    return Status::complete;
}

} // namespace dh2::character_faery_level_registry_v1
