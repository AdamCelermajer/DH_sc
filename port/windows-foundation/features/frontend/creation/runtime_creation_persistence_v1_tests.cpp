#include "runtime_creation_persistence_v1.hpp"

#include "../../../asset_catalog.hpp"
#include "../../../../game-data/loot_creation_v8.hpp"
#include "../../../../script-runtime/script_constants.hpp"

#include <cassert>
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::frontend::creation;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void run_source_skill_row_projection_tests(dh2::data::SkillTables::Borrow skills) {
    std::string error;
    check(bool(skills), "source skill-row projector test needs the original SkillTables borrow");
    for (const auto& choice : class_choices()) {
        CharacterState state;
        state.class_id = std::string(choice.profile_token);
        check(initialize_source_skill_rows_v1(skills,
                  static_cast<std::int32_t>(choice.skill_list_row), state, error),
              "source skill-row projector failed for " + std::string(choice.profile_token) + ": " + error);
        const auto& list = skills.lists().at(choice.skill_list_row);
        check(state.source_skill_slots_known && state.skills.size() == list.size(),
              "projector did not publish the complete known source list");
        for (std::size_t row = 0; row < list.size(); ++row) {
            const auto dictionary_id = list[row];
            check(dictionary_id >= 0 && std::size_t(dictionary_id) < skills.skill_names().size() &&
                      state.skills[row].id == skills.skill_names()[std::size_t(dictionary_id)] &&
                      state.skills[row].rank == 0,
                  "projector changed original list order/name or fabricated a starting rank");
        }
        check(state.skill_slots.size() == 2 && state.skill_slots[0].equipment_set == 0 &&
                  state.skill_slots[0].slot == 0 && state.skill_slots[0].saved_skill_row == 0 &&
                  state.skill_slots[1].equipment_set == 1 && state.skill_slots[1].slot == 0 &&
                  state.skill_slots[1].saved_skill_row == 0,
              "projector did not reproduce the two source initial slot-zero maps");
    }

    CharacterState invalid;
    invalid.class_id = "preserve";
    check(!initialize_source_skill_rows_v1(skills,
              static_cast<std::int32_t>(skills.lists().size()), invalid, error) &&
              invalid.class_id == "preserve" && invalid.skills.empty() && invalid.skill_slots.empty() &&
              !invalid.source_skill_slots_known,
          "out-of-range source list rejection partially mutated CharacterState");
    check(!initialize_source_skill_rows_v1({}, 0, invalid, error) && invalid.skills.empty() &&
              invalid.skill_slots.empty() && !invalid.source_skill_slots_known,
          "missing SkillTables rejection partially mutated CharacterState");

    CharacterState learned;
    learned.class_id = "preserve-learned";
    learned.skills.push_back({"saved-rank", 3});
    learned.skill_slots.push_back({0, 0, 0});
    check(!initialize_source_skill_rows_v1(skills, 0, learned, error) &&
              learned.class_id == "preserve-learned" && learned.skills.size() == 1 &&
              learned.skills.front().id == "saved-rank" && learned.skills.front().rank == 3 &&
              learned.skill_slots.size() == 1 && !learned.source_skill_slots_known,
          "projector reseeded an existing saved rank/slot map");
    CharacterState known;
    known.class_id = "preserve-known";
    known.source_skill_slots_known = true;
    check(!initialize_source_skill_rows_v1(skills, 0, known, error) && known.skills.empty() &&
              known.skill_slots.empty() && known.source_skill_slots_known,
          "projector reseeded a state already marked as source-known");
}

void run_class(const AssetCatalog& assets,
               const std::shared_ptr<const OriginalPropertyDatabase>& properties,
               dh2::data::LootTablesV2& loot_owner,
               dh2::data::SkillTables& skill_owner,
               const ClassChoice& choice,
               std::uint32_t seed,
               const std::array<std::uint32_t, 3>& skill_caps,
               const std::filesystem::path& root,
               bool grant_context = true,
               const char* save_suffix = "") {
    using namespace dh::foundation::frontend::creation;
    dh2::data::LootRandom8V2 random{seed, 0};
    auto state = std::make_shared<CharacterState>();
    const auto* original_pointer = state.get();
    auto expected_owner = state;
    bool start_called = false;
    std::shared_ptr<CharacterState> start_argument;
    RuntimeCreationRequestV1 request;
    request.shared_state = state;
    request.character_id = std::string("generic-") + std::string(choice.profile_token);
    request.player_name = "Source Starter";
    request.class_token = std::string(choice.profile_token);
    request.save_path = root / (std::string(choice.profile_token) + save_suffix + ".savestate");
    request.source_timer = 0x12345678;
    request.saved_date = 0x23456789;
    RuntimeCreationServicesV1 services;
    services.source.properties = properties;
    services.source.loot = loot_owner.borrow();
    services.source.skills = skill_owner.borrow();
    services.source.source_loot_random = &random;
    services.source.source_skill_grant_context_known = grant_context;
    services.source.source_skill_level_caps = skill_caps;
    services.start_same_state = [&](const std::shared_ptr<CharacterState>& started,
                                    std::string&) {
        start_called = true;
        start_argument = started;
        check(started.get() == original_pointer, "start received different CharacterState pointer");
        check(!started.owner_before(expected_owner) && !expected_owner.owner_before(started),
              "start received different shared ownership control block");
        return true;
    };

    auto result = RuntimeCreationPersistenceV1::create_reload_start(request, services);
    check(result.status == RuntimeCreationStatusV1::start_provider_returned_success,
          "source-backed class creation failed: " + result.error);
    check(result.saved && result.reloaded && result.published_to_shared_state && result.start_provider_succeeded,
          "persistence/start phases not recorded");
    check(result.same_state_owner(expected_owner), "result lost exact caller owner");
    {   // P14 schema: FS_StartGame stamps a fresh profile (date = request.saved_date, LevelList row 41, act 1, Normal).
        const auto& meta = result.shared_state->menu_metadata;
        check(meta.known && meta.save_time == request.saved_date && result.shared_state->current_difficulty == 0 &&
              result.shared_state->unlocked_difficulty == 0, "new profile did not persist known menu metadata");
        for (std::size_t d = 0; d < 3; ++d)
            check(meta.level_row[d] == 41 && meta.current_act[d] == 1, "new profile LevelList row/act is not 41/1");
    }
    check(start_called && start_argument.get() == original_pointer, "same-state start was not called");
    check(random.calls == result.shared_state->inventory.size(), "source RNG call count differs from authored ItemLists");
    check(result.source_profile_metadata.character_row == static_cast<std::int32_t>(choice.character_row),
          "original fresh metadata row differs");
    check(result.source_profile_metadata.name == request.player_name &&
          result.source_profile_metadata.character == request.class_token &&
          !result.source_profile_metadata.bytes.empty(), "original fresh profile metadata absent");

    OriginalActorProperties actor;
    std::string error;
    check(resolve_original_fresh_player(*properties, std::string(choice.profile_token), actor, error), error);
    const auto& actual = *result.shared_state;
    check(actual.id == request.character_id && actual.name == request.player_name &&
          actual.class_id == request.class_token && actual.stats.level == 1,
          "source starter identity/class/level differs");
    check(actual.stats.health == actor.health && actual.stats.max_health == actor.max_health &&
          actual.stats.resource == actor.resource && actual.stats.max_resource == actor.max_resource,
          "source vital properties differ");
    check(actual.stats.strength == original_signed256(actor.sheets.resolved[149]) &&
          actual.stats.dexterity == original_signed256(actor.sheets.resolved[150]) &&
          actual.stats.endurance == original_signed256(actor.sheets.resolved[151]) &&
          actual.stats.energy == original_signed256(actor.sheets.resolved[152]) &&
          actual.stats.intelligence == 0.0f, "source Str/Dex mapping or unmapped Intelligence differs");
    check(actual.source_endurance_energy_known && actual.source_points_known &&
          actual.source_skill_slots_known && actual.source_faery_state_known,
          "source semantic known flags were not set");
    check(actual.source_stat_points == static_cast<std::uint32_t>(actor.sheets.resolved[148] >> 8),
          "source stat point value differs");
    check(actual.skills.size() == services.source.skills.lists().at(choice.skill_list_row).size(),
          "not every source SkillList row was preserved");
    for (std::size_t i = 0; i < actual.skills.size(); ++i) {
        const auto dictionary_id = services.source.skills.lists().at(choice.skill_list_row)[i];
        check(dictionary_id >= 0 && std::size_t(dictionary_id) < services.source.skills.skill_names().size() &&
              actual.skills[i].id == services.source.skills.skill_names()[std::size_t(dictionary_id)] &&
              (i != 0 || actual.skills[i].rank <= 1) && (i == 0 || actual.skills[i].rank == 0),
              "source SkillList row/order/rank differs");
    }
    const auto& first_skill = services.source.skills.skills().at(static_cast<std::size_t>(
        services.source.skills.lists().at(choice.skill_list_row).front()));
    const auto remaining_points = std::max(0, actor.sheets.resolved[157] >> 8);
    const auto source_level = actor.sheets.resolved[19] >> 8;
    const bool source_incremented = grant_context && remaining_points > 0 && source_level >= first_skill.scalar.words[8] &&
        source_level - first_skill.scalar.words[8] >= 0 && skill_caps[0] > 0;
    check(actual.skills.front().rank == (source_incremented ? 1u : 0u) &&
          actual.source_skill_points == static_cast<std::uint32_t>(remaining_points - (source_incremented ? 1 : 0)),
          "source free-skill increment or point deduction differs");
    check(actual.skill_slots.size() == 2 && actual.skill_slots[0].equipment_set == 0 &&
          actual.skill_slots[0].slot == 0 && actual.skill_slots[0].saved_skill_row == 0 &&
          actual.skill_slots[1].equipment_set == 1 && actual.skill_slots[1].slot == 0 &&
          actual.skill_slots[1].saved_skill_row == 0,
          "source initial row-zero skill-slot maps differ");
    check(actual.source_faery_list_id == static_cast<std::int32_t>(choice.faery_list_row),
          "source class FaeryList ID differs");
    for (const auto& difficulty : actual.faery_by_difficulty) {
        check(difficulty.current_faery == 0, "source initial current faery differs");
        for (const auto& faery : difficulty.faeries)
            check(faery.state == 0 && faery.level == 0, "source initial faery progress differs");
    }
    const auto& loot = services.source.loot.loots().at(choice.starting_loot_row);
    check(actual.inventory.size() == loot.fixed_entries.size(), "source loot fixed entry count differs");
    const auto& items = services.source.loot.items();
    for (std::size_t i = 0; i < loot.fixed_entries.size(); ++i) {
        const auto list_id = loot.fixed_entries[i].words[0];
        const auto& entry = services.source.loot.item_lists().at(std::size_t(list_id)).front();
        std::int32_t selected = entry.item;
        check(dh2::data::select_loot_item_variant_v8(items, entry.item, 0, false, selected, error), error);
        std::int8_t quantity{};
        std::memcpy(&quantity, &entry.quantity, sizeof(quantity));
        if (quantity == -2) quantity = 99;
        check(actual.inventory[i].definition_id == items.identifiers.at(std::size_t(selected)) &&
              actual.inventory[i].quantity == static_cast<std::uint32_t>(quantity),
              "source ItemTable grant ID/quantity differs");
    }
    for (const auto& equipped : actual.equipment) {
        check(equipped.equipment_set == 0 && equipped.source_slot >= 0 && equipped.source_slot < 9,
              "source auto-equip set/slot identity is incomplete");
        check(std::any_of(actual.inventory.begin(), actual.inventory.end(), [&](const InventoryItem& item) {
            return item.instance_id == equipped.item_instance_id;
        }), "source auto-equip references an absent item instance");
    }
    CharacterState disk;
    check(load_character(request.save_path, disk, error), error);
    check(disk.inventory.size() == actual.inventory.size() && disk.skills.size() == actual.skills.size() &&
          disk.class_id == actual.class_id, "persisted/reloaded starter differs from published state");
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "pass the extracted original asset root");
        AssetCatalog assets(argv[1]);
        auto properties = std::make_shared<OriginalPropertyDatabase>();
        std::string error;
        check(load_original_property_tables(assets, "original-cache/data/pydata", *properties, error), error);

        const auto table_root = std::filesystem::path("original-cache/data/pydata");
        auto loot_records = assets.read(table_root / "loot_table_pyarray.bin");
        auto loot_names = assets.read(table_root / "loot_table_pyarraynames.bin");
        auto loot_schema = assets.read(table_root / "loot_table_pystructnames.bin");
        dh2::data::LootTablesV2 loot;
        check(loot.load({loot_records.data(), loot_records.size()},
                        {loot_names.data(), loot_names.size()},
                        {loot_schema.data(), loot_schema.size()}, error), error);

        auto skill_records = assets.read(table_root / "skills_pyarray.bin");
        auto skill_names = assets.read(table_root / "skills_pyarraynames.bin");
        auto skill_schema = assets.read(table_root / "skills_pystructnames.bin");
        dh2::data::SkillTables skills;
        check(skills.load({skill_records.data(), skill_records.size()},
                          {skill_names.data(), skill_names.size()},
                          {skill_schema.data(), skill_schema.size()}, error), error);
        run_source_skill_row_projection_tests(skills.borrow());

        auto constants_bytes = assets.read(table_root / "design_pycst.bin");
        auto* constants = dh2_script_constants_create();
        check(constants != nullptr, "could not allocate original script constants owner");
        dh2_script_constants_reload constants_receipt{};
        check(dh2_script_constants_load(constants, constants_bytes.data(),
              static_cast<std::uint32_t>(constants_bytes.size()), &constants_receipt) == 0 &&
              constants_receipt.consumed == constants_bytes.size(),
              "original CharacterDesign constants did not load completely");
        std::array<std::uint32_t, 3> caps{};
        const char* cap_names[] = {"MaxSkillLevelBNormal", "MaxSkillLevelCHard", "MaxSkillLevelDVeryHard"};
        for (std::size_t i = 0; i < caps.size(); ++i) {
            std::int32_t cap{};
            check(dh2_script_constants_get(constants, "CharacterDesign", cap_names[i], &cap) == 0 && cap > 0,
                  "actual CharacterDesign skill cap is missing");
            caps[i] = static_cast<std::uint32_t>(cap);
        }

        const auto scratch = std::filesystem::temp_directory_path() / "dh_runtime_creation_persistence_v1";
        std::error_code stale_error;
        std::filesystem::remove_all(scratch, stale_error);
        check(!stale_error, "could not clear this test's prior scratch directory");
        std::filesystem::create_directories(scratch);
        for (const auto& choice : class_choices())
            run_class(assets, properties, loot, skills, choice, 0x13579bdfu + choice.character_row, caps, scratch);
        run_class(assets, properties, loot, skills, class_choices().front(), 0x2468ace0u,
                  caps, scratch, false, "-without-caps");

        auto untouched = std::make_shared<CharacterState>();
        untouched->id = "unchanged";
        RuntimeCreationRequestV1 unavailable_request;
        unavailable_request.shared_state = untouched;
        unavailable_request.character_id = "missing-source";
        unavailable_request.player_name = "Missing Source";
        unavailable_request.class_token = "KnightPlayerBase";
        unavailable_request.save_path = scratch / "missing-source.savestate";
        RuntimeCreationServicesV1 missing_source;
        missing_source.start_same_state = [](const auto&, std::string&) { return true; };
        const auto missing = RuntimeCreationPersistenceV1::create_reload_start(unavailable_request, missing_source);
        check(missing.status == RuntimeCreationStatusV1::unavailable_source &&
              untouched->id == "unchanged" && !missing.saved && !missing.published_to_shared_state &&
              !std::filesystem::exists(unavailable_request.save_path),
              "missing actual source tables mutated the caller state or created a save");

        const auto protected_path = scratch / "existing-profile.savestate";
        const std::string protected_bytes = "existing-user-profile-keep-byte-for-byte";
        { std::ofstream file(protected_path, std::ios::binary); file.write(protected_bytes.data(), protected_bytes.size()); }
        auto protected_state = std::make_shared<CharacterState>();
        protected_state->id = "preserve-existing";
        RuntimeCreationRequestV1 existing_request;
        existing_request.shared_state = protected_state;
        existing_request.character_id = "must-not-overwrite";
        existing_request.player_name = "Existing";
        existing_request.class_token = "KnightPlayerBase";
        existing_request.save_path = protected_path;
        RuntimeCreationServicesV1 existing_services;
        existing_services.start_same_state = [](const auto&, std::string&) { return true; };
        const auto protected_result = RuntimeCreationPersistenceV1::create_reload_start(existing_request, existing_services);
        std::ifstream preserved_file(protected_path, std::ios::binary);
        const std::string preserved_bytes((std::istreambuf_iterator<char>(preserved_file)), {});
        check(protected_result.status == RuntimeCreationStatusV1::destination_exists &&
              protected_state->id == "preserve-existing" && preserved_bytes == protected_bytes &&
              !protected_result.saved && !protected_result.published_to_shared_state,
              "creation overwrote an existing profile or mutated its caller state");

        RuntimeCreationRequestV1 unsupported;
        unsupported.shared_state = untouched;
        unsupported.character_id = "never-written";
        unsupported.player_name = "Unsupported";
        unsupported.class_token = "KnightPlayerBase";
        unsupported.save_path = scratch / "unsupported.savestate";
        RuntimeCreationServicesV1 unused;
        const auto failed = RuntimeCreationPersistenceV1::create_reload_start(unsupported, unused);
        check(failed.status == RuntimeCreationStatusV1::unavailable_start &&
              failed.same_state_owner(untouched) && untouched->id == "unchanged" &&
              !failed.saved && !failed.published_to_shared_state,
              "missing start service did not fail before mutation");
        check(RuntimeCreationPersistenceV1::representation_limits().size() >= 4,
              "representation gaps are not surfaced");
        dh2_script_constants_destroy(constants);
        std::error_code ignored;
        std::filesystem::remove_all(scratch, ignored);
        std::cout << "runtime_creation_persistence_v1: 3 source-backed class profiles, v2 skills/faery/equipment, same-owner save/reload/start passed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "runtime_creation_persistence_v1: " << exception.what() << '\n';
        return 1;
    }
}
