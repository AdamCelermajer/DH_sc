#pragma once

#include "character_ai_events.hpp"
#include "character_ai_relations.hpp"
#include "character_ai_set_target.hpp"
#include "character_aggro_candidate_events.hpp"
#include "character_aggro_character_list.hpp"
#include "character_aggro_target_search.hpp"
#include "character_controller_commands.hpp"
#include "character_enemy_spotted.hpp"
#include "character_path_commands.hpp"
#include "character_script_lifecycle.hpp"
#include "monster_external_script_session.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace dh2::ghost_ai_session {

// Game-provided Character methods/data used by the original `monster` Lua
// bodies. Each callback reads live state and returns 0 on success. These are
// typed source boundaries, not synthesized character facts.
struct ScriptQueries {
    void* context;
    std::int32_t (*get_py_struct)(void*, const char* category, const char* member,
                                  std::int32_t* value);
    std::int32_t (*get_prop)(void*, std::uintptr_t owner, std::int32_t property,
                             float* raw_fixed_value);
    std::int32_t (*get_py_constant)(void*, const char* category, const char* member,
                                    std::int32_t* value);
    std::int32_t (*has_target)(void*, std::uintptr_t owner, std::uint32_t* value);
    std::int32_t (*get_target)(void*, std::uintptr_t owner, std::uintptr_t* target);
    std::int32_t (*get_state)(void*, std::uintptr_t owner, std::int32_t* state);
    std::int32_t (*has_path)(void*, std::uintptr_t owner, std::uint32_t* value);
    std::int32_t (*get_py_oid)(void*, const char*, const char*, std::int32_t*) = nullptr;
    std::int32_t (*get_position)(void*, std::uintptr_t, float[3]) = nullptr;
    std::int32_t (*get_host_player_level)(void*, std::int32_t*) = nullptr;
    std::int32_t (*get_host_player_difficulty)(void*, std::int32_t*) = nullptr;
    std::int32_t (*get_current_level_range)(void*, const float*, std::int32_t[2], std::uint32_t*) = nullptr;
    std::int32_t (*set_level)(void*, std::uintptr_t, float) = nullptr;
    std::int32_t (*stop)(void*, std::uintptr_t owner) = nullptr;
    std::int32_t (*attack)(void*, std::uintptr_t owner, std::uintptr_t target) = nullptr;
};

// Stable actor-owned composition inputs. Every pointed-to state projection and
// service context is borrowed; the owner keeps them live while bound. Service
// tables are copied, but callback contexts remain borrowed. This layer owns
// only the per-AIS Lua Session and routes exact source kernels between the
// borrowed projections.
struct Bindings {
    std::uintptr_t ai_identity;
    std::uintptr_t owner_identity;
    std::uintptr_t active_ais_identity;
    std::uintptr_t active_ais_callee;

    dh2::character_enemy_spotted::State* enemy_state;
    const dh2::character_enemy_spotted::Services* enemy_services;
    dh2::character::AIEventState64* event_state;
    const dh2::character::AIEventServices24* event_services;
    dh2::character_ai_relations::State* relation_state;
    const dh2::character_ai_relations::Services* relation_services;
    dh2::character::set_target::State* set_target_state;
    const dh2::character::set_target::Services* set_target_services;
    const dh2::character::aggro_search::Services* search_services;
    ScriptQueries script_queries;
    dh2::character::ControllerCommandState32* controller_state;
    const dh2::character::CharacterControlServices16* control_services;
    dh2::character::PathToState40* path_state;
    const dh2::character::PathToServices16* path_services;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    busy = 2,
    not_ready = 3,
    stale_binding = 4,
    source_failed = 5,
    script_failed = 6,
    allocation_failed = 7,
};

struct ScanResult {
    std::uint32_t candidates_before_dispatch;
    std::uint32_t events_raised;
    std::uint32_t enemy_callbacks;
    std::uint32_t script_dispatches;
    std::uint32_t path_requests;
    std::uint32_t set_target_calls;
    std::uint32_t head_to_calls;
    std::uint32_t move_to_calls;
    std::uint32_t reserved;
    dh2::character_aggro_candidate_events::Result candidate_events;
    dh2::character_enemy_spotted::Result last_enemy_gate;
    std::int32_t search_status;
    std::int32_t candidate_status;
    std::int32_t last_ai_event_status;
    std::int32_t last_enemy_gate_status;
    std::int32_t last_script_status;
    std::int32_t last_path_status;
};

// Owns one external-AIS Lua state for one stable source actor, and composes the
// available inner acquisition branch with Character/RaiseAIEvent,
// CharAI::OnEnemySpotted, the original monster callback, AI_SetTarget, and the
// existing Character controller/PathTo kernels. Caller chooses when the
// source _UpdateAggro inner search branch is reached and supplies its already
// selected radius/cone; _UpdateAggro's timing/type/turn prefix, _UpdateTarget,
// _UpdateMaster, and complete Character::Update are deliberately not claimed.
//
// Bindings must be refreshed after a world/actor rebind. The actor validates
// owner/AI/selected-AIS identities before each composed call and fails closed
// on stale storage. Same-actor rebind/reset from a callback is rejected; two
// actors own separate VM globals. One thread per actor. Destruction from one
// of its synchronous callbacks is forbidden; all borrowed game state and
// service contexts outlive the complete call.
class ActorSession {
public:
    ActorSession();
    ~ActorSession();
    ActorSession(const ActorSession&) = delete;
    ActorSession& operator=(const ActorSession&) = delete;
    ActorSession(ActorSession&&) = delete;
    ActorSession& operator=(ActorSession&&) = delete;

    // Rebinding is atomic: a new script VM is prepared before replacing the
    // old actor binding. A failed bind preserves the previous session.
    Status bind(const Bindings&, dh2::monster_external_script::Source commons,
                dh2::monster_external_script::Source monster, std::string& error);
    // Prepare the stable actor callback context without constructing/loading a
    // second VM. The returned copied service table can create and advance the
    // pending AIS VM. adopt_staged accepts only that ready VM with the exact
    // prepared table; its owner must outlive this ActorSession binding.
    Status prepare_staged(const Bindings&, std::string& error);
    // For the original pending-before-active path, active_ais_identity/callee
    // identify the selected pending AIS. The borrowed lifecycle owns the real
    // pending/active fields. No active projection is fabricated: getters may
    // run while pending is initialized; adoption requires source publication
    // and a refreshed enemy active projection. Retain lifecycle through the VM.
    Status prepare_pending(const Bindings&, const character::ScriptLifecycleState64*,
                           std::string& error);
    bool staged_services(dh2::monster_external_script::Services& output,
                         std::shared_ptr<void>& lifetime) const noexcept;
    Status adopt_staged(dh2::monster_external_script::Session&, std::string& error);
    Status reset(std::string& error);
    bool ready() const noexcept;
    dh2::monster_external_script::Statistics script_statistics() const noexcept;

    // Run the recovered _UpdateAggro search + normal candidate-consumer inner
    // branch from the caller-selected source radius/cone. `list` must already
    // be initialized by the source caller and owned by this actor. This method
    // does not emulate delay/player/follower/faerie/type gates.
    Status search_and_dispatch(dh2::character::aggro_search::TargetList* list,
                               const dh2::character::aggro_search::RoomRegistry* rooms,
                               float view_radius, float cone, ScanResult* result);

    // Actual _UpdateAggro producer: the borrowed flat ObjectManager Character
    // list. It uses the same retained VM/event/controller context as the
    // historical room-query route. It does not infer RoomZone/PFRoom identity.
    Status search_characters_and_dispatch(
        dh2::character::aggro_search::TargetList* list,
        dh2::character::aggro_character_list::CharacterList* characters,
        float view_radius, float cone, ScanResult* result);
    Status search_objects_and_dispatch(
        dh2::character::aggro_search::TargetList* list,
        const dh2::character::aggro_character_list::ObjectListMethods* objects,
        float view_radius, float cone, ScanResult* result);

    // Dispatch OnTargetHit/OnTargetMissed through this actor's already-active
    // AIS VM. The actor Character must be one of the two source combatants.
    Status dispatch_combat_result(monster_external_script::Event, std::uintptr_t attacker,
                                  std::uintptr_t defender, std::string& error);

private:
    struct Impl;
    Status search_and_dispatch_impl(dh2::character::aggro_search::TargetList*,
        const dh2::character::aggro_search::RoomRegistry*,
        dh2::character::aggro_character_list::CharacterList*,
        const dh2::character::aggro_character_list::ObjectListMethods*,
        float, float, ScanResult*);
    Status prepare_callbacks(const Bindings&, const character::ScriptLifecycleState64*,
                             std::string& error);
    std::shared_ptr<Impl> impl_;
};

static_assert(sizeof(ScriptQueries) == 128);

}  // namespace dh2::ghost_ai_session
