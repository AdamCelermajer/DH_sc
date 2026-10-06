#pragma once
#include "character_loot_live_v22.hpp"
#include "world_loot_pickup_v23.hpp"
namespace dh2::character {
// A callback capability over existing canonical receivers. This owns no
// Character fields, copies no sheet and derives no template/OID from names.
struct CharacterLootActorFieldsV31 {
 world::CanonicalObjectBorrowV1 canonical;
 data::PropertyView* properties{};
 std::uintptr_t* object_of_interest14a4{};
 player::PlayerEquipmentRenderOwnerV1* gear{};
 std::shared_ptr<void> receiver_lease;
};
struct CharacterLootActorServicesV31 {
 std::shared_ptr<void> provider_lease;void* context{};
 bool(*borrow)(void*,std::uintptr_t,CharacterLootActorFieldsV31&,std::string&){};
};
class CharacterLootActorBindingV31 {
 skills::CharacterWorldRuntimeV1& world_;const data::AiTables& ai_;
 CharacterLootActorServicesV31 services_;
 bool fields(std::uintptr_t,CharacterLootActorFieldsV31&,skills::WorldTargetActorBorrowV1&,std::string&);
public:
 CharacterLootActorBindingV31(skills::CharacterWorldRuntimeV1& w,const data::AiTables& ai,CharacterLootActorServicesV31 s):world_(w),ai_(ai),services_(std::move(s)){}
 bool drop_actor(std::uintptr_t,CharacterLootLiveBorrowV22&,std::string&);
 bool is_player(std::uintptr_t,bool&,std::string&);
 bool character_cast(std::uintptr_t,std::uintptr_t&,std::string&);
 bool pickup_actor(std::uintptr_t,LootPickupActorV23&,std::string&);
};
}
