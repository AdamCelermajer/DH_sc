#include "fresh_inventory_owned_v4.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh2::data {
namespace {
bool legacy_fixture_random(void* context,std::int32_t bound,std::uint32_t stream,std::int32_t& value,std::string& error){
 if(stream!=0||!context){error="Invalid legacy fixture RNG request";return false;}
 if(dh2_loot_v2_random(static_cast<LootRandom8V2*>(context),bound,&value)){error="Invalid legacy fixture RNG";return false;}
 return true;
}
}
FreshInventoryOwnedV4::FreshInventoryOwnedV4(std::uintptr_t id,LootTablesV2::Borrow b,InventoryRandomServiceV4 random,std::int8_t cap,PropertyState& props):tables_(std::move(b)),random_(random),character_(id),properties_(&props),potion_capacity_(cap){if(!id||!tables_)throw std::invalid_argument("Unbound fresh inventory owner");}
FreshInventoryOwnedV4::FreshInventoryOwnedV4(std::uintptr_t id,LootTablesV2::Borrow b,LootRandom8V2& r,std::int8_t cap,std::shared_ptr<PropertyState> props):tables_(std::move(b)),random_{&r,legacy_fixture_random},character_(id),properties_(props.get()),fixture_properties_(std::move(props)),potion_capacity_(cap){if(!id||!tables_||!properties_)throw std::invalid_argument("Unbound legacy fixture inventory owner");}
bool FreshInventoryOwnedV4::rebind_properties(std::uintptr_t character,PropertyState& properties,std::string& error){
 if(!character||character!=character_||!properties_){error="Property rebind must preserve the retained Character inventory identity";return false;}
 if(running_||callback_depth_){error="Property rebind is unsupported during an active inventory callback";return false;}
 if(fixture_properties_){error="Fixture-owned PropertyState cannot be rebound";return false;}
 properties_=&properties;error.clear();return true;
}
bool FreshInventoryOwnedV4::mutation_allowed(std::string& e)const{if(callback_depth_){e="Unsupported destructive inventory callback reentry";return false;}return true;}
bool FreshInventoryOwnedV4::register_quest_gathering_item_id(std::int32_t item_id,std::string& e){
 if(!mutation_allowed(e))return false;
 if(running_){e="Gathering-ID mutation during active inventory operation is unsupported";return false;}
 if(item_id<0||std::size_t(item_id)>=tables_.items().rows.size()){
  e="Quest gathering Item ID is outside the retained ItemTable";return false;
 }
 const auto found=std::find_if(quest_gathering_item_ids_.begin(),quest_gathering_item_ids_.end(),
  [item_id](const QuestGatheringItemIdV4& entry){return entry.item_id==item_id;});
 if(found!=quest_gathering_item_ids_.end()){
  if(found->registrations==std::numeric_limits<std::uint8_t>::max()){
   e="Quest gathering Item ID registration count would overflow its source byte";return false;
  }
  ++found->registrations;e.clear();return true;
 }
 try{quest_gathering_item_ids_.push_back({item_id,1});}
 catch(const std::exception&){e="Quest gathering Item ID list allocation failed";return false;}
 e.clear();return true;
}
bool FreshInventoryOwnedV4::unregister_quest_gathering_item_id(std::int32_t item_id,std::string& e){
 if(!mutation_allowed(e))return false;
 if(running_){e="Gathering-ID mutation during active inventory operation is unsupported";return false;}
 if(item_id<0||std::size_t(item_id)>=tables_.items().rows.size()){
  e="Quest gathering Item ID is outside the retained ItemTable";return false;
 }
 const auto found=std::find_if(quest_gathering_item_ids_.begin(),quest_gathering_item_ids_.end(),
  [item_id](const QuestGatheringItemIdV4& entry){return entry.item_id==item_id;});
 if(found==quest_gathering_item_ids_.end()||!found->registrations){
  e="Quest gathering Item ID unregister would underflow its source byte";return false;
 }
 if(--found->registrations==0)quest_gathering_item_ids_.erase(found);
 e.clear();return true;
}
bool FreshInventoryOwnedV4::has_quest_gathering_item_id(std::int32_t item_id,bool& registered,std::string& e)const{
 registered=false;
 if(item_id<0||std::size_t(item_id)>=tables_.items().rows.size()){
  e="Quest gathering Item ID is outside the retained ItemTable";return false;
 }
 const auto found=std::find_if(quest_gathering_item_ids_.begin(),quest_gathering_item_ids_.end(),
  [item_id](const QuestGatheringItemIdV4& entry){return entry.item_id==item_id;});
 if(found!=quest_gathering_item_ids_.end()){
  if(!found->registrations){e="Quest gathering Item ID has an invalid zero source refcount";return false;}
  registered=true;
 }
 e.clear();return true;
}
bool FreshInventoryOwnedV4::quest_gathering_item_quantity(std::int32_t item_id,bool& found,std::int32_t& quantity,std::string& e)const{
 found=false;quantity=0;
 if(item_id<0||std::size_t(item_id)>=tables_.items().rows.size()){
  e="GatherLoot quantity query is outside the retained ItemTable";return false;
 }
 for(const auto& slot:items_)if(slot&&slot->item&&slot->item->id==item_id){
  found=true;quantity=slot->item->signed_quantity();e.clear();return true;
 }
 e.clear();return true;
}
bool FreshInventoryOwnedV4::invoke_loot_power_bridge(void* context,const LootPowerRequestV7& q,std::int32_t& result,std::string& e){
 auto& bridge=*static_cast<LootPowerBridgeV4*>(context);OwnedInventoryResponseV4 out;
 auto op=static_cast<OwnedInventoryOperationV4>(q.operation);
 if(op!=OwnedInventoryOperationV4::debug_load&&op!=OwnedInventoryOperationV4::debug_query&&op!=OwnedInventoryOperationV4::add_power){e="Unsupported V7 inventory effect";return false;}
 if(!bridge.owner->deliver(*bridge.services,op,q.source_caller,q.item,q.key,q.power,std::uint32_t(q.difficulty),out,e))return false;
 result=out.value;return true;
}
void FreshInventoryOwnedV4::observe(const OwnedInventoryServicesV4& s,OwnedInventoryOperationV4 op,std::uint32_t caller,ItemInstanceV1* item,std::int32_t arg,std::uint32_t index){if(s.observe_storage){++callback_depth_;struct Guard{std::uint32_t& n;~Guard(){--n;}}guard{callback_depth_};s.observe_storage(s.context,*this,{op,caller,item,nullptr,arg,index});}}
bool FreshInventoryOwnedV4::deliver(const OwnedInventoryServicesV4& s,OwnedInventoryOperationV4 op,std::uint32_t caller,ItemInstanceV1* instance,const char* name,std::int32_t arg,std::uint32_t index,OwnedInventoryResponseV4& out,std::string& e){out={};if(op==OwnedInventoryOperationV4::destroy_item){if(!s.observe_storage&&!s.stateless_temporaries){e="Required Item retirement provider unavailable at "+std::to_string(caller);return false;}observe(s,op,caller,instance,arg,index);return true;}if(op==OwnedInventoryOperationV4::inventory_full){observe(s,op,caller,instance,arg,index);bool full;if(!is_full(full,s,e))return false;out.value=full;return true;}if(!s.invoke){e="Required owned inventory effect unavailable at "+std::to_string(caller);return false;}++callback_depth_;struct Guard{std::uint32_t& n;~Guard(){--n;}}guard{callback_depth_};if(!s.invoke(s.context,*this,{op,caller,instance,name,arg,index},out,e)){if(e.empty())e="Required owned inventory effect failed at "+std::to_string(caller);return false;}return true;}
bool FreshInventoryOwnedV4::debug(const OwnedInventoryServicesV4& s,std::uint32_t call,const char* name,std::int32_t& value,std::string& e){OwnedInventoryResponseV4 out;auto load=call-0x1c;if(call==0x40411c||call==0x403d48||call==0x403138)load=call-0x20;else if(call==0x403c2c)load=call-0x24;if(!deliver(s,OwnedInventoryOperationV4::debug_load,load,nullptr,nullptr,0,0,out,e)||!deliver(s,OwnedInventoryOperationV4::debug_query,call,nullptr,name,0,0,out,e))return false;value=out.value;return true;}
bool FreshInventoryOwnedV4::add_fixed_loot_impl(std::int32_t id,std::unique_ptr<ItemInstanceV1>& owned,bool retained,const OwnedInventoryServicesV4& s,const OwnedLootEffectsV7* loot_effects,const LootEntrySelectionContextV1* selection,std::vector<std::unique_ptr<OwnedItemSlotV4>>* world_items,std::string& e){
 if(!mutation_allowed(e))return false;if(running_||!s.invoke){e="Malformed fresh loot service or unsupported owner reentry";return false;}if(id<0||std::size_t(id)>=tables_.loots().size()){e.clear();return true;}running_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{running_};std::int32_t ignored=0;auto trace=[&](std::uint32_t at){return debug(s,at,"isTracingItemInventory_Loot",ignored,e);};
 if(loot_effects){const auto& expected=loot_effects->creation?loot_effects->creation->random_service():InventoryRandomServiceV4{};if(!loot_effects->creation||!loot_effects->powers||!loot_effects->text.invoke||!loot_effects->text.metadata||loot_effects->text.context!=s.context||random_.context!=expected.context||random_.next!=expected.next){e="Loot effects require this inventory's borrowed RNG and shared V5 services";return false;}if(&loot_effects->creation->resources().powers().rows()!=&loot_effects->powers.rows()){e="Loot effects must borrow the creation owner's V5 power snapshot";return false;}}
 if(!debug(s,0x40411c,"MP_MinimalRandoms",ignored,e))return false;if(ignored){e.clear();return true;}if(!trace(0x404160)||!trace(0x404190))return false;
 std::vector<const LootEntry32V2*> entries;
 std::vector<std::int32_t> recursion_path;
 std::size_t expanded_tables=0;
 const auto& all_loot=tables_.loots();
 static const std::vector<std::vector<LootQuantityChoiceV7>> no_quantities;
 const auto& quantities=loot_effects?loot_effects->creation->resources().quantities():no_quantities;
 auto append_entry=[&](std::vector<const LootEntry32V2*>& destination,const LootEntry32V2& entry,const char* label){
  if(destination.size()>=65536){e="Expanded loot entry count exceeds native budget";return false;}
  const auto list=entry.words[0];if(list<0||std::size_t(list)>=tables_.item_lists().size()){e=label;return false;}
  if(entry.words[1]!=-1&&!loot_effects){e="Required ItemPowerList producer unavailable";return false;}
  destination.push_back(&entry);return true;
 };
 auto roll_entries=[&](const std::vector<const LootEntry32V2*>& pool,std::int32_t quantity_id,std::vector<const LootEntry32V2*>& destination){
  if(pool.empty())return true;
  if(!selection||!loot_effects){e="Random loot requires original class/Debug facts and the shared quantity/power owner";return false;}
  if(quantity_id<0||std::size_t(quantity_id)>=quantities.size()){e="Required invalid NumProbArray Debug continuation";return false;}
  std::int32_t count=0;const auto& quantity_row=quantities[std::size_t(quantity_id)];
  if(loot_quantity_v7(&count,random_,quantity_row.data(),std::uint32_t(quantity_row.size()),0,e)){if(e.empty())e="Required NumProbArray selection continuation";return false;}
  if(count<0||count>4096){e="Random loot entry count exceeds native budget";return false;}
  std::vector<LootEntry32V2> contiguous;contiguous.reserve(pool.size());for(auto* entry:pool)contiguous.push_back(*entry);
  for(std::int32_t n=0;n<count;++n){
   std::uint32_t index=0;
   // The selection routine consumes a contiguous LootEntry array. For a
   // pooled recursive table, materialize only these small source records.
   if(!loot_entries_choose_weighted_v1(contiguous.data(),std::uint32_t(contiguous.size()),selection->player_counts,selection->infinite_loot_drops,random_,index,e))return false;
   const auto& entry=*pool[index];
   // Native _GetRandomLootEntry contributes its selected row unconditionally.
   // The following _DoPctRolls pass independently checks every candidate row,
   // including percentage rows that were excluded from weighted selection.
   if(!append_entry(destination,entry,"Selected random LootEntry ItemList requires unrecovered Debug continuation"))return false;
  }
  for(const auto* candidate:pool){
   bool accepted=false;
   if(!loot_entry_do_percent_roll_v1(*candidate,selection->infinite_loot_drops,
                                     random_,accepted,e))return false;
   if(accepted&&!append_entry(destination,*candidate,
                              "Percent LootEntry ItemList requires unrecovered Debug continuation"))return false;
  }
  return true;
 };
 auto expand_table=[&](auto&& self,std::int32_t table_id,std::vector<const LootEntry32V2*>& output,std::vector<const LootEntry32V2*>* shared_random,std::size_t depth)->bool{
  if(depth>64||++expanded_tables>4096){e="Nested LootTable expansion exceeds native budget";return false;}
  if(table_id<0||std::size_t(table_id)>=all_loot.size()){e="SubLootTable index requires original assertion continuation";return false;}
  if(std::find(recursion_path.begin(),recursion_path.end(),table_id)!=recursion_path.end()){e="Cyclic SubLootTable graph is outside the recovered cache domain";return false;}
  recursion_path.push_back(table_id);struct PathGuard{std::vector<std::int32_t>& p;~PathGuard(){p.pop_back();}} path_guard{recursion_path};
  const auto& table=all_loot[std::size_t(table_id)];
  if(!table.random_entries.empty()&&table.roll_type!=0){e="Random loot roll type requires its original caller continuation";return false;}
  if(!trace(0x403a20)||!trace(0x403a50)||!trace(0x403a94))return false;
  for(const auto& entry:table.fixed_entries){if(!trace(0x403b20)||!append_entry(output,entry,"Loot ItemList requires unrecovered Debug continuation"))return false;}
  std::vector<const LootEntry32V2*> local_random;
  if(!table.random_entries.empty()){
   if(shared_random){
    if(shared_random->size()+table.random_entries.size()>65536){e="Expanded pooled LootEntry count exceeds native budget";return false;}
    for(const auto& entry:table.random_entries)shared_random->push_back(&entry);
   }else{
    std::vector<const LootEntry32V2*> direct_random;direct_random.reserve(table.random_entries.size());for(const auto& entry:table.random_entries)direct_random.push_back(&entry);
    if(!roll_entries(direct_random,table.num_random_item_probs,output))return false;
   }
  }
  auto* child_pool=shared_random?shared_random:&local_random;
  for(auto child:table.sub_loots)if(!self(self,child,output,child_pool,depth+1))return false;
  // Native _AddLootTable selects a root quantity only after descendants have
  // contributed their random rows. Descendant quantity IDs are not consulted.
  if(!shared_random&&!local_random.empty()){
   if(!roll_entries(local_random,table.num_random_item_probs,output))return false;
  }
  return true;
 };
 if(!expand_table(expand_table,id,entries,nullptr,0))return false;
 if(!trace(0x403d48)||!trace(0x403c2c)||!trace(0x404264)||!trace(0x404294))return false;
 struct Prepared {std::int32_t id;std::uint8_t quantity;const Item* row;const LootEntry32V2* entry;};std::vector<Prepared> selected;
 for(const auto* source_entry:entries){const auto& entry=*source_entry;const auto& list=tables_.item_lists()[entry.words[0]];std::uint32_t sum=0;for(const auto& p:list)sum+=std::uint32_t(std::int32_t(p.probability));if(list.empty()||!sum){e="ItemList weighted selection requires source Debug continuation";return false;}
  std::int32_t bound;std::memcpy(&bound,&sum,4);std::int32_t draw;if(!random_.next||!random_.next(random_.context,bound,0,draw,e)){if(e.empty())e="Invalid borrowed source RNG";return false;}std::uint32_t remainder=std::uint32_t(draw);const LootItemEntryV2* chosen=nullptr;for(const auto& p:list){auto weight=std::uint32_t(std::int32_t(p.probability));if(remainder<weight){chosen=&p;break;}remainder-=weight;}if(!chosen){e="Weighted ItemList fell through required Debug continuation";return false;}
  auto generated_id=chosen->item;
  if(loot_effects&&(loot_effects->difficulty==1||loot_effects->difficulty==2)){
   const auto& identifiers=tables_.items().identifiers;const auto offset=std::size_t(loot_effects->difficulty);
   if(std::size_t(generated_id)+offset<identifiers.size()){
    const auto suffix=loot_effects->difficulty==1?"_Hard":"_VeryHard";
    if(identifiers[std::size_t(generated_id)+offset]==identifiers[std::size_t(generated_id)]+suffix)generated_id+=loot_effects->difficulty;
   }
  }
  auto* row=item(tables_.items(),generated_id);if(!row){e="Selected ItemID exceeds genuine ItemTable";return false;}if(!trace(0x403138))return false;selected.push_back({generated_id,chosen->quantity,row,&entry});
 }
 if(selected.empty()){if(!trace(0x404574)||!trace(0x4045a4)||!trace(0x4041f8)||!trace(0x404228))return false;e.clear();return true;}
 if(!trace(0x4042fc)||!trace(0x40432c))return false;OwnedInventoryResponseV4 out;
 for(const auto& chosen:selected){auto type=item_type(*chosen.row);if(type==13&&!loot_effects){e="Gold loot requires the source V7 value provider";return false;}std::int32_t repeats=1;auto distribution=chosen.row->record.words[4];if(distribution==2||distribution==3){if(!deliver(s,OwnedInventoryOperationV4::player_count,0x4043a8,nullptr,nullptr,0,0,out,e))return false;repeats=out.value;if(repeats<=0)continue;if(repeats>1024){e="Source player count exceeds owned budget";return false;}}
   for(std::int32_t j=0;j<repeats;++j){
   if(!(retained?create_item(chosen.id,1,RetainedItemSlotV4{&owned},s,e):create_item(chosen.id,1,owned,s,e)))return false;auto* instance=owned.get();
   std::int8_t qty;std::memcpy(&qty,&chosen.quantity,1);if(qty==-2)qty=99;if(qty<0){e="SetQty requires original negative Debug continuation";return false;}instance->quantity=std::uint16_t(qty);
    auto* valued=item(tables_.items(),instance->id);if(!valued){e="Item effects changed ID outside genuine ItemTable";return false;}if(loot_effects){LootPowerBridgeV4 bridge{this,&s};LootPowerServicesV7 power_services{&bridge,invoke_loot_power_bridge};if(!loot_effects->creation->add_powers(*chosen.entry,*instance,loot_effects->power_bonus256,loot_effects->requested_power_count,loot_effects->difficulty,power_services,e))return false;++callback_depth_;struct TextCallbackGuard{std::uint32_t& depth;~TextCallbackGuard(){--depth;}}text_guard{callback_depth_};if(!loot_item_value_v7(*instance,tables_.items(),loot_effects->powers,random_,loot_effects->value_bonus256,loot_effects->text,e))return false;}else{if(item_type(*valued)==13){e="Gold loot requires the source V7 value provider";return false;}auto value=std::uint32_t(valued->record.words[27])*std::uint32_t(valued->record.words[28]);std::memcpy(&instance->value,&value,4);if(!deliver(s,OwnedInventoryOperationV4::update_name,0x402128,instance,nullptr,0,0,out,e))return false;}
   auto* inserted=item(tables_.items(),instance->id);if(!inserted){e="Item effects changed ID outside genuine ItemTable";return false;}auto inserted_type=item_type(*inserted);
   if(!world_items&&potion_capacity_==0&&inserted_type==14){if(!destroy(owned,s,0x3ff70c,e))return false;continue;}
   auto slot=std::make_unique<OwnedItemSlotV4>();slot->item=std::move(owned);
   if(world_items){world_items->push_back(std::move(slot));continue;}
   if(!potion_&&inserted_type==14)potion_=instance;items_.push_back(std::move(slot));auto index=std::uint32_t(items_.size()-1);if(!deliver(s,OwnedInventoryOperationV4::inventory_full,0x3ff6c4,instance,nullptr,0,index,out,e))return false;if(out.value&&!deliver(s,OwnedInventoryOperationV4::full_notifications,0x3ff7a8,instance,nullptr,0,index,out,e))return false;
  }
 }
 e.clear();return true;
}
bool FreshInventoryOwnedV4::add_fixed_loot(std::int32_t id,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,nullptr,e))return false;if(*slot.value){e="Fixed loot incoming slot already owns an item";return false;}
 return add_fixed_loot_impl(id,*slot.value,true,s,nullptr,nullptr,nullptr,e);
}
bool FreshInventoryOwnedV4::add_fixed_loot(std::int32_t id,const OwnedInventoryServicesV4& s,std::string& e){
 std::unique_ptr<ItemInstanceV1> temporary;bool ok;try{ok=add_fixed_loot_impl(id,temporary,false,s,nullptr,nullptr,nullptr,e);}catch(...){if(temporary)observe(s,OwnedInventoryOperationV4::destroy_item,0,temporary.get());throw;}
 if(temporary)observe(s,OwnedInventoryOperationV4::destroy_item,0,temporary.get());return ok;
}
bool FreshInventoryOwnedV4::add_fixed_loot(std::int32_t id,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,const OwnedLootEffectsV7& effects,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,nullptr,e))return false;if(*slot.value){e="Fixed loot incoming slot already owns an item";return false;}
 return add_fixed_loot_impl(id,*slot.value,true,s,&effects,nullptr,nullptr,e);
}
bool FreshInventoryOwnedV4::add_fixed_loot(std::int32_t id,const OwnedInventoryServicesV4& s,const OwnedLootEffectsV7& effects,std::string& e){
 std::unique_ptr<ItemInstanceV1> temporary;bool ok;try{ok=add_fixed_loot_impl(id,temporary,false,s,&effects,nullptr,nullptr,e);}catch(...){if(temporary)observe(s,OwnedInventoryOperationV4::destroy_item,0,temporary.get());throw;}
 if(temporary)observe(s,OwnedInventoryOperationV4::destroy_item,0,temporary.get());return ok;
}
bool FreshInventoryOwnedV4::add_loot_table(std::int32_t id,const LootEntrySelectionContextV1& selection,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,const OwnedLootEffectsV7& effects,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,nullptr,e))return false;if(*slot.value){e="Loot table incoming slot already owns an item";return false;}
 return add_fixed_loot_impl(id,*slot.value,true,s,&effects,&selection,nullptr,e);
}
bool FreshInventoryOwnedV4::add_world_loot_table(std::int32_t id,const LootEntrySelectionContextV1& selection,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,const OwnedLootEffectsV7& effects,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,nullptr,e))return false;if(*slot.value){e="World loot incoming slot already owns an item";return false;}
 return add_fixed_loot_impl(id,*slot.value,true,s,&effects,&selection,&world_items_,e);
}
bool FreshInventoryOwnedV4::pickup_world_item(std::size_t index,std::int32_t& inventory_index,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e)||index>=world_items_.size()||!world_items_[index]||!world_items_[index]->item){e="Invalid retained world item pickup";return false;}
 const auto* info=metadata(world_items_[index]->item.get(),e);if(!info)return false;
 if(item_type(*info)==14&&num_potions()>=std::max<int>(0,potion_capacity_)){e="Source ItemObject::Interact leaves a full potion drop in the world";return false;}
 // Source Interact checks inventory capacity only for item rows with an
 // equipment slot; slotless items transfer directly, while potions use the
 // separate capacity check above.
 if(info->record.words[26]!=-1){bool full=false;if(!inventory_full(full,s,e))return false;if(full){e="Source ItemObject::Interact leaves a full-inventory drop in the world";return false;}}
 auto* source=world_items_[index]->item.get();const auto source_id=source->id;ItemInstanceV1* merge_target=nullptr;std::uint16_t merge_quantity=0;
 if(item_type(*info)!=13&&std::uint8_t(info->record.words[7])){std::uint32_t candidate=0;bool found=false;if(!has_like(source,candidate,found,e))return false;if(found){merge_target=items_[candidate]->item.get();merge_quantity=merge_target->quantity;}}
 const auto old_gold=gold_;auto* old_potion=potion_;bool accepted=false;
 try{accepted=add_item(world_items_[index]->item,false,true,inventory_index,s,e);}
 catch(const std::exception& x){e=x.what();}catch(...){e="World item pickup callback threw";}
 if(!world_items_[index]->item){
  if(!accepted&&inventory_index<0)for(std::size_t i=0;i<items_.size();++i)if(items_[i]&&items_[i]->item.get()==source){inventory_index=std::int32_t(i);break;}
  world_items_.erase(world_items_.begin()+std::ptrdiff_t(index));
  if(!accepted)return false;
  if(s.after_world_pickup){
   ++callback_depth_;struct Guard{std::uint32_t& depth;~Guard(){--depth;}}guard{callback_depth_};
   try{if(!s.after_world_pickup(s.context,character_,source_id,e)){if(e.empty())e="Source ItemObject::Interact quest tail failed after transfer";return false;}}
   catch(const std::exception& x){e=x.what();return false;}
   catch(...){e="Source ItemObject::Interact quest tail threw after transfer";return false;}
  }
  e.clear();return true;
 }
 // A failed gold notification or stack-retirement callback must not leave a
 // retryable world item after applying its value/quantity to inventory.
 gold_=old_gold;potion_=old_potion;if(merge_target)merge_target->quantity=merge_quantity;
 if(accepted)e="Source AddItem reported success without transferring the world item";return false;
}
WorldItemTransferResultV4 FreshInventoryOwnedV4::transfer_world_item_for_auto_transmute(
    std::size_t index,std::int32_t& inventory_index,std::uintptr_t& destination_identity,
    const OwnedInventoryServicesV4& s,std::string& e){
 inventory_index=-1;destination_identity=0;
 if(!mutation_allowed(e)||index>=world_items_.size()||!world_items_[index]||!world_items_[index]->item){
  e="Invalid retained world Item AutoTransmute transfer";return WorldItemTransferResultV4::not_applied;
 }
 const auto* info=metadata(world_items_[index]->item.get(),e);if(!info)return WorldItemTransferResultV4::not_applied;
 if(item_type(*info)==14){e="Source AutoTransmute branch excludes potion Items";return WorldItemTransferResultV4::not_applied;}
 auto* source=world_items_[index]->item.get();
 ItemInstanceV1* merge_target=nullptr;std::uint16_t merge_quantity=0;
 if(item_type(*info)!=13&&std::uint8_t(info->record.words[7])){
  std::uint32_t candidate=0;bool found=false;if(!has_like(source,candidate,found,e))return WorldItemTransferResultV4::not_applied;
  if(found){merge_target=items_[candidate]->item.get();merge_quantity=merge_target->quantity;}
 }
 const auto old_gold=gold_;auto* old_potion=potion_;bool accepted=false;
 try{accepted=add_item(world_items_[index]->item,false,true,inventory_index,s,e);}
 catch(const std::exception& x){e=x.what();}catch(...){e="AutoTransmute V4 Item transfer callback threw";}
 if(!world_items_[index]->item){
  if(!accepted&&inventory_index<0)for(std::size_t i=0;i<items_.size();++i)
   if(items_[i]&&items_[i]->item.get()==source){inventory_index=std::int32_t(i);break;}
  if(inventory_index>=0&&std::size_t(inventory_index)<items_.size()&&items_[std::size_t(inventory_index)]&&items_[std::size_t(inventory_index)]->item)
   destination_identity=reinterpret_cast<std::uintptr_t>(items_[std::size_t(inventory_index)]->item.get());
  world_items_.erase(world_items_.begin()+std::ptrdiff_t(index));
  if(!accepted||!destination_identity){if(e.empty())e="AutoTransmute transfer changed ownership without a retained destination Item";return WorldItemTransferResultV4::indeterminate;}
  e.clear();return WorldItemTransferResultV4::committed;
 }
 // AddItem rejected before source ownership moved: restore any synchronous
 // gold/merge prefix just as the normal pickup owner does, leaving the world
 // Item available for ordinary pickup.
 gold_=old_gold;potion_=old_potion;if(merge_target)merge_target->quantity=merge_quantity;
 if(accepted)e="AutoTransmute AddItem reported success without transferring its source Item";
 return WorldItemTransferResultV4::not_applied;
}
bool FreshInventoryOwnedV4::drop_inventory_item_offline(std::uint32_t index,const OwnedInventoryServicesV4& inventory_services,const OfflineWorldItemDropServicesV4& drop_services,std::int32_t& world_index,std::string& e){
 world_index=-1;if(!mutation_allowed(e))return false;
 if(!drop_services.is_online){e="Required source online-session query unavailable for inventory drop";return false;}
 bool online=false;bool queried=false;
 try{++callback_depth_;struct Guard{std::uint32_t& depth;~Guard(){--depth;}}guard{callback_depth_};queried=drop_services.is_online(drop_services.context,online,e);}catch(const std::exception& x){e=x.what();}catch(...){e="Source online-session query threw during inventory drop";}
 if(!queried){if(e.empty())e="Source online-session query failed during inventory drop";return false;}
 if(online){e="Online inventory drop is unsupported until the source CMsgDropLoot provider is connected";return false;}
 if(index>=items_.size()||!items_[index]||!items_[index]->item){e="Inventory drop source index is outside the retained Item domain";return false;}
 auto* source=items_[index].get();auto* identity=source->item.get();const auto available=identity->signed_quantity();
 if(available<0){e="Negative source Item quantity is outside the inventory-drop domain";return false;}
 if(available==0){e.clear();return true;}
 if(!drop_services.spawn_and_lock){e="Required source scatter/ItemManager/player-lock provider unavailable for offline inventory drop";return false;}
 if(world_items_.size()>=std::size_t(INT32_MAX)){e="Retained world-item index exceeds source result domain";return false;}
 world_items_.reserve(world_items_.size()+1);
 if(available>1){
  auto dropped=std::make_unique<OwnedItemSlotV4>();
  if(!split_item(*identity,1,RetainedItemSlotV4{&dropped->item},inventory_services,e))return false;
  if(!dropped->item){e="Source TransferItemTo split did not produce the requested one-item transfer";return false;}
  world_items_.push_back(std::move(dropped));
 }else{
  // TransferItemTo first clears both equipment sets through the same owner;
  // UnEquipSlot may consume a stack into an existing stack, so re-find by the
  // exact Item/cell identity after each callback-capable operation.
  for(std::uint32_t set=0;set<2;++set){
   auto found=std::find_if(items_.begin(),items_.end(),[&](const auto& cell){return cell.get()==source;});
   if(found==items_.end()||!source->item||source->item.get()!=identity){e="Source TransferItemTo unequip consumed its Item before world transfer";return false;}
   const auto slot=source->slots[set];
   if(slot==-1)continue;
   if(slot<0||slot>=9){e="Invalid source equipment slot byte during world transfer";return false;}
   if(!unequip_from_slot(std::uint32_t(slot),std::int32_t(set),inventory_services,e))return false;
  }
  auto found=std::find_if(items_.begin(),items_.end(),[&](const auto& cell){return cell.get()==source;});
  if(found==items_.end()||!source->item||source->item.get()!=identity){e="Source TransferItemTo unequip consumed its Item before world transfer";return false;}
  if(potion_==identity)potion_=nullptr;
  const auto source_index=std::size_t(found-items_.begin());
  world_items_.push_back(std::move(items_[source_index]));
  items_.erase(items_.begin()+std::ptrdiff_t(source_index));
 }
 world_index=std::int32_t(world_items_.size()-1);
 auto* dropped=world_items_[std::size_t(world_index)]->item.get();
 bool spawned=false;
 try{++callback_depth_;struct Guard{std::uint32_t& depth;~Guard(){--depth;}}guard{callback_depth_};spawned=drop_services.spawn_and_lock(drop_services.context,*this,std::size_t(world_index),dropped,e);}catch(const std::exception& x){e=x.what();}catch(...){e="Source offline world-drop provider threw after Item transfer";}
 if(!spawned){if(e.empty())e="Source offline world-drop provider failed after Item transfer";return false;}
 e.clear();return true;
}
bool FreshInventoryOwnedV4::retire_world_item(std::size_t index,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e)||index>=world_items_.size()||!world_items_[index]||!world_items_[index]->item){e="Invalid retained world item retirement";return false;}
 if(!s.observe_storage){e="Retained world Item retirement provider unavailable";return false;}
 try{observe(s,OwnedInventoryOperationV4::destroy_item,0,world_items_[index]->item.get());}catch(const std::exception& x){e=x.what();return false;}catch(...){e="World Item retirement provider threw";return false;}
 world_items_[index]->item.reset();
 world_items_.erase(world_items_.begin()+std::ptrdiff_t(index));e.clear();return true;
}
std::int32_t FreshInventoryOwnedV4::num_potions()const noexcept{std::int16_t value=0;if(potion_)std::memcpy(&value,&potion_->quantity,2);return value;}

namespace {
std::int32_t wrap32(std::uint32_t v){std::int32_t n;std::memcpy(&n,&v,4);return n;}
}
const Item* FreshInventoryOwnedV4::metadata(const ItemInstanceV1* instance,std::string& e)const{auto* p=instance?item(tables_.items(),instance->id):nullptr;if(!p)e="Item index requires original assertion continuation";return p;}
std::int32_t FreshInventoryOwnedV4::set_for_slot(std::int32_t slot)const noexcept{return slot<0||slot==1||slot==2?selected_:0;}
std::int8_t& FreshInventoryOwnedV4::slot_for_set(OwnedItemSlotV4& slot,std::uint32_t set)noexcept{return slot.slots[set];}
bool FreshInventoryOwnedV4::inventory_full(bool& out,const OwnedInventoryServicesV4& s,std::string& e){return is_full(out,s,e);}
bool FreshInventoryOwnedV4::is_full(bool& full,const OwnedInventoryServicesV4& s,std::string& e){OwnedInventoryResponseV4 r;if(!deliver(s,OwnedInventoryOperationV4::debug_load,0x3fe364,nullptr,nullptr,0,0,r,e)||!deliver(s,OwnedInventoryOperationV4::debug_query,0x3fe3a8,nullptr,"InfiniteInventory",0,0,r,e))return false;full=!r.value&&!unlimited_&&items_.size()>99;return true;}
bool FreshInventoryOwnedV4::destroy(std::unique_ptr<ItemInstanceV1>& p,const OwnedInventoryServicesV4& s,std::uint32_t caller,std::string& e){if(!p){e="Null source item destruction";return false;}if(!s.observe_storage&&!s.stateless_temporaries){e="Required Item retirement provider unavailable at "+std::to_string(caller);return false;}observe(s,OwnedInventoryOperationV4::destroy_item,caller,p.get());p.reset();return true;}
bool FreshInventoryOwnedV4::equal(const ItemInstanceV1& a,const ItemInstanceV1& b)noexcept{return a.id==b.id&&a.powers==b.powers;}
bool FreshInventoryOwnedV4::add_quantity(ItemInstanceV1& p,std::int32_t n,std::string& e){auto q=wrap32(std::uint32_t(p.signed_quantity())+std::uint32_t(n));if(q==-2)q=99;if(q<0){e="Negative SetQty requires original assertion continuation";return false;}p.quantity=std::uint16_t(q);return true;}
bool FreshInventoryOwnedV4::lifetime_slot(RetainedItemSlotV4 slot,const ItemInstanceV1* original,std::string& e)const{
 auto at=reinterpret_cast<std::uintptr_t>(slot.value);if(!slot.value||at%alignof(std::unique_ptr<ItemInstanceV1>)||at>UINTPTR_MAX-sizeof(*slot.value)){e="Invalid borrowed Item lifetime slot";return false;}
 auto overlaps=[&](const void* p,std::size_t n){auto q=reinterpret_cast<std::uintptr_t>(p);return p&&q<=UINTPTR_MAX-n&&at<q+n&&q<at+sizeof(*slot.value);};
 if(overlaps(this,sizeof(*this))||overlaps(properties_,sizeof(*properties_))||(original&&overlaps(original,sizeof(*original)))){e="Item lifetime slot aliases an owner";return false;}
 for(const auto& cell:items_)if(overlaps(cell.get(),sizeof(*cell))||overlaps(cell->item.get(),sizeof(*cell->item))){e="Item lifetime slot aliases retained inventory storage";return false;}
 for(const auto& cell:world_items_)if(overlaps(cell.get(),sizeof(*cell))||overlaps(cell->item.get(),sizeof(*cell->item))){e="Item lifetime slot aliases retained world-item storage";return false;}
 return true;
}
bool FreshInventoryOwnedV4::create_item(std::int32_t id,std::uint32_t quantity,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,nullptr,e))return false;auto& out=*slot.value;if(out||!item(tables_.items(),id)){e="Malformed constructor output or source item index";return false;}
 out=std::make_unique<ItemInstanceV1>();out->id=id;out->quantity=std::uint16_t(quantity);auto* actual=out.get();OwnedInventoryResponseV4 r;
 for(auto op:{OwnedInventoryOperationV4::update_name,OwnedInventoryOperationV4::update_stats,OwnedInventoryOperationV4::update_requirements}){
  if(!deliver(s,op,op==OwnedInventoryOperationV4::update_name?0x3fc36c:op==OwnedInventoryOperationV4::update_stats?0x3fc374:0x3fc37c,actual,nullptr,0,0,r,e))return false;
  if(out.get()!=actual){e="Constructor callback changed retained Item identity";return false;}
 }e.clear();return true;
}
bool FreshInventoryOwnedV4::create_item(std::int32_t id,std::uint32_t quantity,std::unique_ptr<ItemInstanceV1>& out,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e))return false;if(out||!item(tables_.items(),id)){e="Malformed constructor output or source item index";return false;}
 if(!s.stateless_temporaries){e="Constructor requires retained Item slot or explicit stateless temporary callbacks";return false;}
 std::unique_ptr<ItemInstanceV1> temporary;
 bool ok;try{ok=create_item(id,quantity,RetainedItemSlotV4{&temporary},s,e);}
 catch(...){if(temporary)observe(s,OwnedInventoryOperationV4::destroy_item,0,temporary.get());throw;}
 if(!ok){if(temporary)observe(s,OwnedInventoryOperationV4::destroy_item,0,temporary.get());return false;}out=std::move(temporary);return true;
}
bool FreshInventoryOwnedV4::retire_item(RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,nullptr,e))return false;if(!*slot.value){e="Null retained Item retirement";return false;}
 if(!s.observe_storage){e="Retained Item retirement provider unavailable";return false;}
 try{observe(s,OwnedInventoryOperationV4::destroy_item,0,slot.value->get());}catch(const std::exception& x){e=x.what();return false;}catch(...){e="Retained Item retirement provider threw";return false;}
 slot.value->reset();e.clear();return true;
}
bool FreshInventoryOwnedV4::saved_item_effect(OwnedInventoryOperationV4 op,std::uint32_t at,ItemInstanceV1* p,std::int32_t argument,std::uint32_t index,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;if(!p||(op!=OwnedInventoryOperationV4::update_name&&op!=OwnedInventoryOperationV4::add_power)){e="Invalid direct saved Item effect";return false;}OwnedInventoryResponseV4 out;return deliver(s,op,at,p,nullptr,argument,index,out,e);}
bool FreshInventoryOwnedV4::split_item(ItemInstanceV1& original,std::int32_t amount,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e)||!lifetime_slot(slot,&original,e))return false;auto& out=*slot.value;if(out){e="Split output already owns an item";return false;}auto* info=metadata(&original,e);if(!info)return false;
 if(!std::uint8_t(info->record.words[7])||amount<=0||amount>=original.signed_quantity()){e.clear();return true;}
 if(!add_quantity(original,-amount,e)||!create_item(original.id,std::uint32_t(amount),slot,s,e))return false;auto* actual=out.get();out->value=original.value;OwnedInventoryResponseV4 r;
 if(!deliver(s,OwnedInventoryOperationV4::update_name,0x3fc44c,actual,nullptr,0,0,r,e))return false;if(out.get()!=actual){e="Split SetValue changed retained Item identity";return false;}
 out->identified=0;for(std::size_t i=0;i<original.powers.size();++i){if(i>65536){e="Source power iteration exceeds owned budget";return false;}auto id=original.powers[i];if(!deliver(s,OwnedInventoryOperationV4::add_power,0x3fc468,actual,nullptr,id,UINT32_MAX,r,e))return false;if(out.get()!=actual){e="Split AddPower changed retained Item identity";return false;}}
 out->identified=original.identified;e.clear();return true;
}
bool FreshInventoryOwnedV4::split_item(ItemInstanceV1& original,std::int32_t amount,std::unique_ptr<ItemInstanceV1>& out,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e))return false;if(out){e="Split output already owns an item";return false;}auto* info=metadata(&original,e);if(!info)return false;if(!std::uint8_t(info->record.words[7])||amount<=0||amount>=original.signed_quantity()){e.clear();return true;}
 if(!s.stateless_temporaries){e="Split requires retained Item slot or explicit stateless temporary callbacks";return false;}
 return split_item(original,amount,RetainedItemSlotV4{&out},s,e);
}
bool FreshInventoryOwnedV4::is_equipped(std::uint32_t index,bool& out,std::string& e)const{if(index>=items_.size()||!items_[index]||!items_[index]->item){e="Equipped query requires original valid slot";return false;}auto* p=metadata(items_[index]->item.get(),e);if(!p)return false;auto slot=p->record.words[26];auto set=slot>=0||slot<-4?set_for_slot(slot):slot==-2?set_for_slot(5):slot==-1?set_for_slot(-1):set_for_slot(1);out=items_[index]->slots[set]!=-1;e.clear();return true;}
bool FreshInventoryOwnedV4::has_like(const ItemInstanceV1* p,std::uint32_t& index,bool& found,std::string& e)const{found=false;for(std::size_t i=0;i<items_.size();++i){auto* candidate=items_[i]->item.get();if(candidate&&candidate!=p&&equal(*candidate,*p)){bool equipped;if(!is_equipped(std::uint32_t(i),equipped,e))return false;if(!equipped){index=std::uint32_t(i);found=true;break;}}}return true;}
bool FreshInventoryOwnedV4::delete_instance(ItemInstanceV1* p,const OwnedInventoryServicesV4& s,std::string& e,std::uint32_t source_caller){if(potion_==p)potion_=nullptr;for(std::size_t i=0;i<items_.size();++i)if(items_[i]->item.get()==p){for(const auto& set:equipment_)for(auto* cell:set)if(cell==items_[i].get()){e="Source invalid deletion of a still-equipped slot";return false;}if(!s.observe_storage&&!s.stateless_temporaries){e="Required Item retirement provider unavailable at "+std::to_string(source_caller);return false;}observe(s,OwnedInventoryOperationV4::destroy_item,source_caller,p,0,std::uint32_t(i));items_.erase(items_.begin()+std::ptrdiff_t(i));e.clear();return true;}e.clear();return true;}
bool FreshInventoryOwnedV4::add_quantity_to_item(ItemInstanceV1& instance,std::int32_t amount,std::string& e){
 if(!mutation_allowed(e))return false;
 const auto found=std::any_of(items_.begin(),items_.end(),[&](const auto& slot){return slot&&slot->item.get()==&instance;});
 if(!found){e="Quantity item does not belong to this inventory";return false;}
 return add_quantity(instance,amount,e);
}
bool FreshInventoryOwnedV4::set_item_buyback(std::uint32_t index,bool buyback,std::string& e){
 if(!mutation_allowed(e))return false;
 if(running_||index>=items_.size()||!items_[index]||!items_[index]->item){
  e="Merchant buyback write requires an idle live ItemInventory index";return false;
 }
 items_[index]->item->buyback=std::uint8_t(buyback);e.clear();return true;
}
bool FreshInventoryOwnedV4::remove_inventory_item(std::uint32_t index,const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e))return false;
 if(index>=items_.size()||!items_[index]||!items_[index]->item){e="RemoveItem assertion domain unsupported";return false;}
 auto* cell=items_[index].get();auto* instance=cell->item.get();auto* info=metadata(instance,e);if(!info)return false;
 auto target=info->record.words[26];if(target==-4||target==-3)target=1;else if(target==-2)target=5;
 const auto original_selection=selected_;
 auto clear_selected=[&](){
  bool equipped;if(!is_equipped(index,equipped,e))return false;if(!equipped)return true;
  const auto set=(target<0||target==1||target==2)?std::uint32_t(selected_):0u;
  const auto slot=cell->slots[set];
  if(slot<0||slot>=9){e="Invalid RemoveItem equipment slot byte";return false;}
  equipment_[set][std::uint32_t(slot)]=nullptr;
  return true;
 };
 if(!clear_selected())return false;
 selected_=std::uint8_t(original_selection^1u);
 const bool second_cleared=clear_selected();
 selected_=original_selection;
 if(!second_cleared)return false;
 // RemoveItem clears the potion alias and then retires the source Item. The
 // observer must succeed while that exact Item remains alive; only then does
 // delete_instance erase its owning slot.
 if(potion_==instance)potion_=nullptr;
 return delete_instance(instance,s,e,0x3fe558);
}
bool FreshInventoryOwnedV4::remove_one_potion(const OwnedInventoryServicesV4& s,std::string& e){
 if(!mutation_allowed(e))return false;
 auto* instance=potion_;
 if(!instance){e.clear();return true;}
 const auto found=std::find_if(items_.begin(),items_.end(),[&](const auto& slot){return slot&&slot->item.get()==instance;});
 if(found==items_.end()){e="Source potion alias does not belong to this inventory";return false;}
 if(instance->signed_quantity()>1)return add_quantity_to_item(*instance,-1,e);
 // _DelItemInstance clears the potion alias before synchronously destroying
 // and erasing the exact retained Item. It does not route through Character's
 // higher-level RemoveItem equipment callbacks.
 return delete_instance(instance,s,e,0x40e878);
}
bool FreshInventoryOwnedV4::set_gold(std::int32_t value,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;if(value<0){e="Negative SetGold requires original assertion continuation";return false;}gold_=value<=gold_limit_?value:gold_limit_;OwnedInventoryResponseV4 out;if(!deliver(s,OwnedInventoryOperationV4::gold_notifications,0x3fdfd8,nullptr,nullptr,0,0,out,e))return false;e.clear();return true;}
bool FreshInventoryOwnedV4::update_localization(ItemPresentationOwnerV5& presentation,const ItemTextServicesV5& text,std::string& e){
 e.clear();if(callback_depth_||running_){e="Inventory localization refresh during active mutation unsupported";return false;}
 if(items_.size()>65536){e="Source inventory localization iteration exceeds owned budget";return false;}
 ++callback_depth_;struct Guard{std::uint32_t& n;~Guard(){--n;}}guard{callback_depth_};
 for(const auto& slot:items_){if(!slot||!slot->item){e="Source ItemInventory localization encountered an invalid dense slot";return false;}if(!presentation.update_localization(*slot->item,text,e))return false;}
 return true;
}
bool FreshInventoryOwnedV4::add_gold(std::int32_t amount,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;if(amount<0){auto neg=wrap32(0u-std::uint32_t(amount));if(gold_<neg)amount=wrap32(0u-std::uint32_t(gold_));}if(amount>0){auto room=wrap32(std::uint32_t(gold_limit_)-std::uint32_t(gold_));if(amount>room)amount=room<0?0:room;}return set_gold(wrap32(std::uint32_t(gold_)+std::uint32_t(amount)),s,e);}
bool FreshInventoryOwnedV4::add_item(std::unique_ptr<ItemInstanceV1>& incoming,bool force,bool convert_gold,std::int32_t& index,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;auto* info=metadata(incoming.get(),e);if(!info)return false;auto* p=incoming.get();index=-1;if(potion_capacity_==0&&item_type(*info)==14)return destroy(incoming,s,0x3ff70c,e);if(!potion_&&item_type(*info)==14)potion_=p;if(convert_gold&&item_type(*info)==13){if(!add_gold(p->value,s,e))return false;return destroy(incoming,s,0x3ff744,e);}if(std::uint8_t(info->record.words[7])&&!force){for(std::size_t i=0;i<items_.size();++i){auto* candidate=items_[i]->item.get();if(!candidate)continue;bool equipped;if(!is_equipped(std::uint32_t(i),equipped,e))return false;if(equipped||!equal(*candidate,*p))continue;auto qty=p->signed_quantity();info=metadata(p,e);if(!info)return false;if(item_type(*info)==14){auto room=wrap32(std::uint32_t(std::int32_t(potion_capacity_))-std::uint32_t(candidate->signed_quantity()));if(room<qty)qty=room<0?0:room;}if(!add_quantity(*candidate,qty,e))return false;index=std::int32_t(i);return destroy(incoming,s,0x3ff794,e);}}auto slot=std::make_unique<OwnedItemSlotV4>();slot->item=std::move(incoming);items_.push_back(std::move(slot));auto i=std::uint32_t(items_.size()-1);bool full;observe(s,OwnedInventoryOperationV4::inventory_full,0x3ff6c4,p,0,i);if(!is_full(full,s,e))return false;if(full){OwnedInventoryResponseV4 r;if(!deliver(s,OwnedInventoryOperationV4::full_notifications,0x3ff7a8,p,nullptr,0,i,r,e))return false;}index=std::int32_t(items_.size()-1);e.clear();return true;}
OwnedItemTransferResultV4 FreshInventoryOwnedV4::transfer_item_to(
    std::uint32_t index,std::int32_t amount,FreshInventoryOwnedV4& destination,
    bool force,bool convert_gold,std::int32_t& destination_index,
    const OwnedInventoryServicesV4& source_services,
    const OwnedInventoryServicesV4& destination_services,
    RetainedItemSlotV4 retained,std::string& e){
 e.clear();destination_index=-1;
 if(this==&destination||!mutation_allowed(e)||!destination.mutation_allowed(e)||
    !lifetime_slot(retained,nullptr,e)||!destination.lifetime_slot(retained,nullptr,e)||
    !retained.value||*retained.value||amount<=0||
    index>=items_.size()||!items_[index]||!items_[index]->item){
  if(e.empty())e="Invalid or aliased TransferItemTo owners/output";
  return OwnedItemTransferResultV4::not_applied;
 }
 auto* source_slot=items_[index].get();auto* instance=source_slot->item.get();
 auto* info=metadata(instance,e);if(!info)return OwnedItemTransferResultV4::not_applied;
 const auto quantity=instance->signed_quantity();
 if(quantity<=0){e="TransferItemTo source has non-positive quantity";return OwnedItemTransferResultV4::not_applied;}
 // Destination AddItemInstance performs its own metadata lookup. Preflight its
 // immutable ItemTable so invalid cross-table identities cannot detach source.
 if(!item(destination.tables_.items(),instance->id)){
  e="TransferItemTo destination ItemTable lacks source item ID";
  return OwnedItemTransferResultV4::not_applied;
 }
 const auto move_all=amount>=quantity;
 if(!move_all&&!std::uint8_t(info->record.words[7])){
  e="Partial TransferItemTo requires the source stackable-item Split path";
  return OwnedItemTransferResultV4::not_applied;
 }
 // TransferItemTo is not Character::RemoveItem: it detaches the item object
 // from this dense inventory without retiring its shared presentation state.
 if(move_all){
  *retained.value=std::move(source_slot->item);
  for(auto& set:equipment_)for(auto*& equipped:set)if(equipped==source_slot)equipped=nullptr;
  source_slot->slots={{-1,-1}};
  if(potion_==instance)potion_=nullptr;
  items_.erase(items_.begin()+std::ptrdiff_t(index));
 }else{
  try{
   if(!split_item(*instance,amount,retained,source_services,e))
    return OwnedItemTransferResultV4::indeterminate;
  }catch(const std::exception& x){e=x.what();return OwnedItemTransferResultV4::indeterminate;}
  catch(...){e="TransferItemTo source Split callback threw";return OwnedItemTransferResultV4::indeterminate;}
  if(!*retained.value){e="TransferItemTo source Split produced no retained Item";return OwnedItemTransferResultV4::indeterminate;}
 }
 auto* moved=retained.value->get();
 if(!item(destination.tables_.items(),moved->id)){
  e="TransferItemTo moved Item ID changed outside its source ItemTable";
  return OwnedItemTransferResultV4::indeterminate;
 }
 try{
  if(!destination.add_item(*retained.value,force,convert_gold,destination_index,
                           destination_services,e))
   return OwnedItemTransferResultV4::indeterminate;
 }catch(const std::exception& x){e=x.what();return OwnedItemTransferResultV4::indeterminate;}
 catch(...){e="TransferItemTo destination AddItem callback threw";return OwnedItemTransferResultV4::indeterminate;}
 if(*retained.value){e="TransferItemTo destination accepted without consuming Item";return OwnedItemTransferResultV4::indeterminate;}
 e.clear();return OwnedItemTransferResultV4::committed;
}
bool FreshInventoryOwnedV4::has_two_hander(bool ignore,bool& out,std::string& e)const{out=false;auto* slot=equipment_[set_for_slot(1)][1];if(!slot){e.clear();return true;}auto* p=metadata(slot->item.get(),e);if(!p)return false;auto type=item_type(*p);auto slotting=p->record.words[26];out=(std::uint32_t(type)-4<=1||ignore)?slotting==-4:slotting==-4&&properties_->resolved[203]==0;e.clear();return true;}
bool FreshInventoryOwnedV4::unequip_from_slot(std::uint32_t slot,std::int32_t selected,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;if(slot>=9||selected<-1||selected>1){e="UnEquip assertion domain unsupported";return false;}auto active=selected==-1?set_for_slot(std::int32_t(slot)):selected;auto* old=equipment_[active][slot];equipment_[active][slot]=nullptr;if(!old){e.clear();return true;}old->slots[active]=-1;if(old->slots[0]!=-1||old->slots[1]!=-1){e.clear();return true;}auto* info=metadata(old->item.get(),e);if(!info)return false;if(!std::uint8_t(info->record.words[7])){e.clear();return true;}std::uint32_t index=0;bool found;if(!has_like(old->item.get(),index,found,e))return false;if(!found){e.clear();return true;}bool equipped;if(!is_equipped(index,equipped,e))return false;if(equipped){e.clear();return true;}if(!add_quantity(*items_[index]->item,old->item->signed_quantity(),e))return false;return delete_instance(old->item.get(),s,e);}
bool FreshInventoryOwnedV4::equip_to_slot_impl(std::uint32_t requested,std::uint32_t index,bool forced,std::unique_ptr<ItemInstanceV1>& remainder,bool retained,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;if(requested>=9||index>=items_.size()){e="Equip assertion domain unsupported";return false;}auto* slot=items_[index].get();if(!slot||!slot->item){e.clear();return true;}auto active=set_for_slot(std::int32_t(requested));auto* info=metadata(slot->item.get(),e);if(!info)return false;auto slotting=info->record.words[26];auto type=item_type(*info);if(type!=5&&type!=4&&slotting==-4&&properties_->resolved[203])slotting=1;if(info->record.words[26]==-1){e.clear();return true;}auto old=slot->slots[active];if(old==std::int32_t(requested)&&equipment_[active][requested]==slot){e.clear();return true;}if(!unequip_from_slot(requested,-1,s,e))return false;old=slot->slots[active];if(old!=-1){if(old<0||old>=9){e="Invalid live equipment slot byte";return false;}if(equipment_[active][std::uint32_t(old)]==slot&&!unequip_from_slot(std::uint32_t(old),-1,s,e))return false;}auto target=requested;if(slotting==-4){if(!forced&&!unequip_from_slot(2,-1,s,e))return false;if(!unequip_from_slot(1,-1,s,e))return false;target=1;}else if(requested==2){bool two;if(!has_two_hander(false,two,e))return false;if(two&&!forced&&!unequip_from_slot(1,-1,s,e))return false;}if(slot->item->signed_quantity()!=1){if(!(retained?split_item(*slot->item,slot->item->signed_quantity()-1,RetainedItemSlotV4{&remainder},s,e):split_item(*slot->item,slot->item->signed_quantity()-1,remainder,s,e)))return false;if(!remainder){e="Source Equip split failed required assertion continuation";return false;}}equipment_[active][target]=slot;equipment_[active][target]->slots[active]=std::int8_t(target);if(remainder){std::int32_t added;return add_item(remainder,true,true,added,s,e);}e.clear();return true;}
bool FreshInventoryOwnedV4::equip_to_slot(std::uint32_t requested,std::uint32_t index,bool forced,RetainedItemSlotV4 slot,const OwnedInventoryServicesV4& s,std::string& e){if(!lifetime_slot(slot,nullptr,e))return false;if(*slot.value){e="Equip remainder already owns an item";return false;}return equip_to_slot_impl(requested,index,forced,*slot.value,true,s,e);}
bool FreshInventoryOwnedV4::equip_to_slot(std::uint32_t requested,std::uint32_t index,bool forced,const OwnedInventoryServicesV4& s,std::string& e){std::unique_ptr<ItemInstanceV1> remainder;bool ok;try{ok=equip_to_slot_impl(requested,index,forced,remainder,false,s,e);}catch(...){if(remainder)observe(s,OwnedInventoryOperationV4::destroy_item,0,remainder.get());throw;}if(remainder)observe(s,OwnedInventoryOperationV4::destroy_item,0,remainder.get());return ok;}
bool FreshInventoryOwnedV4::auto_equip(std::uint32_t index,std::int32_t& result,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;result=0;if(index>=items_.size()){e="AutoEquip assertion domain unsupported";return false;}auto* slot=items_[index].get();auto* info=metadata(slot->item.get(),e);if(!info)return false;auto slotting=info->record.words[26];if(slotting==-1){e.clear();return true;}auto type=item_type(*info);if(type!=5&&type!=4){if(slotting==1&&properties_->resolved[202])slotting=-3;else if(slotting==-4&&properties_->resolved[203])slotting=1;}if(slotting>=0&&slotting<9){if(slotting==2){bool two;if(!has_two_hander(false,two,e))return false;if(two&&!unequip_from_slot(1,-1,s,e))return false;}if(!equip_to_slot(std::uint32_t(slotting),index,false,s,e))return false;}else if(slotting==-3||slotting==-2){auto first=slotting==-3?1u:5u;auto second=first+1;auto chosen=first;if(equipment_[set_for_slot(std::int32_t(first))][first]){if(equipment_[set_for_slot(std::int32_t(second))][second]){e.clear();return true;}chosen=second;}if(!equip_to_slot(chosen,index,false,s,e))return false;}else if(slotting==-4){if(!unequip_from_slot(2,-1,s,e)||!equip_to_slot(1,index,false,s,e))return false;}else{e.clear();return true;}result=1;e.clear();return true;}
bool FreshInventoryOwnedV4::character_auto_equip(std::uint32_t index,std::int32_t& result,const OwnedInventoryServicesV4& s,std::string& e){if(!mutation_allowed(e))return false;std::int32_t captured;if(!auto_equip(index,captured,s,e))return false;OwnedInventoryResponseV4 r;for(auto op:{OwnedInventoryOperationV4::update_gear_properties,OwnedInventoryOperationV4::skin,OwnedInventoryOperationV4::validate_hp_mp})if(!deliver(s,op,op==OwnedInventoryOperationV4::update_gear_properties?0x3a9fc0:op==OwnedInventoryOperationV4::skin?0x3a9fc8:0x3a9fd0,nullptr,nullptr,0,0,r,e))return false;result=captured;e.clear();return true;}
}
