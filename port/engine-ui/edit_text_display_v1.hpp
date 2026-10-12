#pragma once
#include "text_display_v2.hpp"
#include "text_filter_v1.hpp"
#include <functional>
namespace dh2::ui::edit_text_display_v1 {
struct Owner { std::shared_ptr<text_filter_v1::Effect> effect; std::weak_ptr<Owner> parent; };
struct State {
    std::shared_ptr<Owner> owner;
    bool border{}, grid_fit{}, focus{}, callback_present{};
    std::uint32_t background{0xffffffff};
    text_display_v2::Rect rectangle{};
    float xcursor{},ycursor{},text_height{};
};
struct Policy { bool buffering{},flushing{},render_cache{},filter_engine{};float provider_scale{1}; };
struct Command {
    enum Kind {matrix,fill_color,mesh,line_color,line_width,line,cache_set,cache_draw,grid_fit} kind{};
    text_display_v2::Matrix transform{1,0,0,0,1,0};
    std::uint32_t rgba{};float width{};bool enabled{};
    std::vector<float> xy;
};
struct Services {
    std::function<bool(Policy&,std::string&)> policy;
    std::function<bool(bool&,std::string&)> renderer_present;
    std::function<bool(text_display_v2::Matrix&,std::string&)> world_matrix;
    std::function<bool(const Command&,std::string&)> renderer;
    std::function<bool(std::string&)> enqueue;
    std::function<bool(bool&,std::string&)> cache_validate;
    std::function<bool(const text_display_v2::Context&,bool supplied_matrix,std::string&)> records;
    std::function<bool(std::string&)> display_callback;
    // Original imported trig. Required only for a reached shadow record.
    std::function<bool(float,float&,std::string&)> cosine,sine;
};
// Complete 7928cc outer coordinator. The caller projects native owners and
// supplies genuine renderer/cache/root services. Reached unavailable services
// fail after the same completed prefix. No stock mask or reformat is invoked.
bool display(State&,const Services&,std::string&);
bool cursor(State&,const Services&,std::string&);
}
