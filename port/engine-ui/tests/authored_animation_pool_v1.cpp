#include "../authored_animation_pool_v1.hpp"

#include <cstdio>

int main() {
    using namespace dh2::ui::authored_animation_pool_v1;
    State state;
    StylePool style;
    std::size_t context{}, clone{};
    bool create{};

    for (std::uint32_t i = 0; i < clones_per_style; ++i) {
        if (!acquire(state, style, 17, context, clone, create) ||
            context != i || clone != i || !create) {
            std::fprintf(stderr, "first eight calls must create clones 0..7\n");
            return 1;
        }
    }
    if (!acquire(state, style, 17, context, clone, create) ||
        context != 8 || clone != saturated_clone_slot || create) {
        std::fprintf(stderr, "full style pool must reuse existing clone 7\n");
        return 1;
    }

    // The source has twelve global contexts, independent of the style pool.
    StylePool other_style;
    for (std::size_t i = 9; i < playback_context_count; ++i) {
        if (!acquire(state, other_style, 18, context, clone, create) ||
            context != i) {
            std::fprintf(stderr, "global contexts must allocate through 11\n");
            return 1;
        }
    }
    if (acquire(state, other_style, 18, context, clone, create)) {
        std::fprintf(stderr, "thirteenth active context must fail closed\n");
        return 1;
    }
    if (!stop(state, style, 8) ||
        !acquire(state, style, 17, context, clone, create) ||
        context != 8 || clone != 7 || create) {
        std::fprintf(stderr, "retirement must free a context and reuse its retained clone\n");
        return 1;
    }
    std::puts("PASS authored animation pools: 12 contexts, 8 per-style clones, saturated slot 7 reuse");
    return 0;
}
