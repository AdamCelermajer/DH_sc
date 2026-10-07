#include "character_deferred_script.hpp"


#include <cstddef>
namespace {
using namespace dh2::character;
struct RequiredFailure {};
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
void checked(void* context,ScriptLifecycleState64* state,const ScriptLifecycleRequest32* request,ScriptLifecycleResponse16* response){
 const auto& services=*static_cast<const DeferredScriptServices16*>(context);
 if(services.invoke(services.context,state,request,response))throw RequiredFailure{};
}
}
extern "C" int dh2_character_deferred_script(dh2::character::ScriptLifecycleState64* state,
 std::uint32_t operation,std::uint32_t argument,const dh2::character::DeferredScriptServices16* services){
 using namespace dh2::character;
 if(!state||!services||!services->invoke||overlap(state,sizeof(*state),services,sizeof(*services))||
  (operation!=script_load_process&&operation!=script_init_process&&operation!=script_load_and_init&&operation!=script_on_init_post&&operation!=script_on_init_final))return -1;
 const ScriptLifecycleServices16 bridge{const_cast<DeferredScriptServices16*>(services),&checked};
 try{return dh2_character_script_lifecycle(state,operation,argument,&bridge);}
 catch(...){return -2;}
}
