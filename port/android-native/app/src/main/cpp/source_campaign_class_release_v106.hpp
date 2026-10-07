#pragma once
#include <memory>
#include <string>
namespace dh2::world {class CanonicalGameObjectBaseOwnerV1;class CanonicalRoomZoneV3;}
namespace model_renderer {
// Ordinary source class D0 may run during ObjectManager.Update. This checks
// actual GL/draw/physics/contact/Scene delivery; it never closes the World.
bool source_campaign_gameobject_destroy_v107(const std::shared_ptr<void>& actual_world,dh2::world::CanonicalGameObjectBaseOwnerV1&,std::string&);
bool source_campaign_room_destroy_v106(const std::shared_ptr<void>& actual_world,dh2::world::CanonicalRoomZoneV3&,std::string&);
bool require_source_campaign_class_delivery_v106(const std::shared_ptr<void>& actual_world,std::string&);
}
