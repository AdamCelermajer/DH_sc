#include "semantic_input.hpp"
#include <algorithm>
#include <cmath>
#include <utility>

namespace dh::foundation::platform_input {
namespace {
int button_index(Control c) {
    switch(c) {
    case Control::attack:return 0; case Control::skill1:return 1;
    case Control::skill2:return 2; case Control::skill3:return 3;
    case Control::spell:return 4; case Control::potion:return 5;
    default:return -1;
    }
}
bool same_hit(Hit a, Hit b) { return a.control == b.control && a.item == b.item; }
}
SemanticInput::SemanticInput(Bindings bindings):bindings_(bindings) {}
void SemanticInput::set_surface(Surface surface) { cancel_controls(); surface_=std::move(surface); }
std::array<bool,6> SemanticInput::held_buttons() const {
    std::array<bool,6> result{};
    if(menu_open_ || !level_enabled_)return result;
    const std::array<int,6> codes{{bindings_.attack,bindings_.skills[0],bindings_.skills[1],bindings_.skills[2],bindings_.spell,bindings_.potion}};
    for(std::size_t i=0;i<codes.size();++i) {
        auto k=keys_.find(codes[i]); auto s=suppressed_keys_.find(codes[i]);
        result[i]=k!=keys_.end() && k->second && !(s!=suppressed_keys_.end()&&s->second);
    }
    for(const auto& item:pointers_)if(!item.second.menu) {
        const int index=button_index(item.second.hit.control);
        if(index>=0)result[std::size_t(index)]=true;
    }
    return result;
}
void SemanticInput::refresh_buttons() {
    auto held=held_buttons();
    for(std::size_t i=0;i<buttons_.size();++i) {
        if(held[i]&&!buttons_[i].held)buttons_[i].pressed=true;
        if(!held[i]&&buttons_[i].held)buttons_[i].released=true;
        buttons_[i].held=held[i];
    }
}
void SemanticInput::cancel_controls() {
    pointers_.clear();
    for(const auto& key:keys_)if(key.second)suppressed_keys_[key.first]=true;
    for(auto& button:buttons_)button.pressed=false;
    pending_={}; refresh_buttons();
}
void SemanticInput::set_menu_open(bool open) {
    if(menu_open_==open)return;
    menu_open_=open; cancel_controls();
}
void SemanticInput::set_level_input_enabled(bool enabled) {
    if(level_enabled_==enabled)return;
    level_enabled_=enabled; cancel_controls();
}
void SemanticInput::lose_focus() { cancel_controls(); keys_.clear(); suppressed_keys_.clear(); refresh_buttons(); }
void SemanticInput::key(int code,bool down) {
    const bool was=keys_[code];keys_[code]=down;
    if(!down)suppressed_keys_.erase(code);
    if(down&&!was) {
        if(code==bindings_.profile)pending_.profile_pressed=true;
        if(menu_open_) { if(code==bindings_.back)pending_.menu_back=true; }
        else if(level_enabled_) {
            if(code==bindings_.back)pending_.pause_pressed=true;
            if(code==bindings_.target)pending_.actions.targetSelect=true;
            if(code==bindings_.interact)pending_.actions.interact=true;
        }
    }
    refresh_buttons();
}
void SemanticInput::pointer(std::int64_t id,PointerPhase phase,Point p) {
    if(phase==PointerPhase::cancel) {
        pointers_.erase(id);refresh_buttons();return;
    }
    if(!std::isfinite(p.x)||!std::isfinite(p.y))return;
    if(phase==PointerPhase::down) {
        if(pointers_.count(id))return;
        if(!menu_open_&&!level_enabled_)return;
        Hit hit=surface_.hit?surface_.hit(p):Hit{};
        // A modal menu never captures gameplay controls even if its host hit
        // callback accidentally returns a HUD receiver underneath the menu.
        if(menu_open_&&hit.control!=Control::menu_item&&hit.control!=Control::profile)hit={};
        pointers_.emplace(id,Capture{hit,p,p,menu_open_});refresh_buttons();return;
    }
    auto it=pointers_.find(id);if(it==pointers_.end())return;
    it->second.current=p;
    if(phase==PointerPhase::move)return;
    const auto captured=it->second;pointers_.erase(it);refresh_buttons();
    const Hit released=surface_.hit?surface_.hit(p):Hit{};
    if(!same_hit(captured.hit,released))return;
    if(captured.hit.control==Control::profile)pending_.profile_pressed=true;
    else if(captured.hit.control==Control::pause&&!menu_open_&&level_enabled_)pending_.pause_pressed=true;
    else if(captured.menu&&captured.hit.control==Control::menu_item)pending_.clicks.push_back({captured.hit,p});
    else if(!menu_open_&&level_enabled_) {
        if(captured.hit.control==Control::interact)pending_.actions.interact=true;
        if(captured.hit.control==Control::target)pending_.actions.targetSelect=true;
        if(captured.hit.control==Control::world) { pending_.target_point_requested=true;pending_.target_point=p; }
    }
}
Frame SemanticInput::take_frame(bool blocked) {
    Frame result=std::move(pending_);pending_={};
    result.attack=buttons_[0];result.spell=buttons_[4];result.potion=buttons_[5];
    for(std::size_t i=0;i<3;++i)result.skills[i]=buttons_[i+1];
    for(auto& button:buttons_) {button.pressed=false;button.released=false;}
    if(!menu_open_&&level_enabled_) {
        auto down=[&](int k) { return keys_[k]&&!suppressed_keys_[k]; };
        result.actions.move2D={float(down(bindings_.right))-float(down(bindings_.left)),float(down(bindings_.forward))-float(down(bindings_.backward))};
        for(const auto& item:pointers_)if(item.second.hit.control==Control::joystick&&surface_.joystick) {
            auto move=surface_.joystick(item.second.origin,item.second.current);
            if(std::isfinite(move.x)&&std::isfinite(move.y)) { result.actions.move2D.x+=move.x;result.actions.move2D.y+=move.y; }
        }
        const float length=std::hypot(result.actions.move2D.x,result.actions.move2D.y);
        if(length>1) {result.actions.move2D.x/=length;result.actions.move2D.y/=length;}
        const bool moving=result.actions.move2D.x!=0||result.actions.move2D.y!=0;
        result.actions.run=moving&&(bindings_.default_run?!down(bindings_.run):down(bindings_.run));result.actions.attack=result.attack.held;
    } else { result.actions={};result.target_point_requested=false; }
    if(blocked) {result.actions.move2D={};result.actions.run=false;result.actions.targetSelect=false;result.actions.interact=false;result.target_point_requested=false;}
    return result;
}
}
