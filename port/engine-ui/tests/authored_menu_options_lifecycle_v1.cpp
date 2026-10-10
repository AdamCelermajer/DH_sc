#include "authored_shared_menu_roster_v27.hpp"
#include "gameswf/gameswf_sprite.h"
#include <cassert>
#include <iostream>
using namespace dh2::ui;
namespace {
std::vector<std::uint8_t> movie_bytes(){
 std::vector<std::uint8_t> bytes{'F','W','S',6,0,0,0,0};unsigned bit=0;
 auto bits=[&](unsigned value,unsigned count){for(unsigned i=count;i-->0;){
  if(!(bit%8))bytes.push_back(0);bytes.back()|=((value>>i)&1u)<<(7-bit%8);++bit;}};
 bits(12,5);bits(0,12);bits(2000,12);bits(0,12);bits(2000,12);
 for(auto byte:{0,12,2,0,0x40,0,0x40,0,0,0})bytes.push_back(byte);
 for(unsigned i=0;i<4;++i)bytes[4+i]=std::uint8_t(bytes.size()>>(8*i));return bytes;
}
struct Fixture {
 std::shared_ptr<SwfMovie> movie=std::make_shared<SwfMovie>();
 std::shared_ptr<MenuStackOwnerV1> stack=std::make_shared<MenuStackOwnerV1>(16);
 AuthoredMenuApplicationFieldsV3 fields;MenuStackGlobalsV1 globals{};
 std::unique_ptr<AuthoredSharedMenuRosterV27> roster;
 gameswf::sprite_instance *options{},*custom{},*unrelated{};
 bool current_level{},reject{};unsigned queries{},debug_calls{};
 static bool create(void* raw,SwfAsGraph& graph,std::string& e){auto& f=*static_cast<Fixture*>(raw);
  SwfAsValue root;gameswf::as_object* object{};
  if(!graph.root_value(root,e)||!graph.borrow_object(root,object,e))return false;
  auto* sprite=static_cast<gameswf::sprite_instance*>(object);
  auto* other=static_cast<gameswf::sprite_instance*>(sprite->add_empty_movieclip("menu_Other",1));assert(other);
  f.unrelated=static_cast<gameswf::sprite_instance*>(other->add_empty_movieclip("option_Custom",1));assert(f.unrelated);f.unrelated->set_visible(true);
  f.options=static_cast<gameswf::sprite_instance*>(sprite->add_empty_movieclip("menu_Options",2));assert(f.options);
  f.custom=static_cast<gameswf::sprite_instance*>(f.options->add_empty_movieclip("option_Custom",1));assert(f.custom);f.custom->set_visible(true);return true;
 }
 Fixture(){
  std::string e;SwfServices io;
  io.read=[](void*,const char*,auto& out,auto&){out=movie_bytes();return true;};
  io.draw=[](void*,const auto&,auto&){return true;};
  assert(movie->load({},"options.swf",io,e)&&movie->menu_action_script(this,create,e));
  assert(stack->publish_source_c1_v104(globals,e));
  constexpr std::uintptr_t render_id=0x12345678;
  assert(stack->register_render_live_v27(render_id,0,nullptr,nullptr,e));
  AuthoredSharedMenuServicesV27 s;s.owner=movie;s.fields=&fields;
  s.debug=[this](const char*,auto& out,auto&){++debug_calls;out=0;return true;};
  s.level_running=[this](bool& out,auto& e){++queries;
   assert(options->get_visible()); // SetVisible/onPush precede the query.
   if(reject){e="Rejected actual current Level query";return false;}
   out=current_level;return true;
  };
  roster=std::make_unique<AuthoredSharedMenuRosterV27>(stack,std::move(s));
  const auto before_invalid_render_debug=debug_calls;
  assert(!roster->post_load(*movie,reinterpret_cast<std::uintptr_t>(movie.get()),movie,0,e));
  assert(e=="Required same movie RenderFX registration before PostLoad");e.clear();
  assert(debug_calls==before_invalid_render_debug); // Reject before touching/discovering movie characters.
  assert(roster->post_load(*movie,render_id,movie,0,e));
  auto* menu=stack->menu("menu_Options");assert(menu);
  // The discovered menu must be appended to this exact pre-registered owner.
  // The SwfMovie facade address is deliberately different from render_id.
  assert(reinterpret_cast<std::uintptr_t>(movie.get())!=render_id);
  assert(menu->render==stack->render(render_id)&&menu->render->identity==render_id);
  // Localization is independently proved; this test isolates its completed
  // source gate and reaches the actual shared-roster Options continuation.
  roster->receiver_fields_v59(menu->identity)->localized75=1;
 }
 bool lifecycle(MenuStackOperationV1 op,std::string& e){
  auto* menu=stack->menu("menu_Options");assert(menu);
  MenuStackRequestV1 q{};q.operation=op;q.menu=menu;q.render=menu->render;bool handled{};
  const bool delivered=roster->route(*stack->view(),q,handled,e);assert(handled);return delivered;
 }
};
}
int main(){
 std::string e;
 {Fixture f;
  assert(f.lifecycle(MenuStackOperationV1::menu_show,e)&&f.queries==1&&!f.custom->get_visible()&&f.unrelated->get_visible());
  assert(f.lifecycle(MenuStackOperationV1::menu_hide,e)&&!f.options->get_visible());
  f.current_level=true;
  assert(f.lifecycle(MenuStackOperationV1::menu_show,e)&&f.queries==2&&f.custom->get_visible());
  assert(f.lifecycle(MenuStackOperationV1::menu_hide,e));
  f.current_level=false;
  assert(f.lifecycle(MenuStackOperationV1::menu_show,e)&&f.queries==3&&!f.custom->get_visible());
 }
 {Fixture f;f.reject=true;const auto before=f.debug_calls;
  assert(!f.lifecycle(MenuStackOperationV1::menu_show,e)&&e=="Rejected actual current Level query");
  assert(f.queries==1&&f.custom->get_visible()&&f.options->get_visible());
  assert(f.debug_calls==before+2); // failed query prevents RegisterDeadZones.
 }
 {Fixture f;f.options->remove_display_object(f.custom);f.custom=nullptr;
  assert(!f.lifecycle(MenuStackOperationV1::menu_show,e)&&f.queries==0&&e=="Required actual Options character: option_Custom");
  assert(f.unrelated->get_visible()); // another menu's same name is not used.
 }
 std::cout<<"Options shared-roster Show/Hide, live Level toggles, scoped child and failure prefixes passed\n";
}
