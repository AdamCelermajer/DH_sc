#include "authored_hud_portrait_v4.hpp"
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
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
namespace dh2::ui {
namespace {
gameswf::character* find_ring(gameswf::character* c,unsigned depth,unsigned& matches){
 if(!c||depth>64)return nullptr;gameswf::character* found=nullptr;
 if(c->get_id()==155){++matches;found=c;}
 if(c->is(gameswf::sprite_instance::m_class_id)){
  auto* s=static_cast<gameswf::sprite_instance*>(c);
  for(int i=0;i<s->m_display_list.size();++i){auto* value=find_ring(s->m_display_list.get_character(i),depth+1,matches);if(value)found=value;}
 }return found;
}
}
bool authored_hud_portrait_align_v5(SwfMovie& movie,const AuthoredGameplayHudV1& hud,AuthoredHudPortraitAlignmentV5& out,std::string& error){
 struct Request{const AuthoredGameplayHudV1& hud;AuthoredHudPortraitAlignmentV5& out;
  static bool run(void* raw,SwfAsGraph& g,std::string& e){auto& q=*static_cast<Request*>(raw);SwfAsValue root,value;gameswf::as_object* object=nullptr;
   if(!g.root_value(root,e)||!g.find_target(root,(q.hud.control_path(AuthoredHudControlV1::character)+".btimg.HudChar").c_str(),value,e)||!g.borrow_object(value,object,e))return false;
   if(!object||!object->is(gameswf::sprite_instance::m_class_id)){e="Modern portrait alignment requires actual HudChar sprite";return false;}
   auto* portrait=static_cast<gameswf::sprite_instance*>(object);
   if(portrait->m_display_list.size()!=1){e="Modern portrait alignment requires single actual portrait shape";return false;}
   auto* paint=portrait->m_display_list.get_character(0);if(!paint){e="Modern portrait alignment lost actual paint";return false;}
   // Exact decoded alpha>=128 connected paint bounding-box centers. Source
   // fill matrices map the atlas samples into their genuine shape coordinates.
   float u=0,v=0,tx=0,ty=0;
   switch(paint->get_id()){case 38:u=700;v=687;tx=-10293;ty=-10052;break;case 39:u=630.5f;v=688.5f;tx=-9234;ty=-10082;break;case 40:u=559;v=689;tx=-8160;ty=-10082;break;default:e="Modern portrait alignment unsupported source frame";return false;}
   constexpr float fill=15.10906982421875f;gameswf::point local(u*fill+tx,v*fill+ty),world;
   paint->get_world_matrix().transform(&world,local);q.out.shape=paint->get_id();q.out.before_world[0]=world.m_x;q.out.before_world[1]=world.m_y;
   if(!g.find_target(root,(q.hud.elements_path()+".HealthBars.player").c_str(),value,e)||!g.borrow_object(value,object,e))return false;
   if(!object||!object->is(gameswf::character::m_class_id)){e="Modern portrait alignment requires same HealthBars player";return false;}
   unsigned matches=0;auto* ring=find_ring(static_cast<gameswf::character*>(object),0,matches);
   if(!ring||matches!=1){e="Modern portrait alignment requires unique genuine ring155";return false;}
   // The largest closed dark aperture in the real frame155 atlas ROI has
   // bounds[698,734]x[480,517]. Its bitmap matrix is20/-15450/-9986.
   gameswf::point target;ring->get_world_matrix().transform(&target,gameswf::point(716.f*20.f-15450.f,498.5f*20.f-9986.f));
   q.out.target_world[0]=target.m_x;q.out.target_world[1]=target.m_y;
   auto* parent=portrait->get_parent();if(!parent){e="Modern portrait alignment requires same parent transform";return false;}
   gameswf::point target_parent,paint_parent;
   parent->get_world_matrix().transform_by_inverse(&target_parent,target);
   const auto shape_local=local;paint->get_matrix().transform(&local,shape_local); // actual shape-local placement
   auto matrix=portrait->get_matrix();matrix.transform(&paint_parent,local);
   const float dx=target_parent.m_x-paint_parent.m_x,dy=target_parent.m_y-paint_parent.m_y;
   q.out.delta_local[0]=dx;q.out.delta_local[1]=dy;
   matrix.m_[0][2]+=dx;matrix.m_[1][2]+=dy;portrait->set_matrix(matrix);
   paint->get_world_matrix().transform(&world,gameswf::point(u*fill+tx,v*fill+ty));q.out.after_world[0]=world.m_x;q.out.after_world[1]=world.m_y;
   return true;
  }} request{hud,out};
 return movie.menu_action_script(&request,Request::run,error);
}
bool authored_hud_portrait_v4(SwfMovie& movie,const AuthoredGameplayHudV1& hud,AuthoredHudPortraitDiagnosticV4& out,std::string& error){
 const auto path=hud.control_path(AuthoredHudControlV1::character);
 if(path.empty()){error="Required actual bound HUD character control";return false;}
 AuthoredHudPortraitDiagnosticV4 actual;
 if(!movie.clip(path.c_str(),actual.button,error)||!movie.clip((path+".btimg").c_str(),actual.bitmap_container,error)||!movie.clip((path+".btimg.HudChar").c_str(),actual.portrait,error)||!movie.source_display_rectangle(actual.display_rectangle,actual.viewport,error))return false;
 struct Query {std::string path;AuthoredHudPortraitDiagnosticV4& out;
  static bool run(void* raw,SwfAsGraph& graph,std::string& error){auto& q=*static_cast<Query*>(raw);SwfAsValue root,value;gameswf::as_object* object=nullptr;
   if(!graph.root_value(root,error)||!graph.find_target(root,q.path.c_str(),value,error)||!graph.borrow_object(value,object,error))return false;
   if(!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required same actual HudChar sprite";return false;}
   auto* sprite=static_cast<gameswf::sprite_instance*>(object);
   if(sprite->m_display_list.size()!=1){error="Actual source HudChar frame has unexpected display-list extent";return false;}
   auto* shape=sprite->m_display_list.get_character(0);if(!shape){error="Required actual HudChar shape receiver";return false;}
   gameswf::rect bounds;shape->get_bound(&bounds);auto* parent=shape->get_parent();if(parent)parent->get_world_matrix().transform(&bounds);
   q.out.shape_world_bounds[0]=bounds.m_x_min;q.out.shape_world_bounds[1]=bounds.m_x_max;q.out.shape_world_bounds[2]=bounds.m_y_min;q.out.shape_world_bounds[3]=bounds.m_y_max;q.out.actual_shape_id=shape->get_id();return true;
  }} query{path+".btimg.HudChar",actual};
 if(!movie.menu_action_script(&query,Query::run,error))return false;
 out=actual;return true;
}
}
