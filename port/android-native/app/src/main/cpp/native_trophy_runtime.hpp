#pragma once

#include "../../../../../../port/game-data/trophy_manager_owner_v1.hpp"

namespace dh2::native::trophies {

// Process-wide native TrophyManager state. The owner binds the source
// TrophyTable and its separate achievements.savegame bitset sidecar.
class OwnerV1 {
    data::TrophyManagerOwnerV1 manager_;
public:
    bool initialize(data::Bytes records, data::Bytes names, data::Bytes fields,
                    std::string& error);
    // Matches TrophyManager::LoadTrophies/SaveTrophies: a 16-byte little-endian
    // 128-bit bitmap in the runtime data directory. Missing file means no
    // unlocked achievements; malformed files do not partially mutate state.
    bool load_savegame(const std::string& runtime_directory,
                       std::string& error);
    bool save_savegame(const std::string& runtime_directory,
                       std::string& error) const;
    bool initialized() const noexcept { return manager_.initialized(); }
    data::TrophyManagerOwnerV1* manager() noexcept {
        return initialized() ? &manager_ : nullptr;
    }
    const data::TrophyManagerOwnerV1* manager() const noexcept {
        return initialized() ? &manager_ : nullptr;
    }
    static constexpr bool persistence_available = true;
};

} // namespace dh2::native::trophies
