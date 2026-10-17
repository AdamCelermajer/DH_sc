#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::skinning {

// One exact category/module row from the original prince_modular BRES. The
// module ID is local to its category; controller_index is the independent
// BRES controller-library index used to load its mesh and skin.
struct ModularSkinModule {
    std::string category;
    std::string item_name;
    std::string controller;
    std::uint32_t category_id=0;
    std::uint32_t module_id=0;
    std::uint32_t controller_index=0;
};

struct ModularSkinCatalog {
    std::vector<ModularSkinModule> modules;
};

// Reads the modular categories embedded in the Prince model itself and binds
// every URI to exactly one controller-library row. No synthetic aliases.
bool load_modular_skin_catalog(const std::uint8_t* model,std::size_t size,
                               ModularSkinCatalog&,std::string& error);

const ModularSkinModule* find_modular_skin(const ModularSkinCatalog&,
                                           const std::string& category,
                                           const std::string& item_name) noexcept;

// Mirrors Character::INV_UpdateSkin's exact -> __placeholder / __naked
// selection. Failure leaves callers free to retain the already-visible skin.
bool resolve_modular_skin(const ModularSkinCatalog&,const std::string& category,
                          const std::string& equipped_item,bool equipped,
                          const ModularSkinModule*& result,bool& placeholder,
                          std::string& error);

} // namespace dh2::skinning
