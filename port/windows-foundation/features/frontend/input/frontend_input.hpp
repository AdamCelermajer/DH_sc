#pragma once
#include "../../platform_input/semantic_input.hpp"
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace dh::foundation::frontend::input {
using ItemId = std::uint64_t;
using Point = platform_input::Point;
using PointerPhase = platform_input::PointerPhase;
struct Item { ItemId id{}; bool enabled{true}; };
struct Frame { std::vector<ItemId> activated; bool back{}, text_committed{}; };
enum class NameStatus { accepted, embedded_nul, too_long, authored_policy_unavailable, authored_rejected };
struct NameResult { NameStatus status{}; std::string detail; bool accepted() const { return status == NameStatus::accepted; } };
using NamePolicy = std::function<NameResult(std::string_view)>;
// GSKeyboard::QueryString 0x385cf0 clamps requested size to 100;
// Update 0x385a8c supplies size+1, onEvent 0x51b444 reserves the terminator.
// FS_SetPlayerName 0x422280 supplies no alphabet/minimum-length restriction.
NameResult validate_name_bytes(std::string_view name, std::size_t requested_limit = 100);
// Authored menu_EnterName trim/isValidName removes only TAB/LF/CR/SPACE
// for the emptiness test and preserves the raw string sent to SetPlayerName.
NameResult validate_creation_name(std::string_view name);
NameResult validate_authored_name(std::string_view name, const NamePolicy& policy,
                                  std::size_t requested_limit = 100);

class FrontendInput {
public:
    // IDs and hit geometry come from the actual authored menu; zero is no hit.
    void set_surface(std::vector<Item> items, std::function<ItemId(Point)> hit);
    // Updates admission only, preserving current geometry and valid captures.
    void update_items(std::vector<Item> items);
    void set_enabled(bool enabled);
    void focus(ItemId id);
    ItemId focused() const noexcept { return focused_; }
    void key(int virtual_key, bool down, bool shift = false);
    void pointer(std::int64_t id, PointerPhase phase, Point point);
    void lose_focus();
    // Byte keyboard policy mirrors source; platform text/IME delivers encoded bytes.
    bool begin_name(std::string initial, std::size_t requested_limit = 100);
    bool text(std::string_view bytes);
    void end_name();
    const std::string& name() const noexcept { return name_; }
    bool editing_name() const noexcept { return editing_; }
    Frame take_frame();
private:
    std::vector<Item> items_;
    std::function<ItemId(Point)> hit_;
    std::map<std::int64_t, ItemId> captures_;
    std::map<int, bool> keys_;
    ItemId focused_{}, return_capture_{}, space_capture_{};
    bool enabled_{true}, editing_{};
    std::size_t name_limit_{100};
    std::string name_;
    Frame pending_;
    bool available(ItemId id) const;
    void move_focus(int direction);
    void activate(ItemId id);
};
}
