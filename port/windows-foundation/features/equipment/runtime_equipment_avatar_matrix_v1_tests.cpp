#include "runtime_equipment_binding_v1.hpp"
#include "preview/runtime_equipment_preview_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../original_combat_properties.hpp"
#include "../../save_store.hpp"
#include "../../../engine-skinning/visual_skin_owner_v6.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message.empty() ? "B019 matrix check failed" : message);
}

struct ClassCase {
    const char* profile;
    const char* name;
    std::int32_t class_row;
    const char* skin;
    const char* suit;
    std::int32_t suit_row;
    const char* boots;
    std::int32_t boots_row;
    const char* gloves;
    std::int32_t gloves_row;
    const char* weapon;
    std::int32_t weapon_row;
    const char* weapon_geometry;
};

const ClassCase cases[] = {
    {"KnightPlayerBase", "Knight", 263, "_default_warrior-mesh-skin",
     "StartingSuit", 1079, "StartingBoots", 1073, "StartingGloves", 1076,
     "Longsword01", 664, "MC_RWeapon_Longsword_01"},
    {"RoguePlayerBase", "Rogue", 325, "_default_rogue-mesh-skin",
     "StartingSuitRogue", 1081, "StartingBootsRogue", 1075,
     "StartingGlovesRogue", 1078, "Dagger01", 370, "MC_RWeapon_Dagger_01"},
    {"MagePlayerBase", "Mage", 290, "_default_mage-mesh-skin",
     "StartingSuitMage", 1080, "StartingBootsMage", 1074,
     "StartingGlovesMage", 1077, "Staff01", 1025, "MC_RWeapon_Quarterstaff_01"}
};

SourceAppearanceDebugServices test_debug() {
    SourceAppearanceDebugServices debug;
    debug.load = [](std::string&) { return true; };
    debug.query = [](const char* name, std::string& error) {
        if (std::string(name) != "isTracingChar_Modular") {
            error = "Unexpected source Debug switch";
            return false;
        }
        return true;
    };
    return debug;
}

bool preview(RuntimeEquipmentBindingV1& binding, ActorId actor_id,
             const std::string& class_id, const CharacterVisual* visual,
             const dh2::scene::Scene* scene,
             std::vector<dh2::skinning::VisualDrawViewV32>& copy,
             std::string& error) {
    return binding.with_preview_borrow([&](const RuntimeEquipmentPreviewBorrowV1& value,
                                           std::string& callback_error) {
        if (value.actor_id != actor_id || value.class_id != class_id ||
            value.visual != visual || value.same_scene != scene || !value.source_views) {
            callback_error = "Equipment avatar identity mismatch actor=" + std::to_string(value.actor_id) +
                "/" + std::to_string(actor_id) + " class=" + value.class_id + "/" + class_id +
                " visual=" + std::to_string(reinterpret_cast<std::uintptr_t>(value.visual)) +
                "/" + std::to_string(reinterpret_cast<std::uintptr_t>(visual)) +
                " scene=" + std::to_string(reinterpret_cast<std::uintptr_t>(value.same_scene)) +
                "/" + std::to_string(reinterpret_cast<std::uintptr_t>(scene)) +
                " views=" + std::to_string(value.source_views != nullptr);
            return false;
        }
        copy = *value.source_views;
        return !copy.empty();
    }, error);
}

std::set<std::string> materials(const std::vector<dh2::skinning::VisualDrawViewV32>& views) {
    std::set<std::string> result;
    for (const auto& view : views) {
        if (!view.geometry || !view.material_table || !view.materials) continue;
        for (const auto material_index : *view.materials) {
            if (material_index >= view.material_table->size()) continue;
            const auto& material = view.material_table->at(material_index);
            if (!material.id.empty()) result.insert(material.id);
        }
    }
    return result;
}

std::set<std::pair<std::int32_t, std::int32_t>> body_modules(
        const std::vector<dh2::skinning::VisualDrawViewV32>& views) {
    std::set<std::pair<std::int32_t, std::int32_t>> result;
    for (const auto& view : views)
        if (view.weapon_slot == 0 && view.category >= 0)
            result.emplace(view.category, view.module);
    return result;
}

std::size_t weapon_views(const std::vector<dh2::skinning::VisualDrawViewV32>& views,
                         const char* geometry_id) {
    return static_cast<std::size_t>(std::count_if(views.begin(), views.end(),
        [&](const auto& view) {
            return view.weapon_slot == 1 && view.geometry &&
                   view.geometry->id.find(geometry_id) != std::string::npos;
        }));
}

std::set<std::string> weapon_materials(
        const std::vector<dh2::skinning::VisualDrawViewV32>& views,
        const char* geometry_id) {
    std::set<std::string> result;
    for (const auto& view : views) {
        if (view.weapon_slot != 1 || !view.geometry ||
            view.geometry->id.find(geometry_id) == std::string::npos ||
            !view.material_table || !view.materials) continue;
        for (const auto material_index : *view.materials) {
            if (material_index >= view.material_table->size()) continue;
            const auto& material = view.material_table->at(material_index);
            if (!material.id.empty()) result.insert(material.id);
        }
    }
    return result;
}

void run_case(const std::filesystem::path& root, const AssetCatalog& item_assets,
              const AssetCatalog& visual_assets, const OriginalPropertyDatabase& database,
              const OriginalMeleeBindings& melee, const ClassCase& test) {
    std::string error;
    check(test.class_row >= 0 &&
          static_cast<std::size_t>(test.class_row) < database.characters.names.size() &&
          database.characters.names[static_cast<std::size_t>(test.class_row)] == test.profile,
          std::string(test.name) + " class is not its actual CharacterTable starter row");
    ActorCustomization customization;
    customization.skin_id_contains = test.skin;
    customization.expected_controller_count = 4;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(visual_assets, melee, test.profile,
          customization, std::string("b019-matrix-") + test.name, visual_plan, error), error);

    CombatSessionConfig config;
    config.diagnosticRngSeed = 0x19019;
    config.playerId = test.profile[0] == 'K' ? 601 : test.profile[0] == 'R' ? 602 : 603;
    config.playerProfileId = test.profile;
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = visual_plan.config;
    CombatSessionProfile profile;
    profile.action = {"AttackStatic", 0, {0, 1}};
    profile.initialIdle = {"Idle", 0, {0}};
    profile.damageMarkerNames = {"attack_mainhand"};
    profile.propertyOptions = {256, true};
    config.profiles.emplace(test.profile, profile);

    ActorPopulation population;
    CharacterVisual visual;
    CombatSession session;
    check(session.initialize(visual_assets, database, melee, config, visual, population,
                             {0, 0, 0}, customization, error),
          std::string(test.name) + " source Session initialize: " + error);
    auto* actor = session.actor(config.playerId);
    const auto* combat = session.world()->combat_properties(config.playerId);
    const auto* same_visual = session.retained_actor_visual_borrow(config.playerId);
    check(actor && combat && same_visual == &visual && actor->definition_id == test.profile &&
          actor->equipment.empty() && !session.actor_binding_lease().expired(),
          std::string(test.name) + " fixture is not its exact same-Session player owner");

    CharacterState character = make_default_character(
        std::string("b019-") + test.name, test.name, test.profile);
    const auto instance_prefix = std::string("starter-") + test.name + "-";
    character.inventory = {
        {instance_prefix + "suit", test.suit, 1},
        {instance_prefix + "boots", test.boots, 1},
        {instance_prefix + "gloves", test.gloves, 1},
        {instance_prefix + "weapon", test.weapon, 1}
    };

    auto item_bytes = item_assets.read("original-cache/data/pydata/loot_table_pyarray.bin");
    auto item_names = item_assets.read("original-cache/data/pydata/loot_table_pyarraynames.bin");
    auto item_fields = item_assets.read("original-cache/data/pydata/loot_table_pystructnames.bin");
    dh2::data::ItemTable items;
    check(dh2::data::load_items({item_bytes.data(), item_bytes.size()},
          {item_names.data(), item_names.size()}, {item_fields.data(), item_fields.size()},
          items, error), error);
    auto require_row = [&](const char* name, std::int32_t row) {
        check(dh2::data::item_id(items, name) == row,
              std::string(test.name) + " starter item no longer matches source row: " + name);
    };
    require_row(test.suit, test.suit_row);
    require_row(test.boots, test.boots_row);
    require_row(test.gloves, test.gloves_row);
    require_row(test.weapon, test.weapon_row);

    RuntimeEquipmentOptionsV1 options;
    options.source_appearance_debug = test_debug();
    RuntimeEquipmentBindingV1 binding;
    check(binding.bind(session, character, item_assets, visual_assets, visual_assets,
                       database, std::move(options), error),
          std::string(test.name) + " same-Session equipment bind: " + error);
    check(binding.ready(error), error);
    const auto* scene = same_visual->retained_scene_borrow();
    check(scene, std::string(test.name) + " retained avatar Scene unavailable");

    std::vector<dh2::skinning::VisualDrawViewV32> baseline;
    check(preview(binding, config.playerId, test.profile, same_visual, scene, baseline, error), error);
    const auto baseline_materials = materials(baseline);
    check(!baseline_materials.empty(), std::string(test.name) + " source avatar has no material identity");
    const auto initial_clock = same_visual->animation_elapsed_seconds();

    const auto suit_id = instance_prefix + "suit";
    check(binding.equip_to_slot(suit_id, 0, error),
          std::string(test.name) + " actual starter torso equip: " + error);
    std::vector<dh2::skinning::VisualDrawViewV32> suited;
    check(preview(binding, config.playerId, test.profile, same_visual, scene, suited, error), error);
    check(character.equipment.size() == 1 && character.equipment.front().item_instance_id == suit_id &&
          actor->equipment.size() == 1 && actor->equipment.front().item_instance_id == suit_id &&
          body_modules(suited) != body_modules(baseline) &&
          same_visual->animation_elapsed_seconds() == initial_clock,
          std::string(test.name) + " torso equip did not update this avatar's source module identity");
    check(binding.unequip(0, error), std::string(test.name) + " starter torso unequip: " + error);
    std::vector<dh2::skinning::VisualDrawViewV32> torso_restored;
    check(preview(binding, config.playerId, test.profile, same_visual, scene,
          torso_restored, error), error);
    check(character.equipment.empty() && actor->equipment.empty() &&
          body_modules(torso_restored) == body_modules(baseline) &&
          materials(torso_restored) == baseline_materials &&
          same_visual->animation_elapsed_seconds() == initial_clock,
          std::string(test.name) + " torso unequip did not restore the original avatar materials");

    const auto weapon_id = instance_prefix + "weapon";
    check(binding.equip_to_slot(weapon_id, 1, error),
          std::string(test.name) + " actual starter weapon equip: " + error);
    RuntimeEquipmentRenderChangeV1 receipt;
    check(binding.take_render_change(receipt, error) && error.empty() &&
          receipt.actor_id == config.playerId && receipt.same_session_visual == same_visual &&
          receipt.same_scene == scene && receipt.attachments &&
          receipt.attachments->attachments().size() == 1 &&
          receipt.attachments->attachments().front().definition.model_uri.find(
              test.weapon_geometry) != std::string::npos,
          std::string(test.name) + " render receipt did not retain its exact starter weapon mesh");
    std::vector<dh2::skinning::VisualDrawViewV32> armed;
    check(preview(binding, config.playerId, test.profile, same_visual, scene, armed, error), error);
    check(weapon_views(armed, test.weapon_geometry) > 0 &&
          !weapon_materials(armed, test.weapon_geometry).empty() &&
          !materials(armed).empty() &&
          character.equipment.size() == 1 && actor->equipment.size() == 1 &&
          character.equipment.front().item_instance_id == weapon_id &&
          actor->equipment.front().item_instance_id == weapon_id,
          std::string(test.name) + " equipped avatar did not expose the exact source weapon/material and actor instance");

    const auto save_path = root / ".local-inputs" / "b019-avatar-matrix" /
                           (std::string(test.name) + ".save");
    std::filesystem::create_directories(save_path.parent_path());
    check(save_character(save_path, character, error), error);
    CharacterState restored = make_default_character("sentinel", "sentinel", "KnightPlayerBase");
    check(load_character(save_path, restored, error), error);
    check(restored.id == character.id && restored.class_id == test.profile &&
          restored.inventory.size() == character.inventory.size() &&
          restored.equipment.size() == 1 &&
          restored.equipment.front().item_instance_id == weapon_id,
          std::string(test.name) + " SaveStore roundtrip lost the source gear instance/class identity");
    RuntimeEquipmentOptionsV1 rebound_options;
    rebound_options.source_appearance_debug = test_debug();
    RuntimeEquipmentBindingV1 rebound;
    check(rebound.bind(session, restored, item_assets, visual_assets, visual_assets,
                       database, std::move(rebound_options), error),
          std::string(test.name) + " SaveStore rebind to same Session: " + error);
    std::vector<dh2::skinning::VisualDrawViewV32> rebound_views;
    check(preview(rebound, config.playerId, test.profile, same_visual, scene,
          rebound_views, error), error);
    RuntimeEquipmentRenderChangeV1 rebound_receipt;
    check(rebound.take_render_change(rebound_receipt, error) && error.empty() &&
          rebound_receipt.same_scene == scene && rebound_receipt.attachments &&
          rebound_receipt.attachments->attachments().size() == 1 &&
          rebound_receipt.attachments->attachments().front().definition.model_uri.find(
              test.weapon_geometry) != std::string::npos &&
          weapon_views(rebound_views, test.weapon_geometry) > 0 &&
          !weapon_materials(rebound_views, test.weapon_geometry).empty() &&
          same_visual->animation_elapsed_seconds() == initial_clock,
          std::string(test.name) + " SaveStore rebind changed avatar/mesh identity or advanced pose time");

    check(rebound.unequip(1, error), std::string(test.name) + " same-Session weapon unequip: " + error);
    std::vector<dh2::skinning::VisualDrawViewV32> unarmed;
    check(preview(rebound, config.playerId, test.profile, same_visual, scene,
          unarmed, error), error);
    check(restored.equipment.empty() && actor->equipment.empty() &&
          weapon_views(unarmed, test.weapon_geometry) == 0 &&
          same_visual->retained_scene_borrow() == scene &&
          same_visual->animation_elapsed_seconds() == initial_clock,
          std::string(test.name) + " weapon unequip left a stale/aliased source avatar model");
    std::filesystem::remove(save_path);
    std::cout << test.profile << " source_rows=" << test.suit_row << ',' << test.boots_row << ','
              << test.gloves_row << ',' << test.weapon_row
              << " same_actor_scene=PASS suit_material_refresh=PASS weapon_mesh=PASS"
                 " SaveStore_rebind=PASS unequip_restore=PASS\n";
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Repository root required");
        const std::filesystem::path root(argv[1]);
        AssetCatalog item_assets(root / ".local-inputs/windows-shared-assets");
        AssetCatalog visual_assets(root / ".local-inputs/windows-foundation-build/runtime-equipment-avatar-matrix-v1/assets");
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        std::string error;
        check(load_original_property_tables(item_assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(item_assets, "original-melee-bindings.xml", error), error);
        std::set<ActorId> actor_ids;
        for (const auto& test : cases) {
            const ActorId id = test.profile[0] == 'K' ? 601 : test.profile[0] == 'R' ? 602 : 603;
            check(actor_ids.insert(id).second, "Class matrix reused a player ActorId");
            run_case(root, item_assets, visual_assets, database, melee, test);
        }
        std::cout << "PASS B019 actual Knight/Rogue/Mage starter row and same-Session avatar matrix\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL B019 avatar matrix: " << e.what() << '\n';
        return 1;
    }
}
