#pragma once

#include "../../../../../../port/level-world/character_ai_frame.hpp"
#include "../../../../../../port/level-world/character_ai_update.hpp"
#include "../../../../../../port/level-world/ais_external_update.hpp"
#include "../../../../../../port/level-world/character_aggro_acquisition_prefix.hpp"
#include "../../../../../../port/level-world/character_aggro_target_search.hpp"
#include "../../../../../../port/level-world/character_monster_retarget.hpp"
#include "../../../../../../port/level-world/character_enemy_retention.hpp"
#include "../../../../../../port/level-world/ghost_ai_session.hpp"

#include <cstdint>
#include <string>
#include <vector>

extern "C" {
#include "../../../../../../port/random/random.h"
}

namespace dh2::native::ghost_ai {

struct Identity {
    std::uintptr_t actor;
    std::uintptr_t character;
    std::uintptr_t ai;
    std::uintptr_t active_ais;
    std::uintptr_t active_ais_enemy_spotted;
};

struct Bindings {
    Identity identity;
    ghost_ai_session::Bindings script;
    monster_external_script::Source commons;
    monster_external_script::Source monster;
    const character::AIFrameServices24* frame_services;
    // Direct owner facts consumed by source CharAI::OnUpdate after the active
    // AIS virtual. State must be refreshed from the actor before each tick.
    character::AIUpdateState80* ai_update_state;
    const character::AIUpdateServices24* ai_update_services;
    // Borrowed live AIS field projection and its source virtual/Lua services.
    // The stable AIS identity must equal Identity::active_ais.
    dh2::ais_external_update::State* ais_update_state;
    const dh2::ais_external_update::Services* ais_update_services;
    character_aggro_acquisition_prefix::State* acquisition_state;
    const character_aggro_acquisition_prefix::Services* acquisition_services;
    // Optional existing-target providers. A missing branch remains explicit;
    // the ordinary acquisition scan must never replace retention.
    const character_monster_retarget::Services* retarget_services;
    const character_enemy_retention::Services* retention_services;
    std::int32_t (*resolve_retention_owner)(void*, std::uintptr_t,
                                         character_enemy_retention::Owner**);
    dh2_random_state* random;
    const character::aggro_search::RoomRegistry* rooms;
    void* character_registry_context;
    std::int32_t (*resolve_character)(void*, std::uintptr_t,
                                     character::aggro_search::Character**);
    std::uint32_t candidate_capacity;
    // Select exactly one producer. This is the source _UpdateAggro list;
    // `rooms` is retained only for the historical host adapter.
    character::aggro_character_list::CharacterList* characters = nullptr;
    const character::aggro_character_list::ObjectListMethods* objects = nullptr;
};

struct FrameInput {
    Identity identity;
    character::AIFrameOwner48 owner;
    std::uint32_t paused;
    std::uint32_t global_blocked;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    busy = 2,
    stale_owner = 3,
    script_not_ready = 4,
    source_failed = 5,
    unsupported_branch = 6,
    allocation_failed = 7,
};

struct FrameResult {
    Status status;
    std::int32_t source_frame_status;
    std::int32_t source_aggro_status;
    std::uint32_t frame_skip;
    std::uint32_t source_service;
    std::uint32_t acquisition_decision;
    std::uint32_t source_search_started;
    std::uint32_t candidate_count;
    std::uintptr_t actor_identity;
    std::uintptr_t character_identity;
    std::uintptr_t ai_identity;
    std::uintptr_t active_ais_identity;
    std::uintptr_t target_identity;
    std::uint32_t path_nonempty;
    float path_target[3];
    std::int32_t ais_update_status;
    std::uint32_t ais_update_calls;
    dh2::ais_external_update::Result ais_update;
    std::int32_t character_update_status;
    character::AIUpdateResult16 character_update;
    character_aggro_acquisition_prefix::Result acquisition;
    std::uintptr_t retarget_entry_owner;
    std::uint32_t retarget_started, retention_started;
    std::int32_t retarget_status, retention_status;
    character_monster_retarget::Result retarget;
    character_enemy_retention::Result retention;
    ghost_ai_session::ScanResult scan;
    character::AIFrameResult16 frame;
};

// Stable per-SpawnOwner source frame/search/VM composition. The actor owner
// object must itself be heap-stable (for example, shared by SpawnOwner). It
// owns the source CharAI frame projection, target-list scratch and one
// independent monster.lua VM; room/object/Character projections and every
// source callback context are borrowed from the live native level.
//
// `tick` is called after PhysicalWorld::update and before Character state,
// animator and GameObject path/rotation/subobject updates. It executes the
// recovered CharAI::Update dispatcher in source order. Its aggro service runs
// the recovered source prefix, then only the supported normal filter-2 search
// and existing Ghost ActorSession. Existing-target Monster updates use the
// recovered retarget/retention bodies when their live providers are bound.
// `CharAI::OnUpdate` is routed through the
// recovered AISExternal/AISDefault wrapper when its stable AIS projection and
// source services are bound. Other non-aggro CharAI frame services remain explicit.
class Owner final {
public:
    Owner() = default;
    ~Owner() = default;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    Status bind(const Bindings&, std::string& error);
    // The AIS resource owner constructs/loads/initializes its own pending VM
    // using these retained actor callbacks. This does not publish active.
    Status prepare_pending(const ghost_ai_session::Bindings&,
                           const character::ScriptLifecycleState64*,
                           monster_external_script::Services&,
                           std::shared_ptr<void>& callback_lifetime, std::string& error);
    // After source publication and projection refresh, use that exact VM for
    // frame dispatch. The VM, lifecycle and borrowed backing outlive this Owner.
    Status bind_staged(const Bindings&, monster_external_script::Session&, std::string& error);
    Status reset(std::string& error);
    bool ready() const noexcept;
    monster_external_script::Statistics script_statistics() const noexcept;
    Status dispatch_combat_result(monster_external_script::Event,
                                  std::uintptr_t attacker, std::uintptr_t defender,
                                  std::string& error);

    Status tick(const FrameInput&, FrameResult*);

private:
    Status bind_impl(const Bindings&, monster_external_script::Session*, std::string& error);
    static std::int32_t dispatch_frame(void*, character::AIFrameState32*,
                                      const character::AIFrameRequest16*,
                                      std::uint32_t*);
    static std::int32_t dispatch_character_update(void*, character::AIUpdateState80*,
                                      const character::AIUpdateRequest32*,
                                      std::uint32_t*);
    static std::int32_t dispatch_acquisition(void*, character_aggro_acquisition_prefix::State*,
        character_aggro_acquisition_prefix::Query, std::uintptr_t, std::uint32_t*);
    static std::int32_t capture_acquisition_props(void*, character_aggro_acquisition_prefix::State*,
        const character_aggro_acquisition_prefix::AiPropsTable**);
    std::int32_t update_existing_target(character_aggro_acquisition_prefix::State*);
    std::int32_t update_aggro(character::AIFrameState32*);
    std::int32_t update_ais(character::AIFrameState32*);
    std::int32_t update_character(character::AIFrameState32*);
    void capture_observation(FrameResult&) const noexcept;

    Identity identity_{};
    bool bound_ = false;
    bool busy_ = false;
    bool pending_prepared_ = false;
    ghost_ai_session::Bindings pending_bindings_{};
    Bindings bindings_{};
    character::AIFrameOwner48 frame_owner_{};
    character::AIFrameState32 frame_state_{};
    character::AIFrameServices24 frame_services_{};
    character::AIUpdateServices24 character_update_services_{};
    character::AIUpdateServices24 upstream_character_update_services_{};
    dh2::ais_external_update::Services ais_update_services_{};
    character_aggro_acquisition_prefix::Services acquisition_services_{};
    character_aggro_acquisition_prefix::Services upstream_acquisition_services_{};
    std::uintptr_t target_read_owner_ = 0;
    std::vector<character_enemy_retention::Target> retention_heap_;
    std::vector<character::aggro_search::TargetInfo> candidate_heap_;
    character::aggro_search::TargetList candidate_list_{};
    ghost_ai_session::ActorSession script_;
    FrameResult* active_result_ = nullptr;
};

static_assert(sizeof(Identity) == 40);

}  // namespace dh2::native::ghost_ai
