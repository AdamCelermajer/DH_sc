#include "character_world_npc_object_v1.hpp"
namespace dh2::character {
namespace {
bool source_obstacle(void*,bool& value,std::string&){value=true;return true;}
bool source_radius(void*,float& value,std::string&){value=50.f;return true;}
bool source_strength(void*,float& value,std::string&){value=20.f;return true;}
}
WorldNpcObjectServicesV1 character_pf_services_v1(WorldNpcObjectServicesV1 s) noexcept {
 s.is_pf_obstacle=source_obstacle;s.obstacle_weight=source_radius;
 s.obstacle_extent=source_strength;return s;
}
CharacterWorldNpcObjectV1::CharacterWorldNpcObjectV1(WorldNpcObjectFieldsV1& f,
 navigation::NavigationObject& pf,const navigation::CollisionWorld* g,
 navigation::ObstacleRegistry* o,std::uint64_t id,WorldNpcObjectServicesV1 s)
 :fields_(f),pf_(pf),geometry_(g),obstacles_(o),identity_(id),services_(s){}
bool CharacterWorldNpcObjectV1::method(WorldNpcObjectMethodV1 m){
 if(!services_.method){error_="required source Object lifecycle method unavailable";return false;}
 if(services_.method(services_.context,m,error_)){if(error_.empty())error_="source Object lifecycle method failed";return false;}return true;
}
bool CharacterWorldNpcObjectV1::visual(){
 if(!services_.has_visual){error_="required source VisualObject presence unavailable";return false;}
 if(services_.has_visual(services_.context))return method(WorldNpcObjectMethodV1::SyncVisibility);
 return true;
}
bool CharacterWorldNpcObjectV1::set_visible_impl(bool v){
 fields_.visible80=v?fields_.enabled8a:0;fields_.visible_written=true;
 return visual();
}
bool CharacterWorldNpcObjectV1::set_visible(bool v){error_.clear();return set_visible_impl(v);}
bool CharacterWorldNpcObjectV1::game_enabled(bool enabled){
 if(enabled){if(!set_visible_impl(true))return false;fields_.updating85=1;pf_.motion.object_flags|=8;}
 else{fields_.updating85=0;if(!set_visible_impl(false))return false;pf_.motion.object_flags&=~8u;}
 if(!services_.physical){error_="required source PhysicalObject presence unavailable";return false;}
 if(services_.physical(services_.context)&&!method(enabled?WorldNpcObjectMethodV1::EnablePhysicalFilter:WorldNpcObjectMethodV1::DisablePhysicalFilter))return false;
 fields_.disabled373=enabled?0:1;return true;
}
bool CharacterWorldNpcObjectV1::enabled(){error_.clear();return game_enabled(true);}
bool CharacterWorldNpcObjectV1::disabled(){error_.clear();return game_enabled(false)&&method(WorldNpcObjectMethodV1::CharacterStop);}
bool CharacterWorldNpcObjectV1::set_enable(bool enabled){
 error_.clear();if(fields_.enabled8a==unsigned(enabled))return true;
 fields_.enabled8a=enabled?1:0;return enabled?game_enabled(true):(game_enabled(false)&&method(WorldNpcObjectMethodV1::CharacterStop));
}
bool CharacterWorldNpcObjectV1::zone_entered(){
 error_.clear();fields_.entered2f0=1;
 if(fields_.zoning2ee){
  if(!services_.has_visual){error_="required source VisualObject presence unavailable";return false;}
  if(services_.has_visual(services_.context)){
   if(!fields_.visible_written){error_="source ObjectBase byte80 has no producer";return false;}
   if(fields_.visible80&&!method(WorldNpcObjectMethodV1::SyncVisibility))return false;
  }
 }
 fields_.updating85=(!fields_.non_zonable2ed&&fields_.zoning2ee)?fields_.entered2f0:1;return true;
}
bool CharacterWorldNpcObjectV1::zone_exited(){
 error_.clear();fields_.entered2f0=0;
 if(fields_.zoning2ee&&!visual())return false;
 fields_.updating85=(!fields_.non_zonable2ed&&fields_.zoning2ee)?fields_.entered2f0:1;return true;
}
bool CharacterWorldNpcObjectV1::read_visible(std::uint8_t& v)const noexcept{
 if(!fields_.visible_written)return false;v=fields_.visible80;return true;
}
bool CharacterWorldNpcObjectV1::init_pf_object(const float* position,const float* aabb){
 error_.clear();
 if(!position||!aabb){error_="required source InitFinal position/AABB unavailable";return false;}
 const float width=aabb[3]-aabb[0],height=aabb[4]-aabb[1];
 // The source passes the full extent here, unlike UpdatePFObject's half extent.
 const navigation::ObjectInitRequest request{geometry_,&pf_,identity_,
  {position[0],position[1],position[2]},width<height?height:width,fields_.static84,0};
 if(dh2_nav_init_object(&request)){error_="source PFWorld::InitObject rejected inputs";return false;}
 return true;
}
bool CharacterWorldNpcObjectV1::update_pf(){
 error_.clear();if(pf_.motion.floor==~0u)return true;
 if(!services_.is_pf_obstacle){error_="required source IsPFObstacle virtual unavailable";return false;}
 bool obstacle=false;if(!services_.is_pf_obstacle(services_.context,obstacle,error_))return false;
 if(obstacle){
  if(!services_.physical){error_="required source PhysicalObject presence unavailable";return false;}
  const bool had_physical=services_.physical(services_.context)!=nullptr;
  float weight=0,extent=0;
  if(!services_.obstacle_weight){error_="required source PF obstacle weight unavailable";return false;}
  if(!services_.obstacle_weight(services_.context,weight,error_))return false;
  if(!services_.obstacle_extent){error_="required source PF obstacle extent unavailable";return false;}
  if(!services_.obstacle_extent(services_.context,extent,error_))return false;
  const navigation::ObstacleInitRequest request{geometry_,obstacles_,&pf_,identity_,weight,extent,unsigned(had_physical),0};
  if(dh2_nav_init_obstacle(&request)){error_="source PFWorld::InitObstacle backend unavailable or rejected inputs";return false;}
 }
 if(!services_.physical){error_="required source PhysicalObject presence unavailable";return false;}
 auto* physical=services_.physical(services_.context);
 if(physical){pf_.radius=physical->radius*100.f;return true;}
 if(!services_.absolute_aabb){error_="required source absolute AABB unavailable";return false;}
 float aabb[6];if(!services_.absolute_aabb(services_.context,aabb,error_))return false;
 const float width=aabb[3]-aabb[0],height=aabb[4]-aabb[1];
 pf_.radius=(width<height?height:width)*.5f;return true;
}
}
