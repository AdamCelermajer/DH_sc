#include "swf_input_session_v1.hpp"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_function.h"
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::ui;
namespace {
unsigned checks{},destroyed{};
void require(bool b,const std::string&e="Input session assertion"){++checks;if(!b)throw std::runtime_error(e+" ["+std::to_string(checks)+"]");}
struct Bits {std::vector<std::uint8_t>b;unsigned bit{};void put(unsigned v,unsigned n){for(unsigned j=n;j;--j){if(!(bit%8))b.push_back(0);b.back()|=((v>>(j-1))&1)<<(7-bit%8);++bit;}}};
void tag(std::vector<std::uint8_t>&b,unsigned type,const std::vector<std::uint8_t>&p){unsigned head=(type<<6)|(p.size()<63?p.size():63);b.push_back(head);b.push_back(head>>8);if(p.size()>=63)for(unsigned j=0;j<4;++j)b.push_back(p.size()>>(j*8));b.insert(b.end(),p.begin(),p.end());}
std::vector<std::uint8_t>movie(bool startup){std::vector<std::uint8_t>b{'F','W','S',8,0,0,0,0};Bits rect;rect.put(15,5);for(unsigned v:{0u,9600u,0u,6400u})rect.put(v,15);b.insert(b.end(),rect.b.begin(),rect.b.end());b.insert(b.end(),{0,30,1,0});
 if(startup){std::vector<std::uint8_t>p{7,0,0,0,0,0};for(char c:std::string("FixtureStartup"))p.push_back(c);p.push_back(0);std::vector<std::uint8_t>a{0x96,static_cast<std::uint8_t>(p.size()),0};a.insert(a.end(),p.begin(),p.end());a.insert(a.end(),{0x3d,0x17,0});tag(b,12,a);}tag(b,1,{});tag(b,0,{});unsigned size=b.size();for(unsigned j=0;j<4;++j)b[4+j]=size>>(j*8);return b;}
struct Shape:gameswf::character_def {explicit Shape(gameswf::player*p):character_def(p){}bool point_test_local(float x,float y)override{return x>=0&&x<=400&&y>=0&&y<=400;}};
struct Test {
 SwfInputSessionV1*session{};std::unique_ptr<SwfInputSessionV1>*destroy_session{};int width=480,height=320,orientation_value=0;
 unsigned starts{},reads{},startup_calls{},native_events{},methods{},draws{},begins{};
 bool reject_start{},reject_event{},release_event{},nested_event{},release_start{},reject_draw{};
 gameswf::root*root{};gameswf::sprite_instance*button{};SwfAsValue*escaped{};
 SwfDraw last_begin;std::vector<unsigned>order;
 ~Test(){++destroyed;}
 static bool read(void*p,const char*n,std::vector<std::uint8_t>&out,std::string&e){auto&t=*static_cast<Test*>(p);++t.reads;t.order.push_back(2);if(std::string(n)=="missing"){e="Actual resource missing";return false;}out=movie(std::string(n)=="shared");return true;}
 static bool draw(void*p,const SwfDraw&d,std::string&e){auto&t=*static_cast<Test*>(p);++t.draws;if(d.kind==SwfDraw::begin){++t.begins;t.last_begin=d;}if(t.reject_draw){e="Required GPU sink unavailable";return false;}return true;}
 static bool start(void*p,const SwfAsLease&lease,std::string&e){auto&t=*static_cast<Test*>(p);require(lease.owner&&lease.player&&!lease.root);++t.starts;t.order.push_back(1);if(t.release_start)t.session->release();if(t.reject_start){e="Required startup dependency unavailable";return false;}return true;}
 static bool native_as(void*p,const char*n,const gameswf::fn_call&f,std::string&){auto&t=*static_cast<Test*>(p);require(std::string(n)=="FixtureStartup"&&f.nargs==0);++t.startup_calls;t.order.push_back(3);return true;}
 static bool orientation(void*p,std::int32_t&o,std::string&){o=static_cast<Test*>(p)->orientation_value;return true;}
 static bool dimensions(void*p,std::int32_t&w,std::int32_t&h,std::string&){auto&t=*static_cast<Test*>(p);w=t.width;h=t.height;return true;}
 static bool accepts(void*,SwfEvent48&,bool&a,std::string&){a=true;return true;} // Explicit native receiver fixture.
 static bool event(void*p,SwfEvent48&ev,std::string&e){auto&t=*static_cast<Test*>(p);++t.native_events;
  if(t.reject_event){e="Required game event owner unavailable";return false;}
  if(t.nested_event&&ev.kind==4){t.nested_event=false;require(t.session->reset_focus(ev.cursor,e),e);}
  if(t.destroy_session){t.destroy_session->reset();t.destroy_session=nullptr;t.session=nullptr;require(t.root&&t.root->get_root_movie(),"Graph closed during session destructor callback");}
  if(t.release_event){t.release_event=false;t.session->release();require(t.root&&t.root->get_root_movie(),"Graph closed during native callback");}
  return true;
 }
 SwfInputSessionConfigV1 config(const std::shared_ptr<Test>&owner){SwfInputSessionConfigV1 c;c.shared={"shared"};c.movie="main";c.provider_owner=owner;c.movie_services.context=this;c.movie_services.read=read;c.movie_services.draw=draw;c.movie_services.native_owner=owner;c.movie_services.native_actions={"FixtureStartup"};c.movie_services.native_action=native_as;c.movie_services.graph_start=start;
  c.driver={this,orientation,dimensions};float r[4]{0,9600,0,6400};std::memcpy(c.viewport.movie_rect,r,16);std::int32_t v[4]{0,0,480,320};std::memcpy(c.viewport.viewport,v,16);std::memcpy(c.viewport.bounds,v,16);c.viewport.pixel_scale=1;
  c.input_services={owner,this,1,accepts,event,nullptr,nullptr};return c;
 }
};
Test*active;
void method(const gameswf::fn_call&fn){require(fn.nargs==0);++active->methods;fn.result->set_string("source-discarded");}
bool setup(void*p,SwfAsGraph&as,std::string&e){auto&t=*static_cast<Test*>(p);SwfAsValue root;if(!as.root_value(root,e))return false;gameswf::as_object*object=nullptr;if(!as.borrow_object(root,object,e))return false;*t.escaped=root;auto*clip=static_cast<gameswf::sprite_instance*>(object);t.root=clip->get_root();
 auto*def=clip->m_def.get_ptr();gameswf::gc_ptr<gameswf::sprite_instance>button=new gameswf::sprite_instance(clip->get_player(),def,t.root,clip,7);button->set_name("btn_fixture");for(const char*n:{"onPress","onRelease","on_clicked","on_focus_in","on_focus_out","onRollOver","onRollOut","onDragOver","onDragOut","onReleaseOutside"})button->set_member(n,gameswf::as_value(method));
 gameswf::matrix m;gameswf::cxform cx;clip->m_display_list.add_display_object(button.get_ptr(),1,false,cx,m,0,0,0);
 gameswf::gc_ptr<Shape>shape=new Shape(clip->get_player());gameswf::gc_ptr<gameswf::character>hit=shape->create_character_instance(button.get_ptr(),8);hit->set_name("hitzone");button->m_display_list.add_display_object(hit.get_ptr(),1,false,cx,m,0,0,0);t.button=button.get_ptr();return true;
}
bool drag(void*p,SwfAsGraph&,std::string&){auto&t=*static_cast<Test*>(p);gameswf::character::drag_state d;d.SetCharacter(t.button);t.root->set_drag_state(d);return true;}
bool check_stale(void*p,SwfAsGraph&as,std::string&e){auto&t=*static_cast<Test*>(p);gameswf::as_object*o=nullptr;require(!as.borrow_object(*t.escaped,o,e)&&e=="AS object belongs to a different retained movie");e.clear();return true;}
bool viewport_global(void*p,SwfAsGraph&as,std::string&e){auto&t=*static_cast<Test*>(p);SwfAsValue global,v,x;bool found=false;if(!as.global_value(global,e)||!as.get_member(global,"Viewport",v,found,e)||!found)return false;double xmax=0;if(!as.get_member(v,"xMax",x,found,e)||!found||!as.to_number(x,xmax,e))return false;require(xmax==480);require(t.root->m_viewport_width==t.width&&t.root->m_viewport_height==t.height);return true;}
}
int main(){try{
 SwfInputSessionV1 session;auto t=std::make_shared<Test>();t->session=&session;active=t.get();SwfAsValue escaped;t->escaped=&escaped;auto config=t->config(t);std::string e;
 require(session.load(config,e),e);require(t->starts==1&&t->startup_calls==1&&t->order==std::vector<unsigned>({1,2,3,2}),"Observers/native functions must precede shared construction/actions");require(session.action_script(t.get(),setup,e),e);
 require(session.update(0,false,e),e);for(unsigned i=0;i<4;++i){require(session.cursor({10,10,0,0},i,e),e);require(session.cursor({10,10,0,1},i,e),e);require(session.cursor({10,10,0,0},i,e),e);}
 require(t->methods>0&&t->native_events>0,"Actual AS/native event pipeline absent");
 require(session.input(16,3,e),e);require(session.update(34,false,e),e);
 SwfInputState288 before{},after{};unsigned selection=0;require(session.snapshot(before,selection,e),e);
 require(session.action_script(t.get(),drag,e),e);require(session.update(34,false,e),e);const auto offset_x=t->root->m_drag_state.OffsetX(),offset_y=t->root->m_drag_state.OffsetY();
 t->width=960;t->height=640;FlashCamera40 camera{};require(session.camera_update(camera,e),e);require(session.snapshot(after,selection,e),e);require(!std::memcmp(before.slots,after.slots,sizeof(before.slots)),"Resize reset focus/cursors/strong slots");
 require(t->root->m_drag_state.GetCharacter()==t->button&&t->root->m_drag_state.OffsetX()==offset_x&&t->root->m_drag_state.OffsetY()==offset_y,"Resize reset active drag");
 float point[2]{20,20};require(session.screen_to_logical(point,e)&&point[0]==10&&point[1]==10,e);require(session.action_script(t.get(),viewport_global,e),e);require(session.cursor({20,20,0,0},0,e),e);require(t->root->m_mouse_x==10&&t->root->m_mouse_y==10);require(session.display(nullptr,e),e);require(t->last_begin.viewport[2]==960&&t->last_begin.bounds[1]==9600);
 require(session.cursor({24,30,0,0},0,e),e);require(session.update(34,true,e),e);require(t->button->get_matrix().m_[0][2]==40&&t->button->get_matrix().m_[1][2]==100,"Source delayed drag after resize");
 t->orientation_value=1;t->width=640;t->height=960;require(session.camera_update(camera,e),e);point[0]=20;point[1]=20;require(session.screen_to_logical(point,e)&&point[0]==10&&point[1]==10,e);require(t->root->m_drag_state.GetCharacter()==t->button);require(session.display(nullptr,e),e);require(t->last_begin.viewport[2]==640&&t->last_begin.viewport[3]==960);t->root->stop_drag();
 const auto original_root=after.root;auto bad=config;bad.movie="missing";require(!session.load(bad,e));require(session.snapshot(after,selection,e)&&after.root==original_root,"Failed reload replaced graph");
 t->reject_start=true;const auto reads=t->reads;require(!session.load(config,e)&&t->reads==reads);t->reject_start=false;
 bad=config;bad.context_path="_root.missing";require(!session.load(bad,e));require(session.snapshot(after,selection,e)&&after.root==original_root);
 t->nested_event=true;require(session.cursor({30,30,0,1},0,e),e);require(!t->nested_event,"Synchronous source ResetFocus did not reenter same graph");
 t->reject_event=true;require(!session.focus("_root.btn_fixture",0,e)&&e=="Required game event owner unavailable");t->reject_event=false;
 require(!session.cursor({0,0,0,0},4,e));require(!session.action_script(nullptr,nullptr,e));
 t->reject_draw=true;require(!session.display(nullptr,e)&&e=="Required GPU sink unavailable");t->reject_draw=false;
 require(session.load(config,e),e);require(session.action_script(t.get(),check_stale,e),e);escaped={};require(session.action_script(t.get(),setup,e),e);require(session.update(0,false,e),e);
 require(session.snapshot(after,selection,e)&&after.root!=original_root);for(auto&s:after.slots)require(!s.focus&&!s.hover&&!s.pending&&!s.pressed&&s.enabled==1,"Reload reused prior cursor ownership");
 t->release_start=true;require(!session.load(config,e)&&e=="Input session candidate cancelled by owner release");t->release_start=false;require(!session.bound());require(session.load(config,e),e);require(session.action_script(t.get(),setup,e),e);require(session.update(0,false,e),e);
 t->release_event=true;require(session.focus("_root.btn_fixture",0,e),e);require(!session.bound());escaped={};t->root=nullptr;t->button=nullptr;
 auto weak=std::weak_ptr<Test>(t);const auto count=destroyed;config={};bad={};t.reset();active=nullptr;require(weak.expired()&&destroyed==count+1,"Provider/graph ownership cycle");
 require(!session.update(16,false,e));
 auto deleting=std::make_unique<SwfInputSessionV1>();auto other=std::make_shared<Test>();SwfAsValue extra;other->escaped=&extra;other->session=deleting.get();active=other.get();auto other_config=other->config(other);other_config.camera.desired[0]=17;
 require(deleting->load(other_config,e),e);FlashCamera40 owned_camera{};require(deleting->camera_state(owned_camera,e)&&owned_camera.current[0]==2,"Initial source camera write was discarded");require(deleting->camera_update(owned_camera,e)&&owned_camera.current[0]==4);require(deleting->action_script(other.get(),setup,e),e);require(deleting->update(34,false,e),e);other->destroy_session=&deleting;auto*called=deleting.get();require(called->focus("_root.btn_fixture",0,e),e);require(!deleting,"Callback did not destroy the outer session");extra={};auto other_weak=std::weak_ptr<Test>(other);other_config={};other.reset();active=nullptr;require(other_weak.expired(),"Destroyed-session graph/provider cycle");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"four_cursor_pipeline\":true,\"active_focus_drag_resize\":true,\"orientation_source_mapping\":true,\"retained_movie_reload_release\":true,\"startup_before_shared_load\":true,\"actual_native_AS_callbacks\":true,\"mismatches\":0}\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
