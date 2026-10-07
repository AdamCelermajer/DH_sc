#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::ui {
struct SwfAnimTooltipServicesV104 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,std::uintptr_t&,std::string&)> grab;
 std::function<bool(std::uintptr_t,std::string&)> drop;
 std::function<bool(std::uintptr_t,const char*,std::string&)> play;
 std::function<bool(std::uintptr_t,bool,std::string&)> visibility;
 std::function<bool(std::uintptr_t,bool&,std::string&)> visible,playing;
 std::function<bool(std::uint32_t&,std::string&)> dt;
 std::function<bool(std::string&)> missing_anim_assertion;
};
//Whole actual SWFAnimToolTip C1/IsVisible/FadeIn/FadeOut/Update/D1.
//The original SWFAnimManager owns the borrowed anim8 art/clip instance.
class SwfAnimTooltipV104 final {
 SwfAnimTooltipServicesV104 services_;
 std::int32_t delay0_{},state4_{};std::uintptr_t anim8_{};
 bool construct_attempted_{},destroyed_{};
 bool play(const char*,std::string&);bool over(bool&,std::string&);
 bool do_fade_out(std::string&);
public:
 explicit SwfAnimTooltipV104(SwfAnimTooltipServicesV104 services):services_(std::move(services)){}
 bool construct(std::string&);bool destroy(std::string&);
 bool is_visible(bool&,std::string&);bool fade_in(std::string&);bool fade_out(std::int32_t,std::string&);bool update(std::string&);
 std::uintptr_t anim()const noexcept{return anim8_;}
};
}
