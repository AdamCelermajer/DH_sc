#include "equipment_main_page.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

static void check(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

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

        CharacterState character = make_default_character("equipment-main-page", "Knight", "warrior");
        character.inventory = {{"suit-instance", "StartingSuit", 1}};
        ActorState actor;
        EquipmentAdapter adapter(character, actor, properties, table, database);
        equipment_menu::Options equipment_options;
        equipment_options.item_name = [](const InventoryItem& owned, const dh2::data::Item&,
                                         std::string& value, std::string& e) {
            value = "source-item:" + owned.instance_id; e.clear(); return true;
        };
        equipment_options.empty_name = [](std::string& value, std::string& e) {
            value = "GLOBAL_EMPTY"; e.clear(); return true;
        };
        equipment_menu::Presenter equipment(character, table, properties.sheets, adapter,
                                              equipment_options);
        inventory::MenuPresenter inventory_page(character, table);
        inventory::DetailsPresenter details(character, table, equipment);
        inventory::MenuBindings inventory_text;
        inventory_text.item_name = [](const inventory::Row& row, std::string& value, std::string& e) {
            value = "source-item:" + row.instance_id; e.clear(); return true;
        };
        inventory_text.symbol = [](const std::string& symbol, std::string& value, std::string& e) {
            value = symbol; e.clear(); return true;
        };
        inventory_text.potions = [](std::uint32_t count, std::string& value, std::string& e) {
            value = "potions:" + std::to_string(count); e.clear(); return true;
        };
        inventory::DetailBindings details_text;
        details_text.symbol = inventory_text.symbol;
        details_text.item_name = [](const InventoryItem& owned, const dh2::data::Item&,
                                    std::string& value, std::string& e) {
            value = "source-item:" + owned.instance_id; e.clear(); return true;
        };
        equipment_menu::MainPage page(equipment, inventory_page, details,
                                      inventory_text, details_text);

        unsigned other_tab_calls = 0;
        const auto content = page.content_callback([&](character_menu::Tab,
                character_menu::Frame&, std::string& e) {
            ++other_tab_calls; e.clear(); return true;
        });
        character_menu::Frame frame;
        check(content(character_menu::Tab::stats, frame, error), error);
        check(other_tab_calls == 1, "Equipment binding swallowed the other character menu tabs");
        frame.art.batches = character_menu::original_menu_art(character_menu::Tab::equipment).batches;
        check(content(character_menu::Tab::equipment, frame, error), error);
        check(std::any_of(frame.art.batches.begin(), frame.art.batches.end(), [](const auto& batch) {
            return batch.role.find("menu_InventorySheetMain/inv_anim/btn_torso/btimg") != std::string::npos;
        }), "Equipment MAINPAGE omitted original slot art");

        const auto& slot = *std::find_if(equipment_menu::original_slot_art().begin(),
            equipment_menu::original_slot_art().end(), [](const auto& art) { return art.source_slot == 0; });
        check(slot.hit_triangles.size() >= 3, "Original equipment slot0 hit contour unavailable");
        const auto slot_x = (slot.hit_triangles[0].x + slot.hit_triangles[1].x + slot.hit_triangles[2].x) / 3;
        const auto slot_y = (slot.hit_triangles[0].y + slot.hit_triangles[1].y + slot.hit_triangles[2].y) / 3;
        equipment_menu::MainPageCommand command{};
        check(page.release(slot_x, slot_y, command, error), error);
        check(details.is_open() && equipment.selected_slot() == 0 &&
              equipment.selected_instance() == "suit-instance",
              "Slot hit did not open Details with the shared actual instance selection");

        frame = {};
        frame.art.batches = character_menu::original_menu_art(character_menu::Tab::equipment).batches;
        check(page.content(character_menu::Tab::equipment, frame, error), error);
        check(std::any_of(frame.art.batches.begin(), frame.art.batches.end(), [](const auto& batch) {
            return batch.role.find("menu_InventorySheetDetails/") == 0;
        }), "Open MAINPAGE Details panel was not composed");

        const auto& equip_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::equip;
            });
        check(equip_action.triangles.size() >= 3, "Original Details equip contour unavailable");
        const auto equip_x = (equip_action.triangles[0].x + equip_action.triangles[1].x + equip_action.triangles[2].x) / 3;
        const auto equip_y = (equip_action.triangles[0].y + equip_action.triangles[1].y + equip_action.triangles[2].y) / 3;
        check(page.release(equip_x, equip_y, command, error), error);
        check(command == equipment_menu::MainPageCommand::equipped && character.equipment.size() == 1 &&
              character.equipment.front().slot == "slot0" &&
              character.equipment.front().item_instance_id == "suit-instance",
              "Original Details equip action did not mutate through shared adapter");

        const auto& unequip_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::unequip;
            });
        const auto unequip_x = (unequip_action.triangles[0].x + unequip_action.triangles[1].x + unequip_action.triangles[2].x) / 3;
        const auto unequip_y = (unequip_action.triangles[0].y + unequip_action.triangles[1].y + unequip_action.triangles[2].y) / 3;
        check(page.release(unequip_x, unequip_y, command, error), error);
        check(command == equipment_menu::MainPageCommand::unequipped && character.equipment.empty() &&
              actor.equipment.empty(), "Original Details unequip action left a stale owner binding");

        const auto& drop_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::drop;
            });
        const auto drop_x = (drop_action.triangles[0].x + drop_action.triangles[1].x + drop_action.triangles[2].x) / 3;
        const auto drop_y = (drop_action.triangles[0].y + drop_action.triangles[1].y + drop_action.triangles[2].y) / 3;
        check(page.release(drop_x, drop_y, command, error), error);
        check(command == equipment_menu::MainPageCommand::request_drop &&
              character.inventory.size() == 1 && character.equipment.empty(),
              "Details drop did not return to the root world owner without local mutation");

        const auto& auto_equip_action = *std::find_if(inventory::original_inventory_details().actions.begin(),
            inventory::original_inventory_details().actions.end(), [](const auto& hit) {
                return hit.action == inventory::DetailAction::auto_equip;
            });
        const auto auto_x = (auto_equip_action.triangles[0].x + auto_equip_action.triangles[1].x + auto_equip_action.triangles[2].x) / 3;
        const auto auto_y = (auto_equip_action.triangles[0].y + auto_equip_action.triangles[1].y + auto_equip_action.triangles[2].y) / 3;
        check(page.release(auto_x, auto_y, command, error), error);
        check(command == equipment_menu::MainPageCommand::request_auto_equip &&
              character.inventory.size() == 1 && character.equipment.empty() && actor.equipment.empty(),
              "Details Auto-equip request did not reach the root owner without local mutation");

        // The production root binds these callbacks to its already-published
        // Character candidate: resolve the same projected ID against its
        // constructor inventory and call that candidate's one native Gear.
        // This contract test ensures the page forwards the selected identity
        // and never silently commits through the compatibility adapter.
        unsigned native_equip_calls = 0, native_unequip_calls = 0;
        equipment_menu::NativeEquipmentActions native_actions;
        native_actions.equip = [&](const std::string& id, unsigned source_slot, std::string& e) {
            check(id == "suit-instance" && source_slot == 0,
                  "Native action did not receive the shared selected source identity");
            ++native_equip_calls; e.clear(); return true;
        };
        native_actions.unequip = [&](unsigned source_slot, std::string& e) {
            check(source_slot == 0, "Native unequip did not receive the selected source slot");
            ++native_unequip_calls; e.clear(); return true;
        };
        equipment_menu::MainPage native_page(equipment, inventory_page, details,
                                             inventory_text, details_text,
                                             std::move(native_actions));
        check(native_page.release(slot_x, slot_y, command, error), error);
        check(native_page.release(equip_x, equip_y, command, error), error);
        check(command == equipment_menu::MainPageCommand::equipped && native_equip_calls == 1 &&
              character.equipment.empty() && actor.equipment.empty(),
              "Canonical Gear callback did not remain the sole native mutation path");
        check(native_page.release(unequip_x, unequip_y, command, error), error);
        check(command == equipment_menu::MainPageCommand::unequipped && native_unequip_calls == 1 &&
              character.equipment.empty() && actor.equipment.empty(),
              "Canonical Gear unequip callback fell through to compatibility mutation");

        equipment_menu::NativeEquipmentActions partial_actions;
        partial_actions.equip = [](const std::string&, unsigned, std::string& e) {
            e.clear(); return true;
        };
        equipment_menu::MainPage partial_page(equipment, inventory_page, details,
                                               inventory_text, details_text,
                                               std::move(partial_actions));
        check(!partial_page.release(equip_x, equip_y, command, error) &&
              error.find("both equip and unequip") != std::string::npos &&
              character.equipment.empty(),
              "Partial canonical Gear binding was accepted or mutated compatibility state");
        // B046: the Details slot rail. Icon N (SideList/btn_TypeN) sets InvSlotId N; btn_left (up) and btn_right (down)
        // step InvSlotId one slot with wrap (ClassChangeUp 9 -> ... ; ClassChangeDown 9 -> 0). Each step re-selects the
        // slot's first candidate and moves the single shared selection; only the selected icon's Highlight is drawn.
        {
            const auto rail_centre = [&](unsigned slot, float& x, float& y) {
                inventory::DetailRailBox box{};
                check(inventory::details_rail_box(inventory::original_inventory_details(), slot, box), "Details rail icon missing");
                x = (box.x0 + box.x1) * 0.5f;
                y = (box.y0 + box.y1) * 0.5f;
            };
            const auto arrow = [&](inventory::DetailAction action, float& x, float& y) {
                const auto& hit = *std::find_if(inventory::original_inventory_details().actions.begin(),
                    inventory::original_inventory_details().actions.end(), [&](const auto& h) { return h.action == action; });
                x = (hit.triangles[0].x + hit.triangles[1].x + hit.triangles[2].x) / 3;
                y = (hit.triangles[0].y + hit.triangles[1].y + hit.triangles[2].y) / 3;
            };
            const auto highlight_shown = [&](unsigned slot) {
                character_menu::Frame rail_frame;
                rail_frame.art.batches = character_menu::original_menu_art(character_menu::Tab::equipment).batches;
                check(page.content(character_menu::Tab::equipment, rail_frame, error), error);
                const std::string role = "menu_InventorySheetDetails/SideList/btn_Type" + std::to_string(slot) + "/Highlight/";
                return std::any_of(rail_frame.art.batches.begin(), rail_frame.art.batches.end(),
                    [&](const auto& batch) { return batch.role.find(role) == 0; });
            };
            float x = 0, y = 0;
            rail_centre(3, x, y);
            check(page.release(x, y, command, error) && command == equipment_menu::MainPageCommand::none &&
                  details.is_open() && equipment.selected_slot() == 3, "Rail icon 3 did not select InvSlotId 3");
            // Icons with normal art (3) show the bright Highlight only when selected; icon 0 has only Highlight art and stays drawn.
            check(highlight_shown(3) && !highlight_shown(9) && highlight_shown(0),
                  "Rail Highlight must follow the selected icon for icons with normal art");
            rail_centre(9, x, y);
            check(page.release(x, y, command, error) && equipment.selected_slot() == 9,
                  "Rail icon 9 (potions) did not select InvSlotId 9");
            arrow(inventory::DetailAction::next, x, y);
            check(page.release(x, y, command, error) && equipment.selected_slot() == 0 &&
                  equipment.selected_instance() == "suit-instance",
                  "Down arrow at InvSlotId 9 did not wrap to 0 and select its first candidate");
            arrow(inventory::DetailAction::previous, x, y);
            check(page.release(x, y, command, error) && equipment.selected_slot() == 9,
                  "Up arrow at InvSlotId 0 did not wrap to 9");
            arrow(inventory::DetailAction::previous, x, y);
            check(page.release(x, y, command, error) && equipment.selected_slot() == 8,
                  "Up arrow did not step one slot");
            arrow(inventory::DetailAction::next, x, y);
            check(page.release(x, y, command, error) && equipment.selected_slot() == 9 && highlight_shown(9),
                  "Down arrow did not step one slot back, or the highlight did not follow");
            rail_centre(0, x, y);
            check(page.release(x, y, command, error) && equipment.selected_slot() == 0 &&
                  equipment.selected_instance() == "suit-instance" && command == equipment_menu::MainPageCommand::none,
                  "Rail icon 0 did not reselect the torso list");
            check(character.equipment.empty() && actor.equipment.empty(), "Rail navigation mutated equipment");
        }
        page.leave_page();
        check(!details.is_open(), "Leaving MAINPAGE retained Details visibility");
        // Main sheet btn_GAMEPLAYMENUS_AUTOEQUIP_ALL (NativeInvAutoEquipSlot(-1)): the authored banner returns a
        // request to the root owner and mutates nothing locally. The same point does nothing while Details is open.
        {
            const std::string prefix = "menu_InventorySheetMain/btn_GAMEPLAYMENUS_AUTOEQUIP_ALL/";
            float all_x = 0, all_y = 0; bool found = false;
            for (const auto& batch : character_menu::original_menu_art(character_menu::Tab::equipment).batches)
                if (!found && batch.role.compare(0, prefix.size(), prefix) == 0 && batch.triangles.size() >= 3) {
                    all_x = (batch.triangles[0].x + batch.triangles[1].x + batch.triangles[2].x) / 3;
                    all_y = (batch.triangles[0].y + batch.triangles[1].y + batch.triangles[2].y) / 3;
                    found = true;
                }
            check(found, "Original ALL auto-equip banner art unavailable");
            check(page.release(all_x, all_y, command, error), error);
            check(command == equipment_menu::MainPageCommand::request_auto_equip_all && character.equipment.empty() &&
                  actor.equipment.empty() && !details.is_open(), "ALL banner did not return a typed request without local mutation");
            // A press outside every control stays a no-op.
            check(page.release(1.0f, 1.0f, command, error) && command == equipment_menu::MainPageCommand::none, "Empty space produced a command");
        }
        // B057: requirement gate on the Details panel (authored ItemEquippable). Pick a source torso item that needs Energy the
        // fresh Knight lacks; the row must show the red X (Status "No"), EQUIP must be the disabled art and must give no command.
        {
            std::string gated;
            for (std::size_t i = 0; i < table.rows.size() && gated.empty(); ++i) {
                const auto& w = table.rows[i].record.words;
                if (w[26] == 0 && w[34] == 0 && w[29] == 0 && w[30] == 0 && w[31] == 0 && w[32] == 0 && w[33] >= 3 && w[33] <= 12)
                    gated = table.identifiers[i];
            }
            check(!gated.empty(), "No source torso item with an Energy-only requirement in the cache");
            const auto& gated_row = table.rows[dh2::data::item_id(table, gated)];
            const int need = gated_row.record.words[33];
            character.inventory = {{"suit-instance", "StartingSuit", 1}, {"gated-instance", gated, 1}};
            const auto status_in = [&](const character_menu::Frame& f) {
                return std::any_of(f.art.batches.begin(), f.art.batches.end(), [](const auto& b) {
                    return b.role.find("menu_InventorySheetDetails/list/") == 0 && b.role.find("/Status/") != std::string::npos; });
            };
            // Signature of the drawn EQUIP button art (the disabled frame reuses the same authored paths with different shapes).
            const auto equip_art = [&](const character_menu::Frame& f) {
                double sum = 0; std::size_t count = 0;
                for (const auto& b : f.art.batches) if (b.role.find("menu_InventorySheetDetails/btn_EquipItem/") == 0)
                    for (const auto& v : b.triangles) { sum += v.x * 1.31 + v.y * 2.17 + v.u * 3.71 + v.v * 5.03; ++count; }
                return std::make_pair(sum, count);
            };
            const auto compose = [&](character_menu::Frame& f) {
                f = {};
                f.art.batches = character_menu::original_menu_art(character_menu::Tab::equipment).batches;
                check(page.content(character_menu::Tab::equipment, f, error), error);
            };
            check(page.release(slot_x, slot_y, command, error), error);
            check(details.is_open(), "B057 test: Details did not open");
            check(equipment.select_instance("gated-instance", error), error);
            // Below: energy need-1 (and fresh Knight energy is below the item's need).
            properties.sheets.resolved[152] = (need - 1) * 256 + 255;
            character_menu::Frame f; compose(f);
            check(status_in(f), "Unmet item row has no red X (Status No)");
            const auto unmet_art = equip_art(f);
            check(page.release(equip_x, equip_y, command, error) && command == equipment_menu::MainPageCommand::none &&
                  character.equipment.empty(), "Disabled EQUIP on an unmet item still produced a command or equipped it");
            // Equal: exact boundary passes -> no X, EQUIP enabled and equips.
            properties.sheets.resolved[152] = need * 256;
            compose(f);
            check(!status_in(f), "Met item (equal requirement) still shows the red X");
            check(page.release(equip_x, equip_y, command, error) && command == equipment_menu::MainPageCommand::equipped &&
                  character.equipment.size() == 1 && character.equipment.front().item_instance_id == "gated-instance",
                  "Met item (equal requirement) did not equip through the Details EQUIP button");
            // The equipped item is back in the bag for the remaining branches.
            check(page.release(unequip_x, unequip_y, command, error) && character.equipment.empty(), "B057 test: unequip failed");
            check(equipment.select_instance("gated-instance", error), error);
            // Above: more energy keeps it met. Then Prereq_Energy (156) lowers the bar for a character below the need.
            properties.sheets.resolved[152] = (need + 5) * 256; compose(f);
            check(!status_in(f), "Item with more than the required Energy shows the red X");
            properties.sheets.resolved[152] = (need - 1) * 256; properties.sheets.resolved[156] = 256; compose(f);
            check(!status_in(f), "Prereq_Energy bonus not counted by the requirement gate (IsEquippableBy adds Stat + Prereq)");
            const auto met_art = equip_art(f);
            properties.sheets.resolved[156] = 0; compose(f);
            check(status_in(f), "Gate did not revert after the Prereq bonus was removed");
            check(equip_art(f) != met_art && equip_art(f) == unmet_art, "EQUIP button art did not switch between enabled and disabled");
            check(page.release(equip_x, equip_y, command, error) && command == equipment_menu::MainPageCommand::none &&
                  character.equipment.empty(), "Disabled EQUIP released a command after the requirement dropped again");
        }
        std::cout << "equipment_main_page_tests PASS: shared source slot/instance, Details, native Gear routing and compatibility equip/unequip, B057 requirement gate\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
