#include "original_ui_input_session_v1.hpp"
#include <exception>
namespace dh2::android_ui {
struct OriginalUiInputSessionV1::State {
 struct Surface {
  // Independent real provider lifetime; it owns no session/strong graph value.
  std::shared_ptr<void> original_owner;
  int width{},height{};std::int32_t orientation{};
  static bool dimensions(void*p,std::int32_t&w,std::int32_t&h,std::string&){auto&s=*static_cast<Surface*>(p);w=s.width;h=s.height;return true;}
  static bool direction(void*p,std::int32_t&o,std::string&){o=static_cast<Surface*>(p)->orientation;return true;}
 };
 std::shared_ptr<Surface>surface;
 ui::SwfInputSessionV2 session;
 ui::SwfInputSessionStatusV2 status;
 ui::FlashCamera40 camera{};
};
struct OriginalUiInputSessionV1::Control {std::shared_ptr<State>state;std::uint64_t revision{};};
OriginalUiInputSessionV1::OriginalUiInputSessionV1():control_(std::make_shared<Control>()){};
OriginalUiInputSessionV1::~OriginalUiInputSessionV1(){release();}
bool OriginalUiInputSessionV1::load(const ui::SwfInputSessionConfigV1&config,int w,int h,std::int32_t orientation,std::int32_t initial_ms,bool initial_flag,const char*sha,std::string&e){
 auto control=control_;const auto revision=control->revision;
 if(w<=0||h<=0||!sha||!config.provider_owner||!config.input_services.owner||!config.input_services.can_handle_event||!config.input_services.native_event){e="Required actual HUD/input provider or surface unavailable";return false;}
 try{
  auto next=std::make_shared<State>();next->surface=std::make_shared<State::Surface>();next->surface->original_owner=config.provider_owner;next->surface->width=w;next->surface->height=h;next->surface->orientation=orientation;
  auto source=config;source.provider_owner=next->surface;source.driver={next->surface.get(),State::Surface::direction,State::Surface::dimensions};
  if(!next->session.load(source,e)||!next->session.camera_state(next->camera,e)||!next->session.update(initial_ms,initial_flag,e)||!next->session.bind_status_hud(sha,next->status,e))return false;
  if(control->revision!=revision){e="HUD candidate cancelled by outer owner release";return false;}control->state=std::move(next);++control->revision;e.clear();return true;
 }catch(const std::exception&x){e=x.what();return false;}
}
bool OriginalUiInputSessionV1::resize(int w,int h,std::int32_t orientation,std::string&e){auto s=control_->state;if(!s||w<=0||h<=0){e="Required HUD session/surface unavailable";return false;}s->surface->width=w;s->surface->height=h;s->surface->orientation=orientation;return s->session.camera_update(s->camera,e);}
bool OriginalUiInputSessionV1::cursor(const ui::SwfCursor16&v,std::uint32_t i,std::string&e){auto s=control_->state;if(!s){e="Original HUD input session inactive";return false;}return s->session.cursor(v,i,e);}
bool OriginalUiInputSessionV1::input(std::int32_t mask,std::uint32_t i,std::string&e){auto s=control_->state;if(!s){e="Original HUD input session inactive";return false;}return s->session.input(mask,i,e);}
bool OriginalUiInputSessionV1::reset_focus(std::uint32_t i,std::string&e){auto s=control_->state;if(!s){e="Original HUD input session inactive";return false;}return s->session.reset_focus(i,e);}
bool OriginalUiInputSessionV1::frame(const OriginalUiFrameV1&f,std::string&e){auto s=control_->state;if(!s){e="Original HUD input session inactive";return false;}
 // This explicit application adapter sequence borrows actual current world
 // values, runs its supplied source manager batch, then original RenderFX
 // Update and display. It does not claim recovered whole Application ordering.
 if(!s->status.update(f.player_properties,f.property_count,f.player_character,e))return false;
 if(f.manager_update&&!s->session.action_script(f.manager_context,f.manager_update,e))return false;
 if(!s->session.update(f.milliseconds,f.source_advance_flag,e))return false;
 return s->session.display("_root.menu_HUD_0.HUDelements.HealthBars",e)&&s->session.display("_root.HurtCorners",e);
}
bool OriginalUiInputSessionV1::action_script(void*p,bool(*apply)(void*,ui::SwfAsGraph&,std::string&),std::string&e){auto s=control_->state;if(!s){e="Original HUD input session inactive";return false;}return s->session.action_script(p,apply,e);}
bool OriginalUiInputSessionV1::active()const noexcept{auto s=control_->state;return s&&s->session.bound()&&s->status.current();}
void OriginalUiInputSessionV1::release()noexcept{auto c=control_;++c->revision;c->state.reset();}
}
