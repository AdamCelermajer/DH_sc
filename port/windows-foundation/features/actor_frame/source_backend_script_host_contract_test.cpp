#include "source_backend_script_host_v1.hpp"

#include <type_traits>

using namespace dh::foundation::actor_frame;

static_assert(sizeof(dh2::character::HostContextBindings16) == 16);
static_assert(sizeof(dh2::character::LevelServices16) == 16);
static_assert(std::is_same_v<decltype(std::declval<const SourceBackendScriptHostV1&>().bindings()),
    const dh2::character::HostContextBindings16*>);
static_assert(std::is_same_v<decltype(std::declval<const SourceBackendScriptHostV1&>().level_services()),
    const dh2::character::LevelServices16*>);
static_assert(std::is_same_v<decltype(std::declval<const SourceBackendScriptHostV1&>().context_pin()),
    std::shared_ptr<void>>);

int main() { return 0; }
