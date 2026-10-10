#include "screen_interaction.hpp"
#include <utility>

namespace dh::foundation::frontend::input {
namespace {
std::string normalize(std::string_view path) {std::string result(path);for(auto& c:result)if(c=='/')c='.';return result;}
std::string_view leaf(std::string_view path) { auto pos=path.rfind('.');return pos==path.npos?path:path.substr(pos+1); }
bool prefix(std::string_view value,std::string_view start) {return value.substr(0,start.size())==start;}
bool fail(std::string& error,const char* message) {error=message;return false;}
}
void ScreenInteraction::set_animation_input_enabled(bool enabled) {
    if(animation_input_enabled_==enabled)return;
    animation_input_enabled_=enabled;
    // Update admission on a gate edge without replacing geometry. Disabled
    // class captures cancel; Back captures survive the transition gate edge.
    if(navigator_.top()=="menu_SelectClass") {
        std::vector<Item> items;
        for(std::size_t i=0;i<paths_.size();++i)items.push_back({i+1,paths_[i].enabled&&this->enabled(paths_[i].path)});
        input_.update_items(std::move(items));
        if(!enabled)pending_paths_.clear();
    }
}
void ScreenInteraction::sync() {
    if(screen_==navigator_.top()) return;
    auto previous=screen_;screen_=navigator_.top();keys_.clear();input_.lose_focus();pending_paths_.clear();
    if(screen_=="menu_EnterName") {
        input_.begin_name(previous=="menu_SelectClass"?input_.name():std::string{},100);
        if(previous!="menu_SelectClass"){caps_=false;upper_visible_=true;}
    }
    else input_.end_name();
    if(screen_=="menu_SelectClass") {
        for(unsigned i=0;i<3;++i) if(navigator_.player_class()==flow::playable_class(i)) class_index_=i;
    }
}
bool ScreenInteraction::enabled(std::string_view path) const {
    const auto normalized=normalize(path);path=normalized;
    const auto top=navigator_.top();
    if(!prefix(path,top)||path.size()<=top.size()||path[top.size()]!='.')return false;
    const auto button=leaf(path);
    if(top=="menu_EnterName") {
        if(prefix(path,"menu_EnterName.KeyBoard.UpperCase.")&&!upper_visible_)return false;
        if(prefix(path,"menu_EnterName.KeyBoard.LowerCase.")&&upper_visible_)return false;
        return button=="btn_back"||button=="btn_Accept"||prefix(button,"btn_EnterName");
    }
    if(top=="menu_SelectClass")return button=="btn_back"||(animation_input_enabled_&&(button=="btn_Confirm"||(button=="btn_left"&&class_index_>0)||(button=="btn_right"&&class_index_<2)));
    if(top=="menu_MainMenu") {
        if(navigator_.erase_confirmation())
            return button=="btn_GAMEPLAYMENUS_ACCEPT"||button=="btn_GAMEPLAYMENUS_REFUSE";
        if(button=="btn_MENU_SINGLE_PLAYER")return true;
        if(button=="btnLeftArrow")return navigator_.current_slot()>0;
        if(button=="btnRightArrow")return navigator_.current_slot()>=0&&navigator_.current_slot()<3;
        if(button=="btnDelete")return navigator_.selected_slot_fact().in_use;
        return false;
    }
    if(top=="menu_StartGame")return button=="btn_back"||path=="menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER";
    return false;
}
void ScreenInteraction::set_surface(std::vector<PathItem> paths,std::function<ItemId(Point)> hit) {
    sync();paths_=std::move(paths);std::vector<Item> items;
    for(std::size_t i=0;i<paths_.size();++i)items.push_back({i+1,paths_[i].enabled&&enabled(paths_[i].path)});
    input_.set_surface(std::move(items),std::move(hit));
}
bool ScreenInteraction::append(std::string_view bytes) {
    // isFullString tests before a source character append. Host text batches
    // are expanded into their original byte keyboard characters in order.
    for(char byte:bytes) {if(input_.name().size()>=8)break; if(!input_.text(std::string_view(&byte,1)))return false;}
    return true;
}
bool ScreenInteraction::text(std::string_view bytes) {sync();return screen_=="menu_EnterName"&&append(bytes);}
void ScreenInteraction::key(int vk,bool down,bool shift) {
    sync();bool prior=keys_[vk];keys_[vk]=down;if(prior==down)return;
    if(screen_=="menu_SelectClass"&&(vk==0x25||vk==0x27||vk==13)) {
        // Queue every release; the gate is evaluated in flush() against the
        // state left by earlier presses in the same frame (see flush).
        if(!down){const char* path=vk==0x25?"menu_SelectClass.btn_left":vk==0x27?"menu_SelectClass.btn_right":"menu_SelectClass.btn_Confirm";pending_paths_.emplace_back(path);}return;
    }
    input_.key(vk,down,shift);
}
bool ScreenInteraction::dispatch(std::string_view path,std::string& error) {
    const auto normalized=normalize(path);path=normalized;
    sync();if(!enabled(path))return fail(error,"Source button is not active on the current menu");
    const auto button=leaf(path);
    if(button=="btn_back")return navigator_.back(error);
    if(screen_=="menu_MainMenu") {
        if(navigator_.erase_confirmation())
            return navigator_.resolve_remove_selected(button=="btn_GAMEPLAYMENUS_ACCEPT",error);
        if(button=="btn_MENU_SINGLE_PLAYER")return navigator_.single_player(navigator_.selected_slot_fact(),error);
        if(button=="btnLeftArrow")return navigator_.select_adjacent_slot(-1,error);
        if(button=="btnRightArrow")return navigator_.select_adjacent_slot(1,error);
        if(button=="btnDelete")return navigator_.begin_remove_selected(error);
    }
    if(screen_=="menu_StartGame") {
        if(difficulty_<0)return fail(error,"Selected source difficulty fact is unavailable");
        return navigator_.start_game(difficulty_,error);
    }
    if(screen_=="menu_SelectClass") {
        if(button=="btn_Confirm")return navigator_.confirm_class(error);
        auto next=button=="btn_left"?class_index_-1:class_index_+1;
        if(!navigator_.select_class(next,error))return false;
        class_index_=next;error.clear();return true;
    }
    if(button=="btn_Accept")return navigator_.accept_name(input_.name(),error);
    if(button=="btn_EnterNameShift") {caps_=!caps_;upper_visible_=caps_;error.clear();return true;}
    if(button=="btn_EnterNameDel") {input_.key(8,true);input_.key(8,false);error.clear();return true;}
    if(button=="btn_EnterNameSpace") {append(" ");error.clear();return true;}
    constexpr std::string_view key_prefix="btn_EnterName";
    if(button.size()!=key_prefix.size()+1)return fail(error,"No recovered keyboard callback for this path");
    char byte=button.back();
    if(byte>='A'&&byte<='Z'&&prefix(path,"menu_EnterName.KeyBoard.LowerCase."))byte=char(byte-'A'+'a');
    if(!((byte>='a'&&byte<='z')||(byte>='A'&&byte<='Z')||(byte>='0'&&byte<='9')))return fail(error,"No recovered keyboard key value");
    append(std::string_view(&byte,1));error.clear();return true;
}
bool ScreenInteraction::flush(std::string& error) {
    sync();auto frame=input_.take_frame();
    auto pc_paths=std::move(pending_paths_);pending_paths_.clear();
    // Releases are applied in order against live state: a press that is no
    // longer enabled (for example a bound arrow after an earlier press in this
    // frame) is ignored, as a source press on a disabled control would be.
    for(const auto& path:pc_paths) {if(!enabled(path))continue;if(!dispatch(path,error))return false;if(screen_!=navigator_.top())return true;}
    if(frame.back)return navigator_.back(error);
    if(frame.text_committed)return navigator_.accept_name(input_.name(),error);
    for(auto id:frame.activated) {
        if(!id||id>paths_.size())return fail(error,"Authored hit ID has no current path");
        if(!dispatch(paths_[id-1].path,error))return false;
        // A transition or keyboard visibility change requires new host geometry
        // before admitting any remaining clicks captured on the old screen.
        if(screen_!=navigator_.top())break;
    }
    error.clear();return true;
}
}
