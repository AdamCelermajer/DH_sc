#pragma once

#include <cstdint>

namespace dh2::character_zonability {

// Full-width borrowed Character identity. This is not an ARM object overlay.
struct State { std::uintptr_t character; };

enum class Operation : std::uint32_t { is_player, is_faerie };
struct Request {
    Operation operation;
    std::uint32_t reserved;
    std::uintptr_t character;
};
struct Response { std::uint32_t word; };
struct Services {
    void* context;
    // Zero means the source query completed. The provider can compose the
    // recovered Character classification getters without copying their data.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};

enum class Decision : std::uint32_t { incomplete, player, faerie, base_condition };
struct Result {
    Decision decision;
    std::uint32_t service_calls;
    std::uint32_t is_player_word;
    std::uint32_t is_faerie_word;
    std::uint32_t zonable;
    std::uint32_t reserved;
    std::uintptr_t captured_character;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3,
};

// Reconstructs Character::IsZonable's exact caller order: virtual IsPlayer;
// if false, Character::IsFaerie; if false, GameObject::MeetCondition. The
// latter pinned source leaf returns one. Thus players/faeries are not zonable;
// all other Characters reach the constant-true base condition. `State`'s
// identity is captured before callbacks and reused, matching the source's
// saved `this` register even if a provider updates the live projection.
// Classification provider callbacks are synchronous and may retain effects on
// failure. The caller must check Status before consuming `zonable`.
Status evaluate(State*, const Services*, Result*);

static_assert(sizeof(State) == 8);
static_assert(sizeof(Request) == 16);
static_assert(sizeof(Response) == 4);
static_assert(sizeof(Services) == 16);
static_assert(sizeof(Result) == 32);

}  // namespace dh2::character_zonability

extern "C" int dh2_character_zonability_evaluate(
    dh2::character_zonability::State*, const dh2::character_zonability::Services*,
    dh2::character_zonability::Result*);
