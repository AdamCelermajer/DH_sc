#include "script_source_services_v1.hpp"

namespace dh::foundation::cinematics {
namespace {
bool missing(const char* name,std::string& e){e=std::string("Unbound source script service: ")+name;return false;}
}
bool execute_script_play_sound_v1(const ScriptSoundCommandV1& c,bool skip,
                                 const ScriptSoundServicesV1& s,std::string& e){
    e.clear();
    if(!c.music&&skip)return true;
    if(c.music){
        if(!s.play_music)return missing("PlayMusic",e);
        return s.play_music(c.ordinal,c.loop,c.fade_ms!=0,2000,e);
    }
    if(!s.play_plain)return missing("Play",e);
    return s.play_plain(c.ordinal,c.loop,c.fade_ms,e);
}
bool execute_script_stop_sound_v1(const ScriptSoundCommandV1& c,bool,
                                 const ScriptSoundServicesV1& s,std::string& e){
    e.clear();
    if(c.music){
        if(!s.stop_music)return missing("StopMusic",e);
        return s.stop_music(c.fade_ms,e);
    }
    if(!s.stop_plain)return missing("Stop",e);
    return s.stop_plain(c.ordinal,c.fade_ms,e);
}
bool execute_script_save_game_v1(bool,const ScriptSaveServicesV1& s,std::string& e){
    e.clear();bool present=false;
    if(!s.current_level)return missing("GetCurrentLevel",e);
    if(!s.current_level(present,e))return false;
    if(present){
        if(!s.block_level)return missing("SG_BlockLevel",e);
        if(!s.block_level(false,e))return false;
        if(!s.quick_save)return missing("QuickSave",e);
        if(!s.quick_save(false,e))return false;
    }
    present=false;
    if(!s.local_character)return missing("GetLocalPlayer Character",e);
    if(!s.local_character(present,e))return false;
    if(present){
        if(!s.block_character)return missing("SG_Block",e);
        if(!s.block_character(false,e))return false;
        if(!s.save_character)return missing("SG_Save",e);
        if(!s.save_character(e))return false;
    }
    return true;
}
} // namespace dh::foundation::cinematics
