#include "runtime_equipment_binding_v1.hpp"
#include "runtime_equipment_page_v1.hpp"
#include "runtime_equipment_text_v1.hpp"
#include "preview/runtime_equipment_preview_v1.hpp"
#include "source_equipment_material_binding.hpp"
#include "../effects/effects_material_binding.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../original_combat_properties.hpp"
#include "../../../engine-math/math.hpp"
#include "../character_menu/character_menu.hpp"
#include "../character_menu/source_composition.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message.empty() ? "Runtime equipment test failed without diagnostic" : message);
}
bool same_character(const CharacterState& a, const CharacterState& b) {
    if (a.schema_version != b.schema_version || a.id != b.id || a.name != b.name ||
        a.class_id != b.class_id || a.stats.level != b.stats.level ||
        a.stats.health != b.stats.health || a.stats.max_health != b.stats.max_health ||
        a.stats.resource != b.stats.resource || a.stats.max_resource != b.stats.max_resource ||
        a.stats.strength != b.stats.strength || a.stats.dexterity != b.stats.dexterity ||
        a.stats.intelligence != b.stats.intelligence || a.stats.endurance != b.stats.endurance ||
        a.stats.energy != b.stats.energy || a.experience != b.experience || a.gold != b.gold ||
        a.source_stat_points != b.source_stat_points || a.source_skill_points != b.source_skill_points ||
        a.inventory.size() != b.inventory.size() || a.equipment.size() != b.equipment.size() ||
        a.skills.size() != b.skills.size() || a.unlocks != b.unlocks) return false;
    for (std::size_t i = 0; i < a.inventory.size(); ++i)
        if (a.inventory[i].instance_id != b.inventory[i].instance_id ||
            a.inventory[i].definition_id != b.inventory[i].definition_id ||
            a.inventory[i].quantity != b.inventory[i].quantity) return false;
    for (std::size_t i = 0; i < a.equipment.size(); ++i)
        if (a.equipment[i].slot != b.equipment[i].slot ||
            a.equipment[i].item_instance_id != b.equipment[i].item_instance_id ||
            a.equipment[i].equipment_set != b.equipment[i].equipment_set ||
            a.equipment[i].source_slot != b.equipment[i].source_slot) return false;
    for (std::size_t i = 0; i < a.skills.size(); ++i)
        if (a.skills[i].id != b.skills[i].id || a.skills[i].rank != b.skills[i].rank) return false;
    return true;
}
bool same_actor(const ActorState& a, const ActorState& b) {
    if (a.id != b.id || a.definition_id != b.definition_id || a.class_id != b.class_id ||
        a.faction_id != b.faction_id || a.transform.position != b.transform.position ||
        a.transform.rotation != b.transform.rotation || a.transform.scale != b.transform.scale ||
        a.health != b.health || a.max_health != b.max_health || a.resource != b.resource ||
        a.max_resource != b.max_resource || a.action != b.action ||
        a.action_elapsed_seconds != b.action_elapsed_seconds || a.target_id != b.target_id ||
        a.equipment.size() != b.equipment.size() || a.attack_ids != b.attack_ids ||
        a.persistent_character_id != b.persistent_character_id ||
        a.source_flags520 != b.source_flags520 || a.source_movement_type != b.source_movement_type ||
        a.source_validate_boundary452 != b.source_validate_boundary452 ||
        a.source_target_node180 != b.source_target_node180 ||
        a.source_target_position184 != b.source_target_position184) return false;
    for (std::size_t i = 0; i < a.equipment.size(); ++i)
        if (a.equipment[i].slot != b.equipment[i].slot ||
            a.equipment[i].definition_id != b.equipment[i].definition_id ||
            a.equipment[i].item_instance_id != b.equipment[i].item_instance_id) return false;
    return true;
}
bool same_combat(const OriginalCombatProperties& a, const OriginalCombatProperties& b) {
    return a.sheets.base == b.sheets.base && a.sheets.saved == b.sheets.saved &&
        a.sheets.gear == b.sheets.gear && a.sheets.resolved == b.sheets.resolved &&
        a.facts.main_damage_class == b.facts.main_damage_class &&
        a.facts.off_damage_class == b.facts.off_damage_class &&
        a.facts.two_hander == b.facts.two_hander && a.facts.dual_wield == b.facts.dual_wield &&
        a.facts.shield == b.facts.shield && a.facts.original_state == b.facts.original_state &&
        a.facts.combo_hits == b.facts.combo_hits;
}
SourceAppearanceDebugServices test_source_debug(std::vector<std::string>* events = nullptr,
        const std::shared_ptr<unsigned>& query_count = {},
        const std::shared_ptr<unsigned>& fail_query_at = {}) {
    SourceAppearanceDebugServices debug;
    debug.load = [events](std::string&) {
        if (events) events->push_back("load");
        return true;
    };
    debug.query = [events, query_count, fail_query_at](const char* name, std::string& error) {
        if (events) events->push_back(std::string("query:") + name);
        if (query_count) ++*query_count;
        if (query_count && fail_query_at && *fail_query_at &&
            *query_count == *fail_query_at) {
            error = "fixture source Debug query failure";
            return false;
        }
        return std::string(name) == "isTracingChar_Modular";
    };
    return debug;
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Repository root required");
        const std::filesystem::path root(argv[1]);
        dh2::scene::Node identity_root;
        Mat4 source_rebase{};
        std::string error;
        check(equipment_preview_rebase_v1(identity_root, source_rebase, error), error);
        dh2::math::Quaternion source_z_rotation{};
        dh2_quat_from_euler(&source_z_rotation, 0.0f, 0.0f, -0.5f);
        dh2::math::Matrix4f source_z_matrix{};
        dh2_quat_matrix_transposed(&source_z_rotation, &source_z_matrix);
        for (unsigned i = 0; i < 16; ++i)
            check(std::abs(source_rebase[i] - source_z_matrix.m[i]) < 0.00001f,
                  "Equipment preview rebase did not preserve the authored identity-root Z=-0.5 transform");
        AssetCatalog assets(root / ".local-inputs/windows-shared-assets");
        AssetCatalog weapons(root / ".local-inputs/windows-equipment-assets/original-cache");
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        dh2::data::PropertyRules rules;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(dh2::data::load_property_rules(database.characters, rules, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 0x19a7;
        config.playerId = 501;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan visual_plan;
        check(build_original_combat_visual_plan(assets, melee, config.playerProfileId,
              customization, "runtime-equipment-player", visual_plan, error), error);
        config.playerVisualConfig = visual_plan.config;

        CombatSessionProfile profile;
        profile.action = {"AttackStatic", 0, {0, 1}};
        profile.initialIdle = {"Idle", 0, {0}};
        profile.damageMarkerNames = {"attack_mainhand"};
        profile.propertyOptions = {256, true};
        config.profiles.emplace(config.playerProfileId, profile);

        ActorPopulation population;
        CharacterVisual player_visual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, player_visual, population,
                                 {0, 0, 0}, customization, error), error);
        auto* live_actor = session.actor(config.playerId);
        const auto* live_combat = session.world()->combat_properties(config.playerId);
        const auto* live_visual = session.retained_actor_visual_borrow(config.playerId);
        const auto old_scene = live_visual ? live_visual->retained_scene_borrow() : nullptr;
        check(live_actor && live_combat && live_visual && live_visual == &player_visual &&
              live_visual->retained_scene_borrow() == player_visual.retained_scene_borrow(),
              "Fixture did not expose the actual CombatSession player Scene/world owners");

        CharacterState character = make_default_character("runtime-equipment", "Session Player",
                                                           live_actor->definition_id);
        check(live_actor->class_id == "KnightPlayerClass" &&
              live_actor->definition_id == "KnightPlayerBase" &&
              character.class_id == "KnightPlayerBase",
              "Focused fixture did not expose the actual source-profile/class-table identity tuple");
        character.inventory = {{"source-sword", "Longsword01", 1},
                               {"source-sword-2", "Longsword01", 1},
                               {"source-suit", "StartingSuit", 1},
                               {"source-boots", "StartingBoots", 1},
                               {"source-gloves", "StartingGloves", 1},
                               {"source-potion", "Potion0", 3}};
        const auto item_records = assets.read("original-cache/data/pydata/loot_table_pyarray.bin");
        const auto item_names = assets.read("original-cache/data/pydata/loot_table_pyarraynames.bin");
        const auto item_fields = assets.read("original-cache/data/pydata/loot_table_pystructnames.bin");
        dh2::data::ItemTable provider_table;
        check(dh2::data::load_items({item_records.data(), item_records.size()},
              {item_names.data(), item_names.size()}, {item_fields.data(), item_fields.size()},
              provider_table, error), error);
        character_menu::MenuLocalization menu_localization;
        check(menu_localization.load(assets, "original-cache/data", 0, error), error);
        check(menu_localization.bind_profile(&character, error), error);
        bool reject_suit_power = false;
        std::vector<std::string> source_debug_events;
        auto source_debug_query_count = std::make_shared<unsigned>(0);
        auto source_debug_fail_at = std::make_shared<unsigned>(0);
        RuntimeEquipmentOptionsV1 options;
        options.source_appearance_debug = test_source_debug(&source_debug_events,
            source_debug_query_count, source_debug_fail_at);
        options.powers = [&](const InventoryItem& instance,
                std::vector<dh2::data::GearPowerProperty12V5>&, std::string& power_error) {
            if (reject_suit_power && instance.instance_id == "source-suit") {
                power_error = "fixture source power provider rejected";
                return false;
            }
            return true;
        };
        RuntimeEquipmentPageBindingsV1 page_bindings;
        RuntimeEquipmentTextProviderV1 text_provider;
        RuntimeEquipmentBareDefinitionPolicyV1 bare_policy;
        bare_policy.supported_unpowered_definition_ids = {
            "Longsword01", "StartingSuit", "StartingBoots", "StartingGloves", "Potion0"};
        check(text_provider.bind(provider_table, database.characters, menu_localization,
              character, bare_policy, options, page_bindings, error), error);
        check(options.menu.item_name && options.menu.empty_name &&
              page_bindings.inventory_text.item_name && page_bindings.inventory_text.symbol &&
              page_bindings.inventory_text.potions && page_bindings.details_text.item_name &&
              page_bindings.details_text.item_details,
              "Original ItemTable/HudText default equipment text provider left a callback unbound: " +
              std::to_string(bool(options.menu.item_name)) + "/" +
              std::to_string(bool(options.menu.empty_name)) + "/" +
              std::to_string(bool(page_bindings.inventory_text.item_name)) + "/" +
              std::to_string(bool(page_bindings.inventory_text.symbol)) + "/" +
              std::to_string(bool(page_bindings.inventory_text.potions)) + "/" +
              std::to_string(bool(page_bindings.details_text.item_name)) + "/" +
              std::to_string(bool(page_bindings.details_text.item_details)));
        const auto unsupported = std::find_if(provider_table.identifiers.begin(),
            provider_table.identifiers.end(), [&](const auto& id) {
                return std::find(bare_policy.supported_unpowered_definition_ids.begin(),
                    bare_policy.supported_unpowered_definition_ids.end(), id) ==
                    bare_policy.supported_unpowered_definition_ids.end();
            });
        check(unsupported != provider_table.identifiers.end(),
              "Actual ItemTable has no out-of-policy definition for explicit unsupported coverage");
        InventoryItem generated_projection{"unsupported-generated-witness", *unsupported, 1};
        std::string unsupported_name;
        check(!options.menu.item_name(generated_projection,
              provider_table.rows[static_cast<std::size_t>(unsupported-provider_table.identifiers.begin())],
              unsupported_name, error) &&
              error.find("powered/generated or unknown") != std::string::npos,
              "Bare-definition provider silently described a non-approved generated/powered row");
        const auto sword_source_id = dh2::data::item_id(provider_table, "Longsword01");
        check(sword_source_id >= 0, "Original provider ItemTable lacks the authored sword row");
        std::string source_sword_name;
        check(options.menu.item_name(character.inventory.front(),
              provider_table.rows[static_cast<std::size_t>(sword_source_id)],
              source_sword_name, error), error);
        check(!source_sword_name.empty() && source_sword_name != "source-sword",
              "Original HudText item-name callback emitted an ID or empty placeholder");
        std::string source_empty_name, source_potion_text;
        check(options.menu.empty_name(source_empty_name, error), error);
        check(!source_empty_name.empty() && source_empty_name != "GLOBAL_EMPTY",
              "Original StringManager empty-slot callback emitted its key");
        check(page_bindings.inventory_text.potions(3, source_potion_text, error), error);
        check(!source_potion_text.empty() && source_potion_text.find("3") != std::string::npos,
              "Original GAMEPLAYMENUS_POTIONS formatter omitted the actual quantity");
        {
            CharacterState wrong_source_profile = character;
            wrong_source_profile.class_id = "RoguePlayerBase";
            RuntimeEquipmentBindingV1 wrong_profile_binding;
            check(!wrong_profile_binding.bind(session, wrong_source_profile, assets, assets,
                  weapons, database, options, error) &&
                  error.find("source definition") != std::string::npos,
                  "Equipment accepted a different valid source CharacterTable profile");
            CharacterState legacy_class_state = character;
            legacy_class_state.class_id = live_actor->class_id;
            check(std::find(database.classes.names.begin(), database.classes.names.end(),
                  legacy_class_state.class_id) != database.classes.names.end(),
                  "Legacy compatibility fixture is not an actual ClassTables ID");
            RuntimeEquipmentBindingV1 legacy_binding;
            check(legacy_binding.bind(session, legacy_class_state, assets, assets,
                  weapons, database, options, error), error);
            check(legacy_binding.ready(error), error);
        }
        RuntimeEquipmentBindingV1 equipment;
        RuntimeEquipmentOptionsV1 restore_options = options;
        const auto unarmed_state_before_bind = character;
        const auto unarmed_actor_before_bind = *live_actor;
        const auto unarmed_combat_before_bind = *live_combat;
        const auto actual_bind_debug_start = source_debug_events.size();
        check(equipment.bind(session, character, assets, assets, weapons, database,
                             std::move(options), error), error);
        check(equipment.ready(error), error);
        auto inspect_preview = [&](RuntimeEquipmentPreviewFrameV1& preview,
                                   std::vector<std::pair<std::int32_t, std::int32_t>>& modules) {
            return equipment.with_preview_borrow(
                [&](const RuntimeEquipmentPreviewBorrowV1& borrowed, std::string& callback_error) {
                    EquipmentPreviewSourceV1 source;
                    source.revision = borrowed.revision;
                    source.actor_id = borrowed.actor_id;
                    source.class_id = borrowed.class_id;
                    source.visual = borrowed.visual;
                    source.same_scene = borrowed.same_scene;
                    source.source_views = borrowed.source_views;
                    source.attachments = borrowed.attachments;
                    if (borrowed.actor_id != config.playerId || borrowed.class_id != character.class_id ||
                        borrowed.visual != live_visual || borrowed.same_scene != old_scene ||
                        !borrowed.attachments || !borrowed.source_views) {
                        callback_error = "Preview callback lost same-session visual/Scene/gear identity";
                        return false;
                    }
                    if (!compose_runtime_equipment_preview_frame_v1(source, preview, callback_error))
                        return false;
                    if (!preview.same_visual_root || preview.same_visual_root_world != preview.same_visual_root->world) {
                        callback_error = "Preview did not borrow the source-priority root from the same Scene";
                        return false;
                    }
                    modules.clear();
                    for (const auto& view : *borrowed.source_views) {
                        if (view.weapon_slot == 0) modules.emplace_back(view.category, view.module);
                    }
                    if (modules.size() < 4) {
                        callback_error = "Preview source draw_views omitted original modular body categories";
                        return false;
                    }
                    return true;
                }, error);
        };
        const double preview_clock_before = live_visual->animation_elapsed_seconds();
        RuntimeEquipmentPreviewFrameV1 unarmed_preview;
        std::vector<std::pair<std::int32_t, std::int32_t>> naked_modules;
        check(inspect_preview(unarmed_preview, naked_modules), error);
        check(unarmed_preview.revision == 1 && unarmed_preview.actor_id == config.playerId &&
              unarmed_preview.same_scene == old_scene &&
              unarmed_preview.attachments->attachments().empty() &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Initial preview did not borrow the unarmed actual player pose without advancing its clock");
        std::uint32_t next_source_texture = 1;
        effects::EffectTextureServices preview_texture_services;
        preview_texture_services.upload = [&](const TextureImage& image, std::uint32_t& texture,
                                              std::string& upload_error) {
            if (!image.width || !image.height || image.rgba.empty()) {
                upload_error = "Original source texture decoder returned empty pixels";
                return false;
            }
            texture = next_source_texture++;
            return true;
        };
        preview_texture_services.release = [](std::uint32_t) {};
        SourceEquipmentOriginalBindingsV1 preview_materials(weapons, assets,
            std::move(preview_texture_services), {});
        RuntimeEquipmentPreviewPacketsV1 initial_packet_frame;
        check(equipment.with_preview_packets(preview_materials.callbacks(),
            [&](const RuntimeEquipmentPreviewPacketsV1& frame, std::string& packet_error) {
                if (!frame.packets || frame.packets->packets.empty() ||
                    frame.presentation.same_scene != old_scene ||
                    frame.presentation.visual != live_visual ||
                    frame.presentation.revision != unarmed_preview.revision ||
                    !frame.presentation.source_views) {
                    packet_error = "Prepared packet frame lost the live source visual or Scene";
                    return false;
                }
                std::size_t packet_index = 0;
                for (const auto& view : *frame.presentation.source_views) {
                    if (!view.geometry || !view.materials) {
                        packet_error = "Current source draw view lacks original geometry/material rows";
                        return false;
                    }
                    for (std::size_t primitive = 0; primitive < view.geometry->primitives.size(); ++primitive) {
                        if (packet_index >= frame.packets->packets.size()) {
                            packet_error = "Source renderer omitted an actual draw-view primitive";
                            return false;
                        }
                        const auto& packet = frame.packets->packets[packet_index++];
                        const auto material_index = view.materials->at(primitive);
                        if (!packet.source_retention || packet.mesh.indices.empty() ||
                            packet.mesh.vertices.empty() || packet.source.resource_uri.empty() ||
                            packet.source.geometry_id != view.geometry->id ||
                            packet.source.material_id != view.material_table->at(material_index).id ||
                            packet.source.category != view.category || packet.source.module != view.module ||
                            packet.source.weapon_slot != view.weapon_slot ||
                            packet.source.primitive_index != primitive ||
                            packet.source.pose_revision != view.pose_revision || packet.world != view.world) {
                            packet_error = "Prepared packet changed actual source geometry/pose/material provenance";
                            return false;
                        }
                    }
                }
                if (packet_index != frame.packets->packets.size()) {
                    packet_error = "Prepared source packet count includes non-source primitives";
                    return false;
                }
                initial_packet_frame = frame;
                return true;
            }, error), error);
        check(initial_packet_frame.packets && !initial_packet_frame.packets->packets.empty() &&
              initial_packet_frame.presentation.pane.left == 155.05f &&
              initial_packet_frame.presentation.camera.aspectRatio > 0.85f &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Actual source packet preparation changed the authored camera or advanced paused pose time");
        auto capture_preview_packets = [&](RuntimeEquipmentPreviewPacketsV1& output,
                                           std::uint64_t expected_revision) {
            return equipment.with_preview_packets(preview_materials.callbacks(),
                [&](const RuntimeEquipmentPreviewPacketsV1& frame, std::string& packet_error) {
                    if (!frame.packets || frame.packets->packets.empty() ||
                        frame.presentation.same_scene != old_scene ||
                        frame.presentation.visual != live_visual ||
                        frame.presentation.revision != expected_revision) {
                        packet_error = "Source packet refresh lost same-session preview identity";
                        return false;
                    }
                    output = frame;
                    return true;
                }, error);
        };
        auto packet_modules = [](const RuntimeEquipmentPreviewPacketsV1& frame) {
            std::vector<std::pair<std::int32_t, std::int32_t>> result;
            if (frame.packets) for (const auto& packet : frame.packets->packets)
                if (!packet.source.weapon_slot)
                    result.emplace_back(packet.source.category, packet.source.module);
            return result;
        };
        const std::vector<std::string> initial_debug_tail{
            "load", "query:isTracingChar_Modular", "load", "query:isTracingChar_Modular",
            "load", "query:isTracingChar_Modular", "load", "query:isTracingChar_Modular"};
        check(source_debug_events.size() == actual_bind_debug_start + initial_debug_tail.size() &&
              std::equal(initial_debug_tail.begin(), initial_debug_tail.end(),
                  source_debug_events.begin() + static_cast<std::ptrdiff_t>(actual_bind_debug_start)),
              "Initial source modular refresh did not call actual Debug services in source category order");
        RuntimeEquipmentRenderChangeV1 initial_unarmed_receipt;
        check(equipment.take_render_change(initial_unarmed_receipt, error) && error.empty() &&
              initial_unarmed_receipt.revision == 1 &&
              initial_unarmed_receipt.actor_id == config.playerId &&
              initial_unarmed_receipt.same_session_visual == live_visual &&
              initial_unarmed_receipt.same_scene == live_visual->retained_scene_borrow() &&
              initial_unarmed_receipt.attachments &&
              initial_unarmed_receipt.attachments->attachments().empty(),
              "Initial restore bind did not publish an empty same-session render receipt");
        check(same_character(character, unarmed_state_before_bind) &&
              same_actor(*live_actor, unarmed_actor_before_bind) &&
              same_combat(*live_combat, unarmed_combat_before_bind),
              "Initial empty render staging mutated inventory, actor or source properties");
        auto expect_debug_refresh_suffix = [&](std::size_t previous_size) {
            const std::vector<std::string> expected{
                "load", "query:isTracingChar_Modular", "load", "query:isTracingChar_Modular",
                "load", "query:isTracingChar_Modular", "load", "query:isTracingChar_Modular"};
            check(source_debug_events.size() == previous_size + expected.size() &&
                  std::equal(expected.begin(), expected.end(),
                      source_debug_events.begin() + static_cast<std::ptrdiff_t>(previous_size)),
                  "Source modular refresh did not preserve Load/GetSwitch call order");
        };
        const auto source_debug_before_suit = source_debug_events.size();
        check(equipment.equip_to_slot("source-suit", 0, error), error);
        expect_debug_refresh_suffix(source_debug_before_suit);
        RuntimeEquipmentRenderChangeV1 suit_receipt;
        check(equipment.take_render_change(suit_receipt, error) && error.empty() &&
              suit_receipt.revision == 2 && suit_receipt.same_scene == old_scene,
              "Same-session suit equip did not publish its refreshed render revision");
        RuntimeEquipmentPreviewFrameV1 suited_preview;
        std::vector<std::pair<std::int32_t, std::int32_t>> suited_modules;
        check(inspect_preview(suited_preview, suited_modules), error);
        const double clock_after_suit = live_visual->animation_elapsed_seconds();
        check(suited_preview.revision == 2 && suited_modules != naked_modules &&
              live_visual->retained_scene_borrow() == old_scene &&
              clock_after_suit == preview_clock_before,
              "Equipped preview did not show source suit: rev=" + std::to_string(suited_preview.revision) +
              " module_count=" + std::to_string(suited_modules.size()) + " changed=" +
              std::to_string(suited_modules != naked_modules) + " scene=" +
              std::to_string(live_visual->retained_scene_borrow() == old_scene) + " clock=" +
              std::to_string(clock_after_suit) + "/" + std::to_string(preview_clock_before));
        RuntimeEquipmentPreviewPacketsV1 suited_packet_frame;
        check(capture_preview_packets(suited_packet_frame, suited_preview.revision), error);
        check(packet_modules(suited_packet_frame) != packet_modules(initial_packet_frame) &&
              suited_packet_frame.packets->packets.size() > 0 &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Prepared packets did not refresh to the actual equipped modular source rows");
        const auto source_debug_before_unequip = source_debug_events.size();
        check(equipment.unequip(0, error), error);
        expect_debug_refresh_suffix(source_debug_before_unequip);
        RuntimeEquipmentRenderChangeV1 naked_receipt;
        check(equipment.take_render_change(naked_receipt, error) && error.empty() &&
              naked_receipt.revision == 3 && naked_receipt.same_scene == old_scene,
              "Same-session suit unequip did not publish its refreshed render revision");
        RuntimeEquipmentPreviewFrameV1 unequipped_preview;
        std::vector<std::pair<std::int32_t, std::int32_t>> unequipped_modules;
        check(inspect_preview(unequipped_preview, unequipped_modules), error);
        check(unequipped_preview.revision == 3 &&
              unequipped_modules == naked_modules &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Unequipped preview did not restore the original naked module without advancing pose time");
        RuntimeEquipmentPreviewPacketsV1 unequipped_packet_frame;
        check(capture_preview_packets(unequipped_packet_frame, unequipped_preview.revision), error);
        check(packet_modules(unequipped_packet_frame) == packet_modules(initial_packet_frame) &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Prepared packets did not return to the original naked module set after unequip");
        const auto debug_events_before_prefix_failure = source_debug_events.size();
        *source_debug_fail_at = *source_debug_query_count + 2;
        check(!equipment.equip_to_slot("source-suit", 0, error) &&
              error == "fixture source Debug query failure" &&
              character.equipment.size() == 1 && live_actor->equipment.size() == 1,
              "Reached source Debug query failure hid the committed gear state or source error");
        *source_debug_fail_at = 0;
        const std::vector<std::string> failed_prefix_calls{
            "load", "query:isTracingChar_Modular", "load", "query:isTracingChar_Modular"};
        check(source_debug_events.size() == debug_events_before_prefix_failure + failed_prefix_calls.size() &&
              std::equal(failed_prefix_calls.begin(), failed_prefix_calls.end(),
                  source_debug_events.begin() + static_cast<std::ptrdiff_t>(debug_events_before_prefix_failure)),
              "Failed source appearance did not stop at the exact reached Debug call prefix");
        RuntimeEquipmentRenderChangeV1 prefix_failure_receipt;
        check(equipment.take_render_change(prefix_failure_receipt, error) && error.empty() &&
              prefix_failure_receipt.revision == 4 && prefix_failure_receipt.same_scene == old_scene,
              "Source setter failure lost the already-committed render revision");
        RuntimeEquipmentPreviewFrameV1 prefix_preview;
        std::vector<std::pair<std::int32_t, std::int32_t>> prefix_modules;
        check(inspect_preview(prefix_preview, prefix_modules), error);
        check(prefix_modules != naked_modules &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Reached source setter prefix was rolled back or advanced the paused visual clock");
        check(equipment.unequip(0, error), error);
        RuntimeEquipmentRenderChangeV1 prefix_recovery_receipt;
        check(equipment.take_render_change(prefix_recovery_receipt, error) && error.empty() &&
              prefix_recovery_receipt.revision == 5,
              "Source prefix recovery did not publish the next render revision");
        RuntimeEquipmentPreviewFrameV1 prefix_recovered_preview;
        std::vector<std::pair<std::int32_t, std::int32_t>> prefix_recovered_modules;
        check(inspect_preview(prefix_recovered_preview, prefix_recovered_modules), error);
        check(prefix_recovered_modules == naked_modules &&
              live_visual->animation_elapsed_seconds() == preview_clock_before,
              "Source appearance recovery failed to restore the same original module set");
        auto* composed_presenter = equipment.presenter_for_composition(error);
        auto menu_actions = equipment.menu_actions(error);
        check(composed_presenter && menu_actions.bound() && !menu_actions.partial(),
              "Runtime binding did not expose ready equipment menu actions");
        const auto* table = equipment.item_table(error);
        check(table != nullptr, error);
        const auto sword_id = dh2::data::item_id(*table, "Longsword01");
        check(sword_id >= 0, "Longsword01 absent from loaded runtime source ItemTable");
        const auto* sword_row = equipment.item_for_instance("source-sword", error);
        check(sword_row == &table->rows[static_cast<std::size_t>(sword_id)],
              "Runtime item lookup did not return the actual ItemTable row");
        check(equipment.select_slot(1, error), error);
        std::vector<OwnedSelection> choices;
        check(equipment.selected_items(choices, error), error);
        check(choices.size() == 2 && choices.front().instance_id == "source-sword" &&
              choices.front().applicable_to_selected_slot && choices.back().instance_id == "source-sword-2",
              "Actual ItemTable/main-hand slot selection did not expose the owned source sword");
        check(equipment.select_instance("source-sword", error), error);

        unsigned checked_projection_rows = 0;
        bool reject_source_projection = false;
        page_bindings.source_instance_current = [&](const InventoryItem& owned,
                const dh2::data::Item& item, std::string& check_error) {
            ++checked_projection_rows;
            if (reject_source_projection) {
                check_error = "fixture source inventory resolver rejected stale instance lease";
                return false;
            }
            if (dh2::data::item(*table,
                    dh2::data::item_id(*table, owned.definition_id)) != &item) {
                check_error = "source projection item identity changed";
                return false;
            }
            check_error.clear(); return true;
        };
        auto page = std::make_shared<RuntimeEquipmentPageV1>();
        check(page->bind(equipment, character, std::move(page_bindings), error), error);
        const auto session_lease = session.actor_binding_lease().lock();
        check(session_lease != nullptr, "Runtime equipment source composition lacks the live session owner lease");
        std::shared_ptr<void> source_owner(session_lease, const_cast<void*>(session_lease.get()));
        character_menu::SourceCompositionV1 composition(source_owner);
        character_menu::SourcePageProviderV1 equipment_provider;
        check(page->source_page_provider(source_owner, equipment_provider, error), error);
        check(equipment_provider.owner.get() == source_owner.get() &&
              equipment_provider.ready && equipment_provider.append && equipment_provider.release,
              "Runtime equipment page did not expose its same-owner source composition provider");
        check(composition.register_page(character_menu::Tab::equipment,
              std::move(equipment_provider), error), error);
        equipment_provider = {};
        character_menu::Bindings composition_bindings;
        composition_bindings.character = &character;
        unsigned other_tab_calls = 0;
        composition_bindings.content = [&](character_menu::Tab,
                character_menu::Frame&, std::string& callback_error) {
            ++other_tab_calls; callback_error.clear(); return true;
        };
        check(composition.install_content(composition_bindings, error), error);
        character_menu::Presenter composition_menu;
        composition_menu.open();
        check(composition.select(composition_menu, character_menu::Tab::equipment, error), error);
        check(composition_menu.tab() == character_menu::Tab::equipment &&
              composition_bindings.character == &character,
              "Registered runtime page did not select over the same menu CharacterState");
        reject_source_projection = true;
        character_menu::Frame rejected_projection_frame;
        check(!page->content(character_menu::Tab::equipment, rejected_projection_frame, error) &&
              error == "fixture source inventory resolver rejected stale instance lease" &&
              character.equipment.empty() && live_actor->equipment.empty(),
              "Runtime equipment page ignored a failed source inventory identity check");
        reject_source_projection = false;
        character_menu::Frame composed;
        check(composition_bindings.content(character_menu::Tab::stats, composed, error), error);
        check(other_tab_calls == 1, "Runtime equipment page intercepted an unrelated character tab");
        composed.art.batches = character_menu::original_menu_art(character_menu::Tab::equipment).batches;
        check(composition_bindings.content(character_menu::Tab::equipment, composed, error), error);
        check(!composed.art.batches.empty() && !composed.text.empty() && checked_projection_rows >= 4,
              "Runtime page did not compose original equipment art, source names and live inventory projection checks");
        const auto potion_slot = std::find_if(inventory::original_inventory_slots().begin(),
            inventory::original_inventory_slots().end(), [](const auto& art) {
                return art.source_slot == 9;
            });
        check(potion_slot != inventory::original_inventory_slots().end(),
              "Original inventory layout has no potion row");
        for (const auto& field : potion_slot->fields) {
            const auto rendered = std::find_if(composed.text.begin(), composed.text.end(),
                [&](const auto& value) { return value.field.path == field.path; });
            check(rendered != composed.text.end() && rendered->value == source_potion_text,
                  "Runtime page did not use the original potion formatter for the actual inventory quantity");
        }
        const auto& slot_hit = *std::find_if(original_slot_art().begin(), original_slot_art().end(),
            [](const auto& art) { return art.source_slot == 1; });
        const float slot_x = (slot_hit.hit_triangles[0].x + slot_hit.hit_triangles[1].x + slot_hit.hit_triangles[2].x) / 3;
        const float slot_y = (slot_hit.hit_triangles[0].y + slot_hit.hit_triangles[1].y + slot_hit.hit_triangles[2].y) / 3;
        RuntimeEquipmentPageReleaseV1 page_release;
        check(page->release(slot_x, slot_y, page_release, error), error);
        check(page_release.command == MainPageCommand::none && !page_release.has_render_change &&
              composed_presenter->selected_slot() == 1 &&
              composed_presenter->selected_instance() == "source-sword",
              "Runtime page did not open Details over the shared equipment selection");
        const auto& next_hit = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::next;
            });
        const float next_x = (next_hit.triangles[0].x + next_hit.triangles[1].x + next_hit.triangles[2].x) / 3;
        const float next_y = (next_hit.triangles[0].y + next_hit.triangles[1].y + next_hit.triangles[2].y) / 3;
        check(page->release(next_x, next_y, page_release, error), error);
        check(page_release.command == MainPageCommand::none && !page_release.has_render_change &&
              composed_presenter->selected_instance() == "source-sword-2",
              "Original Details next-instance action did not update the single source selection");
        composed = {};
        composed.art.batches = character_menu::original_menu_art(character_menu::Tab::equipment).batches;
        check(composition_bindings.content(character_menu::Tab::equipment, composed, error), error);
        check(std::any_of(composed.art.batches.begin(), composed.art.batches.end(), [](const auto& batch) {
            return batch.role.find("menu_InventorySheetDetails/") == 0;
        }), "Runtime equipment page did not compose original Details panel art");
        const auto selected_name_field = std::find_if(composed.text.begin(), composed.text.end(),
            [](const auto& text) {
                return text.field.path.find("menu_InventorySheetDetails/SelectedItemName/") == 0;
            });
        const auto selected_stats_field = std::find_if(composed.text.begin(), composed.text.end(),
            [](const auto& text) {
                return text.field.path.find("menu_InventorySheetDetails/ItemInfo1/") == 0;
            });
        check(selected_name_field != composed.text.end() &&
              selected_name_field->value == source_sword_name &&
              selected_stats_field != composed.text.end() &&
              selected_stats_field->value != "source-sword-2",
              "Runtime Details did not use the same original ItemTable/HudText provider for the selected item");
        const auto& equip_hit = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::equip;
            });
        const float equip_x = (equip_hit.triangles[0].x + equip_hit.triangles[1].x + equip_hit.triangles[2].x) / 3;
        const float equip_y = (equip_hit.triangles[0].y + equip_hit.triangles[1].y + equip_hit.triangles[2].y) / 3;

        check(composition.release(composition_menu, equip_x, equip_y, 480, 320, error) ==
              character_menu::Action::none, error);
        RuntimeEquipmentRenderChangeV1 receipt;
        check(page->take_source_render_change(receipt, error),
              "Runtime source composition equip unexpectedly omitted its typed receipt");
        check(error.empty(), "Runtime source composition receipt retrieval failed");
        check(receipt.actor_id == config.playerId && receipt.same_session_visual == live_visual &&
              receipt.same_scene == old_scene && receipt.attachments &&
              receipt.attachments->attachments().size() == 1,
              "Runtime source composition equip did not return its same-binding render receipt");
        check(session.actor(config.playerId) == live_actor &&
              session.world()->combat_properties(config.playerId) == live_combat &&
              character.equipment.size() == 1 && live_actor->equipment.size() == 1 &&
              character.equipment.front().slot == "slot1" &&
              character.equipment.front().equipment_set == 0 &&
              character.equipment.front().source_slot == 1 &&
              character.equipment.front().item_instance_id == "source-sword-2" &&
              live_actor->equipment.front().definition_id == "Longsword01",
              "Runtime equip replaced or diverged the shared CharacterState/session actor");
        const auto* updated = session.world()->combat_properties(config.playerId);
        check(updated->facts.main_damage_class == sword_row->record.words[37] &&
              updated->facts.off_damage_class == -1 &&
              updated->sheets.gear[79] == 3072 && updated->sheets.gear[80] == 3840,
              "Runtime equip did not publish actual source main-hand facts and gear sheets");
        check(live_visual->retained_scene_borrow() == old_scene,
              "Runtime source attachment binding replaced the CombatSession Scene");
        check(receipt.actor_id == config.playerId && receipt.same_session_visual == live_visual &&
              receipt.same_scene == old_scene && receipt.attachments &&
              receipt.attachments->attachments().size() == 1 &&
              receipt.attachments->attachments().front().definition.model_uri.find("MC_RWeapon_Longsword_01.bdae") != std::string::npos,
              "Runtime equip omitted the same-session source weapon render receipt");
        Mat4 exact_socket{};
        const Mat4 identity_matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        check(live_visual->socket_world("anchor_weapon_right_offset", identity_matrix, exact_socket, error), error);
        check(receipt.attachments->attachments().front().socket_world == exact_socket,
              "Runtime weapon attachment guessed an offset instead of borrowing the live source socket");

        const auto& drop_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::drop;
            });
        const float drop_x = (drop_action.triangles[0].x + drop_action.triangles[1].x + drop_action.triangles[2].x) / 3;
        const float drop_y = (drop_action.triangles[0].y + drop_action.triangles[1].y + drop_action.triangles[2].y) / 3;
        const auto before_drop_request = character;
        check(composition.release(composition_menu, drop_x, drop_y, 480, 320, error) ==
              character_menu::Action::none, error);
        check(composition.release(composition_menu, drop_x, drop_y, 480, 320, error) ==
              character_menu::Action::none &&
              error.find("Consume the previous typed source inventory command") != std::string::npos,
              "Source composition silently overwrote an unconsumed drop command");
        RuntimeEquipmentPageReleaseV1::PendingCommand pending;
        check(page->take_source_pending_command(pending, error) && error.empty() &&
              pending.command == MainPageCommand::request_drop &&
              pending.selected_instance_id == "source-sword-2" && pending.source_slot == 1,
              "SourceComposition lost drop action identity or selected slot");
        check(same_character(character, before_drop_request),
              "Typed drop request mutated the shared CharacterState before world-owner handling");

        check(equipment.select_instance("source-sword", error), error);
        const auto& auto_equip_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::auto_equip;
            });
        const float auto_x = (auto_equip_action.triangles[0].x + auto_equip_action.triangles[1].x + auto_equip_action.triangles[2].x) / 3;
        const float auto_y = (auto_equip_action.triangles[0].y + auto_equip_action.triangles[1].y + auto_equip_action.triangles[2].y) / 3;
        check(composition.release(composition_menu, auto_x, auto_y, 480, 320, error) ==
              character_menu::Action::none, error);
        check(page->take_source_pending_command(pending, error) && error.empty() &&
              pending.command == MainPageCommand::request_auto_equip &&
              pending.selected_instance_id == "source-sword" && pending.source_slot == 1,
              "SourceComposition lost auto-equip request identity or selected slot");
        const auto equipment_before_auto = character.equipment;
        check(equipment.auto_equip(pending.selected_instance_id, error), error);
        RuntimeEquipmentRenderChangeV1 auto_receipt;
        check(equipment.take_render_change(auto_receipt, error) && error.empty() &&
              auto_receipt.same_scene == old_scene && auto_receipt.attachments &&
              character.equipment.size() == 1 &&
              character.equipment.front().equipment_set == 0 &&
              character.equipment.front().source_slot == 1 &&
              character.equipment.front().item_instance_id == "source-sword" &&
              equipment_before_auto.front().item_instance_id == "source-sword-2",
              "Typed auto-equip was not consumable through the source EquipmentAdapter auto kernel");

        const auto& transmute_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::transmute;
            });
        const float transmute_x = (transmute_action.triangles[0].x + transmute_action.triangles[1].x + transmute_action.triangles[2].x) / 3;
        const float transmute_y = (transmute_action.triangles[0].y + transmute_action.triangles[1].y + transmute_action.triangles[2].y) / 3;
        check(composition.release(composition_menu, transmute_x, transmute_y, 480, 320, error) ==
              character_menu::Action::none, error);
        check(page->take_source_pending_command(pending, error) && error.empty() &&
              pending.command == MainPageCommand::request_transmute &&
              pending.selected_instance_id == "source-sword" && pending.source_slot == 1,
              "SourceComposition lost transmute request identity or selected slot");
        check(character.equipment.size() == 1 && character.equipment.front().item_instance_id == "source-sword",
              "Transmute request silently mutated equipment before its owner was connected");

        {
            const auto equipped_character_before_bind = character;
            const auto equipped_actor_before_bind = *live_actor;
            const auto equipped_combat_before_bind = *live_combat;
            RuntimeEquipmentBindingV1 restored_equipment;
            check(restored_equipment.bind(session, character, assets, assets, weapons,
                  database, restore_options, error), error);
            const double restore_preview_clock = live_visual->animation_elapsed_seconds();
            RuntimeEquipmentPreviewFrameV1 restored_preview;
            check(restored_equipment.with_preview_borrow(
                      [&](const RuntimeEquipmentPreviewBorrowV1& borrowed, std::string& callback_error) {
                          EquipmentPreviewSourceV1 source;
                          source.revision = borrowed.revision;
                          source.actor_id = borrowed.actor_id;
                          source.class_id = borrowed.class_id;
                          source.visual = borrowed.visual;
                          source.same_scene = borrowed.same_scene;
                          source.source_views = borrowed.source_views;
                          source.attachments = borrowed.attachments;
                          return compose_runtime_equipment_preview_frame_v1(source, restored_preview,
                                                                             callback_error);
                      }, error), error);
            check(restored_preview.revision == 1 && restored_preview.same_scene == old_scene &&
                  restored_preview.attachments->attachments().size() == 1 &&
                  std::any_of(restored_preview.source_views->begin(), restored_preview.source_views->end(),
                      [](const auto& view) { return view.weapon_slot == 1; }) &&
                  live_visual->animation_elapsed_seconds() == restore_preview_clock,
                  "Restored equipped Session preview lost its actual weapon view/attachment or advanced the pose clock");
            RuntimeEquipmentRenderChangeV1 restored_receipt;
            check(restored_equipment.take_render_change(restored_receipt, error) && error.empty() &&
                  restored_receipt.revision == 1 &&
                  restored_receipt.attachments &&
                  restored_receipt.attachments->attachments().size() == 1 &&
                  restored_receipt.attachments->attachments().front().definition.model_uri.find(
                      "MC_RWeapon_Longsword_01.bdae") != std::string::npos,
                  "Rebinding an equipped source state did not stage its actual weapon render model");
            check(same_character(character, equipped_character_before_bind) &&
                  same_actor(*live_actor, equipped_actor_before_bind) &&
                  same_combat(*live_combat, equipped_combat_before_bind),
                  "Initial equipped render staging mutated inventory, actor or source properties");
        }

        const auto saved_character = character;
        const auto saved_actor = *live_actor;
        const auto saved_combat = *updated;
        check(!equipment.equip_to_slot("missing-source-instance", 1, error),
              "Missing source instance was accepted by the live equipment binding");
        check(same_character(character, saved_character) && same_actor(*live_actor, saved_actor) &&
              same_combat(*live_combat, saved_combat),
              "Rejected runtime transaction mutated shared CharacterState, world actor or combat sheets");
        RuntimeEquipmentRenderChangeV1 absent_receipt;
        check(!equipment.take_render_change(absent_receipt, error) && error.empty(),
              "Rejected runtime transaction emitted a render-model-change receipt");
        reject_suit_power = true;
        check(!equipment.equip_to_slot("source-suit", 0, error) &&
              error == "fixture source power provider rejected",
              "Source power preparation rejection was not propagated");
        check(same_character(character, saved_character) && same_actor(*live_actor, saved_actor) &&
              same_combat(*session.world()->combat_properties(config.playerId), saved_combat),
              "Rejected source power preparation changed shared actor/properties");
        check(!equipment.take_render_change(absent_receipt, error) && error.empty(),
              "Rejected source power preparation emitted a render receipt");

        const auto& unequip_hit = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::unequip;
            });
        const float unequip_x = (unequip_hit.triangles[0].x + unequip_hit.triangles[1].x + unequip_hit.triangles[2].x) / 3;
        const float unequip_y = (unequip_hit.triangles[0].y + unequip_hit.triangles[1].y + unequip_hit.triangles[2].y) / 3;
        check(composition.release(composition_menu, unequip_x, unequip_y, 480, 320, error) ==
              character_menu::Action::none, error);
        check(page->take_source_render_change(page_release.render_change, error) && error.empty(),
              "Runtime source composition unequip omitted its typed render receipt");
        page_release.has_render_change = true;
        check(character.equipment.empty() && live_actor->equipment.empty() &&
              session.world()->combat_properties(config.playerId)->facts.main_damage_class == -1 &&
              session.world()->combat_properties(config.playerId)->sheets.gear ==
                  rules.defaults,
              "Runtime unequip did not clear the same player gear/projection");
        check(page_release.render_change.attachments->attachments().empty(),
              "Runtime page unequip receipt did not contain the empty source render model");
        RuntimeEquipmentRenderChangeV1 consumed;
        check(!page->take_source_render_change(consumed, error) && error.empty(),
              "Runtime composition emitted a duplicate render receipt");
        check(composition.release(composition_menu, equip_x, equip_y, 480, 320, error) ==
              character_menu::Action::none, error);
        check(page->take_source_render_change(page_release.render_change, error) && error.empty() &&
              character.equipment.size() == 1 && character.equipment.front().slot == "slot1" &&
              character.equipment.front().equipment_set == 0 &&
              character.equipment.front().source_slot == 1 &&
              character.equipment.front().item_instance_id == "source-sword" &&
              page_release.render_change.attachments->attachments().size() == 1,
              "Source page re-equip did not restore active set/slot binding metadata and render receipt");
        page->leave_page();
        std::weak_ptr<RuntimeEquipmentPageV1> page_lifetime = page;
        page.reset();
        check(!page_lifetime.expired(), "SourceComposition did not retain the registered equipment page");
        composition_bindings = {};
        composition = character_menu::SourceCompositionV1(std::shared_ptr<void>{});
        check(page_lifetime.expired(), "Released source composition retained the equipment page after teardown");

        CharacterVisual retired_session_visual;
        ActorPopulation retired_session_population;
        CharacterState retired_session_character;
        RuntimeEquipmentBindingV1 retired_session_binding;
        {
            auto doomed_session = std::make_unique<CombatSession>();
            check(doomed_session->initialize(assets, database, melee, config,
                  retired_session_visual, retired_session_population, {0, 0, 0},
                  customization, error), error);
            const auto* doomed_actor = doomed_session->actor(config.playerId);
            check(doomed_actor != nullptr, "Retired-session fixture has no actual player actor");
            retired_session_character = make_default_character(
                "retired-runtime-equipment", "Retired Session Player", doomed_actor->definition_id);
            RuntimeEquipmentOptionsV1 retired_options;
            retired_options.source_appearance_debug = test_source_debug();
            check(retired_session_binding.bind(*doomed_session, retired_session_character,
                  assets, assets, weapons, database, std::move(retired_options), error), error);
            doomed_session.reset();
        }
        check(!retired_session_binding.ready(error) && error.find("actor lease") != std::string::npos,
              "Equipment ready() dereferenced its destroyed CombatSession instead of rejecting its expired lease");
        check(retired_session_binding.item_table(error) == nullptr &&
              error.find("actor lease") != std::string::npos,
              "Equipment source-table query dereferenced its destroyed CombatSession instead of rejecting its expired lease");
        bool expired_preview_callback_called = false;
        check(!retired_session_binding.with_preview_borrow(
                  [&](const RuntimeEquipmentPreviewBorrowV1&, std::string&) {
                      expired_preview_callback_called = true;
                      return true;
                  }, error) && error.find("actor lease") != std::string::npos &&
              !expired_preview_callback_called,
              "Equipment preview touched its source owner before rejecting the expired Session lease");
        bool expired_packet_callback_called = false;
        check(!retired_session_binding.with_preview_packets({},
                  [&](const RuntimeEquipmentPreviewPacketsV1&, std::string&) {
                      expired_packet_callback_called = true;
                      return true;
                  }, error) && error.find("actor lease") != std::string::npos &&
              !expired_packet_callback_called,
              "Equipment packet preview accessed source state before rejecting the expired Session lease");

        // The native session lease is the lifetime gate. Detach invalidates
        // the binding before a potentially replacing world/visual owner can be used.
        session.detach_for_restore();
        check(!equipment.ready(error) && error.find("actor lease") != std::string::npos,
              "Runtime equipment continued borrowing a detached CombatSession graph");
        std::cout << "PASS same CombatSession player/world/CharacterState, original MAINPAGE/Details composition, "
                     "typed drop/auto-equip/transmute source requests, existing AutoEquip kernel and render receipt, "
                     "source modular preview transitions/prefix failure/recovery, same-Scene actual source packet preparation and provenance, "
                     "same-Scene draw_views/gear borrow, "
                     "equipped rebind and unchanged pose clock, "
                     "slot-driven equip/unequip receipts, source socket identity, rollback and detach lease gate\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
