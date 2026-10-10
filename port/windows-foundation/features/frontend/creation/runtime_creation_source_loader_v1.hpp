#pragma once

#include "runtime_creation_persistence_v1.hpp"
#include "../../../asset_catalog.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::frontend::creation {

struct RuntimeCreationSourceHashV1 {
    std::string asset_path;
    std::uint64_t byte_count{};
    std::string sha256;
};

// Retains every table owner backing RuntimeCreationSourceV1 borrows. Random
// remains a borrowed adapter owned and advanced by the caller.
struct RuntimeCreationSourceOwnerV1 {
    std::shared_ptr<const OriginalPropertyDatabase> properties;
    std::shared_ptr<dh2::data::LootTablesV2> loot_owner;
    std::shared_ptr<dh2::data::SkillTables> skill_owner;
    dh2::data::LootRandom8V2* caller_owned_random_adapter{};
    std::array<std::uint32_t, 3> character_design_skill_caps{};
    bool character_design_caps_known{};
    std::vector<RuntimeCreationSourceHashV1> source_hashes;

    RuntimeCreationSourceV1 source() const;
    bool valid() const noexcept;
};

// Loads the actual source data from AssetCatalog's original cache. The source
// bytes hashed in source_hashes are the exact same buffers parsed into table
// owners. Publication is transactional: failure leaves `output` unchanged.
bool load_runtime_creation_source_v1(
    const AssetCatalog& assets,
    dh2::data::LootRandom8V2& caller_owned_random_adapter,
    RuntimeCreationSourceOwnerV1& output,
    std::string& error);

} // namespace dh::foundation::frontend::creation
