#ifdef NDEBUG
#undef NDEBUG
#endif
#include "player_saved_skill_slots_v1.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh2::data;
using namespace dh2::player_saved_skill_slots_v1;

namespace {
unsigned checks = 0;
const char* stage = "startup";
void require(bool value) {
    ++checks;
    if (!value) throw std::runtime_error("saved-slot V4 binding check " +
                                         std::to_string(checks) + " at " + stage);
}
std::vector<std::uint8_t> file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(bool(input));
    return {std::istreambuf_iterator<char>(input), {}};
}
struct UpdateContext {
    PlayerSavegameV1* save{};
    std::uintptr_t identity{};
    std::int32_t slot{-1};
    std::uint32_t row{};
    std::uint32_t calls{};
    bool fail{};
    bool saw_mutated_map{};
};
bool update_skills(void* opaque, std::uintptr_t character, std::string& error) {
    auto& context = *static_cast<UpdateContext*>(opaque);
    ++context.calls;
    context.saw_mutated_map =
        character == context.identity &&
        context.save->skill_in_slot(context.slot) ==
            static_cast<std::int32_t>(context.row) &&
        context.save->skill_slots()[1].empty();
    if (context.fail) {
        error = "source UpdateSkills returned failure";
        return false;
    }
    return context.saw_mutated_map;
}
}

int main(int argc, char** argv) {
    try {
        require(argc == 2);
        const std::string cache = argv[1];
        const auto records = file(cache + "/loot_table_pyarray.bin");
        const auto names = file(cache + "/loot_table_pyarraynames.bin");
        const auto schema = file(cache + "/loot_table_pystructnames.bin");
        LootTablesV2 loot;
        std::string error;
        require(loot.load({records.data(), records.size()},
                          {names.data(), names.size()},
                          {schema.data(), schema.size()}, error));

        constexpr std::uintptr_t identity = UINT64_C(0x1234567800000099);
        PropertyState properties{};
        FreshInventoryOwnedV4 inventory(identity, loot.borrow(), {}, 12,
                                         properties);
        PlayerSavegameV1 save;
        save.set_character(identity);
        require(save.initialize_skills_from_character_list({11, 22, 33}, error));
        BoundSlotsV1 slots(save, inventory);
        require(slots.owner_matches());
        require(slots.has_skill_slots().status == Status::ok &&
                slots.has_skill_slots().value == 0);
        require(slots.skill_in_slot(0).status == Status::ok &&
                slots.skill_in_slot(0).value == -1);

        for (const auto selector : {-9, -1, 0, 1, 2, 3, 99})
            require(source_current_skill_set(inventory, selector) == 0);
        require(source_current_equipment_set(inventory, -1) == 0);
        require(source_current_equipment_set(inventory, 0) == 0);
        require(source_current_equipment_set(inventory, 1) == 0);
        require(source_current_equipment_set(inventory, 2) == 0);
        require(source_current_equipment_set(inventory, 3) == 0);

        stage = "first map-zero slot assignment";
        UpdateContext update{&save, identity, 0, 0};
        const SavedSkillUpdateServicesV1 services{&update, update_skills};
        require(slots.set_skill_in_slot(0, 0, services, error) == Status::ok);
        require(update.calls == 1 && update.saw_mutated_map);
        require(slots.has_skill_slots().value == 1 &&
                slots.skill_in_slot(0).value == 0 &&
                slots.skill_slot(0).value == 0);
        require(save.skill_slots()[0].size() == 1 &&
                save.skill_slots()[1].empty());

        stage = "equipment-map separation";
        inventory.swap_equipment();
        require(inventory.current_equipment() == 1);
        require(source_current_skill_set(inventory, -1) == 0);
        require(source_current_equipment_set(inventory, -1) == 1);
        require(source_current_equipment_set(inventory, 0) == 0);
        require(source_current_equipment_set(inventory, 1) == 1);
        require(source_current_equipment_set(inventory, 2) == 1);
        require(source_current_equipment_set(inventory, 3) == 0);
        update.slot = 1;
        update.row = 1;
        require(slots.set_skill_in_slot(1, 1, services, error) == Status::ok);
        require(update.calls == 2 && update.saw_mutated_map);
        require(save.skill_slots()[0].size() == 2 &&
                save.skill_slots()[1].empty());
        require(slots.skill_in_slot(0).value == 0 &&
                slots.skill_in_slot(1).value == 1 &&
                slots.skill_slot(0).value == 0 &&
                slots.skill_slot(1).value == 1);

        // Equipment selection does not choose the saved skill map. The source
        // selector still returns map zero after swapping back as well.
        stage = "swapped selection and saved slot readback";
        inventory.swap_equipment();
        require(inventory.current_equipment() == 0);
        require(slots.skill_in_slot(1).value == 1 &&
                save.skill_slots()[1].empty());

        stage = "duplicate saved-row assignment";
        update.slot = 2;
        update.row = 1;
        require(slots.set_skill_in_slot(2, 1, services, error) == Status::ok);
        require(update.calls == 3 && update.saw_mutated_map);
        require(slots.skill_in_slot(1).value == -1 &&
                slots.skill_in_slot(2).value == 1 &&
                save.skill_slots()[1].empty());

        stage = "source sentinel removal";
        // Source sentinel removal is a map mutation without UpdateSkills.
        require(slots.set_skill_in_slot(2, UINT32_MAX, services, error) ==
                Status::ok);
        require(update.calls == 3 && slots.skill_in_slot(2).value == -1 &&
                slots.skill_in_slot(0).value == 0 && save.has_skill_slots());

        stage = "UpdateSkills failure prefix";
        // The original normal setter mutates map zero before the update call;
        // a false source callback retains that prefix.
        update.slot = 3;
        update.row = 2;
        update.fail = true;
        require(slots.set_skill_in_slot(3, 2, services, error) ==
                Status::source_rejected);
        require(update.calls == 4 && update.saw_mutated_map &&
                slots.skill_in_slot(3).value == 2 &&
                error == "source UpdateSkills returned failure");

        stage = "mutable save identity rejection";
        // Full-width owner identity is checked for every call, so rebinding the
        // mutable save identity cannot make stale slots appear valid.
        save.set_character(identity + UINT64_C(0x100000000));
        require(!slots.owner_matches());
        auto mismatch = slots.skill_in_slot(3);
        require(mismatch.status == Status::owner_mismatch && mismatch.value == -1);
        require(slots.set_skill_in_slot(4, 0, services, error) ==
                Status::owner_mismatch);
        require(update.calls == 4 && save.skill_slots()[0].size() == 2 &&
                save.skill_slots()[1].empty());
        save.set_character(identity);
        require(slots.owner_matches());

        stage = "foreign inventory owner rejection";
        // A genuinely different inventory owner is rejected without mutating
        // the saved owner, even when only the high identity word differs.
        FreshInventoryOwnedV4 other_inventory(identity + UINT64_C(0x100000000),
                                                loot.borrow(), {}, 12,
                                                properties);
        BoundSlotsV1 wrong_pair(save, other_inventory);
        require(!wrong_pair.owner_matches());
        require(wrong_pair.has_skill_slots().status == Status::owner_mismatch);
        require(wrong_pair.set_skill_in_slot(5, 0, services, error) ==
                Status::owner_mismatch);
        require(update.calls == 4 && save.skill_slots()[1].empty());

        stage = "invalid and missing source effects";
        // Missing UpdateSkills is rejected before map mutation for a normal
        // assignment; a negative slot is rejected by the existing save owner.
        require(slots.set_skill_in_slot(6, 0, {}, error) ==
                Status::source_rejected);
        require(slots.skill_in_slot(6).value == -1);
        require(slots.set_skill_in_slot(-1, 0, services, error) ==
                Status::source_rejected);
        require(slots.skill_in_slot(-1).value == -1);

        std::cout << "{\"validation\":\"PASS\",\"saved_owner_identity\":\"0x1234567800000099\","
                     "\"source_skill_set_selector\":\"constant_zero\","
                     "\"source_equipment_set_selector\":\"verified_separately\","
                     "\"equipment_swaps\":2,\"saved_slot_map\":0,"
                     "\"map1_mutations\":0,\"update_calls\":4,"
                     "\"owner_mismatch_rejections\":2,\"checks\":"
                  << checks << ",\"mismatches\":0}\n";
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
