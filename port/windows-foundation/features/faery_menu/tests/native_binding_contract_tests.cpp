#include "../faery_menu.hpp"
#include <array>
#include <iostream>

namespace dh::foundation::faery_menu {
bool source_text_symbol_v1(const std::string&, std::int32_t, std::int32_t,
                           std::string&, std::string&);
}

int main() {
    using dh::foundation::faery_menu::source_text_symbol_v1;
    const std::array<const char*, 5> field_paths{{
        "menu_FaerySheet/faery_desc/text",
        "menu_FaerySheet/faery_spell/text",
        "menu_FaerySheet/FaeryNameText/text",
        "menu_FaerySheet/menu_title/txt_title",
        "not-a-source-field"}};
    const std::array<std::array<const char*, 3>, 5> expected{{
        {{"GAMEPLAYMENUS_FAERY_1_DESC", "GAMEPLAYMENUS_FAERY_4_DESC", "GAMEPLAYMENUS_FAERY_5_DESC"}},
        {{"GAMEPLAYMENUS_FAERY_1_SPELL", "GAMEPLAYMENUS_FAERY_4_SPELL", "GAMEPLAYMENUS_FAERY_5_SPELL"}},
        {{"GAMEPLAYMENUS_FAERY_1", "GAMEPLAYMENUS_FAERY_4B", "GAMEPLAYMENUS_FAERY_5"}},
        {{"GAMEPLAYMENUS_FAERIES_TITLE", "GAMEPLAYMENUS_FAERIES_TITLE", "GAMEPLAYMENUS_FAERIES_TITLE"}},
        {{"", "", ""}}}};
    const std::array<int, 3> slots{{0, 1, 4}};
    for (std::size_t field = 0; field < 4; ++field) {
        for (std::size_t case_index = 0; case_index < slots.size(); ++case_index) {
            std::string symbol, error;
            const std::int32_t level = case_index == 1 ? 1 : 0;
            if (!source_text_symbol_v1(field_paths[field], slots[case_index], level,
                                       symbol, error) || symbol != expected[field][case_index]) {
                std::cerr << "source text symbol mismatch: " << field_paths[field]
                          << " slot=" << slots[case_index] << " got=" << symbol
                          << " error=" << error << '\n';
                return 1;
            }
        }
    }
    std::string symbol, error;
    if (source_text_symbol_v1(field_paths[0], -1, 0, symbol, error) || error.empty()) return 2;
    if (source_text_symbol_v1(field_paths[4], 0, 0, symbol, error) || error.empty()) return 3;
    std::cout << "PASS native Faery symbol bridge: source menu slot order, first FaeryList symbol IDs, saved-level B suffix, title, and invalid fields\n";
}
