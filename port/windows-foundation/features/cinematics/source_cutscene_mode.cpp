#include "source_cutscene_mode.hpp"
namespace dh::foundation {namespace {
bool missing(const char* service,std::string& e){if(e.empty())e=std::string("Required original cutscene ")+service;return false;}
}
bool SourceCutsceneMode::command(bool enter,bool,int,std::string& e){
    e.clear();const auto app=providers_.application.lock();if(!app)return missing("SAME Application",e);
    const auto pm=app->source_player_manager_v59();if(!pm||!pm->manager()||!pm->belongs_to_application(app))return missing("published SAME PlayerManager",e);
    using Player=dh2::player::PlayerInfoFieldsV1;
    auto local=[&](bool allow,Player*& out){if(!pm->get_local_player(0,allow,out,e)||!out)return missing("actual local PlayerInfo",e);return true;};
    if(enter){
        std::shared_ptr<void> pin;const std::uint8_t* level198=nullptr;
        if(!providers_.currentLevel198||!providers_.currentLevel198(pin,level198,e))return missing("actual current Level198",e);
        if(pin){
            // Source repeats GetCurrentLevel. A null second result is failure.
            if(!providers_.currentLevel198(pin,level198,e)||!pin||!level198)return missing("nonnull repeated current Level198",e);
            bool show=*level198==0;
            if(!show){bool forced=false;if(!providers_.forceUi||!providers_.forceUi(forced,e))return missing("MenuBase.s_igmOpened",e);show=forced;}
            if(show){
                if(!providers_.menuVirtual38||!providers_.menuVirtual38(true,e))return missing("MenuManager.f4 virtual38",e);
                Player* info=nullptr;if(!local(true,info))return false;
                if(info->character660){if(!local(true,info)||!info->character660)return missing("repeated nonnull local Character",e);
                    if(!providers_.reloadSkills||!providers_.reloadSkills(info->character660,e))return missing("Character.ReloadSkills",e);}
                // Application.ShowStatubBar31f668 is literal BX LR.
            }
        }else if(level198)return missing("consistent actual current-Level borrow",e);
        if(app->get_online_loading_v55()->byte5()!=0){
            Player* info=nullptr;if(!local(true,info))return false;
            if(info->character660){if(!local(true,info)||!info->character660)return missing("repeated online Character",e);
                bool dead=false;if(!providers_.isDead||!providers_.isDead(info->character660,dead,e))return missing("actual Character.IsDead",e);
                if(dead){if(!local(false,info))return false;if(!providers_.resetDeadLocal||!providers_.resetDeadLocal(*info,e))return missing("dead-player NetStruct reset",e);}
            }
        }
    }
    if(!providers_.hudNoArgs||!providers_.hudNoArgs(enter?"StartCutscene":"StopCutscene",e))return missing("zero-arg HUD AS callback",e);
    const auto scripts=app->source_script_manager_v52();if(!scripts)return missing("published SAME ScriptManager byte30",e);
    scripts->fields().byte30=enter?1:0;
    if(!providers_.storeDisplayHud||!providers_.storeDisplayHud(enter?0:1,e))return missing("AnimController.s_scalingEnabled",e);
    if(app->get_online_loading_v55()->byte5()==0)return true;
    if(!enter&&(!providers_.sendScriptMessage||!providers_.sendScriptMessage(true,-2,-1,e)))return missing("StopCutscene network message",e);
    for(int i=0;;++i){
        const auto* count=pm->count_field();if(!count)return missing("actual PM count6c4",e);if(i>=*count)break;
        Player* info=nullptr;if(!pm->manager()->get_player(i,false,info,e)||!info)return missing("actual PM.GetPlayer(i,false)",e);
        bool isLocal=false;if(!pm->network()||!pm->network()->is_local(*info,isLocal,e))return missing("actual PlayerInfo.IsLocal",e);
        if(isLocal){if(!providers_.playerInCutscene||!providers_.playerInCutscene(*info,enter,e))return missing("PlayerInfo.SetInCutscene NetStruct",e);}
        else if(info->character660&&(!providers_.renderVisible||!providers_.renderVisible(info->character660,!enter,e)))return missing("Character RenderVisible virtual40",e);
        // Reread real PM count after each effect, preserving source mutation.
    }
    return true;
}
}
