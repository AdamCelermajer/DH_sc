#include "native_ghost_script_queries.hpp"

namespace dh2::native::ghost_script_queries {

Status get_state(const Binding* binding, std::uintptr_t requested_owner,
                 std::int32_t* output) noexcept {
    if (!binding || !binding->owner || !binding->character_state || !output)
        return Status::invalid_argument;
    if (requested_owner != binding->owner) return Status::stale_owner;
    *output = *binding->character_state;
    return Status::complete;
}

Status has_path(const Binding* binding, std::uintptr_t requested_owner,
                std::uint32_t* output) noexcept {
    if (!binding || !binding->owner || !binding->path_count || !output)
        return Status::invalid_argument;
    if (requested_owner != binding->owner) return Status::stale_owner;
    *output = *binding->path_count != 0;
    return Status::complete;
}

}  // namespace dh2::native::ghost_script_queries
