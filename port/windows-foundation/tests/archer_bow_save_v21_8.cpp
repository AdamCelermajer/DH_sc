// v21.8 w2-archer: TEST-ONLY save writer (not part of the product, not a CTest). It turns a character save into a
// bow-equipped save for a real hidden-window run: the main-hand binding (EquipmentBinding source_slot 1) takes the
// definition Bow01 (ItemTable Type 4, ProjectileTable row 10 BowProjectile), and the off-hand binding (source_slot 2)
// is removed, so the stance is the bow stance and no second weapon attacks.
//
// usage: archer_bow_save_v21_8 <in.save> <out.save>
#include "../save_store.hpp"

#include <iostream>
#include <string>

using namespace dh::foundation;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: archer_bow_save_v21_8 <in.save> <out.save>\n";
        return 2;
    }
    std::string error;
    CharacterState state;
    if (!load_character(argv[1], state, error)) {
        std::cerr << "load failed: " << error << "\n";
        return 1;
    }
    int main_bound = 0;
    for (auto it = state.equipment.begin(); it != state.equipment.end();) {
        if (it->source_slot == 2) {
            it = state.equipment.erase(it);
            continue;
        }
        if (it->source_slot == 1) {
            bool found = false;
            for (auto& item : state.inventory) {
                if (item.instance_id == it->item_instance_id) {
                    item.definition_id = "Bow01";
                    found = true;
                }
            }
            if (!found) {
                std::cerr << "main-hand instance is not in the inventory: " << it->item_instance_id << "\n";
                return 1;
            }
            ++main_bound;
        }
        ++it;
    }
    if (main_bound != 1) {
        std::cerr << "expected exactly one main-hand binding, found " << main_bound << "\n";
        return 1;
    }
    if (!save_character(argv[2], state, error)) {
        std::cerr << "save failed: " << error << "\n";
        return 1;
    }
    std::cout << "wrote " << argv[2] << " class=" << state.class_id << " main=Bow01\n";
    return 0;
}
