#include "character_template_assets_v35.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace dh2;
static void require(bool value,const std::string&error){if(!value)throw std::runtime_error(error);}
int main(int argc,char**argv){if(argc!=2)return 2;try{
 auto read=[&](const char*name){std::ifstream in(std::string(argv[1])+"/"+name,std::ios::binary);require(bool(in),"Fixture missing");return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(in)),{});};
 auto records=read("character_templates_pyarray.bin"),names=read("character_templates_pyarraynames.bin"),schema=read("character_templates_pystructnames.bin"),gold=read("selection-original-gold-v35.bin");
 auto bytes=[](const std::vector<std::uint8_t>&v){return data::Bytes{v.data(),v.size()};};std::string error;loader::CharacterTemplateAssetsV35::Borrow retained;
 {loader::CharacterTemplateAssetsV35 owner;require(owner.load(bytes(records),bytes(names),bytes(schema),error),error);retained=owner.borrow();require(retained.names().size()==121&&retained.rows().size()==121,"Original template table count differs");require(!owner.load(bytes(records),bytes(names),bytes(schema),error)&&error=="Character template snapshot has live borrowers","Reload invalidated pinned template table");}
 std::size_t at=0;auto word=[&](){require(at+4<=gold.size(),"Short original selector gold");std::uint32_t v;std::memcpy(&v,gold.data()+at,4);at+=4;return v;};const auto count=word();
 for(unsigned i=0;i<count;++i){auto n=word();require(n<=gold.size()-at,"Short gold key");std::string name(reinterpret_cast<const char*>(gold.data()+at),n);at+=n;data::LootRandom8V2 random{word(),word()};auto properties=static_cast<std::int16_t>(word()),template_id=static_cast<std::int16_t>(word());auto expected_properties=static_cast<std::int16_t>(word()),expected_template=static_cast<std::int16_t>(word());auto expected_seed=word(),expected_calls=word();require(retained.select(name,properties,template_id,random,error),error);require(properties==expected_properties&&template_id==expected_template&&random.seed==expected_seed&&random.calls==expected_calls,"Original selector/RNG/cache result differs");
  if(properties!=-1){const auto before=random;require(retained.select(name,properties,template_id,random,error)&&random.seed==before.seed&&random.calls==before.calls,"Selected template replay rerolled application RNG");}}
 require(at==gold.size()&&count==371,"Original selector corpus count/suffix differs");
 loader::CharacterTemplateAssetsV35 atomic;require(atomic.load(bytes(records),bytes(names),bytes(schema),error),error);auto short_records=records;short_records.pop_back();require(!atomic.load(bytes(short_records),bytes(names),bytes(schema),error),"Truncated table fabricated success");require(atomic.borrow().names()==retained.names()&&atomic.borrow().rows()==retained.rows(),"Malformed reload changed retained table");
 auto malformed_schema=schema;malformed_schema.back()^=1;require(!atomic.load(bytes(records),bytes(names),bytes(malformed_schema),error),"Foreign template schema fabricated success");
 std::cout<<"PASS original_rows=121 original_selector_cases=371 source_rng_cache_verified=1 snapshot_survives_owner=1 malformed_atomic=1\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
