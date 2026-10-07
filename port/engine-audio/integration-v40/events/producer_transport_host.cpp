#include "audio_source_emission_v40.hpp"
#include "audio_authored_event_services_v40.hpp"
#include "audio_runtime_transport_v40.hpp"
#include "audio_session_transport_v40.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool v){++checks;if(!v)throw std::runtime_error("V40 producer contract mismatch");}
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct F {std::vector<int> trace;std::int64_t event=123456789;std::uintptr_t manager=7;character::CombatSoundPlayV1 delivered{};std::int64_t time{};bool accept=true;int fail{};
 static std::int64_t clock(void* p){auto& f=*static_cast<F*>(p);f.trace.push_back(4);return f.event;}
 static bool submit(void* p,const character::CombatSoundPlayV1& q,std::int64_t event,std::string& error){auto& f=*static_cast<F*>(p);f.trace.push_back(5);f.delivered=q;f.time=event;if(!f.accept)error="Required exact authored missing clip";return f.accept;}
 audio::AudioProducerTransportV40 transport(){return {this,submit,this,clock};}
};
}
int main(int argc,char** argv){try{
 check(argc==2);F f;std::string error;auto request=audio::audio_world_request_v38(7,2,-1,{1,2,3});
 check(audio::audio_submit_actual_producer_v40(f.transport(),request,error));check(f.trace==std::vector<int>{4,5});check(f.delivered.sound_id==-1&&f.time==f.event&&f.delivered.manager==7&&f.delivered.target==2);
 f.trace.clear();f.event=0;check(!audio::audio_submit_actual_producer_v40(f.transport(),request,error)&&f.trace==std::vector<int>{4});
 f.event=123456789;f.trace.clear();f.accept=false;check(!audio::audio_submit_actual_producer_v40(f.transport(),request,error));check(error=="Required exact authored missing clip");
 f.accept=true;auto missing=f.transport();missing.submit_actual_play=nullptr;f.trace.clear();check(!audio::audio_submit_actual_producer_v40(missing,request,error)&&f.trace.empty());
 audio::AudioSourceEmissionV40 container{&f,2,
 [](void* p,std::uintptr_t& manager,std::string&){auto& f=*static_cast<F*>(p);f.trace.push_back(1);manager=f.manager;return f.fail!=1;},
 [](void* p,std::int32_t& id,std::string&){auto& f=*static_cast<F*>(p);f.trace.push_back(2);f.manager=8;id=33;return f.fail!=2;},
 [](void* p,std::array<float,3>& pos,std::string&){auto& f=*static_cast<F*>(p);f.trace.push_back(3);f.manager=9;pos={100,-200,-0.f};return f.fail!=3;},f.transport()};
 f.trace.clear();f.manager=7;check(audio::audio_emit_container_source_v40(container,error));check(f.trace==std::vector<int>{1,2,3,4,5});check(f.delivered.manager==7&&f.delivered.sound_id==33&&!f.delivered.source_bool&&f.delivered.source_integer==1);
 for(int failure:{1,2,3}){f.fail=failure;f.trace.clear();check(!audio::audio_emit_container_source_v40(container,error));check(f.trace.size()==unsigned(failure));}f.fail=0;
 for(std::int16_t id:{std::int16_t(-1),std::int16_t(-32768),std::int16_t(32767)}){f.trace.clear();check(audio::audio_emit_actual_item_v40(f.transport(),7,55,id,{11,12,13},error));check(f.delivered.sound_id==id&&f.delivered.target==55&&f.delivered.position==std::array<float,3>{11,12,13});}
 const std::string binding=argv[1];auto records=read(binding+"/sdd_dungeon_hunter_2_iphone_pyarray.bin"),names=read(binding+"/sdd_dungeon_hunter_2_iphone_pyarraynames.bin");audio::AudioSourceBindingsV38 table;
 check(table.load(records.data(),records.size(),names.data(),names.size(),error));
 audio::AudioAuthoredEventBorrowV40 b;b.actual_names=&table;b.transport=f.transport();
 b.actual_named_source={&f,2,
 [](void* raw,std::uintptr_t& manager){auto& f=*static_cast<F*>(raw);f.trace.push_back(1);manager=f.manager;return 0;},
 [](void* raw,std::uintptr_t actor,std::array<float,3>& pos){auto& f=*static_cast<F*>(raw);check(actor==2);f.trace.push_back(2);f.manager=9;pos={100,-200,-0.f};return 0;},nullptr};
 b.remaining={&f,[](void* raw,const character::skills::MeleeAnimationRequestV1* q,character::skills::MeleeAnimationResponseV1* out){auto& f=*static_cast<F*>(raw);using namespace character::skills;
  if(q->operation==melee_event_step_index){f.trace.push_back(10);out->value=7;return 0;}
  if(q->operation==melee_event_step_count){f.trace.push_back(11);out->value=9;return 0;}
  f.trace.push_back(12);return -1;},nullptr};
 character::skills::MeleeAnimationOutputV1 out{};
 for(const auto id:{0,33,190,478,637}){f.trace.clear();f.manager=7;const auto name="sfx_"+table.names()[id];check(!audio::audio_dispatch_actual_authored_event_v40(out,name.c_str(),b,error));check(f.trace==std::vector<int>{10,11,1,2,4,5});check(f.delivered.sound_id==id&&f.delivered.manager==7&&f.delivered.target==2&&f.time==f.event);}
 f.trace.clear();check(!audio::audio_dispatch_actual_authored_event_v40(out,"sfx_unknown_v40",b,error));check(f.trace==std::vector<int>{10,11});
 f.trace.clear();check(audio::audio_dispatch_actual_authored_event_v40(out,"attack_mainhand",b,error)==-2);check(f.trace==std::vector<int>{10,11,12});
 f.trace.clear();f.accept=false;check(audio::audio_dispatch_actual_authored_event_v40(out,"sfx_ChestOpen",b,error)==-2);check(f.trace==std::vector<int>{10,11,1,2,4,5}&&error=="Required exact authored missing clip");
 std::cout<<"PASS V40 producer-contract checks="<<checks<<" source-names="<<table.names().size()<<" no-playback\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
