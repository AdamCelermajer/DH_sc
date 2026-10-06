#include "swf_menu_save_slots.hpp"
#include "swf_frame_connection.hpp"
#include "swf_input_history.hpp"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_environment.h"
#include <limits>
#include <cstdio>
#include <stdexcept>
using namespace dh2::ui;
static void check(bool v,const char* why){if(!v)throw std::runtime_error(why);}
struct Fixture {
 std::string calls;unsigned slot=99;bool flush_ok=true,erase_ok=true;
 static bool flush(void* p,std::string& e){auto& f=*static_cast<Fixture*>(p);f.calls+='F';e=f.flush_ok?"":"flush rejected";return f.flush_ok;}
 static bool erase(void* p,unsigned slot,std::string& e){auto& f=*static_cast<Fixture*>(p);f.calls+='E';f.slot=slot;e=f.erase_ok?"":"erase rejected";return f.erase_ok;}
};
int main(){try{
 gameswf::gc_ptr<gameswf::player> player=new gameswf::player;
 auto history=std::make_shared<SwfInputHistory>();SwfFrameConnection frames;std::string error;
 check(history->bind(player.get_ptr(),error)&&frames.bind(player.get_ptr(),history,error),"bind");
 gameswf::gc_ptr<gameswf::movie_def_impl> def=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
 def->set_frame_count(1);def->m_playlist.resize(1);def->m_init_action_list.resize(1);
 gameswf::gc_ptr<gameswf::root> root=def->create_root();gameswf::as_environment env(player.get_ptr());
 Fixture f;SwfFrontEraseSlotServicesV1 services{&f,Fixture::flush,Fixture::erase};gameswf::as_value result(55),call_this;
 auto invoke=[&](const gameswf::as_value& arg,int nargs=1){
  f.calls.clear();result.set_double(55);env.push(arg);
  gameswf::fn_call fn(&result,call_this,&env,nargs,env.get_top_index());
  const bool ok=swf_front_erase_save_slot_v1(fn,services,error);env.drop(1);
  check(result.to_number()==55,"AS result modified");return ok;
 };
 for(double v:{0.,1.75,2.,3.99,-.5})check(invoke(gameswf::as_value(v))&&f.calls=="FE"&&f.slot==unsigned(int(v)),"numeric truncation/order");
 check(invoke(gameswf::as_value("2"))&&f.calls=="FE"&&f.slot==2,"actual AS numeric string conversion");
 check(invoke(gameswf::as_value(true))&&f.calls=="FE"&&f.slot==1,"actual AS boolean conversion");
 check(invoke(gameswf::as_value(-1.9))&&f.calls.empty(),"negative no-op");
 for(double v:{4.,2147483648.,std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()})check(!invoke(gameswf::as_value(v))&&f.calls.empty(),"unsafe number");
 f.flush_ok=false;check(!invoke(gameswf::as_value(2))&&f.calls=="F"&&error=="flush rejected","flush failure deleted");
 f.flush_ok=true;f.erase_ok=false;check(!invoke(gameswf::as_value(2))&&f.calls=="FE"&&error=="erase rejected","erase failure");
 services.flush_jobs=nullptr;check(!invoke(gameswf::as_value(2))&&f.calls.empty(),"missing flush");
 check(!invoke(gameswf::as_value(2),0)&&f.calls.empty(),"missing argument");
 std::puts("PASS real AS erase conversion/truncation, negative no-op, flush-before-erase, failure prefixes, unchanged result and bounded invalid calls");
}catch(const std::exception& e){std::fprintf(stderr,"FAIL %s\n",e.what());return 1;}}
