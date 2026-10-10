#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation {
struct ModularDefaultCategory {
    std::string node_id;
    std::string category;
    std::string default_uri;
    // Empty means original default URI did not select an available module.
    std::string controller_id;
    std::vector<std::string> available_controller_ids;
};

// Original CModularSkinnedMesh constructor selects each category's serialized
// default URI. Character class/player appearance is not an input. Local-player
// equipment initialization can replace these defaults afterward.
// Nonmodular content succeeds with no categories. Unsupported external/extra
// modular descriptors reject. Failure preserves the caller's previous output.
bool decode_modular_defaults(const std::vector<std::uint8_t>& bytes,
                             std::vector<ModularDefaultCategory>& output,
                             std::string& error);
} // namespace dh::foundation
