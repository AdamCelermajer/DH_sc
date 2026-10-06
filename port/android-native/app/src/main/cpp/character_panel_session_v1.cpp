#include "character_panel_session_v1.hpp"
#include "character_menu_queries_owner_v1.hpp"
#include "character_menu_inventory_order_v1.hpp"
#include "item_category_presentation_v1.hpp"
#include "model_renderer.hpp"
#include "player_initial_grants_v2.hpp"
#include "gameplay_icons.hpp"
#include <array>
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
namespace dh2::android_ui {namespace {
using Value=ui::CharacterMenuValueV1;
std::string quote(const std::string& value){
 std::ostringstream out;out<<'"';
 for(unsigned char c:value){if(c=='"')out<<"\\\"";else if(c=='\\')out<<"\\\\";
  else if(c<32)out<<"\\u00"<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(c)<<std::dec;else out<<char(c);}
 out<<'"';return out.str();
}
std::string json(const Value& v){if(v.kind==3||v.kind==4)return quote(v.text);if(v.kind==1)return v.boolean?"true":"false";
 if(v.kind==2&&std::isfinite(v.number)){std::ostringstream out;out<<std::setprecision(17)<<v.number;return out.str();}return "null";}
using Row=std::map<std::string,Value>;
std::string json(const Row& row){std::string out="{";for(const auto& entry:row){if(out.size()>1)out+=',';out+=quote(entry.first)+':'+json(entry.second);}return out+'}';}
struct Context {
 const model_renderer::PlayerGameplayBinding& player;
 explicit Context(const model_renderer::PlayerGameplayBinding& p):player(p){}
 ui::CharacterMenuActionsGraphV1 actions(){
  ui::CharacterMenuActionsGraphV1 g{};g.owner=player.world_owner;g.equipment=player.gear;g.skills=player.skills;g.save=player.save;
  g.skill_tables=player.skill_tables;g.skill_list_index=player.skill_list_index;
  g.save_binding=[this](auto& save,const auto& skills,auto& error){
   const bool same=skills.identity()==player.skills&&player.skills&&player.skills->native_savegame()==&save&&&save==player.save;
   if(!same)error="Character panel lost the live player's Save binding";return same;
  };
  const auto* rows=player.design.class_rows();
  g.stats={player.gear->properties().get(),player.gear->property_view(),player.design.characters(),rows?rows->data():nullptr,rows?std::uint32_t(rows->size()):0,player.actor_index,{},{}};
  g.stats.debug_load=[this](auto& error){
   if(player.debug&&player.debug_files&&dh2_character_debug_load(player.debug,player.debug_files)==1)return true;
   error="Character panel Debug.Load service failed";return false;
  };
  g.stats.debug_query=[this](const char* key,bool& result,auto& error){std::uint32_t value=0;
   if(player.debug&&player.debug_files&&dh2_character_debug_get(&value,player.debug,key,player.debug_files)==1){result=value!=0;return true;}
   error="Character panel Debug.GetSwitch service failed";return false;
  };
  g.potion_capacity_store=[this](std::uint8_t value,auto& error){
   if(!player.potion_capacity_store){error="Live inventory capacity store unavailable";return false;}
   return player.potion_capacity_store(player.potion_capacity_context,value,error);
  };
  return g;
 }
};
// This sink serializes source query outputs for native Android widgets. It is
// not an emulated AS object: the original AS transport remains a separate module.
ui::CharacterMenuCallV1 query(std::vector<Value> args,Row& row){
 ui::CharacterMenuCallV1 c;c.arguments=std::move(args);
 c.member=[&row](std::uintptr_t object,const char* key,const Value& value,auto& error){
  if(object!=1||!key){error="Character panel output receiver invalid";return false;}row[key]=value;return true;
 };return c;
}
bool coherent(const model_renderer::PlayerGameplayBinding& p){return p.active&&p.world_owner&&p.gear&&p.gear->ready()&&p.skills&&p.save&&p.skill_tables&&p.skill_list_index&&p.temporary&&p.save==p.skills->native_savegame()&&p.gear->properties()==p.skills->session().properties()&&p.save->character()==p.character;}
}
bool CharacterPanelSessionV1::prepare(std::string& error){
 if(ready_)return true;
 std::array<std::vector<std::uint8_t>,3> data;
 const char* names[]{"data/pydata/common_text_pyarray.bin","data/pydata/common_text_pyarraynames.bin","data/pydata/common_text_pystructnames.bin"};
 for(unsigned i=0;i<3;++i){bool found=false;if(!assets_.read(names[i],found,data[i],error)||!found){if(error.empty())error="Character panel source text table missing";return false;}}
 if(!text_.load({data[0].data(),data[0].size()},{data[1].data(),data[1].size()},{data[2].data(),data[2].size()},error)||!text_.switch_pack(0,false,error))return false;
  std::array<std::vector<std::uint8_t>,2> fonts;
  const char* files[]{"data/fonts_pyarray.bin","data/fonts_pystructnames.bin"};
  for(unsigned i=0;i<2;++i){
   auto* input=AAssetManager_open(manager_,files[i],AASSET_MODE_STREAMING);
   if(!input){error="Original APK FontPalette resource unavailable";return false;}
   const auto size=AAsset_getLength64(input);
   if(size<=0||size>1024*1024){AAsset_close(input);error="Original FontPalette input size invalid";return false;}
   fonts[i].resize(std::size_t(size));std::size_t at=0;
   while(at<fonts[i].size()){const auto count=AAsset_read(input,fonts[i].data()+at,fonts[i].size()-at);if(count<=0){AAsset_close(input);error="Short original FontPalette input read";return false;}at+=std::size_t(count);}
   AAsset_close(input);
  }
  if(!palette_.load({fonts[0].data(),fonts[0].size()},{fonts[1].data(),fonts[1].size()},error))return false;
  palette_ready_=true;ready_=true;return true;
}
bool initialize_player_skill_slots_v1(const model_renderer::PlayerGameplayBinding& p,std::string& error){
 if(!coherent(p)){error="Initial skill slots require the live player graph";return false;}
 Context context(p);ui::CharacterMenuActionsOwnerV1 actions(context.actions());
 struct Init {const model_renderer::PlayerGameplayBinding& p;ui::CharacterMenuActionsOwnerV1& actions;std::string& error;} init{p,actions,error};
 player::InitialGrantServices16V2 services{&init,[](void* opaque,const player::InitialGrantRequest32V2* request,player::InitialGrantResponse8V2* out)->int {
  auto& c=*static_cast<Init*>(opaque);if(!request||!out||request->owner!=c.p.character)return -1;out->value=0;out->reserved=0;
  using namespace player;
  switch(request->operation){
   case has_skill_slots:out->value=c.p.save->has_skill_slots();return 0;
   case set_skill_slot:{data::SavedSkillUpdateServicesV1 update{&c,[](void* raw,std::uintptr_t character,std::string& e){auto& live=*static_cast<Init*>(raw);if(character!=live.p.character){e="Initial slot update changed character owner";return false;}if(live.p.skills->update()<0){e=live.p.skills->error();return false;}return true;}};
    return c.p.save->set_skill_in_slot(request->arguments[0],std::uint32_t(request->arguments[1]),update,c.error)?0:-1;}
   case swap_equipment:return c.p.gear->swap_inventory_for_initial_slots(c.error)?0:-1;
   case skill_level:out->value=c.p.save->skill_level(std::uint32_t(request->arguments[0]));return 0;
   case increment_skill:{std::int32_t points=0;return c.actions.train_skill(request->arguments[0],points,c.error)?0:-1;}
   default:c.error="Unexpected initial skill slot operation";return -1;
  }
 }};
 if(dh2_player_initial_skill_slots_v2(p.character,&services)!=0){if(error.empty())error="Source initial skill slot service failed";return false;}error.clear();return true;
}
std::string CharacterPanelSessionV1::snapshot(const model_renderer::PlayerGameplayBinding& p){try{
 std::string error;if(!coherent(p)||!prepare(error))return "{\"ready\":false,\"error\":"+quote(error.empty()?"Live character is unavailable":error)+"}";
 Context context(p);ui::CharacterMenuActionsOwnerV1 actions(context.actions());
 ui::CharacterMenuQueriesGraphV1 g;g.owner=p.world_owner;g.actions=&actions;g.characters=p.design.characters();g.text=&text_;g.text_environment=p.text_environment;g.temporary=p.temporary;
 g.temporary_binding=[&p](const auto& authority,const auto& sheet,auto& e){if(authority.identity()==p.skills&&sheet==p.temporary)return true;e="Character panel skill scratch does not belong to the live player";return false;};
 g.player=[&p](auto index,bool remote,auto& actor,auto&){actor=index==0&&!remote?p.character:0;return true;};
 g.difficulty=[&p](auto& value,auto&){value=p.difficulty;return true;};
  g.item_equippable=[&p](const auto& item,bool& value,auto& e){const auto* row=data::item(p.gear->inventory()->table(),item.id);return row&&ui::character_menu_item_equippable_v1(row->record,p.gear->properties()->resolved,p.save->class_id(),*p.design.characters(),false,false,value,e);};
  g.powers=[&](const auto& item,const auto*& output,auto& e){return presentation_.powers(p,item,text_,p.text_environment,output,e);};
 ui::CharacterMenuQueriesOwnerV1 queries(std::move(g));
 Row stats;auto stats_call=query({Value::reference(1),Value::numeric(0)},stats);
 if(!queries.dispatch("NativeGetPlayerStats",stats_call,error))return "{\"ready\":false,\"error\":"+quote(error)+"}";
 const auto& inventory=*p.gear->inventory();const auto set=unsigned(inventory.current_equipment());
 std::string items="[";for(unsigned i=0;i<inventory.items().size();++i){const auto& cell=inventory.items()[i];if(!cell||!cell->item)continue;const auto& item=*cell->item;
  Row row;auto call=query({Value::numeric(i),Value::reference(1),Value::numeric(0)},row);
  if(!queries.dispatch("NativeInvGetItemDetails",call,error))row["Error"]=Value::string(error);
  row["Index"]=Value::numeric(i);row["ItemID"]=Value::numeric(item.id);row["ItemName"]=Value::string(item.name);row["Quantity"]=Value::numeric(item.signed_quantity());
  int slot=-1;for(unsigned s=0;s<inventory.equipment()[set].size();++s)if(inventory.equipment()[set][s]==cell.get()){slot=int(s);break;}
  row["EquippedSlot"]=Value::numeric(slot);
   std::string compatible_slots,categories_json;
   if(const auto* metadata=data::item(inventory.table(),item.id)){
    ui::ItemCategoryPresentationServicesV1 services{&context,[](void* raw,const char* group,const char* key,std::int32_t& value,std::string& e){
     const auto& player=static_cast<Context*>(raw)->player;const auto* design=player.design.design();
     if(design&&design->lookup&&!design->lookup(design->context,0,group,key,&value))return true;
     e=std::string("Original item category constant unavailable: ")+key;return false;
    }};
    std::vector<ui::ItemCategoryPresentationEntryV1> categories;
    if(!ui::item_category_presentation_v1(item,metadata->record,p.gear->properties()->resolved,services,categories,error))return "{\"ready\":false,\"error\":"+quote(error)+"}";
    std::string candidates="[",encoded_categories="[",category_icon;
    for(const auto& category:categories){
     ui::LocalizationResult localized;
     if(!text_.native_string(category.localization_symbol,p.text_environment.localization,localized,error)||!localized.found)return "{\"ready\":false,\"error\":"+quote(error.empty()?"Original item category string unavailable":error)+"}";
     Row descriptor{{"Index",Value::numeric(category.index)},{"Icon",Value::string(category.icon)},{"Name",Value::string(localized.text)}};
     if(encoded_categories.size()>1)encoded_categories+=',';encoded_categories+=json(descriptor);
     if(category.index<9){if(candidates.size()>1)candidates+=',';candidates+=std::to_string(category.index);}
     if(category_icon.empty()||category.index==slot)category_icon=category.icon;
    }
    compatible_slots=candidates+']';categories_json=encoded_categories+']';
    row["ItemCategoryIcon"]=Value::string(category_icon);
   }
   if(slot>=0){std::string name,slot_error;if(equipment_slot_icon_name(p,slot,name,slot_error))row["EquipmentSlotName"]=Value::string(name);}
  if(items.size()>1)items+=',';
  auto encoded=json(row);
   if(!compatible_slots.empty()){encoded.pop_back();encoded+=",\"CompatibleSlots\":"+compatible_slots+",\"ItemCategories\":"+categories_json+'}';}
  items+=encoded;
 }items+=']';
 std::string skills="[";for(unsigned i=0;i<p.save->skills().size();++i){Row row;auto call=query({Value::numeric(i),Value::reference(1),Value::numeric(0)},row);
  if(!queries.dispatch("NativeGetSkillDetails",call,error))row["Error"]=Value::string(error);
  const auto* record=actions.skill_record(int(i),error);
  row["Index"]=Value::numeric(i);row["SkillLevel"]=Value::numeric(p.save->skill_level(i));row["EquippedSlot"]=Value::numeric(p.save->skill_slot(i));
  if(record){row["RequiredLevel"]=Value::numeric(std::int32_t(record->scalar.words[8]));row["SkillIcon"]=Value::string(record->icon);}
  if(skills.size()>1)skills+=',';skills+=json(row);
 }skills+=']';
  std::string equipment_slots="[";
  for(unsigned s=0;s<9;++s){std::string name,slot_error;Row row{{"Index",Value::numeric(s)}};
   if(!equipment_slot_icon_name(p,int(s),name,slot_error))return "{\"ready\":false,\"error\":"+quote(slot_error)+"}";
   row["Name"]=Value::string(name);row["Icon"]=Value::string(name);int item_index=-1;
   const auto* equipped=inventory.equipment()[set][s];if(equipped)for(unsigned i=0;i<inventory.items().size();++i)if(inventory.items()[i].get()==equipped){item_index=int(i);break;}
   row["ItemIndex"]=Value::numeric(item_index);if(equipment_slots.size()>1)equipment_slots+=',';equipment_slots+=json(row);
  }equipment_slots+=']';
  std::string faeries="[";
  if(p.difficulty>=0&&p.difficulty<3){for(unsigned i=0;i<5;++i){const auto& f=p.save->faeries()[p.difficulty][i];Row row{{"Index",Value::numeric(i)},{"State",Value::numeric(f.state)},{"Level",Value::numeric(f.level)},{"Selected",Value::flag(p.save->current_faery(p.difficulty)==int(i))}};
   row["Icon"]=Value::string("Faery");
   FaeryPresentationV1 descriptor;if(presentation_.faery(p,i,text_,p.text_environment,descriptor,error)){row["Name"]=Value::string(descriptor.name);row["Description"]=Value::string(descriptor.description);row["Identifier"]=Value::string(descriptor.identifier);row["Element"]=Value::numeric(descriptor.element);row["ModelID"]=Value::numeric(descriptor.model_id);row["SpellType"]=Value::numeric(descriptor.spell_type);}else row["Error"]=Value::string(error);
   if(faeries.size()>1)faeries+=',';
   faeries+=json(row);
  }}
  faeries+=']';
 return "{\"ready\":true,\"stats\":"+json(stats)+",\"items\":"+items+",\"equipmentSlots\":"+equipment_slots+",\"skills\":"+skills+",\"faeries\":"+faeries+",\"skillPoints\":"+std::to_string(p.gear->properties()->resolved[157]>>8)+",\"equipmentSet\":"+std::to_string(set)+",\"gold\":"+std::to_string(p.gear->properties()->resolved[213]>>8)+"}";
 }catch(const std::exception& failure){return "{\"ready\":false,\"error\":"+quote(failure.what())+"}";}}
std::string CharacterPanelSessionV1::action(const model_renderer::PlayerGameplayBinding& p,int operation,int index,int slot){try{
 if(!coherent(p)||!p.life||p.life->dead)return "Character action unavailable";
 if(operation==0)return model_renderer::player_equipment_action(0,index,-1);
 if(operation==1)return model_renderer::player_equipment_action(2,-1,slot);
 if(operation==5)return model_renderer::player_equipment_action(3,-1,-1);
 Context context(p);ui::CharacterMenuActionsOwnerV1 actions(context.actions());std::string error;
 if(operation==2){if(index<0||index>3)return "Invalid stat";if(p.gear->properties()->resolved[148]<=0)return "No unspent stat points";if(!actions.assign_stat(unsigned(index),error))return "Stat action failed: "+error;return "Stat point assigned";}
 if(operation==3){if(index<0||unsigned(index)>=p.save->skills().size())return "Invalid skill";std::int32_t points=0;if(!actions.train_skill(index,points,error))return "Skill training failed: "+error;return "Skill training completed; points left "+std::to_string(points);}
 if(operation==4){if(index<0||unsigned(index)>=p.save->skills().size()||slot<0||slot>2)return "Invalid skill slot";if(!actions.equip_skill(slot,index,error))return "Skill equipment failed: "+error;return p.save->skill_in_slot(slot)==index?"Skill assigned to slot "+std::to_string(slot+1):"Skill is not learned or assignable";}
 return "Character action unsupported";
  }catch(const std::exception& failure){return std::string("Character action failed: ")+failure.what();}}
bool CharacterPanelSessionV1::dispatch(const model_renderer::PlayerGameplayBinding& p,const char* name,ui::CharacterMenuCallV1& call,std::string& error){try{
 if(!coherent(p)){error="Authored character callback requires the live coherent player graph";return false;}
 if(name&&std::string(name)=="NativeReloadSkills"){
  if(!gameplay_services_.owner||!gameplay_services_.reload){error="Required whole same-player Character::ReloadSkills provider";return false;}
  ui::CharacterMenuReloadActionGraphV1 reload;reload.owner=p.world_owner;
  if(!gameplay_services_.reload(p,reload,error))return false;
  if(!reload.owner||!reload.player||!reload.player_index||!reload.reload.invoke){error="Incomplete source Character::ReloadSkills binding";return false;}
  struct Scoped {
   ui::MenuReloadServices16V1 original;ui::CharacterMenuCallV1& call;std::string detail;
   static int invoke(void* raw,const ui::MenuReloadRequest32V1* q,ui::MenuReloadResponse16V1* out){
    auto& c=*static_cast<Scoped*>(raw);
    if(q&&out&&q->service==ui::reload_spec_prompt_v1&&c.call.invoke_boolean){
     *out={};return c.call.invoke_boolean(q->path,q->callback,q->argument!=0,c.detail)?0:-1;
    }
    return c.original.invoke(c.original.context,q,out);
   }
  } scoped{reload.reload,call,{}};
  const auto prior_detail=reload.failure_detail;
  reload.reload={&scoped,Scoped::invoke};
  reload.failure_detail=[&scoped,prior_detail]{return scoped.detail.empty()?(prior_detail?prior_detail():std::string{}):scoped.detail;};
  return ui::CharacterMenuReloadActionV1(std::move(reload)).dispatch(name,call,error);
 }
 if(!prepare(error))return false;
 Context context(p);auto action_graph=context.actions();
 if(gameplay_services_.actions){if(!gameplay_services_.owner||!gameplay_services_.actions(p,action_graph,error))return false;}
 if(action_graph.equipment!=p.gear||action_graph.skills.identity()!=p.skills||action_graph.save!=p.save){error="Character panel supplemental provider replaced live profile authority";return false;}
 ui::CharacterMenuActionsOwnerV1 actions(std::move(action_graph));
 ui::CharacterMenuQueriesGraphV1 graph;graph.owner=p.world_owner;graph.actions=&actions;
 graph.characters=p.design.characters();graph.text=&text_;graph.text_environment=p.text_environment;
 graph.temporary=p.temporary;graph.font_palette=&palette_;
 graph.temporary_binding=[&p](const auto& authority,const auto& sheet,auto& e){if(authority.identity()==p.skills&&sheet==p.temporary)return true;e="Authored skill query lost the player's actual shared scratch";return false;};
 graph.player=[&p](auto index,bool remote,auto& actor,auto&){actor=index==0&&!remote?p.character:0;return true;};
 graph.difficulty=[&p](auto& value,auto&){value=p.difficulty;return true;};
 graph.item_equippable=[&p](const auto& item,bool& value,auto& e){const auto* row=data::item(p.gear->inventory()->table(),item.id);return row&&ui::character_menu_item_equippable_v1(row->record,p.gear->properties()->resolved,p.save->class_id(),*p.design.characters(),false,false,value,e);};
 graph.powers=[&](const auto& item,const auto*& output,auto& e){return presentation_.powers(p,item,text_,p.text_environment,output,e);};
 graph.can_increment=[&actions,&p](auto actor,auto row,bool& out,auto& e){if(actor!=p.character){e="CanIncSkill lost same selected player";return false;}return actions.can_increment(row,out,e);};
  if(gameplay_services_.queries){if(!gameplay_services_.owner||!gameplay_services_.queries(p,actions,graph,error))return false;}
  if(graph.actions!=&actions||graph.temporary!=p.temporary){error="Character panel supplemental query replaced live action/scratch authority";return false;}
  ui::CharacterMenuQueriesOwnerV1 queries(std::move(graph));
  return queries.dispatch(name,call,error);
 }catch(const std::exception& failure){error=failure.what();return false;}}
}
