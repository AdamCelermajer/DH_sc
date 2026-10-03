#include "../character_ai_frame.hpp"
#include "../character_ai_update.hpp"
#include "../character_script_update.hpp"
#include <array>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Reader {std::vector<unsigned char> bytes;std::size_t at=0;explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing frame gold");bytes={std::istreambuf_iterator<char>(f),{}};}template<class T>T get(){check(at+sizeof(T)<=bytes.size(),"Truncated frame gold");T value;std::memcpy(&value,bytes.data()+at,sizeof(T));at+=sizeof(T);return value;}};
constexpr std::uintptr_t base=0x100000000ull;
std::uintptr_t native(std::uintptr_t id){return id?base+id*0x10000:0;}
std::uintptr_t id(std::uintptr_t p){return p?(p-base)/0x10000:0;}
using Snapshot=std::array<unsigned char,176>;using Event=std::array<unsigned char,212>;
struct Context {AIFrameState32 frame{};std::array<AIFrameOwner48,2> owners{};ScriptUpdateState48 script{};AIUpdateState80 update{};std::array<unsigned,6> params{};std::vector<Event> events;unsigned fail=~0u;};
Snapshot snapshot(const Context& c){Snapshot out{};const std::uintptr_t prefix[2]={1,c.frame.owner==&c.owners[0]?2u:9u};std::memcpy(out.data(),prefix,16);std::memcpy(out.data()+16,&c.frame.paused,16);
 for(unsigned i=0;i<2;++i){auto owner=c.owners[i];owner.owner=id(owner.owner);owner.controller=id(owner.controller);std::memcpy(out.data()+32+i*48,&owner,48);}auto script=c.script;std::array<std::uintptr_t,4> p{};std::memcpy(p.data(),&script,32);for(auto& v:p)v=id(v);std::memcpy(&script,p.data(),32);std::memcpy(out.data()+128,&script,48);return out;}
void put(Context& c,const Snapshot& raw){std::uintptr_t owner;std::memcpy(&owner,raw.data()+8,8);c.frame.ai=native(1);c.frame.owner=&c.owners[owner==9];std::memcpy(&c.frame.paused,raw.data()+16,16);
 for(unsigned i=0;i<2;++i){std::memcpy(&c.owners[i],raw.data()+32+i*48,48);c.owners[i].owner=native(c.owners[i].owner);c.owners[i].controller=native(c.owners[i].controller);}std::memcpy(&c.script,raw.data()+128,48);std::array<std::uintptr_t,4> p{};std::memcpy(p.data(),&c.script,32);for(auto& v:p)v=native(v);std::memcpy(&c.script,p.data(),32);}
void event(Context& c,unsigned service,std::uintptr_t subject,unsigned value=0,unsigned argument=0,const float* position=nullptr){AIUpdateRequest32 request{service,argument,id(subject),{0,0,0},0};if(position)std::memcpy(request.position,position,12);Event out{};const auto raw=snapshot(c);std::memcpy(out.data(),&request,32);std::memcpy(out.data()+32,raw.data(),176);std::memcpy(out.data()+208,&value,4);c.events.push_back(out);}
void mutate(Context& c,unsigned service){if(c.params[2]!=service||!c.params[1])return;switch(c.params[1]){
 case 1:c.frame.owner=&c.owners[1];c.script.ai_owner=native(9);break;
 case 2:c.owners[0].zoned=255;break;
 case 3:c.owners[0].in_zone=0;break;
 case 4:c.frame.paused=255;c.script.paused=255;break;
 case 5:c.frame.global_blocked=255;break;
 case 6:c.owners[0].flags520=0;break;
 case 7:c.frame.owner=&c.owners[1];c.script.ai_owner=native(9);c.owners[0].zoned=255;c.owners[0].in_zone=0;break;
 case 8:c.frame.owner=&c.owners[1];c.script.ai_owner=native(9);c.owners[1].zoned=255;c.owners[1].in_zone=0;break;
 default:throw std::runtime_error("Unknown frame mutation");}}
void script_service(void* opaque,ScriptUpdateState48* s,const ScriptUpdateRequest32* r){auto& c=*static_cast<Context*>(opaque);c.frame.paused=s->paused;event(c,32+r->service,r->subject,0,r->service==0?1000:0);if(r->service==1)c.update.state=c.params[5];}
int update_service(void* opaque,AIUpdateState80*,const AIUpdateRequest32* r,unsigned* value){auto& c=*static_cast<Context*>(opaque);*value=r->service==3?c.params[0]:0;event(c,16+r->service,r->subject,*value,r->argument,r->position);
 if(r->service==0){ScriptUpdateServices16 callbacks{&c,script_service};check(dh2_character_script_update(&c.script,0,&callbacks)==1,"Actual selected AIS update failed");c.frame.paused=c.script.paused;}return 0;}
int frame_service(void* opaque,AIFrameState32*,const AIFrameRequest16* r,unsigned* value){auto& c=*static_cast<Context*>(opaque);*value=r->service==0?c.params[0]:0;event(c,r->service,r->subject,*value);mutate(c,r->service);if(c.fail==r->service)return 17;
 if(r->service==4&&c.params[3]){const auto& owner=*c.frame.owner;c.update={native(1),owner.owner,0,0,native(7),native(8),static_cast<int>(c.params[5]),1,owner.zoned,0,{0,0,0},0};const unsigned position[3]={0x80000000,0x7fc01234,0x7f800000};std::memcpy(c.update.saved_position,position,12);AIUpdateResult16 out{};AIUpdateServices24 callbacks{&c,update_service,255,0};check(dh2_character_ai_update(&out,&c.update,&callbacks)==0,"Actual complete OnUpdate failed");}return 0;}
Snapshot seed(){Context c;c.frame.ai=native(1);c.owners[0]={native(2),native(3),0x100,0,0,0,0,17,0,0};c.owners[1]={native(9),native(10),0x100,0,0,0,1,19,0,0};c.frame.owner=&c.owners[0];c.script={native(1),native(2),native(2),native(3),0,0,0,0};return snapshot(c);}
}
int main(int argc,char**argv){try{check(argc==2,"usage: character_ai_frame_audit frame-gold");Reader r(argv[1]);check(r.get<unsigned>()==0x31464143,"Wrong frame gold");const auto cases=r.get<unsigned>();unsigned requests=0,composed=0;const auto clean=seed();
 for(unsigned i=0;i<cases;++i){Context c;put(c,r.get<Snapshot>());c.params=r.get<std::array<unsigned,6>>();const auto expected=r.get<Snapshot>();const auto count=r.get<unsigned>();std::vector<Event> events;for(unsigned j=0;j<count;++j)events.push_back(r.get<Event>());AIFrameServices24 svc{&c,frame_service,31,0};AIFrameResult16 out{};check(dh2_character_ai_frame(&out,&c.frame,&svc)==0&&out.phase==6&&snapshot(c)==expected&&c.events==events,"Original frame state/order differs");requests+=count;composed+=bool(c.params[3]);}
 check(r.at==r.bytes.size(),"Trailing frame gold");unsigned guards=0,missing=0,failures=0;
 auto reject=[&](unsigned kind){Context c;put(c,clean);AIFrameServices24 svc{&c,frame_service,31,0};AIFrameResult16 out{11,12,13,14};auto* result=&out;auto* state=&c.frame;auto* callback=&svc;
  if(kind<3){if(kind==0)result=nullptr;if(kind==1)state=nullptr;if(kind==2)callback=nullptr;}
  else switch(kind){case 3:result=reinterpret_cast<AIFrameResult16*>(&c.frame);break;case 4:result=reinterpret_cast<AIFrameResult16*>(&svc);break;case 5:callback=reinterpret_cast<AIFrameServices24*>(&c.frame);break;case 6:c.frame.owner=reinterpret_cast<AIFrameOwner48*>(&c.frame);break;case 7:c.frame.owner=reinterpret_cast<AIFrameOwner48*>(&svc);break;case 8:c.frame.owner=reinterpret_cast<AIFrameOwner48*>(&out);break;case 9:c.frame.ai=0;break;case 10:c.frame.owner=nullptr;break;case 11:c.frame.paused=256;break;case 12:c.frame.global_blocked=256;break;case 13:c.frame.reserved0=1;break;case 14:c.frame.reserved1=1;break;case 15:c.owners[0].owner=0;break;case 16:c.owners[0].controller=0;break;case 17:c.owners[0].forced=256;break;case 18:c.owners[0].locked=256;break;case 19:c.owners[0].zoned=256;break;case 20:c.owners[0].in_zone=256;break;case 21:c.owners[0].updated88=256;break;case 22:c.owners[0].reserved0=1;break;case 23:c.owners[0].reserved1=1;break;case 24:svc.invoke=nullptr;break;case 25:svc.available=32;break;case 26:svc.reserved=1;break;default:throw std::runtime_error("Bad guard");}
  const auto prior=c;const auto prior_out=out;check(dh2_character_ai_frame(result,state,callback)==1&&!std::memcmp(&c.frame,&prior.frame,32)&&!std::memcmp(c.owners.data(),prior.owners.data(),96)&&!std::memcmp(&out,&prior_out,16)&&c.events.empty(),"Malformed frame partially committed");++guards;};
 for(unsigned i=0;i<27;++i)reject(i);
 for(unsigned service=0;service<5;++service){Context c;put(c,clean);AIFrameServices24 svc{&c,frame_service,31u^(1u<<service),0};AIFrameResult16 out{};check(dh2_character_ai_frame(&out,&c.frame,&svc)==2&&out.phase==service+1&&out.last_service==service&&c.events.size()==service,"Missing frame service silently accepted");++missing;}
 for(unsigned service=0;service<5;++service){Context c;put(c,clean);c.params={0,1,service,0,0,3};c.fail=service;AIFrameServices24 svc{&c,frame_service,31,0};AIFrameResult16 out{};check(dh2_character_ai_frame(&out,&c.frame,&svc)==3&&out.phase==service+1&&out.last_service==service&&c.events.size()==service+1&&c.frame.owner==&c.owners[1],"Frame failure effects/order hidden");++failures;}
 // Actual event31 clears pause and returns before AIS forwarding. This is a
 // source helper invocation, not fabricated timer expiry or a clock advance.
 Context c;put(c,clean);c.frame.paused=c.script.paused=255;check(dh2_character_script_pause_expired(&c.script)==1&&!c.script.paused,"Actual pause-expiry helper failed");c.frame.paused=c.script.paused;
 Dl_info frame{},update{},script{};check(dladdr(reinterpret_cast<void*>(&dh2_character_ai_frame),&frame)&&dladdr(reinterpret_cast<void*>(&dh2_character_ai_update),&update)&&dladdr(reinterpret_cast<void*>(&dh2_character_script_update),&script)&&std::string(update.dli_fname).find("libdh2_level_world.so")!=std::string::npos&&std::string(script.dli_fname).find("libdh2_level_world.so")!=std::string::npos,"Genuine world DSO bindings missing");
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_service_requests\":"<<requests<<",\"composed_cases\":"<<composed<<",\"atomic_rejections\":"<<guards<<",\"unavailable_services\":"<<missing<<",\"runtime_failure_prefixes\":"<<failures<<",\"pause_expiry_helper\":1,\"frame_library\":\""<<frame.dli_fname<<"\",\"on_update_library\":\""<<update.dli_fname<<"\",\"selected_library\":\""<<script.dli_fname<<"\",\"sanitizer_findings\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
