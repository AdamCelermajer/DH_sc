#pragma once
#include "game_clock_v1.hpp"
#include "../../../../../engine-audio/spatial_playback_contract_v1.hpp"
#include "../../../../../level-world/character_combat_sound_v1.hpp"
#include "gameplay_sfx_catalog_v1.hpp"
#include "gameplay_sfx_types_v1.hpp"
#include "source_sound_names_v1.hpp"
#include <algorithm>
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
#include <vector>

namespace dh2::player_gameplay_audio {
// The hand-audited retained playback subset (skills, Faery spells, CharSounds,
// cutscene cues). CatalogEntry and the Sounds.xml attribute rows live in
// gameplay_sfx_types_v1.hpp; every other staged Sounds.xml SFX is in the
// generated extended_catalog (gameplay_sfx_catalog_v1.hpp).
inline constexpr std::array<CatalogEntry,51> catalog{{
    {"sfx_impact_burst_shot","sfx_impact_burst_shot.wav","sfx_impact_burst_shot.wav",63,457},
    {"sfx_passive_skill_activate_1","sfx_passive_skill_activate_1.wav","sfx_passive_skill_activate_1.wav",143,503},
    {"sfx_passive_skill_activate_2","sfx_passive_skill_activate_2.wav","sfx_passive_skill_activate_2.wav",144,504},
    {"sfx_passive_skill_activate_3","sfx_passive_skill_activate_3.wav","sfx_passive_skill_activate_3.wav",145,505},
    {"sfx_passive_skill_activate_4","sfx_passive_skill_activate_4.wav","sfx_passive_skill_activate_4.wav",146,506},
    {"sfx_skill_mage_doppelganger_statue_explode","sfx_skill_mage_doppelganger_statue_explode.wav","sfx_skill_mage_doppelganger_statue_explode.wav",115,527},
    {"sfx_skill_mage_ilu_hold_time_hit","sfx_skill_mage_ilu_hold_time_hit.wav","sfx_skill_mage_ilu_hold_time_hit.wav",125,530},
    {"sfx_skill_mage_ilu_nightmare","sfx_skill_mage_ilu_nightmare.wav","sfx_skill_mage_ilu_nightmare.wav",123,531},
    {"sfx_skill_mage_thunder_braid","sfx_skill_mage_thunder_braid.wav","sfx_skill_mage_thunder_braid.wav",116,538},
    {"sfx_skill_mage_thunder_braid_add_1","sfx_skill_mage_thunder_braid_add_1.wav","sfx_skill_mage_thunder_braid_add_1.wav",117,539},
    {"sfx_skill_mage_thunder_braid_add_2","sfx_skill_mage_thunder_braid_add_2,wav","sfx_skill_mage_thunder_braid_add_2.wav",118,540},
    {"sfx_skill_rogue_ass_throwing_knife","sfx_skill_rogue_ass_throw_knife.wav","sfx_skill_rogue_ass_throw_knife.wav",139,550},
    {"sfx_skill_rogue_desert_pinwheel","sfx_skill_rogue_desert_pinwheel.wav","sfx_skill_rogue_desert_pinwheel.wav",133,551},
    {"sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear","sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear.wav","sfx_skill_warrior_bsk_bloodmoon_howl_hit_fear.wav",110,555},
    {"sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun","sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun.wav","sfx_skill_warrior_bsk_bloodmoon_howl_hit_stun.wav",111,556},
    {"sfx_spell_lightning_big","sfx_faerie_lightning_big.wav","sfx_faerie_lightning_big.wav",90,584},
    {"sfx_spell_lightning_medium","sfx_faerie_lightning_medium.wav","sfx_faerie_lightning_medium.wav",89,585},
    {"sfx_spell_lightning_small","sfx_faerie_lightning_small.wav","sfx_faerie_lightning_small.wav",88,586},
    // Original BDAE sfx_ event cues found in the supplied source cache.
    // The final field is the exact ordered SDD Sounds row; UID is from sounds.xml.
    {"sfx_cs_earth_troll_intro_spawn","sfx_cutscene_earth_troll_spawn.wav","sfx_cutscene_earth_troll_spawn.wav",434,338},
    {"sfx_cs_swampking_death","sfx_cutscene_swamp_king_die.wav","sfx_cutscene_swamp_king_die.wav",435,341},
    {"sfx_cutscene_final_outro","sfx_cutscene_final_outro.wav","sfx_cutscene_final_outro.wav",458,356},
    {"sfx_cutscene_swamp_intro_cave_troll_1","sfx_cutscene_swamp_intro_cave_troll_1.wav","sfx_cutscene_swamp_intro_cave_troll_1.wav",436,365},
    {"sfx_cutscene_swamp_intro_cave_troll_2","sfx_cutscene_swamp_intro_cave_troll_2.wav","sfx_cutscene_swamp_intro_cave_troll_2.wav",437,366},
    {"sfx_cutscene_swamp_intro_faerie","sfx_cutscene_swamp_intro_faerie.wav","sfx_cutscene_swamp_intro_faerie.wav",438,367},
    {"sfx_cutscene_swamp_intro_rene_thunder","sfx_cutscene_swamp_intro_rene_thunder.wav","sfx_cutscene_swamp_intro_rene_thunder.wav",439,368},
    {"sfx_mc_shoot_arrow","sfx_mc_shoot_arrow.wav","sfx_mc_shoot_arrow.wav",28,491},
    {"sfx_skill_rogue_arc_burst_shot","sfx_skill_rogue_arc_burst_shot.wav","sfx_skill_rogue_arc_burst_shot.wav",137,542},
    {"sfx_skill_rogue_arc_maim_shot","sfx_skill_rogue_arc_maim_shot.wav","sfx_skill_rogue_arc_maim_shot.wav",136,543},
    {"sfx_skill_rogue_arc_pierce_shot","sfx_skill_rogue_arc_pierce_shot.wav","sfx_skill_rogue_arc_pierce_shot.wav",135,544},
    {"sfx_skill_rogue_arc_rain_of_arrows","sfx_skill_rogue_arc_rain_of_arrows.wav","sfx_skill_rogue_arc_rain_of_arrows.wav",134,545},
    {"sfx_skill_rogue_arc_venom_shot","sfx_skill_rogue_arc_venom_shot.wav","sfx_skill_rogue_arc_venom_shot.wav",138,546},
    // Source-scripted Faery elemental loops and spell hits.
    {"sfx_mage_staff_elemental_earth","sfx_mage_staff_elemental_earth.wav","sfx_mage_staff_elemental_earth.wav",31,478},
    {"sfx_mage_staff_elemental_fire","sfx_mage_staff_elemental_fire.wav","sfx_mage_staff_elemental_fire.wav",29,479},
    {"sfx_mage_staff_elemental_ice","sfx_mage_staff_elemental_ice.wav","sfx_mage_staff_elemental_ice.wav",30,480},
    {"sfx_mage_staff_elemental_wind","sfx_mage_staff_elemental_wind.wav","sfx_mage_staff_elemental_wind.wav",32,482},
    {"sfx_spell_fire_big","sfx_faerie_fire_big.wav","sfx_faerie_fire_big.wav",97,581},
    {"sfx_spell_fire_medium","sfx_faerie_fire_medium.wav","sfx_faerie_fire_medium.wav",96,582},
    {"sfx_spell_fire_small","sfx_faerie_fire_small.wav","sfx_faerie_fire_small.wav",95,583},
    {"sfx_spell_water_big","sfx_faerie_water_big.wav","sfx_faerie_water_big.wav",100,587},
    {"sfx_spell_water_medium","sfx_faerie_water_medium.wav","sfx_faerie_water_medium.wav",99,588},
    {"sfx_spell_water_small","sfx_faerie_water_small.wav","sfx_faerie_water_small.wav",98,589},
    // Source-derived CharSounds IDs -> SDD Sounds array -> sounds.xml UID.
    {"CharacterDie","sfx_character_death.wav","sfx_character_death.wav",50,28},
    {"CharacterHurt1","sfx_character_hurt_1.wav","sfx_character_hurt_1.wav",38,29},
    {"CharacterHurt2","sfx_character_hurt_2.wav","sfx_character_hurt_2.wav",39,30},
    {"CharacterHurt3","sfx_character_hurt_3.wav","sfx_character_hurt_3.wav",40,31},
    {"ImpactMetal1","sfx_impact_metal_metal_1.wav","sfx_impact_metal_1.wav",41,117},
    {"ImpactMetal2","sfx_impact_metal_metal_2.wav","sfx_impact_metal_2.wav",42,118},
    {"ImpactMetal3","sfx_impact_metal_metal_3.wav","sfx_impact_metal_3.wav",43,119},
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
enum class Kind : std::uint8_t {play,stop,stop_all,music};
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
    for(const auto& entry:extended_catalog)if(requested==entry.label)return &entry;
    return nullptr;
}
// sounds.xml UID (the VoxSoundManager::Play argument used by level scripts).
inline const CatalogEntry* find_uid(std::uint16_t uid){
    for(const auto& entry:catalog)if(entry.uid==uid)return &entry;
    for(const auto& entry:extended_catalog)if(entry.uid==uid)return &entry;
    return nullptr;
}
inline const CatalogEntry* find_source_sound_id(std::uint16_t id){
    if(id==0xffff)return nullptr;
    for(const auto& entry:catalog)if(entry.source_sound_id==id)return &entry;
    for(const auto& entry:extended_catalog)if(entry.source_sound_id==id)return &entry;
    return nullptr;
}
// Sounds.xml attribute rows for every playable UID (sorted by UID).
inline const SfxDefV1* find_sfx_def(std::uint16_t uid) noexcept {
    const auto it=std::lower_bound(sfx_defs.begin(),sfx_defs.end(),uid,
        [](const SfxDefV1& row,std::uint16_t value){return row.uid<value;});
    return it!=sfx_defs.end()&&it->uid==uid?&*it:nullptr;
}
inline const SfxBankV1* find_sfx_bank(std::uint8_t id) noexcept {
    return id<sfx_banks.size()?&sfx_banks[id]:nullptr;
}
inline const SfxGroupV1* find_sfx_group(std::uint8_t id) noexcept {
    return id<sfx_groups.size()?&sfx_groups[id]:nullptr;
}
inline const SfxEventV1* find_sfx_event(std::uint8_t id) noexcept {
    for(const auto& event:sfx_events)if(event.id==id)return &event;
    return nullptr;
}
// SDD Sounds rows that name a Sounds.xml <event> rather than a sound UID.
inline const SfxEventRowV1* find_sfx_event_row(std::uint16_t row) noexcept {
    const auto it=std::lower_bound(sfx_event_rows.begin(),sfx_event_rows.end(),row,
        [](const SfxEventRowV1& entry,std::uint16_t value){return entry.row<value;});
    return it!=sfx_event_rows.end()&&it->row==row?&*it:nullptr;
}
// Sound-pack Group pos3d == 0 emitters never receive a position (the else branch
// of PlaySoundPackSound sets raw parameter 0 = 1): centred, no distance gain.
// Vox::VoxEngine::Play (0x8621c0) receives the Sounds.xml loop flag from
// VoxSoundPackXML::GetEmitterInfo, never the script/caller argument (Play 0x36b80c
// and PlaySoundPackSound 0x36a7c0 ignore it), so the XML flag decides.
inline bool source_loop(std::uint16_t uid,bool requested) noexcept {
    const auto* def=find_sfx_def(uid);
    return def?def->loop:requested;
}
inline bool sfx_is_positional(std::uint16_t uid) noexcept {
    const auto* def=find_sfx_def(uid);
    const auto* group=def?find_sfx_group(def->group):nullptr;
    return !group||group->pos3d!=0;
}
// vox::VoxSoundPackXML::GetEventSoundUid (0x88bb6c). The original draws from
// lrand48(); the default stream is the POSIX 48-bit LCG. Playlist events cycle
// in order; random events never repeat any of the last `history` picks.
class EventPicker {
public:
    static EventPicker& instance(){static EventPicker value;return value;}
    void seed(std::uint32_t value){
        std::lock_guard<std::mutex> lock(mutex_);
        state_=(std::uint64_t(value)<<16)|0x330eu;seeded_=true;states_={};
    }
    // Returns the picked UID, or -1 for the probability miss (nothing plays).
    std::int32_t pick(const SfxEventV1& event){
        std::lock_guard<std::mutex> lock(mutex_);
        if(!event.count||event.id>=states_.size())return -1;
        if(lrand48_locked()%100>=event.probability)return -1;
        auto& state=states_[event.id];
        if(!state.ready){
            state.pool.assign(event.uids.begin(),event.uids.begin()+event.count);
            state.index=event.count;state.ready=true;
        }
        if(event.playlist){
            if(state.index>=event.count)state.index=0;
            return event.uids[state.index++];
        }
        const auto at=static_cast<std::size_t>(lrand48_locked()%state.pool.size());
        const auto uid=state.pool[at];
        state.pool[at]=state.pool.back();state.pool.pop_back();
        state.history.push_back(uid);
        if(state.history.size()>event.history||state.pool.empty()){
            state.pool.push_back(state.history.front());state.history.erase(state.history.begin());
        }
        return uid;
    }
private:
    struct State {
        bool ready=false;
        std::size_t index=0;
        std::vector<std::uint16_t> pool,history;
    };
    std::uint32_t lrand48_locked(){
        if(!seeded_){state_=0x1234abcd330eULL;seeded_=true;}
        state_=(state_*0x5deece66dULL+0xbULL)&0xffffffffffffULL;
        return static_cast<std::uint32_t>(state_>>17);
    }
    std::mutex mutex_;
    bool seeded_=false;
    std::uint64_t state_=0;
    std::array<State,64> states_{};
};
// First staged member of an event; representative playback entry for status
// reporting (the real member is chosen per call by EventPicker).
inline const CatalogEntry* first_event_member(const SfxEventV1& event) noexcept {
    for(std::size_t i=0;i<event.count;++i)
        if(const auto* entry=find_uid(event.uids[i]))return entry;
    return nullptr;
}
enum class SourceSoundStatusV1 : std::uint8_t {
    not_in_source_table,
    known_but_unavailable,
    playable,
};
struct SourceSoundResolutionV1 {
    SourceSoundStatusV1 status=SourceSoundStatusV1::not_in_source_table;
    const SourceSoundNameV1* source=nullptr;
    const CatalogEntry* playback=nullptr;
};
inline const SourceSoundNameV1* find_source_sound_name(std::string_view name) noexcept {
    for(const auto& source:source_sound_names_v1)
        if(source.name==name)return &source;
    return nullptr;
}
// Resolve an exact SDD Sounds member name. Source-table membership is
// independent of whether this build retained its PCM/audio backend.
inline SourceSoundResolutionV1 resolve_source_sound_name(
    std::string_view name) noexcept {
    SourceSoundResolutionV1 result{};
    result.source=find_source_sound_name(name);
    if(!result.source)return result;
    result.playback=find_source_sound_id(result.source->id);
    if(!result.playback)
        if(const auto* row=find_sfx_event_row(result.source->id))
            if(const auto* event=find_sfx_event(row->event))
                result.playback=first_event_member(*event);
    result.status=result.playback?SourceSoundStatusV1::playable:
                                  SourceSoundStatusV1::known_but_unavailable;
    return result;
}
// Animation events supply the text after the marker's `sfx_` prefix. The
// source Sounds members include that prefix for authored animation cues.
inline SourceSoundResolutionV1 resolve_sfx_marker_sound(
    std::string_view marker_name) noexcept {
    constexpr std::string_view prefix="sfx_";
    if(marker_name.size()>=prefix.size()&&marker_name.substr(0,prefix.size())==prefix)
        return resolve_source_sound_name(marker_name);
    for(const auto& source:source_sound_names_v1)
        if(source.name.size()==prefix.size()+marker_name.size()&&
           source.name.substr(0,prefix.size())==prefix&&
           source.name.substr(prefix.size())==marker_name){
            return resolve_source_sound_name(source.name);
        }
    // If a source event uses a non-prefixed SDD name, retain its exact-table
    // semantics as a fallback rather than inventing a prefixed lookup.
    return resolve_source_sound_name(marker_name);
}
// Q14 left/right gains of one Play3D request: the model-2 distance gain folded
// into the equal-power pan. Fails closed on any invalid request, like the
// original contract.
inline bool spatial_gains_q14(
    const engine_audio::spatial_playback_contract_v1::Request& request,
    std::int32_t& left,std::int32_t& right){
    using namespace engine_audio::spatial_playback_contract_v1;
    const auto spatial=evaluate_stereo_pan(request);
    if((spatial.status!=StereoPanStatus::ready&&
        spatial.status!=StereoPanStatus::degenerate_center)||
       !spatial.has_q14||!spatial.has_distance_gain_q14)return false;
    if(spatial.left_q14<0||spatial.left_q14>16384||
       spatial.right_q14<0||spatial.right_q14>16384||
       spatial.distance_gain_q14<0||spatial.distance_gain_q14>16384)return false;
    left=static_cast<std::int32_t>(
        (std::int64_t(spatial.left_q14)*spatial.distance_gain_q14)>>14);
    right=static_cast<std::int32_t>(
        (std::int64_t(spatial.right_q14)*spatial.distance_gain_q14)>>14);
    return true;
}
inline bool enqueue_spatial_match(
    const CatalogEntry* match,std::uint16_t request_sound_id,
    std::uintptr_t owner,bool looping,std::int32_t fade_ms,
    const engine_audio::spatial_playback_contract_v1::Request& request){
    if(!match||request.sound_id!=request_sound_id)return false;
    std::int32_t left=0,right=0;
    if(!spatial_gains_q14(request,left,right))return false;
    // Non-positional Sounds.xml groups (sfx_int, sfx_prod_elements, ambience)
    // play centred at full gain even through Play3D.
    if(!sfx_is_positional(match->uid))left=right=16384;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=dh2::game_clock::steady_now();
    prune_expired_plays_locked(now);
    if(pending.size()>=queue_limit)return false;
    pending.push_back({Kind::play,owner,match->uid,fade_ms,source_loop(match->uid,looping),
                       match->filename,now,true,left,right});
    return true;
}
inline bool enqueue_play(const char* label,std::size_t length,std::uintptr_t owner,
                         bool looping,std::int32_t fade_ms){
    const auto* match=find_label(label,length);
    if(!match)return false;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=dh2::game_clock::steady_now();
    prune_expired_plays_locked(now);
    if(pending.size()>=queue_limit)return false;
    pending.push_back({Kind::play,owner,match->uid,fade_ms,source_loop(match->uid,looping),
                       match->filename,now});
    return true;
}
inline bool enqueue_spatial_play(
    const char* label,std::size_t length,std::uintptr_t owner,bool looping,
    std::int32_t fade_ms,
    const engine_audio::spatial_playback_contract_v1::Request& request){
    const auto* match=find_label(label,length);
    return enqueue_spatial_match(match,match?match->uid:0,owner,looping,fade_ms,request);
}
// VoxSoundManager::Play3D (0x36b5d8) returns void: a valid SDD Sounds row whose
// PCM is not among the retained assets (most monster, door and loot sounds are
// absent from the supplied cache) is audible silence, never a failed gameplay
// action. Producers that treat a rejected enqueue as an error check this first.
inline bool source_row_without_pcm(std::uint16_t source_sound_id) noexcept {
    return source_sound_id<source_sound_names_v1.size()&&
           !find_source_sound_id(source_sound_id)&&
           !find_sfx_event_row(source_sound_id);
}
// CharSounds stores IDs from the SDD Sounds array; the queue and StopSound
// owner need the resolved sounds.xml UID and exact packaged PCM filename.
inline bool enqueue_spatial_source_sound(
    std::uint16_t source_sound_id,std::uintptr_t owner,bool looping,
    std::int32_t fade_ms,
    const engine_audio::spatial_playback_contract_v1::Request& request){
    const auto* match=find_source_sound_id(source_sound_id);
    if(!match){
        // An SDD row flagged as an event (Play3D -> PlayEvent): validate the
        // request exactly like a direct row, then let GetEventSoundUid choose.
        // A probability miss or an event member without retained PCM is the
        // source "nothing plays" outcome, not a failure.
        const auto* row=find_sfx_event_row(source_sound_id);
        const auto* event=row?find_sfx_event(row->event):nullptr;
        if(!event)return false;
        std::int32_t left=0,right=0;
        if(request.sound_id!=source_sound_id||!spatial_gains_q14(request,left,right))
            return false;
        const auto uid=EventPicker::instance().pick(*event);
        const auto* member=uid<0?nullptr:find_uid(static_cast<std::uint16_t>(uid));
        if(!member)return true;
        return enqueue_spatial_match(member,source_sound_id,owner,looping,fade_ms,request);
    }
    return enqueue_spatial_match(match,source_sound_id,owner,looping,fade_ms,request);
}
enum class MenuSoundResultV1 : std::uint8_t {queued,unavailable,unknown,rejected};
// NativePlaySoundFX 0x43ae10 -> Arrays::GetMemberIDByString<Sounds> ->
// VoxSoundManager::PlayMenu 0x36bc34 -> Play(id,loop=0,fade=0,group=0,menu=true):
// the Sounds row's UID plays 2D (raw emitter parameter 0 = 1), non-positional,
// with the Sounds.xml loop flag. Event rows are not valid PlayMenu targets.
inline MenuSoundResultV1 enqueue_source_menu_sound(std::string_view name){
    const auto resolved=resolve_source_sound_name(name);
    if(resolved.status==SourceSoundStatusV1::not_in_source_table)
        return MenuSoundResultV1::unknown;
    const auto* match=resolved.source?find_source_sound_id(resolved.source->id):nullptr;
    if(!match)return MenuSoundResultV1::unavailable;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=dh2::game_clock::steady_now();
    prune_expired_plays_locked(now);
    if(pending.size()>=queue_limit)return MenuSoundResultV1::rejected;
    pending.push_back({Kind::play,0,match->uid,0,source_loop(match->uid,false),match->filename,now});
    return MenuSoundResultV1::queued;
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
                       dh2::game_clock::steady_now()});
    return true;
}
// VoxSoundManager::StopAllSounds sends a millisecond fade to every active
// emitter, then lets the retained audio owner release each voice at its end.
inline bool enqueue_stop_all(std::int32_t fade_ms){
    if(fade_ms<0)return false;
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    // Level::Unload must enqueue StopAllSounds after its source-ordered
    // teardown work. Reserve one command beyond the ordinary 64-entry cap so
    // a full cue backlog cannot make that lifecycle edge fail. A repeated
    // identical request already at the tail is source-equivalent and can be
    // coalesced; a different pending control still reports exhausted reserve.
    if(pending.size()>queue_limit){
        if(pending.size()==queue_limit+1&&pending.back().kind==Kind::stop_all&&
           pending.back().fade_ms==fade_ms)return true;
        return false;
    }
    Command command;command.kind=Kind::stop_all;command.fade_ms=fade_ms;
    command.queued_at=dh2::game_clock::steady_now();
    pending.push_back(std::move(command));
    return true;
}
// These source Sounds rows resolve through retained original audio inputs.
// UIDs 484-487 share the cathedral VXN bank; UID 467 has the SWAMP VXN bank;
// UIDs 468-504 are the other exported level VXN banks; UIDs 505-509 are single
// looping safe-zone ADPCM WAVs. Their state sets are derived from source
// LevelConfig labels and sounds.xml rows.
struct SourceMusicResource {
    std::uint16_t sound_uid;
    const char* source_label;
    const char* source_filename;
    bool has_vxn_states;
};
inline constexpr std::array<SourceMusicResource,27> source_music_resources{{
    {467,"SwampHubAmbientMusic","m_level_swamp_sfx_swamp.vxn",true},
    {468,"SwampWitchCaveAmbientMusic","m_level_swamp_sfx_witch.vxn",true},
    {470,"WaterTempleCaveAmbientMusic","m_level_swamp_sfx_water.vxn",true},
    {471,"DarkWoodHubAmbientMusic","m_level_darkwood_sfx_darkwood.vxn",true},
    {472,"RootDenOneAmbientMusic","m_level_darkwood_sfx_root_den.vxn",true},
    {474,"EarthTempleAmbientMusic","m_level_darkwood_sfx_void_maze.vxn",true},
    {475,"InfectedVillageAmbientMusic","m_level_darkwood_sfx_infected.vxn",true},
    {477,"GothicusCastleHubAmbientMusic","m_level_gothicus_no_ambiance.vxn",true},
    {484,"AbbeyRuinsOneAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {485,"PortalRoomAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {486,"CryptOneAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {487,"SecretCryptAmbientMusic","m_level_cathedral_sfx_catacomb.vxn",true},
    {488,"RedDesertHubAmbientMusic","m_level_red_desert_sfx_red_desert.vxn",true},
    {489,"RedDesertCaveOneAmbientMusic","m_level_red_desert_sfx_red_desert.vxn",true},
    {492,"LighthouseTempleAmbientMusic","m_level_red_desert_no_ambiance.vxn",true},
    {493,"FireTempleAmbientMusic","m_level_red_desert_sfx_fire.vxn",true},
    {494,"IcyCliffHubAmbientMusic","m_level_icy_cliff_sfx_ice_hub.vxn",true},
    {496,"IcyCaveTwoAmbientMusic","m_level_icy_cliff_sfx_ice_cavern.vxn",true},
    {501,"WindTempleAmbientMusic","m_level_icy_cliff_sfx_wind.vxn",true},
    {502,"UnderworldHubAmbientMusic","m_level_hell_sfx_dark.vxn",true},
    {503,"MachineRoomsAmbientMusic","m_level_hell_sfx_dark.vxn",true},
    {504,"DarkDungeonTempleAmbientMusic","m_level_hell_sfx_dark.vxn",true},
    {505,"SwampMerchantCampMusic","m_safe_zone_01_sfx_swamp.wav",false},
    {506,"BloodSandsMusic","m_safe_zone_01_sfx_red_desert.wav",false},
    {507,"KingsGraveMusic","m_safe_zone_01_sfx_dark_temple.wav",false},
    {508,"GatedCastleVillageMusic","m_safe_zone_02_sfx_darkwood.wav",false},
    {509,"NortherlandMusic","m_safe_zone_02_sfx_ice_hub.wav",false},
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
    command.filename.assign(state);command.queued_at=dh2::game_clock::steady_now();
    pending.push_back(std::move(command));
    return true;
}
// Level music rows are either two-state VXN banks or single-track ambient WAVs;
// enqueue_source_music_state enforces each row's own state set.
inline bool enqueue_source_vxn_music_state(std::uint16_t sound_uid,
                                           std::string_view state){
    return enqueue_source_music_state(sound_uid,state);
}
// The currently selected Crypt LevelConfig resolves specifically to UID 486.
inline bool enqueue_crypt_music_state(std::uint16_t sound_uid,
                                      std::string_view state){
    return sound_uid==486&&enqueue_source_vxn_music_state(sound_uid,state);
}
inline bool consume(std::string& serialized){
    std::lock_guard<std::mutex> lock(queue_mutex());
    auto& pending=queue();
    const auto now=dh2::game_clock::steady_now();
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
        }else if(command.kind==Kind::stop){
            serialized="dh2fx,";
            serialized+="stop,"+std::to_string(command.owner)+","+
                std::to_string(command.sound_id)+","+std::to_string(command.fade_ms);
        }else{
            serialized="dh2fx,stopAll,"+std::to_string(command.fade_ms);
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
