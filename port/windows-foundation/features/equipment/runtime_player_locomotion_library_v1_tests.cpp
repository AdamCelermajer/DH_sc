#include "runtime_player_locomotion_library_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../actor_profiles.hpp"
#include "../../content_paths.hpp"
#include "../../../game-data/items.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;
using namespace dh2::data;

namespace {
void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message.empty() ? "Player locomotion library assertion failed" : message);
}

std::vector<std::uint8_t> read_pydata(const AssetCatalog& assets, const char* name) {
    return read_content(assets, std::string("data/pydata/") + name);
}

std::int32_t source_item_id(const ItemTable& table, const char* name) {
    const auto found = std::find(table.identifiers.begin(), table.identifiers.end(), name);
    require(found != table.identifiers.end(), std::string("Missing actual ItemTable row ") + name);
    const auto id = static_cast<std::size_t>(found - table.identifiers.begin());
    require(id < table.rows.size() && id <= static_cast<std::size_t>(INT32_MAX),
            "Actual ItemTable identifier is outside the source row domain");
    return static_cast<std::int32_t>(id);
}

RuntimePlayerLocomotionLibraryV1 build_for(
        const AssetCatalog& assets, const CharacterVisualConfig& visual,
        std::int32_t main_id, std::int32_t off_id, std::size_t max_programs,
        std::string& error) {
    RuntimePlayerLocomotionLibraryRequestV1 request;
    request.profile_id = "KnightPlayerBase";
    request.role = "library-native-test";
    request.main_hand_item_id = main_id;
    request.off_hand_item_id = off_id;
    request.character_flag_1324 = 0;
    request.max_programs = max_programs;
    RuntimePlayerLocomotionLibraryV1 result;
    require(RuntimePlayerLocomotionLibraryV1::build(assets, visual, request, result, error), error);
    return result;
}

void check_selection(const RuntimePlayerLocomotionSelectionV1& selected,
                     std::int32_t stance, const AssetCatalog& assets) {
    require(bool(selected) && selected.stance == stance, "Selected actual source stance differs");
    require(selected.program && selected.program->plan.sequences.size() == 3,
            "Selection does not retain the source Idle/Walk/Run program");
    for (const char* state : {"Idle", "Walk", "Run"}) {
        const auto* sequence = selected.program->plan.sequence(state, 0);
        require(sequence && sequence->id >= 0 && !sequence->phases.empty(),
                std::string("Missing source program state ") + state);
        const auto policy = selected.program->policies.find(sequence->id);
        require(policy != selected.program->policies.end() && policy->second.id == sequence->id &&
                policy->second.name == sequence->name && policy->second.loop == sequence->loop &&
                policy->second.type == sequence->type,
                "Source sequence identity/alias/Loop/Type policy changed");
    }
    require(!selected.program->named_clips.empty(), "Source program has no exact named clip receipts");
    for (const auto& receipt : selected.program->named_clips) {
        require(!receipt.source_uri.empty() && !receipt.source_alias.empty() &&
                !receipt.named_alias.empty() && receipt.animation_id >= 0,
                "Source clip receipt lost its source ID/alias/URI");
        require(assets.resolve(receipt.resolved_path).is_absolute(),
                "Named clip receipt is not in the same AssetCatalog");
        const auto step = selected.program->steps.find({receipt.state, receipt.source_path});
        require(step != selected.program->steps.end() && step->second.anim == receipt.animation_id &&
                step->second.speed == receipt.speed && step->second.blend_out == receipt.blend_out &&
                step->second.move_go == receipt.move_go,
                "Clip receipt changed source Speed/BlendOut/MoveGO or phase ID");
        const auto rate = selected.program->plan.clipRates.find(receipt.named_alias);
        require(rate != selected.program->plan.clipRates.end() && rate->second == receipt.speed,
                "External clip bank lost the source Speed value");
    }
}
}

int main(int argc, char** argv) {
    try {
        require(argc == 2, "Caller-supplied original AssetCatalog root required");
        const AssetCatalog assets(argv[1]);
        std::string error;
        ActorProfileLibrary profiles;
        require(profiles.load(assets, "actor-profiles-v2.xml", error), error);
        const auto* profile = profiles.find("KnightPlayerBase");
        require(profile && profile->animation_table == "48", "Actual source player profile/AnimTable row absent");
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        CharacterVisualConfig base_visual;
        require(make_visual_config(assets, *profile, customization, base_visual, error), error);

        const auto item_data = read_pydata(assets, "loot_table_pyarray.bin");
        const auto item_names = read_pydata(assets, "loot_table_pyarraynames.bin");
        const auto item_fields = read_pydata(assets, "loot_table_pystructnames.bin");
        ItemTable item_table;
        require(load_items({item_data.data(), item_data.size()},
                           {item_names.data(), item_names.size()},
                           {item_fields.data(), item_fields.size()}, item_table, error), error);
        const auto sword = source_item_id(item_table, "Longsword01");
        const auto dagger = source_item_id(item_table, "Dagger01");
        const auto staff = source_item_id(item_table, "Staff01");

        auto dual_library = build_for(assets, base_visual, sword, dagger, 4, error);
        require(dual_library.program_count() == 4, "Current pair/null combinations were not deduplicated and bounded");
        RuntimePlayerLocomotionSelectionV1 dual;
        require(dual_library.select_exact(sword, dagger, dual, error), error);
        check_selection(dual, 2, assets);
        RuntimePlayerLocomotionSelectionV1 ordinary;
        require(dual_library.select_ordinary_one_hand(sword, ordinary, error), error);
        check_selection(ordinary, 0, assets);
        RuntimePlayerLocomotionSelectionV1 off_only;
        require(dual_library.select_exact(-1, dagger, off_only, error), error);
        check_selection(off_only, off_only.stance, assets);
        RuntimePlayerLocomotionSelectionV1 empty_hands;
        require(dual_library.select_exact(-1, -1, empty_hands, error), error);
        check_selection(empty_hands, 0, assets);
        RuntimePlayerLocomotionSelectionV1 missing_selection;
        require(!dual_library.select_exact(staff, -1, missing_selection, error) && !missing_selection,
                "Selection outside the prepared current/null combinations was accepted");
        RuntimePlayerLocomotionSelectionV1 invalid_negative = ordinary;
        require(!dual_library.select_ordinary_one_hand(-1, invalid_negative, error) &&
                invalid_negative.program == ordinary.program,
                "Negative/non-item main-hand ID was accepted as an ordinary one-hand selection");

        auto empty_library = build_for(assets, base_visual, -1, -1, 1, error);
        require(empty_library.program_count() == 1,
                "Identical empty-hand/null tuples were not deduplicated before the caller budget check");
        auto one_hand_library = build_for(assets, base_visual, sword, -1, 2, error);
        require(one_hand_library.program_count() == 2,
                "Identical one-hand/null tuples were not deduplicated before the caller budget check");

        CharacterVisualConfig merged_visual = base_visual;
        require(dual_library.merge_named_clips(merged_visual, error), error);
        require(merged_visual.model_path == base_visual.model_path &&
                merged_visual.clips.size() > base_visual.clips.size(),
                "Exact named clip receipts were not merged into the same visual config");
        for (const auto& receipt : dual.program->named_clips) {
            const auto found = std::find(merged_visual.clips.begin(), merged_visual.clips.end(),
                std::make_pair(receipt.named_alias, receipt.resolved_path));
            require(found != merged_visual.clips.end(), "Merged config omitted an exact named clip receipt");
        }

        auto staff_library = build_for(assets, base_visual, staff, -1, 4, error);
        RuntimePlayerLocomotionSelectionV1 staff_selection;
        require(staff_library.select_exact(staff, -1, staff_selection, error), error);
        check_selection(staff_selection, 3, assets);
        RuntimePlayerLocomotionSelectionV1 invalid_ordinary = ordinary;
        require(!staff_library.select_ordinary_one_hand(staff, invalid_ordinary, error) &&
                invalid_ordinary.program == ordinary.program,
                "Staff was accepted as an ordinary one-hand stance or changed output on failure");

        // An undersized request fails before replacing a previously complete
        // library. Its existing shared selection lease remains valid.
        auto preserved_program = ordinary.program;
        RuntimePlayerLocomotionLibraryRequestV1 too_small;
        too_small.profile_id = "KnightPlayerBase";
        too_small.role = "library-native-test";
        too_small.main_hand_item_id = sword;
        too_small.off_hand_item_id = dagger;
        too_small.max_programs = 3;
        const auto old_count = dual_library.program_count();
        require(!RuntimePlayerLocomotionLibraryV1::build(assets, base_visual, too_small,
                    dual_library, error) && dual_library.program_count() == old_count,
                "Over-budget equipment/null requests were not rejected atomically");
        require(preserved_program && preserved_program->plan.sequence("Idle", 0),
                "Selection shared ownership did not preserve its source program");
        too_small.max_programs = RuntimePlayerLocomotionLibraryV1::hard_program_limit + 1;
        require(!RuntimePlayerLocomotionLibraryV1::build(assets, base_visual, too_small,
                    dual_library, error) && dual_library.program_count() == old_count,
                "Request above the hard program limit was not rejected atomically");
        too_small.max_programs = 4;
        too_small.profile_library_uri = "missing/custom-profile-table.xml";
        require(!RuntimePlayerLocomotionLibraryV1::build(assets, base_visual, too_small,
                    dual_library, error) && dual_library.program_count() == old_count &&
                dual_library.select_exact(sword, dagger, dual, error),
                "Missing caller-selected profile table did not fail atomically");

        CharacterVisualConfig colliding_visual = base_visual;
        const auto& collision = dual.program->named_clips.front();
        colliding_visual.clips.emplace_back(collision.named_alias, "models/not-the-source-clip.bdae");
        const auto collision_before = colliding_visual.clips;
        require(!dual_library.merge_named_clips(colliding_visual, error) &&
                colliding_visual.clips == collision_before,
                "Named clip collision partially mutated the caller's visual config");

        // A caller retains a selection lease even after the library drops its
        // own entries; no session/actor/clock is part of that ownership.
        auto retained = dual;
        dual_library = RuntimePlayerLocomotionLibraryV1{};
        require(retained.program && retained.program->plan.sequence("Walk", 0),
                "External bank selection lease did not outlive its library");

        std::cout << "PASS exact AssetCatalog tables/profile/constants; ordinary stance=0, dual=2, staff=3; "
                     "null combinations, selection leases, external clip merge and atomic bounds/collisions\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
