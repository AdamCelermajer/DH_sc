#include "../../audio_named_animation_sound_v38.hpp"
#include "../../../level-world/character_melee_animation_event_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool x){if(!x)throw std::runtime_error("named animation source mismatch");}
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct Reader {const std::vector<std::uint8_t>& b;std::size_t at{};
 std::uint32_t word(){check(b.size()-at>=4);std::uint32_t w{};std::memcpy(&w,b.data()+at,4);at+=4;return w;}
 std::string text(){const auto n=word();check(n<=b.size()-at);std::string s(reinterpret_cast<const char*>(b.data()+at),n);at+=n;return s;}
};
struct Fixture {
 const audio::AudioSourceBindingsV38& bindings;std::vector<unsigned> trace;
 std::uintptr_t current_manager{1};int fail{};character::CombatSoundPlayV1 captured{};
 audio::AudioNamedAnimationSoundResultV38 leaf;
 static int invoke(void* raw,const character::skills::MeleeAnimationRequestV1* q,character::skills::MeleeAnimationResponseV1* out){
  auto& f=*static_cast<Fixture*>(raw);using namespace character::skills;
  if(q->operation==melee_event_step_index){f.trace.push_back(1);out->value=7;return 0;}
  if(q->operation==melee_event_step_count){f.trace.push_back(2);out->value=9;return 0;}
  if(q->operation!=melee_event_sound_fx)return -1;
  audio::AudioNamedAnimationSoundServicesV38 s{&f,2,
   [](void* raw,std::uintptr_t& manager){auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(3);manager=f.current_manager;return f.fail==3?-1:0;},
   [](void* raw,std::uintptr_t actor,std::array<float,3>& position){auto& f=*static_cast<Fixture*>(raw);check(actor==2);f.trace.push_back(4);f.current_manager=9;const std::uint32_t bits[]={0x42c80000,0xc3480000,0x80000000};std::memcpy(position.data(),bits,12);return f.fail==4?-1:0;},
   [](void* raw,const character::CombatSoundPlayV1& p){auto& f=*static_cast<Fixture*>(raw);f.trace.push_back(5);f.captured=p;check(p.manager==1&&p.target==2&&!p.source_bool&&p.source_integer==1&&p.source_float0==-1&&p.source_float1==-1);return f.fail==5?-1:0;}};
  return audio::audio_named_animation_sound_v38(q->text,f.bindings,s,f.leaf);
 }
};
}
int main(int argc,char** argv){try{
 check(argc==3);const std::string binding=argv[1],reference=argv[2];
 auto records=read(binding+"/sdd_dungeon_hunter_2_iphone_pyarray.bin"),names=read(binding+"/sdd_dungeon_hunter_2_iphone_pyarraynames.bin");
 audio::AudioSourceBindingsV38 table;std::string error;check(table.load(records.data(),records.size(),names.data(),names.size(),error));check(table.names().size()==638);
 auto gold=read(reference+"/named-animation-gold.bin");Reader r{gold};check(r.word()==0x384d414e);const auto count=r.word();unsigned checks=0;
 for(unsigned i=0;i<count;++i){auto name=r.text();const auto id=static_cast<std::int32_t>(r.word());const auto original_queries=r.word();Fixture f{table,{},1,0,{}, {}};character::skills::MeleeAnimationServicesV1 services{&f,Fixture::invoke,nullptr};character::skills::MeleeAnimationOutputV1 out{};
  check(character::skills::dh2_character_melee_animation_event_v1(&out,name.c_str(),&services)==0);check(out.step==7&&out.calls==3);check(f.leaf.source_id==id);
  if(id<0){check(f.trace==std::vector<unsigned>{1,2});check(original_queries==2);}else{check(f.trace==std::vector<unsigned>{1,2,3,4,5});check(original_queries==4);check(f.captured.sound_id==id);const std::uint32_t bits[]={0x42c80000,0xc3480000,0x80000000};check(!std::memcmp(f.captured.position.data(),bits,12));}
  ++checks;
 }
 check(r.at==gold.size());
 for(int fail:{3,4,5}){Fixture f{table,{},1,fail,{}, {}};character::skills::MeleeAnimationServicesV1 services{&f,Fixture::invoke,nullptr};character::skills::MeleeAnimationOutputV1 out{};check(character::skills::dh2_character_melee_animation_event_v1(&out,"sfx_ChestOpen",&services)==-2);check(f.leaf.required&&f.leaf.phase==unsigned(fail-1));check(f.trace.size()==unsigned(fail));++checks;}
 std::cout<<"PASS named-animation="<<checks<<" original="<<count<<" failures=3\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
