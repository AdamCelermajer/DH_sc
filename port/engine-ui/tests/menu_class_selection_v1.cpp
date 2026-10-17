#include "../menu_class_selection_v1.hpp"

#include <cstring>
#include <iostream>

int main() {
    using dh2::ui::menu_class_choice_v1;
    using dh2::ui::menu_class_selection_click_v1;
    constexpr const char* characters[] = {
        "KnightPlayerBase", "RoguePlayerBase", "MagePlayerBase"
    };
    constexpr const char* titles[] = {
        "MENU_CLASS_00", "MENU_CLASS_01", "MENU_CLASS_02"
    };
    constexpr const char* descriptions[] = {
        "MENU_KNIGHT_DESC", "MENU_ROGUE_DESC", "MENU_MAGE_DESC"
    };
    unsigned checks = 0;
    bool ok = true;
    const auto check = [&](bool value, const char* message) {
        if (!value) std::cerr << "FAIL: " << message << '\n';
        ok &= value;
        ++checks;
    };
    for (int index = 0; index < 3; ++index) {
        const auto* choice = menu_class_choice_v1(index);
        check(choice && std::strcmp(choice->profile_character, characters[index]) == 0,
              "selected index reaches the matching NativeCreateSaveSlot class");
        check(choice && std::strcmp(choice->title_symbol, titles[index]) == 0,
              "selected index keeps the authored title for its persisted class");
        check(choice && std::strcmp(choice->description_symbol, descriptions[index]) == 0,
              "selected index keeps the authored description for its persisted class");
    }
    check(menu_class_choice_v1(-1) == nullptr && menu_class_choice_v1(3) == nullptr,
          "invalid source selection index cannot alias a persisted class");
    int next = -1;
    check(menu_class_selection_click_v1(2, 30, 10, 30, true, 0, next) && next == 1,
          "authored right Character click advances the source class index");
    check(menu_class_choice_v1(next) &&
              std::strcmp(menu_class_choice_v1(next)->profile_character, "RoguePlayerBase") == 0,
          "clicked class is the exact class passed to profile creation");
    check(menu_class_selection_click_v1(2, 30, 10, 30, true, 1, next) && next == 2 &&
              std::strcmp(menu_class_choice_v1(next)->profile_character, "MagePlayerBase") == 0,
          "second right click reaches the final authored class");
    check(!menu_class_selection_click_v1(2, 30, 10, 30, true, 2, next) && next == 2,
          "right edge click is clamped without changing selection");
    check(menu_class_selection_click_v1(2, 10, 10, 30, true, 2, next) && next == 1,
          "authored left Character click moves back one class");
    check(!menu_class_selection_click_v1(6, 30, 10, 30, true, 0, next) && next == 0,
          "onRelease alone does not change class before on_clicked");
    check(!menu_class_selection_click_v1(2, 30, 10, 30, false, 0, next) && next == 0,
          "class clicks during source camera transition are ignored");
    check(!menu_class_selection_click_v1(2, 99, 10, 30, true, 0, next) && next == 0,
          "unrelated Character click cannot change the selected class");
    check(!menu_class_selection_click_v1(2, 30, 0, 30, true, 0, next) && next == 0,
          "missing authored button identity fails closed");
    check(!menu_class_selection_click_v1(2, 30, 10, 30, true, 3, next) && next == 3,
          "invalid source index cannot be advanced or persisted");
    std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
              << "\",\"checks\":" << checks << "}\n";
    return ok ? 0 : 1;
}
