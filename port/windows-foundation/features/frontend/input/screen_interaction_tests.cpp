#include "screen_interaction.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::frontend;
void check(bool value,const char* message) {if(!value)throw std::runtime_error(message);}
int main(){try {
    std::vector<std::string> calls;
    flow::Services services;
    services.create_save=[&](const auto& name,const auto& klass,int& slot,std::string&){calls.push_back(name+":"+klass);slot=2;return true;};
    services.assign_save=[&](int,int,std::string&){calls.push_back("assign");return true;};
    services.start_game=[&](int difficulty,std::string&){calls.push_back("start:"+std::to_string(difficulty));return true;};
    flow::Navigator nav(services);input::ScreenInteraction ui(nav);std::string error;
    ui.selected_slot({2,false});
    check(ui.dispatch("menu_MainMenu.btn_MENU_SINGLE_PLAYER",error),"source singleplayer route");
    check(nav.top()=="menu_EnterName","name screen not pushed");
    ui.set_surface({{"menu_EnterName/KeyBoard/UpperCase/btn_EnterNameQ",true}},[](input::Point)->input::ItemId{return 1;});
    ui.pointer(4,input::PointerPhase::down,{0,0});check(ui.flush(error)&&ui.name().empty(),"touch key fired before source onRelease");
    ui.pointer(4,input::PointerPhase::up,{0,0});check(ui.flush(error),"original Q callback/path separators");
    check(ui.name()=="Q","original uppercase keyvalue");
    check(ui.dispatch("menu_EnterName.KeyBoard.UpperCase.btn_EnterNameShift",error)&&ui.uppercase_visible(),"source first shift quirk");
    check(ui.dispatch("menu_EnterName.KeyBoard.UpperCase.btn_EnterNameShift",error)&&!ui.uppercase_visible(),"source shift layer toggle");
    check(!ui.dispatch("menu_EnterName.KeyBoard.UpperCase.btn_EnterNameW",error),"hidden uppercase accepted");
    check(ui.dispatch("menu_EnterName.KeyBoard.LowerCase.btn_EnterNameW",error)&&ui.name()=="Qw","lowercase keyvalue");
    check(ui.dispatch("menu_EnterName.KeyBoard.LowerCase.btn_EnterNameSpace",error),"space callback");
    check(ui.dispatch("menu_EnterName.KeyBoard.LowerCase.btn_EnterNameDel",error)&&ui.name()=="Qw","delete callback");
    check(ui.text("123456789")&&ui.name()=="Qw123456","original isFullString cap");
    check(ui.dispatch("menu_EnterName.buttons.btn_Accept",error)&&nav.top()=="menu_SelectClass","name Confirm route");
    ui.set_animation_input_enabled(false);
    check(!ui.enabled("menu_SelectClass.btn_Confirm")&&!ui.dispatch("menu_SelectClass.btn_right",error),"class animation gate missing");
    check(ui.enabled("menu_SelectClass.btn_back"),"class transition incorrectly blocked Back");
    ui.key(0x27,true);ui.key(0x27,false);check(ui.flush(error)&&ui.class_index()==0,"blocked PC class arrow admitted");
    ui.set_animation_input_enabled(true);
    ui.key(0x25,true);ui.key(0x25,false);check(ui.flush(error)&&ui.class_index()==0,"left bound wraps");
    ui.key(0x27,true);ui.key(0x27,false);check(ui.flush(error)&&ui.class_index()==1,"PC right original class switch");
    ui.key(0x27,true);ui.key(0x27,false);check(ui.flush(error)&&ui.class_index()==2,"class2 missing");
    check(!ui.enabled("menu_SelectClass.btn_right"),"right bound visible");
    // Several releases batched into one frame apply in order against live state:
    // Left,Right,Right from index 2 must end at 2 (the second Right is bound).
    ui.key(0x25,true);ui.key(0x25,false);ui.key(0x27,true);ui.key(0x27,false);ui.key(0x27,true);ui.key(0x27,false);
    check(ui.flush(error)&&ui.class_index()==2,"same-frame Left,Right,Right dropped a press");
    // Left x3 from 2 stops at the left bound without a source error.
    ui.key(0x25,true);ui.key(0x25,false);ui.key(0x25,true);ui.key(0x25,false);ui.key(0x25,true);ui.key(0x25,false);
    check(ui.flush(error)&&ui.class_index()==0,"same-frame Left x3 did not stop at the left bound");
    ui.key(0x27,true);ui.key(0x27,false);ui.key(0x27,true);ui.key(0x27,false);
    check(ui.flush(error)&&ui.class_index()==2,"same-frame Right x2 from the left bound failed");
    check(ui.dispatch("menu_SelectClass.btn_Confirm",error)&&nav.top()=="menu_StartGame","class Confirm source flow");
    check(calls.size()==2&&calls[0].find("Qw123456:")==0,"rawname/create/assign ownership");
    check(!ui.dispatch("menu_StartGame/StartMenuButtons/btn_MENU_SINGLE_PLAYER",error),"missing difficulty fact succeeded");
    ui.selected_difficulty(0);
    check(ui.dispatch("menu_StartGame/StartMenuButtons/btn_MENU_SINGLE_PLAYER",error)&&calls.size()==4&&calls[3]=="start:0","source assign/start callback order");
    flow::Navigator missing;input::ScreenInteraction blocked(missing);blocked.selected_slot({0,false});
    check(blocked.dispatch("menu_MainMenu.btn_MENU_SINGLE_PLAYER",error)&&blocked.text("A"),"missing owner setup");
    check(blocked.dispatch("menu_EnterName.buttons.btn_Accept",error),"missing owner name route");
    blocked.key(13,true);blocked.key(13,false);check(!blocked.flush(error)&&!error.empty(),"PC Confirm swallowed unavailable service");
    input::ScreenInteraction gated(missing);
    gated.set_surface({{"menu_SelectClass/btn_back",true},{"menu_SelectClass/btn_Confirm",true}},[](input::Point p)->input::ItemId{return p.x<1?1:2;});
    gated.pointer(3,input::PointerPhase::down,{2,0});gated.set_animation_input_enabled(false);
    gated.pointer(3,input::PointerPhase::up,{2,0});check(gated.flush(error)&&missing.top()=="menu_SelectClass","gate-down failed to cancel Confirm capture");
    gated.pointer(4,input::PointerPhase::down,{0,0});gated.set_animation_input_enabled(true);
    gated.pointer(4,input::PointerPhase::up,{0,0});check(gated.flush(error)&&missing.top()=="menu_EnterName","gate edge discarded source Back capture");
    int slot=0,removed=0;bool occupied=true;std::filesystem::path profile="slot-1.sav";
    flow::Services slot_services;
    slot_services.select_slot=[&](int from,int direction,flow::SlotFact& out,std::string&){slot=from+direction;out={slot,occupied,profile};return true;};
    slot_services.remove_selected_slot=[&](const flow::SlotFact& selected,std::string&){check(selected.id==1&&selected.save_path==profile,"remove receives current exact profile path");++removed;occupied=false;return true;};
    slot_services.inspect_slot=[&](int id,flow::SlotFact& out,std::string&){out={id,occupied,profile};return true;};
    flow::Navigator slot_nav(slot_services);input::ScreenInteraction slots(slot_nav);slots.selected_slot({0,false,"slot-0.sav"});
    check(slots.enabled("menu_MainMenu.btnRightArrow")&&slots.dispatch("menu_MainMenu.btnRightArrow",error)&&slot_nav.current_slot()==1,"real Main right-arrow selects exact next slot");
    check(slots.enabled("menu_MainMenu.btnDelete")&&slots.dispatch("menu_MainMenu.btnDelete",error)&&slot_nav.erase_confirmation(),"Delete opens inline confirmation only");
    check(slots.enabled("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_REFUSE")&&slots.dispatch("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_REFUSE",error)&&removed==0&&!slot_nav.erase_confirmation(),"Refuse closes confirmation without touching profile");
    check(slots.dispatch("menu_MainMenu.btnDelete",error)&&slots.dispatch("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_ACCEPT",error)&&removed==1&&!slot_nav.selected_slot_fact().in_use&&slot_nav.selected_slot_fact().save_path=="slot-1.sav","Accept performs host removal then preserves the selected slot's canonical path");
    flow::Services repeat_services;
    repeat_services.create_save=[](const std::string&,const std::string&,int& created,std::string&){created=0;return true;};
    repeat_services.assign_save=[](int,int,std::string&){return true;};
    repeat_services.start_game=[](int,std::string&){return true;};
    repeat_services.inspect_slot=[](int id,flow::SlotFact& fact,std::string&){fact={id,true,"created-0.sav"};return true;};
    flow::Navigator repeat_nav(repeat_services);input::ScreenInteraction repeat(repeat_nav);repeat.selected_slot({0,false,"empty-0.sav"});
    check(repeat.dispatch("menu_MainMenu.btn_MENU_SINGLE_PLAYER",error)&&repeat.text("First")&&repeat.dispatch("menu_EnterName.buttons.btn_Accept",error),"first create reaches class selection");
    check(repeat.dispatch("menu_SelectClass.btn_right",error)&&repeat.class_index()==1&&repeat_nav.player_class()=="RoguePlayerBase","first run selects actual Rogue row");
    check(repeat.dispatch("menu_SelectClass.btn_back",error)&&repeat.dispatch("menu_EnterName.btn_back",error)&&repeat_nav.top()=="menu_MainMenu","leave first creation without retaining its input screen");
    check(repeat.dispatch("menu_MainMenu.btn_MENU_SINGLE_PLAYER",error)&&repeat.text("")&&repeat.name().empty()&&repeat.text("Second")&&repeat.dispatch("menu_EnterName.buttons.btn_Accept",error),"second create gets a fresh name input buffer");
    repeat.key(0,false);
    check(repeat.class_index()==0&&repeat_nav.player_name()=="Second"&&repeat_nav.player_class()=="KnightPlayerBase","second creation restores source default class/name projection");
    int double_create=0;
    flow::Services double_services;
    double_services.create_save=[&](const std::string&,const std::string&,int& created,std::string&){++double_create;created=0;return true;};
    double_services.assign_save=[](int,int,std::string&){return true;};
    flow::Navigator double_nav(double_services);input::ScreenInteraction double_ui(double_nav);double_ui.selected_slot({0,false,"empty-0.sav"});
    check(double_ui.dispatch("menu_MainMenu.btn_MENU_SINGLE_PLAYER",error)&&double_ui.text("Double")&&double_ui.dispatch("menu_EnterName.buttons.btn_Accept",error),"double-confirm setup reaches class selection");
    double_ui.set_surface({{"menu_SelectClass/btn_Confirm",true}},[](input::Point)->input::ItemId{return 1;});
    for(int tap=0;tap<2;++tap){double_ui.pointer(8,input::PointerPhase::down,{4,4});double_ui.pointer(8,input::PointerPhase::up,{4,4});}
    check(double_ui.flush(error)&&double_nav.top()=="menu_StartGame"&&double_create==1,"two source releases in one event batch create once and leave a valid StartGame state");
    std::cout<<"PASS original frontend paths: QWERTY/shift/space/delete/cap/nameaccept/classbounds/Confirm/service failure\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
