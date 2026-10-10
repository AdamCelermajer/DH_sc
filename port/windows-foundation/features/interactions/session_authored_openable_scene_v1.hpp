#pragma once

#include "session_source_object_admission_v1.hpp"
#include <cstddef>
#include <cstdint>
#include <map>

namespace dh::foundation::interactions {

struct SessionAuthoredOpenableSceneProvidersV1 {
    using PolicyFactory = bool(*)(void*,const ActorDefinition&,
        const dh2::world::OpenableContainerRowV1&,
        SessionContainerModernOpenablePolicyV1&,std::string&);
    using VisualRowValidator = bool(*)(void*,const ActorDefinition&,
        const dh2::world::OpenableContainerRowV1&,const WorldObject&,
        std::string&);

    // Owns callback contexts returned by either function and every raw context
    // placed in the generated policy. The callbacks must be source-backed.
    std::shared_ptr<const void> owner;
    void* context{};
    PolicyFactory make_policy{};
    VisualRowValidator validate_visual_row{};
    const std::uint8_t* source_row_names{};
    std::size_t source_row_names_size{};
};

class SessionAuthoredOpenableSceneV1 final
    : public std::enable_shared_from_this<SessionAuthoredOpenableSceneV1> {
public:
    static bool bind(CombatSession&,
        const std::vector<ActorDefinition>& source_scene,
        const std::vector<SessionSourceObjectAdmissionReceiptV1>& admitted,
        const AssetCatalog&,
        std::shared_ptr<SessionContainerRetainedVisualV1>,
        std::shared_ptr<SessionContainerModernDropV1>,
        std::shared_ptr<const dh2::world::OpenableContainerTableV1>,
        SessionAuthoredOpenableSceneProvidersV1,
        std::shared_ptr<SessionAuthoredOpenableSceneV1>&,
        std::string& error);

    ~SessionAuthoredOpenableSceneV1();
    SessionAuthoredOpenableSceneV1(const SessionAuthoredOpenableSceneV1&)=delete;

    bool interact(ObjectId,ActorId opener,std::string& error);
    bool animation_event(ObjectId,const RetainedAnimationEvent&,std::string& error);
    bool animation_finished(ObjectId,std::uint64_t generation,bool active,
                            std::string& error);
    bool find(ObjectId,std::shared_ptr<SessionContainerModernOpenableV1>&,
              std::string& error) const;
    bool release(ObjectId,std::string& error);
    bool release_all(std::string& error);
    std::vector<ObjectId> object_ids()const;

private:
    friend bool bind_session_authored_openable_scene_v1(CombatSession&,
        const std::vector<ActorDefinition>&,
        const std::vector<SessionSourceObjectAdmissionReceiptV1>&,
        const AssetCatalog&,
        std::shared_ptr<SessionContainerRetainedVisualV1>,
        std::shared_ptr<SessionContainerModernDropV1>,
        std::shared_ptr<const dh2::world::OpenableContainerTableV1>,
        SessionAuthoredOpenableSceneProvidersV1,
        std::shared_ptr<SessionAuthoredOpenableSceneV1>&,std::string&);
    struct Entry {
        std::shared_ptr<SessionContainerModernOpenableV1> owner;
        std::string definition_name;
    };
    SessionAuthoredOpenableSceneV1(CombatSession&,
        std::shared_ptr<const void>,std::shared_ptr<const void>,
        std::shared_ptr<const std::vector<ActorDefinition>>,
        std::shared_ptr<SessionContainerRetainedVisualV1>,
        std::shared_ptr<SessionContainerModernDropV1>,
        std::shared_ptr<const dh2::world::OpenableContainerTableV1>,
        std::shared_ptr<const void>);
    bool current(std::string& error)const;

    CombatSession* session_{};
    std::shared_ptr<const void> session_lease_,session_lifetime_lease_;
    std::shared_ptr<const std::vector<ActorDefinition>> definitions_;
    std::shared_ptr<SessionContainerRetainedVisualV1> visual_;
    std::shared_ptr<SessionContainerModernDropV1> drop_;
    std::shared_ptr<const dh2::world::OpenableContainerTableV1> table_;
    std::shared_ptr<const void> provider_owner_;
    std::map<ObjectId,Entry> entries_;
};

// Scans only already-present source Openable WorldObjects. Every object must
// have a matching successful source-admission receipt; absence from the world
// means the loader did not enroll that declaration, so it is skipped. The
// collection pins copied ActorDefinitions, retained visuals, drop/store/RNG,
// source table and callback owners until release or destruction.
bool bind_session_authored_openable_scene_v1(CombatSession&,
    const std::vector<ActorDefinition>& source_scene,
    const std::vector<SessionSourceObjectAdmissionReceiptV1>& admitted,
    const AssetCatalog&,
    std::shared_ptr<SessionContainerRetainedVisualV1>,
    std::shared_ptr<SessionContainerModernDropV1>,
    std::shared_ptr<const dh2::world::OpenableContainerTableV1>,
    SessionAuthoredOpenableSceneProvidersV1,
    std::shared_ptr<SessionAuthoredOpenableSceneV1>&,
    std::string& error);

} // namespace dh::foundation::interactions
