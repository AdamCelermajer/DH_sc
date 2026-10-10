#include "runtime_creation_flow_adapter_v1.hpp"

#include "../../../asset_catalog.hpp"
#include "../../../original_actor_properties.hpp"
#include "../../../save_store.hpp"
#include "../../../../game-data/loot_tables_v2.hpp"
#include "../../../../game-data/skill_tables.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::frontend;
using namespace dh::foundation::frontend::creation;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
bool same(const std::shared_ptr<CharacterState>& a, const std::shared_ptr<CharacterState>& b) {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}

struct OriginalInputs {
    std::shared_ptr<OriginalPropertyDatabase> properties = std::make_shared<OriginalPropertyDatabase>();
    dh2::data::LootTablesV2 loot;
    dh2::data::SkillTables skills;
    dh2::data::LootRandom8V2 random{0x55443322u, 0};

    void load(const AssetCatalog& assets) {
        std::string error;
        check(load_original_property_tables(assets, "original-cache/data/pydata", *properties, error), error);
        const std::filesystem::path root("original-cache/data/pydata");
        auto records = assets.read(root / "loot_table_pyarray.bin");
        auto names = assets.read(root / "loot_table_pyarraynames.bin");
        auto schema = assets.read(root / "loot_table_pystructnames.bin");
        check(loot.load({records.data(), records.size()}, {names.data(), names.size()},
                        {schema.data(), schema.size()}, error), error);
        records = assets.read(root / "skills_pyarray.bin");
        names = assets.read(root / "skills_pyarraynames.bin");
        schema = assets.read(root / "skills_pystructnames.bin");
        check(skills.load({records.data(), records.size()}, {names.data(), names.size()},
                          {schema.data(), schema.size()}, error), error);
    }
    RuntimeCreationSourceV1 source() {
        RuntimeCreationSourceV1 out;
        out.properties = properties;
        out.loot = loot.borrow();
        out.skills = skills.borrow();
        out.source_loot_random = &random;
        return out;
    }
};

RuntimeCreationFlowServicesV1 make_flow_services(
    const RuntimeCreationSourceV1& source,
    const std::shared_ptr<CharacterState>& state,
    const std::filesystem::path& root,
    unsigned& creates, unsigned& assignments, unsigned& starts) {
    RuntimeCreationFlowServicesV1 services;
    services.shared_state = state;
    services.source = source;
    services.select_new_profile = [&](const std::string& name, const std::string& cls, int& slot,
                                      RuntimeCreationRequestV1& request, std::string& error) {
        ++creates;
        slot = 4;
        request.shared_state = state;
        request.character_id = "frontend-new-slot-4";
        request.player_name = name;
        request.class_token = cls;
        request.save_path = root / "new-slot-4.savestate";
        request.source_timer = 0x34561234;
        request.saved_date = 0x45672345;
        error.clear();
        return true;
    };
    services.selected_save_path = [root](int slot, std::filesystem::path& path, std::string& error) {
        if (slot == 7) path = root / "existing-slot-7.savestate";
        else if (slot == 9) path = root / "missing-slot-9.savestate";
        else { error = "Unexpected selected slot"; return false; }
        error.clear();
        return true;
    };
    services.assign_selected_slot = [&](int slot, int player, std::string& error) {
        if (player != 0 || (slot != 4 && slot != 7 && slot != 9)) {
            error = "Assignment was not for the selected local player slot";
            return false;
        }
        ++assignments;
        error.clear();
        return true;
    };
    services.start_same_state = [&, state](const std::shared_ptr<CharacterState>& started,
                                           int difficulty, std::string& error) {
        if (!same(started, state)) { error = "Start state owner changed"; return false; }
        if (difficulty != 0) { error = "Unexpected selected difficulty"; return false; }
        ++starts;
        error.clear();
        return true;
    };
    return services;
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "pass the original shared asset root");
        AssetCatalog assets(argv[1]);
        OriginalInputs original;
        original.load(assets);
        const auto scratch = std::filesystem::temp_directory_path() / "dh_runtime_creation_flow_adapter_v1";
        std::error_code ec;
        std::filesystem::remove_all(scratch, ec);
        check(!ec, "could not clear this focused test's scratch directory");
        std::filesystem::create_directories(scratch);

        auto state = std::make_shared<CharacterState>();
        state->id = "caller-owned-state";
        const auto* pointer = state.get();
        auto owner_witness = state;
        unsigned creates = 0, assignments = 0, starts = 0;
        auto services = make_flow_services(original.source(), state, scratch, creates, assignments, starts);
        auto adapter = std::make_shared<RuntimeCreationFlowAdapterV1>(std::move(services));
        flow::Navigator navigator(adapter->bind_navigation());
        std::string error;
        check(navigator.single_player({0, false}, error), error);
        check(navigator.accept_name("QA", error), error);
        check(navigator.select_class(1, error), error);
        check(navigator.confirm_class(error), error);
        check(navigator.top() == "menu_StartGame" && navigator.creation_stage() == flow::CreationStage::complete &&
              !navigator.start_delivered() && starts == 0 && creates == 1 && assignments == 1,
              "Confirm did not stop at StartGame after the actual create/assignment prefix");
        check(same(state, owner_witness) && state.get() == pointer && state->class_id == "RoguePlayerBase" &&
              std::filesystem::exists(scratch / "new-slot-4.savestate"),
              "Confirm did not persist/publish into the caller's exact state");
        check(!adapter->start_receipt(), "Confirm alone produced a generic gameplay start receipt");
        check(navigator.start_game(0, error), error);
        const auto new_receipt = adapter->start_receipt();
        check(navigator.start_delivered() && starts == 1 && assignments == 2 && new_receipt &&
              new_receipt->valid_for(owner_witness, 4) && new_receipt->created_in_this_flow &&
              new_receipt->save_path == scratch / "new-slot-4.savestate",
              "StartGame did not return the exact new profile state/save identity");

        // Existing-profile Start loads the one explicitly selected file into
        // the caller state before assignment/start; it never invokes create.
        auto legacy_state = make_default_character("existing-source-id", "Loaded Player", "mage");
        legacy_state.source_endurance_energy_known = true;
        legacy_state.stats.endurance = 11.0f;
        const auto existing_path = scratch / "existing-slot-7.savestate";
        check(save_character(existing_path, legacy_state, error), error);
        auto existing_shared = std::make_shared<CharacterState>();
        existing_shared->id = "old-memory-value";
        const auto* existing_pointer = existing_shared.get();
        auto existing_owner = existing_shared;
        unsigned existing_creates = 0, existing_assignments = 0, existing_starts = 0;
        auto existing_services = make_flow_services(original.source(), existing_shared, scratch,
                                                    existing_creates, existing_assignments, existing_starts);
        auto existing_adapter = std::make_shared<RuntimeCreationFlowAdapterV1>(std::move(existing_services));
        flow::Navigator existing_navigator(existing_adapter->bind_navigation());
        check(existing_navigator.single_player({7, true, existing_path}, error), error);
        check(existing_navigator.start_game(0, error), error);
        const auto existing_receipt = existing_adapter->start_receipt();
        check(existing_creates == 0 && existing_assignments == 1 && existing_starts == 1 &&
              existing_receipt && !existing_receipt->created_in_this_flow &&
              existing_receipt->valid_for(existing_owner, 7) &&
              existing_receipt->save_path == existing_path && existing_shared.get() == existing_pointer &&
              same(existing_shared, existing_owner) && existing_shared->id == legacy_state.id &&
              existing_shared->stats.endurance == legacy_state.stats.endurance,
              "Existing-profile Load did not preserve selected-file and same-state identity");
        check(existing_navigator.back(error)&&existing_navigator.single_player({7,true,existing_path},error),error);
        check(!existing_adapter->start_receipt(),"selecting a profile for another run retained an earlier start receipt");
        check(existing_navigator.start_game(0,error)&&existing_adapter->start_receipt()&&existing_starts==2,
              "restarting the same selected existing profile did not reload and deliver a fresh start");

        auto mismatch_shared=std::make_shared<CharacterState>();mismatch_shared->id="unchanged-on-path-mismatch";
        unsigned mismatch_creates=0,mismatch_assignments=0,mismatch_starts=0;
        auto mismatch_services=make_flow_services(original.source(),mismatch_shared,scratch,mismatch_creates,mismatch_assignments,mismatch_starts);
        auto mismatch_adapter=std::make_shared<RuntimeCreationFlowAdapterV1>(std::move(mismatch_services));
        flow::Navigator mismatch_navigator(mismatch_adapter->bind_navigation());
        check(mismatch_navigator.single_player({7,true,scratch/"wrong-selected-file.savestate"},error),error);
        check(!mismatch_navigator.start_game(0,error)&&error.find("disagrees")!=std::string::npos&&
              mismatch_shared->id=="unchanged-on-path-mismatch"&&mismatch_assignments==0&&mismatch_starts==0,
              "Load refused a file path that disagreed with the selected slot instead of substituting the slot resolver");

        auto missing_shared = std::make_shared<CharacterState>();
        missing_shared->id = "unchanged-on-missing-file";
        unsigned missing_creates = 0, missing_assignments = 0, missing_starts = 0;
        auto missing_services = make_flow_services(original.source(), missing_shared, scratch,
                                                   missing_creates, missing_assignments, missing_starts);
        auto missing_adapter = std::make_shared<RuntimeCreationFlowAdapterV1>(std::move(missing_services));
        flow::Navigator missing_navigator(missing_adapter->bind_navigation());
        check(missing_navigator.single_player({9, true}, error), error);
        check(!missing_navigator.start_game(0, error) && error.find("Cannot open save file") != std::string::npos &&
              missing_shared->id == "unchanged-on-missing-file" && missing_creates == 0 &&
              missing_assignments == 0 && missing_starts == 0 && !missing_adapter->start_receipt(),
              "Missing selected existing profile fell through to create or mutated state");

        std::filesystem::remove_all(scratch, ec);
        std::cout << "runtime_creation_flow_adapter_v1: Confirm staging, delayed StartGame, selected existing-file load, same-state/save receipts passed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "runtime_creation_flow_adapter_v1: " << exception.what() << '\n';
        return 1;
    }
}
