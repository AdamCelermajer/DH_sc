#include "../item_text_owner_v5.hpp"
#include "../../game-data/player_equipment_queries_live_v1.hpp"
extern "C" {
#include "../../pydata-constants/constants.h"
#include "../../random/random.h"
}
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
using namespace dh2;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void ck(bool okay,const std::string& why="check"){++checks;if(!okay)throw std::runtime_error(why+" #"+std::to_string(checks));}
static Raw file(const std::filesystem::path& p){std::ifstream f(p,std::ios::binary);ck(bool(f),p.string());return {std::istreambuf_iterator<char>(f),{}};}
static data::Bytes span(const Raw& b){return {b.data(),b.size()};}
struct Reader{const Raw& b;std::size_t at{};std::uint32_t word(){ck(at<=b.size()&&b.size()-at>=4);std::uint32_t n;std::memcpy(&n,b.data()+at,4);at+=4;return n;}};
struct Context {
 std::filesystem::path root;Raw constants_bytes,colors_bytes;dh2_pycst_view constants{},colors{};
 std::map<std::uintptr_t,std::string> leases;std::uintptr_t next_lease{};
 std::set<std::string> reached_files;
 unsigned opened{},closed{},debug{},constants_called{},text_effects{};bool reject_open{},reject_close{},reject_constant{},reject_debug{};
 ui::ItemTextOwnerV5* item_text{};
 static bool open(void* p,const char* uri,bool& found,Raw& bytes,std::uintptr_t& lease,std::string& error){auto& c=*static_cast<Context*>(p);if(c.reject_open){error="Explicit host file rejection";return false;}ck(std::string(uri).rfind("text/",0)==0);auto path=c.root/uri;std::ifstream f(path,std::ios::binary);found=bool(f);lease=0;if(!found)return true;bytes={std::istreambuf_iterator<char>(f),{}};lease=++c.next_lease;c.leases.emplace(lease,uri);c.reached_files.insert(uri);++c.opened;return true;}
 static bool close(void* p,std::uintptr_t lease,std::string& error){auto& c=*static_cast<Context*>(p);ck(c.leases.erase(lease)==1);++c.closed;if(c.reject_close){error="Explicit host close rejection";return false;}return true;}
 static bool debug_service(void* p,const char* key,std::string& error){auto& c=*static_cast<Context*>(p);ck(std::string(key)=="isTracingStringManager");++c.debug;if(c.reject_debug){error="Explicit host Debug rejection";return false;}return true;}
 static bool constant(void* p,const char* group,const char* key,std::uint32_t& value,std::string& error){auto& c=*static_cast<Context*>(p);++c.constants_called;if(c.reject_constant){error="Explicit host constant rejection";return false;}dh2_pycst_result r{};auto* table=std::string(group)=="FontTextColors"?&c.colors:&c.constants;if(dh2_pycst_get(table,group,std::strlen(group),key,std::strlen(key),&r)||!r.found){error=std::string("Actual constant absent: ")+group+"."+key;return false;}std::memcpy(&value,&r.value,4);return true;}
 ui::HudTextEnvironmentV1 environment(){return {{this,open,close,debug_service,constant,nullptr,nullptr},nullptr,nullptr,nullptr};}
 static bool random(void* p,std::int32_t bound,std::uint32_t stream,std::int32_t& out,std::string&){out=dh2_random_next(static_cast<dh2_random_state*>(p),std::uint32_t(bound),stream);return true;}
 static bool effect(void* p,data::FreshInventoryOwnedV4&,const data::OwnedInventoryRequestV4& q,data::OwnedInventoryResponseV4& out,std::string& error){auto& c=*static_cast<Context*>(p);using O=data::OwnedInventoryOperationV4;if(q.operation==O::debug_load||q.operation==O::debug_query){out.value=0;return true;}if(q.operation==O::update_name||q.operation==O::update_stats||q.operation==O::update_requirements){ck(q.item&&c.item_text);++c.text_effects;auto s=c.item_text->services();if(q.operation==O::update_name)return data::item_update_name_v5(*q.item,s,error);if(q.operation==O::update_stats)return data::item_update_stats_v5(*q.item,s,error);return data::item_update_requirements_v5(*q.item,s,error);}error="Unbound native continuation in host composition";return false;}
};
int main(int argc,char** argv){try{
 ck(argc==3);std::filesystem::path cache=argv[1];auto gold=file(argv[2]);Reader r{gold};ck(r.word()==0x31515245);auto req=r.word(),queries=r.word();r.at+=std::size_t(req)*200;ck(r.at<=gold.size());
 for(unsigned k=0;k<queries;++k){r.word();auto flag=std::int32_t(r.word());auto a=r.word(),b=r.word();data::ItemRecord164 rows[2]{};for(auto& row:rows)for(auto& w:row.words)w=std::int32_t(r.word());data::EquipmentWeaponFacts12V1 expected;expected.main_category=std::int32_t(r.word());expected.off_category=std::int32_t(r.word());expected.flags=r.word();data::EquipmentWeaponFacts12V1 actual;ck(data::equipment_weapon_facts_v1(&actual,a?&rows[0]:nullptr,b?&rows[1]:nullptr,flag)==0);ck(!std::memcmp(&actual,&expected,sizeof actual),"original weapon gold mismatch");}ck(r.at==gold.size());
 data::EquipmentWeaponFacts12V1 kept{11,12,13};auto saved=kept;alignas(4) std::uint8_t bad[200]{};ck(data::equipment_weapon_facts_v1(nullptr,nullptr,nullptr,0)==-1);ck(data::equipment_weapon_facts_v1(&kept,reinterpret_cast<data::ItemRecord164*>(bad+1),nullptr,0)==-1&&!std::memcmp(&kept,&saved,sizeof kept));ck(data::equipment_weapon_facts_v1(reinterpret_cast<data::EquipmentWeaponFacts12V1*>(bad),reinterpret_cast<data::ItemRecord164*>(bad),nullptr,0)==-1);
 auto table=[&](const char* name,auto load){auto a=file(cache/(std::string(name)+"_pyarray.bin")),b=file(cache/(std::string(name)+"_pyarraynames.bin")),c=file(cache/(std::string(name)+"_pystructnames.bin"));std::string error;ck(load(a,b,c,error),error);};
 data::LootTablesV2 loot;data::CharacterTable chars;data::ItemPowerTablesV5 powers;ui::HudTextV1 text;std::string error;
 table("loot_table",[&](auto& a,auto& b,auto& c,auto& e){return loot.load(span(a),span(b),span(c),e);});
 table("character_properties",[&](auto& a,auto& b,auto& c,auto& e){return data::load_characters(span(a),span(b),span(c),chars,e);});
 table("item_powers",[&](auto& a,auto& b,auto& c,auto& e){return powers.load(span(a),span(b),span(c),e);});
 table("common_text",[&](auto& a,auto& b,auto& c,auto& e){return text.load({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},e);});
 ck(text.switch_pack(0,false,error),error);Context context;context.root=cache.parent_path();context.constants_bytes=file(cache/"common_text_pycst.bin");context.colors_bytes=file(cache/"fonts_pycst.bin");ck(!dh2_pycst_open(&context.constants,context.constants_bytes.data(),context.constants_bytes.size()));ck(!dh2_pycst_open(&context.colors,context.colors_bytes.data(),context.colors_bytes.size()));
 data::PropertyRules rules;ck(data::load_property_rules(chars,rules,error),error);data::PropertyState state;data::reset_properties(rules,state,&chars.rows.at(263));auto view=data::property_view(rules,state);dh2_random_state random{{991,887},{0,31}};data::FreshInventoryOwnedV4 inventory(0x1001,loot.borrow(),{&random,Context::random},-1,state);ui::ItemTextOwnerV5 item_text(inventory.table(),chars,text,context.environment());context.item_text=&item_text;auto services=item_text.services();data::OwnedInventoryServicesV4 effects{&context,Context::effect,nullptr,true};
 unsigned item_cases=0,power_cases=0,null_power_prefixes=0;std::vector<std::int32_t> weapon_ids;
 for(std::uint32_t id=0;id<inventory.table().rows.size();++id){std::unique_ptr<data::ItemInstanceV1> actual;ck(inventory.create_item(std::int32_t(id),1,actual,effects,error),"actual Item constructor "+std::to_string(id)+": "+error);ck(actual&&actual->id==std::int32_t(id));++item_cases;if(weapon_ids.size()<4&&inventory.table().rows[id].record.words[26]==1&&inventory.table().rows[id].record.words[37]>=0)weapon_ids.push_back(std::int32_t(id));}
 // ClassName is an actual row word5 read, followed by genuine integer lookup.
 data::ItemInstanceV1 class_probe;for(auto row:{263,264,265,325,327,326,290,292,291}){data::ItemTextResponseV5 response;std::string unused;ck(services.invoke(services.context,class_probe,{data::ItemTextOperationV5::class_name,row},response,unused,error));ck(response.value==chars.rows.at(row)[5]);std::string localized;bool is_null;ck(text.integer_string(response.value,context.environment().localization,localized,is_null,error)&&!is_null&&!localized.empty());}
 data::ItemPresentationOwnerV5 presentation(powers.borrow());
 for(std::size_t id=0;id<powers.borrow().rows().size();++id){data::ItemInstanceV1 actual;actual.id=0;auto okay=presentation.add_power(actual,std::int32_t(id),0,services,error);if(powers.borrow().rows()[id].scalars.description<0){ck(!okay&&actual.powers.size()==1&&presentation.powers(actual)&&error.find("null")!=std::string::npos);++null_power_prefixes;}else{ck(okay,"actual Power text "+std::to_string(id)+": "+error);++power_cases;}ck(presentation.forget(actual,error)&&!presentation.powers(actual));}
 ck(weapon_ids.size()==4);data::PlayerEquipmentQueriesLiveV1 facade(inventory,view);data::EquipmentWeaponFacts12V1 empty;ck(facade.facts(empty,error)&&empty.flags==0&&empty.main_category==-1&&empty.off_category==-1);auto before_random=random;auto before_properties=state;
 for(unsigned i=0;i<4;++i){if(i==2)inventory.swap_equipment();std::unique_ptr<data::ItemInstanceV1> actual;ck(inventory.create_item(weapon_ids[i],1,actual,effects,error),error);std::int32_t index;ck(inventory.add_item(actual,true,false,index,effects,error),error);ck(inventory.equip_to_slot(1,std::uint32_t(index),false,effects,error),error);data::EquipmentWeaponFacts12V1 q;ck(facade.facts(q,error));ck(q.main_category==inventory.table().rows[weapon_ids[i]].record.words[37]&&(q.flags&data::weapon_main));data::CombatantView combat{nullptr,-1,-1,0,0,0,6,77};ck(facade.combat_view(combat,error)&&combat.properties==state.resolved.data()&&combat.state==6&&combat.combo_hits==77&&combat.main_damage_class==q.main_category);inventory.swap_equipment();ck(facade.facts(q,error));if(i<2)ck(q.flags==0);else ck(q.main_category==inventory.table().rows[weapon_ids[1]].record.words[37]&&(q.flags&data::weapon_main));inventory.swap_equipment();}
 ck(!std::memcmp(&random,&before_random,sizeof random)&&!std::memcmp(&state,&before_properties,sizeof state));data::PropertyState other;auto wrong=data::property_view(rules,other);data::PlayerEquipmentQueriesLiveV1 mismatch(inventory,wrong);data::EquipmentWeaponFacts12V1 sentinel{11,12,13};ck(!mismatch.facts(sentinel,error)&&sentinel.main_category==11&&sentinel.off_category==12&&sentinel.flags==13);
 context.reject_constant=true;std::unique_ptr<data::ItemInstanceV1> failed;ck(!inventory.create_item(weapon_ids[0],1,failed,effects,error)&&!failed&&!error.empty());context.reject_constant=false;
 const auto old_pack=text.pack();ck(!text.switch_pack(9,false,error)&&text.pack()==old_pack);std::string output="retained";bool null=true;ck(text.integer_string(-1,context.environment().localization,output,null,error)&&null&&output=="retained");
 data::ItemTextResponseV5 class_response;std::string unused;ck(!services.invoke(services.context,class_probe,{data::ItemTextOperationV5::class_name,-1},class_response,unused,error));ck(!services.invoke(services.context,class_probe,{data::ItemTextOperationV5::class_name,std::int32_t(chars.rows.size())},class_response,unused,error));
 unsigned loaded_sheet=0;for(;loaded_sheet<37;++loaded_sheet)if(context.reached_files.count("text/"+text.sheet_filename(0,loaded_sheet)))break;ck(loaded_sheet<37);
 auto opened=context.opened,closed=context.closed;context.reject_open=true;ck(!text.preload(0,loaded_sheet,true,context.environment().localization,error)&&context.leases.empty()&&context.opened==opened&&context.closed==closed);context.reject_open=false;
 context.reject_close=true;ck(!text.preload(0,loaded_sheet,true,context.environment().localization,error)&&context.leases.empty()&&context.opened==opened+1&&context.closed==closed+1);context.reject_close=false;
 opened=context.opened;closed=context.closed;context.reject_debug=true;ck(!text.preload(0,loaded_sheet,true,context.environment().localization,error)&&context.leases.empty()&&context.opened==opened+1&&context.closed==closed+1);context.reject_debug=false;
 // Original sheet commit precedes the failing Debug/close continuation. The
 // retained loaded sheet bypasses a second read when force=false.
 opened=context.opened;closed=context.closed;ck(text.preload(0,loaded_sheet,false,context.environment().localization,error)&&context.opened==opened&&context.closed==closed);
 ck(context.leases.empty()&&context.opened==context.closed&&context.opened>0&&power_cases>0);
 std::cout<<"{\"validation\":\"PASS\",\"weapon_original_cases\":"<<queries<<",\"actual_item_constructors\":"<<item_cases<<",\"actual_power_text_cases\":"<<power_cases<<",\"null_power_retained_prefixes\":"<<null_power_prefixes<<",\"localized_class_rows\":9,\"live_query_cases\":4,\"required_provider_rejections\":4,\"class_bounds_rejections\":2,\"pack_bounds_rejections\":1,\"localized_null_branches\":1,\"loaded_sheet_retained_prefixes\":2,\"file_leases\":"<<context.opened<<",\"checks\":"<<checks<<",\"mismatches\":0,\"text_files\":[";bool comma=false;for(const auto& name:context.reached_files){if(comma)std::cout<<',';comma=true;std::cout<<'\"'<<name<<'\"';}std::cout<<"]}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
