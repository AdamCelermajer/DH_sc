#include "../frustum_bounds.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace dh2::engine_camera::frustum_bounds;

namespace {
struct Context {
    Frustum* frustum = nullptr;
    Word points[4][3]{};
    std::size_t intersections = 0;
    std::size_t greater_calls = 0;
    std::size_t less_calls = 0;
    char events[64]{};
    std::size_t event_count = 0;
    bool bad_planes = false;
    bool mutate_service_table = false;
    bool try_same_frustum_reentry = false;
    bool reentry_attempted = false;
    std::size_t replacement_calls = 0;
    Status reentry_status = Status::complete;
    Services* mutable_services = nullptr;
};

void event(Context& state, char value) {
    if (state.event_count >= sizeof(state.events)) std::abort();
    state.events[state.event_count++] = value;
}

Word intersect(void* opaque, const Word* first, const Word* second,
               const Word* third, Word output[3]) {
    auto& state = *static_cast<Context*>(opaque);
    constexpr std::size_t expected[4][3] = {{0, 5, 2}, {0, 5, 3}, {0, 4, 2}, {0, 4, 3}};
    const auto plane_index = [&state](const Word* p) -> std::size_t {
        const auto base = reinterpret_cast<Address>(&state.frustum->planes[0][0]);
        const auto value = reinterpret_cast<Address>(p);
        if (value < base || value >= base + sizeof(state.frustum->planes) ||
            (value - base) % sizeof(state.frustum->planes[0]) != 0) return 99;
        return (value - base) / sizeof(state.frustum->planes[0]);
    };
    const std::size_t call = state.intersections++;
    if (call >= 4 || plane_index(first) != expected[call][0] ||
        plane_index(second) != expected[call][1] ||
        plane_index(third) != expected[call][2]) state.bad_planes = true;
    event(state, 'i');
    if (state.try_same_frustum_reentry && !state.reentry_attempted) {
        state.reentry_attempted = true;
        state.reentry_status = recalculate(state.frustum, state.mutable_services);
    }
    if (state.mutate_service_table && call == 0)
        state.mutable_services->greater = [](void* inner, Word, Word) -> Word {
            auto& context = *static_cast<Context*>(inner);
            ++context.replacement_calls;
            event(context, 'x');
            return 1;
        };
    if (call < 4) std::memcpy(output, state.points[call], sizeof(state.points[call]));
    return static_cast<Word>(call & 1u); // Source ignores this helper result.
}

float as_float(Word bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

Word greater(void* opaque, Word left, Word right) {
    auto& state = *static_cast<Context*>(opaque);
    ++state.greater_calls;
    event(state, 'g');
    return static_cast<Word>(as_float(left) > as_float(right));
}

Word less(void* opaque, Word left, Word right) {
    auto& state = *static_cast<Context*>(opaque);
    ++state.less_calls;
    event(state, 'l');
    return static_cast<Word>(as_float(left) < as_float(right));
}

Word byte_context_intersect(void* opaque, const Word*, const Word*,
                            const Word*, Word output[3]) {
    auto* bytes = static_cast<unsigned char*>(opaque);
    ++bytes[0];
    output[0] = output[1] = output[2] = 0;
    return 0;
}

Word byte_context_greater(void* opaque, Word, Word) {
    auto* bytes = static_cast<unsigned char*>(opaque);
    ++bytes[1];
    return 0;
}

Word byte_context_less(void* opaque, Word, Word) {
    auto* bytes = static_cast<unsigned char*>(opaque);
    ++bytes[2];
    return 0;
}

Services services(Context& context) {
    return {&context, sizeof(context), &intersect, &greater, &less};
}

bool untouched(const Frustum& frustum, Word word) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&frustum);
    const auto expected = static_cast<unsigned char>(word & 0xffu);
    for (std::size_t i = 0; i != sizeof(frustum); ++i)
        if (bytes[i] != expected) return false;
    return true;
}

int guards() {
    Frustum frustum{};
    std::memset(&frustum, 0x5a, sizeof(frustum));
    constexpr Word sentinel = 0x5a5a5a5au;
    Context context;
    context.frustum = &frustum;
    auto service = services(context);

    if (recalculate(nullptr, &service) != Status::invalid_argument ||
        !untouched(frustum, sentinel)) return 10;
    alignas(Frustum) unsigned char storage[sizeof(Frustum) + alignof(Frustum)]{};
    auto* misaligned = reinterpret_cast<Frustum*>(storage + 1);
    if (recalculate(misaligned, &service) != Status::invalid_argument ||
        !untouched(frustum, sentinel)) return 11;
    alignas(Services) unsigned char service_storage[sizeof(Services) + alignof(Services)]{};
    auto* misaligned_services = reinterpret_cast<const Services*>(service_storage + 1);
    if (recalculate(&frustum, misaligned_services) != Status::invalid_argument ||
        !untouched(frustum, sentinel)) return 12;
    auto* overlapping_services = reinterpret_cast<const Services*>(&frustum);
    if (recalculate(&frustum, overlapping_services) != Status::invalid_argument ||
        !untouched(frustum, sentinel)) return 13;
    auto bad = service;
    bad.context = &frustum;
    bad.context_extent = sizeof(frustum);
    if (recalculate(&frustum, &bad) != Status::invalid_argument ||
        !untouched(frustum, sentinel)) return 14;
    bad = service;
    bad.intersect_three_planes = nullptr;
    if (recalculate(&frustum, &bad) != Status::invalid_argument ||
        !untouched(frustum, sentinel)) return 15;

    Frustum valid{};
    Context valid_context;
    valid_context.frustum = &valid;
    valid_context.points[0][0] = 0x3f800000u;
    auto valid_service = services(valid_context);
    valid_context.mutable_services = &valid_service;
    if (recalculate(&valid, &valid_service) != Status::complete ||
        valid_context.intersections != 4 || valid_context.greater_calls != 12 ||
        valid_context.less_calls != 12 || valid_context.bad_planes ||
        valid.box_max[0] != 0x3f800000u) return 16;

    Frustum guarded{};
    Context guarded_context;
    guarded_context.frustum = &guarded;
    guarded_context.mutate_service_table = true;
    guarded_context.try_same_frustum_reentry = true;
    auto guarded_services = services(guarded_context);
    guarded_context.mutable_services = &guarded_services;
    if (recalculate(&guarded, &guarded_services) != Status::complete ||
        guarded_context.reentry_status != Status::reentrant_call ||
        guarded_context.intersections != 4 || guarded_context.greater_calls != 12 ||
        guarded_context.less_calls != 12 || guarded_context.replacement_calls != 0 ||
        guarded_context.event_count != 28 || guarded_context.bad_planes) return 17;

    Frustum byte_frustum{};
    unsigned char raw_context[8]{};
    auto* unaligned_context = raw_context + 1;
    const Services byte_services{unaligned_context, 4, &byte_context_intersect,
                                 &byte_context_greater, &byte_context_less};
    if (reinterpret_cast<Address>(unaligned_context) % alignof(Services) == 0 ||
        recalculate(&byte_frustum, &byte_services) != Status::complete ||
        unaligned_context[0] != 4 || unaligned_context[1] != 12 ||
        unaligned_context[2] != 12 || unaligned_context[3] != 0) return 18;

    std::puts("{\"host_guard_cases\":9,\"service_calls\":[4,12,12],\"same_frustum_reentry\":\"rejected\",\"service_table_snapshot\":\"stable\",\"unaligned_byte_context\":\"accepted\"}");
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    if (argc == 2 && std::strcmp(argv[1], "guards") == 0) return guards();
    if (argc != 47 || std::strcmp(argv[1], "bounds") != 0) return 2;
    Frustum frustum{};
    Word input_words[sizeof(frustum) / sizeof(Word)]{};
    for (std::size_t i = 0; i != sizeof(input_words) / sizeof(input_words[0]); ++i)
        input_words[i] = static_cast<Word>(std::strtoul(argv[i + 2], nullptr, 16));
    std::memcpy(&frustum, input_words, sizeof(frustum));
    Context state;
    state.frustum = &frustum;
    for (std::size_t i = 0; i != 4; ++i)
        for (std::size_t axis = 0; axis != 3; ++axis)
            state.points[i][axis] = static_cast<Word>(
                std::strtoul(argv[35 + i * 3 + axis], nullptr, 16));
    const auto service = services(state);
    const auto result = recalculate(&frustum, &service);
    std::printf("{\"status\":%d,\"bounds\":[", static_cast<int>(result));
    for (std::size_t i = 0; i != 3; ++i)
        std::printf("%s%u", i ? "," : "", frustum.box_min[i]);
    for (std::size_t i = 0; i != 3; ++i)
        std::printf(",%u", frustum.box_max[i]);
    std::printf("],\"calls\":[%zu,%zu,%zu],\"events\":\"%.*s\",\"bad_planes\":%s}\n",
        state.intersections, state.greater_calls, state.less_calls,
        static_cast<int>(state.event_count), state.events,
        state.bad_planes ? "true" : "false");
    return result == Status::complete && !state.bad_planes ? 0 : 1;
}
