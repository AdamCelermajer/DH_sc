#include "runtime_source_checkpoint_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_combat_properties.hpp"
#include "../../playable_actor_world.hpp"
#include "../../../game-data/data.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::campaign;

namespace {
int checks{};
void check(bool condition, const std::string& message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}
std::vector<char> read(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(stream), {}};
}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
static ActorState source_player(ActorId id, const OriginalCombatProperties& props) {
    ActorState actor;
    actor.id = id;
    actor.definition_id = "KnightPlayerBase";
    actor.faction_id = props.sheets.resolved[0];
    actor.health = original_signed256(props.sheets.resolved[36]);
    actor.max_health = original_signed256(props.sheets.resolved[38]);
    actor.resource = original_signed256(props.sheets.resolved[41]);
    actor.max_resource = original_signed256(props.sheets.resolved[43]);
    actor.persistent_character_id = "checkpoint-player";
    return actor;
}
void source_route_fixture(const std::filesystem::path& repository) {
    const auto module = read(repository / "port/windows-foundation/assets/original-cache/data/3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp");
    const std::string text(module.begin(), module.end());
    check(text.find("name=\"_prim_ExitLevelZone\"") != std::string::npos,
          "actual boardwalk exit source object exists");
    check(text.find("levelName=\"SWAMP_CAVE_WITCH_A\"") != std::string::npos,
          "actual boardwalk exit targets authored Witch cave member");
    check(text.find("entrypointID=\"0\"") != std::string::npos,
          "actual boardwalk exit carries entrypoint zero");
    check(text.find("fasttravel=\"a01_SWAMP_CAMP\"") != std::string::npos,
          "actual boardwalk exit retains authored camp fast-travel key");

    const auto barrier = read(repository / "port/windows-foundation/assets/original-cache/data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp");
    const std::string barrier_text(barrier.begin(), barrier.end());
    check(barrier_text.find("unlock_cond=\"IsAfter_Swamp_KillWitch2\"") != std::string::npos &&
          barrier_text.find("data=\"Swamp_Door_Ruins\"") != std::string::npos,
          "actual Witch-gated Swamp barrier identity remains authored data");
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 4) throw std::runtime_error("Supply repository root, source asset root and isolated save path");
        const std::filesystem::path repository(argv[1]);
        source_route_fixture(repository);

        AssetCatalog assets(argv[2]);
        const std::string root = "original-cache/data/pydata/";
        std::string error;
        OriginalPropertyDatabase properties;
        dh2::data::AiTables ai;
        check(load_original_property_tables(assets, root, properties, error), error);
        check(load_original_ai_tables(assets, root, ai, error), error);
        const auto item_bytes = assets.read(root + "loot_table_pyarray.bin");
        const auto item_names = assets.read(root + "loot_table_pyarraynames.bin");
        const auto item_fields = assets.read(root + "loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(item_bytes), bytes(item_names), bytes(item_fields), items, error), error);
        const auto sword = items.rows.at(664).record;
        OriginalCombatFacts facts;
        check(original_combat_equipment_facts(&sword, nullptr, facts, error), error);
        OriginalCombatProperties player_properties;
        check(build_original_combat_properties(properties, "KnightPlayerBase", {256, true},
              {{sword, false, {}}}, facts, player_properties, error), error);

        dh2::data::CombatRandom random{711, 9};
        PlayableActorWorld world(ai, random);
        auto player = source_player(1, player_properties);
        player.transform.position = {1411.88f, 915.238f, 143.497f};
        player.attack_ids = {"source-main"};
        player.equipment.push_back({"main", items.identifiers.at(664), "sword-instance"});
        check(world.bind_actor(player, player_properties, {true, true, sword}, error), error);
        check(world.bind_source("source-main", {}, error), error);

        auto character = make_default_character("checkpoint-player", "Cris", "KnightPlayerBase");
        character.inventory.push_back({"sword-instance", items.identifiers.at(664), 1});
        character.equipment.push_back({"main", "sword-instance", 0, 1});
        GameSave captured;
        constexpr const char* destination = "SWAMP_CAVE_WITCH_A";
        check(capture_game_save(destination, 1, character, world, captured, error), error);
        captured.random.seed = 431;
        captured.random.calls = 12;

        const SourceTransitionV1 route{
            "deadend_brdwalk_w_00.mgp", "_prim_ExitLevelZone",
            "SWAMP_CAVE_WITCH_A", destination, 0};
        RuntimeSourceCheckpointReceiptV1 written;
        const std::filesystem::path save_path(argv[3]);
        check(save_runtime_source_checkpoint_v1(save_path, route, captured, written, error), error);
        check(written.persisted && !written.source_entrypoint_persisted &&
              written.transition_object == "_prim_ExitLevelZone" &&
              written.destination_member == "SWAMP_CAVE_WITCH_A" && written.entrypoint_id == 0,
              "source transition receipt names the actual authored destination");

        GameSave loaded;
        RuntimeSourceCheckpointReceiptV1 restored;
        check(load_runtime_source_checkpoint_v1(save_path, route, loaded, restored, error), error);
        check(loaded.level_uri == captured.level_uri && loaded.character.id == captured.character.id &&
              loaded.character.inventory.size() == 1 && loaded.character.inventory[0].instance_id == "sword-instance" &&
              loaded.actors.size() == 1 && loaded.actors[0].actor.transform.position[0] == 1411.88f &&
              loaded.actors[0].actor.transform.position[1] == 915.238f &&
              loaded.random.seed == 431 && loaded.random.calls == 12,
              "same destination, CharacterState, actor transform and RNG reload from existing GameSave");
        check(restored.persisted && !restored.source_entrypoint_persisted &&
              restored.level_uri == destination &&
              restored.character_id == captured.character.id,
              "reload receipt remains tied to same source route and character");

        GameSave sentinel; sentinel.level_uri = "untouched";
        RuntimeSourceCheckpointReceiptV1 rejected_receipt;
        rejected_receipt.transition_object = "sentinel";
        auto wrong_destination = route; wrong_destination.destination_level_uri = "SWAMP_02";
        check(!load_runtime_source_checkpoint_v1(save_path, wrong_destination, sentinel, rejected_receipt, error) &&
              sentinel.level_uri == "untouched" && rejected_receipt.transition_object == "sentinel",
              "wrong transition destination rejects reload without publishing outputs");

        std::cout << "PASS source transition checkpoint adapter: " << checks
                  << " assertions; existing GameSave only; Door/LNAM/LEPT/LUSP not represented\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
