#include "player_status_hud.hpp"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_sound.h"
#include <limits>

namespace dh2::ui {
PlayerStatusHud::PlayerStatusHud(SwfMovie& movie):movie_(movie){}
void PlayerStatusHud::release(){
    bound_=false;clips_={};character_=0;frames_={};failure_.clear();
    advance_=HudAdvanceOwner{};
}
bool PlayerStatusHud::bind(const char* hash,std::string& error){
    release();
    std::array<SwfHudClip,5> candidate;
    for(unsigned i=0;i<candidate.size();++i){
        const auto* relative=hud_value_clip_path(HudValueClip(i));
        const auto path=i==4?std::string(relative):std::string("_root.menu_HUD_0.")+relative;
        if(!movie_.hud_bind(path.c_str(),hash,candidate[i],error))return false;
    }
    clips_=std::move(candidate);bound_=true;error.clear();return true;
}
bool PlayerStatusHud::notify(void* context,gameswf::sprite_instance* sprite,std::string& error){
    return static_cast<PlayerStatusHud*>(context)->advance_.notify(sprite,error);
}
bool PlayerStatusHud::sound(void*,std::uintptr_t& result,std::string&){
    // Invoked by hud_play inside the exact retaining facade Scope. The genuine
    // core handler may be absent; no fabricated audio service is installed.
    result=reinterpret_cast<std::uintptr_t>(gameswf::get_sound_handler());return true;
}
bool PlayerStatusHud::pause(void*,std::uintptr_t handler,std::int32_t id,bool paused,std::string& error){
    auto* current=gameswf::get_sound_handler();
    if(!current||reinterpret_cast<std::uintptr_t>(current)!=handler){error="HUD sound handler changed during delivery";return false;}
    current->pause(id,paused);return true;
}
HudSpriteCoreServices PlayerStatusHud::sprite_services(){return {this,notify,sound,pause};}
int PlayerStatusHud::values(void* context,HudValuesState24*,const HudValueRequest32* request,HudValueResponse16* response){
    auto& self=*static_cast<PlayerStatusHud*>(context);
    const auto index=unsigned(request->index);
    if(index>=self.clips_.size()){self.failure_="Invalid source HUD clip index";return 0;}
    const auto& clip=self.clips_[index];
    switch(request->operation){
    case HudValueOperation::resolve_clip:response->clip=clip.identity();return 1;
    case HudValueOperation::is_sprite:
        if(request->clip!=clip.identity()){self.failure_="Source HUD identity differs";return 0;}
        response->value=1;return 1; // bind verified the actual type-2 sprite
    case HudValueOperation::goto_frame:
        if(request->clip!=clip.identity()||!self.movie_.hud_goto(clip,request->value,self.sprite_services(),self.failure_))return 0;
        // Original goto ignores an out-of-range frame and stops at its prior
        // frame. Preserve that history in telemetry as well as in the backend.
        if(request->value>=0&&request->value<(index<2?100:index==2?101:103))self.frames_[index]=request->value;
        return 1;
    case HudValueOperation::set_play_state:
        return request->clip==clip.identity()&&self.movie_.hud_play(clip,request->value,self.sprite_services(),self.failure_)?1:0;
    case HudValueOperation::divide_zero:
        self.failure_="Required original HUD integer zero-division handler unavailable";return 0;
    }
    self.failure_="Unknown source HUD operation";return 0;
}
bool PlayerStatusHud::update(const std::int32_t* sheet,std::size_t count,std::uintptr_t character,std::string& error){
    if(!bound_||!character||count>std::numeric_limits<std::uint32_t>::max()){
        error="Live player status owner unavailable";return false;
    }
    failure_.clear();
    HudValuesState24 state{sheet,static_cast<std::uint32_t>(count),0,reinterpret_cast<std::uintptr_t>(&movie_)};
    const HudValueServices16 services{this,values};
    const auto status=dh2_ui_hud_player_values(&state,&services);
    if(status){error=failure_.empty()?"Source player status update rejected":failure_;return false;}
    character_=character;error.clear();return true;
}
}
