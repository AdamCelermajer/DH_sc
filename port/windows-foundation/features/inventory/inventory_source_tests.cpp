#include "source_item_descriptors.hpp"
#include "inventory_details.hpp"
#include "../character_menu/menu_text.hpp"
#include "../../asset_catalog.hpp"
#include "../../../engine-ui/item_text_owner_v5.hpp"
#include "../../../engine-ui/character_menu_potions_v4.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool yes,const std::string& error){if(!yes)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
 check(argc==2,"Supply actual original assets root");AssetCatalog assets(argv[1]);std::string error;
 OriginalPropertyDatabase database;check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
 auto r=assets.read("original-cache/data/pydata/loot_table_pyarray.bin"),n=assets.read("original-cache/data/pydata/loot_table_pyarraynames.bin"),s=assets.read("original-cache/data/pydata/loot_table_pystructnames.bin");dh2::data::ItemTable table;check(dh2::data::load_items({r.data(),r.size()},{n.data(),n.size()},{s.data(),s.size()},table,error),error);
 auto owner=make_default_character();owner.inventory={{"sword","Longsword01",1},{"sword2","Longsword01",1},{"suit","StartingSuit",1}};
 character_menu::MenuLocalization localization;check(localization.load(assets,"original-cache/data",0,error),error);check(localization.bind_profile(&owner,error),error);
 dh2::ui::HudTextV1* text=nullptr;dh2::ui::HudTextEnvironmentV1 environment;check(localization.borrow_text(text,environment,error),error);
 dh2::ui::ItemTextOwnerV5 text_owner(table,database.characters,*text,environment);const auto services=text_owner.services();
 inventory::SourceDescriptors descriptors;check(inventory::source_bare_item_descriptors(owner.inventory[0],table,services,descriptors,error),error);
 check(!descriptors.name.empty()&&!descriptors.stats.empty(),"Actual source longsword name/stats text empty");
 const auto sword=descriptors;check(inventory::source_bare_item_descriptors(owner.inventory[2],table,services,descriptors,error),error);check(!descriptors.name.empty()&&!descriptors.stats.empty(),"Actual source suit text empty");check(sword.stats!=descriptors.stats,"Source weapon and armor formats collapsed");
 // Component source-owner fixture built from actual original Power tables and
 // shared original text services; exercises lease identity, never save mirroring.
 auto pr=assets.read("original-cache/data/pydata/item_powers_pyarray.bin"),pn=assets.read("original-cache/data/pydata/item_powers_pyarraynames.bin"),ps=assets.read("original-cache/data/pydata/item_powers_pystructnames.bin");dh2::data::ItemPowerTablesV5 power_tables;check(power_tables.load({pr.data(),pr.size()},{pn.data(),pn.size()},{ps.data(),ps.size()},error),error);
 struct ActualOwner{dh2::data::ItemInstanceV1 item;dh2::data::ItemPresentationOwnerV5 powers;explicit ActualOwner(dh2::data::ItemPowerTablesV5::Borrow tables):powers(std::move(tables)){}};
 auto native=std::make_shared<ActualOwner>(power_tables.borrow());native->item.id=dh2::data::item_id(table,"Longsword01");native->item.quantity=1;native->item.name=sword.name;native->item.description=sword.stats;native->item.requirements=sword.requirements;
 const auto& power_rows=power_tables.borrow().rows();auto power=std::find_if(power_rows.begin(),power_rows.end(),[](const auto& value){return value.scalars.description>0;});check(power!=power_rows.end(),"No actual original Power description");check(native->powers.add_power(native->item,std::int32_t(power-power_rows.begin()),0,services,error),error);
 bool reject_ownership=false;inventory::SourceInstanceDescriptorProvider canonical(table,[&](const auto& id,auto& lease,auto& e){if(id!="sword"){e="Actual source lease identity missing";return false;}lease.owner=native;lease.item=&native->item;lease.powers=&native->powers;lease.text_owner=&text_owner;lease.descriptors_current=true;lease.owns=[&](const auto* item){return !reject_ownership&&item==&native->item;};return true;});
 inventory::SourceOwnedDescriptors powered;check(canonical.present(owner.inventory[0],powered,error),error);check(powered.powers.size()==1&&!powered.powers[0].empty()&&powered.base.name==sword.name,"Actual source power descriptors lost");const auto unchanged_power=powered.powers[0];reject_ownership=true;check(!canonical.present(owner.inventory[0],powered,error)&&powered.powers[0]==unchanged_power,"Invalid canonical ownership lease committed output");reject_ownership=false;std::string power_text;check(canonical.field(owner.inventory[0],"menu_InventorySheetDetails/ItemInfo2/text",power_text,error)&&power_text==powered.powers[0],"Actual source powered detail field mismatch");
 auto invalid=owner.inventory[0];invalid.quantity=32768;const auto unchanged=descriptors.name;check(!inventory::source_bare_item_descriptors(invalid,table,services,descriptors,error)&&descriptors.name==unchanged,"Source signed16 boundary or atomic descriptor failed");
 std::string potion_format,potion;bool source_null=false;check(dh2::ui::character_menu_potion_localized_format_v4(*text,environment,potion_format,source_null,error),error);check(!source_null,"Original potion format is null");check(dh2::ui::character_menu_potion_integer_text_v4(*text,environment,potion_format.c_str(),3,potion,error),error);check(potion.find('3')!=std::string::npos,"Source potion formatter lost actual count");
 OriginalActorProperties properties;check(resolve_original_fresh_player(database,"KnightPlayerBase",properties,error),error);ActorState actor;
 EquipmentAdapter adapter(owner,actor,properties,table,database);equipment_menu::Presenter equipment(owner,table,properties.sheets,adapter);
 inventory::DetailsPresenter details(owner,table,equipment);check(details.open(1,error),error);check(equipment.selected_instance()=="sword","Details did not select actual first candidate");
 inventory::DetailBindings bindings;bindings.symbol=[&](const auto& key,auto& value,auto& e){return localization.symbol(key,&owner,value,e);};bindings.item_name=[&](const auto& item,const auto&,auto& value,auto& e){inventory::SourceDescriptors d;if(!inventory::source_bare_item_descriptors(item,table,services,d,e))return false;value=std::move(d.name);return true;};
 bindings.item_details=[&](const auto& field,const auto& item,auto& value,auto& e){inventory::SourceDescriptors d;if(!inventory::source_bare_item_descriptors(item,table,services,d,e))return false;if(field.find("ItemInfo1")!=std::string::npos)value=d.stats;else if(field.find("ItemReq")!=std::string::npos)value=d.requirements;else value.clear();return true;};
 bindings.transmute_value=[](const auto&,auto& value,auto&){value="12";return true;};
 character_menu::Frame frame;frame.art.batches=character_menu::original_menu_art(character_menu::Tab::equipment).batches;
 frame.art.batches.push_back({"menu_InventorySheetMain/stale",0,{}});frame.text.push_back({{"menu_InventorySheetDetails/stale"},"stale"});
 frame.solids.push_back({{"menu_InventorySheetMain/stale",0,{}},{1,1,1,1},""});
 check(details.frame(bindings,frame,error),error);check(!frame.art.batches.empty()&&!frame.text.empty(),"Original details generated no actual UI");
 check(std::none_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& value){return (value.role.find("menu_InventorySheetMain/")==0&&value.role!="menu_InventorySheetMain/34"&&value.role!="menu_InventorySheetMain/177")||value.role.find("menu_InventorySheetDetails/stale")==0;}),"Original details did not clear stale main/details bitmap art");
 check(std::none_of(frame.text.begin(),frame.text.end(),[](const auto& value){return value.field.path.find("menu_InventorySheetDetails/stale")==0;}),"Original details did not clear stale source fields");
 check(std::any_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& value){return value.role=="menu_InventorySheetMain/34";})&&
       std::any_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& value){return value.role=="menu_InventorySheetMain/177";}),
       "Original full-stage InventorySheetMain background plates were removed under the Details overlay");
 check(frame.solids.size()==3&&frame.solids[0].geometry.role=="menu_InventorySheetDetails/avatarpane/1"&&
       frame.solids[1].geometry.role=="menu_InventorySheetDetails/list/btn_0/8"&&
       frame.solids[2].geometry.role=="menu_InventorySheetDetails/list/btn_post0/8",
       "Original detail panel/list source solid order or mask roles changed");
 for(const auto& solid:frame.solids)check(std::any_of(frame.art.batches.begin(),frame.art.batches.end(),[&](const auto& batch){return batch.role==solid.after_bitmap_role;}),"Original detail solid lost its source display-list bitmap anchor");
 bool selected=false,stats=false;for(const auto& field:frame.text){if(field.field.path=="menu_InventorySheetDetails/SelectedItemName/text")selected=field.value==sword.name;if(field.field.path=="menu_InventorySheetDetails/ItemInfo1/text")stats=field.value==sword.stats;}check(selected&&stats,"Original source detail fields not bound to owned item");
  const auto& detail_art=inventory::original_inventory_details();const auto& idle=detail_art.text_states.transmute_idle;const auto& disabled=detail_art.text_states.transmute_disabled;
 check(idle.fields.size()==3&&disabled.fields.size()==3&&!idle.batches.empty()&&disabled.batches.empty()&&disabled.solids.empty(),"Original sprite449 idle/disabled source display-list variants changed");
 auto field_at=[](const auto& fields,const std::string& path)->const character_menu::MenuTextField*{const auto at=std::find_if(fields.begin(),fields.end(),[&](const auto& f){return f.path==path;});return at==fields.end()?nullptr:&*at;};
 const auto button_path=std::string("menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ButtonName/text");
 const auto value_path=std::string("menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ValueText/value");
 const auto amount_path=std::string("menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/ValueBox/value");
 const auto* idle_button=field_at(idle.fields,button_path);const auto* idle_value=field_at(idle.fields,value_path);const auto* idle_amount=field_at(idle.fields,amount_path);
 const auto* disabled_button=field_at(disabled.fields,button_path);const auto* disabled_value=field_at(disabled.fields,value_path);const auto* disabled_amount=field_at(disabled.fields,amount_path);
 check(idle_button&&idle_value&&idle_amount&&disabled_button&&disabled_value&&disabled_amount,"Original transmute button did not retain its three separate source fields");
 check(idle_button->font_id==103&&idle_button->source_height==12.0f&&std::abs(idle_button->matrix[5]-299.35f)<0.01f&&std::abs(disabled_button->matrix[5]-325.35f)<0.01f&&std::abs(idle_value->matrix[5]-269.2f)<0.01f&&std::abs(disabled_value->matrix[5]-331.4f)<0.01f&&std::abs(idle_amount->matrix[5]-280.3f)<0.01f&&std::abs(disabled_amount->matrix[5]-333.5f)<0.01f,"Original sprite449 frame0/frame23 font or field placements changed");
 auto field_value=[&](const auto& path){const auto at=std::find_if(frame.text.begin(),frame.text.end(),[&](const auto& f){return f.field.path==path;});return at==frame.text.end()?std::string{}:at->value;};
 std::string auto_equip_label;check(localization.symbol("GAMEPLAYMENUS_AUTOEQUIP",&owner,auto_equip_label,error),error);check(!auto_equip_label.empty()&&field_value("menu_InventorySheetDetails/btn_AutoEquip/text")==auto_equip_label,"Open source Details panel omitted its localized Auto-equip label");
 std::string source_transmute,source_value;check(bindings.symbol("GAMEPLAYMENUS_TRANSMUTE2",source_transmute,error),error);check(bindings.symbol("MENU_VALUE_TITLE",source_value,error),error);
 check(field_value(button_path)==source_transmute&&field_value(value_path)==source_value&&field_value(amount_path)=="12","Transmute source button/value/amount fields were duplicated or mislabeled");
 for(const auto& f:frame.text)check(!(f.field.path==button_path&&f.value==source_value)&&!(f.field.path==value_path&&f.value==source_transmute)&&!(f.field.path==amount_path&&f.value==source_transmute),"Three Transmute strings were projected into one source role");
 const auto transmute_provider=bindings.transmute_value;bindings.transmute_value={};check(details.frame(bindings,frame,error),error);check(field_value(amount_path).empty(),"Missing ItemTransmuteValueString provider fabricated a value");bindings.transmute_value=transmute_provider;check(details.frame(bindings,frame,error),error);
 const auto row_host=std::find_if(frame.text.begin(),frame.text.end(),[](const auto& f){return f.field.path=="menu_InventorySheetDetails/list/btn_0/Host";});
 const auto row_number=std::find_if(frame.text.begin(),frame.text.end(),[](const auto& f){return f.field.path=="menu_InventorySheetDetails/list/btn_0/Number";});
 check(row_host!=frame.text.end()&&row_number!=frame.text.end()&&row_host->value==sword.name&&row_number->value=="1"&&row_host->field.font_id==103&&row_number->field.font_id==103&&row_host->field.source_height==20.0f&&row_number->field.source_height==22.0f&&std::abs(row_host->field.matrix[4]-55.21977f)<0.02f&&std::abs(row_number->field.matrix[4]-36.97684f)<0.02f&&std::abs(row_host->field.matrix[0]-0.5709838867f)<0.00001f&&std::abs(row_host->field.matrix[3]-0.5678462074f)<0.00001f&&std::abs(row_number->field.matrix[0]-0.5709838867f)<0.00001f&&std::abs(row_number->field.matrix[3]-0.5678462074f)<0.00001f,"Original list title/count source field font or placement changed");
 auto source_quad=[](const auto& field,float x,float y,float width,float height){const auto&m=field.matrix;return std::array<std::array<float,2>,4>{{{{m[0]*x+m[2]*y+m[4],m[1]*x+m[3]*y+m[5]}},{{m[0]*(x+width)+m[2]*y+m[4],m[1]*(x+width)+m[3]*y+m[5]}},{{m[0]*(x+width)+m[2]*(y+height)+m[4],m[1]*(x+width)+m[3]*(y+height)+m[5]}},{{m[0]*x+m[2]*(y+height)+m[4],m[1]*x+m[3]*(y+height)+m[5]}}}};};
 const auto& source_row_host=(*row_host).field;const auto expected_quad=source_quad(source_row_host,0,0,20,20);
 check(std::abs((expected_quad[1][0]-expected_quad[0][0])-11.419677734f)<0.001f&&std::abs((expected_quad[3][1]-expected_quad[0][1])-11.356924134f)<0.001f,"Original row glyph quad must apply the full source text matrix before viewport scaling");
 check(std::count_if(frame.text.begin(),frame.text.end(),[](const auto& f){return f.field.path=="menu_InventorySheetDetails/list/btn_0/Host";})==1,"Selected list title was projected more than once");
 check(std::none_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& b){return b.role=="menu_InventorySheetDetails/EquipedSwordIcon/1";}),"Original equipped-only sword marker leaked into an unequipped item detail");
 const auto torso=std::find_if(inventory::original_inventory_slots().begin(),inventory::original_inventory_slots().end(),[](const auto& slot){return slot.source_slot==0;});
 check(torso!=inventory::original_inventory_slots().end()&&!torso->icons.empty()&&torso->icons.front().shape_id==415,"Garb category icon no longer uses source sprite424 torso frame0");
 owner.equipment.push_back({"slot1","sword",0,1});check(details.frame(bindings,frame,error),error);
 check(field_value(button_path)==source_transmute&&field_value(value_path)==source_value&&field_value(amount_path)=="12","Disabled transmute frame lost its independent source text roles");
 const auto disabled_draw=std::find_if(frame.text.begin(),frame.text.end(),[&](const auto& f){return f.field.path==button_path;});
 check(disabled_draw!=frame.text.end()&&std::abs(disabled_draw->field.matrix[5]-325.35f)<0.01f&&std::none_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& b){return b.role.find("menu_InventorySheetDetails/btn_GAMEPLAYMENUS_TRANSMUTE2/")==0;}),"Equipped selection did not switch to sprite449 disabled display-list state");
 check(std::any_of(frame.art.batches.begin(),frame.art.batches.end(),[](const auto& b){return b.role=="menu_InventorySheetDetails/EquipedSwordIcon/1";}),"Original equipped marker did not follow source item visibility state");
 owner.equipment.clear();
 const auto& art=inventory::original_inventory_details();const auto next=std::find_if(art.actions.begin(),art.actions.end(),[](const auto& a){return a.action==inventory::DetailAction::next;});check(next!=art.actions.end()&&next->triangles.size()>=3,"Original arrow hit absent");auto&a=next->triangles[0];auto&b=next->triangles[1];auto&c=next->triangles[2];inventory::DetailAction action;check(details.release((a.x+b.x+c.x)/3,(a.y+b.y+c.y)/3,action,error),error);check(action==inventory::DetailAction::next&&equipment.selected_instance()=="sword2","Source arrow selection did not select second actual instance");
 for(const auto expected:{inventory::DetailAction::equip,inventory::DetailAction::unequip,inventory::DetailAction::drop,inventory::DetailAction::transmute,inventory::DetailAction::auto_equip}){
  const auto hit=std::find_if(art.actions.begin(),art.actions.end(),[&](const auto& value){return value.action==expected;});check(hit!=art.actions.end()&&hit->triangles.size()>=3,"Original mutation action hit contour absent");const auto& x=hit->triangles[0];const auto& y=hit->triangles[1];const auto& z=hit->triangles[2];
  check(details.release((x.x+y.x+z.x)/3,(x.y+y.y+z.y)/3,action,error),error);check(action==expected,"Original detail action did not expose its source owner command");
 }
 check(owner.inventory.size()==3&&owner.equipment.empty(),"Menu draw/navigation/action routing mutated persistent inventory/equipment");
 std::cout<<"original inventory source tests PASS name="<<sword.name<<" stats="<<sword.stats<<" potions="<<potion<<"\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
