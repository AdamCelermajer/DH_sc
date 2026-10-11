#pragma once
#include <cstdint>
#include <functional>
#include <string>

namespace dh::foundation::cinematics {

// Script_PlaySound 45fefc / Script_StopSound 45fe2c decoded command fields.
// The loop byte is passed to the source manager; the XML emitter's loop flag
// determines playback in Play36b80c. These services borrow the current manager.
struct ScriptSoundCommandV1 {
    std::int32_t fade_ms=0; // field8: plain/stop fade; music play uses !=0 as change
    bool music=false;       // field12
    bool loop=false;        // field13
    std::int32_t ordinal=-1;// field16: generated Sounds row, not XML uid
};
struct ScriptSoundServicesV1 {
    std::function<bool(std::int32_t,bool,bool,int,std::string&)> play_music;
    std::function<bool(std::int32_t,bool,int,std::string&)> play_plain;
    std::function<bool(int,std::string&)> stop_music;
    std::function<bool(std::int32_t,int,std::string&)> stop_plain;
};
// Source manager gates (disabled, online/IGM, volume, ready/resource) and voice
// lifetimes stay in the borrowed audio owner. No Play3D/listener gate is added.
bool execute_script_play_sound_v1(const ScriptSoundCommandV1&,bool skip,
                                 const ScriptSoundServicesV1&,std::string&);
bool execute_script_stop_sound_v1(const ScriptSoundCommandV1&,bool skip,
                                 const ScriptSoundServicesV1&,std::string&);

// Script_SaveGame45c994: current Level/Character are queried in source order.
// Callbacks must use the same regular save receivers, not checkpoint caches.
struct ScriptSaveServicesV1 {
    std::function<bool(bool& present,std::string&)> current_level;
    std::function<bool(bool blocked,std::string&)> block_level;
    std::function<bool(bool disable_save_block,std::string&)> quick_save;
    std::function<bool(bool& present,std::string&)> local_character;
    std::function<bool(bool blocked,std::string&)> block_character;
    std::function<bool(std::string&)> save_character;
};
// QuickSave's phase38/dead/offline/host/position and PlayerSG_Save's receiver
// gates belong to those existing owners. Modern provider errors stop the
// remaining prefix; already reached effects are not rolled back.
bool execute_script_save_game_v1(bool skip,const ScriptSaveServicesV1&,
                                std::string&);

} // namespace dh::foundation::cinematics
