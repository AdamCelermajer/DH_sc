#pragma once

#include "session_openable_interaction_v1.hpp"
#include "../../../level-world/destructible_container_data_v16.hpp"

namespace dh::foundation::interactions {

// Persistent fields owned by the same source ActorId/session state provider.
// Stage counts are populated from the attached original visual's real slot
// count; this type never guesses a breakable clip count or marker time.
struct SessionDestructibleFieldsV1 {
    std::string data_desc;
    std::int32_t data_id{-1};
    std::int32_t state394{2};
    ActorId opener{invalid_actor_id};
    std::uint32_t animation_count{}, stages{}, remaining{};
    bool has_visual{}, initialized{};
};

struct SessionDestructibleInteractionServicesV1 {
    const void* session_identity{};
    std::shared_ptr<const void> session_lease;
    void* actor_context{};
    SessionContainerActorResolverV1 resolve_actor{};
    std::shared_ptr<const dh2::world::DestructibleContainerTableV16> table;
    void* visual_context{};
    bool (*visual_asset)(void*, const void*, ActorId, std::int32_t, std::string&){};
    bool (*has_visual)(void*, const void*, ActorId, bool&, std::string&){};
    bool (*animation_count)(void*, const void*, ActorId, std::uint32_t&, std::string&){};
    bool (*bind_retained_visual)(void*, const void*, ActorId,
        SessionContainerEventCallbackV1, SessionContainerCompletionCallbackV1,
        std::string&){};
    bool (*play_retained_clip)(void*, const void*, ActorId, const char*, bool&,
                               std::string&){};
    bool (*play_retained_index)(void*, const void*, ActorId, std::uint32_t, bool,
                                std::string&){};
    bool (*retained_scene_flags)(void*, const void*, ActorId, std::uint32_t,
                                 std::uint32_t, std::string&){};
    bool (*detach_physical)(void*, const void*, ActorId, std::string&){};
    bool (*play_sound_3d)(void*, const void*, ActorId, std::int32_t, std::string&){};
    bool (*source_on_interact)(void*, const void*, ActorId, std::string&){};
    bool (*load_object_script)(void*, const void*, ActorId, const char* script,
                               const char* directory, std::string&){};
    bool (*raise_destroy_quest)(void*, const void*, ActorId source, ActorId actor,
                                std::int32_t data_id, std::string&){};
    bool (*has_script)(void*, const void*, ActorId, bool&, std::string&){};
    bool (*script_call)(void*, const void*, ActorId, const char* method,
                        ActorId opener, const char* event, std::string&){};
    bool (*as_character)(void*, const void*, ActorId, bool&, std::string&){};
    bool (*increment_stat)(void*, const void*, ActorId, std::int32_t stat,
                           std::int32_t amount, std::string&){};
    bool (*get_stat)(void*, const void*, ActorId, std::int32_t stat,
                     std::int32_t&, std::string&){};
    bool (*is_local_player)(void*, const void*, ActorId, bool&, std::string&){};
    bool (*trophy_id)(void*, const char*, std::int32_t&, std::string&){};
    bool (*unlock_trophy)(void*, std::int32_t, std::string&){};
    SourceContainerLootServicesV1 loot;
};

// Session-native DestructibleContainer interaction, with staged hit state and
// authored visual callbacks but no CanonicalGameObject receiver/InitPost. It
// forwards DropLoot through the shared pure source selector and same-session
// drop sink; script, quest, physical, audio, and actor-stat tails remain typed
// requirements from the same session.
class SessionDestructibleInteractionV1 final
    : public std::enable_shared_from_this<SessionDestructibleInteractionV1> {
public:
    static std::shared_ptr<SessionDestructibleInteractionV1> create(
        SessionDestructibleInteractionServicesV1, std::string& error);
    bool initialize(ActorId source, std::string& error);
    bool interact(ActorId source, ActorId opener, std::string& error);
    bool animation_event(ActorId source, const RetainedAnimationEvent&,
                         std::string& error);
    bool animation_finished(ActorId source, std::uint64_t generation,
                            bool timeline_active, std::string& error);
private:
    struct EventKey {
        ActorId actor{}; std::uint64_t lifecycle{}, generation{};
        std::uint32_t slot{}, wall_ms{}; std::string clip, name;
        bool operator<(const EventKey&) const noexcept;
    };
    struct Receipt { bool success{}; std::string error; };
    explicit SessionDestructibleInteractionV1(SessionDestructibleInteractionServicesV1);
    bool borrow(ActorId, SessionContainerActorBorrowV1&, std::string&);
    bool borrow_interactor(ActorId, SessionContainerActorBorrowV1&, std::string&);
    bool services(ActorId, std::string&);
    bool persist_container_state(const SessionContainerActorBorrowV1&,
                                 std::int32_t, std::string&);
    bool open(ActorId, SessionContainerActorBorrowV1&, std::string&);
    bool drop_table(ActorId, std::int32_t, std::uintptr_t, std::int32_t,
                    bool, std::string&);
    bool raise_destroy(ActorId, ActorId, SessionContainerActorBorrowV1&, std::string&);
    bool play(ActorId, const char*, bool&, std::string&);
    bool flags(ActorId, std::uint32_t, std::uint32_t, std::string&);
    SessionDestructibleInteractionServicesV1 services_;
    SourceContainerLootV1 loot_;
    std::map<ActorId, std::pair<std::uint64_t,bool>> init_attempts_;
    std::map<EventKey, Receipt> event_receipts_;
    std::map<std::pair<ActorId,std::uint64_t>,Receipt> completion_receipts_;
};

} // namespace dh::foundation::interactions
