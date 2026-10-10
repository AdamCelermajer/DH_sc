#pragma once

#include "../../asset_catalog.hpp"
#include "../../../game-data/class_preview_setup.hpp"
#include "../../../engine-skinning/visual_skin_owner_v6.hpp"

namespace dh::foundation::equipment_menu {

struct RuntimeCreationAssetV1 {
    std::string role, authored_uri, sha256;
    std::uint64_t bytes{};
    bool available = false;
};

struct RuntimeCreationMaterialV1 {
    std::string source_asset_uri, primitive_symbol, id, diffuse_uri, alpha_map_uri;
    std::string effect_file, effect_uri, technique;
    std::uint32_t material_index{};
};

struct RuntimeCreationAliasV1 {
    std::string category, controller_alias, controller_uri, geometry_id;
    std::vector<RuntimeCreationMaterialV1> materials;
};

struct RuntimeCreationStarterItemV1 {
    std::int32_t item_index{-1};
    std::string identifier, module;
    std::uint8_t quantity{};
    std::int32_t item_type{-1}, slotting{-1};
};

struct RuntimeCreationWeaponV1 {
    std::int32_t starter_item_index{-1};
    std::string module, asset_uri, anchor;
    std::uint32_t attachment_order{};
    bool asset_available = false;
    std::vector<RuntimeCreationMaterialV1> materials;
};

struct RuntimeCreationAppearanceRecipeV1 {
    std::string character, body_model_uri, idle_clip_uri, select_clip_uri, template_clip_uri;
    std::int32_t character_row{-1}, loot_id{-1}, animation_table{-1};
    std::vector<RuntimeCreationStarterItemV1> starter_items;
    std::vector<std::string> body_controller_aliases;
    std::vector<RuntimeCreationAliasV1> body_aliases;
    std::vector<RuntimeCreationMaterialV1> body_materials;
    std::vector<RuntimeCreationWeaponV1> weapons;
    std::vector<RuntimeCreationAssetV1> assets;
};

// Reuses class_preview_definitions' genuine class-table, starting-loot and
// animation-clip facts. The returned manifest describes assets/controllers;
// it does not create a Character, grant loot, equip an item or invent powers.
bool build_runtime_creation_appearance_recipes_v1(
    const AssetCatalog&, const dh2::data::CharacterTable&, const dh2::data::ClassTables&,
    const dh2::data::PropertyRules&, const dh2::data::LootTablesV2::Borrow&,
    const dh2::data::AnimationTables&, const dh2::data::Dictionary& animation_clips,
    const dh2::data::Dictionary& character_models,
    std::array<RuntimeCreationAppearanceRecipeV1, 3>&, std::string& error);

} // namespace dh::foundation::equipment_menu
