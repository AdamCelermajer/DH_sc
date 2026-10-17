#pragma once

#include <cstdint>

namespace dh2::character::aggro_search {

// These are borrowed adapter projections, not overlays of GameObject or
// Character memory. `position` and `target_position` are the values observed
// from the corresponding source getters for this query.
struct GameObject {
    std::uintptr_t identity;
    float position[3];
    float target_position[3];
    float forward[3];
    std::uint8_t visible;
    std::uint8_t has_target_position;
    std::uint8_t character_2ee;
    std::uint8_t character_2f0;
};

struct Character {
    std::uintptr_t identity;
    GameObject* object;
    std::int32_t source_word_1310;
    std::int32_t source_word_1314;
    // Source Objective_TalkToNPC marker state on this canonical Character
    // projection. These are semantic byte values, not offsets into this
    // adapter struct or a second marker registry.
    std::uint8_t talk_objective_marker_2fa{};
    std::uint8_t talk_objective_priority_marker_2fb{};
};

struct ObjectEntry { ObjectEntry* next; GameObject* object; };
struct Room { Room* next; ObjectEntry* objects; };
struct RoomRegistry { Room* rooms; };  // circular sentinel list

struct TargetInfo {
    std::uintptr_t object_identity;
    std::uintptr_t character_identity;
    float distance;
    float angle;
    std::uint32_t flags;
    std::uint32_t reserved;
};

struct TargetList {
    TargetInfo* heap;
    std::uint32_t count;
    std::uint32_t capacity;
    Character* owner;
    std::uint32_t sort_type;
    std::uint32_t reserved;
};

// Source services that are still actor-owned or virtual. A normal return of
// zero means success; nonzero means unavailable/error at the adapter boundary.
// Successful responses return source results in `word`/`number`. All identities are source actor
// identities, never distance-derived IDs.
struct Request {
    std::uint32_t operation;
    std::uint32_t reserved;
    std::uintptr_t subject;
    std::uintptr_t other;
};
struct Response {
    std::uintptr_t word;
    float number;
    std::uint32_t reserved;
};
struct Services {
    void* context;
    int (*invoke)(void*, const Request*, Response*);
};

enum Operation : std::uint32_t {
    resolve_character = 1,      // response.word: borrowed Character*
    is_zonable,                  // vtable +0xc4, called with candidate GameObject*
    is_interactive,              // vtable +0x88(candidate, owner)
    interaction_radius,          // vtable +0x94 on candidate GameObject*
    ai_melee_radius              // source CharAI::AI_GetMeleeRadius()
};

enum Status : int {
    complete = 0,
    invalid_argument = 1,
    source_service_failed = 2,
    capacity_exhausted = 3,
    invalid_topology = 4
};

inline constexpr std::uint32_t kSourceFlags = 0x7fffffffU;
inline constexpr std::uint32_t kSourceObjectFilter = 2U;
inline constexpr std::uint32_t kSourceSortClosest = 1U;

// Reconstruct only the source call used by CharAI::_UpdateAggro:
// TargetList(owner, 0x7fffffff, 2, 1). The source's flag-0x7fffffff
// Character path still applies owner.word1314 >= candidate.word1310 but
// bypasses the narrower dead/enemy/player/group filters. Object filter 2
// admits character objects only. Owner and all borrowed registry objects must
// remain live for the synchronous call.
extern "C" int dh2_aggro_target_list_init(TargetList*, TargetInfo*,
                                            std::uint32_t capacity,
                                            Character* owner);
extern "C" int dh2_aggro_target_search(TargetList*, const RoomRegistry*,
                                         float view_radius, float cone,
                                         const Services*);
extern "C" int dh2_aggro_target_pop(TargetList*, TargetInfo*);

static_assert(sizeof(GameObject) == 48);
static_assert(sizeof(Character) == 32);
static_assert(sizeof(ObjectEntry) == 16);
static_assert(sizeof(Room) == 16);
static_assert(sizeof(TargetInfo) == 32);
static_assert(sizeof(Request) == 24);
static_assert(sizeof(Response) == 16);

}  // namespace dh2::character::aggro_search
