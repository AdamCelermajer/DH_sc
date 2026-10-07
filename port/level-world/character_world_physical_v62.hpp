#pragma once
#include "character_world_npc_physical_v1.hpp"
#include "navigation_avoidance.hpp"
#include "native_physical_filter_v1.hpp"
namespace dh2::character {
// Borrow the actual selected AIS/VM dispatcher. A player uses its existing V6
// receiver; an NPC uses its existing ScriptSession. Neither is constructed here.
struct CharacterPhysicalCollisionBorrowV62 {
 std::shared_ptr<void> receiver;
 std::function<bool(std::uint16_t,bool&,std::string&)> filter;
 std::function<bool(WorldNpcPhysicalEventV1,std::uintptr_t,std::uint32_t,std::string&)> event;
 std::string diagnostic;
 bool permits_filter(std::uint16_t c,bool& allowed){if(!receiver||!filter){diagnostic="Required same Character collision filter dispatcher";return false;}return filter(c,allowed,diagnostic);}
 bool physical_event(WorldNpcPhysicalEventV1 e,std::uintptr_t peer,std::uint32_t p){if(!receiver||!event){diagnostic="Required same selected AIS physical-event dispatcher";return false;}return event(e,peer,p,diagnostic);}
 const std::string& error()const noexcept{return diagnostic;}
};
class CharacterWorldPhysicalV62 {
 physical::NativeWorld& world_;
 CharacterPhysicalCollisionBorrowV62 collision_;
 ScriptCharacterObject& object_;
 data::PropertyView& properties_;
 WorldNpcPhysicalServicesV1 services_;
 std::function<bool(std::uintptr_t,const physical::NativeBody*,bool,std::string&)> source_assignment_;
 physical::WorldObject world_object_{};
 physical::NativeBody native_{};
 physical::NpcBodyProjection projection_{};
 b2FilterData saved_filter_{};
 b2Shape* primary_{};b2Shape* secondary_{};
 std::uint8_t filter_disabled_{};std::uint32_t phase_{};bool published_v7_{};
 std::string error_;
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
 [[noreturn]] void unavailable(const char*);
public:
 using ProjectionV62=std::function<bool(const physical::NpcBodyRequest&,physical::NpcBodyProjection&,std::string&)>;
 CharacterWorldPhysicalV62(physical::NativeWorld&,ScriptCharacterObject&,data::PropertyView&,CharacterPhysicalCollisionBorrowV62,WorldNpcPhysicalServicesV1,
  std::function<bool(std::uintptr_t,const physical::NativeBody*,bool,std::string&)> source_assignment={});
 ~CharacterWorldPhysicalV62();
 CharacterWorldPhysicalV62(const CharacterWorldPhysicalV62&)=delete;
 bool initialize_source(physical::NpcBodyRequest,const ProjectionV62&);
 bool release();bool enable_filter();bool disable_filter();
 // DeadState focus/blur targets the actual primary shape, independently of
 // AI DisablePhysicsFilter's separate byte and optional secondary shape.
 bool source_death_filter_v84(std::int16_t,std::uint16_t,std::uint16_t);
 bool source_reset_filter_v84();
 bool borrow_avoidance_contact_v108(std::uint8_t actual_owner80,
  navigation::PhysicalContact&,std::string&)const;
 bool source_filter_borrow_v111(physical::NativePhysicalFilterBorrowV1& out,std::string& e){
  if(!published_v7_||!native_.body){e="Required actual published POCharacter filter receiver";return false;}
  out={&world_,&native_,&primary_,&secondary_,&saved_filter_,&filter_disabled_};return true;
 }
 physical::NativeBody& native()noexcept{return native_;}
 physical::WorldObject& world_object()noexcept{return world_object_;}
 const physical::NpcBodyProjection& projection()const noexcept{return projection_;}
 std::uint32_t phase()const noexcept{return phase_;}
 const std::string& error()const noexcept{return error_;}
};
}
