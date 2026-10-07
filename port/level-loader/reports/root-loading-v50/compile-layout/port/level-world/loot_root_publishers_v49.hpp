#pragma once
#include "loot_source_fields_v47.hpp"
#include "retained_gameobject_decor_v1.hpp"
#include "canonical_podecor_body_v49.hpp"
namespace dh2::character {
struct PlayerSourceVisibilityServicesV49 {
 std::shared_ptr<void> receiver_lease;
 // Reached only by SetVisible. Source missing slot differs from ctor NULL.
 const std::uintptr_t* visual2d8{};
 void* context{};
 bool(*sync_visibility4713d0)(void*,std::uintptr_t actual_visual,std::uint8_t actual80,std::string&){};
};
// Only newly missing source slots. No owner/pose/default/InitPost replay.
// ObjectBaseC1 stores enabled8a1; PropertyMap ctor template8 is empty.
// Neither C1 stores80, retained in the SAME V47 field association.
class PlayerSourceVisibilityV49 {
 std::shared_ptr<LootPlayerFieldAssociationV47> fields_;
 std::shared_ptr<void> actor_lease_;
 const char* class_name20_{};const std::uint8_t* static84_{};
 std::string template8_;std::uint8_t enabled8a_{1};
 bool default_attempted_{};
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);
public:
 PlayerSourceVisibilityV49(std::shared_ptr<LootPlayerFieldAssociationV47>,
  std::shared_ptr<void> actual_actor_lease,const char* actual_class20,const std::uint8_t* actual_static84);
 world::CanonicalPropertyActorV1 properties()noexcept;
 bool publish_missing_default(world::CanonicalPropertyMapV1&,std::string&);
 bool set_visible38b0f0(bool,const PlayerSourceVisibilityServicesV49&,std::string&);
 std::uint8_t& source_enabled8a()noexcept{return enabled8a_;}
 std::string& source_template8()noexcept{return template8_;}
};
class LootPhysicalAssociationsV49 {
 world::CanonicalObjectManagerV1& manager_;
 struct Record {
  std::weak_ptr<void> lease;
  world::RetainedGameObjectDecorV1* decor{};
  world::CanonicalPodDecorBodyV49* pod{};
  const std::uintptr_t* nullable_owner8{};
 };
 std::map<void*,Record> records_; // callback-address index only, no object authority
 bool validate_decor(Record&,LootPhysicalPeerBorrowV44&,std::string&);
 bool validate_pod(Record&,LootPhysicalPeerBorrowV44&,std::string&);
public:
 explicit LootPhysicalAssociationsV49(world::CanonicalObjectManagerV1& m):manager_(m){}
 // Invoke at actual PODecor allocation BEFORE initialize/CreateShape callbacks.
 bool constructed_decor(world::RetainedGameObjectDecorV1&,std::shared_ptr<void> same_receiver_lease,std::string&);
 bool constructed_pod(world::CanonicalPodDecorBodyV49&,std::shared_ptr<void> same_receiver_lease,std::string&);
 // Only an actual PhysicalObject owner8 slot that contains NULL; never a
 // legacy DTO/unpublished canonical object shortcut. NULL shape userdata
 // takes native default filtering before custom callbacks and needs no entry.
 bool constructed_null_owner(void* actual_physical_context,const std::uintptr_t* actual_owner8,
  std::shared_ptr<void> same_receiver_lease,std::string&);
 bool peer(void* callback_address,LootPhysicalPeerBorrowV44&,std::string&);
 bool fields(std::uintptr_t canonical_owner,LootPhysicalPeerBorrowV44&,std::string&);
 bool recognizes(void* address)const noexcept{return records_.find(address)!=records_.end();}
 bool physical_contact(void*,navigation::PhysicalContact&,std::string&);
 // AFTER body release/Remove callbacks, before receiver/World destruction.
 void released(void* address)noexcept{records_.erase(address);}
 void clear_after_bodies_released()noexcept{records_.clear();}
};
}
