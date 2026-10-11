#include "script_source_services_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh::foundation::cinematics;
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int main(){try{
    std::string e;std::vector<std::string> calls;
    ScriptSoundServicesV1 audio;
    audio.play_plain=[&](auto ordinal,bool loop,int fade,std::string&){require(ordinal==343&&!loop&&fade==17,"plain payload");calls.push_back("play");return true;};
    audio.play_music=[&](auto ordinal,bool loop,bool change,int fade,std::string&){require(ordinal==22&&loop&&change&&fade==2000,"music payload");calls.push_back("music");return true;};
    audio.stop_plain=[&](auto ordinal,int fade,std::string&){require(ordinal==343&&fade==17,"stop payload");calls.push_back("stop");return true;};
    audio.stop_music=[&](int fade,std::string&){require(fade==2000,"stop music fade");calls.push_back("stop-music");return true;};
    ScriptSoundCommandV1 plain{17,false,false,343},music{2000,true,true,22};
    require(execute_script_play_sound_v1(plain,true,{},e)&&calls.empty(),"skip SFX must not require owner");
    require(execute_script_play_sound_v1(plain,false,audio,e),"played SFX");
    require(execute_script_play_sound_v1(plain,false,audio,e)&&calls.size()==2,"repeated source Play not deduplicated");
    require(execute_script_play_sound_v1(music,true,audio,e),"music survives skip");
    require(execute_script_stop_sound_v1(plain,true,audio,e),"plain stop survives skip");
    require(execute_script_stop_sound_v1(music,true,audio,e),"music stop survives skip");
    audio.play_music=[&](auto ordinal,bool,bool change,int fade,std::string&){require(ordinal==-1&&!change&&fade==2000,"negative music unchanged flag");return true;};
    require(execute_script_play_sound_v1({0,true,false,-1},false,audio,e),"source music -1 forwarded to manager");
    require(!execute_script_play_sound_v1(music,true,{},e)&&e.find("PlayMusic")!=std::string::npos,"unbound music explicit");
    audio.stop_plain=[](auto,int,std::string& e){e="stop refused";return false;};
    require(!execute_script_stop_sound_v1(plain,false,audio,e)&&e=="stop refused","provider error preserved");

    ScriptSaveServicesV1 save;bool level=true,character=true;
    save.current_level=[&](bool& p,std::string&){calls.push_back("level");p=level;return true;};
    save.block_level=[&](bool blocked,std::string&){require(!blocked,"level unblock");calls.push_back("unblock-level");return true;};
    save.quick_save=[&](bool disable,std::string&){require(!disable,"QuickSave false preserves receiver flag");calls.push_back("quick-save");return true;};
    save.local_character=[&](bool& p,std::string&){calls.push_back("character");p=character;return true;};
    save.block_character=[&](bool blocked,std::string&){require(!blocked,"character unblock");calls.push_back("unblock-character");return true;};
    save.save_character=[&](std::string&){calls.push_back("save-character");return true;};
    const std::vector<std::string> full{"level","unblock-level","quick-save","character","unblock-character","save-character"};
    calls.clear();require(execute_script_save_game_v1(true,save,e)&&calls==full,"skip save ordered");
    require(execute_script_save_game_v1(false,save,e)&&calls.size()==12,"repeat save not deduplicated");
    calls.clear();level=false;require(execute_script_save_game_v1(false,save,e)&&calls==std::vector<std::string>({"level","character","unblock-character","save-character"}),"no level still player save");
    calls.clear();level=true;character=false;require(execute_script_save_game_v1(false,save,e)&&calls==std::vector<std::string>({"level","unblock-level","quick-save","character"}),"no character still level save");
    calls.clear();level=false;require(execute_script_save_game_v1(false,save,e)&&calls==std::vector<std::string>({"level","character"}),"both absent valid noop");
    level=true;character=true;calls.clear();save.quick_save=[&](bool,std::string& e){calls.push_back("quick-fail");e="write failure";return false;};
    require(!execute_script_save_game_v1(false,save,e)&&e=="write failure"&&calls==std::vector<std::string>({"level","unblock-level","quick-fail"}),"failed prefix no player side effects");
    require(!execute_script_save_game_v1(false,{},e)&&e.find("GetCurrentLevel")!=std::string::npos,"unbound save explicit");
    std::cout<<"script source audio/save service branches passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
