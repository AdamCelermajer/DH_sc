#pragma once
#include "character_world_runtime_v1.hpp"
#include "character_world_npc_object_v1.hpp"
#include "physical_world.hpp"
#include <list>
namespace dh2::character {
// Index of PhysicalObject+8 borrows, not an ObjectManager/target authority.
// Each nonnull owner must already exist in the canonical registered World.
struct WorldPhysicalPeerBorrowV1 {
 physical::WorldObject* physical{};physical::NativeBody* body{};
 std::uintptr_t identity{};WorldNpcObjectFieldsV1* object_fields{};
 const std::uint32_t* object_type{}; // SAME ObjectBase+f4 constructor field.
};
// Read-only registration adapter for real base GameObjects (AnimatedDecor).
// Position, node180 and lifecycle bytes remain borrowed source owners.
class WorldPhysicalBaseActorV1 {
 std::uintptr_t identity_;const float* position_;WorldNpcObjectFieldsV1& fields_;
 const scene::Scene* scene_;const std::uintptr_t& node_;target_search::Object48 search_{};
 static int refresh(void*,skills::WorldTargetActorBorrowV1*);
public:
 WorldPhysicalBaseActorV1(std::uintptr_t id,const float* position,WorldNpcObjectFieldsV1& fields,
  const scene::Scene* scene,const std::uintptr_t& node):identity_(id),position_(position),fields_(fields),scene_(scene),node_(node){}
 skills::WorldActorRegistrationV1 registration(std::int32_t key,target_providers::Handle16&);
};
class CharacterWorldPhysicalPeersV1 {
 skills::CharacterWorldRuntimeV1& world_;
 std::list<WorldPhysicalPeerBorrowV1> peers_;
 std::string error_;
public:
 explicit CharacterWorldPhysicalPeersV1(skills::CharacterWorldRuntimeV1& w):world_(w){}
 bool add(const WorldPhysicalPeerBorrowV1&);
 // Remove only after DestroyBody has delivered End to still-live receivers.
 bool remove(void* physical_context);
 bool clear();
 bool owner(void* physical_context,std::uintptr_t&,std::string&);
 bool enabled(std::uintptr_t,std::uint8_t&,std::string&);
 bool type(std::uintptr_t,std::uint32_t&,std::string&);
 const std::string& error()const noexcept{return error_;}
 static bool peer_owner(void*,void*,std::uintptr_t&,std::string&);
 static bool enabled80(void*,std::uintptr_t,std::uint8_t&,std::string&);
 static bool object_type(void*,std::uintptr_t,std::uint32_t&,std::string&);
};
enum class WorldPhysicalReceiverV1 {base_physical,character};
struct WorldPhysicalCharacterServicesV1 {
 void* context{};
 // Whole POCharacter filter prefix over the same registered machine.
 int (*filter)(void*,std::uint16_t,bool&,std::string&){};
 // Whole POCharacter Begin/Persist/End, with source Debug/handle/RaiseEvent.
 // May borrow CharacterWorldNpcCollisionV1 for NPCs; player needs its own
 // actual Character/AI delivery. No absent callback succeeds.
 int (*event)(void*,physical::ContactEvent,std::uintptr_t,unsigned,std::string&){};
};
// Reusable actual player/decor WorldObject adapter. Decor uses original
// PhysicalObject empty contact bodies3883c4/c8/cc/d0. Character events are
// mandatory continuations. Never interprets another receiver's C++ layout.
class CharacterWorldPhysicalReceiverV1 {
 CharacterWorldPhysicalPeersV1& peers_;physical::NativeBody& body_;
 std::uintptr_t identity_;WorldPhysicalReceiverV1 kind_;
 WorldPhysicalCharacterServicesV1 services_;physical::WorldObject object_{};
 std::string error_;
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
 [[noreturn]] void fail(const char*);
public:
 CharacterWorldPhysicalReceiverV1(CharacterWorldPhysicalPeersV1&,physical::NativeBody&,
  std::uintptr_t,WorldPhysicalReceiverV1,WorldPhysicalCharacterServicesV1={});
 physical::WorldObject& world_object()noexcept{return object_;}
 const std::string& error()const noexcept{return error_;}
};
}
