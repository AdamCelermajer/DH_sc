#include "character_host_context.hpp"
#include <cstdio>
#include <cstring>
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
std::int32_t integer(float f){
 std::uint32_t b;std::memcpy(&b,&f,4);auto e=(b>>23)&255;
 if(e==255&&(b&0x7fffff))return 0;
 if(e<127)return 0;
 if(e>=158)return b>>31?INT32_MIN:INT32_MAX;
 auto magnitude=(b&0x7fffff)|0x800000u;
 magnitude=e>=150?magnitude<<(e-150):magnitude>>(150-e);
 return b>>31?-static_cast<std::int32_t>(magnitude):static_cast<std::int32_t>(magnitude);
}
int send(const HostContextServices16* s,std::uint32_t operation,HostContextService service,std::int32_t value,HostContextResponse16& response){
 const HostContextRequest16 request{service,operation,value,0};response={};
 return s->invoke(s->context,&request,&response)||response.reserved?-2:1;
}
int push(const HostContextServices16* s,std::uint32_t operation,std::int32_t value){HostContextResponse16 response{};return send(s,operation,host_push_integer,value,response);}
struct Capture {const HostContextServices16* source;dh2_script_value* values;std::uint32_t count,capacity;};
int capture(void* context,const HostContextRequest16* request,HostContextResponse16* response){
 auto& c=*static_cast<Capture*>(context);
 if(request->service!=host_push_integer)return c.source->invoke(c.source->context,request,response);
 if(c.count>=c.capacity)return 1;
 auto& value=c.values[c.count++];value={};value.type=DH2_SCRIPT_NUMBER;value.number=static_cast<float>(request->value);return 0;
}
int script(void* context,std::uint32_t operation,const dh2_script_value* args,std::uint32_t count,dh2_script_value* values,std::uint32_t capacity,std::uint32_t* returned,char* error,std::size_t size){
 if(!returned||!values||capacity<(operation==host_level_range?2u:1u)){if(error&&size)std::snprintf(error,size,"host context output capacity");return 1;}
 *returned=0;auto* b=static_cast<const HostContextBindings16*>(context);
 if(!aligned(b,alignof(HostContextBindings16))||!b->services.invoke){if(error&&size)std::snprintf(error,size,"host context owner missing");return 1;}
 Capture captured{&b->services,values,0,capacity};HostContextServices16 services{&captured,capture};
 auto result=dh2_character_host_context_query(operation,args,count,&services);
 if(result<0){if(error&&size)std::snprintf(error,size,"host context source provider/data failure (%d)",result);return 1;}
 *returned=captured.count;return 0;
}
int level(void* c,const dh2_script_value* a,std::uint32_t n,dh2_script_value* v,std::uint32_t z,std::uint32_t* r,char* e,std::size_t s){return script(c,0,a,n,v,z,r,e,s);}
int difficulty(void* c,const dh2_script_value* a,std::uint32_t n,dh2_script_value* v,std::uint32_t z,std::uint32_t* r,char* e,std::size_t s){return script(c,1,a,n,v,z,r,e,s);}
int range(void* c,const dh2_script_value* a,std::uint32_t n,dh2_script_value* v,std::uint32_t z,std::uint32_t* r,char* e,std::size_t s){return script(c,2,a,n,v,z,r,e,s);}
}
extern "C" int dh2_character_host_context_query(std::uint32_t operation,const dh2_script_value* args,std::uint32_t count,const HostContextServices16* s){
 if(operation>host_level_range||!aligned(s,alignof(HostContextServices16))||!s->invoke||count>1048576||(count&&!aligned(args,alignof(dh2_script_value))))return -1;
 HostContextResponse16 response{};
 auto result=send(s,operation,operation==host_player_level?host_get_player:host_get_current_level,0,response);if(result<0)return result;
 if(operation==host_player_level){auto* p=static_cast<const HostPlayer8*>(response.data);if(!aligned(p,alignof(HostPlayer8))||p->reserved)return -2;return push(s,operation,p->cached_level);}
 if(operation==host_player_difficulty){if(!response.data)return push(s,operation,0);auto* p=static_cast<const HostLevel8*>(response.data);if(!aligned(p,alignof(HostLevel8)))return -2;return push(s,operation,p->difficulty);}
 auto* level=static_cast<const HostLevel8*>(response.data);if(!aligned(level,alignof(HostLevel8)))return -2;
 const auto index=level->row_index;
 if(index==-1){result=push(s,operation,-1);return result<0?result:push(s,operation,-1);}
 const auto tier=count&&args[0].type==DH2_SCRIPT_NUMBER?integer(args[0].number):0;
 if(tier<0||tier>2)return 1;
 result=send(s,operation,host_get_range_rows,index,response);if(result<0)return result;
 auto valid=[&](){return index>=0&&response.count<=1048576&&static_cast<std::uint32_t>(index)<response.count&&aligned(response.data,alignof(LevelRangeRow24));};
 if(!valid())return -2;
 const auto minimum=static_cast<const LevelRangeRow24*>(response.data)[index].minimum[tier];
 result=push(s,operation,minimum);if(result<0)return result;
 result=send(s,operation,host_get_range_rows,index,response);if(result<0)return result;
 if(!valid())return -2;
 return push(s,operation,static_cast<const LevelRangeRow24*>(response.data)[index].maximum[tier]);
}
extern "C" int dh2_character_host_context_sync_level(HostPlayer8* player,const dh2::data::PropertyView* properties){
 if(!aligned(player,alignof(HostPlayer8))||player->reserved)return 1;
 std::int32_t level_value=0;if(dh2_character_get_level(&level_value,properties))return 1;
 const auto target=reinterpret_cast<std::uintptr_t>(player);
 auto overlaps=[&](const void* data,std::size_t size){auto at=reinterpret_cast<std::uintptr_t>(data);return target<=at?at-target<sizeof(*player):target-at<size;};
 if(overlaps(properties,sizeof(*properties)))return 1;
 for(auto sheet:{properties->defaults,properties->types,properties->base,static_cast<const std::int32_t*>(properties->saved),properties->gear,static_cast<const std::int32_t*>(properties->resolved)})if(overlaps(sheet,224*4))return 1;
 player->cached_level=level_value;return 0;
}
extern "C" int dh2_character_host_context_bind(dh2_script_vm* vm,const HostContextBindings16* b){
 if(!vm||!aligned(b,alignof(HostContextBindings16))||!b->services.invoke)return -1;
 auto result=dh2_script_vm_bind_source_values(vm,"GetHostPlayerLevel",level,const_cast<HostContextBindings16*>(b));if(result)return result;
 result=dh2_script_vm_bind_source_values(vm,"GetHostPlayerDifficulty",difficulty,const_cast<HostContextBindings16*>(b));if(result)return result;
 return dh2_script_vm_bind_source_values(vm,"GetCurrentLevelRange",range,const_cast<HostContextBindings16*>(b));
}
