#include "../character_script_objects.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::character;
namespace {
unsigned checks=0;
void require(bool value,unsigned line){++checks;if(!value)throw std::runtime_error("Character _Rand test line "+std::to_string(line));}
#define check(x) require(bool(x),__LINE__)
using Raw=std::vector<std::uint8_t>;
std::uint32_t word(std::istream& in){std::uint32_t value{};in.read(reinterpret_cast<char*>(&value),4);check(in);return value;}
Raw blob(std::istream& in){Raw bytes(word(in));if(!bytes.empty())in.read(reinterpret_cast<char*>(bytes.data()),bytes.size());check(in);return bytes;}
Raw file(const char* path){std::ifstream in(path,std::ios::binary);check(in);return {std::istreambuf_iterator<char>(in),{}};}
struct Inputs {
 std::array<std::array<Raw,3>,5> tables;std::vector<Raw> constants;std::vector<dh2::data::Bytes> views;GameDesignInputs256 input{};
 explicit Inputs(const char* path){std::ifstream in(path,std::ios::binary);check(in&&word(in)==0x314f4447);
  for(auto& table:tables)for(auto& bytes:table)bytes=blob(in);auto count=word(in);for(unsigned i=0;i<count;++i){blob(in);constants.push_back(blob(in));}
  count=word(in);for(unsigned i=0;i<count;++i)blob(in);check(in.peek()==EOF);
  GameDesignTableInput48* target[]={&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};
  for(unsigned i=0;i<5;++i)*target[i]={{tables[i][0].data(),tables[i][0].size()},{tables[i][1].data(),tables[i][1].size()},{tables[i][2].data(),tables[i][2].size()}};
  for(auto& bytes:constants)views.push_back({bytes.data(),bytes.size()});input.constants=views.data();input.constant_count=views.size();
 }
};
struct Debug {
 std::string path;DebugSwitches* owner=dh2_character_debug_create();DebugFileServices24 files{this,open,close};
 explicit Debug(const char* value):path(value){check(owner);}
 ~Debug(){dh2_character_debug_destroy(owner);}
 static int open(void* raw,const char* name,std::uintptr_t* out){auto& self=*static_cast<Debug*>(raw);if(std::strcmp(name,"DebugSwitches.savegame"))return 1;errno=0;auto* file=std::fopen(self.path.c_str(),"rb");*out=reinterpret_cast<std::uintptr_t>(file);return file||errno==ENOENT?0:1;}
 static int close(void*,std::uintptr_t file){return !file||std::fclose(reinterpret_cast<std::FILE*>(file))?1:0;}
};
void script(dh2_script_vm* vm,const char* source){const auto status=dh2_script_vm_load_source_file(vm,source,std::strlen(source));if(status)std::cerr<<dh2_script_vm_error(vm)<<'\n';check(!status);}
void call(dh2_script_vm* vm,const char* function,std::uintptr_t id){dh2_script_value object{};object.type=DH2_SCRIPT_SOURCE_OBJECT;object.identity=id;check(!dh2_script_vm_call_discard_source_objects(vm,function,&object,1));}
}
int main(int argc,char** argv){try{
 check(argc==2);Inputs raw(argv[1]);CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));auto tables=design.borrow();Debug debug("/tmp/dh2-rand-debug-switches-missing");CharacterScriptObjects objects(design.borrow(),debug.owner,&debug.files);
 const auto character=std::find(tables.characters()->names.begin(),tables.characters()->names.end(),"Crypt_Skeleton");check(character!=tables.characters()->names.end());
 auto make=[&](std::uintptr_t id,const char* name){auto properties=std::make_shared<dh2::data::PropertyState>();auto life=std::make_shared<dh2::data::CombatActorState>();dh2::data::reset_properties(*tables.rules(),*properties,&tables.characters()->rows[character-tables.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*tables.classes(),*tables.rules(),*properties,error));return objects.add(id,name,properties,life,{0,0,0});};
 const auto first=make(UINT64_C(0x125000101),"first_npc"),second=make(UINT64_C(0x125000102),"second_npc");
 auto random=std::make_shared<dh2::data::LootRandom8V2>(dh2::data::LootRandom8V2{0x123456u,0});bool online=false;unsigned online_queries=0;
 check(objects.bind_source_random_channel0_v125(random,[&](bool& value,std::string&){++online_queries;value=online;return true;},error));
 auto* vm=dh2_script_vm_create(1024*1024);check(vm);check(!dh2_script_vm_set_source_objects(vm,&objects.services()));
 script(vm,"function RandBounds(o) local x=o:GetRand(0,100); assert(x>=0 and x<100) end; function RandOffset(o) local x=o:GetRand(7,12); assert(x>=7 and x<12) end; function RandZero(o) assert(o:GetRand(0)==0) end; function RandDefault(o) local x=o:GetRand(); assert(x>=0 and x<100) end; function RandOnline(o) local ok=pcall(function() return o:GetRand(0,100) end); assert(not ok) end");
 auto expected=*random;std::int32_t roll{};check(!dh2_loot_v2_random(&expected,100,&roll));call(vm,"RandBounds",first->identity);check(random->seed==expected.seed&&random->calls==expected.calls&&online_queries==1);
 expected=*random;check(!dh2_loot_v2_random(&expected,5,&roll));call(vm,"RandOffset",second->identity);check(random->seed==expected.seed&&random->calls==expected.calls&&online_queries==2);
 expected=*random;check(!dh2_loot_v2_random(&expected,0,&roll));call(vm,"RandZero",first->identity);check(random->seed==expected.seed&&random->calls==expected.calls&&online_queries==3);
 expected=*random;check(!dh2_loot_v2_random(&expected,100,&roll));call(vm,"RandDefault",second->identity);check(random->seed==expected.seed&&random->calls==expected.calls&&online_queries==4);
 online=true;const auto before=*random;call(vm,"RandOnline",first->identity);check(random->seed==before.seed&&random->calls==before.calls&&online_queries==5);
 dh2_script_vm_destroy(vm);
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"shared_channel0_across_two_characters\":true,\"range_cases\":4,\"zero_bound_advances_counter_without_seed\":true,\"online_fc_seed_unavailable_fails_without_rng_use\":true}\n";
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
