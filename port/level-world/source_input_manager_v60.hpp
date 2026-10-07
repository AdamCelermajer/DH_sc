#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <string>

namespace dh2::player {struct FirstLocalControllerServicesV59;struct FirstLocalControllerBorrowV59;}
namespace dh2::input {
// Typed native fields produced by the linked original constructor. Offset
// suffixes describe provenance, not the layout of this modern C++ object.
struct InputStateC1V60 {
 float word00{},word04{},word08{1.f},word0c{},word10{},word14{},word18{1.f};
 std::uint8_t byte1c{};
};
struct InputDeviceC1V60 {
 std::int32_t type4{};
 std::uint32_t word8{};
 explicit InputDeviceC1V60(std::int32_t type)noexcept:type4(type){}
};
struct KeyboardC1V60 {
 InputDeviceC1V60 device{1}; // Original Keyboard C1 also passes type1.
 std::array<InputStateC1V60,97> states;
};
struct MouseC1V60 {
 InputDeviceC1V60 device{1};
 std::array<InputStateC1V60,8> first_states,second_states;
};
struct GamepadC1V60 {
 InputDeviceC1V60 device{2};
 std::array<InputStateC1V60,45> states0c;
 std::array<InputStateC1V60,9> states5ac;
 std::array<std::array<float,3>,4> vectors6cc{},vectors70c{};
 std::array<std::uint32_t,4> indices6fc{{0,1,2,3}},indices73c{{0,1,2,3}};
 std::uint8_t byte74c{};
 std::uint32_t word750{},word754{128};
 std::uint8_t connected758{};
};

// InputManager.GetInstance34dda4 constructs the linked InputManagerWin32 C1
// even in the supplied Android ELF. That C1 has fixed counts1,1,4 and four
// embedded Gamepad C1 receivers. Counts are capacity, not detected hardware.
// This owner reconstructs its constructor/query domain; frame/event/device
// publication remains separate and must not be claimed implemented here.
class SourceInputManagerV60 final:public std::enable_shared_from_this<SourceInputManagerV60> {
 std::int32_t mouse_count4_{1},keyboard_count8_{1},gamepad_countc_{4};
 std::uint8_t enabled10_{1};
 KeyboardC1V60 keyboard_;
 MouseC1V60 mouse_;
 std::array<GamepadC1V60,4> gamepads_;
 static bool count_service(void*,std::int32_t&,std::string&);
 static bool first_gamepad_service(void*,player::FirstLocalControllerBorrowV59&,std::string&);
public:
 SourceInputManagerV60()=default;
 SourceInputManagerV60(const SourceInputManagerV60&)=delete;
 SourceInputManagerV60& operator=(const SourceInputManagerV60&)=delete;
 std::int32_t gamepad_count()const noexcept{return gamepad_countc_;}
 std::int32_t keyboard_count()const noexcept{return keyboard_count8_;}
 std::int32_t mouse_count()const noexcept{return mouse_count4_;}
 std::uint8_t enabled()const noexcept{return enabled10_;}
 const KeyboardC1V60& keyboard()const noexcept{return keyboard_;}
 const MouseC1V60& mouse()const noexcept{return mouse_;}
 bool get_gamepad(std::int32_t,std::shared_ptr<const GamepadC1V60>&,std::string&);
 bool get_first_connected_gamepad_v107(std::shared_ptr<const GamepadC1V60>&,std::string&);
 player::FirstLocalControllerServicesV59 first_local_services();
};
}
