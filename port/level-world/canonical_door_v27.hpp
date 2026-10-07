#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "zone_startup_v76.hpp"
#include "source_startup_sound_v87.hpp"
#include "../game-data/game_object_dictionary_v11.hpp"
#include <optional>
namespace dh2::world {
class CanonicalDoorV27;
// Original3e80d4 constructs three bool members in order, and two separate
// owners3b0/540. Zero backing is an explicit modern safety correction for the
// original poison-sensitive read of member+1d; original notifications are not
// represented as parity with a zero allocator.
struct DoorNetworkMemberV27 {
 std::uint32_t type134{1};std::uint64_t sequence138{};
 std::int32_t raw140{-1},raw144{-1};std::uint32_t raw148{};
 std::uint8_t changed14c{},value14d{};
};
struct DoorNetworkOwnerV27 {
 std::array<DoorNetworkMemberV27,3> members;
 std::array<DoorNetworkMemberV27*,64> declared{};
 std::uint32_t count104{3};
 DoorNetworkOwnerV27(){for(unsigned i=0;i<3;++i)declared[i]=&members[i];}
 DoorNetworkOwnerV27(const DoorNetworkOwnerV27&)=delete;
 DoorNetworkOwnerV27& operator=(const DoorNetworkOwnerV27&)=delete;
};
// Read-only typed borrow of the SAME original Arrays::Doors producer.
// Source24-byte row has dispatch0, CString length4/pointer8 and signedc/10/14; modern typed
// fields are not an ARM-image/vptr cast or a second table/registry.
struct DoorDeclarationRowV77 {std::string script4;std::int32_t sound_c{},sound10{},visual14{};};
struct DoorTableBorrowV77 {
 std::shared_ptr<const void> receiver;
 const std::vector<std::string>* names{};
 const std::vector<DoorDeclarationRowV77>* rows{};
 std::shared_ptr<const data::GameObjectDictionaryV11> objects;
};
struct DoorStartupServicesV77 {
 std::shared_ptr<void> owner;ZoneStartupServicesV76 zone;
 std::function<bool(DoorTableBorrowV77&,std::string&)> borrow_tables;
 // SAME visual+38 controller binds Door::__Callback3e8568 and actual Door
 // userdata, plus __EventCallback3e8450 with THIS userdata, then ApplyMeshBox.
 std::function<bool(CanonicalDoorV27&,std::uintptr_t,std::string&)> register_animation_callbacks,apply_mesh_box;
 // Genuine40-byte physical C1: App44 world, THIS,1,stack0/0/0/0/2/0xffff/1,
 // original derived dispatch, then394bf8 SetPhysicalObject(false). Retain
 // every allocated native failure prefix in Main's existing resource journal.
 std::function<bool(CanonicalDoorV27&,std::string&)> create_door_physical;
 std::function<bool(StartupSoundManagerBorrowV87&,std::string&)> borrow_sound_manager;
 // Actual controller virtual20/name/arg2/arg3/stackarg4, never fake animation.
 std::function<bool(CanonicalDoorV27&,std::uintptr_t,const char*,bool,std::int32_t,std::uint32_t,std::string&)> play_idle_animation;
 std::function<bool(CanonicalDoorV27&,std::uintptr_t,bool,std::string&)> physical_filter;
 // Exact PFWorld5252ec over THIS runtime PFObject1c8 and current source world.
 std::function<bool(CanonicalDoorV27&,bool,std::string&)> flag_floor_dead_end;
 std::function<bool(CanonicalDoorV27&,std::string&)> source_delete;
};
struct DoorServicesV27 {
 std::shared_ptr<void> owner;DoorStartupServicesV77 startup;
 std::function<bool(CanonicalDoorV27&,std::string&)> whole_destroy;
};
// Source340824 -> Door3e8274 -> Zone397ca0 -> GameObject2. Behavior, real
// Zone collision, animation/sound, conditions and save/network execution are
// required engine services; this receiver transports actual fields/identity.
class CanonicalDoorV27 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 GameObjectInitializationOwnerV1 initialization_;
 DoorServicesV27 services_;
 std::array<float,3> dimensions374_{};
 bool physical380_{},trigger381_{true};
 std::uintptr_t colzone384_{};std::shared_ptr<void> colzone_lease_;
 std::string data388_;
 std::optional<std::int32_t> table_id3a0_;
 std::uint8_t opened3a4_{},collision3a5_{1},byte3ac_{};
 std::int32_t state3a8_{};
 std::array<DoorNetworkOwnerV27,2> network_;
 bool destroyed_{};
 bool startup_spawn_gate_v77(bool&,std::string&);
 bool startup_state_v77(bool opened,std::string&);
 bool source_transition_v91(bool opened,bool skip_animation,std::string&);
public:
 CanonicalDoorV27(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,DoorServicesV27);
 CanonicalDoorV27(const CanonicalDoorV27&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);
 bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 bool write_vector3(std::uint32_t,const std::array<float,3>&,std::string&);
 std::array<float,3>& source_dimensions374()noexcept{return dimensions374_;}
 const std::array<float,3>& dimensions()const noexcept{return dimensions374_;}
 bool physical()const noexcept{return physical380_;}
 bool trigger()const noexcept{return trigger381_;}
 std::uintptr_t& source_colzone384()noexcept{return colzone384_;}
 std::shared_ptr<void>& source_colzone_lease()noexcept{return colzone_lease_;}
 std::string& source_data388()noexcept{return data388_;}
 std::optional<std::int32_t>& source_table_id3a0()noexcept{return table_id3a0_;}
 std::uint8_t& source_opened3a4()noexcept{return opened3a4_;}
 std::uint8_t& source_collision3a5()noexcept{return collision3a5_;}
 std::int32_t& source_state3a8()noexcept{return state3a8_;}
 std::uint8_t& source_byte3ac()noexcept{return byte3ac_;}
 DoorNetworkOwnerV27& source_network(unsigned i){return network_.at(i);}
 bool init_post(std::string&);
 bool init_final(std::string&);
 bool source_enabled_tail_v105(bool enabled,std::string& e){
  if(state3a8_==1)return true;
  if(!services_.startup.flag_floor_dead_end){e="Required actual Door Enabled/Disabled FlagFloorAsDeadEnd";return false;}
  return services_.startup.flag_floor_dead_end(*this,enabled,e);
 }
 bool source_opened_v91(bool skip_animation,std::string& e){return source_transition_v91(true,skip_animation,e);}
 bool source_closed_v91(bool skip_animation,std::string& e){return source_transition_v91(false,skip_animation,e);}
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static constexpr bool is_updatable()noexcept{return true;}
 static constexpr bool is_animated()noexcept{return true;}
 static constexpr bool is_interactive()noexcept{return false;}
 static constexpr bool is_zonable()noexcept{return true;}
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalDoorV27>,std::shared_ptr<const void>);
};
}
