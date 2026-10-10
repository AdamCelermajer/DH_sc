#include "runtime_creation_appearance_v1.hpp"
#include "../../content_paths.hpp"
#include "../../../asset-payloads/sha256.hpp"
#include <algorithm>
#include <iomanip>
#include <map>
#include <sstream>
#include <stdexcept>

namespace dh::foundation::equipment_menu {
namespace {
using namespace dh2::data;
using namespace dh2::skinning;
namespace scene = dh2::scene;
namespace resources = dh2::resources;

std::string hex_digest(const dh2::assets::Sha256Digest& digest) {
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (const auto byte : digest) out << std::setw(2) << unsigned(byte);
    return out.str();
}

bool add_asset(const AssetCatalog& assets, const std::string& role,
        const std::string& uri, const std::filesystem::path& owner_uri,
        RuntimeCreationAppearanceRecipeV1& recipe, bool required = true) {
    if (uri.empty()) return false;
    std::filesystem::path resolved;
    try { resolved = resolve_content_path(assets, uri, owner_uri); }
    catch (const std::exception&) {
        if (required) throw;
        const auto found = std::find_if(recipe.assets.begin(), recipe.assets.end(),
            [&](const auto& existing) { return existing.authored_uri == uri; });
        if (found == recipe.assets.end()) recipe.assets.push_back({role, uri, "", 0, false});
        return false;
    }
    const auto relative = resolved.lexically_relative(assets.root()).generic_string();
    const auto found = std::find_if(recipe.assets.begin(), recipe.assets.end(),
        [&](const auto& existing) { return existing.authored_uri == relative; });
    if (found != recipe.assets.end()) return found->available;
    const auto bytes = assets.read(relative);
    dh2::assets::Sha256Digest digest{};
    if (!dh2::assets::sha256(bytes.data(), bytes.size(), digest))
        throw std::runtime_error("Source appearance asset SHA-256 failed: " + uri);
    recipe.assets.push_back({role, relative, hex_digest(digest), bytes.size(), true});
    return true;
}

RuntimeCreationMaterialV1 material(const scene::Material& source,
        const std::string& owner, std::uint32_t index, const std::string& symbol) {
    return {owner, symbol, source.id, source.diffuse, source.alpha_map,
            source.effect_file, source.effect_uri, source.gles2_technique, index};
}

void add_material_textures(const AssetCatalog& assets,
        const RuntimeCreationMaterialV1& source, RuntimeCreationAppearanceRecipeV1& recipe) {
    add_asset(assets, "diffuse_texture", source.diffuse_uri, source.source_asset_uri, recipe, false);
    add_asset(assets, "alpha_map_texture", source.alpha_map_uri, source.source_asset_uri, recipe, false);
}

std::string body_category(const std::string& module) {
    for (const char* category : {"MC_Torso", "MC_Feet", "MC_Hands"}) {
        const std::string prefix = std::string(category) + "_";
        if (module.compare(0, prefix.size(), prefix) == 0) return category;
    }
    return {};
}

const VisualModuleResourceV6* find_module(const VisualSkinResourcesV6::Borrow& body,
        const std::string& category, const std::string& alias) {
    const auto uri = "#" + alias;
    for (const auto& group : body.categories()) {
        if (group.name != category) continue;
        const auto found = std::find_if(group.modules.begin(), group.modules.end(),
            [&](const auto& module) { return module.uri == uri; });
        return found == group.modules.end() ? nullptr : &*found;
    }
    return nullptr;
}

RuntimeCreationAliasV1 make_alias(const VisualSkinResourcesV6::Borrow& body,
        const std::string& category, const std::string& alias,
        const std::string& body_uri, RuntimeCreationAppearanceRecipeV1& recipe,
        const AssetCatalog& assets) {
    const auto* module = find_module(body, category, alias);
    if (!module) throw std::runtime_error("Original class appearance controller alias absent: " + alias);
    RuntimeCreationAliasV1 output;
    output.category = category;
    output.controller_alias = alias;
    output.controller_uri = module->uri;
    output.geometry_id = module->part.geometry.id;
    const auto& materials = body.factory_scene().materials;
    for (std::size_t primitive = 0; primitive < module->part.geometry.primitives.size(); ++primitive) {
        if (primitive >= module->part.materials.size())
            throw std::runtime_error("Original class appearance primitive/material count differs: " + alias);
        const auto index = module->part.materials[primitive];
        if (index >= materials.size())
            throw std::runtime_error("Original class appearance material index is out of range: " + alias);
        auto binding = material(materials[index], body_uri, index,
                                module->part.geometry.primitives[primitive].material_symbol);
        add_material_textures(assets, binding, recipe);
        output.materials.push_back(std::move(binding));
    }
    return output;
}

void add_scene_materials(const AssetCatalog& assets, const scene::Scene& source,
        const std::string& asset_uri, std::vector<RuntimeCreationMaterialV1>& output,
        RuntimeCreationAppearanceRecipeV1& recipe) {
    for (const auto& instance : source.instances) {
        for (std::size_t slot = 0; slot < instance.materials.size(); ++slot) {
            const auto index = instance.materials[slot];
            if (index >= source.materials.size())
                throw std::runtime_error("Original class appearance scene material index is out of range");
            const auto symbol = slot < instance.material_symbols_v1.size()
                ? instance.material_symbols_v1[slot].symbol : std::string{};
            auto binding = material(source.materials[index], asset_uri, index, symbol);
            add_material_textures(assets, binding, recipe);
            output.push_back(std::move(binding));
        }
    }
}

} // namespace

bool build_runtime_creation_appearance_recipes_v1(const AssetCatalog& assets,
        const CharacterTable& characters, const ClassTables& classes,
        const PropertyRules& rules, const LootTablesV2::Borrow& loot,
        const AnimationTables& animations, const Dictionary& animation_clips,
        const Dictionary& character_models,
        std::array<RuntimeCreationAppearanceRecipeV1, 3>& output, std::string& error) {
    try {
        std::array<ClassPreviewDefinition, 3> definitions;
        if (!class_preview_definitions(characters, classes, rules, loot, animations,
                                       animation_clips, definitions, error)) return false;
        const auto model_field = std::find(characters.fields.begin(), characters.fields.end(), "ModelFile");
        if (model_field == characters.fields.end()) throw std::runtime_error("Original ModelFile field absent");
        const auto model_offset = static_cast<std::size_t>(model_field - characters.fields.begin());
        std::array<RuntimeCreationAppearanceRecipeV1, 3> next;
        for (std::size_t class_index = 0; class_index < definitions.size(); ++class_index) {
            const auto& definition = definitions[class_index];
            auto& recipe = next[class_index];
            recipe.character = definition.character;
            recipe.character_row = definition.row;
            recipe.loot_id = definition.loot;
            recipe.animation_table = definition.animation_table;
            const auto model_id = definition.properties.resolved.at(model_offset);
            if (model_id < 0 || static_cast<std::size_t>(model_id) >= character_models.values.size())
                throw std::runtime_error("Original class ModelFile dictionary index rejected: " + definition.character);
            recipe.body_model_uri = character_models.values[static_cast<std::size_t>(model_id)];
            recipe.idle_clip_uri = definition.idle_clip;
            recipe.select_clip_uri = definition.select_clip;
            recipe.template_clip_uri = definition.template_clip;

            add_asset(assets, "body_model_bres", recipe.body_model_uri, {}, recipe);
            add_asset(assets, "menu_idle_clip", recipe.idle_clip_uri, {}, recipe, false);
            add_asset(assets, "menu_select_clip", recipe.select_clip_uri, {}, recipe, false);
            add_asset(assets, "model_template_clip", recipe.template_clip_uri, {}, recipe, false);

            VisualSkinResourcesV6 body_resources;
            const auto body_bytes = read_content(assets, recipe.body_model_uri);
            if (!body_resources.load(body_bytes, error))
                throw std::runtime_error("Original class body BRES rejected: " + error);
            const auto body = body_resources.borrow();
            add_scene_materials(assets, body.factory_scene(), recipe.body_model_uri,
                                recipe.body_materials, recipe);

            // CreationPreview's source recipe begins with the empty head
            // controller, then adds the authored torso/feet/hands from the
            // genuine singleton starter-loot entries in their source order.
            recipe.body_controller_aliases.push_back("MC_Head__naked-mesh-skin");
            recipe.body_aliases.push_back(make_alias(body, "MC_Head",
                "MC_Head__naked-mesh-skin", recipe.body_model_uri, recipe, assets));
            for (const auto& starting : definition.starting_items) {
                if (starting.item < 0 || static_cast<std::size_t>(starting.item) >= loot.items().rows.size())
                    throw std::runtime_error("Original class starter item index is out of range");
                const auto& item = loot.items().rows[static_cast<std::size_t>(starting.item)];
                recipe.starter_items.push_back({starting.item, starting.identifier, starting.module,
                    starting.quantity, item_type(item), item.record.words[26]});
                const auto category = body_category(starting.module);
                if (!category.empty()) {
                    const auto alias = starting.module + "-mesh-skin";
                    recipe.body_controller_aliases.push_back(alias);
                    recipe.body_aliases.push_back(make_alias(body, category, alias,
                        recipe.body_model_uri, recipe, assets));
                }
                if (starting.module.compare(0, std::string("MC_RWeapon_").size(), "MC_RWeapon_") == 0) {
                    if (recipe.weapons.size() >= 2)
                        throw std::runtime_error("Original class starting weapon count exceeds source preview capacity");
                    RuntimeCreationWeaponV1 weapon;
                    weapon.starter_item_index = starting.item;
                    weapon.module = starting.module;
                    weapon.attachment_order = static_cast<std::uint32_t>(recipe.weapons.size());
                    weapon.asset_uri = "data/3d/characters/prince/weapons/" + starting.module + ".bdae";
                    weapon.anchor = recipe.weapons.empty()
                        ? "anchor_weapon_right_offset" : "anchor_weapon_left_offset";
                    weapon.asset_available = add_asset(assets, "starting_weapon_bres",
                        weapon.asset_uri, {}, recipe, false);
                    if (weapon.asset_available) {
                        const auto bytes = read_content(assets, weapon.asset_uri);
                        resources::BresView view{};
                        if (dh2_bres_open(&view, bytes.data(), bytes.size()) != resources::BresError::ok)
                            throw std::runtime_error("Original class weapon BRES rejected: " + weapon.asset_uri);
                        scene::Scene weapon_scene;
                        if (!scene::load(view, weapon_scene, error))
                            throw std::runtime_error("Original class weapon material scene rejected: " + error);
                        add_scene_materials(assets, weapon_scene, weapon.asset_uri, weapon.materials, recipe);
                    }
                    recipe.weapons.push_back(std::move(weapon));
                }
            }
            if (recipe.body_controller_aliases.size() != 4 || recipe.weapons.empty())
                throw std::runtime_error("Original class starter appearance does not match source preview shape: " + definition.character);
        }
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

} // namespace dh::foundation::equipment_menu
