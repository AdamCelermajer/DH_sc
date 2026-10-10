#pragma once
#include "original_actor_bounds.hpp"
#include "original_trigger_contacts.hpp"
#include "../level-world/physical_world.hpp"
#include "../level-world/character_set_position_v7.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/properties.hpp"
namespace dh::foundation {
struct OriginalActorSubobjectsBorrow {
 std::shared_ptr<void> owner_lease;
 std::uintptr_t identity{};
 float* position160{};float* destination1a8{};
 const float* relative144{};float* absolute12c{};
 const std::uintptr_t* attached2e0{};const std::uintptr_t* visual2d8{};
 const std::uintptr_t* physical2dc{};
 dh2::physical::NativeBody* native{};
};
struct OriginalActorPhysicalBindings {
 std::shared_ptr<void> actor_lease,world_lease,data_lease;
 std::uintptr_t identity{};
 float* position160{};float* destination1a8{};
 const std::uintptr_t* attached2e0{};const std::uintptr_t* visual2d8{};
 dh2::physical::NativeWorld* world{};
 dh2::data::PropertyView* properties{};const dh2::data::AiTables* ai{};
 // Read-only same resolved sheets alternative; this owner never recalculates
 // or writes properties. Exactly one property backing must be supplied.
 const dh2::data::PropertyState* readonly_properties{};
 // Actual original static84 and IsPlayer branch result producers.
 std::function<bool(std::uint8_t&,std::string&)> static84;
 std::function<bool(std::int32_t,bool&,std::string&)> is_player;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(std::uintptr_t,const dh2::physical::NativeBody&,const dh2::physical::CharacterBodyConfig&,std::string&)> update_pf;
 std::function<bool(void*,const dh2::physical::Filter&,const dh2::physical::Filter&,bool&,std::string&)> filter;
 std::function<bool(dh2::physical::ContactEvent,void*,unsigned,std::string&)> contact;
 dh2::character::CharacterPositionServicesV7 position;
 std::function<void()> publish_position;
};
// Lightweight in-house storage; SAME borrowed actor position/property sheets,
// no second HP, target, FSM or script owner. Uses recovered body/position math.
class OriginalActorPhysical : public std::enable_shared_from_this<OriginalActorPhysical> {
public:
 explicit OriginalActorPhysical(OriginalActorPhysicalBindings);
 ~OriginalActorPhysical();
 OriginalActorPhysical(const OriginalActorPhysical&)=delete;
 bool bind_bounds(const OriginalActorBoundsResult&,std::string&);
 bool initialize(std::string&);
 bool set_position(const std::array<float,3>&,bool destination,std::string&);
 bool release(std::string&);
 bool set_filter_enabled(bool,std::string&);
 // PhysicalObject::setFilter / resetFilter from the source character states.
 // These preserve the saved filter and the source's applySecondary policy;
 // they are separate from GameObject DisableCollisions.
 bool set_source_filter(std::int16_t group,std::uint16_t category,
                       std::uint16_t mask,bool apply_secondary,std::string&);
 bool reset_source_filter(std::string&);
 bool set_pinned(bool,std::string&);
 bool actor_borrow(OriginalTriggerActorBorrow&,std::string&)const;
 bool physical_borrow(std::uintptr_t,OriginalTriggerPhysicalBorrow&,std::string&)const;
 // Reacquire at each source phase: lifecycle can change physical assignment.
 // Null native is the actual absent physical2dc slot, not a guessed policy.
 bool subobjects_borrow(OriginalActorSubobjectsBorrow&,std::string&);
 const dh2::physical::NativeBody& native()const{return body_;}
 const dh2::physical::CharacterBodyConfig& definition()const{return config_;}
 const float* relative_bounds()const{return relative_.data();}
 unsigned phase()const{return phase_;}
 std::uintptr_t actor_identity()const noexcept{return bindings_.identity;}
private:
 OriginalActorPhysicalBindings bindings_;
 std::array<float,6> relative_{},absolute_{};std::uintptr_t physical2dc_{};
 dh2::physical::NativeBody body_{};dh2::physical::WorldObject world_object_{};
 dh2::physical::CharacterBodyConfig config_{};
 b2Shape* primary_{};b2Shape* secondary_{};b2FilterData saved_filter_{};
 std::uint8_t filter_disabled_{};
 bool bounds_bound_=false;unsigned phase_=0;std::string error_;
 bool validate(std::string&)const;
 static unsigned test(void*,void*,const dh2::physical::Filter*,const dh2::physical::Filter*);
 static void contact(void*,dh2::physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
};
}
