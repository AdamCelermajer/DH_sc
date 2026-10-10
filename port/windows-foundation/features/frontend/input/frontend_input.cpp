#include "frontend_input.hpp"
#include <algorithm>
#include <cmath>
#include <utility>

namespace dh::foundation::frontend::input {
NameResult validate_name_bytes(std::string_view name, std::size_t limit) {
    if (name.find('\0') != std::string_view::npos) return {NameStatus::embedded_nul, "Name contains a C-string terminator"};
    if (name.size() > std::min(limit, std::size_t{100})) return {NameStatus::too_long, "Name exceeds requested keyboard byte limit"};
    return {NameStatus::accepted, {}};
}
NameResult validate_authored_name(std::string_view name, const NamePolicy& policy, std::size_t limit) {
    auto result = validate_name_bytes(name, limit);
    if (!result.accepted()) return result;
    if (!policy) return {NameStatus::authored_policy_unavailable, "Authored name validation is not connected"};
    return policy(name);
}
NameResult validate_creation_name(std::string_view name) {
    auto result = validate_name_bytes(name);
    if (!result.accepted()) return result;
    if (name.find_first_not_of("\t\n\r ") == std::string_view::npos)
        return {NameStatus::authored_rejected, "Authored isValidName requires a nonempty trimmed name"};
    return {NameStatus::accepted, {}};
}
bool FrontendInput::available(ItemId id) const {
    return id && std::any_of(items_.begin(), items_.end(), [id](const Item& item) { return item.id == id && item.enabled; });
}
void FrontendInput::set_surface(std::vector<Item> items, std::function<ItemId(Point)> hit) {
    lose_focus(); items_ = std::move(items); hit_ = std::move(hit);
    if (!available(focused_)) { focused_ = 0; move_focus(1); }
}
void FrontendInput::update_items(std::vector<Item> items) {
    items_ = std::move(items);
    for(auto it=captures_.begin();it!=captures_.end();) {
        if(!available(it->second))it=captures_.erase(it);else ++it;
    }
    if(!available(return_capture_))return_capture_=0;
    if(!available(space_capture_))space_capture_=0;
    if(!available(focused_)){focused_=0;move_focus(1);}
}
void FrontendInput::set_enabled(bool enabled) { if (enabled_ != enabled) lose_focus(); enabled_ = enabled; }
void FrontendInput::focus(ItemId id) { if (available(id)) focused_ = id; }
void FrontendInput::move_focus(int direction) {
    std::vector<ItemId> enabled;
    for (const auto& item : items_) if (item.enabled && item.id) enabled.push_back(item.id);
    if (enabled.empty()) { focused_ = 0; return; }
    auto found = std::find(enabled.begin(), enabled.end(), focused_);
    if (found == enabled.end()) { focused_ = direction > 0 ? enabled.front() : enabled.back(); return; }
    auto index = std::distance(enabled.begin(), found);
    index = (index + direction + static_cast<std::ptrdiff_t>(enabled.size())) % static_cast<std::ptrdiff_t>(enabled.size());
    focused_ = enabled[static_cast<std::size_t>(index)];
}
void FrontendInput::activate(ItemId id) { if (available(id)) pending_.activated.push_back(id); }
void FrontendInput::key(int key, bool down, bool shift) {
    if (!enabled_) return;
    bool was_down = keys_[key]; keys_[key] = down;
    if (down == was_down) return;
    if (key == 0x1b && !down) { pending_.back = true; return; }
    if (editing_) {
        if (key == 8 && down && !name_.empty()) name_.pop_back();
        if (key == 13 && !down) pending_.text_committed = true;
        return;
    }
    if (down && (key == 0x26 || key == 0x25)) move_focus(-1);
    if (down && (key == 0x28 || key == 0x27 || key == 9)) move_focus(key == 9 && shift ? -1 : 1);
    if (key == 13 || key == 32) {
        auto& capture = key == 13 ? return_capture_ : space_capture_;
        if (down) capture = focused_;
        else { if (capture == focused_) activate(capture); capture = 0; }
    }
}
void FrontendInput::pointer(std::int64_t id, PointerPhase phase, Point point) {
    if (!enabled_) return;
    if (!std::isfinite(point.x) || !std::isfinite(point.y)) { captures_.erase(id); return; }
    if (phase == PointerPhase::cancel) { captures_.erase(id); return; }
    if (phase == PointerPhase::down) {
        captures_.erase(id);
        auto item = hit_ ? hit_(point) : 0;
        if (available(item)) { captures_[id] = item; focused_ = item; }
        return;
    }
    if (phase != PointerPhase::up) return;
    auto found = captures_.find(id);
    if (found == captures_.end()) return;
    auto captured = found->second; captures_.erase(found);
    if (hit_ && hit_(point) == captured) activate(captured);
}
void FrontendInput::lose_focus() {
    captures_.clear(); keys_.clear(); return_capture_ = space_capture_ = 0; pending_ = {};
}
bool FrontendInput::begin_name(std::string initial, std::size_t limit) {
    if (!validate_name_bytes(initial, limit).accepted()) return false;
    lose_focus(); name_ = std::move(initial); name_limit_ = std::min(limit, std::size_t{100}); editing_ = true; return true;
}
bool FrontendInput::text(std::string_view bytes) {
    if (!enabled_ || !editing_ || bytes.find('\0') != std::string_view::npos || name_.size() + bytes.size() > name_limit_) return false;
    name_.append(bytes); return true;
}
void FrontendInput::end_name() { lose_focus(); editing_ = false; }
Frame FrontendInput::take_frame() { Frame result = std::move(pending_); pending_ = {}; return result; }
}
