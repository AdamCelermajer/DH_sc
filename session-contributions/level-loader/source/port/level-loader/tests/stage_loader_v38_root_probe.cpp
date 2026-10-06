#include "stage_loader_v38_root_file.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <iomanip>
#include <tuple>
using namespace dh2::loader;
static void check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
int main(){try{unsigned cases=0;
 for(const std::string name:{"001_swamp.mlx","x005_crypt.rnd","nodot","folder/x05_area.mlx","abc.part.mlx"})for(unsigned procedural=0;procedural<2;++procedural)for(unsigned online=0;online<2;++online)for(unsigned generated=0;generated<2;++generated){
  auto level=std::make_shared<int>();std::uint32_t state=7,progress=0,counter=0,current=123,dc=77,e0=99,gdc=0,ge0=0,file13c=5;std::uint8_t proc=procedural;std::string actual_name=name;unsigned root_calls=0;std::vector<std::string> events;
  Stage7FieldsV38 fields{{level,1,{&progress,&state,&counter,&current}},&dc,&e0,&proc,&actual_name};Stage7ServicesV38 services;
  services.online_byte5=[&](std::uint8_t& byte,std::string&){check(gdc==77&&ge0==99,"globals not produced before online");events.push_back("online");byte=online;return true;};
  services.construct_stream=[&](GeneratedSourceStreamV38& s,std::string&){events.push_back("construct");s={std::make_shared<int>(),4};return true;};
  services.generate=[&](const GeneratedSourceStreamV38&,std::uint32_t seed,bool& result,std::string&){events.push_back("generate");check(seed==(online?99u:77u),"wrong original global seed");result=generated;return LifecycleStepV36::complete;};
  services.assign_stream=[&](const LifecycleBorrowV36& b,const GeneratedSourceStreamV38&,std::string&){check(b.actual_level_owner==level,"foreign Level assignment");events.push_back("assign");return true;};
  services.destroy_stream=[&](GeneratedSourceStreamV38&,std::string&){events.push_back("destroy");return true;};
  auto expected_name=name;if(procedural&&!generated){expected_name[0]='x';auto dot=expected_name.find('.');if(dot!=std::string::npos){expected_name.resize(dot);expected_name+="_BACKUP.mlx";}}
  services.root_file_step=[&](const std::string& filename,const char* root,std::string&){events.push_back("root");check(filename==expected_name&&std::string(root)=="Level"&&current==500,"source filename/root/counter mismatch");return ++root_calls==3?LifecycleStepV36::complete:LifecycleStepV36::pending;};
  services.increment_actual_file13c=[&](std::string&){check(state==8,"source file13c increment before130");++file13c;return true;};
  Stage7BodyV38 body(fields,{level,&gdc,&ge0},services);auto result=LifecycleStepV36::pending;for(unsigned tick=0;tick<30&&result==LifecycleStepV36::pending;++tick)result=body.step();check(result==LifecycleStepV36::complete&&file13c==5&&state==7,"stage7 counterfeit dispatcher advancement");state=8;std::string error;check(body.after_source_increment(error)&&file13c==6,"source post130 file13c increment");check(body.after_source_increment(error)&&file13c==6,"post increment replay");
  std::vector<std::string> expected;if(procedural){expected={"online","construct","generate"};if(generated)expected.push_back("assign");expected.push_back("destroy");}expected.insert(expected.end(),{"root","root","root"});check(events==expected,"original stage7 event order mismatch");auto before=events.size();body.step();check(events.size()==before,"stage7 replay");++cases;
  std::cout<<"{\"filename\":"<<std::quoted(name)<<",\"procedural\":"<<procedural<<",\"online\":"<<online<<",\"generated_fixture\":"<<generated<<",\"final_name\":"<<std::quoted(actual_name)<<",\"state130\":"<<state<<",\"file13c\":"<<file13c<<",\"current138\":"<<current<<",\"event_names\":[";
  for(std::size_t i=0;i<events.size();++i){if(i)std::cout<<',';std::cout<<std::quoted(events[i]);}std::cout<<"]}\n";

 }
 std::cout<<"PASS original_stage7_fixture_cases="<<cases<<" actual_root_factory_effects_verified=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
