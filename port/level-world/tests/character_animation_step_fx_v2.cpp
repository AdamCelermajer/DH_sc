#include "../character_animation_step_fx_v2.hpp"
#include "../../game-data/skill_tables.hpp"
#include <fstream>
#include <iterator>
#include <cstdio>
#include <vector>
#include <stdexcept>
using namespace dh2;
struct Fixture{std::vector<int> calls;int set=-1,fail=0;std::uintptr_t anchor=0;bool rotation=false,gate=true;float position[3]{};};
int main(int argc,char** argv){try{
 if(argc!=2)return 2;auto read=[&](const char* name){std::ifstream f(std::string(argv[1])+"/"+name,std::ios::binary);return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(f),{});};
 auto bytes=[](const auto& b){return data::Bytes{b.data(),b.size()};};
 auto n=read("animations_dictionary_pyarraynames.bin"),v=read("animations_dictionary_pyarray.bin");data::Dictionary dictionary;std::string error;if(!data::load_dictionary(bytes(n),bytes(v),dictionary,error))throw std::runtime_error(error);
 auto records=read("animations_pyarray.bin"),names=read("animations_pyarraynames.bin"),schema=read("animations_pystructnames.bin");data::AnimationTables tables;if(!data::load_animation_tables(bytes(records),bytes(names),bytes(schema),dictionary,tables,error))throw std::runtime_error(error);
 auto sr=read("skills_pyarray.bin"),sn=read("skills_pyarraynames.bin"),ss=read("skills_pystructnames.bin");data::SkillTables skills;if(!skills.load(bytes(sr),bytes(sn),bytes(ss),error))throw std::runtime_error(error);auto skill=skills.borrow();
 int checks=0;auto check=[&](bool good){++checks;if(!good)throw std::runtime_error("step FX check "+std::to_string(checks));};
 auto run=[&](const data::AnimationStep& step,Fixture& f){character::AnimationStepFxServicesV2 s{};s.context=&f;
 s.owner=[](void* p,std::uintptr_t& out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(1);out=0x123456789ull;return 0;};
 s.swoosh_fx_gate=[](void* p,const data::AnimationStep&,bool& out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(2);out=x.gate;return x.fail==2?-1:0;};
 s.target_position=[](void* p,std::uintptr_t id,float* out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(3);if(id!=0x123456789ull)return -1;out[0]=11;out[1]=-23;out[2]=37;return 0;};
 s.rotation=[](void* p,std::uintptr_t id,float* out){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(4);if(id!=0x123456789ull)return -1;out[0]=.1f;out[1]=.2f;out[2]=.3f;return x.fail==4?-1:0;};
 s.play=[](void* p,int set,const float* pos,const float* rot,std::uintptr_t anchor){auto& x=*static_cast<Fixture*>(p);x.calls.push_back(5);x.set=set;x.anchor=anchor;x.rotation=rot!=nullptr;for(unsigned i=0;i<3;++i)x.position[i]=pos[i];return x.fail==5?-1:0;};
 return character::character_animation_step_fx_v2(step,s,error);
 };
 for(const char* name:{"BashDown","Charge","GroundSlam"}){auto id=skill.skill_index(name);check(id>=0);auto sequence=std::int32_t(skill.skills()[id].scalar.words[1]);check(sequence>=0&&std::size_t(sequence)<tables.sequences.size());const auto& step=tables.sequences[sequence].steps.at(0);Fixture f;check(run(step,f)==0);check(f.set==step.fx&&f.anchor==0x123456789ull&&!f.rotation&&f.position[0]==0&&f.position[1]==0&&f.position[2]==0);check(f.calls==std::vector<int>{1,5});}
 auto step=tables.sequences.at(347).steps.at(0);step.anchor_fx=false;Fixture absolute;check(run(step,absolute)==0&&absolute.calls==std::vector<int>{1,3,4,5}&&!absolute.anchor&&absolute.rotation&&absolute.position[1]==-23);
 Fixture required;required.fail=4;check(run(step,required)<0&&required.calls==std::vector<int>{1,3,4});
 step.swoosh=true;Fixture gate;gate.gate=false;check(run(step,gate)==0&&gate.calls==std::vector<int>{2});
 step.fx=-1;Fixture invalid;check(run(step,invalid)==0&&invalid.calls==std::vector<int>{2});
 step.swoosh=false;Fixture noop;check(run(step,noop)==0&&noop.calls.empty());
 std::printf("PASS %d actual Knight cache/source step FX routing and failure-prefix checks; playback/GPU providers remain explicit fixtures\n",checks);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
