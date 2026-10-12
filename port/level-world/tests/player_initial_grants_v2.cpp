#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../player_initial_grants_v2.hpp"
#include "../../game-data/player_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2::player;using Raw=std::vector<std::uint8_t>;
static unsigned checks;static void ck(bool b){++checks;if(!b)throw std::runtime_error("Initial grants check "+std::to_string(checks));}
static Raw file(const char* p){std::ifstream f(p,std::ios::binary);ck(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
static void w(Raw& b,std::uint32_t n){for(int i=0;i<4;++i)b.push_back(std::uint8_t(n>>(8*i)));}
static std::uint32_t rd(const Raw& b,std::size_t& at){ck(at<=b.size()&&b.size()-at>=4);std::uint32_t n;std::memcpy(&n,b.data()+at,4);at+=4;return n;}
static std::int32_t signed_word(std::uint32_t n){std::int32_t v;std::memcpy(&v,&n,4);return v;}
struct Fixture {std::int32_t values[12]{};bool increment{},after{},test{},nested{};unsigned difficulty_calls{},delivered{};Raw requests;std::int32_t level{},points{},capacity{165};int fail{-1};dh2::data::PlayerSavegameV1* save{};};
static int service(void* p,const InitialGrantRequest32V2* q,InitialGrantResponse8V2* out){auto& c=*static_cast<Fixture*>(p);ck(q&&out&&q->owner==UINT64_C(0x123400005678));if(int(c.delivered++)==c.fail)return 1;w(c.requests,q->operation);for(auto v:q->arguments)w(c.requests,v);auto* a=q->arguments;out->value=0;out->reserved=0;
 if(c.increment){switch(q->operation){
 case has_savegame:case has_saved_rows:out->value=1;break;
 case property_integer:out->value=a[0]==157?c.points:c.values[8];break;
 case skill_available:out->value=c.values[1];break;
 case skill_limit:out->value=c.values[2+a[0]];break;
 case difficulty_unlocked:out->value=c.values[c.difficulty_calls++?6:5];break;
 case saved_level_read:out->value=c.save?c.save->skill_level(0):c.level;break;
 case can_increment:out->value=c.values[7];break;
 case property_add:--c.points;if(c.values[9])c.level=c.values[10];break;
 case saved_level_increment:if(c.save){std::string e;ck(c.save->set_skill_level(0,c.save->skill_level(0)+1,e));}else c.level=std::uint16_t(c.level+1);break;
 case potion_capacity_store:c.capacity=a[0];break;
 default:break;
 }}else{switch(q->operation){
 case online:out->value=c.values[0];break;case online_player_record:out->value=c.values[1];break;
 case num_items:out->value=c.values[c.after?5:2];break;case read_gold:out->value=c.values[3];break;case loot_property:out->value=c.values[4];break;
 case add_loot:c.after=true;break;case is_equippable:out->value=(std::uint32_t(c.values[6])>>a[0])&1;break;
 case has_skill_slots:out->value=c.save?c.save->has_skill_slots():c.values[7];break;
 case skill_level:out->value=c.save?c.save->skill_level(0):c.values[8];break;
 case set_skill_slot:if(c.save){std::string e;dh2::data::SavedSkillUpdateServicesV1 updates{nullptr,[](void*,std::uintptr_t id,std::string&){ck(id==UINT64_C(0x123400005678));return true;}};ck(c.save->set_skill_in_slot(a[0],a[1],updates,e));}break;
 case increment_skill:if(c.save){c.increment=true;c.level=c.save->skill_level(0);c.points=1;c.values[1]=c.values[7]=1;c.values[2]=5;c.values[5]=c.values[6]=0;std::int32_t accepted=0;InitialGrantServices16V2 same{&c,service};ck(!dh2_player_increment_skill_v2(&accepted,q->owner,a[0],a[1],&same)&&accepted==1);c.increment=false;}break;
 default:out->value=c.values[9];break;
 }}return 0;}
int main(int argc,char** argv){try{ck(argc==4);auto grant=file(argv[1]);std::size_t at=0;ck(rd(grant,at)==0x32564750);auto cases=rd(grant,at);unsigned requests=0,guards=0;
 for(unsigned k=0;k<cases;++k){Fixture c;auto kind=rd(grant,at);for(unsigned i=0;i<10;++i)c.values[i]=signed_word(rd(grant,at));auto n=rd(grant,at);ck(n*24<=grant.size()-at);Raw expected(grant.begin()+at,grant.begin()+at+n*24);at+=n*24;InitialGrantServices16V2 s{&c,service};ck(!(kind?dh2_player_initial_skill_slots_v2(UINT64_C(0x123400005678),&s):dh2_player_initial_equipment_v2(UINT64_C(0x123400005678),&s))&&c.requests==expected);requests+=n;}ck(at==grant.size());
 auto skill=file(argv[2]);at=0;ck(rd(skill,at)==0x32564950);auto skill_cases=rd(skill,at);for(unsigned k=0;k<skill_cases;++k){Fixture c;c.increment=true;c.test=rd(skill,at);for(auto& v:c.values)v=signed_word(rd(skill,at));auto expected_result=rd(skill,at);auto replaced=rd(skill,at),level=rd(skill,at),capacity=rd(skill,at),points=rd(skill,at);auto n=rd(skill,at);ck(n*24<=skill.size()-at);Raw expected(skill.begin()+at,skill.begin()+at+n*24);at+=n*24;c.points=c.values[0];c.level=c.values[11];InitialGrantServices16V2 s{&c,service};std::int32_t result=-99;ck(!dh2_player_increment_skill_v2(&result,UINT64_C(0x123400005678),0,c.test,&s)&&std::uint32_t(result)==expected_result&&c.requests==expected&&std::uint32_t(c.points)==points&&std::uint32_t(c.level)==level&&std::uint32_t(c.capacity)==capacity&&replaced==std::uint32_t(c.values[9]&&result&&!c.test));requests+=n;}ck(at==skill.size());
 // Actual immutable SkillList14 is the source KnightPlayerBase row producer,
 // not an arbitrary dictionary-ID assignment. Update services below are
 // explicit controlled providers, separately counted rather than engine proof.
 auto directory=std::string(argv[3]);auto data=file((directory+"/skills_pyarray.bin").c_str()),names=file((directory+"/skills_pyarraynames.bin").c_str()),schema=file((directory+"/skills_pystructnames.bin").c_str());dh2::data::SkillTables tables;std::string error;ck(tables.load({data.data(),data.size()},{names.data(),names.size()},{schema.data(),schema.size()},error));auto b=tables.borrow();for(auto list:{14,21,27}){dh2::data::PlayerSavegameV1 save;save.set_character(UINT64_C(0x123400005678));ck(save.initialize_skills(b.lists().at(list),error));Fixture composed;composed.save=&save;InitialGrantServices16V2 services{&composed,service};ck(!dh2_player_initial_skill_slots_v2(save.character(),&services));ck(save.skill_in_slot(0)==0&&save.skill_level(0)==1&&save.skill_id(0)==b.lists()[list][0]);auto old=composed.requests;ck(!dh2_player_initial_skill_slots_v2(save.character(),&services)&&composed.requests.size()==old.size()+24);++guards;}
 for(unsigned i=0;i<7;++i){Fixture c;c.values[7]=0;c.fail=i;InitialGrantServices16V2 s{&c,service};ck(dh2_player_initial_skill_slots_v2(UINT64_C(0x123400005678),&s)==-2&&c.delivered==i+1);++guards;}
 Fixture c;InitialGrantServices16V2 s{&c,service};std::int32_t result=777;ck(dh2_player_increment_skill_v2(&result,0,0,0,&s)==-1&&result==777&&c.requests.empty());++guards;ck(dh2_player_increment_skill_v2(&result,1,-1,0,&s)==-1&&result==777);++guards;ck(dh2_player_initial_equipment_v2(1,nullptr)==-1);++guards;
 InitialGrantServices16V2 reserved{nullptr,[](void*,const InitialGrantRequest32V2*,InitialGrantResponse8V2* o){o->reserved=1;return 0;}};ck(dh2_player_initial_equipment_v2(1,&reserved)==-2);++guards;
 std::cout<<"{\"validation\":\"PASS\",\"original_initial_grant_cases\":"<<cases<<",\"original_increment_cases\":"<<skill_cases<<",\"ordered_requests\":"<<requests<<",\"actual_cache_owned_skill_slot_compositions\":3,\"skill_effect_providers\":\"explicit fixtures\",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
