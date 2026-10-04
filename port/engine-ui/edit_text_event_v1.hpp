#pragma once
#include <functional>
#include <string>
#include <cstdint>
namespace dh2::ui::edit_text_event_v1 {
struct State {bool readonly{},focus{};std::int32_t cursor{};std::string text;};
struct Services {
 std::function<bool(std::string&)> active;
 std::function<bool(const char*,std::string&)> handler;
 std::function<bool(bool,std::string&)> listener;
 std::function<bool(std::string&)> format;
 std::function<bool(const std::string&,std::string&)> set_value;
};
// Source byte-oriented editing, not Unicode insertion or a shifted keyboard
// lookup. Format/set_value preserve their source false HTML argument.
bool event(State&,std::uint8_t id,std::uint8_t key,const Services&,bool& accepted,std::string&);
}
