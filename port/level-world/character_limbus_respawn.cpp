#include "character_limbus_respawn.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::character_limbus_respawn {
namespace {

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

bool valid_result(const Services* services, const Result* result) {
    if (services == nullptr || result == nullptr ||
        overlaps(services, sizeof(*services), result, sizeof(*result))) {
        return false;
    }
    // This nested block is the only borrowed service storage whose extent is
    // known here. Compare ranges without reading the provider; +0x530=0 must
    // retain its no-read behavior.
    return services->respawn_services == nullptr ||
           !overlaps(services->respawn_services,
                     sizeof(*services->respawn_services), result,
                     sizeof(*result));
}

Status read_delay(const Services& services,
                  dh2::character_respawn::DelayResult* delay,
                  Result& completed) {
    if (services.read_init_spawned_suppression == nullptr)
        return Status::invalid_argument;

    std::uint8_t suppression = 0;
    if (services.read_init_spawned_suppression(services.context,
                                               &suppression) != 0) {
        return Status::init_spawned_read_failed;
    }
    ++completed.init_spawned_byte_reads;

    const dh2::character_respawn::Facts facts{0, suppression, {0, 0, 0}};
    const auto status = dh2::character_respawn::get_delay(
        &facts, services.respawn_services, delay);
    if (status != dh2::character_respawn::Status::complete)
        return Status::respawn_delay_failed;
    ++completed.delay_getter_calls;
    completed.property_reads += delay->property_read;
    return Status::complete;
}

}  // namespace

Status on_focus(const Services* services, Result* result) {
    if (!valid_result(services, result) ||
        services->clear_character_focus_word == nullptr ||
        services->set_character_visible == nullptr ||
        services->read_limbus_respawn_gate == nullptr ||
        services->clear_all_aggro == nullptr) {
        return Status::invalid_argument;
    }

    const Services callbacks = *services;
    Result completed{};

    callbacks.clear_character_focus_word(callbacks.context);
    completed.focus_word_cleared = 1;
    callbacks.set_character_visible(callbacks.context, 0);
    completed.visibility_set_false = 1;

    std::uint8_t limbus_gate = 0;
    if (callbacks.read_limbus_respawn_gate(callbacks.context,
                                           &limbus_gate) != 0) {
        return Status::limbus_gate_read_failed;
    }
    completed.limbus_gate_read = 1;
    completed.limbus_gate_byte = limbus_gate;

    if (limbus_gate != 0) {
        dh2::character_respawn::DelayResult first_delay{};
        auto status = read_delay(callbacks, &first_delay, completed);
        if (status != Status::complete) return status;
        completed.first_delay_ms = first_delay.delay_ms;

        if (first_delay.delay_ms > 0) {
            if (callbacks.read_online_byte5 == nullptr)
                return Status::invalid_argument;

            std::uint8_t online_byte = 0;
            if (callbacks.read_online_byte5(callbacks.context,
                                            &online_byte) != 0) {
                return Status::online_byte_read_failed;
            }
            completed.online_byte_read = 1;
            completed.online_byte5 = online_byte;

            bool schedule = online_byte == 0;
            if (online_byte != 0) {
                if (callbacks.is_local_player_hosting == nullptr)
                    return Status::invalid_argument;
                std::int32_t hosting = 0;
                if (callbacks.is_local_player_hosting(callbacks.context,
                                                      &hosting) != 0) {
                    return Status::hosting_query_failed;
                }
                completed.hosting_query = 1;
                completed.hosting_source_result = hosting;
                schedule = hosting != 0;
            }

            if (schedule) {
                dh2::character_respawn::DelayResult second_delay{};
                status = read_delay(callbacks, &second_delay, completed);
                if (status != Status::complete) return status;

                completed.second_delay_ms = second_delay.delay_ms;
                completed.timer_duration_bits =
                    static_cast<std::uint32_t>(second_delay.delay_ms);
                if (callbacks.start_timer == nullptr)
                    return Status::timer_service_missing;

                // Source calls StartTimer(duration, 0, 0x2f, nullptr), then
                // discards the returned timer ID. It does not re-check the
                // second GetRespawnDelay result for positivity.
                (void)callbacks.start_timer(
                    callbacks.context, completed.timer_duration_bits, 0,
                    0x2f, 0);
                completed.timer_requested = 1;
            }
        }
    }

    // This is the normal-flow convergence at the end of the recovered source
    // routine. Port adapter errors and exceptions return/propagate before this
    // point; they do not synthesize an extra source mutation.
    callbacks.clear_all_aggro(callbacks.context);
    completed.all_aggro_cleared = 1;
    *result = completed;
    return Status::complete;
}

}  // namespace dh2::character_limbus_respawn
