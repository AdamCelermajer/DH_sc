#include "menu_flow.hpp"
namespace dh::foundation::frontend::flow {
void rebase_profile_text_paths(std::vector<SourceText>& fields,std::string_view menu){
    if(menu!="menu_MainMenu"&&menu!="menu_StartGame")return;
    constexpr std::string_view main_root="menu_MainMenu";
    constexpr std::string_view start_root="menu_StartGame";
    const auto target=menu=="menu_StartGame"?start_root:main_root;
    for(auto& field:fields){
        if(field.path.compare(0,main_root.size(),main_root)==0)field.path.replace(0,main_root.size(),target);
        else if(field.path.compare(0,start_root.size(),start_root)==0)field.path.replace(0,start_root.size(),target);
    }
}
PresentationState presentation(std::string_view name,const PresentationFacts& facts){
    PresentationState out;out.menu_name=name;
    auto hide=[&](const char* path){out.hidden_paths.emplace_back(path);};
    auto label=[&](const char* path,const char* value){out.timeline_labels.emplace_back(path,value);};
    auto text=[&](const char* path,const char* symbol){out.text.push_back({path,symbol,true});};
    if(name=="menu_MainMenu"){
        out.draw_main_scene=true;out.draw_saved_avatar=facts.selected_slot.in_use&&!facts.profile_text.empty();
        label("menu_MainMenu","show");
        hide("menu_bg.BrownBG");
        // Main onShow/selectButtonsIcons, original sprite510 SWF290038/290280.
        const char* buttons[]={"menu_MainMenu.btn_MENU_SINGLE_PLAYER","menu_MainMenu.btn_MENU_OPTIONS","menu_MainMenu.btn_MENU_MORE_GAMES","menu_MainMenu.btn_MENU_INFO"};
        for(auto* button:buttons)out.timeline_labels.emplace_back(button,"Idle");
        label("menu_MainMenu.btn_MENU_SINGLE_PLAYER.mc_icon","MenuSingle");
        label("menu_MainMenu.btn_MENU_OPTIONS.mc_icon","MenuOptions");
        label("menu_MainMenu.btn_MENU_MORE_GAMES.mc_icon","GLogo");
        label("menu_MainMenu.btn_MENU_INFO.mc_icon","MenuI");
        text("menu_MainMenu.btn_MENU_SINGLE_PLAYER.text","MENU_START");
        text("menu_MainMenu.btn_MENU_OPTIONS.text","MENU_OPTIONS");
        text("menu_MainMenu.btn_MENU_MORE_GAMES.text","MENU_MORE_GAMES");
        text("menu_MainMenu.btn_MENU_INFO.text","MENU_INFO");
        // getSlot original SWF292352: empty slot0 hides both slot arrows;
        // a later empty slot hides right only, while left stays visible.
        if(facts.erase_confirmation){
            label("menu_MainMenu.Confirmation","show");
            label("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_ACCEPT","Idle");
            label("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_REFUSE","Idle");
            text("menu_MainMenu.Confirmation.ConfirmationBox.GAMEPLAYMENUS_ERASE.text","GAMEPLAYMENUS_ERASE");
            text("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_ACCEPT.text","GAMEPLAYMENUS_ACCEPT");
            text("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_REFUSE.text","GAMEPLAYMENUS_REFUSE");
        }else{
            hide("menu_MainMenu.Confirmation.ConfirmationBox");
        }
        if(!facts.selected_slot.in_use){
            hide("menu_MainMenu.PlayerInfos");hide("menu_MainMenu.PlayerRender");
            hide("menu_MainMenu.btnDelete");
            hide("menu_MainMenu.btnLeftArrowDiff");hide("menu_MainMenu.btnRightArrowDiff");
            text("menu_MainMenu.SlotText.text","GAMEPLAYMENUS_EMPTY");
        }else{
            if(facts.profile_text.empty()){hide("menu_MainMenu.PlayerInfos");hide("menu_MainMenu.PlayerRender");}
            if(facts.difficulty<=0)hide("menu_MainMenu.btnLeftArrowDiff");
            if(facts.difficulty>=facts.unlocked_difficulty)hide("menu_MainMenu.btnRightArrowDiff");
            out.text.insert(out.text.end(),facts.profile_text.begin(),facts.profile_text.end());
        }
        if(facts.selected_slot.id<=0)hide("menu_MainMenu.btnLeftArrow");
        if(facts.selected_slot.id>=3)hide("menu_MainMenu.btnRightArrow");
    }else if(name=="menu_EnterName"){
        // Original EnterName onPush36839/onShow37010 hides RenderedBG and
        // TitleGraphic, shows BrownBG; no reconstructed renderer is evidence.
        hide("menu_bg.TitleGraphic");hide("menu_bg.RenderedBG");
        hide("menu_MainMenu");
        label("menu_EnterName.btn_back.btimg","MenuBack");
        // Generic source btn_Accept sprite69 and Shift sprite89/92 have exact
        // lowercase idle/pressed/released labels. Accept's default idle timeline
        // reaches its authored Stop4 (no highlight); Shift's released end loses
        // glow. It does not claim an AS onLoad initialized a guessed Idle label.
        constexpr const char* state_buttons[]={"menu_EnterName.buttons.btn_Accept","menu_EnterName.KeyBoard.UpperCase.btn_EnterNameShift","menu_EnterName.KeyBoard.LowerCase.btn_EnterNameShift"};
        for(unsigned i=0;i<3;++i){auto* button=state_buttons[i];label(button,facts.pressed_button_path==button?"pressed":i==0?"idle":"released");}
        hide(facts.upper_keyboard_visible?"menu_EnterName.KeyBoard.LowerCase":"menu_EnterName.KeyBoard.UpperCase");
        text("menu_EnterName.MENU_SELECT_CHAR.text","GLOBAL_ENTER_NAME");
        text("menu_EnterName.buttons.btn_Accept.Accept.text","MENU_CONFIRM");
        out.text.push_back({"menu_EnterName.buttons.btn_character_name.text",facts.entered_name,false});
    }else if(name=="menu_SelectClass"){
        // ORIGINAL MenuCharacterSelect::Show0x428f38 destroys main scene and
        // avatar camera BEFORE BaseShow, then installs CLASS_SELECTION scene.
        // These background visibility values inherit Name onPush/onHide along
        // the supported Name->Class route; class AS onShow itself is empty.
        out.draw_class_scene=true;
        hide("menu_bg.BrownBG");hide("menu_bg.RenderedBG");hide("menu_bg.TitleGraphic");
        hide("menu_MainMenu");hide("menu_EnterName");
        label("menu_SelectClass.btn_back.btimg","MenuBack");
        text("menu_SelectClass.MENU_MENUTITLE_CHOOSE_CLASS.text","MENU_MENUTITLE_CHOOSE_CLASS");
        text("menu_SelectClass.btn_Confirm.text","MENU_CONFIRM");
        constexpr const char* titles[]={"MENU_CLASS_00","MENU_CLASS_01","MENU_CLASS_02"};
        constexpr const char* descriptions[]={"MENU_KNIGHT_DESC","MENU_ROGUE_DESC","MENU_MAGE_DESC"};
        if(facts.class_index<3){
            text("menu_SelectClass.class_title.text",titles[facts.class_index]);
            text("menu_SelectClass.class_description.text",descriptions[facts.class_index]);
        }
        if(facts.class_index==0)hide("menu_SelectClass.btn_left");
        if(facts.class_index>=2)hide("menu_SelectClass.btn_right");
    }else if(name=="menu_StartGame"){
        out.draw_main_scene=true;out.draw_saved_avatar=facts.selected_slot.in_use&&!facts.profile_text.empty();
        hide("menu_bg.BrownBG");hide("menu_MainMenu");
        if(facts.profile_text.empty()){hide("menu_StartGame.PlayerInfos");hide("menu_StartGame.PlayerRender");}
        else out.text.insert(out.text.end(),facts.profile_text.begin(),facts.profile_text.end());
        label("menu_StartGame.btn_back.btimg","MenuBack");
        label("menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER.mc_icon","MenuSingle");
        label("menu_StartGame.StartMenuButtons.btn_MENU_MULTIPLAYER.mc_icon","MenuMulti");
        label("menu_StartGame.StartMenuButtons.btn_MENU_Leader.mc_icon","GLogo");
        label("menu_StartGame.StartMenuButtons.btn_Achievements.mc_icon","MenuG");
        // root/sprite471 onShow localizes all four source button receivers.
        text("menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER.text","MENU_SINGLE_PLAYER");
        text("menu_StartGame.StartMenuButtons.btn_MENU_MULTIPLAYER.text","MENU_MULTIPLAYER");
        text("menu_StartGame.StartMenuButtons.btn_Achievements.text","MENU_ACHIEVEMENTS");
        text("menu_StartGame.StartMenuButtons.btn_MENU_Leader.text","MENU_LEADERBOARDS");
    }
    return out;
}
}
