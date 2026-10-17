#pragma once

#include <string>
#include <utility>

namespace dh2::native::player_inventory_save_commit_v1 {

// Call only after the canonical gameplay owner has committed a mutation.
// The Save callback must reach that Character's registered PlayerSavegame
// transport; failure leaves the in-memory source prefix intact and explicit.
template <class Save>
bool after_successful_mutation(Save&& save, std::string& error) {
    if (std::forward<Save>(save)(error)) return true;
    error = "Gameplay mutation completed in memory, but canonical SG_Save failed: " + error;
    return false;
}

}  // namespace dh2::native::player_inventory_save_commit_v1
