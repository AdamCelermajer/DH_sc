#pragma once

#include "../../combat_session.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../../level-world/openable_container_owner_v1.hpp"
#include "../../../level-world/openable_container_interaction_v2.hpp"
#include "source_container_loot_v1.hpp"
#include <map>
#include <set>

namespace dh::foundation::interactions {

struct SessionContainerActorBorrowV1 {
    const void* session_identity{};
    ActorId actor_id{invalid_actor_id};
    const ActorDefinition* definition{};
    // Character-backed receivers use state; neutral authored objects use
    // object. A container never needs fabricated combat vitals.
    ActorState* state{};
    WorldObject* object{};
    // Source-owned interaction fields on the same session ActorId.
    dh2::world::OpenableContainerFieldsV1* openable_state{};
    struct SessionDestructibleFieldsV1* destructible_state{};
    std::shared_ptr<const void> session_lease;
    std::uint64_t binding_lifecycle{};
};
using SessionContainerActorResolverV1 = bool(*)(
    void*, const void* session_identity, ActorId,
    SessionContainerActorBorrowV1&, std::string&);

using SessionContainerEventCallbackV1 = std::function<bool(
    ActorId, const RetainedAnimationEvent&, std::string&)>;
using SessionContainerCompletionCallbackV1 = std::function<bool(
    ActorId, std::uint64_t generation, bool timeline_active, std::string&)>;

struct SessionOpenableInteractionServicesV1 {
    const void* session_identity{};
    std::shared_ptr<const void> session_lease;
    void* actor_context{};
    SessionContainerActorResolverV1 resolve_actor{};
    // Other original OpenableContainerOwner services (conditions, authored
    // row, script, sound, key inventory, physical and InitPost prefixes).
    dh2::world::OpenableContainerServicesV1 source;
    dh2::world::OpenableContainerInteractionServicesV2 interaction;
    void* visual_context{};
    bool (*bind_retained_visual)(void*, const void*, ActorId,
        SessionContainerEventCallbackV1, SessionContainerCompletionCallbackV1,
        std::string&){};
    bool (*play_retained_clip)(void*, const void*, ActorId, const char*, bool&,
                               std::string&){};
    bool (*retained_scene_flags)(void*, const void*, ActorId, std::uint32_t,
                                 std::uint32_t, std::string&){};
    SourceContainerLootServicesV1 loot;
};

// Same-session source owner. The caller resolves ActorIds to the current
// ActorState/ActorDefinition and attached source container fields; visual events
// are fed by the existing retained session clock. This class owns no second
// actor registry, animation clock, loot RNG or world-item pool.
class SessionOpenableInteractionV1 final
    : public std::enable_shared_from_this<SessionOpenableInteractionV1> {
public:
    static std::shared_ptr<SessionOpenableInteractionV1> create(
        SessionOpenableInteractionServicesV1, std::string& error);
    SessionOpenableInteractionV1(const SessionOpenableInteractionV1&) = delete;

    bool init_post(ActorId source, std::string& error);
    // Rebind an already-restored modern WorldObject without replaying source
    // InitPost. State2 receives fresh retained callbacks; state4 stays silent.
    bool restore_silently(ActorId source, std::string& error);
    bool interact(ActorId source, ActorId opener, std::string& error);
    bool animation_event(ActorId source, const RetainedAnimationEvent&,
                         std::string& error);
    bool animation_finished(ActorId source, std::uint64_t generation,
                            bool timeline_active, std::string& error);
    const SourceContainerLootV1& loot_owner() const noexcept { return loot_; }

private:
    struct EventKey {
        ActorId actor{};
        std::uint64_t lifecycle{}, generation{};
        std::uint32_t slot{}, wall_ms{};
        std::string clip, name;
        bool operator<(const EventKey& o) const noexcept;
    };
    struct Receipt { bool success{}; std::string error; };
    SessionOpenableInteractionV1(SessionOpenableInteractionServicesV1);
    bool borrow(ActorId, bool require_openable,
                SessionContainerActorBorrowV1&, std::string&);
    bool initialized(const SessionContainerActorBorrowV1&, std::string&,
                     bool during_init=false);
    bool source_services(ActorId, dh2::world::OpenableContainerServicesV1&,
                         std::string&);
    bool drop_table(ActorId, std::int32_t, std::uintptr_t,
                    std::int32_t, bool, std::string&);
    bool register_visual(ActorId, std::string&);
    bool play_clip(ActorId, const char*, bool&, std::string&);
    bool scene_flags(ActorId, std::uint32_t, std::uint32_t, std::string&);
    bool persist_container_state(const SessionContainerActorBorrowV1&,
                                 std::int32_t, std::string&);

    SessionOpenableInteractionServicesV1 services_;
    SourceContainerLootV1 loot_;
    struct InitReceipt { std::uint64_t lifecycle{}; bool active{}, ready{}; };
    std::map<ActorId, InitReceipt> init_attempts_;
    std::map<EventKey, Receipt> event_receipts_;
    std::map<std::pair<ActorId, std::uint64_t>, Receipt> completion_receipts_;
};

} // namespace dh::foundation::interactions
