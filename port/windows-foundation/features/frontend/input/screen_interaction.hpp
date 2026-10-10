#pragma once
#include "frontend_input.hpp"
#include "../flow/menu_flow.hpp"

namespace dh::foundation::frontend::input {
struct PathItem { std::string path; bool enabled{true}; };
// Host supplies the current source hit-region order. hit returns index+1, zero
// for no authored contour; path dispatch uses source names, never pixel guesses.
class ScreenInteraction {
public:
    explicit ScreenInteraction(flow::Navigator& navigator): navigator_(navigator) {}
    void selected_slot(flow::SlotFact fact) { slot_ = fact; navigator_.set_selected_slot(std::move(fact)); }
    void selected_difficulty(int difficulty) { difficulty_ = difficulty; }
    // MenuSelectClass original preview transition owns this admission gate.
    // Back remains admitted; arrows and Confirm require completed transition.
    void set_animation_input_enabled(bool enabled);
    void set_surface(std::vector<PathItem>,std::function<ItemId(Point)>);
    void key(int vk,bool down,bool shift=false);
    bool text(std::string_view bytes);
    void pointer(std::int64_t id,PointerPhase phase,Point point) { input_.pointer(id,phase,point); }
    void lose_focus() { input_.lose_focus(); keys_.clear(); pending_paths_.clear(); }
    bool flush(std::string& error);
    bool dispatch(std::string_view path,std::string& error);
    bool enabled(std::string_view path) const;
    const std::string& name() const { return input_.name(); }
    unsigned class_index() const { return class_index_; }
    bool keyboard_caps() const { return caps_; }
    bool uppercase_visible() const { return upper_visible_; }
private:
    flow::Navigator& navigator_;
    FrontendInput input_;
    std::vector<PathItem> paths_;
    flow::SlotFact slot_;
    std::string screen_;
    unsigned class_index_{};
    int difficulty_{-1};
    bool caps_{},upper_visible_{true};
    bool animation_input_enabled_{true};
    std::map<int,bool> keys_;
    std::vector<std::string> pending_paths_;
    void sync();
    bool append(std::string_view bytes);
};
}
