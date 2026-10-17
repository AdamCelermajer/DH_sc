#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace dh2::spawn_group_runtime_v1 {

struct SpawnInfo {
    std::int32_t character_id{};
    std::int32_t probability{};
    std::int32_t quantity{};
};

struct GroupDefinition {
    bool active_spot_only{};
    std::int32_t delay_ms{};
    std::vector<SpawnInfo> entries;
};

struct Spot {
    std::uintptr_t handle{};
    bool interactive{};
    bool zoning_enabled{};
    bool in_zone{};
    float position[3]{};
};

// Random::GetRandom is injected so this policy kernel does not introduce a
// second random stream. The callback must return a value in [0, upper_bound).
using RandomBelow = bool (*)(void*, std::uint32_t upper_bound,
                             std::uint32_t* value);
using CreateCharacter = std::uintptr_t (*)(void*, const char* source,
                                           const char* type, const char* name,
                                           bool skip_init_post);
using InitSpawned = void (*)(void*, std::uintptr_t actor,
                             std::int32_t character_id,
                             const float position[3]);
using PlaceObject = void (*)(void*, std::uintptr_t spot,
                             std::uintptr_t actor);

struct Services {
    void* context{};
    RandomBelow random_below{};
    CreateCharacter create_character{};
    InitSpawned init_spawned{};
    PlaceObject place_object{};
};

enum class Status : std::uint8_t { complete, invalid_argument };

class Owner {
public:
    // Replacing a source group resets its manager timer and spot list, matching
    // a fresh manager registration for that group id.
    void define_group(std::int32_t group_id, GroupDefinition definition);
    bool add_spot(std::int32_t group_id, const Spot& spot);
    void remove_spot(std::uintptr_t spot_handle);

    Status update(std::int32_t dt_ms, const Services* services);
    std::int32_t timer_ms(std::int32_t group_id) const noexcept;
    std::size_t group_count() const noexcept { return groups_.size(); }

private:
    struct GroupState {
        GroupDefinition definition;
        std::int32_t timer_ms{};
        std::vector<Spot> spots;
    };
    std::map<std::int32_t, GroupState> groups_;
};

} // namespace dh2::spawn_group_runtime_v1
