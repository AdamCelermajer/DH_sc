#pragma once

#include "player_profile_raw_sections_v1.hpp"

#include <filesystem>

namespace dh2::data::player_profile_atomic_replace_v1 {

struct Result {
    std::size_t previous_bytes{};
    std::size_t published_bytes{};
    std::uint32_t replacement_count{};
    bool backup_replaced{};
    bool primary_replaced{};
    bool index_published{};
};

// Replaces only the named sections in an existing indexed profile. The
// primary must still match `canonical_profile`; all other tags are preserved
// by the raw-section serializer. The old primary is durably copied to .bak,
// the new primary is atomically renamed into place, then the same index object
// and borrowed canonical view are republished. This is a file/profile helper:
// it does not dispatch Savegame::saveAll or infer which section writers run.
bool replace_existing_profile_sections_v1(
    const std::filesystem::path& primary,
    PlayerProfileIndexV1& canonical_index,
    PlayerProfileIndexV1::Borrow& canonical_profile,
    const std::vector<PlayerProfileRawSectionV1>& replacements,
    Result* result, std::string& error);

}  // namespace dh2::data::player_profile_atomic_replace_v1
