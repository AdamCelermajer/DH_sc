#include "item_category_presentation_v1.hpp"
#include <array>
namespace dh2::ui {
bool item_category_presentation_v1(const data::ItemInstanceV1& item,
 const data::ItemRecord164& record,const data::PropertySheet& properties,
 const ItemCategoryPresentationServicesV1& services,
 std::vector<ItemCategoryPresentationEntryV1>& output,std::string& error){
 error.clear();
 if(!services.constant){error="Actual EquipmentSlots constants required for item categories";return false;}
 // Original inventory SideList/SelectedItemType has ten categories. Category9
 // is the source Valuables list, whose authored generic icon frame is Potions.
 constexpr const char* names[]{"Torso","RightHand","LeftHand","Feet","HandArmor","RightHandRingFinger","LeftHandRingFinger","Waist","Head","Valuables"};
 std::array<std::int32_t,10> ids{};
 for(unsigned i=0;i<ids.size();++i){
  if(!services.constant(services.context,"EquipmentSlots",names[i],ids[i],error))return false;
  if(ids[i]!=std::int32_t(i)){error="EquipmentSlots constants differ from retained ten-category authored inventory";return false;}
 }
 std::vector<ItemCategoryPresentationEntryV1> next;
 for(unsigned i=0;i<ids.size();++i){
  const bool member=i==9?record.words[26]==-1:character_menu_slot_candidate_v1(item,record,properties,i);
  if(member)next.push_back({ids[i],i==9?"Potions":names[i],"GAMEPLAYMENUS_CATEGORY_"+std::to_string(ids[i])});
 }
 output=std::move(next);return true;
}
}
