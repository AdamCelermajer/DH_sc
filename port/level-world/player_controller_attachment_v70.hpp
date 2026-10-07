#pragma once
#include "character_controller_commands.hpp"
#include "event_manager_owner_v12.hpp"
#include "source_input_manager_v60.hpp"
#include <functional>
namespace dh2::player {
struct PlayerControllerChildV70 {
 enum class Kind {gamepad,emulation,hud};Kind kind;
 character::ControllerCommandState32 command{};
 std::uintptr_t controllable4{};
 std::uint8_t network_a{};
 std::int32_t gamepad_index10{};
 std::uint32_t word14{},word18{},word1c{};
};
struct PlayerControllerAttachmentServicesV70 {
 std::shared_ptr<events::EventManagerOwnerV12> events;
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(const events::EventBorrowV12&,std::int32_t,std::int32_t&,std::string&)> emulation_event;
};
// Whole offline _AttachControllerToPlayerCharacter36f0dc constructors and
// ownership/publication. Child Update/input methods remain distinct source
// services: attachment does not claim to have delivered an input frame.
class PlayerControllerAttachmentV70 {
 struct Proxy {std::uintptr_t controller4{};} proxy_;
 character::ControllerCommandState32 mixed_{};
 std::uintptr_t controllable4_{};std::uint8_t network_a_{};
 std::vector<std::unique_ptr<PlayerControllerChildV70>> children_;
 std::unique_ptr<PlayerControllerChildV70> constructing_;
 struct CallbackContext {PlayerControllerAttachmentV70* owner{};};
 std::shared_ptr<CallbackContext> callback_token_{std::make_shared<CallbackContext>()};
 PlayerControllerAttachmentServicesV70 services_;
 bool attempted_{},published_{},attached0_{},attached2_{},complete_{};
 static bool event(void*,const events::EventBorrowV12&,events::EventManagerOwnerV12&,std::int32_t&,std::string&);
public:
 PlayerControllerAttachmentV70()=default;
 ~PlayerControllerAttachmentV70();
 PlayerControllerAttachmentV70(const PlayerControllerAttachmentV70&)=delete;
 bool construct(std::uintptr_t actor,std::uintptr_t actual_controllable374,std::int32_t gamepad_index,
  PlayerControllerAttachmentServicesV70,
  const std::function<bool(character::ControllerCommandState32&,std::uintptr_t,bool,std::string&)>& publish,
  std::string&);
 bool close(std::string&);
 bool source_update_v107(input::SourceInputManagerV60&,
  const std::function<bool(PlayerControllerChildV70&,const std::shared_ptr<const input::GamepadC1V60>&,std::string&)>&,
  std::string&);
 bool source_end_loading_unblock_v97(std::uintptr_t actual_controller,std::string&);
 character::ControllerCommandState32& command()noexcept{return mixed_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(&mixed_);}
 std::uintptr_t controllable()const noexcept{return controllable4_;}
 std::uint8_t network_enabled()const noexcept{return network_a_;}
 bool complete()const noexcept{return complete_;}
 const auto& children()const noexcept{return children_;}
};
}

