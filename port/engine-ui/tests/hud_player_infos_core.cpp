#include "hud_player_infos_core.hpp"
#include "script_constants.hpp"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::ui;
namespace {
void check(bool v,const std::string& e){if(!v)throw std::runtime_error(e);}
struct Fixture {std::int32_t sheet[44]{};HudInfosActor32 actor{};dh2_script_constants* constants=nullptr;unsigned calls=0;int seen_index=0,seen_remote=0,fail=0;Fixture(){sheet[36]=75;sheet[38]=100;sheet[41]=25;sheet[43]=100;sheet[33]=50;sheet[34]=100;actor={sheet,44,1,0,0,9,0};constants=dh2_script_constants_create();}~Fixture(){dh2_script_constants_destroy(constants);}
 static int invoke(void* p,const HudInfosRequest32* r,HudInfosResponse16* out){auto& t=*static_cast<Fixture*>(p);++t.calls;if(t.fail==int(r->operation))return 0;
  switch(r->operation){case HudInfosOperation::player:t.seen_index=static_cast<std::int32_t>(r->index);t.seen_remote=r->value;out->identity=reinterpret_cast<std::uintptr_t>(&t.actor);break;
  case HudInfosOperation::skill_slot:out->value=-1;break;
  case HudInfosOperation::spell_info:out->fraction=.75f;break;
  case HudInfosOperation::property_int:out->value=r->index==19?8:3;break;
  case HudInfosOperation::low_health_constant:return dh2_script_constants_get(t.constants,"CharacterDesign","LowHealthPercentage",&out->value)==0?1:0;
  case HudInfosOperation::spell_usable:out->value=1;break;
  case HudInfosOperation::potions:out->value=12;break;
  case HudInfosOperation::saved_dpad:out->value=0;break;
  default:throw std::runtime_error("Unexpected required HUD service");}
  return 1;
 }};
struct Reject:gameswf::as_object {unsigned rejected=0;explicit Reject(gameswf::player* p):as_object(p){}bool set_member(const tu_stringi& name,const gameswf::as_value& value)override{if(name=="PlayerActive"){++rejected;return false;}return as_object::set_member(name,value);}};
}
int main(int argc,char** argv){try{check(argc==2,"Expected actual design_pycst bytes");std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> data((std::istreambuf_iterator<char>(f)),{});Fixture t;dh2_script_constants_reload loaded{};check(dh2_script_constants_load(t.constants,data.data(),static_cast<unsigned>(data.size()),&loaded)==0,"Actual design constants failed");std::int32_t low=0;check(dh2_script_constants_get(t.constants,"CharacterDesign","LowHealthPercentage",&low)==0,"Source constant absent");gameswf::gc_ptr<gameswf::player> player=new gameswf::player;gameswf::gc_ptr<gameswf::movie_def_impl> definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);definition->set_frame_count(1);definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);definition->m_version=7;definition->m_frame_size.m_x_min=definition->m_frame_size.m_y_min=0;definition->m_frame_size.m_x_max=definition->m_frame_size.m_y_max=20;gameswf::gc_ptr<gameswf::root> root=definition->create_root();unsigned sessions=0,checks=0;std::string error;HudInfosServices16 services{&t,Fixture::invoke};
 for(unsigned mode=0;mode<7;++mode){gameswf::as_environment env(player.get_ptr());gameswf::gc_ptr<gameswf::as_object> object=new gameswf::as_object(player.get_ptr());gameswf::as_value result,that;const bool remote=mode%2;env.push(gameswf::as_value(remote));env.push(gameswf::as_value(mode==2?"2":mode==3?"-3":"0"));env.push(gameswf::as_value(object.get_ptr()));gameswf::fn_call call(&result,that,&env,3,2);t.actor.active=mode==4?0:1;t.actor.removed=mode==5?1:0;t.fail=mode==6?int(HudInfosOperation::potions):0;check(hud_player_infos_callback_v1(call,services,error)==(mode!=6),"Adapter delivery mismatch");++checks;
  gameswf::as_value v;check(object->get_member("PlayerActive",&v)&&v.to_bool()==(mode!=4&&mode!=5),"Active object write mismatch");++checks;
  if(mode==4||mode==5){check(!object->get_member("HP_PCT",&v),"Inactive unexpectedly produced values");++checks;}else {check(object->get_member("HP_PCT",&v)&&v.to_number()==75,"Real object HP wrong");check(object->get_member("HP_LOWPCT",&v)&&v.to_number()==low,"Owned source constant wrong");check(object->get_member("MP_PCT",&v)&&v.to_number()==25,"MP wrong");check(object->get_member("XP_PCT",&v)&&v.to_number()==50,"XP wrong");checks+=4;}
  if(mode!=6){check(result.to_object()==object.get_ptr(),"Returned source object identity changed");++checks;}else{check(result.is_undefined()&&!object->get_member("NB_POTIONS",&v),"Failed prefix published result/suffix");++checks;}check(t.seen_remote==int(remote)&&t.seen_index==(mode==2?2:mode==3?-3:0),"Actual AS argument coercion wrong");++checks;++sessions;
 }
 {gameswf::as_environment env(player.get_ptr());gameswf::gc_ptr<Reject> object=new Reject(player.get_ptr());gameswf::as_value result,that;env.push(gameswf::as_value(0));env.push(gameswf::as_value(object.get_ptr()));gameswf::fn_call call(&result,that,&env,2,1);t.fail=0;t.actor.active=1;t.actor.removed=0;check(hud_player_infos_callback_v1(call,services,error)&&object->rejected==1,"Source ignored set_member return changed");gameswf::as_value value;check(object->get_member("TouchToMove",&value)&&value.to_bool(),"Readonly prefix stopped source suffix");checks+=2;++sessions;}
 {gameswf::as_environment env(player.get_ptr());gameswf::as_value result,that;env.push(gameswf::as_value(0));env.push(gameswf::as_value(7));gameswf::fn_call call(&result,that,&env,2,1);check(!hud_player_infos_callback_v1(call,services,error)&&result.is_undefined(),"Primitive output accepted");++checks;gameswf::fn_call invalid(&result,that,&env,0,1);const auto before=t.calls;check(hud_player_infos_callback_v1(invalid,services,error)&&t.calls==before,"Wrong arity touched providers");++checks;}
 player->clear_heap();std::cout<<"{\"validation\":\"PASS\",\"actual_AS_sessions\":"<<sessions<<",\"adapter_checks\":"<<checks<<",\"provider_deliveries\":"<<t.calls<<",\"genuine_low_health_constant\":"<<low<<",\"constants_assignments\":"<<loaded.assignments<<"}\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
