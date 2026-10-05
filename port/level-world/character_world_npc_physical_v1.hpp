#pragma once
#include "character_world_npc_collision_v1.hpp"
#include "character_npc_body.hpp"
#include "physical_world.hpp"
#include "native_body.hpp"
namespace dh2::character {
bool character_npc_physical_debug_v1(DebugSwitches&,const DebugFileServices24&,
 const char* source_key,bool&,std::string&);
struct WorldNpcPhysicalServicesV1 {
 void* context{};
 // Resolve the actual PhysicalObject+8 owner from a persistent WorldObject
 // context; null owner is a valid source value. Do not cast unrelated owners.
 bool (*peer_owner)(void*,void* physical_context,std::uintptr_t&,std::string&){};
 // Source ObjectBase+80, only queried where default onCollisionTest reaches
 // it. Neither renderer visibility nor a nonnull scene supplies this byte.
 bool (*enabled80)(void*,std::uintptr_t,std::uint8_t&,std::string&){};
 // Original Character::UpdatePFObject continuation after physical assignment.
 // Provider must create/update the same actual PFObject and source capabilities
 // from genuine inputs. Missing flying/obstacle producers remain unavailable.
 int (*update_pf)(void*,std::uintptr_t,const physical::NativeBody*,const physical::NpcBodyProjection*,std::string&){};
 // Actual shared Debug Load/string/Get/deallocate. Required by initialize_source.
 bool (*debug_switch)(void*,const char*,bool&,std::string&){};
};
class CharacterWorldNpcPhysicalV1 {
 physical::NativeWorld& world_;
 physical::CharacterNpcBodyModel& model_;
 CharacterWorldNpcCollisionV1& collisions_;
 ScriptCharacterObject& object_;
 CharacterScriptSession& session_;
 WorldNpcPhysicalServicesV1 services_;
 physical::WorldObject world_object_{};
 physical::NativeBody native_{};
 physical::NpcBodyProjection projection_{};
 // PhysicalObject constructor +20/+22/+24 saved filter and +26=0. The
 // existing NPC factory creates exactly the source primary shape; secondary
 // shape is constructor-null on this recovered path.
 b2FilterData saved_filter_{};
 b2Shape* primary_{};
 b2Shape* secondary_{};
 std::uint8_t filter_disabled_{};
 std::uint32_t phase_{};
 std::string error_;
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
 [[noreturn]] void unavailable(const char*);
 bool initialize_impl(physical::NpcBodyRequest,bool source_debug);
public:
 CharacterWorldNpcPhysicalV1(physical::NativeWorld&,physical::CharacterNpcBodyModel&,
  CharacterWorldNpcCollisionV1&,ScriptCharacterObject&,CharacterScriptSession&,WorldNpcPhysicalServicesV1);
 ~CharacterWorldNpcPhysicalV1();
 CharacterWorldNpcPhysicalV1(const CharacterWorldNpcPhysicalV1&)=delete;
 CharacterWorldNpcPhysicalV1& operator=(const CharacterWorldNpcPhysicalV1&)=delete;
 // Initial source InitPhysical projection and genuine body allocation, shape,
 // mass, pin, assignment and explicit UpdatePFObject tail. Request borrows the
 // same property view/AI/model/placement; physical identities are produced here.
 // Failure retains the reached native body/assignment prefix, never success.
 bool initialize(physical::NpcBodyRequest);
 // Production path queries MP_NoCollisions before native allocation and
 // MP_NoPhysics after allocation, preserving the SetPhysicalObject prefix.
 bool initialize_source(physical::NpcBodyRequest);
 // Invoke before clearing the borrowed World and before destroying AI/VM/FSM.
 // Source collision End deliveries remain active during genuine DestroyBody.
 bool release();
 bool enable_filter();  // whole PhysicalObject::enableFilter 46ebe4
 bool disable_filter(); // whole PhysicalObject::disableFilter 46eb70
 std::uint8_t filter_disabled()const noexcept{return filter_disabled_;}
 physical::NativeBody& native()noexcept{return native_;}
 physical::WorldObject& world_object()noexcept{return world_object_;}
 const physical::NpcBodyProjection& projection()const noexcept{return projection_;}
 std::uint32_t phase()const noexcept{return phase_;}
 const std::string& error()const noexcept{return error_;}
};
}
