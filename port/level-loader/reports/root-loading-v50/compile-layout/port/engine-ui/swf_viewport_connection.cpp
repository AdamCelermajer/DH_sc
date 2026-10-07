#include "swf_viewport_connection.hpp"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_object.h"
#include <cstring>
#include <exception>

namespace dh2::ui {
bool SwfViewportConnection::bind(SwfViewportLease lease,const ViewportState64& seed,
                                const SwfViewportDriver& driver,std::string& error) {
    if(!lease.owner||!lease.root||!lease.root->m_def||!driver.orientation||!driver.dimensions||seed.reserved){
        error="Required retained SWF viewport owner/driver unavailable";return false;
    }
    const auto& rect=lease.root->m_def->m_frame_size;
    const float authored[4]={rect.m_x_min,rect.m_x_max,rect.m_y_min,rect.m_y_max};
    if(std::memcmp(authored,seed.movie_rect,sizeof(authored))){error="Viewport seed differs from retained authored rectangle";return false;}
    lease_=std::move(lease);state_=seed;driver_=driver;provider_error_.clear();
    refresh_player();synchronize();error.clear();return true;
}
void SwfViewportConnection::release() noexcept {lease_.root=nullptr;lease_.owner.reset();state_={};driver_={};provider_error_.clear();}
bool SwfViewportConnection::bound() const noexcept {return lease_.owner&&lease_.root;}
void SwfViewportConnection::refresh_player() noexcept {
    state_.player_receiver=bound()?reinterpret_cast<std::uintptr_t>(lease_.root->m_player.get_ptr()):0;
}
void SwfViewportConnection::synchronize() noexcept {
    if(!bound())return;
    auto& root=*lease_.root;
    root.m_viewport_x0=state_.viewport[0];root.m_viewport_y0=state_.viewport[1];
    root.m_viewport_width=state_.viewport[2];root.m_viewport_height=state_.viewport[3];
    root.m_pixel_scale=state_.pixel_scale;
}
int SwfViewportConnection::invoke(void* context,ViewportState64* state,
    const ViewportRequest40* request,ViewportResponse16* response) {
    auto& self=*static_cast<SwfViewportConnection*>(context);
    // Retain the exact graph through synchronous driver/AS reentry, including
    // a callback releasing/rebinding this adapter. Never close it mid-setter.
    const auto retained=self.lease_.owner;
    try {
        if(!self.bound()||state!=&self.state_){self.provider_error_="Retained SWF viewport owner detached";return 0;}
        switch(request->operation){
        case ViewportOperation::orientation:
            return self.driver_.orientation(self.driver_.context,response->values[0],self.provider_error_)?1:0;
        case ViewportOperation::driver_dimensions:
            return self.driver_.dimensions(self.driver_.context,response->values[0],response->values[1],self.provider_error_)?1:0;
        case ViewportOperation::camera_set_viewport: {
            self.refresh_player();self.synchronize();ViewportServices16 services{&self,invoke};
            return dh2_ui_set_viewport(&self.state_,request->values,&services)==0?1:0;
        }
        case ViewportOperation::camera_set_bounds: {
            self.refresh_player();self.synchronize();ViewportServices16 services{&self,invoke};
            return dh2_ui_set_bounds(&self.state_,request->values,0,&services)==0?1:0;
        }
        case ViewportOperation::publish_viewport: {
            // Prefix writes must be visible before an AS property/watcher
            // synchronously reenters the source bounds coordinator.
            self.synchronize();auto* player=self.lease_.root->m_player.get_ptr();
            if(!player)return 1; // fresh weak-player read, no substitute owner
            gameswf::gc_ptr<gameswf::as_object> object=new gameswf::as_object(player);
            constexpr const char* names[4]={"xMin","yMin","xMax","yMax"};
            for(unsigned i=0;i<4;++i)object->set_member(names[i],gameswf::as_value(static_cast<double>(request->rectangle[i])));
            player->get_global()->set_member("Viewport",gameswf::as_value(object.get_ptr()));
            return 1;
        }
        }
        self.provider_error_="Unsupported original viewport operation";return 0;
    } catch(const std::exception& e){self.provider_error_=e.what();return 0;}
}
bool SwfViewportConnection::finish(int status,std::string& error) {
    synchronize();
    if(status){error=provider_error_.empty()?"Required source viewport operation failed: "+std::to_string(status):provider_error_;return false;}
    error.clear();return true;
}
bool SwfViewportConnection::set_viewport(const std::int32_t xywh[4],std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    provider_error_.clear();refresh_player();ViewportServices16 services{this,invoke};
    return finish(dh2_ui_set_viewport(&state_,xywh,&services),error);
}
bool SwfViewportConnection::set_bounds(const std::int32_t xywh[4],std::int32_t mode,std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    provider_error_.clear();refresh_player();ViewportServices16 services{this,invoke};
    return finish(dh2_ui_set_bounds(&state_,xywh,mode,&services),error);
}
bool SwfViewportConnection::camera_update(FlashCamera40& camera,std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    provider_error_.clear();refresh_player();ViewportServices16 services{this,invoke};
    return finish(dh2_ui_flash_camera_update(&camera,&state_,&services),error);
}
bool SwfViewportConnection::screen_to_logical(float point[2],std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    provider_error_.clear();ViewportServices16 services{this,invoke};
    return finish(dh2_ui_screen_to_logical(&state_,point,&services),error);
}
bool SwfViewportConnection::logical_to_screen(float point[2],std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    provider_error_.clear();ViewportServices16 services{this,invoke};
    return finish(dh2_ui_logical_to_screen(&state_,point,&services),error);
}
bool SwfViewportConnection::display_rectangle(float rectangle[4],std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    provider_error_.clear();ViewportServices16 services{this,invoke};
    return finish(dh2_ui_display_rectangle(&state_,rectangle,&services),error);
}
bool SwfViewportConnection::notify_mouse_state(std::int32_t x,std::int32_t y,std::int32_t buttons,std::string& error) {
    if(!bound()){error="Retained SWF viewport owner detached";return false;}
    auto& root=*lease_.root;root.m_mouse_buttons=buttons;root.m_mouse_x=x;root.m_mouse_y=y;
    error.clear();return true;
}
}
