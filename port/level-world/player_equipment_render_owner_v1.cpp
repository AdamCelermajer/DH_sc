#include "player_equipment_render_owner_v1.hpp"
#include "player_initial_grants_v2.hpp"
#include "player_equipment_queries_v1.hpp"
#include "../game-data/item_presentation_v5.hpp"
#include "../engine-ui/character_menu_inventory_mutation_v1.hpp"
#include "../engine-ui/character_menu_item_actions_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::player {
namespace {
using namespace data;
}
struct PlayerEquipmentRenderOwnerV1::Impl {
 PlayerEquipmentRenderInputsV1 input;
 LootTablesV2 loot;ItemPowerTablesV5 powers;ui::HudTextV1 text;
 PropertyView view{};
 std::unique_ptr<FreshInventoryOwnedV4> inventory;
 std::unique_ptr<ui::ItemTextOwnerV5> item_text;
 std::unique_ptr<PlayerGearEffectsV5> gear;
 std::unique_ptr<skinning::VisualSkinOwnerV6> visual;
 std::uintptr_t visual_identity{};
 bool attempted{},ready{},running{};
 bool prepared_v60{},prepare_active_v60{},grant_attempted_v60{};
 bool profile_window_v59{},gear_load_attempted_v59{};
 std::string failure;
 explicit Impl(PlayerEquipmentRenderInputsV1 v):input(std::move(v)){}
 LootTablesV2::Borrow loot_source_v88()const{return input.immutable_loot_v88?input.immutable_loot_v88:loot.borrow();}
 ItemPowerTablesV5::Borrow power_source_v88()const{return input.immutable_powers_v88?input.immutable_powers_v88:powers.borrow();}
 bool query(EquipmentWorldQueryV1 q,std::uintptr_t& id,std::int32_t& value,std::string& e){
  id=0;value=0;if(!input.world.invoke){e="Required actual equipment world query unavailable";return false;}
  if(!input.world.invoke(input.world.context,q,input.character,id,value,e)){if(e.empty())e="Required equipment world query rejected";return false;}return true;
 }
 bool debug(bool load,const char* name,std::int32_t& value,std::string& e){
  if(!input.debug||!input.debug_files.open_read||!input.debug_files.close_read){e="Required genuine equipment DebugSwitches unavailable";return false;}
  if(load){if(dh2_character_debug_load(input.debug,&input.debug_files)==1)return true;}
  else {std::uint32_t w;if(dh2_character_debug_get(&w,input.debug,name,&input.debug_files)==1){std::memcpy(&value,&w,4);return true;}}
  e="Genuine equipment DebugSwitches operation failed";return false;
 }
 static bool visual_debug(void* p,FreshInventoryOwnedV4&,const GearSkinRequestV5& q,std::int32_t& value,std::string& e){auto& s=*static_cast<Impl*>(p);return s.debug(q.operation==GearSkinOperationV5::debug_load,q.name,value,e);}
 GearSkinServicesV5 skin_services(){return visual?(input.source_visual2d8_v62?visual->gear_services_for_source_visual_v62({this,visual_debug},input.source_visual2d8_v62):visual->gear_services({this,visual_debug})):GearSkinServicesV5{};}
 bool skin(std::string& e){if(!visual){e="Equipment visual detached; rebind before source Skin effects";return false;}return gear->update_skin(input.source_visual2d8_v62?input.source_visual2d8_v62:&visual_identity,skin_services(),e);}
 static bool effect(void* p,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string& e){
  auto& s=*static_cast<Impl*>(p);if(s.inventory.get()!=&inventory){e="Equipment authoritative inventory identity mismatch";return false;}
  using O=OwnedInventoryOperationV4;
  switch(q.operation){
   case O::update_name:case O::update_stats:case O::update_requirements:{if(!q.item){e="Equipment text requires actual owned item";return false;}auto t=s.item_text->services();if(q.operation==O::update_name)return item_update_name_v5(*q.item,t,e);if(q.operation==O::update_stats)return item_update_stats_v5(*q.item,t,e);return item_update_requirements_v5(*q.item,t,e);}
   case O::update_gear_properties:return s.gear->update_properties(e);
   case O::skin:return s.skin(e);
   case O::validate_hp_mp:return s.gear->validate_hp_mp(e);
   case O::debug_load:case O::debug_query:return s.debug(q.operation==O::debug_load,q.name,out.value,e);
   case O::current_player:{std::uintptr_t id;std::int32_t value;if(!s.query(q.argument?EquipmentWorldQueryV1::current_difficulty:EquipmentWorldQueryV1::current_player,id,value,e))return false;out.identity=id;out.value=value;return true;}
   case O::player_count:return s.query(EquipmentWorldQueryV1::player_count,out.identity,out.value,e);
   default:if(s.input.required.invoke)return s.input.required.invoke(s.input.required.context,inventory,q,out,e);e="Required equipment continuation unavailable at source "+std::to_string(q.source_caller);return false;
  }
 }
 static void observe(void* p,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& q){auto& s=*static_cast<Impl*>(p);if(s.input.required.observe_storage)s.input.required.observe_storage(s.input.required.context,inventory,q);}
 OwnedInventoryServicesV4 effects(){return {this,effect,observe};}
 static bool drop_effect_v4(void* p,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string& e){
  auto& s=*static_cast<Impl*>(p);
  if(inventory.character()||inventory.properties()!=s.input.properties||&inventory.table()!=&s.inventory->table()){
   e="Menu drop effects require original NULL-character inventory and same property/table backing";return false;
  }
  using O=OwnedInventoryOperationV4;
  switch(q.operation){
   case O::debug_load:case O::debug_query:return s.debug(q.operation==O::debug_load,q.name,out.value,e);
   case O::update_name:case O::update_stats:case O::update_requirements:{if(!q.item){e="Drop text requires actual transferred item";return false;}auto t=s.item_text->services();if(q.operation==O::update_name)return item_update_name_v5(*q.item,t,e);if(q.operation==O::update_stats)return item_update_stats_v5(*q.item,t,e);return item_update_requirements_v5(*q.item,t,e);}
   case O::current_player:return s.query(q.argument?EquipmentWorldQueryV1::current_difficulty:EquipmentWorldQueryV1::current_player,out.identity,out.value,e);
   case O::player_count:return s.query(EquipmentWorldQueryV1::player_count,out.identity,out.value,e);
   default:if(s.input.required.invoke)return s.input.required.invoke(s.input.required.context,inventory,q,out,e);
    e="Required NULL-character drop inventory continuation "+std::to_string(q.source_caller);return false;
  }
 }
 static int grant(void* p,const InitialGrantRequest32V2* q,InitialGrantResponse8V2* out){auto& s=*static_cast<Impl*>(p);try{
  if(q->owner!=s.input.character){s.failure="Initial equipment Character identity mismatch";return -1;}std::uintptr_t id;std::int32_t value=0;bool ok=true;
  switch(q->operation){
   case online:ok=s.query(EquipmentWorldQueryV1::online,id,value,s.failure);break;
   case online_player_record:ok=s.query(EquipmentWorldQueryV1::online_player_record,id,value,s.failure);break;
   case num_items:if(s.inventory->items().size()>65536){s.failure="Initial equipment count exceeds source owned bound";return -1;}value=std::int32_t(s.inventory->items().size());break;
   case read_gold:value=s.inventory->gold();break;
   case loot_property:value=s.input.properties->resolved[9];break;
   case add_loot:ok=s.inventory->add_fixed_loot(q->arguments[0],s.effects(),s.failure);break;
   case is_equippable:{auto i=std::uint32_t(q->arguments[0]);if(i>=s.inventory->items().size()){s.failure="Initial equippable index invalid";return -1;}auto* row=item(s.inventory->table(),s.inventory->items()[i]->item->id);if(!row){s.failure="Initial equippable actual metadata absent";return -1;}value=row->record.words[26]!=-1;break;}
   case dh2::player::auto_equip:ok=s.inventory->character_auto_equip(std::uint32_t(q->arguments[0]),value,s.effects(),s.failure);break;
   case update_skin:ok=s.skin(s.failure);break;
   default:s.failure="Unsupported initial equipment service";return -1;
  }out->value=value;return ok?0:-1;
 }catch(const std::exception& e){s.failure=e.what();return -1;}}
 bool read(const char* uri,std::vector<std::uint8_t>& bytes,std::string& e){if(!input.assets.read){e="Required actual equipment cache provider absent";return false;}const auto r=input.assets.read(input.assets.context,uri,bytes,e);if(r==skinning::VisualAssetResultV6::found)return true;if(e.empty())e=std::string(r==skinning::VisualAssetResultV6::missing?"Missing genuine equipment resource: ":"Equipment resource provider failure: ")+uri;return false;}
 template<class F> bool table(const char* prefix,F load,std::string& e){std::vector<std::uint8_t> b,n,f;auto base=std::string("data/pydata/")+prefix;return read((base+"_pyarray.bin").c_str(),b,e)&&read((base+"_pyarraynames.bin").c_str(),n,e)&&read((base+"_pystructnames.bin").c_str(),f,e)&&load(b,n,f,e);}
 bool available(std::string& e){if(!ready||!visual){e="Equipment owner not ready or visual detached";return false;}if(running){e="Unsupported destructive equipment callback reentry";return false;}return true;}
 bool meets(const ItemInstanceV1* instance,bool& result,std::string& e){std::uintptr_t id;std::int32_t v;if(!query(EquipmentWorldQueryV1::online,id,v,e))return false;EquipmentRequirements32V1 facts{};facts.online=std::uint32_t(v);if(v){if(!query(EquipmentWorldQueryV1::remotely_updated,id,v,e))return false;facts.remote=std::uint32_t(v);if(v){result=true;return true;}}if(!instance){result=true;return true;}auto* row=item(inventory->table(),instance->id);if(!row){e="Equipment requirement metadata absent";return false;}constexpr unsigned fields[5]{19,149,150,151,152};facts.present=1;for(unsigned j=0;j<5;++j)facts.cached[j]=input.properties->resolved[fields[j]];std::int32_t accepted;if(dh2_equipment_requirements_v1(&accepted,&facts,&row->record)){e="Malformed source requirement projection";return false;}result=accepted!=0;return true;}
 bool prune(std::string& e,unsigned depth=0){if(depth>18){e="Requirement pruning source reentry exceeds bounded owner budget";return false;}bool changed=false;for(unsigned slot=0;slot<9;++slot){auto set=slot==1||slot==2?inventory->current_equipment():0;auto* cell=inventory->equipment()[set][slot];bool accepted;if(!meets(cell?cell->item.get():nullptr,accepted,e))return false;if(!accepted){if(!inventory->unequip_from_slot(slot,-1,effects(),e))return false;changed=true;}}
  if(changed)return gear->update_properties(e)&&prune(e,depth+1)&&skin(e)&&gear->validate_hp_mp(e);
  return true;
 }
 bool refresh(std::string& e,bool requirements){return gear->update_properties(e)&&(!requirements||prune(e))&&skin(e)&&gear->validate_hp_mp(e);}
 bool weapon_queries(EquipmentQueries12V1& out,std::string& e)const {const auto& set=inventory->equipment()[inventory->current_equipment()];const ItemRecord164* records[2]{};for(unsigned j=0;j<2;++j)if(set[j+1]){auto* row=item(inventory->table(),set[j+1]->item->id);if(!row){e="Equipment weapon metadata absent";return false;}records[j]=&row->record;}if(dh2_equipment_queries_v1(&out,records[0],records[1],input.properties->resolved[203])){e="Malformed owned weapon query";return false;}return true;}
};
PlayerEquipmentRenderOwnerV1::PlayerEquipmentRenderOwnerV1(PlayerEquipmentRenderInputsV1 v):impl_(std::make_unique<Impl>(std::move(v))){}
PlayerEquipmentRenderOwnerV1::~PlayerEquipmentRenderOwnerV1()=default;
bool PlayerEquipmentRenderOwnerV1::initialize(std::string& e){return prepare_restore_v60(e)&&finish_initial_grants_v60(e);}
bool PlayerEquipmentRenderOwnerV1::prepare_restore_v60(std::string& e){auto& s=*impl_;e.clear();if(s.attempted){e="Equipment initialization already attempted; retained source prefix";return false;}if(!s.input.design||!s.input.properties||!s.input.random||!s.input.character||!s.input.resources||!s.input.live_scene||!s.input.debug||!s.input.world.invoke||s.input.language_pack<0||s.input.language_pack>8){e="Malformed persistent equipment inputs";return false;}s.attempted=true;
 s.prepare_active_v60=true;struct Preparing{bool& value;~Preparing(){value=false;}}preparing{s.prepare_active_v60};
 if(s.input.saved_gear_v50&&s.input.source_profile_load_v59){e="Cannot deliver GEAR through both pre-Grant cache and source SG_Load";return false;}
 try{auto span=[](const std::vector<std::uint8_t>& b){return Bytes{b.data(),b.size()};};
  if((!s.input.immutable_loot_v88&&!s.table("loot_table",[&](auto& b,auto& n,auto& f,std::string& e){return s.loot.load(span(b),span(n),span(f),e);},e))||
     (!s.input.immutable_powers_v88&&!s.table("item_powers",[&](auto& b,auto& n,auto& f,std::string& e){return s.powers.load(span(b),span(n),span(f),e);},e))||
     !s.table("common_text",[&](auto& b,auto& n,auto& f,std::string& e){return s.text.load({b.data(),b.size()},{n.data(),n.size()},{f.data(),f.size()},e);},e)||!s.text.switch_pack(s.input.language_pack,false,e))return false;
  s.view=data::property_view(*s.input.design.rules(),*s.input.properties);
  if(s.input.constructed_inventory_v60){
   auto& actual=*s.input.constructed_inventory_v60;
   if(actual.character()!=s.input.character||actual.properties()!=s.input.properties||
      actual.random_v60()!=s.input.random||actual.table().identifiers!=s.loot_source_v88().items().identifiers){
    e="Constructed Character inventory37c differs from SAME Gear identity/properties/RNG/source table";return false;
   }
   s.inventory=std::move(s.input.constructed_inventory_v60);
  }else s.inventory=std::make_unique<FreshInventoryOwnedV4>(s.input.character,s.loot_source_v88(),*s.input.random,s.input.potion_capacity,s.input.properties);
  s.item_text=std::make_unique<ui::ItemTextOwnerV5>(s.inventory->table(),*s.input.design.characters(),s.text,s.input.text_environment);const auto& rows=*s.input.design.class_rows();s.gear=std::make_unique<PlayerGearEffectsV5>(*s.inventory,s.view,rows.data(),std::uint32_t(rows.size()),s.power_source_v88());
  s.visual=std::make_unique<skinning::VisualSkinOwnerV6>(s.input.resources,*s.input.live_scene,s.input.assets);if(!s.visual->initialize(e))return false;s.visual_identity=s.visual->identity();
  if(s.input.source_profile_load_v59){
   s.profile_window_v59=true;struct ProfileWindow{bool& value;~ProfileWindow(){value=false;}}window{s.profile_window_v59};
   if(!s.input.source_profile_load_v59(*this,e))return false;
  }
  if(s.input.saved_gear_v50){bool present{};Bytes payload{};std::shared_ptr<const void> lease;
   if(!s.input.saved_gear_v50(present,payload,lease,e))return false;
   if(present){if(!lease){e="Required actual selected profile GEAR lifetime";return false;}
    InventoryLoadReceiptV1 receipt;if(!s.inventory->load_saved_section_v50(payload,s.power_source_v88().names(),s.effects(),receipt,e)||!receipt.completed)return false;
   }
  }
  s.prepared_v60=true;return true;
 }catch(const std::exception& x){e=x.what();return false;}}
bool PlayerEquipmentRenderOwnerV1::finish_initial_grants_v60(std::string& e){
 auto& s=*impl_;e.clear();
 if(!s.prepared_v60||s.prepare_active_v60||s.profile_window_v59||s.running||s.grant_attempted_v60||s.ready){
  e="Required completed SAME equipment restore before once-only InitialGrant";return false;
 }
 s.grant_attempted_v60=true;
 s.running=true;struct Granting{bool& value;~Granting(){value=false;}}granting{s.running};
 try{InitialGrantServices16V2 services{&s,Impl::grant};
  if(dh2_player_initial_equipment_v2(s.input.character,&services)){
   e=s.failure.empty()?"Required initial equipment provider failed":s.failure;return false;
  }
  s.ready=true;return true;
 }catch(const std::exception& x){e=x.what();return false;}
}
bool PlayerEquipmentRenderOwnerV1::prepared_v60()const noexcept{return impl_->prepared_v60;}
bool PlayerEquipmentRenderOwnerV1::source_reset_gear_properties_v61(std::string& e){
 auto& s=*impl_;if(!s.prepared_v60||s.prepare_active_v60||s.running||!s.inventory){e="Required prepared SAME Gear for ResetGearsProperties";return false;}
 if(dh2_gear_reset_v5(s.input.properties->gear.data(),s.view.defaults)){e="Required genuine gear/default sheets";return false;}e.clear();return true;
}
bool PlayerEquipmentRenderOwnerV1::source_load_gear_properties_v61(std::string& e){
 auto& s=*impl_;if(!s.prepared_v60||s.prepare_active_v60||s.running||!s.inventory){e="Required prepared SAME Gear for LoadGearsProperties";return false;}
 // Whole3df480 adds selected item stats/powers to existing gear sheet. No
 // reset/class recalc: those are distinct surrounding InitPost calls.
 auto* sheet=s.input.properties->gear.data();
 for(unsigned slot=0;slot<9;++slot){auto* owned=s.inventory->equipment()[(slot==1||slot==2)?s.inventory->current_equipment():0][slot];if(!owned)continue;
  if(!owned->item){e="Required actual equipped item";return false;}auto& instance=*owned->item;const auto* metadata=item(s.inventory->table(),instance.id);
  if(!metadata||dh2_gear_stats_v5(sheet,s.view.defaults,&metadata->record,slot==2)){e="Required source item stats";return false;}
  for(auto id:instance.powers){if(id<0||std::size_t(id)>=s.power_source_v88().rows().size()){e="Required source equipped power";return false;}
   const auto& row=s.power_source_v88().rows()[id];GearPowerView16V5 view{row.properties.data(),std::uint32_t(row.properties.size()),0};
   if(dh2_gear_power_v5(sheet,s.view.defaults,&view,slot==2)){e="Required source power contribution";return false;}
  }
 }e.clear();return true;
}
bool PlayerEquipmentRenderOwnerV1::source_initpost_gold_store_v61(std::int32_t value,std::string& e){
 auto& s=*impl_;if(!s.prepared_v60||s.prepare_active_v60||s.running||!s.inventory){
  e="Required SAME prepared inventory at source Character.InitPost gold store";return false;
 }
 s.inventory->source_initpost_gold_store_v61(value);e.clear();return true;
}
bool PlayerEquipmentRenderOwnerV1::load_saved_section_v59(Bytes bytes,InventoryLoadReceiptV1& receipt,std::string& e){
 auto& s=*impl_;receipt={};
 if(!s.profile_window_v59||s.ready||s.running||!s.inventory||!s.gear||!s.visual){
  e="Required SAME pre-InitialGrant source GEAR delivery window";return false;
 }
 if(s.gear_load_attempted_v59){e="Source fresh GEAR restore cannot retry or append twice";return false;}
 s.gear_load_attempted_v59=true;s.running=true;struct Running{bool& value;~Running(){value=false;}}running{s.running};
 return s.inventory->load_saved_section_v50(bytes,s.power_source_v88().names(),s.effects(),receipt,e)&&receipt.completed;
}
bool PlayerEquipmentRenderOwnerV1::ready()const noexcept{return impl_->ready;}
bool PlayerEquipmentRenderOwnerV1::load_source_inventory_v122(Bytes bytes,InventoryLoadReceiptV1& receipt,std::string& e){
 auto& s=*impl_;receipt={};
 if(!s.ready||s.running||!s.inventory||!s.gear||!s.visual){e="Required nonreentrant SAME live Gear for source inventory load";return false;}
 s.running=true;struct Running{bool& value;~Running(){value=false;}}running{s.running};
 return s.inventory->load_saved_section_v50(bytes,s.power_source_v88().names(),s.effects(),receipt,e)&&receipt.completed;
}
const FreshInventoryOwnedV4* PlayerEquipmentRenderOwnerV1::inventory()const noexcept{return impl_->inventory.get();}
const std::shared_ptr<PropertyState>& PlayerEquipmentRenderOwnerV1::properties()const noexcept{return impl_->input.properties;}
PropertyView* PlayerEquipmentRenderOwnerV1::property_view()noexcept{return impl_->inventory?&impl_->view:nullptr;}
bool PlayerEquipmentRenderOwnerV1::auto_equip(std::uint32_t i,std::int32_t& result,std::string& e){auto& s=*impl_;if(!s.available(e))return false;s.running=true;struct G{bool& b;~G(){b=false;}}g{s.running};return s.inventory->character_auto_equip(i,result,s.effects(),e);}
bool PlayerEquipmentRenderOwnerV1::equip(std::uint32_t slot,std::uint32_t i,std::string& e){auto& s=*impl_;if(!s.available(e))return false;s.running=true;struct G{bool& b;~G(){b=false;}}g{s.running};return s.inventory->equip_to_slot(slot,i,false,s.effects(),e)&&s.refresh(e,true);}
bool PlayerEquipmentRenderOwnerV1::unequip(std::uint32_t slot,std::string& e){auto& s=*impl_;if(!s.available(e))return false;s.running=true;struct G{bool& b;~G(){b=false;}}g{s.running};return s.inventory->unequip_from_slot(slot,-1,s.effects(),e)&&s.refresh(e,true);}
bool PlayerEquipmentRenderOwnerV1::swap(std::string& e){auto& s=*impl_;if(!s.available(e))return false;s.running=true;struct G{bool& b;~G(){b=false;}}g{s.running};s.inventory->swap_equipment();return s.refresh(e,true);}
bool PlayerEquipmentRenderOwnerV1::refresh_effects(std::string& e){auto& s=*impl_;if(!s.available(e))return false;s.running=true;struct G{bool& b;~G(){b=false;}}g{s.running};return s.refresh(e,false);}
bool PlayerEquipmentRenderOwnerV1::remove_one_potion(std::string& e){
 auto& s=*impl_;if(!s.available(e))return false;
 auto* potion=s.inventory->potion();if(!potion||s.inventory->num_potions()<=0){e="No health potions";return false;}
 s.running=true;struct G{bool& b;~G(){b=false;}}guard{s.running};
 data::CharacterMenuInventoryMutationV1 mutation(*s.inventory);
 for(std::uint32_t i=0;i<s.inventory->items().size();++i){auto* actual=s.inventory->items()[i]->item.get();
  if(actual==potion)return actual->signed_quantity()>1?mutation.add_quantity(*actual,-1,e):mutation.remove(i,s.effects(),e);
 }
 e="Potion identity absent from authoritative inventory";return false;
}
void PlayerEquipmentRenderOwnerV1::project_potion_capacity(std::int8_t value)noexcept{if(impl_->inventory)impl_->inventory->project_potion_capacity(value);}
bool PlayerEquipmentRenderOwnerV1::loot_add_item_v8(std::unique_ptr<data::ItemInstanceV1>& incoming,bool force,bool convert_gold,std::int32_t& index,std::string& e){
 auto& s=*impl_;e.clear();if(!s.available(e))return false;
 s.running=true;struct G{bool& b;~G(){b=false;}}guard{s.running};
 return s.inventory->add_item(incoming,force,convert_gold,index,s.effects(),e);
}
bool PlayerEquipmentRenderOwnerV1::loot_add_gold_v8(std::int32_t amount,std::string& e){
 auto& s=*impl_;e.clear();if(!s.available(e))return false;
 s.running=true;struct G{bool& b;~G(){b=false;}}guard{s.running};
 return s.inventory->add_gold(amount,s.effects(),e);
}
bool PlayerEquipmentRenderOwnerV1::try_consuming_source_v108(std::int32_t id,std::int32_t quantity,bool& consumed,std::string& e){
 auto& s=*impl_;consumed=false;if(!s.available(e))return false;
 s.running=true;struct Guard{bool& b;~Guard(){b=false;}}guard{s.running};
 //Source FindItem3fd15c tests actual Potion24 before walking Item slots.
 data::ItemInstanceV1* found{};std::uint32_t index=UINT32_MAX;
 const auto* potion=s.inventory->potion();
 for(std::uint32_t i=0;i<s.inventory->items().size();++i){auto* p=s.inventory->items()[i]->item.get();if(p==potion&&p&&p->id==id){found=p;index=i;break;}}
 if(!found)for(std::uint32_t i=0;i<s.inventory->items().size();++i){auto* p=s.inventory->items()[i]->item.get();if(p&&p->id==id){found=p;index=i;break;}}
 if(!found||quantity>found->signed_quantity()){e.clear();return true;}
 std::int32_t next;auto bits=std::uint32_t(found->signed_quantity())-std::uint32_t(quantity);std::memcpy(&next,&bits,4);
 if(next<0){e="Original TryConsuming reached negative SetQty assertion";return false;}
 found->quantity=static_cast<std::uint16_t>(next); //SetQty source low16 store
 if(found->signed_quantity()<=0){data::CharacterMenuInventoryMutationV1 mutation(*s.inventory);if(!mutation.remove(index,s.effects(),e))return false;}
 consumed=true;e.clear();return true;
}
bool PlayerEquipmentRenderOwnerV1::loot_inventory_full_v10(bool& full,std::string& e){
 auto& s=*impl_;e.clear();if(!s.available(e))return false;
 s.running=true;struct G{bool& b;~G(){b=false;}}guard{s.running};
 return s.inventory->inventory_full(full,s.effects(),e);
}
const data::InventoryGatheringIdsV11* PlayerEquipmentRenderOwnerV1::gathering_ids_v11()const noexcept{return impl_->inventory?&impl_->inventory->gathering_ids_v11():nullptr;}
bool PlayerEquipmentRenderOwnerV1::register_gathering_id_v11(std::int32_t id,std::string& e){
 auto& s=*impl_;e.clear();if(!s.inventory||s.running){e="Required same inventory/nonreentrant gathering registration";return false;}
 s.running=true;struct Guard{bool& b;~Guard(){b=false;}}guard{s.running};
 s.inventory->gathering_ids_v11().register_id(id);return true;
}
bool PlayerEquipmentRenderOwnerV1::unregister_gathering_id_v11(std::int32_t id,const data::InventoryGatheringAssertServicesV11& a,std::string& e){
 auto& s=*impl_;e.clear();if(!s.inventory||s.running){e="Required same inventory/nonreentrant gathering unregistration";return false;}
 s.running=true;struct Guard{bool& b;~Guard(){b=false;}}guard{s.running};
 return s.inventory->gathering_ids_v11().unregister_id(id,a,e);
}
bool PlayerEquipmentRenderOwnerV1::check_item_requirements_v1(std::string& e){
 auto& s=*impl_;e.clear();if(!s.available(e))return false;
 s.running=true;struct G{bool& b;~G(){b=false;}}guard{s.running};
 return s.prune(e);
}
bool PlayerEquipmentRenderOwnerV1::bind_menu_item_actions_v4(ui::CharacterMenuItemActionsGraphV1& graph,std::string& e){
 auto& s=*impl_;if(!s.available(e))return false;
 graph.inventory=s.inventory.get();graph.properties=&s.view;graph.inventory_services=s.effects();
 graph.skin=[this](std::string& error){auto& actual=*impl_;if(!actual.available(error))return false;
  actual.running=true;struct Guard{bool& value;~Guard(){value=false;}}guard{actual.running};return actual.skin(error);};
 graph.drop_container=[this](std::unique_ptr<data::FreshInventoryOwnedV4>& out,data::OwnedInventoryServicesV4& effects,std::string& error){
  auto& actual=*impl_;if(!actual.available(error)||!actual.input.random)return false;
  out=data::FreshInventoryOwnedV4::create_drop_temporary_v4(actual.loot_source_v88(),*actual.input.random,actual.input.properties);
  effects={&actual,Impl::drop_effect_v4,Impl::observe};return true;
 };
 return true;
}
bool PlayerEquipmentRenderOwnerV1::loot_sources_v8(data::LootTablesV2::Borrow& tables,data::ItemPowerTablesV5::Borrow& powers,data::ItemTextServicesV5& text,data::LootRandom8V2*& random,std::string& e)const{
 auto& s=*impl_;e.clear();if(!s.ready||s.running||!s.inventory||!s.item_text||!s.input.random){e="Player loot sources unavailable or active destructive callback";return false;}
 tables=s.loot_source_v88();powers=s.power_source_v88();text=s.item_text->services();random=s.input.random;return true;
}
bool PlayerEquipmentRenderOwnerV1::swap_inventory_for_initial_slots(std::string& error){auto& s=*impl_;if(!s.available(error))return false;s.inventory->swap_equipment();error.clear();return true;}
bool PlayerEquipmentRenderOwnerV1::draw_parts(std::vector<skinning::VisualDrawPartV6>& out,std::string& e)const{if(!impl_->ready||!impl_->visual){e="Equipment draw visual absent";return false;}return impl_->visual->draw_parts(out,e);}
bool PlayerEquipmentRenderOwnerV1::draw_views(const std::vector<skinning::VisualDrawViewV32>*& out,std::string& e)const{out=nullptr;if(!impl_->ready||!impl_->visual){e="Equipment draw visual absent";return false;}return impl_->visual->draw_views(out,e);}
skinning::SkinPoseCountersV32 PlayerEquipmentRenderOwnerV1::pose_counters()const noexcept{return impl_->visual?impl_->visual->pose_counters():skinning::SkinPoseCountersV32{};}
bool PlayerEquipmentRenderOwnerV1::stance_facts(bool player,std::int32_t count,character::StanceFacts16& out,std::string& e)const{auto& s=*impl_;if(!s.ready){e="Equipment stance requires initialized owner";return false;}EquipmentQueries12V1 q;if(!s.weapon_queries(q,e))return false;character::StanceFacts16 f;f.count=count;f.predicates=player?std::uint32_t(character::stance_is_player):0u;if(q.flags&query_main)f.predicates|=character::stance_has_main_hand;if(q.flags&query_bow)f.predicates|=character::stance_has_bow;if(q.flags&query_staff)f.predicates|=character::stance_has_staff;if(q.flags&query_dual)f.predicates|=character::stance_dual_wielding;if(q.flags&query_two_effective)f.predicates|=character::stance_has_two_hander;out=f;return true;}
void PlayerEquipmentRenderOwnerV1::detach_visual()noexcept{impl_->visual_identity=0;impl_->visual.reset();impl_->input.live_scene=nullptr;}
bool PlayerEquipmentRenderOwnerV1::combat_view(data::CombatantView& out,std::string& e)const{auto& s=*impl_;if(!s.ready){e="Equipment combat requires initialized owner";return false;}EquipmentQueries12V1 q;if(!s.weapon_queries(q,e))return false;if(q.main_category<-1||q.main_category>=141||q.off_category<-1||q.off_category>=141){e="Combat equipment category outside genuine kernel domain";return false;}auto value=out;value.properties=s.input.properties->resolved.data();value.main_damage_class=q.main_category;value.off_damage_class=q.off_category;value.dual_wield=bool(q.flags&query_dual);value.shield=bool(q.flags&query_shield);value.two_hander=bool(q.flags&query_two_raw);out=value;return true;}
bool PlayerEquipmentRenderOwnerV1::rebind_visual(skinning::VisualSkinResourcesV6::Borrow b,const scene::Scene& live,std::string& e){auto& s=*impl_;if(!s.ready||s.running||!b){e="Malformed equipment rebind or active callback";return false;}try{auto candidate=std::make_unique<skinning::VisualSkinOwnerV6>(b,live,s.input.assets);if(!candidate->initialize(e))return false;auto id=candidate->identity();auto callbacks=s.input.source_visual2d8_v62?candidate->gear_services_for_source_visual_v62({&s,Impl::visual_debug},s.input.source_visual2d8_v62):candidate->gear_services({&s,Impl::visual_debug});if(!s.gear->update_skin(s.input.source_visual2d8_v62?s.input.source_visual2d8_v62:&id,callbacks,e))return false;s.visual=std::move(candidate);s.visual_identity=id;s.input.resources=std::move(b);s.input.live_scene=&live;return true;}catch(const std::exception& x){e=x.what();return false;}}
}
