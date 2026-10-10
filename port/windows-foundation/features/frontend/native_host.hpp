#pragma once
#include "input/frontend_input.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace dh::foundation::frontend {
struct HostEvent {
 enum class Kind { key, pointer, text, focus_lost } kind{};
 int key{};bool down{},shift{};
 std::int64_t pointer{};input::PointerPhase phase{};input::Point point{};
 std::string text;
};
// Subclasses only this diagnostic's own focused Win32 window. Core Window
// retains the HWND/WGL lifetime and still receives every original message.
class NativeHostInput {
public:
 NativeHostInput()=default;~NativeHostInput();
 NativeHostInput(const NativeHostInput&)=delete;
 NativeHostInput& operator=(const NativeHostInput&)=delete;
 bool attach_focused_window(std::string& error);
 std::vector<HostEvent> take_events();
 // Native event smoke tests send only to this diagnostic's attached HWND.
 bool post_pointer(input::Point,bool down);
 bool post_key(int virtual_key,bool down);
 bool post_ascii_text(const std::string&);
private:
 void* window_{};void* previous_{};
 std::vector<HostEvent> events_;
 static std::intptr_t procedure(void*,unsigned,std::uintptr_t,std::intptr_t);
};
}
