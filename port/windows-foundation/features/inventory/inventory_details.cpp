#include "inventory_details.hpp"
#include <algorithm>
#include <cmath>
namespace dh::foundation::inventory {
namespace {
bool prefix(const std::string& path,const char* base){return path.compare(0,std::char_traits<char>::length(base),base)==0;}
// Details is pushed as its own menu over the still-open InventorySheetMain
// (authored-actions.txt NativePushMenu "menu_InventorySheetDetails"), so the two
// full-stage background plates of the main sheet (shapes 396 at depth 34 and 436 at
// depth 177) remain visible under it. Every other main-sheet layer is still replaced.
bool details_replaces_main(const std::string& path){
 if(path=="menu_InventorySheetMain/34"||path=="menu_InventorySheetMain/177") return false;
 return prefix(path,"menu_InventorySheetMain/");
}
bool contains(const std::vector<HudGeometryVertex>& triangles,float x,float y){
 auto edge=[](const auto& a,const auto& b,float xx,float yy){return (b.x-a.x)*(yy-a.y)-(b.y-a.y)*(xx-a.x);};
 for(std::size_t i=0;i+2<triangles.size();i+=3){const auto&a=triangles[i];const auto&b=triangles[i+1];const auto&c=triangles[i+2];if(std::abs(edge(a,b,c.x,c.y))<1e-6f)continue;const auto aa=edge(a,b,x,y),bb=edge(b,c,x,y),cc=edge(c,a,x,y);if((aa>=0&&bb>=0&&cc>=0)||(aa<=0&&bb<=0&&cc<=0))return true;}return false;
}
const InventoryItem* owned_item(const CharacterState& owner,const std::string& id){const auto found=std::find_if(owner.inventory.begin(),owner.inventory.end(),[&](const auto& item){return item.instance_id==id;});return found==owner.inventory.end()?nullptr:&*found;}
std::size_t focus(const std::vector<equipment_menu::OwnedSelection>& rows,const std::string& selected){const auto at=std::find_if(rows.begin(),rows.end(),[&](const auto& row){return row.instance_id==selected;});return at==rows.end()?0:std::size_t(at-rows.begin());}
bool name(const DetailBindings& bindings,const CharacterState& owner,const dh2::data::ItemTable& table,const std::string& id,std::string& value,std::string& error){
 const auto* owned=owned_item(owner,id);if(!owned){error="Detail item selection no longer owned";return false;}const auto* record=dh2::data::item(table,dh2::data::item_id(table,owned->definition_id));if(!record||!bindings.item_name){error="Required original item name provider unavailable";return false;}return bindings.item_name(*owned,*record,value,error);
}
}
bool DetailsPresenter::candidates(std::vector<equipment_menu::OwnedSelection>& rows,std::string& error)const{return selection_.view_for_selected_slot(rows,error);}
bool DetailsPresenter::open(unsigned slot,std::string& error){if(!selection_.select_slot(slot,error))return false;std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;if(!rows.empty()&&std::none_of(rows.begin(),rows.end(),[&](const auto& row){return row.instance_id==selection_.selected_instance();}))if(!selection_.select_instance(rows.front().instance_id,error))return false;open_=true;error.clear();return true;}
bool DetailsPresenter::frame(const DetailBindings& b,character_menu::Frame& output,std::string& error)const{
 if(!open_){error="Original inventory details panel is closed";return false;}if(!b.symbol){error="Required detail StringManager symbol provider unavailable";return false;}
 std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;
 const auto current=focus(rows,selection_.selected_instance());const auto& art=original_inventory_details();auto next=output;
 std::string equipped_id,equipped_name;for(const auto& row:rows)if(row.equipped){equipped_id=row.instance_id;break;}
 next.art.batches.erase(std::remove_if(next.art.batches.begin(),next.art.batches.end(),[](const auto& batch){return details_replaces_main(batch.role)||prefix(batch.role,"menu_InventorySheetDetails/");}),next.art.batches.end());
 next.text.erase(std::remove_if(next.text.begin(),next.text.end(),[](const auto& value){return details_replaces_main(value.field.path)||prefix(value.field.path,"menu_InventorySheetDetails/");}),next.text.end());
 next.solids.erase(std::remove_if(next.solids.begin(),next.solids.end(),[](const auto& value){return details_replaces_main(value.geometry.role)||prefix(value.geometry.role,"menu_InventorySheetDetails/");}),next.solids.end());
  const auto* transmute_variant=rows.empty()?nullptr:&(rows[current].equipped?art.text_states.transmute_disabled:art.text_states.transmute_idle);
  const bool has_transmute_variant=transmute_variant&&!transmute_variant->fields.empty();
  if(has_transmute_variant){
   bool inserted=false;
   for(const auto& batch:art.panel.batches){
    if(prefix(batch.role,"menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/")){
     if(!inserted){next.art.batches.insert(next.art.batches.end(),transmute_variant->batches.begin(),transmute_variant->batches.end());inserted=true;}
     continue;
    }
    if(equipped_id.empty()&&prefix(batch.role,"menu_InventorySheetDetails/EquipedSwordIcon/"))continue;
    next.art.batches.push_back(batch);
   }
   if(!inserted)next.art.batches.insert(next.art.batches.end(),transmute_variant->batches.begin(),transmute_variant->batches.end());
   for(const auto& solid:art.panel.solids)if(!prefix(solid.geometry.role,"menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/"))next.solids.push_back(solid);
   next.solids.insert(next.solids.end(),transmute_variant->solids.begin(),transmute_variant->solids.end());
  }else{
   for(const auto& batch:art.panel.batches){if(equipped_id.empty()&&prefix(batch.role,"menu_InventorySheetDetails/EquipedSwordIcon/"))continue;next.art.batches.push_back(batch);}
   next.solids.insert(next.solids.end(),art.panel.solids.begin(),art.panel.solids.end());
  }
 std::string selected_name;if(!rows.empty()&&!name(b,owner_,table_,rows[current].instance_id,selected_name,error))return false;
 if(!equipped_id.empty()&&!name(b,owner_,table_,equipped_id,equipped_name,error))return false;
 for(const auto& field:art.panel.text_fields){std::string value;const auto& path=field.path;
  if(has_transmute_variant&&prefix(path,"menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/"))continue;
  if(prefix(path,"menu_InventorySheetDetails/SelectedItemName/"))value=selected_name;
  else if(prefix(path,"menu_InventorySheetDetails/EquipedItemName/"))value=equipped_name;
  else if(prefix(path,"menu_InventorySheetDetails/category_title/")){if(!b.symbol("GAMEPLAYMENUS_CATEGORY_"+std::to_string(selection_.selected_slot()),value,error))return false;}
  else if(path.find("/player_gold/")!=std::string::npos)value=std::to_string(owner_.gold);
  else {const char* symbol=nullptr;
   if(path.find("/TotalGoldText/")!=std::string::npos)symbol="MENU_TOTAL_GOLD";
   else if(path.find("/btn_EquipItem/")!=std::string::npos)symbol="GLOBAL_EQUIP";
   else if(path.find("/btn_Unequip/")!=std::string::npos)symbol="GLOBAL_UNEQUIP";
   else if(path.find("/btn_Drop/")!=std::string::npos)symbol="GAMEPLAYMENUS_INVENTORY_DROP";
   else if(path.find("/btn_AutoEquip/")!=std::string::npos)symbol="GAMEPLAYMENUS_AUTOEQUIP";
   else if(path.find("/btn_GAMEPLAYMENUS_TRANSMUTE2/")!=std::string::npos)symbol="GAMEPLAYMENUS_TRANSMUTE2";
   if(symbol){if(!b.symbol(symbol,value,error))return false;}
   else if(b.item_details){const auto* owned=(path.find("/EquipedItem")!=std::string::npos)?owned_item(owner_,equipped_id):(!rows.empty()?owned_item(owner_,rows[current].instance_id):nullptr);
    if(owned&&(path.find("/ItemInfo")!=std::string::npos||path.find("/ItemReq")!=std::string::npos||path.find("/EquipedItemInfo")!=std::string::npos||path.find("/EquipedItemReq")!=std::string::npos))if(!b.item_details(path,*owned,value,error))return false;}
  }
  if(!value.empty())next.text.push_back({field,std::move(value)});
 }
 if(has_transmute_variant){
  const auto& selected=rows[current];
   const auto& fields=transmute_variant->fields;
  const auto* owned=owned_item(owner_,selected.instance_id);
  if(!owned){error="Detail item selection no longer owned";return false;}
  for(const auto& field:fields){std::string value;const auto& path=field.path;const char* symbol=nullptr;
   if(path=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ButtonName/text")symbol="GAMEPLAYMENUS_TRANSMUTE2";
   else if(path=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ValueText/value")symbol="MENU_VALUE_TITLE";
   else if(path=="menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ValueBox/value"){
    if(b.transmute_value&&!b.transmute_value(*owned,value,error))return false;
   }
   if(symbol&&!b.symbol(symbol,value,error))return false;
   if(!value.empty())next.text.push_back({field,std::move(value)});
  }
 }
 for(const auto& row_art:art.rows){const auto index=static_cast<std::int64_t>(current)+row_art.relative_index;if(index<0||std::size_t(index)>=rows.size())continue;const auto& row=rows[std::size_t(index)];const auto* owned=owned_item(owner_,row.instance_id);if(!owned){error="Original list item disappeared";return false;}std::string title;if(!name(b,owner_,table_,row.instance_id,title,error))return false;const auto& item_art=row_art.relative_index==0?row_art.selected:row_art.unselected;
  next.art.batches.insert(next.art.batches.end(),item_art.batches.begin(),item_art.batches.end());
  next.solids.insert(next.solids.end(),item_art.solids.begin(),item_art.solids.end());
  for(const auto& field:item_art.text_fields){if(field.path.find("/Host")!=std::string::npos)next.text.push_back({field,title});else if(field.path.find("/Number")!=std::string::npos)next.text.push_back({field,std::to_string(owned->quantity)});}
 }
 output=std::move(next);error.clear();return true;
}
bool DetailsPresenter::release(float x,float y,DetailAction& action,std::string& error){
 action=DetailAction::none;if(!open_||!std::isfinite(x)||!std::isfinite(y)){error.clear();return true;}
 std::vector<equipment_menu::OwnedSelection> rows;if(!candidates(rows,error))return false;const auto current=focus(rows,selection_.selected_instance());const auto& art=original_inventory_details();
 for(const auto& hit:art.actions)if(contains(hit.triangles,x,y)){action=hit.action;if(action==DetailAction::previous||action==DetailAction::next){if(rows.empty()){action=DetailAction::none;return true;}auto index=current;if(action==DetailAction::previous&&index)--index;else if(action==DetailAction::next&&index+1<rows.size())++index;if(!selection_.select_instance(rows[index].instance_id,error))return false;}error.clear();return true;}
 for(const auto& row:art.rows)if(contains(row.hit,x,y)){const auto index=static_cast<std::int64_t>(current)+row.relative_index;if(index>=0&&std::size_t(index)<rows.size()){if(!selection_.select_instance(rows[std::size_t(index)].instance_id,error))return false;action=DetailAction::select;}error.clear();return true;}
 error.clear();return true;
}
}
