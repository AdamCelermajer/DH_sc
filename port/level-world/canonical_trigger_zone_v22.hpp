#pragma once
#include "canonical_family_fields_v15.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include <set>
namespace dh2::world {
struct TriggerNetworkMemberV22 {std::uint32_t type134{32};std::uint64_t raw138{};std::int32_t raw140{-1},raw144{-1};std::uint32_t raw148{};std::uint8_t changed14c{};std::int32_t value150{};};
// Source398da4: three integer members type32 in declaration order. Native
// value backing is deliberately zero-initialized: authorized modern repair of
// the original constructor's indeterminate allocation-value read. Poisoned
// original notifications are NOT represented as zero-allocator source parity.
// Positive changes require the SAME source global change sequence producer.
struct TriggerNetworkOwnerV22 {
 std::array<TriggerNetworkMemberV22,3> members;std::array<TriggerNetworkMemberV22*,64> declared{};std::uint32_t count104{3};
 TriggerNetworkOwnerV22(){for(unsigned i=0;i<3;++i)declared[i]=&members[i];}
 bool set_value(std::size_t i,std::int32_t value,std::uint64_t* actual_global_sequence,std::string& e){if(i>=3){e="NetStructTrigger member index";return false;}auto& m=members[i];if(m.value150==value)return true;m.value150=value;m.changed14c=1;m.raw140=m.raw144=static_cast<std::int32_t>(m.raw148);if(!actual_global_sequence){e="Required SAME NetStruct global change sequence";return false;}m.raw138=*actual_global_sequence;++*actual_global_sequence;return true;}
};
struct TriggerLocalPlayerV22 {std::uintptr_t character{};std::uint8_t dead1480{};};
struct TriggerMarkerV22 {std::uintptr_t identity{};std::shared_ptr<void> lease;};
struct TriggerZoneServicesV22 {
 std::shared_ptr<void> owner;GameObjectInitializationServicesV1 initialization;
 std::function<bool(std::int32_t&,std::int32_t&,std::string&)> spawn_roll_probability;
 std::function<bool(const std::array<float,6>&,bool,std::string&)> set_bounding_box;
 // Whole positive _colzone branch: actual root node grab, unique mesh query,
 // mesh box -> InitWithBoundingBox -> hide mesh -> actual triangle selector.
 // It MUST return actual retained node, not a placeholder identity.
 std::function<bool(const char*,std::uintptr_t&,std::shared_ptr<void>&,std::string&)> bind_visual_collision_zone;
 std::function<bool(bool,std::string&)> create_zone_physical;
 std::function<bool(const char*,bool,std::int32_t&,std::string&)> script_id;
 std::function<bool(const char*,std::int32_t&,std::string&)> effect_id;
 std::function<bool(const char*,std::int32_t,bool,std::uintptr_t&,std::uint32_t&,std::string&)> find_named_object;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> door_state;
 std::function<bool(std::int32_t,std::uint8_t&,std::string&)> script_flags;
 std::function<bool(std::int32_t,bool,TriggerLocalPlayerV22&,std::string&)> local_player;
 std::function<bool(std::int32_t&,std::string&)> player_count;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player_character;
 std::function<bool(std::uintptr_t,bool&,std::string&)> touching;
 std::function<bool(std::uintptr_t,bool&,std::string&)> zone_inside,is_character,is_player;
 std::function<bool(bool&,std::string&)> online5,virtual54;
 std::function<bool(std::int32_t&,std::string&)> app_delta_ms;
 std::function<bool(std::string&)> require_online_update,play_idle_sound;
 std::function<bool(std::int32_t,bool&,std::string&)> script_running;
 std::function<bool(std::int32_t,std::int32_t,std::string&)> start_script;
 std::function<bool(std::int32_t,bool,TriggerMarkerV22&,std::string&)> create_marker;
 std::function<bool(TriggerMarkerV22&,std::uintptr_t,std::string&)> marker_owner;
 std::function<bool(TriggerMarkerV22&,bool,std::string&)> marker_restart,marker_visible,marker_loop;
 std::function<bool(TriggerMarkerV22&,std::string&)> marker_release;
 std::function<bool(TriggerNetworkOwnerV22&,std::string&)> destroy_network;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_base;
};
class CanonicalTriggerZoneV22 {
 CanonicalGameObjectBaseOwnerV1 base_;TriggerZoneServicesV22 services_;GameObjectInitializationOwnerV1 initialization_;
 std::array<float,3> dimensions_{};bool dimensions_written_{true}; // Zone C2 zero374/378/37c
 bool physical380_{},trigger381_{true};std::uintptr_t colzone384_{};std::shared_ptr<void> colzone_lease_;
 std::set<std::uintptr_t> contacts_;std::int32_t characters3a0_{},players3a4_{};
 std::int32_t count3a8_{1},delay3ac_{},activated3b4_{},timer3b8_{},touching3c0_{};std::uint8_t reset3b0_{},local_only3bc_{};
 std::array<TriggerNetworkOwnerV22,2> network_;
 std::array<std::int32_t,3> classification_{};std::array<bool,3> classification_written_{};
 std::array<std::string,6> names_;std::array<std::int32_t,5> resolved_{{-1,-1,-1,-1,-1}};
 TriggerMarkerV22 marker_;std::uint8_t active7b4_{},one_player7b5_{};std::uintptr_t door7b8_{};bool destroyed_{};
 bool missing(const char*,std::string&)const;bool zone_init_post(std::string&);bool trigger_update(std::string&);
 bool start_once(std::int32_t,std::string&);bool show_marker(std::string&);bool hide_marker(std::string&);bool number_touching(std::int32_t&,std::string&);
 void activate()noexcept;bool can_activate()noexcept;
public:
 CanonicalTriggerZoneV22(std::shared_ptr<void>,actor::RuntimeState&,TriggerZoneServicesV22,std::uint32_t source_go_id=20);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> l){return base_.canonical(std::move(l));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);bool write_bool(std::uint32_t,std::uint8_t,std::string&);bool write_int(std::uint32_t,std::int32_t,std::string&);bool write_string(std::uint32_t,const std::string&,std::string&);bool write_vector3(std::uint32_t,const std::array<float,3>&,std::string&);
 bool init_post(std::string&);bool init_final(std::string&);bool update(std::string&);bool destroy(std::string&);
 bool update_timer(std::string&);bool source_can_activate_v22()noexcept{return can_activate();}
 bool source_number_touching_v83(std::int32_t& count,std::string& e){return number_touching(count,e);}
 // Actual ZoneEx source insertion/removal order over the ctor-empty tree.
 bool collision_begin(std::uintptr_t,std::string&);bool collision_end(std::uintptr_t,std::string&);
 std::int32_t activation_count()const noexcept{return activated3b4_;}std::int32_t timer()const noexcept{return timer3b8_;}
 // Exact same source-field borrows for network/update adapters; absent means
 // genuinely unproduced, rather than a fabricated constructor default.
 std::int32_t* source_integer(std::uint32_t)noexcept;
 std::uint8_t* source_byte(std::uint32_t)noexcept;
 std::string* source_string(std::uint32_t)noexcept;
 bool source_set_position_v29(const std::array<float,3>&,bool,std::string&);
 const std::array<std::int32_t,5>& resolved_scripts()const noexcept{return resolved_;}
 std::uintptr_t source_colzone384_v83()const noexcept{return colzone384_;}
 bool set_updating(bool value,std::string& e){if(!base_.store_byte(0x85,value?1:0,e))return false;if(!value)active7b4_=0;return true;} // whole39b26c
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalTriggerZoneV22>,std::shared_ptr<const void>);
};
}
