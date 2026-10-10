#pragma once

// P16 SPAWN: generic "spawn a character from a template at a position".
//
// Original rules (IDA, pseudocode-all.c): GameObject::_Summon 0x39193c creates
// the NPC through Character::CreateNPC, places it (SetInitialPosition +
// SetPosition + SetRotation of the caller), adds it to the caller's room (or the
// no-room manager), and with spawn=true runs CharStateMachine::SM_SetSpawnState
// (false,false): the PreSpawn17 -> Spawn1 transition whose Spawn clip plays and
// whose end moves to Idle3, where combat is admitted.
//
// This owner does the port-side equivalent without a second actor registry:
//   1. resolve a CharacterTable row (directly, or a Charater_Templates row by a
//      caller-owned weighted draw) to its actor profile and monster level;
//   2. take a free SLOT from a pool that was admitted into the same
//      ActorPopulation / CombatSession / OriginalActorLifecycle before the
//      session was built (CombatSession has no runtime add API; see report);
//   3. place it, then drive the same lifecycle the authored population uses.
// Combat enablement, the spawn animation and AI are the existing owners' gates.

#include "../../actor_definitions.hpp"
#include "../../../game-data/character_templates_v78.hpp"
#include "../../../game-data/data.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::spawn {

// Stable IDs for pool slots. The population rejects any declaration whose ID is
// already admitted, so a slot can never shadow an authored actor.
constexpr std::uint64_t pool_stable_id_base = 0x5350574E00000000ull; // "SPWN" high word

enum class SpawnClipPolicy : std::uint8_t {
    // Source Summon(spawn=true) / Script_SpawnCharacter: PreSpawn17 -> Spawn1,
    // Spawn clip, combat after the whole sequence completes (Idle3).
    source_spawn_state,
    // Source Summon(spawn=false): no spawn clip; the actor goes to Idle3 directly.
    none,
};

struct SpawnRequestV1 {
    // CharacterTable row name (direct) or Charater_Templates row name (weighted
    // draw through the caller's shared RNG). Row names double as profile IDs.
    std::string name;
    std::array<float, 3> position{};   // absolute world position (caller resolves summon_spot/offsets)
    float heading_radians = 0.0f;      // Euler Z, as GameObject::SetRotation receives it
    SpawnClipPolicy clip = SpawnClipPolicy::source_spawn_state;
    std::uint64_t summoner = 0;        // owner actor ID, 0 for none
    std::int32_t host_level_raw = 256; // host player level in source fixed point (256 = 1.0)
};

// Result of resolving a request. Pure: no owner was touched.
struct SpawnPlanV1 {
    std::string profile_id;            // CharacterTable row name selected for this spawn
    std::int32_t character_row = -1;   // CharacterTable row index
    std::int32_t template_row = -1;    // Charater_Templates row index, -1 for a direct row
    std::int32_t level_raw = -1;       // monster level in fixed point; -1 keeps the profile's base level
    std::int32_t faction = -1;         // CharacterTable AIFaction (raw)
};

// --spawn-test TEMPLATE@X,Y,Z@FRAME. Debug only; absent by default.
struct SpawnTestRequestV1 {
    std::string name;
    std::array<float, 3> position{};
    std::int64_t frame = 0;
};
bool parse_spawn_test_v1(const std::string& text, SpawnTestRequestV1& output, std::string& error);

// Profile IDs a request can resolve to. A direct row gives one profile; a
// template gives every distinct weighted member (needed to reserve slots before
// the draw happens at spawn time).
bool spawn_candidate_profiles_v1(const std::string& name, const dh2::data::CharacterTable& characters,
                                 const dh2::data::CharacterTemplateTableV78& templates,
                                 std::vector<std::string>& profiles, std::string& error);

// Level scaling from the row's own LevelMin/LevelMax/LevelOffset and the host
// player level, through the same monster-level policy the enemy AI uses
// (resolve_runtime_monster_level_v1). Rows with LevelMax == 0 are not scaled.
bool spawn_level_raw_v1(const dh2::data::CharacterTable& characters, const std::string& row_name,
                        std::int32_t host_level_raw, std::int32_t& level_raw, std::string& error);

// ---------------------------------------------------------------------------
// Pool of admitted slots. Slots exist before the CombatSession is built, so
// that the same population/session/lifecycle owners admit them. Each slot is
// one actor; a busy slot is never handed out again (no duplicates).
// Transient: pool state is never persisted; a reload re-declares every slot
// free, so a reloaded world cannot contain a duplicate summon.
struct SpawnPoolSlotV1 {
    std::uint64_t stable_id = 0;
    std::string profile_id;
    std::int32_t level_raw = -1;
    std::uint64_t summoner = 0;
    bool busy = false;
    // Set when a spawn failed after the owner was touched: the slot stays busy
    // because its actor state is unknown.
    bool failed = false;
};

class SpawnPoolV1 {
public:
    // Reserves `count` slots for one profile at a fixed monster level.
    bool reserve(const std::string& profile_id, std::uint32_t count, std::int32_t level_raw, std::string& error);
    // Declarations for ActorPopulation::admit_declared, in slot order.
    std::vector<ActorDefinition> declarations(const std::string& level_uri) const;
    const std::vector<SpawnPoolSlotV1>& slots() const noexcept { return slots_; }
    bool owns(std::uint64_t stable_id) const noexcept;
    const SpawnPoolSlotV1* slot(std::uint64_t stable_id) const noexcept;
    bool acquire(const std::string& profile_id, std::uint64_t summoner, std::uint64_t& stable_id, std::string& error);
    // Frees a slot after despawn. Failed slots are not freed here.
    bool release(std::uint64_t stable_id, std::string& error);
    // Marks a busy slot whose owner state became unknown after begin().
    void mark_failed(std::uint64_t stable_id) noexcept;
    std::size_t busy_count() const noexcept;
    bool empty() const noexcept { return slots_.empty(); }
private:
    std::vector<SpawnPoolSlotV1> slots_;
};

// ---------------------------------------------------------------------------
// Owner effects. Each callback must act on the SAME actor the population
// admitted and the same session/lifecycle as authored population.
struct SpawnServicesV1 {
    const dh2::data::CharacterTable* characters = nullptr;
    const dh2::data::CharacterTemplateTableV78* templates = nullptr;
    // Caller-owned shared source RNG draw in [0, bound).
    std::function<bool(std::int32_t bound, std::int32_t& index, std::string& error)> random_index;
    // True when the profile is admitted in the session: it has an actor profile,
    // an explicit combat policy and a loaded visual. Without it a spawn is rejected.
    std::function<bool(const std::string& profile_id, std::string& error)> profile_available;
    // Transform + native body placement (same effect as lifecycle restore_initial_position).
    std::function<bool(std::uint64_t actor, const std::array<float, 3>& position, float heading, std::string& error)> place;
    // Starts the clip policy: source_spawn_state -> lifecycle spawn (Spawn clip,
    // combat after completion); none -> lifecycle put_idle.
    std::function<bool(std::uint64_t actor, SpawnClipPolicy clip, std::string& error)> begin;
    // Despawn: lifecycle put_limbus (hidden, no combat).
    std::function<bool(std::uint64_t actor, std::string& error)> hide;
    std::function<void(const std::string& line)> log;
};

struct SpawnResultV1 {
    bool spawned = false;
    std::uint64_t actor = 0;
    SpawnPlanV1 plan;
    std::string line;   // exactly one log line per attempt
    std::string error;
};

// Resolves the request, acquires a free slot, places it and starts its clip
// policy. Every failure leaves owners untouched, except a failure after begin
// (reported as failed; the slot stays busy).
bool spawn_character_v1(SpawnPoolV1& pool, const SpawnRequestV1& request,
                        const SpawnServicesV1& services, SpawnResultV1& result, std::string& error);

bool despawn_character_v1(SpawnPoolV1& pool, std::uint64_t actor, const SpawnServicesV1& services,
                          std::string& error);

} // namespace dh::foundation::spawn
