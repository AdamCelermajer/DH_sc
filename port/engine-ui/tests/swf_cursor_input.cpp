#include "swf_input_connection.hpp"
#include "swf_frame_connection.hpp"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_function.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace dh2::ui;
namespace {
unsigned check_index=0;void require(bool value,const std::string& error="Input assertion failed"){++check_index;if(!value)throw std::runtime_error(error+" [check "+std::to_string(check_index)+"]");}
struct Shape:gameswf::character_def {
 explicit Shape(gameswf::player*p):character_def(p){}
 bool point_test_local(float x,float y)override{return x>=0&&x<=400&&y>=0&&y<=400;}
};
struct Core {
 gameswf::gc_ptr<gameswf::player> player;
 gameswf::gc_ptr<gameswf::movie_def_impl> definition;
 gameswf::gc_ptr<gameswf::root> root;
 std::shared_ptr<SwfInputHistory> history=std::make_shared<SwfInputHistory>();
 SwfFrameConnection frames;
 unsigned* destroyed;
 explicit Core(unsigned*d):destroyed(d){std::string e;player=new gameswf::player;require(history->bind(player.get_ptr(),e),e);require(frames.bind(player.get_ptr(),history,e),e);
  definition=new gameswf::movie_def_impl(player.get_ptr(),gameswf::DO_NOT_LOAD_BITMAPS,gameswf::DO_NOT_LOAD_FONT_SHAPES);
  auto&r=definition->m_frame_size;r.m_x_min=0;r.m_x_max=9600;r.m_y_min=0;r.m_y_max=6400;
  definition->set_frame_count(1);definition->m_frame_rate=30;definition->m_playlist.resize(1);definition->m_init_action_list.resize(1);definition->inc_loading_frame();
  for(auto* label:{"focus_in","focus_out","pressed"})definition->m_named_frames.set(label,0);
  root=definition->create_root();
 }
 ~Core(){
  static_cast<gameswf::sprite_instance*>(root->get_root_movie())->m_display_list.clear();
  for(auto i=player->m_heap.begin();i!=player->m_heap.end();++i){auto*c=i->first.get_ptr();c->m_members.clear();c->m_proto=nullptr;if(auto*env=c->get_environment())env->m_target=nullptr;}
  root->get_root_movie()->m_members.clear();root->get_root_movie()->m_proto=nullptr;
  player->get_global()->m_members.clear();player->get_global()->m_proto=nullptr;player->clear_heap();definition->m_instance=nullptr;player->m_current_root=nullptr;
  root=nullptr;definition=nullptr;frames.release();history->release();player=nullptr;++*destroyed;
 }
 gameswf::sprite_instance* button(const char*name,int depth,float x){
  auto*parent=static_cast<gameswf::sprite_instance*>(root->get_root_movie());gameswf::gc_ptr<gameswf::sprite_instance> b=new gameswf::sprite_instance(player.get_ptr(),definition.get_ptr(),root.get_ptr(),parent,depth);
  b->set_name(name);gameswf::matrix m;m.set_identity();m.m_[0][2]=x;gameswf::cxform cx;
  parent->m_display_list.add_display_object(b.get_ptr(),depth,false,cx,m,0,0,0);
  gameswf::gc_ptr<Shape> shape=new Shape(player.get_ptr());gameswf::gc_ptr<gameswf::character> hit=shape->create_character_instance(b.get_ptr(),depth+10);hit->set_name("hitzone");m.set_identity();b->m_display_list.add_display_object(hit.get_ptr(),1,false,cx,m,0,0,0);
  return b.get_ptr();
 }
};
struct Calls {SwfInputConnection* connection{};SwfFrameConnection*frames{};std::vector<unsigned> events;unsigned as{},accepts{},advances{};bool release{},reject{},consume{},nested{},allow_advance{};unsigned* destroyed{};};
std::shared_ptr<SwfInputHistory> watched_history;unsigned watcher_calls=0;
void watcher(const gameswf::fn_call&f){SwfInputHistoryFlags flags{};std::string e;require(watched_history->read(static_cast<gameswf::character*>(f.this_ptr),flags,e),e);require(!flags.mouse9c,"Observer ran before watcher");++watcher_calls;*f.result=f.arg(2);}
Calls*active;
void method(const gameswf::fn_call&f){require(f.nargs==0);++active->as;f.result->set_string("discarded actual method result");}
bool native(void*p,SwfEvent48&e,std::string&error){auto&c=*static_cast<Calls*>(p);c.events.push_back(e.kind);if(c.consume&&e.kind==4)e.consumed=1;
 if(c.nested&&e.kind==4){c.nested=false;require(c.connection->reset_focus(e.cursor,error),error);}
 if(c.release){c.release=false;c.connection->release();require(*c.destroyed==0,"Graph destroyed within receiver");}return true;}
bool accepts(void*p,SwfEvent48&,bool&value,std::string&){auto&c=*static_cast<Calls*>(p);++c.accepts;value=!c.reject;return true;}
bool orientation(void*,std::int32_t&v,std::string&){v=0;return true;}
bool dimensions(void*,std::int32_t&w,std::int32_t&h,std::string&){w=480;h=320;return true;}
bool advance(void*p,gameswf::root*r,float dt,bool flag,std::string&e){auto&c=*static_cast<Calls*>(p);if(!c.allow_advance){e="Required source advance disabled by fixture";return false;}++c.advances;return c.frames->advance(r,dt,flag,e);}
ViewportState64 seed(){ViewportState64 s{};const float r[4]{0,9600,0,6400};std::copy(r,r+4,s.movie_rect);const std::int32_t b[4]{0,0,480,320};std::copy(b,b+4,s.bounds);std::copy(b,b+4,s.viewport);s.player_receiver=1;return s;}
}
int main(){try{
 unsigned destroyed=0,guards=0,history_cases=0,input_cases=0;std::string error;
 auto core=std::make_shared<Core>(&destroyed);auto*a=core->button("btn_A",1,0);auto*b=core->button("btnDelete",2,800);
 SwfInputHistoryFlags flags{};require(core->history->read(a,flags,error),error);require(flags.mouse9c==0&&flags.need9d==1&&flags.enter_e9==0);++history_cases;
 // Exact source capitalization differs from dispatched RollOver/RollOut names.
 a->set_member("onRollOver",gameswf::as_value(method));require(core->history->read(a,flags,error)&&flags.mouse9c==0);++history_cases;
 a->set_member("onRollover",gameswf::as_value());require(core->history->read(a,flags,error)&&flags.mouse9c==1);++history_cases;
 a->set_member("onEnterFrame",gameswf::as_value());require(core->history->read(a,flags,error)&&flags.enter_e9==1);++history_cases;
 auto*read_only=core->button("ordinary_readonly",3,2000);watched_history=core->history;gameswf::as_value old(42);old.set_flags(gameswf::as_value::READ_ONLY);read_only->m_members.set("onPress",old);gameswf::as_value watcher_pin(watcher);require(read_only->watch("onPress",watcher_pin.to_function(),gameswf::as_value()));read_only->set_member("onPress",gameswf::as_value());gameswf::as_value stored;require(read_only->get_member("onPress",&stored)&&stored.to_int()==42);require(watcher_calls==1&&core->history->read(read_only,flags,error)&&flags.mouse9c==1);++history_cases;watched_history.reset();
 for(auto*button:{a,b})for(auto*name:{"on_focus_in","on_focus_out","onPress","onRelease","on_clicked","onRollOver","onRollOut","onDragOver","onDragOut","onReleaseOutside"})button->set_member(name,gameswf::as_value(method));
 SwfInputConnection connection;auto calls=std::make_shared<Calls>();calls->connection=&connection;calls->destroyed=&destroyed;active=calls.get();std::uint32_t selection=0;
 calls->frames=&core->frames;SwfInputCoreServices services{calls,calls.get(),1,accepts,native,advance,nullptr};SwfViewportDriver driver{nullptr,orientation,dimensions};
 require(connection.bind({core,core->root.get_ptr()},seed(),driver,core->history,core->root->get_root_movie(),0,selection,services,error),error);
 for(unsigned i=0;i<4;++i){require(connection.cursor({10,10,0,0},i,error),error);SwfInputState288 out{};require(connection.snapshot(out,error));require(out.slots[i].focus==reinterpret_cast<std::uintptr_t>(a));++input_cases;
  require(connection.cursor({10,10,0,1},i,error),error);require(connection.snapshot(out,error));require(out.slots[i].pressed==reinterpret_cast<std::uintptr_t>(a));++input_cases;
  require(connection.cursor({10,10,0,0},i,error),error);require(connection.snapshot(out,error));require(!out.slots[i].pending);++input_cases;
  require(connection.reset_focus(i,error),error);require(connection.focus(a,i,error),error);require(connection.input(8,i,error),error);require(connection.snapshot(out,error));require(out.slots[i].focus==reinterpret_cast<std::uintptr_t>(b));require(connection.input(16,i,error),error);require(connection.snapshot(out,error)&&out.slots[i].pending==reinterpret_cast<std::uintptr_t>(b));++input_cases;
 }
 require(!connection.update(16,false,error)&&error.find("advance")!=std::string::npos);++guards;
 calls->allow_advance=true;require(connection.update(100,false,error),error);require(calls->advances==1);++input_cases;
 calls->reject=true;require(connection.focus(a,0,error));SwfInputState288 out{};require(connection.snapshot(out,error)&&!out.slots[0].focus);calls->reject=false;++guards;
 calls->nested=true;require(connection.cursor({10,10,0,1},0,error),error);require(!calls->nested);++guards;
 float raw[2];std::int32_t index;require(connection.raw_cursor(raw,index,error)&&raw[0]==10&&raw[1]==10&&index==0);++guards;
 require(!connection.reset_focus(4,error));++guards;
 // Source menus connect at original WidthScreen/HeightScreen, then draw on
 // the Android surface. The retained facade viewport must drive both paths.
 auto viewport=std::make_shared<SwfViewportConnection>();
 require(viewport->bind({core,core->root.get_ptr()},seed(),driver,error),error);
 SwfInputConnection shared_input;auto shared_calls=std::make_shared<Calls>();shared_calls->connection=&shared_input;shared_calls->destroyed=&destroyed;
 SwfInputCoreServices shared_services{shared_calls,shared_calls.get(),1,accepts,native,advance,nullptr};
 auto input_viewport=viewport;
#ifdef DH2_INPUT_VIEWPORT_SNAPSHOT_REPRO
 input_viewport.reset(); // Reproduce the former facade's connection-time copy.
#endif
 require(shared_input.bind({core,core->root.get_ptr()},seed(),driver,core->history,core->root->get_root_movie(),0,selection,shared_services,error,{},input_viewport),error);
 auto*right=core->button("btn_Right",4,8000);auto right_matrix=right->get_matrix();right_matrix.m_[1][2]=1800;right->set_matrix(right_matrix);
 const std::int32_t wide[4]{0,0,2400,1080};require(viewport->set_viewport(wide,error),error);
 float wide_point[2]{2050,343};require(viewport->screen_to_logical(wide_point,error),error);
 require(std::fabs(wide_point[0]-410.f)<.001f&&std::fabs(wide_point[1]-101.62963f)<.001f,"Draw viewport fixture projection");
 require(shared_input.cursor({2050,343,0,1},0,error),error);require(shared_input.snapshot(out,error),error);
 require(out.slots[0].focus==reinterpret_cast<std::uintptr_t>(right)&&out.slots[0].pressed==reinterpret_cast<std::uintptr_t>(right),"Wide source button missed after render viewport update");
 require(core->root->m_mouse_x==410&&core->root->m_mouse_y==101,"Input retained connection-time dimensions");
 require(shared_input.cursor({2050,343,0,0},0,error),error);
 require(std::find(shared_calls->events.begin(),shared_calls->events.end(),4)!=shared_calls->events.end()&&std::find(shared_calls->events.begin(),shared_calls->events.end(),6)!=shared_calls->events.end(),"Wide source tap lacked onPress/onRelease");++input_cases;
 // Camera offsets/aspect bounds must be shared too, not reconstructed from
 // the surface dimensions. This also preserves existing controller state.
 const std::int32_t shifted[4]{100,50,2400,1080};require(viewport->set_bounds(shifted,0,error),error);
 require(shared_input.cursor({2150,393,0,1},0,error),error);require(shared_input.snapshot(out,error)&&out.slots[0].pressed==reinterpret_cast<std::uintptr_t>(right),"Camera bounds diverged from hit projection");
 require(shared_input.cursor({2150,393,0,0},0,error),error);++input_cases;
 shared_input.release();input_viewport.reset();viewport.reset();right=nullptr;
 auto weak=std::weak_ptr<Core>(core);a=nullptr;b=nullptr;core.reset();calls->release=true;require(connection.reset_focus(1,error),error);require(!connection.bound()&&destroyed==1&&weak.expired());++guards;
 active=nullptr;
 std::cout<<"{\"validation\":\"PASS\",\"actual_core_input_cases\":"<<input_cases<<",\"constructor_and_assignment_history\":"<<history_cases<<",\"native_events\":"<<calls->events.size()<<",\"actual_AS_methods\":"<<calls->as<<",\"required_provider_and_reentry_guards\":"<<guards<<",\"whole_original_advance_parity\":false,\"mismatches\":0}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
