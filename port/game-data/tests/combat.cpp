#include "combat.hpp"
#include "combat_events.hpp"
#include "data.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
void check(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
std::uint32_t word(const std::vector<std::uint8_t>& data,std::size_t offset){check(offset<=data.size()&&data.size()-offset>=4,"Reference truncated");return std::uint32_t(data[offset])|(std::uint32_t(data[offset+1])<<8)|(std::uint32_t(data[offset+2])<<16)|(std::uint32_t(data[offset+3])<<24);}
std::int32_t integer(const std::vector<std::uint8_t>& data,std::size_t offset){auto bits=word(data,offset);std::int32_t result;std::memcpy(&result,&bits,4);return result;}
std::array<std::int32_t,224> sheet(const std::vector<std::uint8_t>& data,unsigned row){std::array<std::int32_t,224> result;for(unsigned i=0;i<224;++i)result[i]=integer(data,row*3584+2688+i*4);return result;}
int main(int argc,char** argv){try{
 if(argc!=5)return 2;
 const std::string chars=argv[1];auto values=read(chars+"/character_properties_pyarray.bin"),names=read(chars+"/character_properties_pyarraynames.bin"),fields=read(chars+"/character_properties_pystructnames.bin");CharacterTable table;std::string error;
 check(load_characters({values.data(),values.size()},{names.data(),names.size()},{fields.data(),fields.size()},table,error),error.c_str());auto spawn=read(argv[2]),damage_ref=read(argv[3]),route_ref=read(argv[4]);check(spawn.size()==448*3584,"Spawn reference dimensions differ");
 unsigned defender_row=0;while(defender_row<table.names.size()&&table.names[defender_row]!="Crypt_Skeleton")++defender_row;check(defender_row<table.names.size(),"Defender missing");auto defense=sheet(spawn,defender_row);CombatantView defender{defense.data(),-1,-1,0,0,1,5,0};
 auto damage_count=word(damage_ref,0);check(damage_count==1792&&damage_ref.size()==4+damage_count*64,"Damage reference dimensions differ");
 for(unsigned i=0;i<damage_count;++i){auto pos=4+i*64;auto row=word(damage_ref,pos);check(row<table.names.size(),"Damage character out of bounds");auto source=sheet(spawn,row);CombatantView attacker{source.data(),-1,-1,0,0,1,5,0};CombatRandom random{word(damage_ref,pos+20),word(damage_ref,pos+24)};Damage out;DamageRequest request{&attacker,&defender,&random,integer(damage_ref,pos+12),integer(damage_ref,pos+4),integer(damage_ref,pos+8),word(damage_ref,pos+16)};
  check(dh2_combat_damage(&out,&request)==0,"Damage rejected");std::array<std::int32_t,7> actual;static_assert(sizeof(out)==28);std::memcpy(actual.data(),&out,sizeof(out));for(unsigned j=0;j<7;++j)check(actual[j]==integer(damage_ref,pos+28+j*4),"Damage differs from original instructions");check(random.seed==word(damage_ref,pos+56)&&random.calls==word(damage_ref,pos+60),"Damage RNG differs from original instructions");
 }
 const char* event_names[]={"attack_mainhand","attack_offhand","attack_ranged","do_skill","unknown","Attack_mainhand","attack_mainhand_extra","attack",""};auto route_count=word(route_ref,0);check(route_count==6000&&route_ref.size()==4+route_count*40,"Route reference dimensions differ");
 for(unsigned i=0;i<route_count;++i){auto pos=4+i*40;CombatEventContext context{integer(route_ref,pos),integer(route_ref,pos+4),integer(route_ref,pos+8),word(route_ref,pos+12),integer(route_ref,pos+16)};auto name=word(route_ref,pos+20);check(name<9,"Event name out of bounds");CombatEventAction action;check(dh2_combat_event_route(&action,&context,event_names[name])==0,"Event route rejected");check(int(action.kind)==integer(route_ref,pos+24)&&action.sequence_step==integer(route_ref,pos+28)&&action.attack_step==integer(route_ref,pos+32)&&action.offhand==integer(route_ref,pos+36),"Route differs from original instructions");}
 Damage sentinel{1,2,3,4,5,6,7},before=sentinel;CombatRandom random{123,456};DamageRequest invalid{&defender,&defender,&random,0,0,0,16};check(dh2_combat_damage(&sentinel,&invalid)==1&&std::memcmp(&before,&sentinel,28)==0&&random.seed==123&&random.calls==456,"Invalid flags changed output/RNG");auto invalid_actor=defender;invalid_actor.properties=nullptr;invalid.attacker=&invalid_actor;invalid.flags=0;check(dh2_combat_damage(&sentinel,&invalid)==1&&std::memcmp(&before,&sentinel,28)==0&&random.calls==456,"Invalid actor changed output/RNG");
 CombatEventContext invalid_context{5,0,1,2,-1};CombatEventAction action{CombatEventKind::melee,123,456,1};check(dh2_combat_event_route(&action,&invalid_context,"attack_mainhand")==1&&action.sequence_step==123&&action.attack_step==456,"Invalid capability changed action");
 random={123,0xffffffffu};check(dh2_combat_random(&random,0)==0&&random.seed==123&&random.calls==0,"Zero-range RNG/count wrap differs");
 std::cout<<"{\"original_character_damage_cases\":"<<damage_count<<",\"original_attack_event_decisions\":"<<route_count<<",\"all_damage_words_and_rng_verified\":true,\"invalid_input_atomic\":true,\"zero_range_and_counter_wrap_verified\":true,\"unarmed_crypt_projectile_properties\":{";
 bool comma=false;for(unsigned i=0;i<table.names.size();++i)if(table.names[i]=="Crypt_Skeleton"||table.names[i]=="CryptSlime"||table.names[i]=="CryptSlime_RE"||table.names[i]=="Crypt_Ghost"){if(comma)std::cout<<',';comma=true;std::cout<<'"'<<table.names[i]<<"\":"<<sheet(spawn,i)[32];}std::cout<<"}}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
