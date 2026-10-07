#include "enemy_hud_presentation_v2.hpp"
#include "gameswf/gameswf_character.h"
#include <cmath>
#include <algorithm>
namespace dh2::ui {
bool enemy_hud_anchor_v2(gameswf::character* clip,const float root_twips[2],std::string& error,const float visible[4]){
 if(!clip||!root_twips||!std::isfinite(root_twips[0])||!std::isfinite(root_twips[1])){error="Required live enemy HUD anchor";return false;}
 auto* parent=clip->get_parent();if(!parent){error="Required authored enemy HUD parent";return false;}
 const auto parent_matrix=parent->get_world_matrix();const float determinant=parent_matrix.m_[0][0]*parent_matrix.m_[1][1]-parent_matrix.m_[0][1]*parent_matrix.m_[1][0];
 if(!std::isfinite(determinant)||determinant==0){error="Authored enemy HUD parent has no inverse";return false;}
 gameswf::point point(root_twips[0],root_twips[1]),local;parent_matrix.transform_by_inverse(&local,point);
 gameswf::rect bound;clip->get_bound(&bound);
 const float centre=(bound.m_x_min+bound.m_x_max)*.5f,bottom=bound.m_y_max;
 // Native get_bound already includes this character's local matrix; its
 // rectangle is in parent coordinates. Translate by the desired difference.
 auto matrix=clip->get_matrix();const float ox=centre,oy=bottom;
 if(!std::isfinite(local.m_x)||!std::isfinite(local.m_y)||!std::isfinite(ox)||!std::isfinite(oy)){error="Authored enemy HUD bounds projection invalid";return false;}
 matrix.m_[0][2]+=local.m_x-ox;matrix.m_[1][2]+=local.m_y-oy;clip->set_matrix(matrix);
 if(visible){
  for(unsigned i=0;i<4;++i)if(!std::isfinite(visible[i])){error="Source enemy HUD visible rectangle invalid";return false;}
  if(visible[0]>=visible[1]||visible[2]>=visible[3]){error="Source enemy HUD viewport is empty";return false;}
  clip->get_bound(&bound);gameswf::rect world=bound;parent_matrix.transform(&world);
  float dx=0,dy=0;
  if(world.m_x_max-world.m_x_min>visible[1]-visible[0])dx=visible[0]-world.m_x_min;
  else if(world.m_x_min<visible[0])dx=visible[0]-world.m_x_min;
  else if(world.m_x_max>visible[1])dx=visible[1]-world.m_x_max;
  if(world.m_y_max-world.m_y_min>visible[3]-visible[2])dy=visible[2]-world.m_y_min;
  else if(world.m_y_min<visible[2])dy=visible[2]-world.m_y_min;
  else if(world.m_y_max>visible[3])dy=visible[3]-world.m_y_max;
  if(dx||dy){gameswf::point root_delta(dx,dy),parent_delta,origin;parent_matrix.transform_by_inverse(&parent_delta,root_delta);parent_matrix.transform_by_inverse(&origin,gameswf::point(0,0));matrix=clip->get_matrix();matrix.m_[0][2]+=parent_delta.m_x-origin.m_x;matrix.m_[1][2]+=parent_delta.m_y-origin.m_y;clip->set_matrix(matrix);}
 }
 return true;
}
int enemy_hud_hp_frame_v2(std::int32_t hp,std::int32_t maximum){if(hp<=0||maximum<=0)return 0;const float fraction=float(hp)/float(maximum);return std::clamp(int(std::min(fraction,1.f)*100.f)-1,0,99);}
}
