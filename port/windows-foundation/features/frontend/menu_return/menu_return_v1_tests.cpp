#include "menu_return_v1.hpp"

#include "../../../asset_catalog.hpp"
#include "../../../original_combat_visual_plan.hpp"
#include "../../../original_melee_bindings.hpp"
#include "../../../original_actor_properties.hpp"
#include "../../../../game-data/data.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::frontend::menu_return;

namespace {
void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), {}};
}

dh2::data::Bytes bytes(const std::vector<std::uint8_t>& input) {
    return {input.data(), input.size()};
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 3, "Supply original asset root and isolated output directory");
        AssetCatalog assets(argv[1]);
        const std::filesystem::path output_root(argv[2]);
        std::filesystem::create_directories(output_root);
        const auto live_path = output_root / "menu-return-live.save";
        const auto profile_path = output_root / "menu-return-profile.save";
        std::string error;

        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 0x4d52;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        const auto root = config.tableRoot + "/";
        const auto item_data = assets.read(root + "loot_table_pyarray.bin");
        const auto item_names = assets.read(root + "loot_table_pyarraynames.bin");
        const auto item_fields = assets.read(root + "loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(item_data), bytes(item_names), bytes(item_fields), items, error), error);
        check(items.identifiers.size() > 664, "Source player starter item row is absent");
        config.mainItemId = items.identifiers[664];
        config.equippedItemIds = {config.mainItemId};

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan visual_plan;
        check(build_original_combat_visual_plan(assets, melee, config.playerProfileId,
              customization, "menu-return-player", visual_plan, error), error);
        config.playerVisualConfig = visual_plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile player_policy;
        player_policy.action = {"AttackStatic", 0, {0, 1}};
        player_policy.initialIdle = {"Idle", 0, {0}};
        player_policy.damageMarkerNames = {"attack_mainhand"};
        player_policy.propertyOptions = {256, true};
        config.profiles.emplace(config.playerProfileId, player_policy);

        ActorPopulation population;
        CharacterVisual player_visual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, player_visual,
              population, {0, 0, 0}, customization, error), error);
        auto profile = make_default_character("menu-return-profile", "Return Test", "KnightPlayerBase");
        auto* live_player = session.actor(session.player_id());
        check(live_player && live_player->alive(), "Actual Session player did not initialize alive");
        live_player->persistent_character_id = profile.id;

        GameSave original_live;
        check(capture_game_save("original-cache/data/scene/001_swamp.mlx", session.player_id(),
              profile, *session.world(), original_live, error), error);
        check(save_game(live_path, original_live, error), error);
        check(save_character(profile_path, profile, error), error);
        const auto live_before = read_file(live_path);
        const auto profile_before = read_file(profile_path);

        generic_skills::RuntimeSkillCastCoordinatorV1 casts;
        TicketV1 ticket;
        check(!request_v1({&session, &casts, false}, ticket, error) && !ticket.valid,
              "Unconfirmed Main Menu request was admitted");
        check(read_file(live_path) == live_before && read_file(profile_path) == profile_before,
              "Unconfirmed request changed an existing save");

        // A real coordinator timer bound to an older update serial must reject
        // admission before either existing save is touched. An active source
        // cast is rejected by this same checkpoint_v1 call (covered by the
        // source coordinator's integration regression at its prepared-pending-use branch).
        InputActions no_input;
        check(session.update(1.0 / 60.0, no_input, {0, 0, 0}, 0.0f, error), error);
        check(casts.advance_after_session_update(session, 1.0 / 60.0, error), error);
        check(session.update(1.0 / 60.0, no_input, {0, 0, 0}, 0.0f, error), error);
        check(!request_v1({&session, &casts, true}, ticket, error) && !ticket.valid &&
              error.find("current Session update serial") != std::string::npos,
              "Stale source skill-clock owner was admitted for Main Menu return");
        check(read_file(live_path) == live_before && read_file(profile_path) == profile_before,
              "Rejected checkpoint changed an existing save file");

        // A fresh coordinator represents the current Session source timers as
        // quiet. The successful path calls the actual two save providers.
        generic_skills::RuntimeSkillCastCoordinatorV1 current_casts;
        check(request_v1({&session, &current_casts, true}, ticket, error), error);
        live_player->health -= 1.0f;
        live_player->resource -= 1.0f;
        CommitV1 committed;
        check(commit_v1(ticket, current_casts, "original-cache/data/scene/001_swamp.mlx",
              live_path, profile_path, profile, committed, error), error);
        check(committed.level_saved && committed.profile_saved && committed.may_return_to_frontend &&
              !ticket.valid && profile.stats.health == live_player->health &&
              profile.stats.resource == live_player->resource,
              "Ordinary source menu return did not save current same-Session player facts");
        GameSave loaded_live;
        CharacterState loaded_profile;
        check(load_game(live_path, loaded_live, error), error);
        check(load_character(profile_path, loaded_profile, error), error);
        check(loaded_live.character.stats.health == live_player->health &&
              loaded_live.character.stats.resource == live_player->resource &&
              loaded_profile.stats.health == live_player->health &&
              loaded_profile.stats.resource == live_player->resource,
              "Successful return saves did not round-trip live Session health/resource");

        // Death return follows the current main caller's live-save admission:
        // do not overwrite the last valid world checkpoint with a dead roster,
        // but do persist the actual zero-HP player profile instead of stale UI
        // stats. The level save remains byte-identical.
        const auto live_before_death_return = read_file(live_path);
        apply_actor_damage(*live_player, live_player->health);
        check(!live_player->alive() && live_player->health == 0.0f,
              "Death return fixture did not reach the actual Session dead state");
        generic_skills::RuntimeSkillCastCoordinatorV1 death_casts;
        TicketV1 death_ticket;
        check(request_v1({&session, &death_casts, true}, death_ticket, error), error);
        CommitV1 death_commit;
        check(commit_v1(death_ticket, death_casts, "original-cache/data/scene/001_swamp.mlx",
              live_path, profile_path, profile, death_commit, error), error);
        check(!death_commit.level_saved && death_commit.profile_saved &&
              death_commit.may_return_to_frontend && profile.stats.health == 0.0f &&
              read_file(live_path) == live_before_death_return,
              "Death return overwrote the world checkpoint or retained stale profile health");
        check(load_character(profile_path, loaded_profile, error) &&
              loaded_profile.stats.health == 0.0f && loaded_profile.stats.max_health > 0.0f,
              "Death return did not persist a valid current zero-HP player profile");

        // Existing requests are single-use and cannot be replayed.
        CommitV1 duplicate;
        check(!commit_v1(ticket, current_casts, "original-cache/data/scene/001_swamp.mlx",
              live_path, profile_path, profile, duplicate, error) && !duplicate.may_return_to_frontend,
              "Duplicate Main Menu commit was accepted");
        std::cout << "menu_return_v1 PASS: confirmed source route uses real Session/cast checkpoint and GameSave/Profile writers; rejected request left existing saves byte-identical; ordinary return round-tripped current Session vitals\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
