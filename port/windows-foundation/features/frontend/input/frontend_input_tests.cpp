#include "frontend_input.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace dh::foundation::frontend::input;
void require(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
int main() { try {
    require(validate_name_bytes(std::string(100, 'a')).accepted(), "source 100-byte QueryString cap");
    require(!validate_name_bytes(std::string(101, 'a')).accepted(), "source cap exceeded");
    require(validate_name_bytes("").accepted(), "native SetPlayerName does not impose minimum");
    require(!validate_creation_name("").accepted() && !validate_creation_name(" \t\r\n").accepted(), "authored trim emptiness");
    require(validate_creation_name("\v").accepted() && validate_creation_name(" ! ").accepted(), "invented whitespace/alphabet filter");
    require(!validate_name_bytes(std::string("a\0b", 3)).accepted(), "C string boundary");
    require(validate_authored_name("a", {}).status == NameStatus::authored_policy_unavailable, "missing authored provider succeeded");
    FrontendInput input;
    auto hit = [](Point p)->ItemId { return p.x >= 0 && p.x < 20 ? (p.x < 10 ? 1 : 2) : 0; };
    input.set_surface({{1,true},{2,false},{3,true}}, hit);
    require(input.focused() == 1, "initial enabled focus");
    input.key(0x28,true); input.key(0x28,true); input.key(0x28,false);
    require(input.focused() == 3, "skip disabled and suppress OS repeat");
    input.key(13,true); require(input.take_frame().activated.empty(), "activate on down");
    input.key(13,false); require(input.take_frame().activated == std::vector<ItemId>{3}, "keyboard release activation");
    input.pointer(7,PointerPhase::down,{1,1}); input.pointer(7,PointerPhase::up,{15,1});
    require(input.take_frame().activated.empty(), "drag activated different/disabled target");
    input.pointer(7,PointerPhase::down,{1,1}); input.pointer(8,PointerPhase::down,{1,1});
    input.pointer(7,PointerPhase::cancel,{1,1}); input.pointer(8,PointerPhase::up,{1,1});
    require(input.take_frame().activated == std::vector<ItemId>{1}, "independent touch cancellation");
    input.key(32,true); input.lose_focus(); input.key(32,false);
    require(input.take_frame().activated.empty(), "focus loss fabricated click");
    input.pointer(7,PointerPhase::down,{1,1}); input.set_surface({{1,true}},hit); input.pointer(7,PointerPhase::up,{1,1});
    require(input.take_frame().activated.empty(), "stale pointer crosses menu replacement");
    require(input.begin_name(" raw ",8), "raw initialization");
    require(input.text("abc") && !input.text("d") && input.name() == " raw abc", "requested eight-byte editor limit/raw preservation");
    input.key(8,true); input.key(8,true); input.key(8,false);
    require(input.name() == " raw ab", "backspace edge");
    input.key(13,true); require(!input.take_frame().text_committed, "text Return completes on release");
    input.key(13,false); require(input.take_frame().text_committed, "Return release did not commit");
    input.set_enabled(false); require(!input.text("x"), "disabled keyboard accepted text");
    input.pointer(2,PointerPhase::down,{1,1}); input.set_enabled(true); input.pointer(2,PointerPhase::up,{1,1});
    require(input.take_frame().activated.empty(), "disabled touch captured");
    input.end_name();
    input.key(13,true); input.pointer(9,PointerPhase::down,{1,1});
    input.set_enabled(false); // Host focus loss cancels key and pointer captures.
    input.key(13,false); input.pointer(9,PointerPhase::up,{1,1});
    require(input.take_frame().activated.empty(), "focus loss cancels held keyboard/pointer actions");
    input.set_enabled(true);
    input.key(13,false); input.pointer(9,PointerPhase::up,{1,1});
    require(input.take_frame().activated.empty(), "focus regain does not complete stale captures");
    input.key(13,true); input.pointer(9,PointerPhase::down,{1,1});
    input.key(13,false); input.pointer(9,PointerPhase::up,{1,1});
    require(input.take_frame().activated == std::vector<ItemId>{1,1},
            "fresh input works after actual focus resumes");
    std::cout << "PASS frontend input: authored name trim, source byte bounds, PC release, touch capture, cancellation, focus, disabled gates\n";
    return 0;
} catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; } }
