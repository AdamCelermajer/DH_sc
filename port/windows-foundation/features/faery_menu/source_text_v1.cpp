#include "faery_menu.hpp"

#include <array>
#include <cstdio>

namespace dh::foundation::faery_menu {
namespace {
constexpr std::array<std::int32_t, 5> kMenuSymbols{{1, 4, 2, 3, 5}};
}

bool source_text_symbol_v1(const std::string& field, std::int32_t slot,
                           std::int32_t saved_level, std::string& symbol,
                           std::string& error) {
    if (field == "menu_FaerySheet/menu_title/txt_title") {
        symbol = "GAMEPLAYMENUS_FAERIES_TITLE";
        error.clear();
        return true;
    }
    if (slot < 0 || slot >= static_cast<std::int32_t>(kMenuSymbols.size())) {
        error = "Localized Faery text requires the source selected slot 0..4";
        return false;
    }
    char buffer[64]{};
    const auto localized = kMenuSymbols[static_cast<std::size_t>(slot)];
    if (field == "menu_FaerySheet/faery_desc/text") {
        std::snprintf(buffer, sizeof(buffer), "GAMEPLAYMENUS_FAERY_%d_DESC", localized);
    } else if (field == "menu_FaerySheet/faery_spell/text") {
        std::snprintf(buffer, sizeof(buffer), "GAMEPLAYMENUS_FAERY_%d_SPELL", localized);
    } else if (field == "menu_FaerySheet/FaeryNameText/text") {
        std::snprintf(buffer, sizeof(buffer), "GAMEPLAYMENUS_FAERY_%d%s", localized,
                      saved_level > 0 ? "B" : "");
    } else {
        error = "Unknown authored Faery text field path";
        return false;
    }
    symbol = buffer;
    error.clear();
    return true;
}

} // namespace dh::foundation::faery_menu
