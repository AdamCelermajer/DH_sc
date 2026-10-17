#include "../player_equipment_live_services_v1.hpp"
#include "../player_equipment_queries_live_v1.hpp"
#include "../item_presentation_v5.hpp"
extern "C" {
#include "../../random/random.h"
}
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
#define ck(v) ((v)?++checks:throw std::runtime_error(std::string("Check ")+ #v + "; "+error))
static std::string error;
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Cannot open source fixture: "+p);return {std::istreambuf_iterator<char>(f),{}};}
static Bytes bytes(const Raw& r){return {r.data(),r.size()};}
struct Ctx {FreshInventoryOwnedV4* inventory{};PropertyView* view{};std::uintptr_t visual{};unsigned updates{},skin{},retire{};};
static bool binding(void* p,FreshInventoryOwnedV4& i,PropertyView& v,std::string&){auto& c=*static_cast<Ctx*>(p);return &i==c.inventory&&&v==c.view;}
static bool world(void*,EquipmentWorldQueryV1,std::uintptr_t,std::uintptr_t& id,std::int32_t& value,std::string&){id=0;value=0;return true;}
static bool skin(void* p,FreshInventoryOwnedV4&,const GearSkinRequestV5&,std::int32_t& out,std::string&){++static_cast<Ctx*>(p)->skin;out=0;return true;}
static bool invoke(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string&){auto& c=*static_cast<Ctx*>(p);using O=OwnedInventoryOperationV4;switch(q.operation){case O::update_name:case O::update_stats:case O::update_requirements:case O::debug_load:case O::debug_query:case O::full_notifications:case O::inventory_full:case O::gold_notifications:++c.updates;out={};return true;default:return false;}}
static void observe(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q){if(q.operation==OwnedInventoryOperationV4::destroy_item)++static_cast<Ctx*>(p)->retire;}
static std::int32_t rng(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& value,std::string&){value=dh2_random_next(static_cast<dh2_random_state*>(p),std::uint32_t(bound),stream);return 0;}
static bool next(void* p,std::int32_t b,std::uint32_t s,std::int32_t& v,std::string& e){return rng(p,b,s,v,e)==0;}
static ItemInstanceV1* equip_item(FreshInventoryOwnedV4& inv,const OwnedInventoryServicesV4& svc,std::int32_t id,std::string& e){std::unique_ptr<ItemInstanceV1> owned;ck(inv.create_item(id,1,RetainedItemSlotV4{&owned},svc,e));auto* ptr=owned.get();std::int32_t index=-1;ck(inv.add_item(owned,true,false,index,svc,e)&&!owned&&index>=0);for(const auto& cell:inv.items())if(cell->item.get()==ptr)return ptr;throw std::runtime_error("New source item not retained in canonical inventory");}
static std::uint32_t add(FreshInventoryOwnedV4& inv,const OwnedInventoryServicesV4& svc,std::int32_t id,std::string& e){auto* ptr=equip_item(inv,svc,id,e);auto it=std::find_if(inv.items().begin(),inv.items().end(),[&](const auto& s){return s->item.get()==ptr;});ck(it!=inv.items().end());return std::uint32_t(it-inv.items().begin());}
int main(int argc,char** argv){try{
 ck(argc==2);std::string cache=argv[1];
 auto read=[&](const char* stem){auto b=file(cache+"/"+stem+"_pyarray.bin"),n=file(cache+"/"+stem+"_pyarraynames.bin"),s=file(cache+"/"+stem+"_pystructnames.bin");return std::array<Raw,3>{std::move(b),std::move(n),std::move(s)};};
 auto loot=read("loot_table");LootTablesV2 tables;ck(tables.load(bytes(loot[0]),bytes(loot[1]),bytes(loot[2]),error));
 auto power=read("item_powers");ItemPowerTablesV5 powers;ck(powers.load(bytes(power[0]),bytes(power[1]),bytes(power[2]),error));
 auto chars_raw=read("character_properties");CharacterTable chars;ck(load_characters(bytes(chars_raw[0]),bytes(chars_raw[1]),bytes(chars_raw[2]),chars,error));PropertyRules rules;ck(load_property_rules(chars,rules,error));
 auto class_raw=read("character_classes");ClassTables class_table;ck(load_classes(bytes(class_raw[0]),bytes(class_raw[1]),bytes(class_raw[2]),class_table,error));std::vector<ClassRow> classes;for(auto& r:class_table.rows)classes.push_back({r.data(),std::uint32_t(r.size())});
 const auto source_items=tables.borrow().items();
 std::vector<std::int32_t> found(9,-1);std::int32_t ring_a=-1,ring_b=-1,dual_a=-1,dual_b=-1,twohand=-1,replacement=-1;
 for(std::size_t i=0;i<source_items.rows.size();++i){const auto& item=source_items.rows[i];const auto slot=item.record.words[26],type=item.record.words[22];if(slot>=0&&slot<9&&found[std::size_t(slot)]<0&&item.record.words[29]<=0)found[std::size_t(slot)]=std::int32_t(i);if(slot==3&&found[3]>=0&&std::int32_t(i)!=found[3]&&replacement<0)replacement=std::int32_t(i);if(slot==-2&&ring_a<0)ring_a=std::int32_t(i);else if(slot==-2&&ring_b<0)ring_b=std::int32_t(i);if(slot==-3&&dual_a<0)dual_a=std::int32_t(i);else if(slot==-3&&dual_b<0)dual_b=std::int32_t(i);if(slot==-4&&(type==4||type==5)&&twohand<0)twohand=std::int32_t(i);}
 dh2_random_state random{{17,991},{0,0}};PropertyState state;reset_properties(rules,state,&chars.rows.at(263));auto view=property_view(rules,state);ck(!dh2_class_recalc_base(classes.data(),classes.size(),state.base.data(),&view));
 FreshInventoryOwnedV4 inv(0x100000001ULL,tables.borrow(),{&random,next},12,state);Ctx ctx{&inv,&view};EquipmentLiveHooksV1 hooks{&ctx,binding,{nullptr,world},&ctx.visual,{&ctx,skin},{&ctx,invoke,observe}};PlayerEquipmentLiveServicesV1 adapter(inv,view,classes.data(),classes.size(),powers.borrow(),hooks);auto svc=adapter.services();PlayerEquipmentQueriesLiveV1 queries(inv,view);
 std::array<std::int32_t,9> slot_item;slot_item.fill(-1);std::array<std::int32_t,9> source_slot;source_slot.fill(-99);unsigned direct=0,missing_direct=0;
 for(unsigned expected=0;expected<9;++expected){if(found[expected]<0){++missing_direct;continue;}auto index=add(inv,svc,found[expected],error);std::int32_t result=-1;if(!adapter.auto_equip(index,result,error)||result!=1)throw std::runtime_error("direct slot "+std::to_string(expected)+" item "+std::to_string(found[expected])+" rejected: "+error);auto actual=inv.equipment()[expected==1||expected==2?inv.current_equipment():0][expected];ck(actual&&actual->item->id==found[expected]);ck(inv.properties()==&state&&view.resolved==state.resolved.data());if(expected==1||expected==2){EquipmentWeaponFacts12V1 facts;ck(queries.facts(facts,error));auto category=source_items.rows[std::size_t(found[expected])].record.words[37];ck((expected==1?facts.main_category:facts.off_category)==category);}slot_item[expected]=found[expected];source_slot[expected]=source_items.rows[std::size_t(found[expected])].record.words[26];++direct;}
 bool replacement_test=false;if(found[3]>=0&&replacement>=0){auto index=add(inv,svc,replacement,error);std::int32_t result;ck(adapter.auto_equip(index,result,error)&&result==1);ck(inv.equipment()[0][3]&&inv.equipment()[0][3]->item->id==replacement&&inv.properties()==&state&&view.resolved==state.resolved.data());ck(adapter.unequip(3,error)&&!inv.equipment()[0][3]&&inv.properties()==&state&&view.resolved==state.resolved.data());replacement_test=true;}
 // Slot 5/6 are both populated by the source's -2 ring category.
 if(ring_a>=0&&ring_b>=0){auto a=add(inv,svc,ring_a,error),b=add(inv,svc,ring_b,error);std::int32_t result;if(!adapter.auto_equip(a,result,error)||result!=1)throw std::runtime_error("ring first "+std::to_string(ring_a)+": "+error);ck(inv.equipment()[0][5]&&inv.equipment()[0][5]->item->id==ring_a);if(!adapter.auto_equip(b,result,error)||result!=1)throw std::runtime_error("ring second "+std::to_string(ring_b)+": "+error);ck(inv.equipment()[0][6]&&inv.equipment()[0][6]->item->id==ring_b);}
 // The paired-wield category fills hands 1 then 2; a third AutoEquip is a
 // source no-op. A real two-hand item clears both and owns hand 1 only.
 bool dual_test=false,twohand_test=false;if(dual_a>=0&&dual_b>=0&&twohand>=0){ck(adapter.unequip(1,error)&&adapter.unequip(2,error));auto a=add(inv,svc,dual_a,error),b=add(inv,svc,dual_b,error);std::int32_t result;if(!adapter.auto_equip(a,result,error)||result!=1)throw std::runtime_error("dual first "+std::to_string(dual_a)+": "+error);if(!adapter.auto_equip(b,result,error)||result!=1)throw std::runtime_error("dual second "+std::to_string(dual_b)+": "+error);ck(inv.equipment()[0][1]&&inv.equipment()[0][1]->item->id==dual_a);ck(inv.equipment()[0][2]&&inv.equipment()[0][2]->item->id==dual_b);EquipmentWeaponFacts12V1 facts;ck(queries.facts(facts,error)&&(facts.flags&weapon_dual));ck(facts.main_category==source_items.rows[std::size_t(dual_a)].record.words[37]&&facts.off_category==source_items.rows[std::size_t(dual_b)].record.words[37]);ck(inv.properties()==&state&&view.resolved==state.resolved.data());dual_test=true;auto c=add(inv,svc,twohand,error);if(!adapter.auto_equip(c,result,error)||result!=1)throw std::runtime_error("two hand "+std::to_string(twohand)+": "+error);ck(inv.equipment()[0][1]&&inv.equipment()[0][1]->item->id==twohand&&!inv.equipment()[0][2]);ck(queries.facts(facts,error)&&(facts.flags&weapon_two_raw)&&!(facts.flags&weapon_dual));ck(facts.main_category==source_items.rows[std::size_t(twohand)].record.words[37]&&inv.properties()==&state&&view.resolved==state.resolved.data());ck(adapter.unequip(1,error)&&!inv.equipment()[0][1]&&!inv.equipment()[0][2]);ck(queries.facts(facts,error)&&!(facts.flags&(weapon_main|weapon_dual|weapon_two_raw)));twohand_test=true;}
 // Every source slot row is validated against ItemTable's signed slot field;
 // facts categories come from the actual word37 record, including the empty case.
 EquipmentWeaponFacts12V1 facts;ck(queries.facts(facts,error));ck(facts.main_category==-1&&facts.off_category==-1);
 std::cout<<"{\"validation\":\"PASS\",\"direct_slot_rows\":"<<direct<<",\"missing_direct_slots\":"<<missing_direct<<",\"direct_replacement_removal\":"<<(replacement_test?"true":"false")<<",\"ring_pair\":"<<(ring_a>=0&&ring_b>=0?"true":"false")<<",\"dual_wield_two_hand_removal\":"<<(dual_test&&twohand_test?"true":"false")<<",\"source_rows\":{\"slotting_-2\":"<<ring_a<<",\"slotting_-3\":"<<dual_a<<",\"slotting_-4\":"<<twohand<<"},\"character_property_view_identity\":"<<(inv.properties()==&state&&view.resolved==state.resolved.data()?"true":"false")<<",\"gear_refreshes\":"<<ctx.skin<<",\"checks\":"<<checks<<"}\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
