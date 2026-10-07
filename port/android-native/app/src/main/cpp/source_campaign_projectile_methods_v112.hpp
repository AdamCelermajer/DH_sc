#pragma once
#include <projectile_methods_v112.hpp>
#include <laser_projectile_methods_v112.hpp>
#include <character_script_session_v3.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct SourceCampaignProjectileMethodsNativeV112 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::shared_ptr<dh2::world::CanonicalProjectileV99>&,std::string&)> borrow;
 std::function<bool(dh2::world::CanonicalProjectileV99&,dh2::world::ProjectileMethodServicesV112&,
  dh2::world::LaserProjectileServicesV112&,std::string&)> lend;
 std::function<bool(dh2::world::CanonicalProjectileV99&,bool,std::string&)> visible;
};
bool install_source_campaign_projectile_methods_v112(const SourceCampaignCandidateBorrowV55&,
 SourceCampaignProjectileMethodsNativeV112,std::string&);
bool source_campaign_projectile_update_v112(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,std::string&);
bool source_campaign_projectile_collision_v112(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t projectile,std::uintptr_t peer,const float xy[2],std::string&);
bool source_campaign_projectile_fire_v112(const SourceCampaignCandidateBorrowV55&,
 std::int32_t row,std::uintptr_t source,std::uintptr_t target,std::uintptr_t check,
 std::uintptr_t hit,std::uintptr_t userdata,bool angle_variant,float angle,bool from_node,
 std::uintptr_t& result,std::string&);
//Called after the actual selected NPC/player Script input producer, before
//the original Session constructor registers globals. Preserves its remaining
//bindings; the caller retains the returned holder through VM finalizers.
bool bind_source_character_projectile_natives_v112(const std::shared_ptr<void>& actual_world,
 std::uintptr_t character,dh2::character::CharacterScriptSessionInput&,
 std::shared_ptr<void>& holder,std::string&);
bool bind_source_character_projectile_natives_v112(const std::shared_ptr<void>& actual_world,
 std::uintptr_t character,dh2::character::CharacterScriptSessionInputV3&,
 std::shared_ptr<void>& holder,std::string&);
}
