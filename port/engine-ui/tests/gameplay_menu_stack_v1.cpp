#include "../gameplay_menu_stack_v1.hpp"

#include <iostream>
#include <stdexcept>

namespace {
unsigned checks{};
void require(bool ok, const char* message) {
    ++checks;
    if (!ok) throw std::runtime_error(message);
}
}

int main() {
    using namespace dh2::ui;
    try {
        const auto open = gameplay_menu_transition_plan_v1("", "menu_CharacterMenu", true);
        require(open.size() == 1 && open[0].action == GameplayMenuTransitionActionV1::show &&
                    open[0].menu == "menu_CharacterMenu" && open[0].pushed,
                "opening the character menu should show its root");

        const auto nested = gameplay_menu_transition_plan_v1(
            "menu_CharacterMenu", "menu_CharacterSheetNew", true);
        require(nested.size() == 2 && nested[0].action == GameplayMenuTransitionActionV1::cover &&
                    nested[0].menu == "menu_CharacterMenu" &&
                    nested[1].action == GameplayMenuTransitionActionV1::show &&
                    nested[1].menu == "menu_CharacterSheetNew",
                "pushing a child must send parent OnHide without hiding its tab-bearing clip");

        const std::vector<std::string> stack{
            "menu_CharacterMenu", "menu_CharacterSheetNew", "menu_CharacterSheetStats"};
        std::vector<std::string> drawn;
        require(gameplay_menu_draw_stack_v1(stack, [&](const std::string& menu) {
                    drawn.push_back(menu);
                    return true;
                }),
                "stack renderer rejected a menu");
        require(drawn == stack, "visible menu stack must render parent before nested child panels");

        require(gameplay_menu_named_pop_requested_v1(stack, "menu_CharacterMenu"),
                "named NativePopMenu must pop current when the requested menu exists below it");
        require(gameplay_menu_named_pop_requested_v1(stack, "menu_CharacterSheetStats"),
                "named NativePopMenu must also accept the current menu name");
        require(!gameplay_menu_named_pop_requested_v1(stack, "menu_Inventory"),
                "named NativePopMenu must leave the stack untouched for an absent name");
        const std::vector<std::string> skill_confirmation{
            "menu_CharacterMenu", "menu_SkillTreeSheetNew", "menu_confirm2"};
        require(gameplay_menu_back_dismisses_overlay_v1(skill_confirmation),
                "Android Back from a skill confirmation must dismiss only the overlay");
        const std::vector<std::string> inventory_details{
            "menu_CharacterMenu", "menu_InventorySheetMain", "menu_InventorySheetDetails"};
        require(gameplay_menu_back_dismisses_overlay_v1(inventory_details) &&
                    !gameplay_menu_back_dismisses_overlay_v1(inventory_details, true),
                "Android Back dismisses Inventory Details, but authored NativeBackToHud returns to HUD");
        require(!gameplay_menu_back_dismisses_overlay_v1(
                    std::vector<std::string>{"menu_CharacterMenu"}),
                "Android Back from the root character menu must return to the HUD");
        require(!gameplay_menu_back_dismisses_overlay_v1({}),
                "empty gameplay menu stack must not be treated as a nested overlay");
        require(!gameplay_menu_reveal_existing_requested_v1("menu_CharacterMenu"),
                "repeated character-menu initialization must preserve its active sheet stack");
        require(gameplay_menu_reveal_existing_requested_v1("menu_confirm2"),
                "only the retained confirmation panel should unwind covering menus");
        require(gameplay_native_push_state_routes_to_level_menu_v1("menu_CharacterMenu", true),
                "NativePushState must route CharacterMenu through GSFlashMenu over GSLevel");
        require(gameplay_menu_state_v1("menu_CharacterMenu") &&
                    gameplay_menu_state_v1("menu_InventorySheetMain") &&
                    gameplay_menu_state_v1("menu_SkillTreeSheetNew") &&
                    gameplay_menu_state_v1("menu_FaerySheet"),
                "portrait, Inventory, Talents and Faery authored screens must remain reachable");
        require(!gameplay_menu_state_v1("menu_Inventory") &&
                    !gameplay_menu_state_v1("menu_Talents"),
                "native renderer must accept the original authored state names, not guessed aliases");
        require(gameplay_native_push_state_routes_to_level_menu_v1("menu_Ingame", true),
                "NativePushState must route Ingame through GSFlashMenu over GSLevel");
        require(gameplay_native_push_state_is_level_menu_target_v1("menu_CharacterMenu") &&
                    gameplay_native_push_state_is_level_menu_target_v1("menu_Ingame") &&
                    !gameplay_native_push_state_is_level_menu_target_v1("menu_Options"),
                "only CharacterMenu and Ingame are special NativePushState level targets");
        require(!gameplay_native_push_state_routes_to_level_menu_v1("menu_CharacterMenu", false) &&
                    !gameplay_native_push_state_routes_to_level_menu_v1("menu_Options", true),
                "the GSLevel menu route is limited to CharacterMenu and Ingame with GSLevel active");

        auto confirmation_stack = std::vector<std::string>{
            "menu_CharacterMenu", "menu_InventorySheetMain", "menu_confirm2",
            "menu_InventorySheetDetails"};
        unsigned resumed_pops = 0;
        require(gameplay_menu_reveal_existing_v1(
                    confirmation_stack, "menu_confirm2", [&] {
                        ++resumed_pops;
                        confirmation_stack.pop_back();
                        return true;
                    }),
                "an authored request for a covered confirmation should resume the retained state");
        require(resumed_pops == 1 && confirmation_stack.size() == 3 &&
                    confirmation_stack.back() == "menu_confirm2",
                "resuming a confirmation must pop covered states through the owner and make it active");

        const auto pop = gameplay_menu_transition_plan_v1(
            "menu_CharacterSheetStats", "menu_CharacterSheetNew", false);
        require(pop.size() == 2 && pop[0].action == GameplayMenuTransitionActionV1::hide &&
                    pop[0].menu == "menu_CharacterSheetStats" &&
                    pop[1].action == GameplayMenuTransitionActionV1::show &&
                    pop[1].menu == "menu_CharacterSheetNew",
                "popping a child must hide it and restore the parent");

        const auto exit = gameplay_menu_transition_plan_v1("menu_CharacterMenu", "", false);
        require(exit.size() == 1 && exit[0].action == GameplayMenuTransitionActionV1::hide &&
                    exit[0].menu == "menu_CharacterMenu",
                "returning to gameplay must hide the last menu");

        std::cout << "PASS gameplay_menu_stack_v1 checks=" << checks << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
