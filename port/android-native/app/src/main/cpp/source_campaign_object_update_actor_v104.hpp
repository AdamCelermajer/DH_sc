#pragma once
#include <canonical_object_update_v102.hpp>
#include <room_zone_runtime_v104.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool borrow_source_campaign_object_update_actor_v104(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,dh2::world::ObjectUpdateActorV102&,std::string&);
bool source_campaign_resolve_update_handle_v104(const SourceCampaignCandidateBorrowV55&,
 const dh2::world::ObjectUpdateActorV102&,std::uintptr_t&,std::string&);
bool source_campaign_object_update_debug_v104(const SourceCampaignCandidateBorrowV55&,std::string&);
bool source_campaign_character_update_pointers_v105(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,std::string&);
bool borrow_source_campaign_character_room_v104(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,dh2::world::RoomObjectBorrowV104&,std::string&);
}
