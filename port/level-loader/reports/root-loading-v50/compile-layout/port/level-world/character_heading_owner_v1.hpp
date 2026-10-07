#pragma once
#include "character_controller_commands.hpp"
#include "character_state.hpp"
#include "character_target_bindings.hpp"
#include "navigation_controller.hpp"
#include "native_body.hpp"
#include <string>
namespace dh2::character {
struct CharacterHeadingServicesV1 {
 void* context{};
 // GetTargetPosition returns the actual source backing array, retained across
 // the next synchronous callback; it must not be a temporary/copy.
 bool (*target_position)(void*,std::uintptr_t,const float*&,std::string&){};
 bool (*remotely_updated)(void*,bool&,std::string&){};
 bool (*updating_from_physics)(void*,bool&,std::string&){};
 bool (*raise_event)(void*,std::uint32_t,std::string&){};
};
// Owns orchestration only: every producer/mutation is the existing actor graph.
class CharacterHeadingOwnerV1 {
public:
 CharacterHeadingOwnerV1(ControllerCommandState32& command,State& machine,
  TargetState48& targets,const TargetServices16& target_services,
  navigation::PathController& object,navigation::PathObject& path,
  const float* actual_game_position,float* actual_game_destination,
  physical::NativeBody* const& body,CharacterHeadingServicesV1 services):
  command_(command),machine_(machine),targets_(targets),target_services_(target_services),
  object_(object),path_(path),position_(actual_game_position),destination_(actual_game_destination),body_(body),services_(services){}
 bool command_head_towards(const float direction[3],std::string&);
 bool command_stop(std::string&);
 bool character_head_towards(const float direction[3],std::string&);
 bool character_stop(std::string&);
 bool object_stop(std::string&);
private:
 bool permitted()const noexcept;
 ControllerCommandState32& command_;State& machine_;TargetState48& targets_;
 const TargetServices16& target_services_;navigation::PathController& object_;
 navigation::PathObject& path_;const float* position_;float* destination_;physical::NativeBody* const& body_;
 CharacterHeadingServicesV1 services_;
};
}
