#pragma once

#include "source_backend_script_host_v1.hpp"
#include "source_character_owner_factory_npc_context.hpp"
#include "source_character_owner_factory_npc_script_binding.hpp"
#include "../../../level-world/character_script_objects.hpp"

namespace dh::foundation::actor_frame {

// Connects actual source-owned assets, objects, host/current-Level, and the
// CampaignFsm already retained on each canonical record. It installs only
// CanonicalCharacterCandidateServicesV60::npc_script.
class SourceBackendNpcScriptBootstrapV1 final
    : public std::enable_shared_from_this<SourceBackendNpcScriptBootstrapV1> {
    std::weak_ptr<void> world_lease_;
    std::weak_ptr<void> objects_lease_;
    std::shared_ptr<dh2::windows_foundation::SourceCharacterOwnerFactoryNpcScriptBindingV1> assets_;
    std::weak_ptr<SourceBackendScriptHostV1> host_;
    dh2::character::CharacterScriptObjects* objects_{};
    dh2_script_object_services const* object_services_{};

    SourceBackendNpcScriptBootstrapV1() = default;
    bool bind_record(dh2::world::CanonicalCharacterCandidateRecordV60&,
        dh2::character::CharacterScriptSessionInput&,
        dh::foundation::features::SourceCharacterNpcContextBorrowV1&, std::string&);
    static bool bind_events(dh2::world::CanonicalCharacterCandidateRecordV60&,
        dh2::character::CharacterScriptSessionInput&, std::string&);
    static bool npc_script(void*, dh2::world::CanonicalCharacterCandidateRecordV60&,
        dh2::character::CharacterScriptSessionInput&, std::string&);
public:
    SourceBackendNpcScriptBootstrapV1(const SourceBackendNpcScriptBootstrapV1&) = delete;
    SourceBackendNpcScriptBootstrapV1& operator=(const SourceBackendNpcScriptBootstrapV1&) = delete;

    static bool create(
        std::shared_ptr<void> same_world_lease,
        std::shared_ptr<void> same_objects_lease,
        dh2::character::CharacterScriptObjects& same_objects,
        std::shared_ptr<dh2::windows_foundation::SourceCharacterOwnerFactoryNpcScriptBindingV1> actual_assets,
        std::shared_ptr<SourceBackendScriptHostV1> same_host,
        std::shared_ptr<SourceBackendNpcScriptBootstrapV1>& out,
        std::string& error);

    bool install(dh2::world::CanonicalCharacterCandidateServicesV60&, std::string& error);
};

} // namespace dh::foundation::actor_frame
