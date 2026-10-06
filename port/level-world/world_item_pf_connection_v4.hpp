#pragma once
#include "world_item_physical_v2.hpp"
#include "navigation_producers.hpp"
namespace dh2::character {
// Same canonical Item PFObject, same scene's collision/obstacle registry and
// same actual POItem. Constructor-null PF user takes the original early return;
// this does not pretend that GameObject::InitFinal has run.
class WorldItemPfConnectionV4 {
 RetainedWorldItemObjectV1& item_;
 const navigation::CollisionWorld* geometry_{};
 navigation::ObstacleRegistry* obstacles_{};
 WorldItemPhysicalV2* physical_{};
 std::shared_ptr<void> world_;
public:
 WorldItemPfConnectionV4(RetainedWorldItemObjectV1&,std::shared_ptr<void>,
   const navigation::CollisionWorld*,navigation::ObstacleRegistry*);
 void physical(WorldItemPhysicalV2* same)noexcept{physical_=same;}
 void suspend()noexcept{geometry_=nullptr;obstacles_=nullptr;}
 void rebind(const navigation::CollisionWorld* same,navigation::ObstacleRegistry* registry)noexcept{geometry_=same;obstacles_=registry;}
 bool update(std::string&);
};
}
