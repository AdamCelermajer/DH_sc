#include "combat_application.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
#ifdef DH2_PLAYER_DEFENDER
unsigned player_idle=0;
unsigned apply(MonsterApplication* out,const MonsterApplicationRequest* request){return dh2_combat_apply_monster_to_player(out,request,player_idle);}
#elif defined(DH2_PLAYER_ATTACKER)
constexpr auto apply=dh2_combat_apply_player_to_monster;
#else
constexpr auto apply=dh2_combat_apply_monster;
#endif
namespace {
void check(bool v,const char* e){if(!v)throw std::runtime_error(e);}
template<class T>T load(const std::uint8_t* p){T v;std::memcpy(&v,p,sizeof v);return v;}
}
int main(int argc,char** argv){try{
 if(argc!=3)return 2;std::ifstream rules_file(std::string(argv[1])+"/character_properties_pyarray.bin",std::ios::binary);std::array<std::uint8_t,1800> raw{};rules_file.read(reinterpret_cast<char*>(raw.data()),raw.size());check(bool(rules_file),"Missing original property rules");PropertyRules rules;std::memcpy(rules.defaults.data(),raw.data()+4,896);std::memcpy(rules.types.data(),raw.data()+900,896);
 std::ifstream reference(argv[2],std::ios::binary);std::uint32_t count=0;reference.read(reinterpret_cast<char*>(&count),4);check(bool(reference)&&count,"Missing application reference");
#ifdef DH2_PLAYER_DEFENDER
 std::array<std::uint8_t,14600> record{};
#else
 std::array<std::uint8_t,14596> record{};
#endif
std::array<PropertyState,2> owners{};std::array<CombatActorState,2> states{};MonsterApplication application{};unsigned kills=0,hits=0;std::array<unsigned,8> statuses{};
 for(unsigned i=0;i<count;++i){reference.read(reinterpret_cast<char*>(record.data()),record.size());check(bool(reference),"Truncated application reference");const auto* r=record.data();std::size_t at=0;
#ifdef DH2_PLAYER_DEFENDER
  player_idle=load<unsigned>(r+14596);
#endif
  for(auto& owner:owners)for(auto* sheet:{&owner.base,&owner.saved,&owner.gear,&owner.resolved}){std::memcpy(sheet->data(),r+at,896);at+=896;}
  std::memcpy(states.data(),r+7168,40);auto result=load<CombatResult>(r+7208);auto attacker=property_view(rules,owners[0]),defender=property_view(rules,owners[1]);MonsterApplicationRequest request{&result,&attacker,&defender,&states[0],&states[1]};
  check(apply(&application,&request)==0,"Application fixture rejected");at=7248;
  for(const auto& owner:owners)for(const auto* sheet:{&owner.base,&owner.saved,&owner.gear,&owner.resolved}){check(std::memcmp(sheet->data(),r+at,896)==0,"Application owner differs from original");at+=896;}
  check(sizeof(CombatActorState)==20&&sizeof(CombatResult)==40&&sizeof(MonsterApplication)==100,"Application ABI dimensions differ");check(std::memcmp(states.data(),r+14416,40)==0&&std::memcmp(&result,r+14456,40)==0&&std::memcmp(&application,r+14496,100)==0,"Application state/result/requests differ from original");kills+=application.health.kill_requested;hits+=application.hit_called;for(unsigned bit=0;bit<8;++bit)statuses[bit]+=bool(application.status_requests&(1u<<bit));
 }
 check(reference.peek()==std::char_traits<char>::eof(),"Unexpected reference suffix");auto snapshot=owners;auto actor_snapshot=states;auto output_snapshot=application;auto attacker=property_view(rules,owners[0]),defender=property_view(rules,owners[1]);CombatResult result;result.mask=0x400000;const auto result_snapshot=result;MonsterApplicationRequest request{&result,&attacker,&defender,&states[0],&states[1]};
 auto unchanged=[&]{return std::memcmp(owners.data(),snapshot.data(),sizeof owners)==0&&std::memcmp(states.data(),actor_snapshot.data(),sizeof states)==0&&std::memcmp(&application,&output_snapshot,100)==0&&std::memcmp(&result,&result_snapshot,40)==0;};
 check(apply(&application,&request)==1&&unchanged(),"Unsupported gold path mutated application");
#ifdef DH2_PLAYER_DEFENDER
 result.mask=0;const auto idle_result_snapshot=result;
 check(dh2_combat_apply_monster_to_player(&application,&request,2)==1&&std::memcmp(owners.data(),snapshot.data(),sizeof owners)==0&&std::memcmp(states.data(),actor_snapshot.data(),sizeof states)==0&&std::memcmp(&application,&output_snapshot,100)==0&&std::memcmp(&result,&idle_result_snapshot,40)==0,"Invalid idle fact mutated application");
#endif
result.mask=0;states[0].dead=2;const auto invalid_state=states;check(apply(&application,&request)==1&&std::memcmp(states.data(),invalid_state.data(),sizeof states)==0&&std::memcmp(owners.data(),snapshot.data(),sizeof owners)==0&&std::memcmp(&application,&output_snapshot,100)==0,"Invalid actor changed output/owners");states=actor_snapshot;defender.types=nullptr;check(apply(&application,&request)==1&&std::memcmp(owners.data(),snapshot.data(),sizeof owners)==0&&std::memcmp(&application,&output_snapshot,100)==0&&apply(nullptr,&request)==1&&apply(&application,nullptr)==1,"Invalid property view changed state/output");
 std::cout<<"{\"original_application_cases\":"<<count<<",\"owner_words_per_case\":1792,\"result_and_actor_state_words\":20,\"application_words\":25,\"kill_requests\":"<<kills<<",\"hit_calls\":"<<hits<<",\"status_requests\":[";for(unsigned i=0;i<8;++i){if(i)std::cout<<',';std::cout<<statuses[i];}std::cout<<"],\"invalid_input_atomic\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
