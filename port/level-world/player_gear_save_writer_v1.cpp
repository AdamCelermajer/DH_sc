#include "player_gear_save_writer_v1.hpp"

namespace dh2::player_gear_save_writer_v1 {

Status save(const Bindings& bindings, MutableBytes output, Result* result,
            std::string& error) {
    return player_save_inventory_v1::save(
        player_save_inventory_v1::Bindings{bindings.save, bindings.inventory,
                                           bindings.powers},
        output, result, error);
}

}  // namespace dh2::player_gear_save_writer_v1
