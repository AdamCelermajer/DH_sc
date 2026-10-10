#pragma once

#include "source_summon.hpp"
#include "../actor_frame/source_current_level_backend_v1.hpp"
#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../level-world/character_script_session.hpp"

namespace dh2::enemy_ai {

struct SourceSummonBackendLeavesV1 {
 // Each leaf receives the same current-Level borrow captured at CreateNPC and
 // must call through its actual source PF/RoomZone/Application owner.
 std::function<bool(const dh::foundation::actor_frame::SourceCurrentLevelBorrowV1&,
  world::CanonicalClassReceiverV1&,const std::array<float,3>&,std::string&)> set_initial_position;
 std::function<bool(const dh::foundation::actor_frame::SourceCurrentLevelBorrowV1&,
  world::CanonicalClassReceiverV1&,const std::array<float,3>&,std::string&)> set_rotation;
 std::function<bool(std::uintptr_t,const std::array<float,3>&,const std::array<float,3>&,
  const std::array<float,3>&,bool,std::array<float,3>&,std::string&)> position_from_offsets;
 std::function<bool(std::uintptr_t,std::array<float,3>&,std::array<float,3>&,std::string&)> source_object_pose;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::string&)> set_idle_state;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::string&)> mark_summoned;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::string&)> zone_entered;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,bool,bool,std::string&)> set_spawn_state;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::int32_t,std::string&)> forward_animation;
 std::function<bool(bool&,std::string&)> online_byte5;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::string&)> assign_network_id;
 std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&)> online_post_assignment;
 std::function<bool(const char*,std::string&)> unknown_type_debug;
 // Actual same caller RoomZone::AddInitialObject. `accepted` is the source
 // insertion result; false selects the existing manager no-room continuation.
 std::function<bool(const dh::foundation::actor_frame::SourceCurrentLevelBorrowV1&,
  std::uintptr_t,std::uintptr_t,bool&,std::string&)> room_add_initial_object;
};

struct SourceSummonBackendInputsV1 {
 std::shared_ptr<world::CanonicalCharacterCandidateFactoryV60> characters;
 world::CanonicalClassReceiverBindingsV1* receivers{};
 std::shared_ptr<world::CanonicalObjectManagerV1> objects;
 world::CanonicalPropertyMapV1* properties{};
 std::shared_ptr<dh::foundation::actor_frame::SourceCurrentLevelBackendV1> current_level;
 const data::CharacterTable* character_table{};
 SourceSummonOwnerV1 caller;
 SourceSummonBackendLeavesV1 leaves;
 std::function<bool(const world::CanonicalObjectBorrowV1&,
  world::CanonicalClassReceiverV1&,std::string&)> resolve_same_receiver;
};

// Per-container owner of the exact source _Summon 0x39193c callback. It borrows
// the current Character factory/receiver map/ObjectManager/PropertyMap/Level;
// it constructs no manager, Character, VM, room, PF world, or RNG of its own.
class SourceSummonBackendBindingV1 {
 SourceSummonBackendInputsV1 inputs_;
 SourceSummonServicesV1 services_;
 SourceCreateNpcServicesV1 create_services_;
 SourceCanonicalSpawnOwnerV1 spawn_owner_;
 dh::foundation::actor_frame::SourceCurrentLevelBorrowV1 active_level_;
 std::shared_ptr<void> request_lease_;
 bool callback_active_{};
 bool binding_installed_{};
 void* previous_gameplay_context_{};
 int(*previous_gameplay_binding_)(void*,std::uint32_t,dh2_script_function*,void**){};
 bool validate_inputs(std::string&)const;
 bool same_character_receiver(const world::CanonicalClassReceiverV1&,
  std::shared_ptr<world::CanonicalCharacterCandidateRecordV60>&,std::string&);
 bool current_level(dh::foundation::actor_frame::SourceCurrentLevelBorrowV1&,std::string&);
 bool active_level(std::string&);
 bool construct_character(const world::CanonicalFactoryEntryV1&,
  const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&);
 bool resolve_handle(target_providers::Handle16&,bool,
  const world::CanonicalObjectBorrowV1*&,std::string&);
 bool resolve_receiver(const world::CanonicalObjectBorrowV1&,
  const world::CanonicalClassReceiverV1*&,std::string&);
 bool create_npc(std::int32_t,SourceCreateNpcResultV1&,std::string&);
 bool source_manager_spawn(const char*,const char*,bool,bool,
  world::CanonicalClassReceiverV1&,bool&,std::string&);
 bool gameplay_callback(const dh2_script_value*,std::uint32_t,dh2_script_value*,
  std::uint32_t,std::uint32_t*,char*,std::size_t);
 static int summon_callback(void*,const dh2_script_value*,std::uint32_t,
  dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
 static bool spawn_construct(void*,const world::CanonicalFactoryEntryV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 static bool spawn_debug(void*,const char*,std::string&);
 static bool spawn_resolve(void*,target_providers::Handle16&,bool,
  const world::CanonicalObjectBorrowV1*&,std::string&);
 static bool spawn_condition(void*,const world::CanonicalObjectBorrowV1&,bool,std::string&);
 static bool spawn_virtual38(void*,const world::CanonicalObjectBorrowV1&,bool&,std::string&);
 static bool spawn_append_pending(void*,const world::CanonicalObjectBorrowV1&,std::string&);
 static bool spawn_receiver(void*,const world::CanonicalObjectBorrowV1&,
  const world::CanonicalClassReceiverV1*&,std::string&);
 static int chained_gameplay_binding(void*,std::uint32_t,dh2_script_function*,void**);
public:
 explicit SourceSummonBackendBindingV1(SourceSummonBackendInputsV1);
 SourceSummonBackendBindingV1(const SourceSummonBackendBindingV1&)=delete;
 SourceSummonBackendBindingV1& operator=(const SourceSummonBackendBindingV1&)=delete;
 static int gameplay_binding(void*,std::uint32_t,dh2_script_function*,void**);
 bool attach_gameplay_binding(character::CharacterScriptSessionInput&,std::string&);
 SourceSummonServicesV1& source_services()noexcept{return services_;}
};

} // namespace dh2::enemy_ai


