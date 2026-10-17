#include "object_creation_map_v1.hpp"

#include <cstring>

namespace dh2::object_creation_map_v1 {

Resolution resolve(const char* source_type) noexcept {
    if (!source_type || !*source_type) return {Status::invalid_name, nullptr, 0};
    for (std::uint32_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        if (std::strcmp(source_type, entry.source_type.data()) == 0)
            return {Status::resolved, &entry, i};
    }
    return {Status::not_found, nullptr, 0};
}

} // namespace dh2::object_creation_map_v1
