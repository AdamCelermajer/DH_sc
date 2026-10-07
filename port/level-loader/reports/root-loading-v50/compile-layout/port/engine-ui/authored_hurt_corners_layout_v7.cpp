#include "authored_hurt_corners_layout_v7.hpp"
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
namespace dh2::ui { namespace {
struct Layout {
 HurtCornersLayoutV7 diagnostic{};
 gameswf::gc_ptr<gameswf::character> node;
 gameswf::matrix saved;
 bool changed{};
 static bool apply(void* raw,SwfAsGraph& graph,std::string& error){
  auto& self=*static_cast<Layout*>(raw);SwfAsValue root,value;gameswf::as_object* object{};
  if(!graph.root_value(root,error)||!graph.find_target(root,"_root.HurtCorners",value,error)||!graph.borrow_object(value,object,error))return false;
  if(!object||!object->is(gameswf::sprite_instance::m_class_id)){error="Required actual HurtCorners sprite";return false;}
  auto* sprite=static_cast<gameswf::sprite_instance*>(object);
  if(sprite->get_id()!=31||sprite->get_frame_count()!=103){error="Required authored HP-index HurtCorners identity";return false;}
  auto* source_root=sprite->get_root();if(!source_root||!source_root->m_def){error="Required original HurtCorners movie stage";return false;}
  const auto& stage=source_root->m_def->m_frame_size;auto& d=self.diagnostic;
  d.stage[0]=stage.m_x_min;d.stage[1]=stage.m_x_max;d.stage[2]=stage.m_y_min;d.stage[3]=stage.m_y_max;
  const float sw=d.stage[1]-d.stage[0],sh=d.stage[3]-d.stage[2],dw=d.display[1]-d.display[0],dh=d.display[3]-d.display[2];
  if(!(sw>0&&sh>0&&dw>0&&dh>0)||!std::isfinite(sw+sh+dw+dh)){error="Invalid actual HurtCorners stage/display extent";return false;}
  gameswf::matrix mapped=sprite->get_world_matrix();
  const float sx=dw/sw,sy=dh/sh;
  for(unsigned col=0;col<3;++col){mapped.m_[0][col]*=sx;mapped.m_[1][col]*=sy;}
  mapped.m_[0][2]+=d.display[0]-sx*d.stage[0];mapped.m_[1][2]+=d.display[2]-sy*d.stage[2];
  if(auto* parent=sprite->get_parent()){
   const auto matrix=parent->get_world_matrix();const float det=matrix.m_[0][0]*matrix.m_[1][1]-matrix.m_[0][1]*matrix.m_[1][0];
   if(!std::isfinite(det)||std::fabs(det)<1.e-8f){error="Singular actual HurtCorners parent transform";return false;}
   gameswf::matrix inverse;inverse.set_inverse(matrix);inverse.concatenate(mapped);mapped=inverse;
  }
  self.node=sprite;self.saved=sprite->get_matrix();self.changed=true;sprite->set_matrix(mapped);
  gameswf::rect bounds;sprite->get_bound(&bounds);if(auto* parent=sprite->get_parent())parent->get_world_matrix().transform(&bounds);
  d.paint_bounds[0]=bounds.m_x_min;d.paint_bounds[1]=bounds.m_x_max;d.paint_bounds[2]=bounds.m_y_min;d.paint_bounds[3]=bounds.m_y_max;
  return true;
 }
 static bool restore(void* raw,SwfAsGraph&,std::string&){auto& self=*static_cast<Layout*>(raw);if(self.changed){self.node->set_matrix(self.saved);self.changed=false;}self.node=nullptr;return true;}
};
}
bool display_authored_hurt_corners_v7(SwfMovie& movie,HurtCornersLayoutV7* diagnostic,std::string& error){
 Layout layout;if(!movie.source_display_rectangle(layout.diagnostic.display,layout.diagnostic.viewport,error))return false;
 if(!movie.action_script(&layout,Layout::apply,error))return false;
 bool result{};
 try{result=movie.display_source_clip("_root.HurtCorners",error);}catch(...){std::string cleanup;movie.action_script(&layout,Layout::restore,cleanup);throw;}
 std::string cleanup;if(!movie.action_script(&layout,Layout::restore,cleanup)){error="HurtCorners source-matrix restore failed: "+cleanup;return false;}
 if(diagnostic)*diagnostic=layout.diagnostic;return result;
}
}
