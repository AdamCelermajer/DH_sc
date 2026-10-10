// P15 DROPGATE: the Details Drop button follows the SELECTED item instance, not the definition or the slot.
// Original (IDA NativeInvGetItemsListForSlot, authored displaySelectedItemInfos 0001cbbe): a row is ItemEquipped when it
// is the item equipped in the listed slot, and ItemEquippedOtherHand when it is equipped in the paired slot (other hand,
// other ring). Either flag hides Drop and disables Transmute. Our flag is the instance-level equipped state, which covers
// both cases for the lists this presenter builds.
#include "../equipment/equipment_main_page.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

static void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

namespace {
bool has_drop(const character_menu::Frame& frame) {
    return std::any_of(frame.art.batches.begin(), frame.art.batches.end(),
                       [](const auto& b) { return b.role.find("menu_InventorySheetDetails/btn_Drop/") == 0; });
}
bool has_drop_text(const character_menu::Frame& frame) {
    return std::any_of(frame.text.begin(), frame.text.end(),
                       [](const auto& f) { return f.field.path.find("menu_InventorySheetDetails/btn_Drop/") == 0; });
}
}  // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply staged original asset root");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase database;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        OriginalActorProperties properties;
        check(resolve_original_fresh_player(database, "KnightPlayerBase", properties, error), error);
        const auto root = std::filesystem::path("original-cache/data/pydata");
        const auto bytes = assets.read(root / "loot_table_pyarray.bin");
        const auto names = assets.read(root / "loot_table_pyarraynames.bin");
        const auto structs = assets.read(root / "loot_table_pystructnames.bin");
        dh2::data::ItemTable table;
        check(dh2::data::load_items({bytes.data(), bytes.size()}, {names.data(), names.size()},
                                    {structs.data(), structs.size()}, table, error), error);

        inventory::DetailBindings bindings;
        bindings.symbol = [](const std::string& symbol, std::string& value, std::string&) {
            value = symbol;
            return true;
        };
        bindings.item_name = [](const InventoryItem& owned, const dh2::data::Item&, std::string& value, std::string& e) {
            value = "item:" + owned.instance_id;
            e.clear();
            return true;
        };

        // Case 1: the same definition (Longsword01) equipped in slot 1 and as a bag copy. Slot 1 is the right hand.
        {
            CharacterState character = make_default_character("drop-gate", "Knight", "warrior");
            character.inventory = {{"sword", "Longsword01", 1}, {"sword-bag", "Longsword01", 1}};
            character.equipment = {{"slot-1", "sword", 0, 1}};
            ActorState actor;
            EquipmentAdapter adapter(character, actor, properties, table, database);
            equipment_menu::Presenter equipment(character, table, properties.sheets, adapter);
            inventory::DetailsPresenter details(character, table, equipment);
            check(details.open(1, error), error);
            check(equipment.select_instance("sword-bag", error), error);
            character_menu::Frame frame;
            check(details.frame(bindings, frame, error), error);
            check(has_drop(frame), "Bag copy of the equipped definition lost its Drop button");
            check(equipment.select_instance("sword", error), error);
            frame = {};
            check(details.frame(bindings, frame, error), error);
            check(!has_drop(frame) && !has_drop_text(frame),
                  "Equipped selection still shows Drop (original hides it on the equipped path)");
        }

        // Case 2: rings. Ring01 is equipped in ring slot 5 (LeftHandRingFinger). Ring02 is a bag ring.
        // Viewing ring slot 6 (RightHandRingFinger) lists both; the equipped ring is the paired-slot item, so Drop
        // is hidden for it (ItemEquippedOtherHand), while the bag ring keeps Drop.
        {
            CharacterState character = make_default_character("drop-gate-rings", "Knight", "warrior");
            character.inventory = {{"ring1", "Ring01", 1}, {"ring2", "Ring02", 1}};
            character.equipment = {{"slot-5", "ring1", 0, 5}};
            ActorState actor;
            EquipmentAdapter adapter(character, actor, properties, table, database);
            equipment_menu::Presenter equipment(character, table, properties.sheets, adapter);
            inventory::DetailsPresenter details(character, table, equipment);
            check(details.open(6, error), error);
            check(equipment.select_instance("ring2", error), error);
            character_menu::Frame frame;
            check(details.frame(bindings, frame, error), error);
            check(has_drop(frame), "Bag ring beside the ring equipped in the paired slot lost Drop");
            check(equipment.select_instance("ring1", error), error);
            frame = {};
            check(details.frame(bindings, frame, error), error);
            check(!has_drop(frame), "Ring equipped in the paired slot still shows Drop");
        }

        std::cout << "inventory_drop_gate_tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "inventory_drop_gate_tests failed: " << e.what() << '\n';
        return 1;
    }
}
