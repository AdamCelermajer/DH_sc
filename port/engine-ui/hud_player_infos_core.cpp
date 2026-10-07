#include "hud_player_infos_core.hpp"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"
#include <cmath>
#include <exception>
namespace dh2::ui {
namespace {
std::int32_t trunc32(double n){if(std::isnan(n))return 0;if(n>=2147483648.)return INT32_MAX;if(n<=-2147483648.)return INT32_MIN;return static_cast<std::int32_t>(n);}
struct Context {gameswf::as_object* output;const HudInfosServices16* producer;
 static int invoke(void* ptr,const HudInfosRequest32* r,HudInfosResponse16* out){auto& c=*static_cast<Context*>(ptr);if(r->operation!=HudInfosOperation::write_member)return c.producer->invoke?c.producer->invoke(c.producer->context,r,out):0;
  const char* name=hud_infos_member_name(static_cast<HudInfosMember>(r->index));if(!name||r->output_object!=reinterpret_cast<std::uintptr_t>(c.output))return 0;
  gameswf::as_value value;if(r->type==1)value.set_bool(r->value!=0);else if(r->type==2)value.set_double(static_cast<double>(r->value));else return 0;
  // Original object virtual set_member return is ignored, including read-only
  // member rejection. The exact object's setter/watch machinery executes.
  c.output->set_member(name,value);return 1;
 }};
}
bool hud_player_infos_callback_v1(const gameswf::fn_call& f,const HudInfosServices16& producer,std::string& error){if(f.nargs!=2&&f.nargs!=3)return true;
 try{gameswf::gc_ptr<gameswf::as_object> output=f.arg(0).is_object()?f.arg(0).to_object():nullptr;if(!output){error="NativeGetPlayerHUDInfos requires source AS object";return false;}
  const auto index=trunc32(f.arg(1).to_number());const auto remote=f.nargs==3&&f.arg(2).to_bool();Context c{output.get_ptr(),&producer};HudInfosServices16 s{&c,Context::invoke};HudInfosInput24 in{reinterpret_cast<std::uintptr_t>(output.get_ptr()),index,static_cast<unsigned>(remote),static_cast<unsigned>(f.nargs),0};const int rc=dh2_ui_hud_player_infos_v1(&in,&s);if(rc){error=rc==-1?"NativeGetPlayerHUDInfos malformed projection":"NativeGetPlayerHUDInfos required provider failed";return false;}
  if(f.result)f.result->set_as_object(output.get_ptr());return true;
 }catch(const std::exception& e){error=e.what();return false;}}
}
