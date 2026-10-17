#pragma once

#include "player_save_inventory_v1.hpp"

namespace dh2::player_gear_save_writer_v1 {

// The source PlayerSavegame callback writes only the GEAR payload. Reuse the
// already selected byte writer and expose the canonical Save + V4 owner pair
// as the only production input; no inventory snapshot or second item store.
using MutableBytes = player_save_inventory_v1::MutableBytes;
using Result = player_save_inventory_v1::Result;
using Status = player_save_inventory_v1::Status;

struct Bindings {
    const data::PlayerSavegameV1* save{};
    const data::FreshInventoryOwnedV4* inventory{};
    data::ItemPowerTablesV5::Borrow powers;
};

Status save(const Bindings&, MutableBytes, Result*, std::string& error);

}  // namespace dh2::player_gear_save_writer_v1
