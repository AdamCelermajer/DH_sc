#include "ai.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"AI input unavailable");return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};}
template<class T>T value(const std::uint8_t* p){T out;std::memcpy(&out,p,sizeof out);return out;}
template<class T>void append(std::vector<std::uint8_t>& out,T v){const auto* p=reinterpret_cast<const std::uint8_t*>(&v);out.insert(out.end(),p,p+sizeof v);}
void string(std::vector<std::uint8_t>& out,const std::string& s){append(out,std::uint32_t(s.size()));out.insert(out.end(),s.begin(),s.end());}
bool same_float(unsigned a,unsigned b){return a==b||(std::isnan(value<float>(reinterpret_cast<const std::uint8_t*>(&a)))&&std::isnan(value<float>(reinterpret_cast<const std::uint8_t*>(&b))));}
int main(int argc,char** argv){try{
 check(argc==3,"Usage: ai_audit data-directory original-reference.bin");const char* names[]={"ai_pyarray.bin","ai_pyarraynames.bin","ai_pystructnames.bin","ai_factions_pyarray.bin","ai_factions_pyarraynames.bin","ai_factions_pystructnames.bin"};std::array<std::vector<std::uint8_t>,6> data;for(unsigned i=0;i<6;++i)data[i]=read(std::string(argv[1])+"/"+names[i]);auto bytes=[&](unsigned i){return Bytes{data[i].data(),data[i].size()};};AiTables tables;std::string error;
 check(load_ai(bytes(0),bytes(1),bytes(2),bytes(3),bytes(4),bytes(5),tables,error),error.c_str());check(tables.rows.size()==76&&tables.factions.size()==16,"Original AI dimensions differ");
 std::vector<std::uint8_t> encoded;append(encoded,std::uint32_t(tables.rows.size()));for(const auto& p:tables.rows){append(encoded,p.attack_delay);append(encoded,p.combat_beat);append(encoded,p.combat_music);append(encoded,std::uint8_t(p.delayed_load));append(encoded,p.flags);append(encoded,p.interact_radius);append(encoded,p.leash_distance);append(encoded,p.melee_radius);append(encoded,p.on_aggro_sfx);string(encoded,p.script);append(encoded,p.self_fx);append(encoded,p.trophy);append(encoded,p.type);append(encoded,p.view_radius);append(encoded,p.view_radius_no_aggro);}check(encoded==data[0],"AI packed reader roundtrip differs");
 encoded.clear();append(encoded,std::uint32_t(tables.factions.size()));for(const auto& row:tables.factions){append(encoded,std::uint32_t(row.size()));for(auto e:row){append(encoded,e.id);append(encoded,e.value);}}check(encoded==data[3],"Faction packed roundtrip differs");
 check(ai_props(tables,-1)==&tables.rows[8]&&ai_props(tables,76)==&tables.rows[8],"AI fallback differs");check(ai_enemy(tables,7,11,false,true)&&!ai_enemy(tables,7,7,false,false),"Monster faction differs");
 const auto reference=read(argv[2]);check(reference.size()>=12,"AI reference truncated");const unsigned geometry=value<unsigned>(reference.data()),factions=value<unsigned>(reference.data()+4),targets=value<unsigned>(reference.data()+8);check(reference.size()==12ull+geometry*48ull+factions*16ull+targets*52ull,"AI reference dimensions differ");auto* at=reference.data()+12;
 for(unsigned i=0;i<geometry;++i,at+=48){auto r=value<AiRangeRequest>(at);auto expected=value<AiRangeResult>(at+36);AiRangeResult actual{};check(!dh2_ai_range(&actual,&r)&&same_float(actual.distance_bits,expected.distance_bits)&&actual.melee==expected.melee&&actual.sight==expected.sight,"AI geometry differs");}
 for(unsigned i=0;i<factions;++i,at+=16){const auto owner=value<unsigned>(at),target=value<unsigned>(at+4),players=value<unsigned>(at+8),expected=value<unsigned>(at+12);check(ai_enemy(tables,owner,target,players&1,players>>1)==bool(expected),"AI faction differs");}
 for(unsigned i=0;i<targets;++i,at+=52){auto r=value<AiTargetRequest>(at);auto expected=value<AiTargetResult>(at+16);AiTargetResult actual{};check(!dh2_ai_target_update(&actual,&r)&&!std::memcmp(&actual,&expected,sizeof actual),"Target transition differs");}
 for(unsigned i=0;i<data[0].size();i+=17){const auto old=data[0];data[0].resize(i);check(!load_ai(bytes(0),bytes(1),bytes(2),bytes(3),bytes(4),bytes(5),tables,error)&&tables.rows.empty(),"Truncated AI accepted");data[0]=old;}
 AiTargetResult sentinel{1,2,3,4,5,{6,7,8},9},actual=sentinel;AiTargetRequest invalid{3,1024,0,0};check(dh2_ai_target_update(&actual,&invalid)==1&&!std::memcmp(&sentinel,&actual,sizeof actual),"Invalid AI partially wrote output");
 std::cout<<"{\"geometry_cases\":"<<geometry<<",\"faction_cases\":"<<factions<<",\"target_event_cases\":"<<targets<<",\"original_ai_rows_roundtripped\":76,\"original_faction_rows_roundtripped\":16,\"mismatches\":0,\"truncated_reader_inputs_rejected\":299}"<<std::endl;return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
