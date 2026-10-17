#include "objects.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace {
std::vector<std::uint8_t> read(const std::string& path) {
 std::ifstream stream(path,std::ios::binary);
 if(!stream)throw std::runtime_error("fixture missing: "+path);
 return {std::istreambuf_iterator<char>(stream),{}};
}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {return {value.data(),value.size()};}
void check(bool value,const char* message) {if(!value)throw std::runtime_error(message);}
}
int main(int argc,char** argv) {
 try {
  if(argc!=3)return 2;
  const std::string baseline=argv[1],gated=argv[2];
  auto a=read(gated+"/data/character_properties_pyarray.bin"),
   b=read(gated+"/data/character_properties_pyarraynames.bin"),
   c=read(gated+"/data/character_properties_pystructnames.bin"),
   d=read(gated+"/data/character_models_dictionary_pyarraynames.bin"),
   e=read(gated+"/data/character_models_dictionary_pyarray.bin");
  dh2::data::CharacterTable characters;dh2::data::Dictionary models;std::string error;
  check(dh2::data::load_characters(bytes(a),bytes(b),bytes(c),characters,error),"character fixture rejected");
  check(dh2::data::load_dictionary(bytes(d),bytes(e),models,error),"model fixture rejected");
  const auto old=read(baseline+"/worlds/crypt01.dact"),next=read(gated+"/worlds/crypt01.dact");
  std::vector<dh2::objects::Record> before,after;
  auto load=[&](const auto& raw,auto& records){return dh2::objects::load_records(raw.data(),raw.size(),8,characters,models,records,error);};
  check(load(old,before)&&before.size()==95,"v1 baseline changed");
  check(load(next,after)&&after.size()==97,"v2 gated actors absent");
  for(const auto& record:before) {
   check(!record.gated_spawn,"v1 record unexpectedly gated");
   const auto found=std::find_if(after.begin(),after.end(),[&](const auto& r){return r.room==record.room&&r.name==record.name;});
   check(found!=after.end()&&!found->gated_spawn&&found->kind==record.kind&&
    found->character==record.character&&found->model==record.model&&found->placement==record.placement,
    "v2 modified an automatic actor");
  }
  unsigned count=0,gate_index=0,decor_index=0;
  for(unsigned i=0;i<after.size();++i) {
   const auto& record=after[i];if(record.kind==2)decor_index=i;
   if(!record.gated_spawn)continue;
   ++count;gate_index=i;
   check(record.room==7&&record.kind==1&&record.character=="Crypt_Ghost"&&record.model=="ghost.bdae",
    "gated actor original table link differs");
   check(record.name=="_prim_Monster_SURPRISE_01"||record.name=="_prim_Monster_SURPRISE_02",
    "unresolved factory included");
   check(record.position[1]==19941.857f&&record.position[2]==609.727f,"gated authored placement differs");
  }
  check(count==2,"unexpected gated actor count");
  auto reject=[&](std::vector<std::uint8_t> raw){
   std::vector<dh2::objects::Record> result=after;
   check(!load(raw,result)&&result.empty(),"malformed descriptor leaked records");
  };
  auto bad=next;bad[16+gate_index*256+236]=2;reject(bad);
  bad=next;bad[16+decor_index*256+236]=1;reject(bad);
  bad=next;bad[16+gate_index*256+240]=1;reject(bad);
  bad=old;bad[16+236]=1;reject(bad);
  std::cout<<"{\"baseline_records\":95,\"gated_records\":97,\"gated_ghosts\":2,\"baseline_preserved\":true,\"malformed_gates_rejected\":4}\n";
 } catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
