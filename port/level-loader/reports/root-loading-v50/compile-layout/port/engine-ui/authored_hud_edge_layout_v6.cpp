#include "authored_hud_edge_layout_v6.hpp"
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#pragma clang diagnostic ignored "-Wnew-returns-null"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_movie_def.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#include <cmath>
#include <vector>
namespace dh2::ui { namespace {
struct EdgeLayout {
 struct Saved {gameswf::gc_ptr<gameswf::character> character;gameswf::matrix matrix;};
 const AuthoredGameplayHudV1& hud;float rectangle[4]{};
 std::vector<Saved> saved;
 bool move(SwfAsGraph& graph,const std::string& path,float dx,float dy,std::string& error){
  SwfAsValue root,value;gameswf::as_object* object{};
  if(!graph.root_value(root,error)||!graph.find_target(root,path.c_str(),value,error)||!graph.borrow_object(value,object,error))return false;
  if(!object||!object->is(gameswf::character::m_class_id)){error="Required HUD edge anchor "+path;return false;}
  return move_node(static_cast<gameswf::character*>(object),path,dx,dy,error);
 }
 bool move_node(gameswf::character* node,const std::string& path,float dx,float dy,std::string& error){
  for(const auto& previous:saved)if(previous.character.get_ptr()==node){error="Duplicate actual HUD edge anchor "+path;return false;}
  auto matrix=node->get_matrix();gameswf::point origin(0,0),offset(dx,dy);
  if(auto* parent=node->get_parent()){
   const auto world=parent->get_world_matrix();
   const float det=world.m_[0][0]*world.m_[1][1]-world.m_[0][1]*world.m_[1][0];
   if(!std::isfinite(det)||std::fabs(det)<1.e-8f){error="Singular authored HUD edge parent "+path;return false;}
   gameswf::point a,b;world.transform_by_inverse(&a,origin);world.transform_by_inverse(&b,offset);
   offset.m_x=b.m_x-a.m_x;offset.m_y=b.m_y-a.m_y;
  }
  saved.push_back({node,matrix});matrix.m_[0][2]+=offset.m_x;matrix.m_[1][2]+=offset.m_y;
  node->set_matrix(matrix);return true;
 }
 static bool apply(void* raw,SwfAsGraph& graph,std::string& error){
  auto& self=*static_cast<EdgeLayout*>(raw);SwfAsValue root;gameswf::as_object* object{};
  if(!graph.root_value(root,error)||!graph.borrow_object(root,object,error)||!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required original HUD stage owner";return false;}
  const auto* definition=static_cast<gameswf::sprite_instance*>(object)->get_root()->m_def.get_ptr();
  const auto& stage=definition->m_frame_size;
  const float left=self.rectangle[0]-stage.m_x_min,right=self.rectangle[1]-stage.m_x_max;
  const float top=self.rectangle[2]-stage.m_y_min,bottom=self.rectangle[3]-stage.m_y_max;
  auto move=[&](const std::string& path,float x,float y){return self.move(graph,path,x,y,error);};
  const auto& hud=self.hud;const auto& elements=hud.elements_path();
  if(!move(elements+".HealthBars.player",left,top)||
     !move(hud.control_path(AuthoredHudControlV1::character),left,top)||
     !move(hud.control_path(AuthoredHudControlV1::pause),left,top)||
     !move(hud.control_path(AuthoredHudControlV1::potion),right,top))return false;
  const bool mirrored=hud.style()==1||hud.style()==3;
  const float action=mirrored?left:right,stick=mirrored?right:left;
  if(!move(hud.control_path(AuthoredHudControlV1::joystick),stick,bottom)||
     !move(hud.control_path(AuthoredHudControlV1::faery),action,bottom)||
     !move(hud.control_path(AuthoredHudControlV1::attack),action,bottom))return false;
  if(hud.style()<2){if(!move(elements+".controls.controls.list",action,bottom))return false;}
  else for(auto control:{AuthoredHudControlV1::skill1,AuthoredHudControlV1::skill2,AuthoredHudControlV1::skill3})
   if(!move(hud.control_path(control),action,bottom))return false;
  if(!move(elements+".itemname_text",left,0)||!move(elements+".itunes_controls",(left+right)*.5f,top))return false;
  if(hud.style()==3&&(!move(elements+".controls.controls.btn_dpad",stick,bottom)||!move(elements+".controls.controls.btn_dpad_limits",stick,bottom)))return false;
  SwfAsValue group;gameswf::as_object* group_object{};
  if(!graph.find_target(root,elements.c_str(),group,error)||!graph.borrow_object(group,group_object,error)||!group_object||!group_object->is(gameswf::sprite_instance::m_class_id)){error="Required same HUD deadzone display list";return false;}
  auto* sprite=static_cast<gameswf::sprite_instance*>(group_object);
  for(int i=0;i<sprite->m_display_list.size();++i){
   auto* node=sprite->m_display_list.get_character(i);if(!node)continue;const std::string name=node->get_name().c_str();
   if(name=="deadzone_menus"){if(!self.move_node(node,name,left,top,error))return false;}
   else if(name=="deadzone_mainmenus"){if(!self.move_node(node,name,right,top,error))return false;}
   else if(name=="deadzone_skills1"||name=="deadzone_faery"||name=="deadzone_potion"){
    if(!self.move_node(node,name,action,bottom,error))return false;
   }
  }
  return true;
 }
 static bool restore(void* raw,SwfAsGraph&,std::string&){
  auto& self=*static_cast<EdgeLayout*>(raw);
  for(auto i=self.saved.rbegin();i!=self.saved.rend();++i)i->character->set_matrix(i->matrix);
  self.saved.clear();return true;
 }
};
}
bool with_authored_hud_edge_layout_v6(SwfMovie& movie,const AuthoredGameplayHudV1& hud,
 const std::function<bool(std::string&)>& operation,std::string& error){
 if(hud.style()<0||!operation){error="Required bound HUD edge layout operation";return false;}
 EdgeLayout layout{hud,{}, {}};std::int32_t viewport[4]{};
 if(!movie.source_display_rectangle(layout.rectangle,viewport,error))return false;
 if(!movie.action_script(&layout,EdgeLayout::apply,error)){
  std::string restore_error;if(!layout.saved.empty())movie.action_script(&layout,EdgeLayout::restore,restore_error);return false;
 }
 bool result=false;
 try{result=operation(error);}catch(...){std::string restore_error;movie.action_script(&layout,EdgeLayout::restore,restore_error);throw;}
 std::string restore_error;
 if(!movie.action_script(&layout,EdgeLayout::restore,restore_error)){error="HUD edge layout restore failed: "+restore_error;return false;}
 return result;
}
}
