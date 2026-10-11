#include "../inventory_click_binding_v1.hpp"
#include <cstdlib>
#include <vector>

int main(){
 int canonical{},other{};
 if(!dh2::ui::inventory_click_binding_v1(&canonical,&canonical,0x1234,0x1234))return 1;
 if(dh2::ui::inventory_click_binding_v1(&canonical,&other,0x1234,0x1234))return 2;
 if(dh2::ui::inventory_click_binding_v1(&canonical,&canonical,0x5678,0x1234))return 3;
 if(dh2::ui::inventory_click_binding_v1(nullptr,nullptr,0x1234,0x1234))return 4;
 std::uint32_t owner_index=99;
 if(!dh2::ui::inventory_click_item_index_v1(2,4,owner_index)||owner_index!=2)return 5;
 if(dh2::ui::inventory_click_item_index_v1(-1,4,owner_index)||owner_index!=2)return 6;
 if(dh2::ui::inventory_click_item_index_v1(4,4,owner_index)||owner_index!=2)return 7;
 if(!dh2::ui::inventory_click_item_equippable_v1(true,false))return 8;
 if(!dh2::ui::inventory_click_item_equippable_v1(true,true))return 9;
 if(dh2::ui::inventory_click_item_equippable_v1(false,false))return 10;
 if(!dh2::ui::inventory_click_item_equippable_v1(false,true))return 11;
 const auto equip=dh2::ui::inventory_click_equip_call_v1(6,47,0);
 if(equip.player_index!=0||equip.item_index!=47||equip.equipment_slot!=6)return 12;
 const auto equip_other_slot=dh2::ui::inventory_click_equip_call_v1(2,3,1);
 if(equip_other_slot.player_index!=1||equip_other_slot.item_index!=3||
    equip_other_slot.equipment_slot!=2)return 13;
 std::vector<std::uint32_t> operations;
 if(!dh2::ui::inventory_auto_equip_slot_calls_v1(3,9,
      [&](std::uint32_t slot){operations.push_back(100+slot);return true;},
      [&](std::uint32_t slot){operations.push_back(200+slot);return true;},
      [](std::uint32_t){return false;}))return 14;
 if(operations!=std::vector<std::uint32_t>{103,203})return 15;
 operations.clear();
 if(!dh2::ui::inventory_auto_equip_slot_calls_v1(-2,9,
      [&](std::uint32_t slot){operations.push_back(100+slot);return true;},
      [&](std::uint32_t slot){operations.push_back(200+slot);return true;},
      [](std::uint32_t){return false;}))return 16;
 if(!operations.empty())return 17;
 if(!dh2::ui::inventory_auto_equip_slot_calls_v1(-1,9,
      [&](std::uint32_t slot){operations.push_back(100+slot);return true;},
      [&](std::uint32_t slot){operations.push_back(200+slot);return true;},
      [](std::uint32_t slot){return slot==4;}))return 18;
 const std::vector<std::uint32_t> expected{
     100,101,102,103,104,105,106,107,108,
     208,207,206,205,204,203,202,201,200,
     208,207,206,205,203,200};
 if(operations!=expected)return 19;
 operations.clear();
 if(dh2::ui::inventory_auto_equip_slot_calls_v1(4,9,
      [&](std::uint32_t){operations.push_back(1);return false;},
      [&](std::uint32_t){operations.push_back(2);return true;},
      [](std::uint32_t){return false;}))return 20;
 if(operations!=std::vector<std::uint32_t>{1})return 21;
 return 0;
}
