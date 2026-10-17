#pragma once

#include <cstdint>

namespace dh2::character_faery_placement_v1 {

// Opaque borrowed identities. PlayerManager, its Character list, Players,
// Characters, AI state and all backing data remain owned by the caller.
using Identity = std::uintptr_t;

struct Vector3 { float x = 0.0f, y = 0.0f, z = 0.0f; };

enum class Operation : std::uint32_t {
    list_begin,
    list_end,
    list_value,
    list_next,
    is_faery,
    is_follower,
    online_state,
    hosting_player,
    hosting_player_for_follower_state,
    is_local_player_hosting,
    local_player,
    player_character,
    follower_host_state,
    look_at_vector,
    target_position,
    set_position,
    force_update_position,
    disable_zoning,
    player_count,
    player_at,
    set_player_faery,
    previous_ai_master,
    set_ai_master,
    current_faery_id,
    change_faery,
};

struct Request {
    Operation operation{};
    // Opaque borrowed source identities; zero represents a null source pointer.
    Identity subject = 0;
    Identity argument = 0;
    // Numeric source index plus a signed scalar/flag. For example, both
    // GetLocalPlayer calls use index=0,value=1; current-faery uses value=-1.
    std::uint32_t index = 0;
    const Vector3* vector = nullptr;
    std::int32_t value = 0;
};

struct Reply {
    Identity identity = 0;
    std::int32_t word = 0;
    Vector3 vector{};
};

// Every service is a synchronous borrowed callback into the already-live game
// graph. list_begin/end/value/next expose the existing intrusive Character
// list; callbacks must not replace its owner or synthesize Characters.
struct Services {
    void* context = nullptr;
    int (*invoke)(void*, const Request&, Reply*) = nullptr;
};

struct Result {
    std::uint32_t visited = 0;
    std::uint32_t skipped_unclassified = 0;
    std::uint32_t skipped_null_player = 0;
    std::uint32_t placed_followers = 0;
    std::uint32_t placed_faeries = 0;
    std::uint32_t player_faery_links = 0;
    std::uint32_t callbacks = 0;
    Operation last_operation = Operation::list_begin;
    Identity last_character = 0;
    Identity chosen_player_character = 0;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    source_guard,
};

// Source orchestration for Level::PlaceFaeryAndFollowers @ 0x3f0898.
// Explicit player takes precedence; otherwise online sessions use the host
// Player, offline sessions use GetLocalPlayer(0,1). On a remote-host session,
// the host is queried again for the follower-state read, matching the two
// GetHostingPlayer calls in the original. A null chosen Player Character skips
// that list item before any positioning. The list advances only after each
// item's full source path or explicit early-continue path.
class Runtime {
public:
    explicit Runtime(Services services = {});
    Status place(Identity explicit_player_character, Result* result);

private:
    Services services_;
    bool busy_ = false;
};

} // namespace dh2::character_faery_placement_v1
