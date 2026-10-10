#pragma once
#include "../../input_actions.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

namespace dh::foundation::platform_input {
enum class Control : std::uint8_t {
    none, world, joystick, attack, skill1, skill2, skill3, spell, potion,
    interact, target, profile, menu_item, pause
};
enum class PointerPhase : std::uint8_t { down, move, up, cancel };
struct Point { float x{}, y{}; };
struct Hit { Control control{Control::none}; std::uint64_t item{}; };
struct ButtonEdges { bool pressed{}, held{}, released{}; };
struct Click { Hit hit{}; Point position{}; };
struct Frame {
    InputActions actions{};
    ButtonEdges attack{}, spell{}, potion{};
    std::array<ButtonEdges, 3> skills{};
    bool profile_pressed{}, pause_pressed{}, menu_back{};
    std::vector<Click> clicks;
    bool target_point_requested{};
    Point target_point{};
};
// Host supplies hits from actual authored HUD/menu shapes in its viewport space.
// Joystick callback supplies normalized intent using the captured authored stick.
struct Surface {
    std::function<Hit(Point)> hit;
    std::function<InputMove2D(Point, Point)> joystick;
};
// Public key values match Win32 VK / ASCII, without importing windows.h.
struct Bindings {
    int left{'A'}, right{'D'}, forward{'W'}, backward{'S'}, run{0x10};
    int attack{0x20}, interact{'E'}, target{0x09}, profile{'C'}, back{0x1b};
    std::array<int, 3> skills{{'1', '2', '3'}};
    int spell{'4'}, potion{'5'};
    bool default_run=true; // PC preference: Shift reverses to walking.
};
class SemanticInput {
public:
    explicit SemanticInput(Bindings bindings = {});
    void set_surface(Surface surface);
    void set_menu_open(bool open);
    // Global level input-disabled cancels controls; controller block only masks
    // movement/selection. Attack must reach original controller admission gates.
    void set_level_input_enabled(bool enabled);
    void key(int virtual_key, bool down);
    // Stable pointer IDs supplied by platform. Mouse and touch share this API.
    void pointer(std::int64_t id, PointerPhase phase, Point position);
    void lose_focus();
    Frame take_frame(bool controller_blocked = false);
    bool menu_open() const noexcept { return menu_open_; }
    std::size_t captured_pointers() const noexcept { return pointers_.size(); }
private:
    struct Capture { Hit hit; Point origin, current; bool menu{}; };
    Bindings bindings_;
    Surface surface_;
    bool menu_open_{}, level_enabled_{true};
    std::map<int, bool> keys_, suppressed_keys_;
    std::map<std::int64_t, Capture> pointers_;
    std::array<ButtonEdges, 6> buttons_{}; // attack, skills1..3, spell, potion
    Frame pending_;
    std::array<bool, 6> held_buttons() const;
    void refresh_buttons();
    void cancel_controls();
};
}
