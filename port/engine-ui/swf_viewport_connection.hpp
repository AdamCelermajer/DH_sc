#pragma once
#include "viewport.hpp"
#include <memory>
#include <string>

namespace gameswf { struct root; }
namespace dh2::ui {
// The envelope must retain the EXACT root/player/ActionScript graph instance,
// not merely the outer SwfMovie whose load() can replace that instance. Call
// under the facade's existing core Scope when AS setters/watchers may execute.
struct SwfViewportLease {
    std::shared_ptr<void> owner;
    gameswf::root* root{};
};
struct SwfViewportDriver {
    void* context{};
    // Return true delivered. Values are the original renderer facts; no
    // surface-aspect or Android configuration inference is made here.
    bool (*orientation)(void*,std::int32_t&,std::string&){};
    bool (*dimensions)(void*,std::int32_t&,std::int32_t&,std::string&){};
};
class SwfViewportConnection {
public:
    SwfViewportConnection()=default;
    SwfViewportConnection(const SwfViewportConnection&)=delete;
    SwfViewportConnection& operator=(const SwfViewportConnection&)=delete;
    // Native bind rejection is atomic. Seed includes caller-supplied original
    // bounds/viewport; authored movie_rect must equal this root's definition.
    bool bind(SwfViewportLease,const ViewportState64&,const SwfViewportDriver&,std::string&);
    void release() noexcept;
    bool bound() const noexcept;
    const ViewportState64& state() const noexcept {return state_;}
    bool set_viewport(const std::int32_t xywh[4],std::string&);
    bool set_bounds(const std::int32_t xywh[4],std::int32_t mode,std::string&);
    bool camera_update(FlashCamera40&,std::string&);
    bool screen_to_logical(float point[2],std::string&);
    bool logical_to_screen(float point[2],std::string&);
    bool display_rectangle(float rectangle[4],std::string&);
    // Exact original notify_mouse_state field writes. The caller supplies
    // already-routed signed coordinates/buttons; no event, mapping or advance.
    bool notify_mouse_state(std::int32_t x,std::int32_t y,std::int32_t buttons,std::string&);
private:
    SwfViewportLease lease_;
    ViewportState64 state_{};
    SwfViewportDriver driver_{};
    std::string provider_error_;
    static int invoke(void*,ViewportState64*,const ViewportRequest40*,ViewportResponse16*);
    void refresh_player() noexcept;
    void synchronize() noexcept;
    bool finish(int,std::string&);
};
}
