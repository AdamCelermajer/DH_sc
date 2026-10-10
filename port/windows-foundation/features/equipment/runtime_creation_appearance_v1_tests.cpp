#include "runtime_creation_appearance_v1.hpp"
#include "../../content_paths.hpp"
#include "../../../asset-payloads/sha256.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;
using namespace dh2::data;

namespace {
void check(bool value, const std::string& error) {
    if (!value) throw std::runtime_error(error.empty() ? "Creation appearance regression failed" : error);
}
std::vector<std::uint8_t> read(const AssetCatalog& assets, const std::string& name) {
    try { return read_content(assets, "data/pydata/" + name); }
    catch (const std::runtime_error&) { return read_content(assets, "data/" + name); }
}
std::string sha256(const std::vector<std::uint8_t>& bytes) {
    dh2::assets::Sha256Digest digest{};
    check(dh2::assets::sha256(bytes.data(), bytes.size(), digest), "Independent source asset hash failed");
    constexpr char hex[] = "0123456789abcdef";
    std::string value;
    for (auto byte : digest) { value += hex[byte >> 4]; value += hex[byte & 15]; }
    return value;
}
}

int main(int argc, char** argv) {
    try {
        const bool require_all_appearance_assets = argc == 3 &&
            std::string(argv[2]) == "--require-all-appearance-assets";
        check(argc == 2 || require_all_appearance_assets,
              "Original asset root required, with optional --require-all-appearance-assets");
        AssetCatalog assets(argv[1]);
        std::string error;
        auto character_bytes = read(assets, "character_properties_pyarray.bin");
        auto character_names = read(assets, "character_properties_pyarraynames.bin");
        auto character_fields = read(assets, "character_properties_pystructnames.bin");
        CharacterTable characters;
        check(load_characters({character_bytes.data(), character_bytes.size()},
            {character_names.data(), character_names.size()},
            {character_fields.data(), character_fields.size()}, characters, error), error);
        auto class_bytes = read(assets, "character_classes_pyarray.bin");
        auto class_names = read(assets, "character_classes_pyarraynames.bin");
        auto class_fields = read(assets, "character_classes_pystructnames.bin");
        ClassTables classes;
        PropertyRules rules;
        check(load_classes({class_bytes.data(), class_bytes.size()},
            {class_names.data(), class_names.size()}, {class_fields.data(), class_fields.size()},
            classes, error), error);
        check(load_property_rules(characters, rules, error), error);
        auto loot_bytes = read(assets, "loot_table_pyarray.bin");
        auto loot_names = read(assets, "loot_table_pyarraynames.bin");
        auto loot_fields = read(assets, "loot_table_pystructnames.bin");
        LootTablesV2 loot;
        check(loot.load({loot_bytes.data(), loot_bytes.size()}, {loot_names.data(), loot_names.size()},
                        {loot_fields.data(), loot_fields.size()}, error), error);
        auto animation_bytes = read(assets, "animations_pyarray.bin");
        auto animation_names = read(assets, "animations_pyarraynames.bin");
        auto animation_fields = read(assets, "animations_pystructnames.bin");
        AnimationTables animations;
        Dictionary animation_clips, character_models;
        auto clip_values = read(assets, "animations_dictionary_pyarray.bin");
        auto clip_names = read(assets, "animations_dictionary_pyarraynames.bin");
        check(load_dictionary({clip_names.data(), clip_names.size()},
              {clip_values.data(), clip_values.size()}, animation_clips, error), error);
        auto model_values = read(assets, "character_models_dictionary_pyarray.bin");
        auto model_names = read(assets, "character_models_dictionary_pyarraynames.bin");
        check(load_dictionary({model_names.data(), model_names.size()},
              {model_values.data(), model_values.size()}, character_models, error), error);
        check(load_animation_tables({animation_bytes.data(), animation_bytes.size()},
              {animation_names.data(), animation_names.size()},
              {animation_fields.data(), animation_fields.size()}, animation_clips, animations, error), error);

        std::array<RuntimeCreationAppearanceRecipeV1, 3> recipes;
        check(build_runtime_creation_appearance_recipes_v1(assets, characters, classes, rules,
              loot.borrow(), animations, animation_clips, character_models, recipes, error), error);
        const std::int32_t expected_rows[]{263, 325, 290};
        const std::int32_t expected_loot[]{165, 213, 174};
        const std::vector<std::int32_t> expected_items[]{{1079, 1073, 1076, 664, 925},
            {1081, 1075, 1078, 370, 370, 925}, {1080, 1074, 1077, 1025, 925}};
        const std::size_t expected_weapons[]{1, 2, 1};
        const char* expected_modules[][4] = {
            {"MC_Torso_default_warrior", "MC_Feet_default_warrior", "MC_Hands_default_warrior", "MC_RWeapon_Longsword_01"},
            {"MC_Torso_default_rogue", "MC_Feet_default_rogue", "MC_Hands_default_rogue", "MC_RWeapon_Dagger_01"},
            {"MC_Torso_default_mage", "MC_Feet_default_mage", "MC_Hands_default_mage", "MC_RWeapon_Quarterstaff_01"}};
        const char* expected_idle[] = {
            "prince_menu_idle_knight.bdae", "prince_menu_idle_rogue.bdae", "prince_menu_idle_mage.bdae"};
        const char* expected_select[] = {
            "prince_menu_idle_knight_02.bdae", "prince_menu_idle_rogue_02.bdae", "prince_menu_idle_mage_02.bdae"};
        const std::map<std::string, std::pair<std::string, std::uint64_t>> expected_appearance_assets{
            {"prince_menu_idle_knight.bdae", {"23c4268d68fa761221445ee45cb422aa9e777157b53481f6451876e3ac56915a", 19400}},
            {"prince_menu_idle_knight_02.bdae", {"f7c2c9da65f17b6bb737062bbdd13d3a2390332e6aa7504daf61af755df66aba", 36772}},
            {"prince_menu_idle_rogue.bdae", {"17dee5ad32e29307c25a83b4db898d14091de5e198ed5e50a5928c61444bbfa0", 14580}},
            {"prince_menu_idle_rogue_02.bdae", {"69a972b82dec287e392938a2aae3e810b135cec9a278f172b3a3ba7f592bbefe", 29472}},
            {"prince_menu_idle_mage.bdae", {"63f1673b3c8074079b8033514087f4f0a614f3d32c983fd2193bc48a14462058", 12884}},
            {"prince_menu_idle_mage_02.bdae", {"a9062d260e7b889fc3cf464c0d1edbed0f9b88323f5f0704a98d3d97e6df87c6", 34936}},
            {"mc_rweapon_dagger_01.bdae", {"d8361c9e2479f9a121767a97174ec468d8ddd7be738a8b9419147258a1ef3af9", 11660}},
            {"mc_rweapon_quarterstaff_01.bdae", {"7edea6e46bf94ebb29ac9d21405ee49a0df8342d9ab99de99bfc2ae6ea161553", 8972}}
        };
        std::map<std::string, bool> found_appearance_assets;
        for (std::size_t i = 0; i < recipes.size(); ++i) {
            const auto& recipe = recipes[i];
            check(recipe.character_row == expected_rows[i] && recipe.loot_id == expected_loot[i],
                  "Recipe class row or starting-loot identity differs from source tables");
            check(recipe.idle_clip_uri.find(expected_idle[i]) != std::string::npos &&
                  recipe.select_clip_uri.find(expected_select[i]) != std::string::npos,
                  "Class-specific menu clip aliases were replaced by a fallback");
            check(recipe.starter_items.size() == expected_items[i].size() &&
                  recipe.weapons.size() == expected_weapons[i] && recipe.body_aliases.size() == 4,
                  "Source starter inventory, module aliases or weapon count differs");
            for (std::size_t j = 0; j < expected_items[i].size(); ++j)
                check(recipe.starter_items[j].item_index == expected_items[i][j] &&
                      recipe.starter_items[j].quantity == (j + 1 == expected_items[i].size() ? 5 : 1),
                      "Starter item/quantity differs from actual singleton source loot list");
            check(recipe.starter_items[0].module == expected_modules[i][0] &&
                  recipe.starter_items[1].module == expected_modules[i][1] &&
                  recipe.starter_items[2].module == expected_modules[i][2] &&
                  recipe.weapons.front().module == expected_modules[i][3],
                  "Class-specific armor/weapon module aliases fell back to another class");
            check(recipe.body_controller_aliases.front() == "MC_Head__naked-mesh-skin" &&
                  recipe.weapons.front().anchor == "anchor_weapon_right_offset",
                  "Original empty-head or first weapon anchor alias differs");
            if (recipe.weapons.size() == 2)
                check(recipe.weapons[1].module == recipe.weapons[0].module &&
                      recipe.weapons[1].anchor == "anchor_weapon_left_offset" &&
                      recipe.weapons[1].attachment_order == 1,
                      "Rogue's two authored dagger attachment aliases/order differ");
            for (const auto& alias : recipe.body_aliases)
                check(!alias.controller_uri.empty() && !alias.geometry_id.empty() && !alias.materials.empty(),
                      "Controller alias lacks its actual BRES geometry/material manifest");
            for (const auto& weapon : recipe.weapons)
                check((weapon.asset_available && !weapon.materials.empty()) ||
                      (!weapon.asset_available && weapon.materials.empty()),
                      "Starter weapon material manifest disagrees with asset availability");
            bool has_body = false, has_idle = false, has_select = false, has_template = false;
            bool missing_idle = false, missing_select = false;
            for (const auto& asset : recipe.assets) {
                if (asset.available) {
                    const auto bytes = assets.read(asset.authored_uri);
                    check(!asset.sha256.empty() && asset.sha256.size() == 64 && asset.bytes == bytes.size() &&
                          asset.sha256 == sha256(bytes), "Recipe source asset hash does not match staged bytes");
                } else {
                    check(asset.sha256.empty() && asset.bytes == 0,
                          "Unavailable original source asset has a fabricated hash/size");
                    std::cout << "MISSING " << recipe.character << ' ' << asset.role << ' '
                              << asset.authored_uri << '\n';
                }
                has_body |= asset.role == "body_model_bres";
                has_idle |= asset.role == "menu_idle_clip";
                has_select |= asset.role == "menu_select_clip";
                has_template |= asset.role == "model_template_clip";
                missing_idle |= asset.role == "menu_idle_clip" && !asset.available;
                missing_select |= asset.role == "menu_select_clip" && !asset.available;
                if (asset.role == "menu_idle_clip" || asset.role == "menu_select_clip" ||
                    asset.role == "starting_weapon_bres") {
                    auto filename = std::filesystem::path(asset.authored_uri).filename().string();
                    std::transform(filename.begin(), filename.end(), filename.begin(),
                        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                    const auto expected = expected_appearance_assets.find(filename);
                    if (expected != expected_appearance_assets.end()) {
                        if (require_all_appearance_assets)
                            check(asset.available, "Expected original appearance BDAE unresolved: " + asset.authored_uri);
                        if (asset.available) {
                            check(asset.sha256 == expected->second.first && asset.bytes == expected->second.second,
                                  "Resolved appearance BDAE hash/size differs from original APK: " + asset.authored_uri);
                            const auto parent = std::filesystem::path(asset.authored_uri).parent_path().generic_string();
                            const auto clip = asset.role == "menu_idle_clip" || asset.role == "menu_select_clip";
                            check(parent == (clip ? "animations" : "models"),
                                  "Appearance BDAE did not resolve from the caller's authored asset root: " + asset.authored_uri);
                            found_appearance_assets[filename] = true;
                        }
                    }
                }
                if (asset.available) std::cout << "ASSET " << recipe.character << ' ' << asset.role << ' '
                          << asset.authored_uri << ' ' << asset.sha256 << ' ' << asset.bytes << '\n';
            }
            check(has_body && has_idle && has_select && has_template,
                  "Recipe omitted original body or authored menu clip reference");
            if (require_all_appearance_assets)
                check(!missing_idle && !missing_select,
                      "Caller asset root is missing an authored class menu clip");
            else
                check(missing_idle && missing_select,
                      "Fixture did not explicitly preserve its missing authored class menu clips");
            check(!recipe.body_materials.empty(), "Recipe omitted base body material aliases");
            std::cout << "CLASS " << recipe.character << " row=" << recipe.character_row
                      << " loot=" << recipe.loot_id << " model=" << recipe.body_model_uri
                      << " aliases=" << recipe.body_controller_aliases.size()
                      << " weapons=" << recipe.weapons.size() << '\n';
            for (const auto& item : recipe.starter_items)
                std::cout << "STARTER " << recipe.character << ' ' << item.item_index << ' '
                          << item.identifier << ' ' << item.module << " qty=" << unsigned(item.quantity)
                          << " type=" << item.item_type << " slotting=" << item.slotting << '\n';
            for (const auto& alias : recipe.body_aliases)
                std::cout << "ALIAS " << recipe.character << ' ' << alias.category << ' '
                          << alias.controller_alias << ' ' << alias.controller_uri << ' '
                          << alias.geometry_id << " materials=" << alias.materials.size() << '\n';
            for (const auto& material : recipe.body_materials)
                std::cout << "BODY_MATERIAL " << recipe.character << ' ' << material.source_asset_uri << ' '
                          << material.material_index << ' ' << material.primitive_symbol << ' '
                          << material.id << ' ' << material.diffuse_uri << ' ' << material.alpha_map_uri << ' '
                          << material.effect_uri << ' ' << material.technique << '\n';
            for (const auto& alias : recipe.body_aliases) for (const auto& material : alias.materials)
                std::cout << "ALIAS_MATERIAL " << recipe.character << ' ' << alias.controller_alias << ' '
                          << material.material_index << ' ' << material.primitive_symbol << ' '
                          << material.id << ' ' << material.diffuse_uri << ' ' << material.alpha_map_uri << '\n';
            for (const auto& weapon : recipe.weapons)
                std::cout << "WEAPON " << recipe.character << ' ' << weapon.module << ' '
                          << weapon.asset_uri << ' ' << weapon.anchor << " available="
                          << weapon.asset_available << '\n';
            for (const auto& weapon : recipe.weapons) for (const auto& material : weapon.materials)
                std::cout << "WEAPON_MATERIAL " << recipe.character << ' ' << weapon.module << ' '
                          << material.material_index << ' ' << material.primitive_symbol << ' '
                          << material.id << ' ' << material.diffuse_uri << ' ' << material.alpha_map_uri << '\n';
        }
        check(recipes[0].body_model_uri == recipes[1].body_model_uri &&
              recipes[1].body_model_uri == recipes[2].body_model_uri,
              "Three source class ModelFile aliases are not the same authored BRES");
        if (require_all_appearance_assets) {
            check(found_appearance_assets.size() == expected_appearance_assets.size(),
                  "Caller asset root did not resolve all eight authored appearance BDAEs");
            for (const auto& expected : expected_appearance_assets)
                check(found_appearance_assets.count(expected.first) != 0,
                      "Caller asset root omitted authored appearance BDAE: " + expected.first);
        }

        const auto before = recipes;
        Dictionary invalid_models;
        check(!build_runtime_creation_appearance_recipes_v1(assets, characters, classes, rules,
              loot.borrow(), animations, animation_clips, invalid_models, recipes, error) &&
              error.find("ModelFile") != std::string::npos,
              "Missing class model alias was not rejected explicitly");
        check(recipes[0].character == before[0].character &&
              recipes[0].assets.size() == before[0].assets.size() &&
              recipes[0].assets.front().sha256 == before[0].assets.front().sha256,
              "Failed recipe preparation partially replaced caller output");
        std::cout << "PASS source class-table starter recipes, actual body/weapon BRES aliases/materials, clip assets and SHA-256; atomic failure"
                  << (require_all_appearance_assets ? "; all eight original appearance assets resolved from caller root\n" : "\n");
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
