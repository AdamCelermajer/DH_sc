#pragma once

#include "../frontend/creation/runtime_creation_source_loader_v1.hpp"
#include "../../../game-data/design_settings.hpp"
#include "../../../game-data/loot_audiovisual_v8.hpp"
#include "../../../game-data/loot_entry_selection_v8.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation { class PlayableActorWorld; }

namespace dh::foundation::loot {

// Read-only source inputs for loot/reward consumers. Table borrows pin their
// immutable snapshots; `loot` is the exact six-table snapshot already owned by
// RuntimeCreationSourceOwnerV1. No random state or native application graph is
// created here.
struct RuntimeLootSourceV1 {
    std::shared_ptr<const OriginalPropertyDatabase> properties;
    dh2::data::LootTablesV2::Borrow loot;
    dh2::data::ItemPowerTablesV5::Borrow item_power_tables;
    dh2::data::LootPowerResourcesV7::Borrow power_resources;
    dh2::data::LootAudioVisualV8::Borrow audiovisual;
    dh2::data::DesignSettingsOwner::Borrow design_settings;
    std::array<std::int32_t,3> character_max_level{};
    std::size_t source_merchant_rows{};

    bool valid() const noexcept;
    bool max_level_for_difficulty(std::int32_t unlocked_difficulty,
                                  std::int32_t& output,
                                  std::string& error) const;
};

// Same-source bounded companion to RuntimeCreationSourceOwnerV1. It reads the
// complete original loot cache only to decode the V88 suffix after the exact
// six-table prefix; it does not create another LootTables owner or require the
// V88 localization/GameDesign/native Application graph.
class RuntimeLootSourceOwnerV1 {
    std::shared_ptr<const OriginalPropertyDatabase> properties_;
    dh2::data::LootTablesV2::Borrow loot_;
    std::shared_ptr<dh2::data::ItemPowerTablesV5> item_power_tables_;
    std::shared_ptr<dh2::data::LootPowerResourcesV7> power_resources_;
    std::shared_ptr<dh2::data::LootAudioVisualV8> audiovisual_;
    std::shared_ptr<dh2::data::DesignSettingsOwner> design_owner_;
    std::array<std::int32_t,3> character_max_level_{};
    std::size_t source_merchant_rows_{};
    std::vector<dh::foundation::frontend::creation::RuntimeCreationSourceHashV1> source_hashes_;

public:
    bool load(const AssetCatalog&,
              const dh::foundation::frontend::creation::RuntimeCreationSourceOwnerV1&,
              std::string& error);

    bool valid() const noexcept;
    RuntimeLootSourceV1 borrow() const;
    const std::vector<dh::foundation::frontend::creation::RuntimeCreationSourceHashV1>&
        source_hashes() const noexcept { return source_hashes_; }
};

// Source PlayerManager class-count query implemented over the same live world
// and the CharacterTable identity stored on each ActorState. The original
// receiver compares Character::InitPre's cached CharacterTable base ID; it
// does not compare ClassTables/selectable class IDs.
bool source_player_class_count_v1(
    const dh::foundation::PlayableActorWorld&,
    const OriginalPropertyDatabase&,
    dh2::data::LootEntryOperationV8,
    std::int32_t& output,
    std::string& error);

} // namespace dh::foundation::loot
