#include "../game_object_zoning_visibility.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>

using namespace dh2::game_object_zoning_visibility;

namespace {
constexpr Address kObject = static_cast<Address>(0x02110000u);
constexpr Address kVisualA = static_cast<Address>(0x02220000u);
constexpr Address kVisualB = static_cast<Address>(0x02230000u);
constexpr Address kRoomA = static_cast<Address>(0x02440000u);
constexpr Address kRoomB = static_cast<Address>(0x02450000u);
constexpr Address kSetUpdating = static_cast<Address>(0x33dcf0u);
constexpr Address kIsZonable = static_cast<Address>(0x3883b8u);

struct Context {
    GameObject* object;
    VisualObject* visual_a;
    VisualObject* visual_b;
    std::vector<Request> calls;
    std::uint32_t raw[4];
    std::uint32_t raw_count;
    std::uint32_t zonable_calls;
    std::uint32_t mutate;
    std::uint32_t room_raw_override;
    bool override_room_raw;
    bool fail;
    bool throw_on_update;
};

std::int32_t invoke(void* opaque, const Request* request, std::uint32_t* raw) {
    auto& context = *static_cast<Context*>(opaque);
    context.calls.push_back(*request);
    if (context.fail && request->operation == Operation::room_add_object) return 1;
    if (context.throw_on_update && request->operation == Operation::set_updating)
        throw 7;
    if (request->operation == Operation::room_add_object && context.mutate == 3)
        context.object->room_zone_2f4 = kRoomB;
    if (request->operation == Operation::is_zonable) {
        ++context.zonable_calls;
        if (context.mutate == 1 && context.zonable_calls == 1) {
            context.object->zoning_enabled_2ee = 1;
            context.object->in_zone_2f0 = 0x80;
        } else if (context.mutate == 2 && context.zonable_calls == 1) {
            context.object->visual_2d8 = context.visual_b;
        } else if (context.mutate == 4 && context.zonable_calls == 2) {
            context.object->zoning_enabled_2ee = 0;
            context.object->in_zone_2f0 = 0x80;
        } else if (context.mutate == 5 && context.zonable_calls == 1) {
            context.object->set_updating_target_3c = static_cast<Address>(0xdeadbeefu);
        }
        *raw = context.raw_count ? context.raw[(context.zonable_calls - 1u) % context.raw_count] : 0;
        context.calls.back().value = *raw;
        return 0;
    }
    if (request->operation == Operation::room_is_zoned) {
        *raw = context.override_room_raw ? context.room_raw_override
            : (request->object == kRoomB ? 0u : 1u);
        context.calls.back().value = *raw;
    }
    return 0;
}

struct Fixture {
    GameObject object{};
    VisualObject visual_a{};
    VisualObject visual_b{};
    Context context{};
    Services services{};
    Result result{};

    explicit Fixture(unsigned scenario) {
        object.identity = kObject;
        object.is_zonable_target_c4 = kIsZonable;
        object.set_updating_target_3c = kSetUpdating;
        object.room_zone_2f4 = 0;
        object.visual_2d8 = nullptr;
        object.zoning_enabled_2ee = 0;
        object.in_zone_2f0 = 0;
        object.visibility_80 = 1;
        visual_a = {kVisualA, kObject};
        visual_b = {kVisualB, kObject};
        context.object = &object;
        context.visual_a = &visual_a;
        context.visual_b = &visual_b;
        services = {&context, sizeof(context), &invoke};

        switch (scenario) {
        case 0: // disable: room removal, no-room enrollment, fresh fallback
            object.room_zone_2f4 = kRoomA; object.zoning_enabled_2ee = 1;
            object.in_zone_2f0 = 0x80; context.raw[0] = 0x80000000; context.raw_count = 1;
            break;
        case 1: // disable: IsZonable mutates +2ee/+2f0 after the clear
            object.zoning_enabled_2ee = 1; object.in_zone_2f0 = 2;
            context.raw[0] = 1; context.raw_count = 1; context.mutate = 1;
            break;
        case 2: // enable with room, ZoneEntered, sync, second live query
            object.room_zone_2f4 = kRoomA; object.in_zone_2f0 = 0x80;
            context.raw[0] = 0; context.raw[1] = 1; context.raw_count = 2;
            break;
        case 3: // AddObject callback changes +2f4 to null
            object.room_zone_2f4 = kRoomA; context.raw[0] = 0;
            context.raw_count = 1; context.mutate = 3;
            break;
        case 4: // active enable: visibility-zero sync avoids its own IsZonable
            object.zoning_enabled_2ee = 1; object.visual_2d8 = &visual_a;
            object.visibility_80 = 0; context.raw[0] = 1; context.raw_count = 2;
            break;
        case 5: // active enable: nested SyncVisibility sees raw nonzero in-zone
            object.zoning_enabled_2ee = 1; object.visual_2d8 = &visual_a;
            object.in_zone_2f0 = 0; context.raw[0] = 1; context.raw[1] = 1;
            context.raw[2] = 0; context.raw_count = 3;
            break;
        case 6: // the first query replaces +2d8; source syncs the new VisualObject
            object.zoning_enabled_2ee = 1; object.visual_2d8 = &visual_a;
            context.raw[0] = 1; context.raw[1] = 0; context.raw_count = 2;
            context.mutate = 2;
            break;
        case 7: // direct SyncVisibility false-current-visibility branch
            object.visibility_80 = 0; object.visual_2d8 = &visual_a;
            break;
        case 8: // direct SyncVisibility disabled zoning means show
            object.visual_2d8 = &visual_a; object.zoning_enabled_2ee = 0;
            context.raw[0] = 0xffffffff; context.raw_count = 1;
            break;
        case 9: // room byte zero selects ZoneExited
            object.room_zone_2f4 = kRoomB; context.raw[0] = 0;
            context.raw_count = 1;
            break;
        case 10: // enabled +2ee already set but non-zonable; second query/update
            object.zoning_enabled_2ee = 1; object.in_zone_2f0 = 0x80;
            context.raw[0] = 0; context.raw[1] = 1; context.raw_count = 2;
            break;
        case 11: // sync query mutates fresh zoning/in-zone bytes before selection
            object.visual_2d8 = &visual_a; object.zoning_enabled_2ee = 1;
            object.in_zone_2f0 = 1; context.raw[0] = 1; context.raw[1] = 1;
            context.raw_count = 2; context.mutate = 4;
            break;
        case 12: // injected service error after zoning was enabled
            object.room_zone_2f4 = kRoomA; context.fail = true;
            break;
        case 13: // opaque identity remains full-width in the adapter
            object.identity = static_cast<Address>(0x100000000ull) + kObject;
            visual_a.owner_identity = object.identity;
            object.zoning_enabled_2ee = 1; context.raw[0] = 0;
            context.raw_count = 1;
            break;
        case 14: // source LDRB boundary is rejected if the adapter returns >255
            object.room_zone_2f4 = kRoomA; context.override_room_raw = true;
            context.room_raw_override = 0x100; context.raw[0] = 0;
            context.raw_count = 1;
            break;
        case 15: // any nonzero in-zone byte selects visible
            object.visual_2d8 = &visual_a; object.zoning_enabled_2ee = 1;
            object.in_zone_2f0 = 0x80; context.raw[0] = 1; context.raw_count = 1;
            break;
        case 16: // virtual setUpdating target was captured before IsZonable
            object.zoning_enabled_2ee = 1; context.raw[0] = 1;
            context.raw_count = 1; context.mutate = 5;
            break;
        case 17: // SyncVisibility returns when VisualObject+4 is null
            visual_a.owner_identity = 0;
            break;
        default: std::abort();
        }
    }
};

void print(const Fixture& f, Status status) {
    std::printf("{\"status\":%d,\"calls\":[", static_cast<int>(status));
    for (std::size_t i = 0; i < f.context.calls.size(); ++i) {
        const auto& call = f.context.calls[i];
        if (i) std::putchar(',');
        std::printf("[%u,%llu,%llu,%u]", static_cast<unsigned>(call.operation),
            static_cast<unsigned long long>(call.object),
            static_cast<unsigned long long>(call.related), call.value);
    }
    std::printf("],\"zoning\":%u,\"in_zone\":%u,\"room\":%llu,\"last_update\":%u,\"last_visible\":%u,\"service_calls\":%u}\n",
        static_cast<unsigned>(f.object.zoning_enabled_2ee),
        static_cast<unsigned>(f.object.in_zone_2f0),
        static_cast<unsigned long long>(f.object.room_zone_2f4),
        f.result.last_updating_argument, f.result.last_visible_argument, f.result.service_calls);
}
} // namespace

int main(int argc, char** argv) {
    if (argc != 3) return 2;
    if (argv[1][0] == 'g') {
        Fixture base(0);
        auto calls = [&] { return base.context.calls.size(); };
        auto status = disable_zoning(reinterpret_cast<GameObject*>(
            reinterpret_cast<unsigned char*>(&base.object) + 1), &base.services, &base.result);
        if (status != Status::invalid_argument || calls() != 0) return 10;
        status = disable_zoning(&base.object, &base.services,
                                reinterpret_cast<Result*>(&base.object));
        if (status != Status::invalid_argument || calls() != 0) return 11;
        unsigned char bad_services_storage[sizeof(Services) + alignof(Services)]{};
        auto* misaligned_services = reinterpret_cast<Services*>(bad_services_storage + 1);
        status = disable_zoning(&base.object, misaligned_services, &base.result);
        if (status != Status::invalid_argument || calls() != 0) return 17;
        Services alias_context{&base.object, sizeof(base.object), &invoke};
        status = disable_zoning(&base.object, &alias_context, &base.result);
        if (status != Status::invalid_argument || calls() != 0) return 12;
        Services overflow_context{reinterpret_cast<void*>(
            std::numeric_limits<std::uintptr_t>::max() - 15u), 64u, &invoke};
        status = disable_zoning(&base.object, &overflow_context, &base.result);
        if (status != Status::invalid_argument || calls() != 0) return 18;
        unsigned char visual_storage[sizeof(VisualObject) + alignof(VisualObject)]{};
        auto* misaligned_visual = reinterpret_cast<VisualObject*>(visual_storage + 1);
        status = sync_visibility(misaligned_visual, &base.object, &base.services, &base.result);
        if (status != Status::invalid_argument || calls() != 0) return 13;
        Services visual_context{&base.visual_a, sizeof(base.visual_a), &invoke};
        status = sync_visibility(&base.visual_a, &base.object, &visual_context, &base.result);
        if (status != Status::invalid_argument || calls() != 0) return 19;
        Fixture bad_graph(7); bad_graph.visual_a.owner_identity = kRoomA;
        status = sync_visibility(&bad_graph.visual_a, &bad_graph.object,
                                 &bad_graph.services, &bad_graph.result);
        if (status != Status::invalid_source_fact || !bad_graph.context.calls.empty()) return 20;
        Fixture bad_room(14);
        status = enable_zoning(&bad_room.object, &bad_room.services, &bad_room.result);
        if (status != Status::invalid_source_fact || bad_room.object.zoning_enabled_2ee != 1 ||
            bad_room.context.calls.empty()) return 14;
        Fixture high_identity(13);
        status = disable_zoning(&high_identity.object, &high_identity.services, &high_identity.result);
        if (status != Status::complete || high_identity.context.calls.empty() ||
            high_identity.context.calls.front().object != high_identity.object.identity) return 15;
        Fixture throws(0); throws.context.throw_on_update = true;
        status = disable_zoning(&throws.object, &throws.services, &throws.result);
        if (status != Status::service_failed || throws.object.zoning_enabled_2ee != 0 ||
            throws.context.calls.empty()) return 16;
        Fixture failed_enrollment(12);
        status = enable_zoning(&failed_enrollment.object, &failed_enrollment.services,
                               &failed_enrollment.result);
        if (status != Status::service_failed || failed_enrollment.object.zoning_enabled_2ee != 1 ||
            failed_enrollment.context.calls.size() != 2) return 21;
        std::puts("{\"host_guard_cases\":12}");
        return 0;
    }
    const unsigned scenario = static_cast<unsigned>(std::strtoul(argv[2], nullptr, 0));
    Fixture fixture(scenario);
    Status status = Status::invalid_argument;
    if (argv[1][0] == 'd') status = disable_zoning(&fixture.object, &fixture.services, &fixture.result);
    else if (argv[1][0] == 'e') status = enable_zoning(&fixture.object, &fixture.services, &fixture.result);
    else if (argv[1][0] == 's') status = sync_visibility(&fixture.visual_a,
        scenario == 17 ? nullptr : &fixture.object, &fixture.services, &fixture.result);
    else return 2;
    print(fixture, status);
    return 0;
}
