#include "source_backend_npc_script_bootstrap_v1.hpp"

#include <type_traits>

using namespace dh::foundation::actor_frame;

static_assert(std::is_same_v<decltype(&SourceBackendNpcScriptBootstrapV1::install),
    bool (SourceBackendNpcScriptBootstrapV1::*)(
        dh2::world::CanonicalCharacterCandidateServicesV60&, std::string&)>);
static_assert(std::is_same_v<decltype(std::declval<const SourceBackendScriptHostV1&>().world_lease()),
    const std::shared_ptr<void>&>);

int main() { return 0; }
