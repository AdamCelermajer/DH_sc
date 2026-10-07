#pragma once
#include "character_target_bindings.hpp"
#include <memory>
#include <string>
namespace dh2::character {
struct CharacterUseOoiFieldsV47 {
 std::shared_ptr<void> receiver_lease;
 std::uintptr_t character{};
 TargetState48* ai3c8{}; // current408 is ai3c8->target
 const std::uintptr_t* ooi14a4{};
 std::uint8_t* heading412{}; // SAME SkillStateV4.heading_enabled
};
struct CharacterUseOoiServicesV47 {
 void* context{};
 bool(*remote34)(void*,std::uintptr_t,bool&,std::string&){};
 // Genuine SM_GetState: nullable StateInfo -> -1. Queried at each source call.
 bool(*state)(void*,std::uintptr_t,std::int32_t&,std::string&){};
 TargetServices16 target{};
};
class CharacterUseOoiV47 {
 CharacterUseOoiFieldsV47 fields_;CharacterUseOoiServicesV47 services_;
 bool eligible(bool&,std::string&);
 bool set(std::uintptr_t,std::string&);
public:
 CharacterUseOoiV47(CharacterUseOoiFieldsV47,CharacterUseOoiServicesV47);
 bool force(std::uintptr_t requested,std::string&);
 bool use(std::string&);
 bool control(std::uintptr_t requested,std::string&);
};
struct ControllerUseOoiFieldsV47 {
 std::shared_ptr<void> receiver_lease;
 std::uintptr_t controller{};
 // Source bytes use existing controller kernels' raw-word projections 0..255.
 // Borrow the existing fields; no second controller or endian byte alias.
 const std::uint32_t *forced9{},*locked8{},*networkA{},*global_blocked{};
 const std::uintptr_t *actorC{},*controllable4{};
};
struct UseOoiNetworkActorV47 {
 std::shared_ptr<void> receiver_lease;
 std::uintptr_t identity{};
 const std::uintptr_t* ooi14a4{};
 const std::uint32_t* network_id108{};
};
// Factory owns the actual message; source stores go directly to its fields.
struct UseOoiMessageV47 {
 std::shared_ptr<void> receiver_lease;std::uintptr_t identity{};
 std::uint8_t *action50{},*actor54{};
 std::uint16_t* selected52{};
};
struct ControllerUseOoiServicesV47 {
 void* context{};
 bool(*online5)(void*,bool&,std::string&){};
 bool(*actor)(void*,std::uintptr_t,UseOoiNetworkActorV47&,std::string&){};
 bool(*is_character24)(void*,std::uintptr_t,bool&,std::string&){};
 bool(*is_dead)(void*,std::uintptr_t,bool&,std::string&){};
 bool(*interaction90)(void*,std::uintptr_t selected,std::uintptr_t actor,std::int32_t&,std::string&){};
 bool(*client_get)(void*,std::uintptr_t&,std::shared_ptr<void>&,std::string&){};
 bool(*message_create)(void*,const char* class_name,bool,UseOoiMessageV47&,std::string&){};
 bool(*message_send)(void*,std::uintptr_t client,const UseOoiMessageV47&,std::string&){};
 bool(*control50)(void*,std::uintptr_t controllable,std::uintptr_t original_requested,std::string&){};
};
class ControllerUseOoiV47 {
 ControllerUseOoiFieldsV47 fields_;ControllerUseOoiServicesV47 services_;
public:
 ControllerUseOoiV47(ControllerUseOoiFieldsV47,ControllerUseOoiServicesV47);
 bool command(std::uintptr_t requested,std::string&);
 std::uintptr_t identity()const noexcept{return fields_.controller;}
};
}
