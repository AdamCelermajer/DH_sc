#include "menu_flow.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace dh::foundation::frontend::flow;
namespace {int checks=0;void require(bool value,const char* why){++checks;if(!value)throw std::runtime_error(why);}}
int main(){try{
    std::string error;
    require(!valid_authored_name(""),"empty rejected");
    require(!valid_authored_name(" \t\r\n"),"authored whitespace rejected");
    require(valid_authored_name("!"),"punctuation must remain source valid");
    require(valid_authored_name("\v"),"non-authored whitespace must remain source valid");
    Navigator absent;
    require(!absent.single_player({-1,false},error),"missing slot fact must fail");
    require(absent.single_player({2,false},error)&&absent.top()=="menu_EnterName","empty slot name first");
    require(!absent.accept_name("  ",error)&&absent.top()=="menu_EnterName","invalid name keeps screen");
    require(absent.accept_name(" Ada ",error)&&absent.top()=="menu_SelectClass","accept name then class");
    require(absent.player_name()==" Ada ","source preserves raw name");
    require(absent.select_class(2,error)&&absent.player_class()=="MagePlayerBase","select Mage");
    require(!absent.select_class(3,error),"out-of-roster class rejected");
    require(!absent.confirm_class(error)&&error=="NativeCreateSaveSlot owner unavailable","no invented create success");
    require(absent.creation_stage()==CreationStage::editing&&absent.current_slot()==2,"absent create keeps state");
    Navigator unavailable_start;
    require(unavailable_start.single_player({0,true},error),"existing profile can navigate without local load owner");
    require(!unavailable_start.start_game(0,error)&&error=="NativeAssignSaveSlotToPlayer owner unavailable","existing launch requires actual assignment owner");
    int creates=0,assigns=0,starts=0;bool reject_assignment=true,reject_start=true,reject_main=false;
    std::vector<std::string> operations;
    Services services;
    services.create_save=[&](const std::string& name,const std::string& cls,int& slot,std::string& e){
        require(name=="Hero"&&cls=="RoguePlayerBase","create receives authored raw name/class");
        ++creates;operations.push_back("create");slot=3;e.clear();return true;};
    services.assign_save=[&](int slot,int player,std::string& e){
        require(slot==3&&player==0,"assign returned slot to local player");++assigns;operations.push_back("assign");
        if(reject_assignment){e="save owner failure";return false;}e.clear();return true;};
    services.inspect_slot=[](int slot,SlotFact& fact,std::string& e){if(slot!=3){e="unexpected created slot";return false;}fact={3,true,"created-slot-3.sav"};e.clear();return true;};
    services.start_game=[&](int difficulty,std::string& e){++starts;require(difficulty==1,"difficulty is forwarded");
        if(reject_start){e="load owner failure";return false;}e.clear();return true;};
    services.present_stack=[&](const auto&,const auto& next,std::string& e){
        if(reject_main&&next==std::vector<std::string>{"menu_MainMenu"}){e="visual lifecycle failure";return false;}
        e.clear();return true;};
    Navigator flow(services);
    require(flow.single_player({1,false},error)&&flow.accept_name("Hero",error)&&flow.select_class(1,error),"authored create route");
    require(!flow.confirm_class(error)&&creates==1&&assigns==1,"assignment failure after real save");
    require(flow.creation_stage()==CreationStage::saved&&flow.current_slot()==3&&flow.selected_slot_fact().save_path=="created-slot-3.sav","created slot fact/path is refreshed before assignment");
    reject_assignment=false;reject_main=true;
    require(!flow.confirm_class(error)&&creates==1&&assigns==2&&flow.top()=="menu_SelectClass","visual failure retains class and skips create");
    require(flow.creation_stage()==CreationStage::assigned,"assignment prefix retained");
    reject_main=false;
    require(flow.confirm_class(error)&&flow.top()=="menu_StartGame","offline continuation returns through main to StartGame");
    require(flow.creation_stage()==CreationStage::complete&&!flow.start_delivered()&&starts==0,
            "Confirm completes menu creation but does not deliver StartGame");
    require(flow.stack()==std::vector<std::string>{"menu_MainMenu","menu_StartGame"},"name/class states removed");
    require(flow.confirm_class(error)&&creates==1&&assigns==2,"completed confirm idempotent");
    require(!flow.start_game(1,error)&&starts==1&&flow.top()=="menu_StartGame"&&!flow.start_delivered(),
            "failed load is reported, not gameplay");
    reject_start=false;require(flow.start_game(1,error)&&starts==2&&flow.start_delivered(),
                               "real start owner accepted and delivered");
    reject_assignment=true;
    require(!flow.start_game(1,error)&&starts==2&&!flow.start_delivered(),"assignment failure blocks level start");
    reject_assignment=false;
    require(flow.back(error)&&flow.top()=="menu_MainMenu","submenu Back");
    require(flow.single_player({3,true},error)&&flow.top()=="menu_StartGame","occupied slot skips creation");
    require(!flow.multiplayer(error)&&flow.message_symbol()=="MENU_MULTIPLAYER_NO_CONNECTION","online unavailable explicit");
    require(!flow.start_game(1,error)&&starts==2,"unavailable online does not reach local start");
    Services savedServices;
    savedServices.assign_save=[](int slot,int player,std::string& e){e.clear();return slot==3&&player==0;};
    savedServices.start_game=[](int difficulty,std::string& e){e.clear();return difficulty==1;};
    Navigator saved(savedServices);
    require(saved.single_player({3,true},error)&&saved.mode()==Mode::offline_single_player,
            "saved-slot route starts without a creation-stage setup");
    require(saved.creation_stage()==CreationStage::editing&&!saved.start_delivered(),
            "saved-slot Start is independent of creation completion state");
    require(saved.start_game(1,error)&&saved.start_delivered(),"saved-slot Start delivers through real owners");
    require(flow.back(error)&&flow.single_player({3,true},error)&&flow.mode()==Mode::offline_single_player,
            "singleplayer resets explicit mode");
    require(flow.go_to_main_menu(9,error)&&flow.message_symbol()=="MENU_MULTIPLAYER_TIMEOUT","source error event mapping");
    require(flow.navigation_callback("NativePushMenu",{"menu_Options","ignored"},error)&&flow.top()=="menu_Options","push ignores extra args");
    require(flow.navigation_callback("NativePushMenu",{"menu_info"},error),"second overlay");
    require(flow.navigation_callback("NativePopMenu",{"menu_Options"},error)&&flow.top()=="menu_info","named-pop removes only target");
    require(flow.navigation_callback("NativePopAllAbove",{"menu_MainMenu"},error)&&flow.top()=="menu_MainMenu","pop above preserves target");
    require(flow.navigation_callback("NativePopAllAbove",{},error),"wrong pop above arity source no-op");
    require(!flow.navigation_callback("NativePushMenu",{},error),"malformed unguarded original access rejected");
    require(!flow.navigation_callback("NativePushMenu",{"invented"},error),"unknown state rejected");
    require(!flow.navigation_callback("NativeLogin",{},error),"missing online callback never success");
    require(flow.navigation_callback("NativePopAllMenus",{},error)&&flow.stack().empty(),"pop all native route");
    require(flow.navigation_callback("NativeGoToMainMenu",{},error)&&flow.top()=="menu_MainMenu","go main native route");
    PresentationFacts facts;facts.selected_slot={0,false};
    auto main=presentation("menu_MainMenu",facts);
    auto hidden=[](const auto& p,const char* path){return std::find(p.hidden_paths.begin(),p.hidden_paths.end(),path)!=p.hidden_paths.end();};
    require(main.draw_main_scene&&!main.draw_saved_avatar,"empty profile does not invent an avatar");
    require(hidden(main,"menu_MainMenu.PlayerInfos")&&hidden(main,"menu_MainMenu.btnDelete"),"empty profile visibility");
    require(hidden(main,"menu_MainMenu.btnLeftArrow")&&!hidden(main,"menu_MainMenu.btnRightArrow"),"first empty slot can advance to the next source slot");
    require(std::find(main.timeline_labels.begin(),main.timeline_labels.end(),std::pair<std::string,std::string>{"menu_MainMenu.btn_MENU_OPTIONS.mc_icon","MenuOptions"})!=main.timeline_labels.end(),"original icon label selection");
    facts.selected_slot={1,false};main=presentation("menu_MainMenu",facts);
    require(!hidden(main,"menu_MainMenu.btnLeftArrow")&&!hidden(main,"menu_MainMenu.btnRightArrow"),"later empty slots remain reachable in either direction");
    facts.selected_slot={3,true};facts.profile_text.push_back({"menu_MainMenu.SlotText.text","ActualOwnerName",false});
    main=presentation("menu_MainMenu",facts);
    require(main.draw_saved_avatar&&!hidden(main,"menu_MainMenu.PlayerInfos")&&hidden(main,"menu_MainMenu.btnRightArrow"),"occupied source profile branch");
    require(main.text.back().value=="ActualOwnerName","actual profile text loan copied");
    std::vector<SourceText> start_fields{{"menu_MainMenu/SlotText/text","Ada",false},{"menu_MainMenu/PlayerInfos/Hud_Level/text","LEVEL 4",false}};
    rebase_profile_text_paths(start_fields,"menu_StartGame");
    require(start_fields[0].path=="menu_StartGame/SlotText/text"&&start_fields[1].path=="menu_StartGame/PlayerInfos/Hud_Level/text"&&start_fields[1].value=="LEVEL 4","exact save projection rebases onto StartGame's authored text receivers");
    facts.profile_text=start_fields;auto start_view=presentation("menu_StartGame",facts);
    require(start_view.draw_saved_avatar&&start_view.text[0].value=="Ada"&&start_view.text[1].value=="LEVEL 4","StartGame receives actual selected-save projection instead of SWF placeholders");
    require(std::any_of(start_view.text.begin(),start_view.text.end(),[](const auto& t){return t.path=="menu_StartGame.StartMenuButtons.btn_Achievements.text"&&t.value=="MENU_ACHIEVEMENTS"&&t.localization_symbol;}),"base StartGame third-row receiver gets its original achievements symbol");
    require(std::any_of(start_view.text.begin(),start_view.text.end(),[](const auto& t){return t.path=="menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER.text"&&t.value=="MENU_SINGLE_PLAYER"&&t.localization_symbol;}),"StartGame localizes single-player button through its source receiver");
    require(std::any_of(start_view.text.begin(),start_view.text.end(),[](const auto& t){return t.path=="menu_StartGame.StartMenuButtons.btn_MENU_MULTIPLAYER.text"&&t.value=="MENU_MULTIPLAYER"&&t.localization_symbol;}),"StartGame localizes multiplayer button through its source receiver");
    require(std::any_of(start_view.text.begin(),start_view.text.end(),[](const auto& t){return t.path=="menu_StartGame.StartMenuButtons.btn_MENU_Leader.text"&&t.value=="MENU_LEADERBOARDS"&&t.localization_symbol;}),"StartGame writes the SWF onShow Leaderboards string symbol");
    facts.erase_confirmation=true;main=presentation("menu_MainMenu",facts);
    require(std::any_of(main.text.begin(),main.text.end(),[](const auto& t){return t.path=="menu_MainMenu.Confirmation.ConfirmationBox.GAMEPLAYMENUS_ERASE.text"&&t.value=="GAMEPLAYMENUS_ERASE"&&t.localization_symbol;}),"explicit erase confirmation projects source prompt");
    auto name=presentation("menu_EnterName",facts);
    require(hidden(name,"menu_bg.RenderedBG")&&hidden(name,"menu_bg.TitleGraphic")&&!name.draw_main_scene,"name brown background owns screen");
    require(hidden(name,"menu_EnterName.KeyBoard.LowerCase"),"reference active uppercase keyboard");
    require(std::find(name.timeline_labels.begin(),name.timeline_labels.end(),std::pair<std::string,std::string>{"menu_EnterName.buttons.btn_Accept","idle"})!=name.timeline_labels.end(),"source idle timeline uses actual lowercase label");
    facts.pressed_button_path="menu_EnterName.buttons.btn_Accept";name=presentation("menu_EnterName",facts);
    require(std::find(name.timeline_labels.begin(),name.timeline_labels.end(),std::pair<std::string,std::string>{facts.pressed_button_path,"pressed"})!=name.timeline_labels.end(),"source captured press projects pressed label");
    facts.upper_keyboard_visible=false;name=presentation("menu_EnterName",facts);
    require(hidden(name,"menu_EnterName.KeyBoard.UpperCase")&&!hidden(name,"menu_EnterName.KeyBoard.LowerCase"),"keyboard shifted active layer");
    facts.class_index=0;auto cls=presentation("menu_SelectClass",facts);
    require(cls.text[2].value=="MENU_CLASS_00"&&cls.text[3].value=="MENU_KNIGHT_DESC","Knight selection uses its own source labels");
    facts.class_index=1;cls=presentation("menu_SelectClass",facts);
    require(cls.text[2].value=="MENU_CLASS_01"&&cls.text[3].value=="MENU_ROGUE_DESC","Rogue selection does not retain Warrior labels");
    facts.class_index=2;cls=presentation("menu_SelectClass",facts);
    require(cls.draw_class_scene&&hidden(cls,"menu_SelectClass.btn_right")&&!hidden(cls,"menu_SelectClass.btn_left"),"native class-boundary arrows no wrap");
    require(cls.text.back().value=="MENU_MAGE_DESC","class projection uses selected localization symbol");
    int selected=1,removed=0;bool occupied=true;std::filesystem::path current="slot-1.sav";
    Services slots;slots.select_slot=[&](int from,int direction,SlotFact& out,std::string&){selected=from+direction;out={selected,selected==1,current};return true;};
    slots.inspect_slot=[&](int id,SlotFact& out,std::string&){out={id,occupied,id==selected?current:std::filesystem::path("slot-"+std::to_string(id)+".sav")};return true;};
    slots.remove_selected_slot=[&](const SlotFact& selected_fact,std::string&){require(selected_fact.id==1&&selected_fact.save_path==current,"remove receives the exact selected path");++removed;occupied=false;return true;};
    Navigator indexed(slots);indexed.set_selected_slot({0,false,"slot-0.sav"});
    require(indexed.select_adjacent_slot(1,error)&&indexed.current_slot()==1&&indexed.selected_slot_fact().save_path=="slot-1.sav","source slot arrow returns exact selected path");
    require(indexed.begin_remove_selected(error)&&indexed.erase_confirmation(),"occupied profile asks before removal");
    require(indexed.resolve_remove_selected(false,error)&&removed==0&&indexed.selected_slot_fact().in_use,"refusal preserves profile");
    require(indexed.begin_remove_selected(error)&&indexed.resolve_remove_selected(true,error)&&removed==1&&!indexed.selected_slot_fact().in_use&&indexed.selected_slot_fact().save_path=="slot-1.sav","accept uses recoverable host action then retains the same canonical slot path");
    require(indexed.single_player(indexed.selected_slot_fact(),error)&&indexed.top()=="menu_EnterName"&&indexed.player_name().empty()&&indexed.player_class()=="KnightPlayerBase"&&indexed.creation_stage()==CreationStage::editing,"create-again starts fresh name/class state");
    Navigator bounds;bounds.set_selected_slot({3,false,"slot-3.sav"});
    require(!bounds.select_adjacent_slot(1,error)&&bounds.current_slot()==3,"fourth slot cannot advance past source bound");
    std::cout<<"PASS menu_flow: "<<checks<<" assertions\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
