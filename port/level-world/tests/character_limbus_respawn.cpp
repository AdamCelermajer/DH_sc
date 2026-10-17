#include "../character_limbus_respawn.hpp"

#include "../character_coordinator.hpp"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character_limbus_respawn;

namespace {

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    dh2::character::Coordinator coordinator;
    std::string trace;
    std::vector<std::int32_t> properties;
    std::size_t property_index = 0;
    std::uint32_t property_calls = 0;
    std::int32_t property_id = -1;
    bool fail_property = false;
    std::uint8_t limbus_gate = 0;
    std::uint8_t init_spawned_gate = 0;
    std::vector<std::uint8_t> init_gate_values;
    std::size_t init_gate_index = 0;
    std::uint32_t init_gate_reads = 0;
    std::uint8_t online_byte5 = 0;
    std::uint32_t online_reads = 0;
    bool fail_online = false;
    std::int32_t hosting_result = 0;
    std::uint32_t hosting_reads = 0;
    bool fail_hosting = false;
    bool visible_changes_limbus_gate = false;
    bool throw_on_visible = false;
    std::uint8_t gate_after_visibility = 0;
    std::uint32_t visible_argument = 1;
    std::uint32_t timer_calls = 0;
    std::uint32_t timer_duration = 0;
    std::int32_t timer_repeat = -99;
    std::int32_t timer_event = -99;
    std::uintptr_t timer_user_ref = 99;
    std::int32_t timer_return_override = -999;
    std::uint32_t clear_aggro_calls = 0;

    Fixture() : coordinator(0x1234) {
        trace.reserve(64);
        dh2::character::CoordinatorBindings bindings{};
        bindings.context = this;
        bindings.facts = [](void*) { return dh2::character::Facts{}; };
        bindings.services = {this, [](void*, dh2::character::State*,
                                      const dh2::character::Request*) {}};
        coordinator.bind(bindings);
    }
};

void mark(Fixture& fixture, char event) {
    if (!fixture.trace.empty()) fixture.trace.push_back(',');
    fixture.trace.push_back(event);
}

void clear_focus_word(void* context) {
    mark(*static_cast<Fixture*>(context), 'F');
}

void set_visible(void* context, std::uint32_t visible) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'V');
    if (fixture.throw_on_visible)
        throw std::runtime_error("SetVisible adapter exception");
    fixture.visible_argument = visible;
    if (fixture.visible_changes_limbus_gate)
        fixture.limbus_gate = fixture.gate_after_visibility;
}

std::int32_t read_limbus_gate(void* context, std::uint8_t* raw_byte) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'G');
    *raw_byte = fixture.limbus_gate;
    return 0;
}

std::int32_t read_init_spawned(void* context, std::uint8_t* raw_byte) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'S');
    ++fixture.init_gate_reads;
    if (fixture.init_gate_index < fixture.init_gate_values.size())
        *raw_byte = fixture.init_gate_values[fixture.init_gate_index++];
    else
        *raw_byte = fixture.init_spawned_gate;
    return 0;
}

std::int32_t read_property(void* context, std::int32_t property_id,
                           std::int32_t* raw_value) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'P');
    ++fixture.property_calls;
    fixture.property_id = property_id;
    if (fixture.fail_property) return 1;
    if (fixture.property_index >= fixture.properties.size()) return 1;
    *raw_value = fixture.properties[fixture.property_index++];
    return 0;
}

std::int32_t read_online_byte(void* context, std::uint8_t* raw_byte) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'O');
    ++fixture.online_reads;
    if (fixture.fail_online) return 1;
    *raw_byte = fixture.online_byte5;
    return 0;
}

std::int32_t is_local_player_hosting(void* context, std::int32_t* result) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'H');
    ++fixture.hosting_reads;
    if (fixture.fail_hosting) return 1;
    *result = fixture.hosting_result;
    return 0;
}

std::int32_t start_timer(void* context, std::uint32_t duration,
                         std::int32_t repeat, std::int32_t event,
                         std::uintptr_t user_ref) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'T');
    ++fixture.timer_calls;
    fixture.timer_duration = duration;
    fixture.timer_repeat = repeat;
    fixture.timer_event = event;
    fixture.timer_user_ref = user_ref;
    const auto actual_id = fixture.coordinator.start_timer(
        duration, repeat, event, user_ref);
    return fixture.timer_return_override == -999
        ? actual_id : fixture.timer_return_override;
}

void clear_all_aggro(void* context) {
    auto& fixture = *static_cast<Fixture*>(context);
    mark(fixture, 'A');
    ++fixture.clear_aggro_calls;
}

dh2::character_respawn::Services property_services(Fixture& fixture) {
    return {&fixture, read_property, {nullptr, nullptr}};
}

Services services(Fixture& fixture,
                  const dh2::character_respawn::Services* properties) {
    return {&fixture, clear_focus_word, set_visible, read_limbus_gate,
            read_init_spawned, properties, read_online_byte,
            is_local_player_hosting, start_timer, clear_all_aggro};
}

void require_trace(const Fixture& fixture, const char* expected) {
    require(fixture.trace == expected, "source callback order");
}

void require_real_timer(const Fixture& fixture, std::uint32_t duration) {
    const auto& timers = fixture.coordinator.timers();
    require(fixture.timer_calls == 1 && fixture.timer_duration == duration &&
                fixture.timer_repeat == 0 && fixture.timer_event == 0x2f &&
                fixture.timer_user_ref == 0 && timers.count == 1 &&
                timers.slots[0].active && timers.slots[0].duration_ms == duration &&
                timers.slots[0].repeat == 0 && timers.slots[0].event == 0x2f &&
                timers.slots[0].user_ref == 0,
            "StartTimer parameters passed through real Character Coordinator");
}

}  // namespace

int main() {
    std::uint32_t cases = 0;
    try {
        // Reject a result overlapping the known borrowed property-service
        // structure before any source prefix call or mutation.
        {
            Fixture fixture;
            auto property = property_services(fixture);
            const auto before = property;
            auto dispatch = services(fixture, &property);
            auto* aliased_result = reinterpret_cast<Result*>(&property);
            const auto status = on_focus(&dispatch, aliased_result);
            require(status == Status::invalid_argument &&
                        std::memcmp(&property, &before, sizeof(property)) == 0 &&
                        fixture.trace.empty() && fixture.clear_aggro_calls == 0,
                    "provider/result alias rejected before source prefix");
            ++cases;
        }

        // The exact source prefix is +0x520=0, then virtual SetVisible(false),
        // then the +0x530 gate. With gate closed, property and online service
        // pointers are absent and must not be needed or queried.
        {
            Fixture fixture;
            auto dispatch = services(fixture, nullptr);
            dispatch.read_init_spawned_suppression = nullptr;
            dispatch.read_online_byte5 = nullptr;
            dispatch.is_local_player_hosting = nullptr;
            dispatch.start_timer = nullptr;
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.limbus_gate_byte == 0 &&
                        result.delay_getter_calls == 0 &&
                        result.online_byte_read == 0 && result.hosting_query == 0 &&
                        result.timer_requested == 0 && result.all_aggro_cleared == 1 &&
                        fixture.visible_argument == 0 && fixture.clear_aggro_calls == 1,
                    "closed Limbus timer gate skips all delay and manager reads");
            require_trace(fixture, "F,V,G,A");
            ++cases;
        }

        // Visibility callback changes the field before the source reads it;
        // this catches an eager/cached read of Character+0x530.
        {
            Fixture fixture;
            fixture.visible_changes_limbus_gate = true;
            fixture.gate_after_visibility = 1;
            fixture.init_spawned_gate = 1;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.limbus_gate_byte == 1 &&
                        result.delay_getter_calls == 1 &&
                        result.init_spawned_byte_reads == 1 &&
                        result.property_reads == 0 && result.online_byte_read == 0 &&
                        result.timer_requested == 0,
                    "+0x530 read follows SetVisible while +0x1481 independently suppresses delay");
            require_trace(fixture, "F,V,G,S,A");
            ++cases;
        }

        // A positive raw property is required before the outer online mode
        // byte is read.  A zero RespawnTime reaches neither manager service.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {0};
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.first_delay_ms == 0 &&
                        result.online_byte_read == 0 && result.hosting_query == 0 &&
                        result.timer_requested == 0 && fixture.online_reads == 0,
                    "nonpositive first delay skips online/hosting queries");
            require_trace(fixture, "F,V,G,S,P,A");
            ++cases;
        }

        // Offline mode byte (0) schedules directly. OnFocus calls GetDelay
        // twice, and Coordinator owns the actual timer store.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120, 5120};
            fixture.online_byte5 = 0;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.delay_getter_calls == 2 &&
                        result.init_spawned_byte_reads == 2 &&
                        result.property_reads == 2 && result.first_delay_ms == 20000 &&
                        result.second_delay_ms == 20000 && result.online_byte_read == 1 &&
                        result.hosting_query == 0 && result.timer_requested == 1 &&
                        result.all_aggro_cleared == 1 && fixture.property_id == 11,
                    "offline route makes two independent delay reads");
            require_real_timer(fixture, 20000);
            require_trace(fixture, "F,V,G,S,P,O,S,P,T,A");
            ++cases;
        }

        // Online byte nonzero requires the hosting predicate; false skips the
        // second delay getter and timer request.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120};
            fixture.online_byte5 = 1;
            fixture.hosting_result = 0;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.delay_getter_calls == 1 &&
                        result.online_byte_read == 1 && result.hosting_query == 1 &&
                        result.timer_requested == 0 && fixture.timer_calls == 0,
                    "online nonhost skips second GetRespawnDelay and timer");
            require_trace(fixture, "F,V,G,S,P,O,H,A");
            ++cases;
        }

        // The source does not re-check the second delay. It can turn zero after
        // the manager lookup and is still sent to Coordinator::start_timer.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120, 0};
            fixture.online_byte5 = 1;
            fixture.hosting_result = 1;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.first_delay_ms == 20000 &&
                        result.second_delay_ms == 0 && result.timer_requested == 1,
                    "second zero delay is scheduled without recheck");
            require_real_timer(fixture, 0);
            require_trace(fixture, "F,V,G,S,P,O,H,S,P,T,A");
            ++cases;
        }

        // Same no-recheck rule for a negative second signed delay. StartTimer
        // receives its low 32-bit word, not a clamped or rejected value.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120, -1};
            fixture.online_byte5 = 0;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.second_delay_ms == -1000 &&
                        result.timer_duration_bits == 0xfffffc18u &&
                        result.timer_requested == 1,
                    "negative second delay is passed as raw ARM word");
            require_real_timer(fixture, 0xfffffc18u);
            ++cases;
        }

        // Manager callbacks can change the InitSpawned byte between the two
        // getter invocations. The second getter must observe the new byte and
        // skip property access, yet the source still requests its zero result.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120};
            fixture.init_gate_values = {0, 1};
            fixture.online_byte5 = 0;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.delay_getter_calls == 2 &&
                        result.init_spawned_byte_reads == 2 &&
                        result.property_reads == 1 && result.second_delay_ms == 0 &&
                        result.timer_requested == 1,
                    "second getter observes live InitSpawned suppression byte");
            require_real_timer(fixture, 0);
            require_trace(fixture, "F,V,G,S,P,O,S,T,A");
            ++cases;
        }

        // A negative/nonpositive first delay skips mode and hosting reads even
        // though the Limbus timer gate is open.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {-257};
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.first_delay_ms == -2000 &&
                        result.online_byte_read == 0 && result.hosting_query == 0 &&
                        fixture.online_reads == 0 && fixture.clear_aggro_calls == 1,
                    "negative first delay skips all online/hosting reads");
            ++cases;
        }

        // PlayerManager::IsLocalPlayerHosting is a source truthiness test, not
        // a strict bool. Its return value is used only when byte+5 is nonzero.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120, 1};
            fixture.online_byte5 = 0x80;
            fixture.hosting_result = -1;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.hosting_query == 1 &&
                        result.hosting_source_result == -1 &&
                        result.timer_requested == 1,
                    "any nonzero hosting result schedules");
            require_real_timer(fixture, 0);
            ++cases;
        }

        // Source ignores StartTimer's returned ID; the real Coordinator has
        // already accepted the timer even if an adapter's returned value is
        // otherwise nonzero/negative.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.properties = {5120, 5120};
            fixture.online_byte5 = 0;
            fixture.timer_return_override = -1;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::complete && result.timer_requested == 1,
                    "timer ID return is discarded");
            require_real_timer(fixture, 20000);
            require_trace(fixture, "F,V,G,S,P,O,S,P,T,A");
            ++cases;
        }

        // A port-only callback failure is not a source branch. Preserve the
        // completed prefix, but do not invent the normal-flow cleanup call.
        // No result is published for incomplete adapter work.
        {
            Fixture fixture;
            fixture.limbus_gate = 1;
            fixture.fail_property = true;
            auto property = property_services(fixture);
            auto dispatch = services(fixture, &property);
            Result result{};
            std::memset(&result, 0x5a, sizeof(result));
            Result before = result;
            const auto status = on_focus(&dispatch, &result);
            require(status == Status::respawn_delay_failed &&
                        std::memcmp(&result, &before, sizeof(result)) == 0 &&
                        fixture.clear_aggro_calls == 0,
                    "property service failure avoids invented cleanup and preserves output");
            require_trace(fixture, "F,V,G,S,P");
            ++cases;
        }

        // Exceptions from adapter callbacks propagate. They do not synthesize
        // the original routine's normal-flow ClearAllAggro call.
        {
            Fixture fixture;
            fixture.throw_on_visible = true;
            auto dispatch = services(fixture, nullptr);
            Result result{};
            bool propagated = false;
            try {
                (void)on_focus(&dispatch, &result);
            } catch (const std::runtime_error&) {
                propagated = true;
            }
            require(propagated && fixture.clear_aggro_calls == 0,
                    "adapter exception propagates without invented cleanup");
            require_trace(fixture, "F,V");
            ++cases;
        }

        std::printf("{\"limbus_respawn_focus_cases\":%u,\"byte530_distinct_from_1481\":true,\"delay_getter_called_twice\":true,\"coordinator_timer_store_reused\":true,\"timer_repeat\":0,\"timer_event\":47,\"online_host_gate\":true,\"normal_path_clear_aggro_last\":true,\"adapter_failure_does_not_invent_clear_aggro\":true,\"mismatches\":0}\n",
                    cases);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "Limbus respawn audit: %s\n", error.what());
        return 1;
    }
}
