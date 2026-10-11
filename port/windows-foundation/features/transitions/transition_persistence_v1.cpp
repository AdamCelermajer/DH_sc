// P17 TRANSITIONS2: entry-point component and per-level world snapshot paths (see the header).
#include "transition_persistence_v1.hpp"

#include <cstring>

namespace dh::foundation::transitions {

namespace {
// Payload: u32 little-endian entry id, then the level URI bytes.
constexpr std::size_t entry_header_bytes = 4;
}

void set_entry_component(CharacterState& state, const std::string& level_uri, int entry_id) {
    CharacterComponent component;
    component.id = entry_component_id;
    const auto value = static_cast<std::uint32_t>(entry_id);
    component.bytes.resize(entry_header_bytes + level_uri.size());
    for (std::size_t i = 0; i < entry_header_bytes; ++i) component.bytes[i] = std::uint8_t(value >> (8 * i));
    if (!level_uri.empty()) std::memcpy(component.bytes.data() + entry_header_bytes, level_uri.data(), level_uri.size());
    clear_entry_component(state);
    state.components.push_back(std::move(component));
}

bool entry_component(const CharacterState& state, std::string& level_uri, int& entry_id) {
    for (const auto& component : state.components) {
        if (component.id != entry_component_id) continue;
        if (component.bytes.size() < entry_header_bytes) return false;
        std::uint32_t value = 0;
        for (std::size_t i = 0; i < entry_header_bytes; ++i) value |= std::uint32_t(component.bytes[i]) << (8 * i);
        level_uri.assign(component.bytes.begin() + std::ptrdiff_t(entry_header_bytes), component.bytes.end());
        entry_id = static_cast<int>(value);
        return true;
    }
    return false;
}

void clear_entry_component(CharacterState& state) {
    for (auto it = state.components.begin(); it != state.components.end();) {
        if (it->id == entry_component_id) it = state.components.erase(it);
        else ++it;
    }
}

std::filesystem::path level_world_path(const std::filesystem::path& save, const std::string& level_uri) {
    const std::filesystem::path level(level_uri);
    auto stem = save.stem().string();
    return save.parent_path() / (stem + ".world-" + level.stem().string() + ".gsave");
}

} // namespace dh::foundation::transitions
