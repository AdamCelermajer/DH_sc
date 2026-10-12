#pragma once

#include "../../../../../level-world/ais_external_update.hpp"
#include "../../../../../level-world/character_ai_frame.hpp"
#include "../../../../../level-world/character_ai_initialization.hpp"
#include "../../../../../level-world/character_ai_interaction_range.hpp"
#include "../../../../../level-world/character_ai_master_update.hpp"
#include "../../../../../level-world/character_ai_melee_range.hpp"
#include "../../../../../level-world/character_ai_sight.hpp"
#include "../../../../../level-world/character_ai_update.hpp"
#include "../../../../../level-world/character_aggro_acquisition_prefix.hpp"
#include "../../../../../level-world/character_coordinator.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::native::faery_ai_frame_v1 {

// CharAI::Update 0x3cfbf4 for the shipped Faery companion.
//
// The Faery is a normal Character whose AITable row 20 ("Faery", Type 3,
// ViewRadius 2300) selects scripts/ai/follower.luac through AISExternal. The C++
// AISFaery (0x3de2d0) is not reachable from the shipped tables. CharAI::Update
// is the one per-frame entry that
//   * _UpdateTarget 0x3cb908 (Faery target is null: no-op),
//   * _UpdateMaster 0x3cc5a4: edge events 18/19 (master died/revived), 20/21
//     (master out of/in sight) and, while the master is alive and in sight,
//     one of 22..25 (out of range / ranged / close / melee range) -> the
//     follower.luac handlers WarpBehind / MoveTo / Stop,
//   * _UpdateAggro 0x3cf3f0: a Faerie without a master searches the Level
//     CharacterList within AITable ViewRadius and raises event 9/7/8 per
//     enemy/friend/neutral candidate; event 7 reaches follower_OnFriendSpotted,
//     which calls SetMaster,
//   * _Generic = CharAI::OnUpdate (active AIS virtual OnUpdate, zoning).
// This owner composes the already ported kernels in that source order through
// dh2_character_ai_frame and the Faery's own Coordinator. It creates no Lua VM:
// events are routed by the Host into the Faery's single retained AISExternal
// Session (Record::source_external_ais).
//
// `Host` carries every fact or effect that lives in the renderer-owned
// Character/GameObject/AIS/Level objects.
class Host {
public:
    virtual ~Host() = default;

    // ---- Character / GameObject facts of the Faery owner ----
    // CharStateMachine::SM_GetState.
    virtual bool character_state(std::int32_t& state) = 0;
    // GameObject::GetTargetPosition of the Faery or of another Character.
    virtual bool target_position(std::uintptr_t identity, float out[3]) = 0;
    // Character::IsDead (vfn +52) of an arbitrary master.
    virtual bool is_dead(std::uintptr_t identity, bool& dead) = 0;
    // AITable row of the Faery: +0x3c ViewRadius, +0x40 no-aggro radius,
    // +0x18 interact radius.
    virtual bool ai_row(float& view_radius, float& view_radius_no_aggro,
                        float& interact_radius) = 0;
    // CharAI::AI_GetMeleeRadius for the Faery's own AI, or the embedded CharAI
    // of the resolved Character `identity` (Character::GetInteractionRadius).
    virtual bool melee_radius(bool owner_ai, std::uintptr_t identity,
                              float& radius) = 0;
    // Character::GetInteractionType of `subject` toward `other`.
    virtual bool interaction_type(std::uintptr_t subject, std::uintptr_t other,
                                  std::uint32_t& type) = 0;
    // GameObject::GetInteractionSpot of `identity` (copy), and whether its
    // lazily cached +0x2e8 interaction_position node exists.
    virtual bool interaction_spot(std::uintptr_t identity, float out[3],
                                  bool& node_present) = 0;
    // Source ObjectHandle resolution of `identity` through the ObjectManager.
    virtual bool resolve_object(std::uintptr_t identity) = 0;
    // DebugSwitches IsTracingCharAITarget (key 1) / isTracingCharAITarget (2).
    virtual bool debug_switch(std::uint32_t key, std::uint32_t& value) = 0;
    // Character::CanRangeAttack of the Faery (ItemInventory, none equipped).
    virtual bool can_range_attack(bool& value) = 0;
    // Character::GetCharAIId.
    virtual bool char_ai_id(std::uint32_t& value) = 0;

    // ---- effects ----
    // Character::RaiseEvent(event, payload): CharAI::RaiseAIEvent -> AISExternal
    // callback (and CharStateMachine::RaiseStateEvent where the source does).
    virtual bool raise_event(std::uint32_t event, std::uintptr_t payload) = 0;
    // _UpdateAggro acquisition: TargetList(owner,0x7fffffff,2,1) over the Level
    // CharacterList within `radius`, then the per-candidate 9/7/8 events.
    virtual bool aggro_scan(float radius) = 0;
    // AISExternal::OnUpdate Lua callback (flag bit0) on the retained Session.
    virtual bool ais_script_on_update() = 0;
    // The retained AIS' flags_b8 projection (bit0 = OnUpdate in the VFTable).
    virtual bool ais_flags(std::uint32_t& flags) = 0;
    // Character::DisableZoning / VisualObject::SyncVisibility of the Faery.
    virtual bool disable_zoning() = 0;
    virtual bool sync_visibility() = 0;
};

struct FrameInput {
    std::uint32_t global_blocked = 0;  // v2Controller::s_blocked
    std::uint32_t forced = 0;          // controller +9
    std::uint32_t zoned = 0;           // Character +0x2ee
    std::uint32_t in_zone = 0;         // Character +0x2f0
    std::uintptr_t visual = 0;         // GameObject +0x2d8
};

struct Statistics {
    std::uint32_t frames = 0;
    std::uint32_t skipped = 0;
    std::uint32_t master_events[8]{};  // events 18..25
    std::uint32_t friend_events = 0;
    std::uint32_t enemy_events = 0;
    std::uint32_t neutral_events = 0;
    std::uint32_t scans = 0;
    std::uint32_t last_event = 0;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    not_bound = 2,
    busy = 3,
    source_failed = 4,
    unsupported_branch = 5,
};

struct Result {
    Status status = Status::complete;
    character::AIFrameResult16 frame{};
    int coordinator_status = 0;
    std::int32_t update_master_status = 0;
    std::int32_t update_aggro_status = 0;
    std::int32_t on_update_status = 0;
    std::string error;
};

class Owner final {
public:
    Owner() = default;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    // Binds the Character/CharAI/controller identities. `ai` is the Faery's
    // canonical CharAI state (target/master/alive/sight bytes).
    bool bind(std::uintptr_t character, std::uintptr_t ai_identity,
              std::uintptr_t controller, character_ai_initialization::State* ai);
    bool bound() const noexcept { return ai_ != nullptr; }
    // The stable AIFrameOwner48 the Faery's Coordinator binds (and validates
    // against) for update_ai_frame.
    character::AIFrameOwner48* projection() noexcept { return &frame_owner_; }
    const Statistics& statistics() const noexcept { return statistics_; }

    // CharAI::Update through the Faery's Coordinator.
    Status tick(character::Coordinator& coordinator, Host& host,
                const FrameInput& input, Result* result);

private:
    static std::int32_t dispatch_frame(void*, character::AIFrameState32*,
        const character::AIFrameRequest16*, std::uint32_t*);
    std::int32_t update_target();
    std::int32_t update_master();
    std::int32_t update_aggro();
    std::int32_t on_update();

    static std::int32_t master_service(void*, character_ai_master_update::State*,
        const character_ai_master_update::Request*,
        character_ai_master_update::Response*);
    static std::int32_t prefix_service(void*,
        character_aggro_acquisition_prefix::State*,
        character_aggro_acquisition_prefix::Query, std::uintptr_t,
        std::uint32_t*);
    static std::int32_t prefix_props(void*,
        character_aggro_acquisition_prefix::State*,
        const character_aggro_acquisition_prefix::AiPropsTable**);
    static std::int32_t update_service(void*, character::AIUpdateState80*,
        const character::AIUpdateRequest32*, std::uint32_t*);
    static std::int32_t ais_service(void*, ais_external_update::State*,
        const ais_external_update::Request*);

    std::int32_t sight_value(std::uintptr_t master, std::uint32_t& value);
    std::int32_t melee_value(std::uintptr_t master, std::uint32_t& value);
    std::int32_t interaction_range_value(std::uintptr_t master,
                                         std::uint32_t& value);
    std::int32_t fail(const char* reason);

    std::uintptr_t character_ = 0;
    std::uintptr_t ai_identity_ = 0;
    std::uintptr_t controller_ = 0;
    character_ai_initialization::State* ai_ = nullptr;
    character::AIFrameOwner48 frame_owner_{};
    character::AIFrameState32 frame_state_{};
    character::AIFrameServices24 frame_services_{};

    // Active call context.
    Host* host_ = nullptr;
    Result* result_ = nullptr;
    bool busy_ = false;
    std::string error_;
    Statistics statistics_{};

    // Borrowed storage handed to the range kernels (they keep views live only
    // across the call).
    character_ai_sight::Point points_[3]{};
    character_ai_sight::ViewRadius view_{};
    character_ai_melee_range::ResolvedObject resolved_{};
    character_ai_interaction_range::ObjectFacts spot_facts_{};
    character_ai_interaction_range::AiRow ai_row_{};
    std::vector<character_aggro_acquisition_prefix::AiPropsRow> prefix_rows_;
    character_aggro_acquisition_prefix::AiPropsTable prefix_table_{};
    std::int32_t machine_state_ = -1;
    FrameInput input_{};
};

}  // namespace dh2::native::faery_ai_frame_v1
