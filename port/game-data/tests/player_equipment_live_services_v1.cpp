#include "../player_equipment_live_services_v1.hpp"
#include "../player_equipment_queries_live_v1.hpp"
#include "../combat.hpp"
#include "../item_presentation_v5.hpp"
#include "../../engine-ui/deferred_hud_refresh_v1.hpp"
#include "../../android-native/app/src/main/cpp/player_inventory_save_commit_v1.hpp"
#include "../player_savegame_v1.hpp"
extern "C" {
#include "../../random/random.h"
}
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static std::string error;
#define ck(v) ((v)?++checks:throw std::runtime_error(std::string("Check ")+ #v + "; " + error))
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);ck(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct Reader {const Raw& b;std::size_t at{};void copy(void* p,std::size_t n){ck(n<=b.size()-at);std::memcpy(p,b.data()+at,n);at+=n;}std::uint32_t u(){std::uint32_t n;copy(&n,4);return n;}};
struct Context {
 FreshInventoryOwnedV4* inventory{};PropertyView* view{};PlayerEquipmentLiveServicesV1* adapter{};
 ItemPresentationOwnerV5* presentation{};
 ItemInstanceV1* incoming_destroy{};
 std::uintptr_t visual{};bool attached{true},reentry{},online{},remote{},fail_world{},fail_skin{},change_visual{},check_vitals{},fail_retirement{};
 unsigned text_fixture{},debug_fixture{},world_fixture{},skin_fixture{},binding_fixture{},destroyed{},reentries{};
 std::vector<EquipmentWorldQueryV1> queries;
 std::int32_t expected_hp{},expected_mp{};
 std::int32_t difficulty{};
};
static void reentry(Context& c){if(c.reentry&&c.adapter){auto selected=c.inventory->current_equipment();std::string e;ck(!c.adapter->swap(e)&&!e.empty()&&c.inventory->current_equipment()==selected);++c.reentries;}}
static bool binding(void* p,FreshInventoryOwnedV4& inventory,PropertyView& view,std::string& e){auto& c=*static_cast<Context*>(p);++c.binding_fixture;ck(&inventory==c.inventory&&&view==c.view);reentry(c);if(!c.attached){e="Explicit detached renderer fixture";return false;}return true;}
static bool world(void* p,EquipmentWorldQueryV1 q,std::uintptr_t character,std::uintptr_t& id,std::int32_t& value,std::string& e){auto& c=*static_cast<Context*>(p);ck(character==c.inventory->character());++c.world_fixture;c.queries.push_back(q);reentry(c);if(c.fail_world){e="Explicit world fixture rejection";return false;}id=0;value=0;if(q==EquipmentWorldQueryV1::online)value=c.online;if(q==EquipmentWorldQueryV1::remotely_updated)value=c.remote;if(q==EquipmentWorldQueryV1::current_player)id=character;if(q==EquipmentWorldQueryV1::current_difficulty)value=c.difficulty;if(q==EquipmentWorldQueryV1::player_count)value=1;return true;}
static bool required(void* p,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string& e){auto& c=*static_cast<Context*>(p);ck(&inventory==c.inventory);reentry(c);using O=OwnedInventoryOperationV4;
 if(q.operation==O::update_name||q.operation==O::update_stats||q.operation==O::update_requirements){++c.text_fixture;return true;}
 if(q.operation==O::debug_load||q.operation==O::debug_query){++c.debug_fixture;out={};return true;}
 e="Unimplemented required continuation in bounded equipment fixture";return false;
}
static void observe(void* p,FreshInventoryOwnedV4& inventory,const OwnedInventoryRequestV4& q){auto& c=*static_cast<Context*>(p);ck(&inventory==c.inventory);reentry(c);if(q.operation==OwnedInventoryOperationV4::destroy_item){ck(q.item);bool present=false;for(const auto& slot:inventory.items())if(slot->item.get()==q.item)present=true;ck(present||q.source_caller==0||(q.item==c.incoming_destroy&&q.source_caller==0x3ff794));if(c.fail_retirement)throw std::runtime_error("Explicit lifetime retirement fixture rejection");if(c.presentation){std::string e;ck(c.presentation->forget(*q.item,e)&&!c.presentation->powers(*q.item));}++c.destroyed;}}
static bool skin(void* p,FreshInventoryOwnedV4& inventory,const GearSkinRequestV5& q,std::int32_t& result,std::string& e){auto& c=*static_cast<Context*>(p);ck(&inventory==c.inventory);++c.skin_fixture;reentry(c);if(c.check_vitals)ck(c.view->resolved[36]==c.expected_hp&&c.view->resolved[41]==c.expected_mp);
 if(q.operation!=GearSkinOperationV5::debug_load&&q.operation!=GearSkinOperationV5::debug_query)ck(q.visual==c.visual);
 if(c.change_visual){c.visual+=16;c.change_visual=false;}
 if(c.fail_skin){e="Explicit Skin fixture rejection";return false;}result=0;return true;
}
static bool random_next(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& value,std::string&){value=dh2_random_next(static_cast<dh2_random_state*>(p),std::uint32_t(bound),stream);return true;}
static Damage attack_damage(const std::int32_t* properties,const std::int32_t* defender_properties,
                            std::int32_t main_category=-1){
 CombatantView attacker{properties,main_category,-1,0,0,0,0,0};
 CombatantView defender{defender_properties,-1,-1,0,0,0,0,0};
 CombatRandom random{0x12345,0};Damage damage{};
 DamageRequest request{&attacker,&defender,&random,0,0,0,0};
 if(dh2_combat_damage(&damage,&request))throw std::runtime_error("Live equipped-stat combat query rejected");
 return damage;
}
static EquipmentLiveHooksV1 hooks(Context& c){return {&c,binding,{&c,world},&c.visual,{&c,skin},{&c,required,observe}};}

int main(int argc,char** argv){try{
 ck(argc==4);auto cache=std::string(argv[3]);auto span=[](const Raw& b){return Bytes{b.data(),b.size()};};
 auto bytes=file(cache+"/loot_table_pyarray.bin"),names=file(cache+"/loot_table_pyarraynames.bin"),schema=file(cache+"/loot_table_pystructnames.bin");LootTablesV2 table;ck(table.load(span(bytes),span(names),span(schema),error));
 bytes=file(cache+"/item_powers_pyarray.bin");names=file(cache+"/item_powers_pyarraynames.bin");schema=file(cache+"/item_powers_pystructnames.bin");ItemPowerTablesV5 powers;ck(powers.load(span(bytes),span(names),span(schema),error));
 bytes=file(cache+"/character_properties_pyarray.bin");names=file(cache+"/character_properties_pyarraynames.bin");schema=file(cache+"/character_properties_pystructnames.bin");CharacterTable chars;ck(load_characters(span(bytes),span(names),span(schema),chars,error));PropertyRules rules;ck(load_property_rules(chars,rules,error));
 bytes=file(cache+"/character_classes_pyarray.bin");names=file(cache+"/character_classes_pyarraynames.bin");schema=file(cache+"/character_classes_pystructnames.bin");ClassTables classes;ck(load_classes(span(bytes),span(names),span(schema),classes,error));std::vector<ClassRow> rows;for(auto& row:classes.rows)rows.push_back({row.data(),std::uint32_t(row.size())});
 auto gold=file(argv[1]);Reader q{gold};ck(q.u()==0x31515245);auto requirement_cases=q.u();ck(q.u()==0);
 for(unsigned j=0;j<requirement_cases;++j){EquipmentRequirements32V1 facts;ItemRecord164 row;std::int32_t expected,actual;q.copy(&facts,sizeof facts);q.copy(&row,sizeof row);q.copy(&expected,4);ck(equipment_requirements_v1(&actual,&facts,facts.present?&row:nullptr)==0&&actual==expected);}ck(q.at==gold.size());
 std::int32_t out=123;EquipmentRequirements32V1 invalid{};invalid.present=2;ck(equipment_requirements_v1(&out,&invalid,nullptr)==-1&&out==123);invalid.present=0;ck(equipment_requirements_v1(nullptr,&invalid,nullptr)==-1);ck(equipment_requirements_v1(reinterpret_cast<std::int32_t*>(&invalid),&invalid,nullptr)==-1);invalid.present=1;ck(equipment_requirements_v1(&out,&invalid,reinterpret_cast<const ItemRecord164*>(UINTPTR_MAX-3))==-1&&out==123);
 auto starter=file(argv[2]);Reader r{starter};ck(r.u()==0x35564547);auto owner_cases=r.u();unsigned steps=0,pruned=0,live_buffs=0,vitals=0,prefixes=0,guards=4,skin_calls=0,reentries=0,direct_lifetime_rejections=0,direct_detached_rejections=0,gear_combat=0,armor_combat=0;
 for(unsigned k=0;k<owner_cases;++k){auto baseid=r.u(),selected=r.u(),count=r.u();PropertyState state;reset_properties(rules,state,&chars.rows.at(baseid));auto view=property_view(rules,state);ck(!dh2_class_recalc_base(rows.data(),rows.size(),state.base.data(),&view));PropertyState initial;r.copy(&initial,sizeof initial);ck(!std::memcmp(&state,&initial,sizeof state));const PropertyState unequipped=state;
  dh2_random_state random{{1,991},{0,17}};FreshInventoryOwnedV4 inventory(0x100000001ULL,table.borrow(),{&random,random_next},12,state);if(selected)inventory.swap_equipment();Context c;c.inventory=&inventory;c.view=&view;auto h=hooks(c);PlayerEquipmentLiveServicesV1 adapter(inventory,view,rows.data(),rows.size(),powers.borrow(),h);c.adapter=&adapter;auto services=adapter.services();std::unique_ptr<ItemInstanceV1> bulk_incoming;
  for(unsigned j=0;j<count;++j){auto op=r.u(),a=r.u(),b=r.u();r.u();auto expected=r.u();std::int32_t result=0;
   if(op==0)ck(inventory.add_fixed_loot(std::int32_t(a),RetainedItemSlotV4{&bulk_incoming},services,error));else if(op==2)ck(adapter.auto_equip(a,result,error));else if(op==5)inventory.swap_equipment();else if(op==4)ck(inventory.unequip_from_slot(a,std::int32_t(b),services,error));else if(op==15)ck(adapter.refresh(false,error));else ck(false);
   ck(std::uint32_t(result)==expected);PropertyState after;r.copy(&after,sizeof after);ck(!std::memcmp(&state,&after,sizeof state));
   // Exercise the actual live V1 equipment service's refresh path, then feed
   // its same Character PropertyView into combat. The source ELF receipt is
   // GEV5; its step 4 Warrior starter equip changes props79/80.
   if(baseid==263&&selected==0&&j==4){PlayerEquipmentQueriesLiveV1 queries(inventory,view);EquipmentWeaponFacts12V1 facts{};ck(queries.facts(facts,error)&&(facts.flags&weapon_main));CombatantView active{};ck(queries.combat_view(active,error));ck(active.properties==state.resolved.data()&&inventory.properties()==&state);PropertySheet defender=rules.defaults;auto bare=attack_damage(unequipped.resolved.data(),defender.data());auto equipped=attack_damage(active.properties,defender.data(),active.main_damage_class);ck(state.gear[79]!=rules.defaults[79]&&state.gear[80]!=rules.defaults[80]&&state.resolved[79]==after.resolved[79]&&state.resolved[80]==after.resolved[80]);ck(equipped.amount>bare.amount);++gear_combat;
    // The same original starter trace has equipped torso/armor before step 4.
    // Verify its live defense property is consumed as the combat defender stat,
    // with the same equipped attacker and same canonical Character view.
    ck(state.gear[71]>unequipped.gear[71]&&active.properties[71]==state.resolved[71]);auto unarmored_hit=attack_damage(active.properties,unequipped.resolved.data(),active.main_damage_class);auto armored_hit=attack_damage(active.properties,active.properties,active.main_damage_class);ck(armored_hit.amount<unarmored_hit.amount);++armor_combat;
   }
   ++steps;
  }
  ck(&adapter.inventory()==&inventory&&&adapter.property_view()==&view&&inventory.properties()==&state&&random.seeds[1]==991&&random.counters[1]==17);
  // Live buffs remain on the caller view; mutate the existing sheet in place.
  auto baseline=state;PropertySheet buff=rules.defaults;const std::int32_t* sheets[]{buff.data()};PropertyBuffGroup group{sheets,1};view.groups=&group;view.group_count=1;buff[149]=256*7;ck(adapter.refresh(false,error));auto first=state.resolved[149];buff[149]=256*11;ck(adapter.refresh(false,error));ck(state.resolved[149]-first==256*4&&view.groups==&group&&view.groups[0].sheets[0]==buff.data());++live_buffs;view.groups=nullptr;view.group_count=0;state=baseline;
  bool accepted=false;c.queries.clear();ck(adapter.meets_requirements(nullptr,accepted,error)&&accepted&&c.queries.size()==1&&c.queries[0]==EquipmentWorldQueryV1::online);
  ItemInstanceV1 absent;absent.id=-123;c.online=c.remote=true;c.queries.clear();ck(adapter.meets_requirements(&absent,accepted,error)&&accepted&&c.queries.size()==2);c.online=c.remote=false;accepted=true;ck(!adapter.meets_requirements(&absent,accepted,error)&&accepted);c.fail_world=true;ck(!adapter.meets_requirements(nullptr,accepted,error)&&accepted);c.fail_world=false;guards+=2;
  // Source low level removes active/shared equipment after recalculation.
  state.base[19]=-256;ck(adapter.refresh(false,error));auto before_rng=random;ck(adapter.equip(0,0,error));for(unsigned slot=0;slot<9;++slot){auto set=slot==1||slot==2?inventory.current_equipment():0;auto* cell=inventory.equipment()[set][slot];if(cell)ck(item(inventory.table(),cell->item->id)->record.words[29]<=-1);}ck(!std::memcmp(&random,&before_rng,sizeof random));++pruned;state=baseline;
  // Required Skin fails after property recalculation and before vitals clamp.
  c.visual=0x100000010ULL;c.expected_hp=c.expected_mp=100000000;ck(!dh2_property_set(&view,36,c.expected_hp)&&!dh2_property_set(&view,41,c.expected_mp));c.check_vitals=c.fail_skin=true;state.gear[149]=123456;ck(!adapter.refresh(false,error));ck(state.gear[149]!=123456&&state.resolved[36]==c.expected_hp&&state.resolved[41]==c.expected_mp);++prefixes;
  c.fail_skin=false;c.change_visual=true;c.reentry=true;ck(adapter.refresh(false,error));ck(state.resolved[36]==state.resolved[38]&&state.resolved[41]==state.resolved[43]);++vitals;c.check_vitals=c.reentry=false;
  auto saved=state;auto choice=inventory.current_equipment();c.attached=false;ck(!adapter.swap(error)&&!std::memcmp(&state,&saved,sizeof state)&&choice==inventory.current_equipment());c.attached=true;auto observer=h.required.observe_storage;h.required.observe_storage=nullptr;ck(!adapter.refresh(false,error)&&!std::memcmp(&state,&saved,sizeof state));h.required.observe_storage=observer;guards+=2;
  c.attached=false;std::unique_ptr<ItemInstanceV1> detached;auto detached_rng=random;auto text_calls=c.text_fixture;
  ck(!inventory.create_item(0,1,RetainedItemSlotV4{&detached},services,error)&&detached&&!error.empty()&&c.text_fixture==text_calls&&!std::memcmp(&random,&detached_rng,sizeof random));c.attached=true;ck(inventory.retire_item({&detached},services,error)&&!detached);++direct_detached_rejections;
  PropertyState other;auto wrong=property_view(rules,other);PlayerEquipmentLiveServicesV1 mismatch(inventory,wrong,rows.data(),rows.size(),powers.borrow(),h);ck(!mismatch.refresh(false,error)&&!std::memcmp(&state,&saved,sizeof state));++guards;
  OwnedInventoryResponseV4 response;c.difficulty=2;ck(services.invoke(services.context,inventory,{OwnedInventoryOperationV4::current_player,0,nullptr,nullptr,1,0},response,error)&&response.value==2);c.difficulty=0;ck(services.invoke(services.context,inventory,{OwnedInventoryOperationV4::current_player,0,nullptr,nullptr,0,0},response,error)&&response.identity==inventory.character());ck(services.invoke(services.context,inventory,{OwnedInventoryOperationV4::player_count,0,nullptr,nullptr,0,0},response,error)&&response.value==1);
  // Direct Item construction has no random draw; missing text fails before
  // text delivery; the caller retains the Item with its source constructor prefix.
  auto invoke=h.required.invoke;h.required.invoke=nullptr;std::unique_ptr<ItemInstanceV1> failed;auto draws=random.counters[0];ck(!inventory.create_item(0,1,RetainedItemSlotV4{&failed},services,error)&&failed&&!error.empty()&&random.counters[0]==draws);h.required.invoke=invoke;ck(inventory.retire_item({&failed},services,error)&&!failed);++prefixes;
  skin_calls+=c.skin_fixture;reentries+=c.reentries;
 }
 ck(r.at==starter.size());
 // Drive the production UI swap/deferred-HUD bridge around the actual live
 // equipment adapter, then through the same save-commit gate used by native
 // UI mutations. The fixture save is the canonical Character Save identity;
 // it verifies callback ordering/ownership, not Android profile-file I/O.
 {
  constexpr std::uintptr_t identity=0x200000002ULL;
  std::uint32_t bridge_base;std::memcpy(&bridge_base,starter.data()+8,4);
  PropertyState bridge_state;reset_properties(rules,bridge_state,&chars.rows.at(bridge_base));
  auto bridge_view=property_view(rules,bridge_state);
  ck(!dh2_class_recalc_base(rows.data(),rows.size(),bridge_state.base.data(),&bridge_view));
  dh2_random_state bridge_random{{3,5},{0,0}};
  FreshInventoryOwnedV4 bridge_inventory(identity,table.borrow(),{&bridge_random,random_next},12,bridge_state);
  Context bridge_context;bridge_context.inventory=&bridge_inventory;bridge_context.view=&bridge_view;
  bridge_context.visual=identity+16;
  auto bridge_hooks=hooks(bridge_context);
  PlayerEquipmentLiveServicesV1 bridge_adapter(bridge_inventory,bridge_view,
      rows.data(),rows.size(),powers.borrow(),bridge_hooks);
  bridge_context.adapter=&bridge_adapter;
  PlayerSavegameV1 canonical_save;canonical_save.set_character(identity);
  dh2::ui::DeferredHudRefreshV1 pending;unsigned saves=0;
  std::int32_t persisted_selection=-1;std::vector<int> hud_calls;
  for(std::int32_t expected=1;expected>=0;--expected){
   const auto skin_before=bridge_context.skin_fixture;
   ck(dh2::ui::swap_equipment_and_defer_hud_refresh_v1(pending,true,
    [&](std::string& e){
     if(!bridge_adapter.swap(e))return false;
     return dh2::native::player_inventory_save_commit_v1::after_successful_mutation(
      [&](std::string& save_error){
       if(canonical_save.character()!=identity||bridge_inventory.character()!=identity){
        save_error="UI swap save callback crossed the canonical Character Save";return false;
       }
       ++saves;persisted_selection=bridge_inventory.current_equipment();save_error.clear();return true;
      },e);
    },error));
   ck(bridge_inventory.current_equipment()==expected&&persisted_selection==expected&&saves==unsigned(2-expected));
   ck(bridge_context.skin_fixture>skin_before&&pending.display_right_hud_pending&&pending.fill_action_icon_pending);
   ck(dh2::ui::flush_deferred_hud_refresh_v1(pending,
      [&](std::string&){hud_calls.push_back(1);return true;},
      [&](std::string&){hud_calls.push_back(2);return true;},error));
   const std::vector<int> expected_hud_calls{1,2};
   ck(hud_calls==expected_hud_calls&&
      !pending.display_right_hud_pending&&!pending.fill_action_icon_pending);
   hud_calls.clear();
  }
 }
 // Real stack merge retires the incoming Item before V4 destroys it. No shadow
 // item store is used by the adapter or observer.
 std::uint32_t first_base;std::memcpy(&first_base,starter.data()+8,4);PropertyState state;reset_properties(rules,state,&chars.rows.at(first_base));auto view=property_view(rules,state);ck(!dh2_class_recalc_base(rows.data(),rows.size(),state.base.data(),&view));dh2_random_state random{{7,9},{0,0}};FreshInventoryOwnedV4 inventory(0x100000001ULL,table.borrow(),{&random,random_next},12,state);Context c;c.inventory=&inventory;c.view=&view;ItemPresentationOwnerV5 presentation(powers.borrow());c.presentation=&presentation;auto h=hooks(c);PlayerEquipmentLiveServicesV1 adapter(inventory,view,rows.data(),rows.size(),powers.borrow(),h);c.adapter=&adapter;auto services=adapter.services();std::int32_t stack=-1;
 for(std::uint32_t id=0;id<inventory.table().rows.size();++id){const auto& row=inventory.table().rows[id];if(std::uint8_t(row.record.words[7])&&item_type(row)!=13){stack=std::int32_t(id);break;}}ck(stack>=0);
 std::unique_ptr<ItemInstanceV1> a,b;ck(inventory.create_item(stack,1,RetainedItemSlotV4{&a},services,error)&&inventory.create_item(stack,1,RetainedItemSlotV4{&b},services,error));
 // The source retains AddPower's append before unavailable formatting. Bind
 // both equal stack payloads to real presentation entries from that prefix.
 ck(!presentation.add_power(*a,0,-1,{},error)&&presentation.powers(*a));ck(!presentation.add_power(*b,0,-1,{},error)&&presentation.powers(*b));
 std::int32_t index;ck(inventory.add_item(a,true,false,index,services,error)&&inventory.items().size()==1);c.incoming_destroy=b.get();c.reentry=true;ck(inventory.add_item(b,false,false,index,services,error)&&!b&&inventory.items().size()==1&&inventory.items()[0]->item->signed_quantity()==2&&c.destroyed==1);reentries+=c.reentries;
 // Direct V4 merge enters no bool effect before its void pre-delete delivery.
 // Every missing/detached/failed required lifecycle rejects there, preserving
 // quantity's reached prefix AND incoming ownership/presentation identities.
 for(unsigned failure=0;failure<3;++failure){
  PropertyState local_state;reset_properties(rules,local_state,&chars.rows.at(first_base));auto local_view=property_view(rules,local_state);ck(!dh2_class_recalc_base(rows.data(),rows.size(),local_state.base.data(),&local_view));dh2_random_state local_rng{{7,9},{0,0}};
  FreshInventoryOwnedV4 local_inventory(0x100000001ULL,table.borrow(),{&local_rng,random_next},12,local_state);Context local;local.inventory=&local_inventory;local.view=&local_view;ItemPresentationOwnerV5 local_presentation(powers.borrow());local.presentation=&local_presentation;
  auto local_hooks=hooks(local);PlayerEquipmentLiveServicesV1 local_adapter(local_inventory,local_view,rows.data(),rows.size(),powers.borrow(),local_hooks);local.adapter=&local_adapter;auto local_services=local_adapter.services();std::unique_ptr<ItemInstanceV1> left,incoming;
  ck(local_inventory.create_item(stack,1,RetainedItemSlotV4{&left},local_services,error)&&local_inventory.create_item(stack,1,RetainedItemSlotV4{&incoming},local_services,error));
  ck(!local_presentation.add_power(*left,0,-1,{},error)&&local_presentation.powers(*left));ck(!local_presentation.add_power(*incoming,0,-1,{},error)&&local_presentation.powers(*incoming));
  ck(local_inventory.add_item(left,true,false,index,local_services,error));auto* identity=incoming.get();local.incoming_destroy=identity;auto random_prefix=local_rng;
  if(failure==0)local_hooks.required.observe_storage=nullptr;else if(failure==1)local.attached=false;else local.fail_retirement=true;
  bool rejected=false;try{local_inventory.add_item(incoming,false,false,index,local_services,error);}catch(const EquipmentLifecycleFailureV1& e){ck(std::strlen(e.what())>0);rejected=true;}
  ck(rejected&&incoming.get()==identity&&local_inventory.items().size()==1&&local_inventory.items()[0]->item->signed_quantity()==2&&local_presentation.powers(*identity)&&!local.destroyed&&!std::memcmp(&local_rng,&random_prefix,sizeof local_rng));++direct_lifetime_rejections;++prefixes;
  // Fixture owns this surviving input. Explicit caller retirement precedes
  // its eventual destruction; it is never an invented adapter cleanup.
  ck(local_presentation.forget(*incoming,error)&&!local_presentation.powers(*incoming));incoming.reset();
 }
 ck(gear_combat==1&&armor_combat==1);std::cout<<"{\"validation\":\"PASS\",\"original_requirement_cases\":"<<requirement_cases<<",\"original_owner_cases\":"<<owner_cases<<",\"original_owner_steps\":"<<steps<<",\"live_equipped_property_to_combat_checks\":"<<gear_combat<<",\"live_armor_property_to_combat_checks\":"<<armor_combat<<",\"same_view_live_buff_cases\":"<<live_buffs<<",\"requirement_pruning_cases\":"<<pruned<<",\"skin_before_vitals_cases\":"<<vitals<<",\"retained_failure_prefix_cases\":"<<prefixes<<",\"lifetime_destroy_cases\":"<<c.destroyed<<",\"direct_lifetime_rejections\":"<<direct_lifetime_rejections<<",\"direct_detached_create_rejections\":"<<direct_detached_rejections<<",\"fixture_skin_deliveries\":"<<skin_calls<<",\"reentry_rejections\":"<<reentries<<",\"failure_guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
