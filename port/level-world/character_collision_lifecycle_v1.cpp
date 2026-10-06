#include "character_collision_lifecycle_v1.hpp"
namespace dh2::character {
bool CharacterCollisionLifecycleV1::apply(bool enabled){
 error_.clear();
 if(!services_.physical){error_="Required source PhysicalObject presence";return false;}
 if(services_.physical(services_.context)){
  if(!services_.method||services_.method(services_.context,enabled?WorldNpcObjectMethodV1::EnablePhysicalFilter:WorldNpcObjectMethodV1::DisablePhysicalFilter,error_)){
   if(error_.empty())error_="Required whole source PhysicalObject filter lifecycle";return false;
  }
 }
 bool obstacle{};
 if(!services_.is_pf_obstacle||!services_.is_pf_obstacle(services_.context,obstacle,error_)){
  if(error_.empty())error_="Required actual virtual IsPFObstacle+b4";return false;
 }
 if(!obstacle)return true;
 float weight{},extent{};
 if(!services_.obstacle_weight||!services_.obstacle_weight(services_.context,weight,error_)){
  if(error_.empty())error_="Required actual PF obstacle virtual+b8";return false;
 }
 if(!services_.obstacle_extent||!services_.obstacle_extent(services_.context,extent,error_)){
  if(error_.empty())error_="Required actual PF obstacle virtual+bc";return false;
 }
 const navigation::ObstacleInitRequest request{geometry_,&obstacles_,&object_,identity_,weight,extent,unsigned(enabled),0};
 const auto result=dh2_nav_init_obstacle(&request);
 if(result){error_="Required source PFWorld.InitObstacle backing/status "+std::to_string(result);return false;}
 return true;
}
}
