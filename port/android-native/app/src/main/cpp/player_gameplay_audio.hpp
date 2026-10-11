#pragma once
#include "../../../../../engine-audio/spatial_playback_contract_v1.hpp"
#include "../../../../../level-world/character_combat_sound_v1.hpp"
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <limits>
#include <mutex>
#include <string>
#include <string_view>

namespace dh2::player_gameplay_audio {
// Source: original data/sounds/sounds_he.xml PCM, retained skill and monster
// playback subset. UID is the original sound-pack UID used by StopSound.
struct CatalogEntry {
    const char* label;
    const char* catalog_filename;
    const char* filename;
    std::uint16_t uid;
    // Original SDD Sounds-table ID consumed by VoxSoundManager::Play3D.
    // Most script cues enter by label and do not need this field.
    std::uint16_t source_sound_id=0xffff;
};
inline constexpr std::array<CatalogEntry,28> catalog{{
    {"sfx_impact_burst_shot","sfx_impact_burst_shot.wav","sfx_impact_burst_shot.wav",63},
    {"sfx_passive_skill_activate_1","sfx_passive_skill_activate_1.wav","sfx_passive_skill_activate_1.wav",143},
    {"sfx_passive_skill_activate_2","sfx_passive_skill_activate_2.wav","sfx_passive_skill_activate_2.wav",144},
    {"sfx_passive_skill_activate_3","sfx_passive_skill_activate_3.wav","sfx_passive_skill_activate_3.wav",145},
    {"sfx_passive_skill_activate_4","sfx_passive_skill_activate_4.wav","sfx_passive_skill_activate_4.wav",146},
    {"sfx_skill_mage_doppelganger_statue_explode","sfx_skill_mage_doppelganger_statue_explode.wav","sfx_skill_mage_doppelganger_statue_explode.wav",115},
    {"sfx_skill_mage_ilu_hold_time_hit","sfx_skill_mage_ilu_hold_time_hit.wav","sfx_skill_mage_ilu_hold_time_hit.wav",125},
    {"sfx_skill_mage_ilu_nightmare","sfx_skill_mage_ilu_nightmare.wav","sfx_skill_mage_ilu_nightmare.wav",123},
    {"sfx_skill_mage_thunder_braid","sfx_skill_mage_thunder_braid.wav","sfx_skill_mage_thunder_braid.wav",116},
    {"sfx_skill_mage_thunder_braid_add_1","sfx_skill_mage_thunder_braid_add_1.wav","sfx_skill_mage_thunder_braid_add_1.wav",117},
    {"sfx_skill_mage_thunder_braid_add_2","sfx_skill_mage_thunder_braid_add_2,wav","sfx_skill_mage_thunder_braid_add_2.wav",118},
    {"sfx_skill_rogue_ass_throwing_knife","sfx_skill_rogue_ass_throw_knife.wav","sfx_skill_rogue_ass_throw_knife.wav",139},
    {"sfx_skill_rogue_desert_pinwheel","sfx_skill_rogue_desert_pinwheel.wav","sfx_skill_rogue_desert_pinwheel.wav",133},
    {"sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear","sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear.wav","sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear.wav",110},
    {"sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun","sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun.wav","sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun.wav",111},
    {"sfx_spell_lightning_big","sfx_faerie_lightning_big.wav","sfx_faerie_lightning_big.wav",90},
    {"sfx_spell_lightning_medium","sfx_faerie_lightning_medium.wav","sfx_faerie_lightning_medium.wav",89},
    {"sfx_spell_lightning_small","sfx_faerie_lightning_small.wav","sfx_faerie_lightning_small.wav",88},
    // Source-derived CharSounds IDs -> SDD Sounds array -> sounds.xml UID.
    {"CharacterDie","sfx_character_death.wav","sfx_character_death.wav",50,28},
    {"CharacterHurt1","sfx_character_hurt_1.wav","sfx_character_hurt_1.wav",38,29},
    {"CharacterHurt2","sfx_character_hurt_2.wav","sfx_character_hurt_2.wav",39,30},
    {"CharacterHurt3","sfx_character_hurt_3.wav","sfx_character_hurt_3.wav",40,31},
    {"ImpactMetal1","sfx_impact_metal_1.wav","sfx_impact_metal_1.wav",41,117},
    {"ImpactMetal2","sfx_impact_metal_2.wav","sfx_impact_metal_2.wav",42,118},
    {"ImpactMetal3","sfx_impact_metal_3.wav","sfx_impact_metal_3.wav",43,119},
    {"ImpactMetalFlesh1","sfx_impact_metal_flesh_1.wav","sfx_impact_metal_flesh_1.wav",44,120},
    {"ImpactMetalFlesh2","sfx_impact_metal_flesh_2.wav","sfx_impact_metal_flesh_2.wav",45,121},
    {"ImpactMetalFlesh3","sfx_impact_metal_flesh_3.wav","sfx_impact_metal_flesh_3.wav",46,122},
}};
using SourceSoundList=dh2::character_combat_sound_v1::SoundList;
using CharacterSoundRow=dh2::character_combat_sound_v1::SoundProfile;
// Exact source rows decoded by ELF CharSoundsTable::read (0x004b96bc) from
// data/pydata/sounds_pyarray.bin. These IDs index the 12-byte SDD Sounds row;
// VoxSoundManager::Play3D (0x0036b5d8) resolves its sound-pack UID at +4.
// CharSoundsTable name/row order is in sounds_pyarraynames.bin; row stride is
// 40 bytes. The per-row values below were executed through the original reader.
inline constexpr std::array<CharacterSoundRow,3> character_sound_rows{{
    {"AAA_DONT_DELETE",{},{},{},{},false,false},
    {"ArmoredHuman",{{28,0,0},1},{{29,30,31},3},
     {{120,121,122},3},{{117,118,119},3},false,true},
    {"Default",{},{},{},{},true,false},
}};
inline const CharacterSoundRow* character_sound_row(std::int32_t id){
    if(id<0||static_cast<std::size_t>(id)>=character_sound_rows.size())return nullptr;
    return &character_sound_rows[static_cast<std::size_t>(id)];
}
// Character::GetCharSoundsId (0x003a3294) reads Character+0x1018 and falls
// back to row 2 when the raw ID is negative or outside CharSoundsTable::size.
inline std::uint8_t character_sound_id_or_default(std::int32_t raw_id) noexcept {
    return raw_id<0||static_cast<std::size_t>(raw_id)>=character_sound_rows.size()
        ?2:static_cast<std::uint8_t>(raw_id);
}
inline const CharacterSoundRow* character_sound_row_for_character(
    std::int32_t raw_id) noexcept {
    return &character_sound_rows[character_sound_id_or_default(raw_id)];
}
inline dh2::character_combat_sound_v1::Result apply_character_combat_sound(
    std::int32_t attacker_sound_row,std::int32_t defender_sound_row,
    std::uintptr_t defender_identity,std::int32_t damage,bool defender_dead,
    bool minimal_randoms,
    const dh2::character_combat_sound_v1::Services& services) noexcept {
    const dh2::character_combat_sound_v1::Request request{
        character_sound_row_for_character(attacker_sound_row),
        character_sound_row_for_character(defender_sound_row),
        defender_identity,damage,defender_dead,minimal_randoms};
    return dh2::character_combat_sound_v1::apply(request,services);
}
enum class Kind : std::uint8_t {play,stop,music};
struct Command {
    Kind kind=Kind::play;
    std::uintptr_t owner=0;
    std::uint16_t sound_id=0;
    std::int32_t fade_ms=0;
    bool looping=false;
    std::string filename;
    std::chrono::steady_clock::time_point queued_at{};
    bool spatial_mix=false;
    std::int32_t left_q14=16384;
    std::int32_t right_q14=16384;
};
inline std::mutex& queue_mutex(){static std::mutex value;return value;}
inline std::deque<Command>& queue(){static std::deque<Command> value;return value;}
inline constexpr auto cue_lifetime=std::chrono::seconds(3);
inline constexpr std::size_t queue_limit=64;
inline void prune_expired_plays_locked(std::chrono::steady_clock::time_point now){
    auto& pending=queue();
    for(auto it=pending.begin();it!=pending.end();){
        if(it->kind==Kind::play&&now-it->queued_at>cue_lifetime)
            it=pending.erase(it);
        else
            ++it;
    }
}
inline const CatalogEntry* find_label(const char* label,std::size_t length){
    if(!label||!length)return nullptr;
    const std::string_view requested(label,length);
    for(const auto& entry:catalog)if(requested==entry.label)return &entry;
    return nullptr;
}
inline const CatalogEntry* find_source_sound_id(std::uint16_t id){
    for(const auto& entry:catalog)if(entry.source_sound_id==id)return &entry;
    return nullptr;
}
inline bool enqueue_spatial_match(
    const CatalogEntry* match,std::uint16_t request_sound_id,
    std::uintptr_t owner,bool looping,std::int32_t fade_ms,
    const engine_audio::spatial_playback_contract_v1::Request& request){
    using namespace engine_audio::spatial_playback_contract_v1;
    if(!match||request.sound_id!=request_sound_id)return false;
    const auto spatial=evaluate_stereo_pan(request);
    if((spatial.status!=StereoPanStatus::ready&&
        spatial.status!=StereoPanStatus::degenerate_center)||
       !spatial.has_q14||!spatial.has_distance_gain_q14)return false;
    if(spatial.left_q14<0||spatial.left_q14>16384||
       spatial.right_q14<0||spatial.right_q14>16384||
       spatial.distance_gain_q14<0||spatial.distance_gain_q14>16384)return false;
    const auto left=static_cast<std::int32_t>(
        (std::int64_t(spatial.left_q14)*spatial.distance_gain_q14)>>14);
    const auto right=static_cast<std::int32_t>(
        (std::int64_t(spatial.right_q14)*spatial.distance_gain_q14)>>14);
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=std::chrono::steady_clock::now();
    prune_expired_plays_locked(now);
    if(pending.size()>=queue_limit)return false;
    pending.push_back({Kind::play,owner,match->uid,fade_ms,looping,match->filename,now,
                       true,left,right});
    return true;
}
inline bool enqueue_play(const char* label,std::size_t length,std::uintptr_t owner,
                         bool looping,std::int32_t fade_ms){
    const auto* match=find_label(label,length);
    if(!match)return false;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=std::chrono::steady_clock::now();
    prune_expired_plays_locked(now);
    if(pending.size()>=queue_limit)return false;
    pending.push_back({Kind::play,owner,match->uid,fade_ms,looping,match->filename,now});
    return true;
}
inline bool enqueue_spatial_play(
    const char* label,std::size_t length,std::uintptr_t owner,bool looping,
    std::int32_t fade_ms,
    const engine_audio::spatial_playback_contract_v1::Request& request){
    const auto* match=find_label(label,length);
    return enqueue_spatial_match(match,match?match->uid:0,owner,looping,fade_ms,request);
}
// CharSounds stores IDs from the SDD Sounds array; the queue and StopSound
// owner need the resolved sounds.xml UID and exact packaged PCM filename.
inline bool enqueue_spatial_source_sound(
    std::uint16_t source_sound_id,std::uintptr_t owner,bool looping,
    std::int32_t fade_ms,
    const engine_audio::spatial_playback_contract_v1::Request& request){
    const auto* match=find_source_sound_id(source_sound_id);
    return enqueue_spatial_match(match,source_sound_id,owner,looping,fade_ms,request);
}
inline bool enqueue(const char* label,std::size_t length,bool looping){
    return enqueue_play(label,length,0,looping,0);
}
// The original wrapper passes a float to VoxSoundManager::Stop(int,int): ARM
// __aeabi_f2iz truncates toward zero; the manager forwards that integer as ms.
inline bool enqueue_stop(const char* label,std::size_t length,std::uintptr_t owner,float fade){
    const auto* match=find_label(label,length);
    if(!match)return false;
    if(!std::isfinite(fade)||static_cast<double>(fade)>std::numeric_limits<std::int32_t>::max()||
       static_cast<double>(fade)<std::numeric_limits<std::int32_t>::min())return false;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    if(pending.size()>=queue_limit)return false;
    pending.push_back({Kind::stop,owner,match->uid,static_cast<std::int32_t>(fade),false,{},
                       std::chrono::steady_clock::now()});
    return true;
}
// These source Sounds rows resolve through retained original audio inputs.
// UIDs 484-487 share the cathedral VXN bank; UID 467 has the SWAMP VXN bank;
// UID 505 is a single looping SWAMP safe-zone ADPCM WAV. Their state sets are
// derived from source LevelConfig labels and sounds.xml rows.
struct SourceMusicResource {
    std::uint16_t sound_uid;
    const char* source_label;
    const char* source_filename;
    bool has_vxn_states;
};
inline constexpr std::array<SourceMusicResource,6> source_music_resources{{
    {467,"SwampHubAmbientMusic","m_level_swamp_sfx_swamp.vxn",true},
    {484,"AbbeyRuinsOneAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {485,"PortalRoomAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {486,"CryptOneAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {487,"SecretCryptAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {505,"SwampMerchantCampMusic","m_safe_zone_01_sfx_swamp.wav",false},
}};
inline const SourceMusicResource* source_music_resource(std::uint16_t uid) noexcept {
    for(const auto& row:source_music_resources)
        if(row.sound_uid==uid)return &row;
    return nullptr;
}
inline const SourceMusicResource* source_vxn_music_resource(std::uint16_t uid) noexcept {
    const auto* row=source_music_resource(uid);
    return row&&row->has_vxn_states?row:nullptr;
}
inline bool enqueue_source_music_state(std::uint16_t sound_uid,
                                       std::string_view state){
    const auto* resource=source_music_resource(sound_uid);
    if(!resource||
       (resource->has_vxn_states
           ?(state!="ambient"&&state!="combat")
           :state!="ambient"))return false;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    if(pending.size()>=queue_limit)return false;
    Command command;command.kind=Kind::music;command.sound_id=sound_uid;
    command.filename.assign(state);command.queued_at=std::chrono::steady_clock::now();
    pending.push_back(std::move(command));
    return true;
}
inline bool enqueue_source_vxn_music_state(std::uint16_t sound_uid,
                                           std::string_view state){
    return source_vxn_music_resource(sound_uid)&&
        enqueue_source_music_state(sound_uid,state);
}
// The currently selected Crypt LevelConfig resolves specifically to UID 486.
inline bool enqueue_crypt_music_state(std::uint16_t sound_uid,
                                      std::string_view state){
    return sound_uid==486&&enqueue_source_vxn_music_state(sound_uid,state);
}
inline bool consume(std::string& serialized){
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=std::chrono::steady_clock::now();
    while(!pending.empty()){
        auto command=std::move(pending.front());pending.pop_front();
        if(command.kind==Kind::play&&now-command.queued_at>cue_lifetime)continue;
        if(command.kind==Kind::music){
            serialized="dh2music,"+std::to_string(command.sound_id)+","+command.filename;
        }else if(command.kind==Kind::play){
            serialized="dh2fx,";
            serialized+="play,"+std::to_string(command.owner)+","+
                std::to_string(command.sound_id)+","+std::to_string(command.fade_ms)+","+
                (command.looping?"1,":"0,")+command.filename;
            if(command.spatial_mix)
                serialized+=",1,"+std::to_string(command.left_q14)+","+
                    std::to_string(command.right_q14);
        }else{
            serialized="dh2fx,";
            serialized+="stop,"+std::to_string(command.owner)+","+
                std::to_string(command.sound_id)+","+std::to_string(command.fade_ms);
        }
        return true;
    }
    serialized.clear();return false;
}
// Exact native-session callback boundary. Unknown labels and unsupported
// music controls stay fail-closed; valid supported requests retain source loop
// and millisecond fade arguments through the shared queue.
inline std::int32_t play_source_sound(void* raw,const char* label,std::size_t length,
                                      bool looping,float fade_ms,bool stop_music){
    if(stop_music)return 0;
    if(!std::isfinite(fade_ms)||
       static_cast<double>(fade_ms)>std::numeric_limits<std::int32_t>::max()||
       static_cast<double>(fade_ms)<std::numeric_limits<std::int32_t>::min())return 1;
    if(!find_label(label,length))return 0;
    const auto owner=reinterpret_cast<std::uintptr_t>(raw);
    return enqueue_play(label,length,owner,looping,static_cast<std::int32_t>(fade_ms))?0:1;
}
inline std::int32_t stop_source_sound(void* raw,const char* label,std::size_t length,float fade_ms){
    if(!std::isfinite(fade_ms)||
       static_cast<double>(fade_ms)>std::numeric_limits<std::int32_t>::max()||
       static_cast<double>(fade_ms)<std::numeric_limits<std::int32_t>::min())return 1;
    if(!find_label(label,length))return 0;
    const auto owner=reinterpret_cast<std::uintptr_t>(raw);
    return enqueue_stop(label,length,owner,fade_ms)?0:1;
}
} // namespace dh2::player_gameplay_audio
