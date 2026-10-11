#pragma once
// B110: original menu_Merchant (dqcharmenu_droid.swf sprite 251) chrome not present in the shared Details panel.
#include "../character_menu/character_menu.hpp"
#include "../inventory/inventory_details.hpp"
#include <array>
namespace dh::foundation::merchant {
struct MerchantArt {
    character_menu::MenuArt title,tab_buy_idle,tab_buy_on,tab_sell_idle,tab_sell_on,action_idle,action_disabled,potions,back,error;
    // Hit contours as x,y pairs (two triangles): btn_Buy, btn_Sell, btn_Action, btn_BuyPotions, btn_Back.
    std::array<std::vector<float>,5> hits;
    character_menu::MenuArt confirm,category,potions_disabled;
    std::array<std::vector<float>,2> confirm_hits;
    std::vector<inventory::SourceCharacterPaneV1> panes;
    std::vector<float> error_ok_hit;
};
const MerchantArt& merchant_art();
}
