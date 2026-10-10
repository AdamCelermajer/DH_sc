#include "menu_flow.hpp"
#include <algorithm>
#include <utility>
namespace dh::foundation::frontend::flow {
namespace {
bool known(std::string_view name){
    constexpr std::string_view names[]={"menu_MainMenu","menu_StartGame","menu_EnterName","menu_SelectClass","menu_Options","menu_info","menu_HelpButtons","menu_About","menu_Help","menu_confirm","menu_confirm2"};
    return std::find(std::begin(names),std::end(names),name)!=std::end(names);
}
bool fail(std::string& error,const char* message){error=message;return false;}
}
bool valid_authored_name(std::string_view raw)noexcept{
    return raw.find_first_not_of("\t\n\r ")!=std::string_view::npos;
}
const char* playable_class(unsigned index)noexcept{
    constexpr const char* names[]={"KnightPlayerBase","RoguePlayerBase","MagePlayerBase"};
    return index<3?names[index]:nullptr;
}
const char* main_menu_error_symbol(int event)noexcept{
    switch(event){
    case 1:return "MENU_MULTIPLAYER_ERROR_KICKED";
    case 2:return "MENU_MULTIPLAYER_LOGIN_GENERAL_FAILURE";
    case 3:return "MENU_MULTIPLAYER_ERROR_CONNECTION_LOST";
    case 4:return "MENU_MULTIPLAYER_LOGIN_ANOTHER_DEVICE_ERROR";
    case 5:return "MENU_MULTIPLAYER_NO_CONNECTION";
    case 6:return "MENU_MULTIPLAYER_ERROR_GAME_ROOM_GONE";
    case 8:return "MENU_MULTIPLAYER_ERROR_CONNECTION_FAILED";
    case 9:return "MENU_MULTIPLAYER_TIMEOUT";
    default:return "";
    }
}
bool Navigator::change(std::vector<std::string> next,std::string& error){
    if(services_.present_stack&&!services_.present_stack(stack_,next,error))return false;
    const bool top_changed=next.empty()||stack_.empty()||next.back()!=stack_.back();
    stack_=std::move(next);error.clear();
    if(top_changed&&!stack_.empty()&&services_.menu_entered)services_.menu_entered(stack_.back().c_str());
    return true;
}
bool Navigator::push(std::string_view name,std::string& error){
    if(!known(name))return fail(error,"Menu state is outside the recovered desktop roster");
    if(std::find(stack_.begin(),stack_.end(),name)!=stack_.end())return fail(error,"Menu state already active");
    auto next=stack_;next.emplace_back(name);return change(std::move(next),error);
}
bool Navigator::pop(std::string_view name,std::string& error){
    auto next=stack_;auto it=std::find(next.begin(),next.end(),name);
    if(it==next.end()){error.clear();return true;}
    next.erase(it);return change(std::move(next),error);
}
bool Navigator::back(std::string& error){
    if(erase_confirmation_){erase_confirmation_=false;error.clear();return true;}
    auto next=stack_;if(next.empty()){error.clear();return true;}
    next.pop_back();return change(std::move(next),error);
}
bool Navigator::pop_above(std::string_view name,std::string& error){
    auto next=stack_;auto it=std::find(next.begin(),next.end(),name);
    if(it==next.end()){error.clear();return true;}
    next.erase(it+1,next.end());return change(std::move(next),error);
}
bool Navigator::pop_all(std::string& error){return change({},error);}
bool Navigator::go_to_main_menu(int event,std::string& error){
    if(!change({"menu_MainMenu"},error))return false;
    message_=main_menu_error_symbol(event);return true;
}
bool Navigator::single_player(SlotFact fact,std::string& error){
    if(top()!="menu_MainMenu")return fail(error,"Single-player release requires menu_MainMenu");
    if(fact.id<0)return fail(error,"Selected save-slot fact is unavailable");
    if(services_.authored_menu_sound)services_.authored_menu_sound("menu_MainMenu","btn_MENU_SINGLE_PLAYER","onRelease");
    if(!push(fact.in_use?"menu_StartGame":"menu_EnterName",error))return false;
    mode_=Mode::offline_single_player;message_.clear();slot_=fact.id;selected_slot_=std::move(fact);
    if(services_.selected_profile_changed)services_.selected_profile_changed(selected_slot_);
    erase_confirmation_=false;
    if(creation_!=CreationStage::result_unmapped)creation_=CreationStage::editing;
    created_slot_=-1;
    name_.clear();class_="KnightPlayerBase";start_delivered_=false;
    return true;
}
bool Navigator::select_adjacent_slot(int direction,std::string& error){
    if(top()!="menu_MainMenu"||erase_confirmation_)return fail(error,"Profile slots can only change on the idle main menu");
    if(direction!=-1&&direction!=1)return fail(error,"Profile slot direction must be one authored arrow step");
    const int next=slot_+direction;
    if(next<0||next>3)return fail(error,"Profile slot is outside the authored four-slot range");
    if(!services_.select_slot)return fail(error,"Source selected-slot provider unavailable");
    SlotFact fact;
    if(!services_.select_slot(slot_,direction,fact,error)){if(error.empty())error="Source selected-slot provider failed";return false;}
    if(fact.id!=next||fact.save_path.empty())return fail(error,"Selected-slot provider returned a mismatched slot or no exact profile path");
    selected_slot_=std::move(fact);slot_=selected_slot_.id;
    if(services_.selected_profile_changed)services_.selected_profile_changed(selected_slot_);
    error.clear();return true;
}
bool Navigator::begin_remove_selected(std::string& error){
    if(top()!="menu_MainMenu"||erase_confirmation_)return fail(error,"Remove confirmation is unavailable");
    if(!selected_slot_.in_use||selected_slot_.id<0||selected_slot_.save_path.empty())return fail(error,"Remove requires the exact occupied selected profile");
    erase_confirmation_=true;error.clear();return true;
}
bool Navigator::resolve_remove_selected(bool accept,std::string& error){
    if(!erase_confirmation_)return fail(error,"No profile removal confirmation is active");
    if(!accept){erase_confirmation_=false;error.clear();return true;}
    if(!services_.remove_selected_slot||!services_.inspect_slot)
        return fail(error,"Recoverable selected-profile removal providers unavailable");
    erase_confirmation_=false;
    const int selected=selected_slot_.id;const auto original_path=selected_slot_.save_path;
    if(!services_.remove_selected_slot(selected_slot_,error)){erase_confirmation_=true;if(error.empty())error="Selected profile removal failed";return false;}
    SlotFact refreshed;
    if(!services_.inspect_slot(selected,refreshed,error)){if(error.empty())error="Removed profile slot could not be refreshed";return false;}
    if(refreshed.id!=selected||refreshed.in_use||refreshed.save_path.empty()||refreshed.save_path!=original_path)
        return fail(error,"Removal did not refresh the same selected slot with its canonical empty-slot path");
    selected_slot_=std::move(refreshed);slot_=selected;
    if(creation_!=CreationStage::result_unmapped)creation_=CreationStage::editing;
    created_slot_=-1;
    if(services_.selected_profile_changed)services_.selected_profile_changed(selected_slot_);
    name_.clear();class_="KnightPlayerBase";start_delivered_=false;error.clear();return true;
}
bool Navigator::accept_name(std::string raw,std::string& error){
    if(top()!="menu_EnterName")return fail(error,"Name confirmation requires menu_EnterName");
    if(!valid_authored_name(raw))return fail(error,"Authored isValidName rejected the name");
    if(!push("menu_SelectClass",error))return false;
    name_=std::move(raw);return true;
}
bool Navigator::select_class(unsigned index,std::string& error){
    if(top()!="menu_SelectClass"||creation_!=CreationStage::editing)return fail(error,"Class selection is unavailable outside editing");
    const auto* value=playable_class(index);if(!value)return fail(error,"Class index outside authored roster");
    class_=value;error.clear();return true;
}
bool Navigator::confirm_class(std::string& error){
    if(creation_==CreationStage::result_unmapped)
        return fail(error,"Previous NativeCreateSaveSlot result is outside the authored slot range; reconcile profile storage before retrying");
    if(creation_==CreationStage::complete){error.clear();return true;}
    if(creation_==CreationStage::editing){
        if(top()!="menu_SelectClass")return fail(error,"Class confirmation requires menu_SelectClass");
        if(!services_.create_save)return fail(error,"NativeCreateSaveSlot owner unavailable");
        int created=-1;if(!services_.create_save(name_,class_,created,error))return false;
        if(created<0||created>3){
            creation_=CreationStage::result_unmapped;
            return fail(error,"NativeCreateSaveSlot returned a slot outside the authored 0..3 range");
        }
        created_slot_=created;creation_=CreationStage::saved;
    }
    if(creation_==CreationStage::saved){
        if(services_.inspect_slot){
            SlotFact fact;
            if(!services_.inspect_slot(created_slot_,fact,error)){if(error.empty())error="Newly created profile slot could not be inspected";return false;}
            if(fact.id!=created_slot_||!fact.in_use)return fail(error,"Newly created profile was not visible in its exact selected slot");
            selected_slot_=std::move(fact);slot_=created_slot_;
        }else{
            selected_slot_={created_slot_,true,{}};slot_=created_slot_;
        }
        if(!services_.assign_save)return fail(error,"NativeAssignSaveSlotToPlayer owner unavailable");
        if(!services_.assign_save(created_slot_,0,error))return false;
        slot_=created_slot_;
        creation_=CreationStage::assigned;
    }
    if(creation_==CreationStage::assigned){
        if(!pop_above("menu_MainMenu",error))return false;
        creation_=CreationStage::returned_to_main;
    }
    if(!push("menu_StartGame",error))return false;
    creation_=CreationStage::complete;return true;
}
bool Navigator::start_game(int difficulty,std::string& error){
    start_delivered_=false;
    if(top()!="menu_StartGame")return fail(error,"Start requires menu_StartGame");
    if(mode_!=Mode::offline_single_player)return fail(error,"Online game service unavailable");
    if(!services_.assign_save)return fail(error,"NativeAssignSaveSlotToPlayer owner unavailable");
    if(!services_.start_game)return fail(error,"NativeStartGame owner unavailable");
    if(selected_slot_.in_use&&creation_!=CreationStage::complete&&services_.load_selected_profile&&
       !services_.load_selected_profile(selected_slot_,error)){
        if(error.empty())error="Selected existing profile Load failed";
        return false;
    }
    if(!services_.assign_save(slot_,0,error))return false;
    if(!services_.start_game(difficulty,error))return false;
    start_delivered_=true;
    error.clear();
    return true;
}
bool Navigator::multiplayer(std::string& error){
    mode_=Mode::online_unavailable;message_="MENU_MULTIPLAYER_NO_CONNECTION";
    return fail(error,"Multiplayer service unavailable; offline single-player remains supported");
}
bool Navigator::navigation_callback(std::string_view callback,const std::vector<std::string>& args,std::string& error){
    if(callback=="NativePushMenu"||callback=="PushMenu"){
        if(args.empty())return fail(error,"Malformed menu-push callback");
        return push(args.front(),error);
    }
    if(callback=="NativePopMenu"||callback=="PopMenu")return args.empty()?back(error):pop(args.front(),error);
    if(callback=="NativePopAllMenus"||callback=="PopAllMenu")return pop_all(error);
    if(callback=="NativePopAllAbove"){
        // Actual AS bridge also rejects non STRING/OBJECT arguments before this.
        if(args.size()!=1){error.clear();return true;}return pop_above(args.front(),error);
    }
    if(callback=="NativeGoToMainMenu"||callback=="GoToMainMenu")return go_to_main_menu(0,error);
    return fail(error,"Frontend callback has no recovered navigation adapter");
}
}
