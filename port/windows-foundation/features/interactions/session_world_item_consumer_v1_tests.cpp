#include "session_world_item_consumer_v1.hpp"
#include "source_world_item_drop_render_v1.hpp"
#include "source_world_item_drop_material_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../texture_loader.hpp"
#include "../inventory/inventory_feature.hpp"
#include "../../../game-data/loot_table_selection_v8.hpp"
#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_resources_v7.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>

namespace {
using namespace dh2::data;
using namespace dh::foundation;
using namespace dh::foundation::loot;

void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
Bytes view(const std::vector<std::uint8_t>& bytes) { return {bytes.data(), bytes.size()}; }
std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    check(bool(file), "missing source cache file: " + path.string());
    return {std::istreambuf_iterator<char>(file), {}};
}

bool entry(void*, const LootEntryRequestV8& request, std::int32_t& value,
           std::string& error) {
    if (request.operation == LootEntryOperationV8::debug_load ||
        request.operation == LootEntryOperationV8::debug_query ||
        request.operation == LootEntryOperationV8::mage_count ||
        request.operation == LootEntryOperationV8::rogue_count ||
        request.operation == LootEntryOperationV8::warrior_count) {
        value = request.operation == LootEntryOperationV8::warrior_count ? 1 : 0;
        return true;
    }
    error = "Unsupported loot entry service in consumer fixture";
    return false;
}

struct PlayerContext { ActorId id{}; std::shared_ptr<CharacterState> state; };
bool resolve_player(void* raw, ActorId id, std::shared_ptr<CharacterState>& out,
                    std::string& error) {
    auto* context = static_cast<PlayerContext*>(raw);
    if (!context || id != context->id || !context->state) {
        error = "Consumer fixture could not resolve current CharacterState";
        return false;
    }
    out = context->state;
    error.clear();
    return true;
}

bool same_inventory(const CharacterState& a, const CharacterState& b) {
    if (a.inventory.size() != b.inventory.size() || a.gold != b.gold) return false;
    for (std::size_t i = 0; i < a.inventory.size(); ++i) {
        if (a.inventory[i].instance_id != b.inventory[i].instance_id ||
            a.inventory[i].definition_id != b.inventory[i].definition_id ||
            a.inventory[i].quantity != b.inventory[i].quantity) return false;
    }
    return true;
}

struct Outcomes { std::vector<LootItemInfoV8> items; };
struct CpuTextureLeasesV1 {
    std::uint32_t next{1};
    std::map<std::uint32_t, TextureImage> images;
    std::size_t releases{};
    bool upload(const TextureImage& image, std::uint32_t& id, std::string& error) {
        if (!image.width || !image.height ||
            image.rgba.size() != std::size_t(image.width) * image.height * 4) {
            error = "CPU source-texture lease received invalid decoded source pixels";
            return false;
        }
        id = next++;
        images.emplace(id, image);
        return true;
    }
    void release(std::uint32_t id) { if (images.erase(id)) ++releases; }
};
std::int32_t named_item(const LootTablesV2::Borrow& tables, const char* name) {
    const auto found = std::find(tables.items().identifiers.begin(),
                                 tables.items().identifiers.end(), name);
    return found == tables.items().identifiers.end() ? -1 :
        static_cast<std::int32_t>(std::distance(tables.items().identifiers.begin(), found));
}
}

int main(int argc, char** argv) try {
    check(argc == 6, "supply original cache, Loot power cache, ItemAudioVisual cache, itemdrop root, and isolated source-effect root");
    AssetCatalog assets(argv[1]);
    const auto read_actual = [&](const char* name) {
        return assets.read(std::filesystem::path("original-cache/data/pydata") / name);
    };
    const auto item_records = read_actual("loot_table_pyarray.bin");
    const auto item_names = read_actual("loot_table_pyarraynames.bin");
    const auto item_schema = read_actual("loot_table_pystructnames.bin");
    LootTablesV2 loot_owner;
    std::string error;
    check(loot_owner.load(view(item_records), view(item_names), view(item_schema), error), error);
    const auto tables = loot_owner.borrow();

    const std::filesystem::path power_root(argv[2]);
    std::vector<std::vector<std::uint8_t>> power_files;
    for (const auto* name : {"item_powers_pyarray.bin", "item_powers_pyarraynames.bin",
             "item_powers_pystructnames.bin", "item_powers_monopoly_pyarray.bin",
             "item_powers_monopoly_pyarraynames.bin", "item_powers_monopoly_pystructnames.bin",
             "num_prob_records_v7.bin", "loot_table_pyarraynames.bin",
             "loot_table_pystructnames.bin"})
        power_files.push_back(read_file(power_root / name));
    ItemPowerTablesV5 definitions;
    check(definitions.load(view(power_files[0]), view(power_files[1]), view(power_files[2]), error), error);
    LootPowerInputsV7 power_inputs{view(power_files[0]), view(power_files[1]), view(power_files[2]),
        view(power_files[3]), view(power_files[4]), view(power_files[5]), view(power_files[6]),
        view(power_files[7]), view(power_files[8])};
    LootPowerResourcesV7 power_owner;
    check(power_owner.load(power_inputs, definitions.borrow(), error), error);

    const std::filesystem::path av_root(argv[3]);
    const auto av_records = read_file(av_root / "loot_audiovisual_pyarray.bin");
    const auto av_names = read_file(av_root / "loot_audiovisual_pyarraynames.bin");
    const auto av_schema = read_file(av_root / "loot_audiovisual_pystructnames.bin");
    LootAudioVisualV8 audiovisual_owner;
    check(audiovisual_owner.load(view(av_records), view(av_names), view(av_schema), error), error);
    const auto audiovisual = audiovisual_owner.borrow();

    const auto barrel = std::find(tables.loot_names().begin(), tables.loot_names().end(),
                                  "Barrel_Level_01");
    check(barrel != tables.loot_names().end(), "Actual Barrel_Level_01 table is absent");
    const auto loot_id = static_cast<std::int32_t>(std::distance(tables.loot_names().begin(), barrel));
    const auto potion_id = named_item(tables, "Potion0");
    const auto gold_id = named_item(tables, "GoldStack01");
    check(potion_id == 925 && gold_id == 418, "Original Potion0/GoldStack01 ItemTable IDs changed");

    // Find one actual source table9 expansion containing both item families.
    // The test keeps the same RNG alive through gold's source creation value.
    std::vector<LootItemInfoV8> selected;
    LootRandom8V2 random{};
    std::uint32_t seed{};
    for (std::uint32_t candidate = 1; candidate < 10000 && selected.empty(); ++candidate) {
        LootRandom8V2 attempt{candidate, 0};
        LootTableSelectionV8 selector(tables, power_owner.borrow(), attempt, {nullptr, entry});
        std::vector<const LootEntry32V2*> entries;
        if (!selector.select(loot_id, entries, error)) continue;
        LootItemSelectionV8 expansion(tables, attempt, {nullptr, entry});
        std::vector<LootItemInfoV8> items;
        if (!expansion.expand(entries, false, items, error)) continue;
        const bool has_potion = std::any_of(items.begin(), items.end(), [&](const auto& item) {
            return item.id == potion_id;
        });
        const bool has_gold = std::any_of(items.begin(), items.end(), [&](const auto& item) {
            return item.id == gold_id;
        });
        if (has_potion && has_gold) { selected = std::move(items); random = attempt; seed = candidate; }
    }
    check(!selected.empty(), "No source table9 outcome contained Potion0 and GoldStack01 together");

    RuntimeWorldItemAdapterV1 store(tables);
    ActorDefinition source_definition;
    source_definition.stableId = 51;
    source_definition.name = "Swamp_Normal_DestructibleBarrel";
    WorldObject source;
    source.id = 51;
    source.name = source_definition.name;
    source.transform.position = {1.25f, 3.5f, -2.0f};
    const auto item_value = [&](const LootItemInfoV8& selected_item) {
        check(selected_item.item && selected_item.entry && selected_item.quantity > 0,
              "Source selector returned an incomplete item outcome");
        RuntimeWorldItemRecordV1 record;
        record.source_actor = source.id;
        record.killer_actor = 7;
        record.loot_table = loot_id;
        record.item_id = static_cast<std::int16_t>(selected_item.id);
        record.quantity = selected_item.quantity;
        record.authored_item = selected_item.item;
        record.authored_entry = selected_item.entry;
        if (record.authored_item->record.words[22] == 13) {
            std::int32_t resolved_value{};
            // Explicit source value-bonus input 0; the original kernel consumes
            // the shared Loot RNG once and stores the resulting ItemInstance value.
            check(dh2_loot_item_value_v7(&resolved_value, &random,
                    &record.authored_item->record, nullptr, 0, 0) == 0, "Source gold value kernel failed");
            record.resolved_gold_value = resolved_value;
        }
        RuntimeWorldItemIdV1 world_id{};
        check(store.publish_source_object_drop(record, source_definition, source,
                                                world_id, error), error);
        return std::pair<RuntimeWorldItemIdV1, RuntimeWorldItemRecordV1>{world_id, record};
    };

    std::optional<std::pair<RuntimeWorldItemIdV1, RuntimeWorldItemRecordV1>> potion;
    std::optional<std::pair<RuntimeWorldItemIdV1, RuntimeWorldItemRecordV1>> gold;
    for (const auto& outcome : selected) {
        if (outcome.id == potion_id) potion = item_value(outcome);
        if (outcome.id == gold_id) gold = item_value(outcome);
    }
    check(potion && gold && store.size() == selected.size(), "Same-store source outcomes were not published");

    dh::foundation::interactions::SessionWorldItemConsumerV1 consumer(store, audiovisual);
    std::vector<dh::foundation::interactions::SessionWorldItemPresentationV1> views;
    check(consumer.enumerate(views, error), error);
    const auto find_view = [&](RuntimeWorldItemIdV1 id) -> const auto* {
        const auto found = std::find_if(views.begin(), views.end(),
            [&](const auto& view) { return view.identity == id; });
        return found == views.end() ? nullptr : &*found;
    };
    const auto* potion_view = find_view(potion->first);
    const auto* gold_view = find_view(gold->first);
    check(potion_view && gold_view, "Consumer did not enumerate both exact stored source identities");
    check(potion_view->source_record.source_actor == source.id &&
          potion_view->source_record.authored_item == potion->second.authored_item &&
          potion_view->source_record.authored_entry == potion->second.authored_entry &&
          gold_view->source_record.item_id == gold_id &&
          gold_view->source_record.resolved_gold_value == gold->second.resolved_gold_value,
          "Presentation did not preserve exact ActorId/Loot/Item source record identities");

    const auto verify_visual = [&](const auto* view, const char* id) {
        check(view->item_identifier == id, "Source item identifier changed during enumeration");
        const auto av = view->source_audio_visual_id;
        check(av >= 0 && static_cast<std::size_t>(av) < audiovisual.rows().size(),
              "Actual ItemTable AudioVisualID has no original audiovisual row");
        const auto index = static_cast<std::size_t>(av);
        check(view->source_audio_visual_name == audiovisual.names()[index] &&
              view->source_visual_uri == audiovisual.rows()[index].visual &&
              view->source_model_resource_uri ==
                  dh::foundation::interactions::source_item_model_resource_uri_v1 &&
              view->has_source_visual == !audiovisual.rows()[index].visual.empty(),
              "ItemTable AudioVisualID → source ItemAudioVisualTable URI mapping diverged");
        check(view->exact_icon_name == view->source_record.authored_item->icon_name &&
              view->has_exact_icon == !view->exact_icon_name.empty(),
              "Presentation substituted an ItemTable icon key");
    };
    verify_visual(potion_view, "Potion0");
    verify_visual(gold_view, "GoldStack01");

    // This renderer bridge uses the same records/store as the pickup proof and
    // the original staged itemdrops.bdae. The material callback below is an
    // explicit CPU texture lease (real decoded source pixels, no GPU context).
    // Source scene/material/pass identities and geometry are real assets.
    AssetCatalog itemdrop_assets(argv[4]);
    AssetCatalog source_effect_assets(argv[5]);
    std::size_t material_bindings = 0;
    std::vector<std::pair<std::string, std::string>> material_sources;
    std::vector<std::string> material_declarations;
    CpuTextureLeasesV1 texture_leases;
    dh::foundation::interactions::SourceWorldItemDropTextureServicesV1 texture_services;
    texture_services.upload = [&](const TextureImage& image, std::uint32_t& id, std::string& upload_error) {
        return texture_leases.upload(image, id, upload_error);
    };
    texture_services.release = [&](std::uint32_t id) { texture_leases.release(id); };
    dh::foundation::interactions::SourceWorldItemDropMaterialBindingsV1 original_materials(
        itemdrop_assets, std::move(texture_services), &source_effect_assets);
    std::shared_ptr<const dh::foundation::interactions::SourceWorldItemDropRenderFrameV1>
        rendered_frame;
    std::size_t render_submissions = 0;
    dh::foundation::interactions::SourceWorldItemDropRenderServicesV1 render_services;
    render_services.material = [&](const auto& source, Material& output, std::string& material_error) {
        check(source.authored_material && source.authored_scene_material && source.source_image &&
              source.authored_material->id == source.authored_scene_material->id &&
              source.resource_uri == dh::foundation::interactions::source_item_model_resource_uri_v1,
              "Render material callback lost exact source material/resource identity");
        check(source.authored_material->diffuse == "atlas_dh2_game_objects_001.tga",
              "Itemdrop source material diffuse atlas differs from the authored source table");
        if (source.visual_uri == "dummy_itemdrop_Potion")
            check(source.authored_material->alphaMap.empty(),
                  "Potion0 material unexpectedly gained an authored alpha map");
        if (source.visual_uri == "root_itemdrop_Gold_01")
            check(source.authored_material->alphaMap == "atlas_dh2_game_objects_001_alpha.tga" &&
                  source.authored_scene_material->effect_file == "GL_Diffuse_L1_VC_iPhone.bdae" &&
                  source.authored_scene_material->effect_uri == "#Multilight-fx" &&
                  source.authored_scene_material->gles2_technique == "L1_Vc_Al_Sp_----_----_----",
                  "GoldStack01 source alpha atlas differs from the authored material");
        material_sources.emplace_back(source.visual_uri, source.authored_material->id);
        if (!original_materials.bind(source, output, material_error)) return false;
        check(output.sourcePass.has_value(),
              "Exact source material declaration did not resolve a source pass");
        material_declarations.push_back(source.visual_uri + "/" +
            source.authored_scene_material->id + ":" +
            source.authored_scene_material->effect_file + ":" +
            source.authored_scene_material->effect_uri + ":" +
            source.authored_scene_material->gles2_technique + ":pass");
        ++material_bindings;
        return true;
    };
    render_services.submit = [&](auto frame, std::string&) {
        ++render_submissions;
        rendered_frame = std::move(frame);
        return true;
    };
    std::unique_ptr<dh::foundation::interactions::SourceWorldItemDropRenderV1> drop_renderer;
    check(dh::foundation::interactions::SourceWorldItemDropRenderV1::load(
        itemdrop_assets, store, audiovisual, std::move(render_services), drop_renderer, error), error);
    check(drop_renderer->prepare(rendered_frame, error), error);
    check(rendered_frame && rendered_frame->unresolved.empty() &&
          material_bindings == 3 && original_materials.texture_count() == 2 &&
          texture_leases.images.size() == 2 && rendered_frame->renderer_ready,
          "Original Potion0/GoldStack01 packets did not resolve exact source effects and decoded texture leases");
    check(drop_renderer->submit(error) && render_submissions == 1,
          "Resolved source itemdrop frame did not reach the caller-owned RenderQueue callback");
    TextureImage source_diffuse;
    check(load_texture(resolve_content_path(itemdrop_assets,
            "atlas_dh2_game_objects_001.tga", "data/3D/GameObjects/itemdrops.bdae"),
            source_diffuse, error), error);
    TextureImage source_alpha;
    check(load_texture(resolve_content_path(itemdrop_assets,
            "atlas_dh2_game_objects_001_alpha.tga", "data/3D/GameObjects/itemdrops.bdae"),
            source_alpha, error), error);
    check(source_diffuse.width > 0 && source_diffuse.height > 0 &&
          source_alpha.width == source_diffuse.width && source_alpha.height == source_diffuse.height,
          "Original itemdrop diffuse/alpha atlas dimensions differ");
    auto gold_expected = source_diffuse;
    for (std::size_t pixel = 0; pixel < std::size_t(gold_expected.width) * gold_expected.height; ++pixel) {
        const auto alpha = source_alpha.rgba[pixel * 4];
        const auto destination = pixel * 4 + 3;
        gold_expected.rgba[destination] = static_cast<std::uint8_t>(
            (unsigned(gold_expected.rgba[destination]) * alpha + 127) / 255);
    }
    const auto contains_pixels = [&](const TextureImage& expected) {
        return std::any_of(texture_leases.images.begin(), texture_leases.images.end(),
            [&](const auto& upload) { return upload.second.width == expected.width &&
                upload.second.height == expected.height && upload.second.rgba == expected.rgba; });
    };
    check(contains_pixels(source_diffuse) && contains_pixels(gold_expected),
          "Original diffuse/alpha image pixels were not retained by source texture leases");
    check(std::any_of(material_sources.begin(), material_sources.end(), [](const auto& source) {
              return source.first == "dummy_itemdrop_Potion";
          }) && std::any_of(material_sources.begin(), material_sources.end(), [](const auto& source) {
              return source.first == "root_itemdrop_Gold_01";
          }), "Original material binder was not called for both source itemdrop visuals");
    std::size_t potion_vertices = 0, gold_vertices = 0;
    bool potion_position = false, gold_position = false;
    bool potion_source_pass_ready = false;
    for (const auto& draw : rendered_frame->draws) {
        check(draw.mesh && draw.source_retention && !draw.mesh->vertices.empty() &&
              !draw.mesh->indices.empty() && draw.mesh->ranges.size() > 0 &&
              draw.world[12] == draw.source_position[0] &&
              draw.world[13] == draw.source_position[1] &&
              draw.world[14] == draw.source_position[2],
              "Render packet lost retained source geometry or exact stored position");
        if (draw.identity == potion->first) {
            potion_source_pass_ready = draw.source_pass_ready;
            potion_vertices += draw.mesh->vertices.size();
            potion_position = draw.source_record.item_id == potion_id &&
                draw.source_record.source_actor == source.id && draw.visual_uri == "dummy_itemdrop_Potion";
        } else if (draw.identity == gold->first) {
            check(draw.source_pass_ready,
                  "GoldStack01 source external effect did not resolve its selected BRES technique pass");
            gold_vertices += draw.mesh->vertices.size();
            gold_position = draw.source_record.item_id == gold_id &&
                draw.source_record.source_actor == source.id && draw.visual_uri == "root_itemdrop_Gold_01";
        } else check(false, "Renderer emitted an identity outside this same world-item store");
    }
    check(potion_vertices == 44 && gold_vertices == 50 && potion_position && gold_position,
          "Renderer did not use the source-verified Potion static / Gold skinned rest-pose geometry");

    auto character = std::make_shared<CharacterState>(make_default_character(
        "world-item-consumer", "World Item Consumer", "warrior"));
    const auto gold_before = character->gold;
    PlayerContext context{7, character};
    RuntimeWorldItemInteractionServicesV1 pickup_services{&context, resolve_player};
    ActorState player;
    player.id = 7;
    player.transform.position = {9000.0f, -4000.0f, 2.0f}; // No invented proximity gate.
    RuntimeWorldItemInteractionReceiptV1 receipt;
    check(consumer.interact_selected(7, true, &player, *potion_view,
                                     pickup_services, receipt, error), error);
    check(receipt.pickup.completed && store.size() == selected.size() - 1 &&
          character->inventory.size() == 1 &&
          character->inventory.front().definition_id == "Potion0",
          "Selected Potion0 did not atomically transfer from the same store once");
    const auto potion_commit = *character;
    check(!consumer.interact_selected(7, true, &player, *potion_view,
                                      pickup_services, receipt, error) &&
          same_inventory(*character, potion_commit),
          "Stale selected Potion0 view repeated or mutated pickup");

    check(consumer.interact_selected(7, true, &player, *gold_view,
                                     pickup_services, receipt, error), error);
    check(receipt.pickup.completed && character->gold == gold_before +
          static_cast<std::uint64_t>(*gold->second.resolved_gold_value) &&
          store.size() == selected.size() - 2,
          "Valued GoldStack01 did not credit gold and retire from the same store");

    std::cout << "{\"validation\":\"PASS\",\"table\":\"Barrel_Level_01\",\"seed\":" << seed
              << ",\"same_store_records\":" << selected.size()
              << ",\"potion\":{\"item_id\":925,\"audio_visual_id\":" << potion_view->source_audio_visual_id
              << ",\"model_resource_uri\":\"" << potion_view->source_model_resource_uri
              << "\",\"visual_uri\":\"" << potion_view->source_visual_uri << "\",\"exact_icon\":\""
              << potion_view->exact_icon_name << "\"},\"gold\":{\"item_id\":418,\"resolved_value\":"
              << *gold->second.resolved_gold_value << ",\"audio_visual_id\":" << gold_view->source_audio_visual_id
              << ",\"model_resource_uri\":\"" << gold_view->source_model_resource_uri
              << "\",\"visual_uri\":\"" << gold_view->source_visual_uri << "\",\"exact_icon\":\""
              << gold_view->exact_icon_name << "\"},\"source_rng_calls_after_gold_value\":" << random.calls
              << ",\"source_icon_claim\":false,\"pickup_radius_claim\":false"
              << ",\"itemdrop_render\":{\"draw_packets\":" << rendered_frame->draws.size()
              << ",\"source_material_bindings\":" << material_bindings
              << ",\"decoded_source_texture_leases\":" << original_materials.texture_count()
              << ",\"potion_vertices\":" << potion_vertices << ",\"gold_vertices\":" << gold_vertices
              << ",\"potion_source_pass_ready\":" << (potion_source_pass_ready ? "true" : "false")
              << ",\"gold_external_effect_resolved\":true"
              << ",\"source_texture_pixels_decoded\":true,\"gpu_upload_claim\":false"
              << ",\"render_queue_callback_claim\":true,\"gpu_draw_claim\":false"
              << ",\"source_material_declarations\":[";
    for (std::size_t i = 0; i < material_declarations.size(); ++i) {
        if (i) std::cout << ',';
        std::cout << '"' << material_declarations[i] << '"';
    }
    std::cout << ']'
              << ",\"gold_pose\":\"authored_rest\"}}\n";
    return 0;
} catch (const std::exception& exception) {
    std::cerr << "FAIL: " << exception.what() << '\n';
    return 1;
}
