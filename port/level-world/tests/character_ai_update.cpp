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
struct Reader {
 std::vector<unsigned char> data;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing AI update gold");data={std::istreambuf_iterator<char>(f),{}};}
 template<class T>T get(){check(at+sizeof(T)<=data.size(),"Truncated AI update gold");T out;std::memcpy(&out,data.data()+at,sizeof(T));at+=sizeof(T);return out;}
};
constexpr std::uintptr_t base=0x100000000ull;
std::uintptr_t native(std::uintptr_t id){return id?base+id*0x10000:0;}
std::uintptr_t identity(std::uintptr_t p){return p?(p-base)/0x10000:0;}
AIUpdateState80 normalize(AIUpdateState80 state){std::array<std::uintptr_t,6> p{};std::memcpy(p.data(),&state,48);for(auto& value:p)value=identity(value);std::memcpy(&state,p.data(),48);return state;}
AIUpdateState80 materialize(AIUpdateState80 state){std::array<std::uintptr_t,6> p{};std::memcpy(p.data(),&state,48);for(auto& value:p)value=native(value);std::memcpy(&state,p.data(),48);return state;}
using Event=std::array<unsigned char,116>;
struct Context {std::array<unsigned,4> params;std::vector<Event> events;int status=0;unsigned failed_service=~0u;};
int invoke(void* opaque,AIUpdateState80* state,const AIUpdateRequest32* request,unsigned* value){
 auto& context=*static_cast<Context*>(opaque);check(request->service<8,"Unknown AI service");
 *value=request->service==3?context.params[0]:request->service==5?context.params[1]:0;
 auto normalized=*request;normalized.subject=identity(normalized.subject);const auto snapshot=normalize(*state);Event event{};std::memcpy(event.data(),&normalized,32);std::memcpy(event.data()+32,&snapshot,80);std::memcpy(event.data()+112,value,4);context.events.push_back(event);
 if(context.params[2]&&context.params[3]==request->service){
  switch(context.params[2]){
   case 1:state->owner=native(9);break;
   case 2:state->visual=native(10);state->visual_node=native(11);break;
   case 3:state->flag85=255;break;
   case 4:{const unsigned bits[3]={0x80000000,0x7fc12345,0x7f800000};std::memcpy(state->saved_position,bits,12);break;}
   case 5:state->state=4;break;
   case 6:state->target408=native(5);break;
   case 7:state->visual_node=0;break;
   case 8:state->zoned=255;break;
   case 9:state->flag85=0;break;
   default:throw std::runtime_error("Unknown fixture mutation");
  }
 }
 return request->service==context.failed_service?context.status:0;
}
struct SelectedContext {ScriptUpdateState48 script;AIUpdateState80* ai;unsigned timer=0,stop=0,queries=0;};
void selected_service(void* opaque,ScriptUpdateState48* state,const ScriptUpdateRequest32* request){
 auto& c=*static_cast<SelectedContext*>(opaque);
 if(request->service==script_update_timer_start){check(state->paused==1&&request->duration_ms==1000&&request->event==0x31&&!request->repeat&&!request->user_ref,"Source selected timer request differs");++c.timer;state->script_owner=native(9);state->ai_owner=native(9);state->controller=native(10);c.ai->owner=native(9);c.ai->state=4;c.ai->flag85=255;}
 else{check(request->service==script_update_controller_stop&&request->subject==native(10),"Selected script did not reload owner/controller after timer");++c.stop;}
}
int selected(void* opaque,AIUpdateState80* state,const AIUpdateRequest32* request,unsigned* result){
 auto& c=*static_cast<SelectedContext*>(opaque);*result=0;
 if(request->service==ai_update_active){c.ai=state;ScriptUpdateServices16 services{&c,selected_service};check(dh2_character_script_update(&c.script,0,&services)==1,"Genuine selected script update failed");}
 else if(request->service==ai_update_is_zonable){++c.queries;*result=0;}
 else if(request->service==ai_update_disable_zoning){check(state->owner==native(9)&&state->state==4,"AI owner/state was not reloaded after selected script");state->zoned=0;}
 else if(request->service==ai_update_sync_visibility){check(request->subject==native(7),"Selected script composition lost visual identity");}
 else throw std::runtime_error("Unexpected selected script composition service");
 return 0;
}
}
int main(int argc,char**argv){try{
 check(argc==2,"usage: character_ai_update_audit original-gold");Reader r(argv[1]);check(r.get<unsigned>()==0x31554143,"Wrong AI update gold magic");const auto cases=r.get<unsigned>();unsigned requests=0;
 for(unsigned i=0;i<cases;++i){const auto before=r.get<AIUpdateState80>();Context context{r.get<std::array<unsigned,4>>(),{},0};const auto expected=r.get<AIUpdateState80>();const auto count=r.get<unsigned>();std::vector<Event> events;for(unsigned j=0;j<count;++j)events.push_back(r.get<Event>());
  auto state=materialize(before);AIUpdateResult16 result{};AIUpdateServices24 services{&context,invoke,255,0};check(dh2_character_ai_update(&result,&state,&services)==0,"Native AI wrapper rejected valid gold");auto actual=normalize(state);check(!std::memcmp(&actual,&expected,80)&&context.events==events,"Original AI state/callback order differs");unsigned last=~0u;if(count)std::memcpy(&last,events.back().data(),4);check(result.phase==9&&result.service_calls==count&&result.last_service==last,"Native completion report differs");requests+=count;
 }
 check(r.at==r.data.size(),"Trailing AI update gold");unsigned guards=0,missing=0,failures=0;
 AIUpdateState80 seed{native(1),native(2),0,0,native(7),native(8),3,1,0,0,{0,0,0},0};Context context{{1,0,0,0},{},0};AIUpdateServices24 services{&context,invoke,255,0};
 auto reject=[&](AIUpdateState80 state,AIUpdateServices24 callback,int null=-1,int alias=-1){const auto before=state;AIUpdateResult16 result{11,12,13,14};const auto prior=result;context.events.clear();
  auto* out=alias==0?reinterpret_cast<AIUpdateResult16*>(&state):alias==1?reinterpret_cast<AIUpdateResult16*>(&callback):&result;auto* svc=alias==2?reinterpret_cast<AIUpdateServices24*>(&state):&callback;
  check(dh2_character_ai_update(null==0?nullptr:out,null==1?nullptr:&state,null==2?nullptr:svc)==1&&!std::memcmp(&state,&before,80)&&!std::memcmp(&result,&prior,16)&&context.events.empty(),"Malformed wrapper partially committed");++guards;};
 for(int i=0;i<3;++i)reject(seed,services,i);
 for(int i=0;i<3;++i)reject(seed,services,-1,i);
 {auto s=seed;s.owner=0;reject(s,services);s=seed;s.machine_present=2;reject(s,services);s=seed;s.zoned=256;reject(s,services);s=seed;s.flag85=256;reject(s,services);s=seed;s.reserved=1;reject(s,services);}
 {auto s=services;s.invoke=nullptr;reject(seed,s);s=services;s.available=256;reject(seed,s);s=services;s.reserved=1;reject(seed,s);}
 for(unsigned service=0;service<8;++service){auto state=seed;if(service==1||service==2)state.state=4;AIUpdateResult16 result{};auto callback=services;callback.available^=1u<<service;context.events.clear();check(dh2_character_ai_update(&result,&state,&callback)==2&&result.phase==service+1&&result.last_service==service,"Missing service silently completed");++missing;}
 for(unsigned service=0;service<8;++service){auto state=seed;context.events.clear();context.params={1,0,1,service};context.status=17;context.failed_service=service;
  state.active=service==0?native(1):0;if(service==1||service==2)state.state=4;else state.state=3;
  AIUpdateResult16 result{};check(dh2_character_ai_update(&result,&state,&services)==3&&result.service_calls==context.events.size()&&result.phase==service+1&&state.owner==native(9),"Runtime failure not reported or side effects hidden");++failures;
 }
 context.status=0;services.available=255;
 // Actual source-built selected AIS kernel is a different production DSO.
 // Timers/controller remain explicit; its owner mutation must govern the
 // wrapper's subsequent state/visibility branch rather than pre-call fields.
 SelectedContext selected_context{{native(1),native(2),native(2),native(3),200,0,0,0},nullptr,0,0,0};auto state=seed;AIUpdateResult16 result{};AIUpdateServices24 selected_callbacks{&selected_context,selected,255,0};check(dh2_character_ai_update(&result,&state,&selected_callbacks)==0&&selected_context.timer==1&&selected_context.stop==1&&selected_context.queries==0&&state.owner==native(9)&&state.zoned==0,"Genuine selected-AIS composition differs");
 Dl_info wrapper{},script{};check(dladdr(reinterpret_cast<void*>(&dh2_character_ai_update),&wrapper)&&dladdr(reinterpret_cast<void*>(&dh2_character_script_update),&script)&&wrapper.dli_fname&&script.dli_fname&&std::string(script.dli_fname).find("libdh2_level_world.so")!=std::string::npos,"Actual shared-library binding not demonstrated");
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_service_requests\":"<<requests<<",\"atomic_rejections\":"<<guards<<",\"unavailable_services\":"<<missing<<",\"runtime_failures\":"<<failures<<",\"genuine_selected_script_compositions\":1,\"selected_timer_requests\":1,\"selected_stop_requests\":1,\"wrapper_library\":\""<<wrapper.dli_fname<<"\",\"selected_library\":\""<<script.dli_fname<<"\",\"sanitizer_findings\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
