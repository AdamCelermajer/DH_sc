#pragma once

#include "source_current_level_backend_v1.hpp"
#include "../../../level-world/application_services_owner_v5.hpp"
#include "../../../level-world/character_host_context.hpp"
#include "../../../level-world/character_design_services.hpp"
#include "../../../level-world/application_player_manager_bootstrap_v59.hpp"

namespace dh::foundation::actor_frame {

// Online GetHostingPlayer is a distinct source path. A backend must provide
// the actual matching/network-selected PlayerInfo and its receiver pin;
// offline selection remains the source internal0,false path.
using SourceBackendHostingPlayerSelectorV1 = bool (*)(
    void*, dh2::player::ApplicationPlayerManagerBootstrapV59&,
    dh2::player::PlayerInfoFieldsV1*&, std::shared_ptr<void>&, std::string&);

struct SourceBackendScriptHostServicesV1 {
    // Required lifetime for the context and DebugFileServices24 callback
    // context. The PlayerManager and DebugSwitches themselves are obtained
    // from the same retained Application; they are never independently made.
    std::shared_ptr<void> provider_lease;
    std::shared_ptr<void> debug_files_lease;
    dh2::character::DebugFileServices24 debug_files{};
    SourceBackendHostingPlayerSelectorV1 online_hosting_player{};
    void* online_hosting_player_context{};
};

// Projects the existing Application PlayerManager and the same GS current
// Level/LevelTables into the portable Character host ABI. This is a service
// adapter, not a WorldScriptContext or an alternate source owner.
class SourceBackendScriptHostV1 final : public std::enable_shared_from_this<SourceBackendScriptHostV1> {
    std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
    std::shared_ptr<SourceCurrentLevelBackendV1> current_level_;
    std::shared_ptr<void> provider_lease_;
    std::shared_ptr<void> debug_files_lease_;
    std::shared_ptr<dh2::character::DebugSwitches> debug_;
    dh2::character::DebugFileServices24 debug_files_{};
    dh2::character::DebugLevelBinding16 debug_binding_{};
    dh2::character::LevelServices16 level_services_{};
    dh2::character::HostContextBindings16 host_bindings_{};
    SourceBackendHostingPlayerSelectorV1 online_hosting_player_{};
    void* online_hosting_player_context_{};
    std::vector<dh2::character::LevelRangeRow24> range_projection_;
    std::shared_ptr<void> player_request_pin_, level_request_pin_;
    dh2::character::HostPlayer8 host_player_{};
    dh2::character::HostLevel8 host_level_{};
    std::string error_;

    SourceBackendScriptHostV1() = default;

    static int invoke_host(void*, const dh2::character::HostContextRequest16*,
        dh2::character::HostContextResponse16*);
    static int invoke_level(void*, dh2::character::LevelModel32*,
        const dh2::character::LevelRequest24*);
    bool refresh_ranges(std::string&);
public:
    SourceBackendScriptHostV1(const SourceBackendScriptHostV1&) = delete;
    SourceBackendScriptHostV1& operator=(const SourceBackendScriptHostV1&) = delete;

    static bool create(
        std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>,
        std::shared_ptr<SourceCurrentLevelBackendV1>,
        SourceBackendScriptHostServicesV1,
        std::shared_ptr<SourceBackendScriptHostV1>&, std::string&);

    const dh2::character::HostContextBindings16* bindings() const noexcept {
        return &host_bindings_;
    }
    const dh2::character::LevelServices16* level_services() const noexcept {
        return &level_services_;
    }
    const std::string& error() const noexcept { return error_; }
    std::shared_ptr<void> context_pin() const {
        return std::static_pointer_cast<void>(
            const_cast<SourceBackendScriptHostV1*>(this)->shared_from_this());
    }
    const std::shared_ptr<void>& world_lease() const noexcept {
        return current_level_->graph().world;
    }
};

} // namespace dh::foundation::actor_frame
