#include "source_random_lifecycle_v1.hpp"

#include <string>

namespace dh2::random_lifecycle {
namespace {

dh2_random_state& owned_state() noexcept {
    // Value initialization mirrors the original globals' zero-initialized
    // process state. Runtime lifecycle reseeds below only change the seeds.
    static dh2_random_state streams{{0, 0}, {0, 0}};
    return streams;
}

void reseed(std::uint32_t real_time_ms) noexcept {
    auto& streams = owned_state();
    streams.seeds[0] = real_time_ms;
    streams.seeds[1] = 0;
}

bool inventory_next(void* context, std::int32_t bound, std::uint32_t stream,
                    std::int32_t& value, std::string& error) {
    if (context != &process_state()) {
        error = "Inventory RNG must borrow the original process state";
        return false;
    }
    value = dh2_random_next(static_cast<dh2_random_state*>(context),
                            static_cast<std::uint32_t>(bound), stream);
    error.clear();
    return true;
}

}  // namespace

dh2_random_state& process_state() noexcept {
    return owned_state();
}

data::InventoryRandomServiceV4 inventory_random_service() noexcept {
    return {&process_state(), inventory_next};
}

void seed_from_gsinit_update(std::uint32_t real_time_ms) noexcept {
    reseed(real_time_ms);
}

void seed_from_level_unload(std::uint32_t real_time_ms) noexcept {
    reseed(real_time_ms);
}

}  // namespace dh2::random_lifecycle
