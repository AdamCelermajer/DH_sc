#include "../app/src/main/cpp/native_loot_pickup_quest_binding_v1.hpp"
#include "../app/src/main/cpp/native_quest_owner.hpp"
#include "../../game-data/loot_tables_v2.hpp"
#include "../../game-data/properties.hpp"
#include "../../level-world/source_level_owner_v1.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace n = dh2::native::quests;
namespace d = dh2::data;
namespace t = d::quest_table_bindings_v1;
namespace binding = dh2::native::loot_pickup_quest_binding_v1;
namespace bridge = dh2::character::loot_pickup_event_bridge_v1;

namespace {
using Raw = std::vector<std::uint8_t>;
Raw read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("missing fixture: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
struct Seen {
    unsigned count{};
    std::uintptr_t expected_character{}, character{};
    std::int32_t type{}, item{};
};
std::int32_t receive(void* raw, n::Owner::LevelEventRuntime&,
                     n::Owner::LevelEvent& event, std::string&) {
    auto& seen = *static_cast<Seen*>(raw);
    ++seen.count;
    seen.character = event.character;
    seen.type = event.objective_type;
    seen.item = event.item_id;
    return 0;
}
bool is_player(void* raw, std::uintptr_t character, bool& result,
               std::string&) {
    result = character == static_cast<Seen*>(raw)->expected_character;
    return true;
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "expected original cache directory");
        const std::string cache = argv[1];
        auto quest_bytes = std::make_shared<const Raw>(read(cache + "/v2quests_pyarray.bin"));
        auto quest_names = std::make_shared<const Raw>(read(cache + "/v2quests_pyarraynames.bin"));
        auto quest_constants = read(cache + "/v2quests_pycst.bin");
        t::Input quest_input;
        check(dh2_quests_open(&quest_input.table, quest_bytes->data(),
              std::uint32_t(quest_bytes->size())) == 0, "decode quest table");
        quest_input.packed_owner = quest_bytes;
        quest_input.names = quest_names->data();
        quest_input.names_size = quest_names->size();
        quest_input.names_owner = quest_names;
        t::Owner quest_table;
        std::string error;
        check(quest_table.load(quest_input, error), error);
        n::Constants constants;
        check(dh2_pycst_open(&constants.view, quest_constants.data(),
              std::uint32_t(quest_constants.size())) == 0, "decode quest constants");
        constants.owner = std::make_shared<const Raw>(quest_constants);
        constexpr std::uintptr_t character = 0x9001;
        auto save = std::make_shared<d::PlayerSavegameV1>();
        save->set_character(character);
        n::Owner quests(save, quest_table.borrow(), constants);
        check(quests.initialize(0, error), "initialize canonical Quest owner: " + error);

        auto loot_bytes = read(cache + "/loot_table_pyarray.bin");
        auto loot_names = read(cache + "/loot_table_pyarraynames.bin");
        auto loot_schema = read(cache + "/loot_table_pystructnames.bin");
        d::LootTablesV2 loot;
        check(loot.load({loot_bytes.data(), std::uint32_t(loot_bytes.size())},
              {loot_names.data(), std::uint32_t(loot_names.size())},
              {loot_schema.data(), std::uint32_t(loot_schema.size())}, error), error);
        d::PropertyState properties{};
        d::FreshInventoryOwnedV4 inventory(character, loot.borrow(), {}, 0, properties);
        constexpr std::int32_t item = 841, gather_type = 73;
        check(inventory.register_quest_gathering_item_id(item, error), error);

        dh2::source_level_owner_v1::Owner level;
        int projection{}, fields{};
        check(level.begin_load() && level.request_level(1, 0, 0) &&
              level.begin_source_load() && level.publish_projection(&projection) &&
              level.bind_native_level({&fields, &quests,
                  reinterpret_cast<std::uintptr_t>(save.get()), character,
                  1, 0, 0, 38}), "bind active canonical Level chain");
        Seen seen{};
        seen.expected_character = character;
        check(quests.attach_current_level_receiver(gather_type, 0x7711, 0,
              &seen, receive, error), error);
        binding::Binding pickup{&level, &quests, &inventory, gather_type,
                                &seen, is_player};
        bridge::Result result{};
        check(pickup.after_transfer(character, item, &result, error) ==
              bridge::Status::raised, "production pickup binding: " + error);
        check(result.registered_item && seen.count == 1 &&
              seen.character == character && seen.item == item &&
              seen.type == gather_type && result.event_owner_identity ==
              reinterpret_cast<std::uintptr_t>(&quests),
              "GatherLoot was not raised on the canonical Level Quest owner");
        std::cout << "PASS: native pickup binding dispatched one canonical GatherLoot event\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
