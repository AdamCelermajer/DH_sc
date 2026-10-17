#include "character_faery_placement_v1.hpp"

#include <limits>

namespace dh2::character_faery_placement_v1 {
namespace {
constexpr std::uint32_t kIterationGuard = 1'000'000;

struct Call {
    Services services;
    Result& result;
    Status status = Status::complete;

    bool operator()(Operation operation, Reply& reply, Identity subject = 0,
                    Identity argument = 0, std::uint32_t index = 0,
                    const Vector3* vector = nullptr, std::int32_t value = 0) {
        if (status != Status::complete) return false;
        result.last_operation = operation;
        if (result.callbacks == std::numeric_limits<std::uint32_t>::max()) {
            status = Status::source_guard;
            return false;
        }
        ++result.callbacks;
        if (!services.invoke) {
            status = Status::service_unavailable;
            return false;
        }
        reply = {};
        try {
            if (services.invoke(services.context,
                                {operation, subject, argument, index, vector,
                                 value},
                                &reply) != 0) {
                status = Status::service_failed;
                return false;
            }
        } catch (...) {
            status = Status::service_failed;
            return false;
        }
        return true;
    }
};

bool bool_call(Call& call, Operation operation, Identity subject, bool& value) {
    Reply reply;
    if (!call(operation, reply, subject)) return false;
    value = reply.word != 0;
    return true;
}

bool player_character(Call& call, Identity player, Identity& character) {
    Reply reply;
    if (!player || !call(Operation::player_character, reply, player)) {
        if (!player && call.status == Status::complete)
            call.status = Status::source_guard;
        return false;
    }
    character = reply.identity;
    return true;
}
} // namespace

Runtime::Runtime(Services services) : services_(services) {}

Status Runtime::place(Identity explicit_player_character, Result* output) {
    if (busy_) return Status::source_guard;
    if (!output || reinterpret_cast<std::uintptr_t>(output) % alignof(Result))
        return Status::invalid_argument;
    const auto self_begin = reinterpret_cast<std::uintptr_t>(this);
    const auto output_begin = reinterpret_cast<std::uintptr_t>(output);
    if (self_begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(*this) ||
        output_begin > std::numeric_limits<std::uintptr_t>::max() - sizeof(*output) ||
        (self_begin < output_begin + sizeof(*output) &&
         output_begin < self_begin + sizeof(*this)))
        return Status::invalid_argument;

    *output = {};
    busy_ = true;
    struct Leave { bool& busy; ~Leave() { busy = false; } } leave{busy_};
    Call call{services_, *output};
    Reply reply;

    if (!call(Operation::list_begin, reply)) return call.status;
    Identity cursor = reply.identity;
    if (!call(Operation::list_end, reply)) return call.status;
    const Identity end = reply.identity;

    std::uint32_t iterations = 0;
    while (cursor != end) {
        if (++iterations > kIterationGuard) return Status::source_guard;
        if (!call(Operation::list_value, reply, cursor)) return call.status;
        const Identity character = reply.identity;
        output->last_character = character;
        if (output->visited == std::numeric_limits<std::uint32_t>::max())
            return Status::source_guard;
        ++output->visited;

        if (!character) {
            ++output->skipped_unclassified;
        } else {
            // Source short-circuits IsFollower when IsFaerie is already true.
            bool initially_faery = false;
            if (!bool_call(call, Operation::is_faery, character, initially_faery))
                return call.status;
            bool initially_follower = false;
            if (!initially_faery &&
                !bool_call(call, Operation::is_follower, character,
                           initially_follower))
                return call.status;

            if (!initially_faery && !initially_follower) {
                ++output->skipped_unclassified;
            } else {
                Identity chosen_character = explicit_player_character;
                Identity hosting_player = 0;

                if (!chosen_character) {
                    bool online = false;
                    if (!bool_call(call, Operation::online_state, 0, online))
                        return call.status;
                    if (online) {
                        if (!call(Operation::hosting_player, reply)) return call.status;
                        hosting_player = reply.identity;
                        if (!player_character(call, hosting_player, chosen_character))
                            return call.status;

                        bool local_host = false;
                        if (!bool_call(call, Operation::is_local_player_hosting, 0,
                                       local_host))
                            return call.status;
                        if (!local_host) {
                            // The original fetches the hosting Player again
                            // for the remote-host state read; keep that call
                            // order instead of reusing the earlier pointer.
                            if (!call(Operation::hosting_player_for_follower_state,
                                      reply))
                                return call.status;
                            const Identity state_host = reply.identity;
                            if (!call(Operation::follower_host_state, reply,
                                      character, state_host))
                                return call.status;
                        }
                    } else {
                        // The source call is GetLocalPlayer(0, 1).
                        if (!call(Operation::local_player, reply, 0, 0, 0,
                                  nullptr, 1))
                            return call.status;
                        if (!player_character(call, reply.identity,
                                              chosen_character))
                            return call.status;
                    }

                    // Source's early continue: no look/target queries,
                    // placement, zoning, or faery work when this is null.
                    if (!chosen_character) {
                        ++output->skipped_null_player;
                        if (!call(Operation::list_next, reply, cursor))
                            return call.status;
                        cursor = reply.identity;
                        continue;
                    }
                }
                output->chosen_player_character = chosen_character;

                Reply look, target;
                if (!call(Operation::look_at_vector, look, chosen_character) ||
                    !call(Operation::target_position, target, chosen_character))
                    return call.status;
                const Vector3 position{
                    look.vector.x + target.vector.x,
                    look.vector.y + target.vector.y,
                    look.vector.z + target.vector.z,
                };
                if (!call(Operation::set_position, reply, character, 0, 0,
                          &position) ||
                    !call(Operation::force_update_position, reply, character))
                    return call.status;

                // This is a second, fresh IsFaerie query in the original.
                bool is_faery_after_placement = false;
                if (!bool_call(call, Operation::is_faery, character,
                               is_faery_after_placement))
                    return call.status;
                if (!is_faery_after_placement) {
                    if (!call(Operation::disable_zoning, reply, character))
                        return call.status;
                    ++output->placed_followers;
                } else {
                    ++output->placed_faeries;

                    Reply count_reply;
                    if (!call(Operation::player_count, count_reply)) return call.status;
                    const std::int32_t player_count = count_reply.word;
                    if (player_count > static_cast<std::int32_t>(kIterationGuard))
                        return Status::source_guard;
                    for (std::int32_t i = 0; i < player_count; ++i) {
                        Reply player_reply;
                        if (!call(Operation::player_at, player_reply, 0, 0,
                                  static_cast<std::uint32_t>(i)))
                            return call.status;
                        Identity player_char = 0;
                        if (!player_character(call, player_reply.identity,
                                              player_char))
                            return call.status;
                        if (player_char) {
                            if (!call(Operation::set_player_faery, reply,
                                      player_char, character))
                                return call.status;
                            ++output->player_faery_links;
                        }
                    }

                    Reply previous_master_reply;
                    if (!call(Operation::previous_ai_master, previous_master_reply,
                              character))
                        return call.status;
                    const Identity previous_master = previous_master_reply.identity;

                    // The source independently requests GetLocalPlayer(0,1)
                    // for AI master/current-faery selection, even when the
                    // placement target was explicit or the online host.
                    if (!call(Operation::local_player, reply, 0, 0, 0,
                              nullptr, 1))
                        return call.status;
                    Identity local_character = 0;
                    if (!player_character(call, reply.identity, local_character))
                        return call.status;
                    const Identity master = local_character ? local_character
                                                            : chosen_character;
                    if (!call(Operation::set_ai_master, reply, character, master))
                        return call.status;

                    Reply faery_id_reply;
                    if (!call(Operation::current_faery_id, faery_id_reply, master,
                              0, 0, nullptr, -1))
                        return call.status;
                    if (!call(Operation::change_faery, reply, chosen_character,
                              0, 0, nullptr, faery_id_reply.word))
                        return call.status;
                    if (!call(Operation::set_ai_master, reply, character,
                              previous_master))
                        return call.status;
                }
            }
        }

        // The list's next link is read at the source's common tail, after all
        // service effects. Early continues above read it at their own sites.
        if (!call(Operation::list_next, reply, cursor)) return call.status;
        cursor = reply.identity;
    }
    return Status::complete;
}

} // namespace dh2::character_faery_placement_v1
