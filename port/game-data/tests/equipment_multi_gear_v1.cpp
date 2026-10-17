#include "../player_equipment_live_services_v1.hpp"
#include "../player_equipment_queries_live_v1.hpp"
#include "../item_power_tables_v5.hpp"
#include "../item_gear_properties_v5.hpp"
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
static std::string error;
#define ck(v) ((v)?++checks:throw std::runtime_error(std::string("Check ")+ #v + "; "+error))
static Raw file(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Cannot open "+p);return {std::istreambuf_iterator<char>(f),{}};}
static Bytes bytes(const Raw& r){return {r.data(),r.size()};}
static bool rng(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& out,std::string&){out=dh2_random_next(static_cast<dh2_random_state*>(p),std::uint32_t(bound),stream);return true;}
struct Context{FreshInventoryOwnedV4* inventory{};PropertyView* view{};std::uintptr_t visual{};unsigned text{},skin{};};
static bool binding(void* p,FreshInventoryOwnedV4& i,PropertyView& v,std::string&){auto& c=*static_cast<Context*>(p);return c.inventory==&i&&c.view==&v;}
static bool world(void*,EquipmentWorldQueryV1,std::uintptr_t,std::uintptr_t& id,std::int32_t& value,std::string&){id=0;value=0;return true;}
static bool skin(void* p,FreshInventoryOwnedV4&,const GearSkinRequestV5&,std::int32_t& out,std::string&){++static_cast<Context*>(p)->skin;out=0;return true;}
static bool invoke(void* p,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string&){auto& c=*static_cast<Context*>(p);using O=OwnedInventoryOperationV4;switch(q.operation){case O::update_name:case O::update_stats:case O::update_requirements:++c.text;out={};return true;case O::debug_load:case O::debug_query:case O::full_notifications:out={};return true;default:return false;}}
static void observe(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&){}
static std::uint32_t store_item(FreshInventoryOwnedV4& inv,const OwnedInventoryServicesV4& svc,std::int32_t id,std::int32_t power){std::unique_ptr<ItemInstanceV1> item;ck(inv.create_item(id,1,RetainedItemSlotV4{&item},svc,error));item->powers.push_back(power);std::int32_t index=-1;ck(inv.add_item(item,true,false,index,svc,error)&&!item&&index>=0);return std::uint32_t(index);}
static bool power_has(const ItemPowerTablesV5::Borrow& powers,std::int32_t id,std::int32_t type,std::int32_t value){const auto& p=powers.rows().at(std::size_t(id)).properties;return std::any_of(p.begin(),p.end(),[&](const auto& x){return x.type==type&&x.value==value;});}
static void expect_current_gear(const FreshInventoryOwnedV4& inv,const PropertyView& actual,const PropertyRules& rules,const ItemPowerTablesV5::Borrow& powers){
 ck(inv.properties()!=nullptr);PropertyState expected=*inv.properties();auto view=property_view(rules,expected);ck(dh2_gear_reset_v5(expected.gear.data(),view.defaults)==0);
 for(unsigned slot=0;slot<9;++slot){const auto set=(slot==1||slot==2)?std::uint32_t(inv.current_equipment()):0;auto* cell=inv.equipment()[set][slot];if(!cell)continue;ck(bool(cell->item));const auto* row=item(inv.table(),cell->item->id);ck(row!=nullptr);const auto left=std::uint32_t(slot==2);ck(dh2_gear_stats_v5(expected.gear.data(),view.defaults,&row->record,left)==0);for(const auto id:cell->item->powers){ck(id>=0&&std::size_t(id)<powers.rows().size());const auto& def=powers.rows()[std::size_t(id)];GearPowerView16V5 pv{def.properties.data(),std::uint32_t(def.properties.size()),0};ck(dh2_gear_power_v5(expected.gear.data(),view.defaults,&pv,left)==0);}}
 std::string recalc_error;ck(recalc_properties(rules,expected,recalc_error));ck(actual.resolved==inv.properties()->resolved.data());ck(expected.gear==inv.properties()->gear);ck(expected.resolved==inv.properties()->resolved);
}
int main(int argc,char** argv){try{
 ck(argc==2);const auto dir=std::string(argv[1]);auto read=[&](const char* stem){auto a=file(dir+"/"+stem+"_pyarray.bin"),b=file(dir+"/"+stem+"_pyarraynames.bin"),c=file(dir+"/"+stem+"_pystructnames.bin");return std::array<Raw,3>{std::move(a),std::move(b),std::move(c)};};
 auto l=read("loot_table");LootTablesV2 loot;ck(loot.load(bytes(l[0]),bytes(l[1]),bytes(l[2]),error));auto p=read("item_powers");ItemPowerTablesV5 power_owner;ck(power_owner.load(bytes(p[0]),bytes(p[1]),bytes(p[2]),error));auto powers=power_owner.borrow();
 auto ch=read("character_properties");CharacterTable chars;ck(load_characters(bytes(ch[0]),bytes(ch[1]),bytes(ch[2]),chars,error));PropertyRules rules;ck(load_property_rules(chars,rules,error));auto cl=read("character_classes");ClassTables class_tables;ck(load_classes(bytes(cl[0]),bytes(cl[1]),bytes(cl[2]),class_tables,error));std::vector<ClassRow> classes;for(const auto& r:class_tables.rows)classes.push_back({r.data(),std::uint32_t(r.size())});
 const auto& source_items=loot.borrow().items().rows;ck(source_items.size()>929&&powers.rows().size()>889);ck(source_items[841].record.words[26]==3&&source_items[929].record.words[26]==-2);ck(power_has(powers,193,2,256)&&power_has(powers,889,1,256));
 std::int32_t replacement_boot=-1,dual_a=-1,dual_b=-1,two_hand=-1;for(std::size_t i=0;i<source_items.size();++i){const auto slot=source_items[i].record.words[26],type=source_items[i].record.words[22];if(slot==3&&std::int32_t(i)!=841&&replacement_boot<0)replacement_boot=std::int32_t(i);if(slot==-3&&dual_a<0)dual_a=std::int32_t(i);else if(slot==-3&&dual_b<0)dual_b=std::int32_t(i);if(slot==-4&&(type==4||type==5)&&two_hand<0)two_hand=std::int32_t(i);}ck(replacement_boot>=0&&dual_a>=0&&dual_b>=0&&two_hand>=0);
 dh2_random_state random{{991,17},{0,0}};PropertyState state;reset_properties(rules,state,&chars.rows.at(263));auto view=property_view(rules,state);ck(dh2_class_recalc_base(classes.data(),std::uint32_t(classes.size()),state.base.data(),&view)==0);
 FreshInventoryOwnedV4 inv(0x100000009ULL,loot.borrow(),{&random,rng},12,state);Context ctx{&inv,&view};EquipmentLiveHooksV1 hooks{&ctx,binding,{nullptr,world},&ctx.visual,{&ctx,skin},{&ctx,invoke,observe}};PlayerEquipmentLiveServicesV1 adapter(inv,view,classes.data(),std::uint32_t(classes.size()),powers,hooks);const auto svc=adapter.services();
 // Powered item IDs were observed in source-generated AddLoot evidence: item
 // 841 carries power 193; source ring 929 carries power 889. This fixture
 // isolates live equipment recomputation, not the already-covered RNG creator.
 auto boots=store_item(inv,svc,841,193),ring1=store_item(inv,svc,929,889),ring2=store_item(inv,svc,929,889);std::int32_t result{};
 ck(adapter.auto_equip(boots,result,error)&&result==1);PlayerEquipmentQueriesLiveV1 queries(inv,view);CombatantView combat{};ck(queries.combat_view(combat,error)&&combat.properties==state.resolved.data());expect_current_gear(inv,view,rules,powers);
 ck(adapter.auto_equip(ring1,result,error)&&result==1&&inv.equipment()[0][5]->item->id==929);expect_current_gear(inv,view,rules,powers);ck(state.gear[150]==256&&state.gear[149]==256);const auto one_ring=state.gear;
 ck(adapter.auto_equip(ring2,result,error)&&result==1&&inv.equipment()[0][6]->item->id==929);expect_current_gear(inv,view,rules,powers);ck(state.gear[149]==512&&state.gear[149]-one_ring[149]==256);const auto two_rings=state.gear;
 ck(adapter.unequip(5,error)&&!inv.equipment()[0][5]);expect_current_gear(inv,view,rules,powers);ck(state.gear[149]==256&&two_rings[149]-state.gear[149]==256);
 auto replacement=store_item(inv,svc,replacement_boot,193);ck(adapter.auto_equip(replacement,result,error)&&result==1&&inv.equipment()[0][3]->item->id==replacement_boot);expect_current_gear(inv,view,rules,powers);ck(state.gear[150]==256&&state.gear[149]==256);
 ck(adapter.unequip(3,error)&&!inv.equipment()[0][3]);expect_current_gear(inv,view,rules,powers);ck(state.gear[150]==rules.defaults[150]&&state.gear[149]==256);
 auto w1=store_item(inv,svc,dual_a,193),w2=store_item(inv,svc,dual_b,889);ck(adapter.auto_equip(w1,result,error)&&result==1);expect_current_gear(inv,view,rules,powers);ck(adapter.auto_equip(w2,result,error)&&result==1);ck(inv.equipment()[0][1]&&inv.equipment()[0][2]);expect_current_gear(inv,view,rules,powers);const auto dual=state.gear;
 auto heavy=store_item(inv,svc,two_hand,889);ck(adapter.auto_equip(heavy,result,error)&&result==1);ck(inv.equipment()[0][1]&&inv.equipment()[0][1]->item->id==two_hand&&!inv.equipment()[0][2]);expect_current_gear(inv,view,rules,powers);ck(state.gear[149]==dual[149]&&state.gear[150]!=dual[150]);
 ck(adapter.unequip(1,error)&&!inv.equipment()[0][1]&&!inv.equipment()[0][2]);expect_current_gear(inv,view,rules,powers);ck(state.gear[149]==256&&state.gear[150]==rules.defaults[150]);const auto before_refresh=state.gear;ck(adapter.refresh(false,error)&&state.gear==before_refresh);ck(adapter.refresh(false,error)&&state.gear==before_refresh);ck(inv.properties()==&state&&view.resolved==state.resolved.data());
 std::cout<<"{\"validation\":\"PASS\",\"source_items\":[841,929,"<<replacement_boot<<","<<dual_a<<","<<dual_b<<","<<two_hand<<"],\"source_power_ids\":[193,889],\"ring_power_stacks\":2,\"replacement_and_unequip\":true,\"dual_to_twohand_offhand_removed\":true,\"repeated_refresh_does_not_accumulate\":true,\"reference_order\":\"reset, slots 0..8, item stats then item powers, one class recalc\",\"same_character_property_view\":true,\"checks\":"<<checks<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
