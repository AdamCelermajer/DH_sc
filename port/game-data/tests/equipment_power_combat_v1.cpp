#include "../player_equipment_live_services_v1.hpp"
#include "../player_equipment_queries_live_v1.hpp"
#include "../combat.hpp"
extern "C" {
#include "../../random/random.h"
}
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static std::string error;
#define ck(v) ((v)?++checks:throw std::runtime_error(std::string("Check failed: ")+ #v + "; "+error))
static Raw file(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Cannot open "+path);return {std::istreambuf_iterator<char>(f),{}};}
static Bytes bytes(const Raw& r){return {r.data(),r.size()};}
struct Context{FreshInventoryOwnedV4* inventory{};PropertyView* view{};std::uintptr_t visual{};};
static bool binding(void* p,FreshInventoryOwnedV4& i,PropertyView& v,std::string&){auto& c=*static_cast<Context*>(p);return c.inventory==&i&&c.view==&v;}
static bool world(void*,EquipmentWorldQueryV1,std::uintptr_t,std::uintptr_t& id,std::int32_t& value,std::string&){id=0;value=0;return true;}
static bool skin(void*,FreshInventoryOwnedV4&,const GearSkinRequestV5&,std::int32_t& result,std::string&){result=0;return true;}
static bool required(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string&){using O=OwnedInventoryOperationV4;switch(q.operation){case O::update_name:case O::update_stats:case O::update_requirements:case O::debug_load:case O::debug_query:case O::full_notifications:out={};return true;default:return false;}}
static void observe(void*,FreshInventoryOwnedV4&,const OwnedInventoryRequestV4&){}
static bool random_next(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& value,std::string&){value=dh2_random_next(static_cast<dh2_random_state*>(p),std::uint32_t(bound),stream);return true;}
static Damage hit(const CombatantView& attacker,const std::int32_t* defender){CombatantView target{defender,-1,-1,0,0,0,0,0};CombatRandom rng{0x5a17,0};Damage result{};DamageRequest request{&attacker,&target,&rng,0,0,0,0};if(dh2_combat_damage(&result,&request))throw std::runtime_error("Source combat query rejected");return result;}
int main(int argc,char** argv){try{
 ck(argc==4);const std::string cache=argv[1],source_data=argv[2],power_cache=argv[3];
 auto load=[&](const std::string& root,const char* name){auto a=file(root+"/"+name+"_pyarray.bin"),b=file(root+"/"+name+"_pyarraynames.bin"),c=file(root+"/"+name+"_pystructnames.bin");return std::array<Raw,3>{std::move(a),std::move(b),std::move(c)};};
 auto loot_raw=load(cache,"loot_table");LootTablesV2 loot;ck(loot.load(bytes(loot_raw[0]),bytes(loot_raw[1]),bytes(loot_raw[2]),error));
 auto power_raw=load(power_cache,"item_powers");ItemPowerTablesV5 power_owner;ck(power_owner.load(bytes(power_raw[0]),bytes(power_raw[1]),bytes(power_raw[2]),error));auto powers=power_owner.borrow();
 auto character_raw=load(source_data,"character_properties");CharacterTable characters;ck(load_characters(bytes(character_raw[0]),bytes(character_raw[1]),bytes(character_raw[2]),characters,error));PropertyRules rules;ck(load_property_rules(characters,rules,error));
 auto classes_raw=load(source_data,"character_classes");ClassTables class_table;ck(load_classes(bytes(classes_raw[0]),bytes(classes_raw[1]),bytes(classes_raw[2]),class_table,error));std::vector<ClassRow> classes;for(const auto& r:class_table.rows)classes.push_back({r.data(),std::uint32_t(r.size())});
 // ItemPower type 28 is the original gear branch for direct physical min/max
 // damage (79/80). Select the first real positive table entry, no synthetic
 // power payload, and a cached one-handed weapon with source damage fields.
 std::int32_t power_id=-1,power_value=0;for(std::size_t i=0;i<powers.rows().size()&&power_id<0;++i)for(const auto& p:powers.rows()[i].properties)if(p.type==28&&p.value>0){power_id=std::int32_t(i);power_value=p.value;break;}ck(power_id>=0);
 std::int32_t weapon_id=-1;const auto& item_rows=loot.borrow().items().rows;for(std::size_t i=0;i<item_rows.size();++i){const auto& w=item_rows[i].record.words;if(w[26]==-4&&w[22]>=4&&w[22]<=5&&w[36]>w[35]&&w[37]>=0&&w[37]<141){weapon_id=std::int32_t(i);break;}}ck(weapon_id>=0);
 dh2_random_state random{{1,991},{0,17}};PropertyState state;reset_properties(rules,state,&characters.rows.at(263));auto view=property_view(rules,state);ck(dh2_class_recalc_base(classes.data(),std::uint32_t(classes.size()),state.base.data(),&view)==0);
 FreshInventoryOwnedV4 inventory(0x10000002ULL,loot.borrow(),{&random,random_next},12,state);Context context{&inventory,&view};EquipmentLiveHooksV1 hooks{&context,binding,{nullptr,world},&context.visual,{nullptr,skin},{nullptr,required,observe}};PlayerEquipmentLiveServicesV1 adapter(inventory,view,classes.data(),std::uint32_t(classes.size()),powers,hooks);auto services=adapter.services();
 std::unique_ptr<ItemInstanceV1> weapon;ck(inventory.create_item(weapon_id,1,RetainedItemSlotV4{&weapon},services,error));weapon->powers.push_back(power_id);std::int32_t index=-1;ck(inventory.add_item(weapon,true,false,index,services,error)&&!weapon&&index>=0);std::int32_t equipped=0;ck(adapter.auto_equip(std::uint32_t(index),equipped,error)&&equipped==1);
 const auto& current=*inventory.properties();ck(&current==&state&&view.resolved==state.resolved.data());auto* equipped_item=inventory.equipment()[0][1]->item.get();ck(equipped_item&&equipped_item->id==weapon_id);auto& equipped_powers=equipped_item->powers;equipped_powers.erase(std::find(equipped_powers.begin(),equipped_powers.end(),power_id));ck(adapter.refresh(false,error));const auto bare_gear=state.gear;const PropertySheet bare_resolved=state.resolved;equipped_powers.push_back(power_id);ck(adapter.refresh(false,error));ck(state.gear[79]==bare_gear[79]+power_value&&state.gear[80]==bare_gear[80]+power_value);
 PlayerEquipmentQueriesLiveV1 queries(inventory,view);CombatantView combat{};ck(queries.combat_view(combat,error));ck(combat.properties==state.resolved.data()&&combat.main_damage_class==item_rows[weapon_id].record.words[37]);PropertySheet defender=rules.defaults;const auto before=hit(CombatantView{bare_resolved.data(),combat.main_damage_class,-1,0,0,0,0,0},defender.data());const auto after=hit(combat,defender.data());ck(after.amount>before.amount);
 std::cout<<"{\"validation\":\"PASS\",\"source_weapon_id\":"<<weapon_id<<",\"source_power_id\":"<<power_id<<",\"power_type\":28,\"power_value\":"<<power_value<<",\"gear_damage_min_delta\":"<<state.gear[79]-bare_gear[79]<<",\"gear_damage_max_delta\":"<<state.gear[80]-bare_gear[80]<<",\"bare_damage\":"<<before.amount<<",\"powered_damage\":"<<after.amount<<",\"same_character_property_view\":true,\"checks\":"<<checks<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
