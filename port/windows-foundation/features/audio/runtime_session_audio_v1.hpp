#pragma once
#include "runtime_audio_host_v1.hpp"
#include "runtime_attack_sound_v1.hpp"
#include "../../renderer.hpp"
#include "../../../engine-audio/audio_listener_rows_v38.hpp"
#include "../../../game-data/data.hpp"
#include <array>
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <exception>
#include <utility>
#include <vector>

namespace dh::foundation::audio {
class StepEntryPresentationObserversV1 final {
    std::vector<CombatSessionStepObserver> observers_;
public:
    bool add(CombatSessionStepObserver observer,std::string& error) {
        if(!observer) {error="Step-entry presentation observer is empty";return false;}
        if(observers_.size()>=64) {
            error="Step-entry presentation observer capacity reached";return false;
        }
        observers_.push_back(std::move(observer));error.clear();return true;
    }
    void clear() noexcept { observers_.clear(); }
    std::size_t size()const noexcept{return observers_.size();}
    CombatSessionStepObserver compose(CombatSessionStepObserver primary,
        std::function<void(const std::string&)> diagnostic={}) const {
        const auto observers=observers_;
        return [primary=std::move(primary),observers,diagnostic=std::move(diagnostic)](
            const CombatSessionStepEntry& event) {
            const auto invoke=[&](const CombatSessionStepObserver& observer,const char* role) {
                if(!observer)return;
                try {observer(event);}
                catch(const std::exception& ex) {
                    try {if(diagnostic)diagnostic(std::string("Audio step observer ")+role+
                        " threw: "+ex.what());} catch(...) {}
                } catch(...) {
                    try {if(diagnostic)diagnostic(std::string("Audio step observer ")+role+
                        " threw an unknown exception");} catch(...) {}
                }
            };
            invoke(primary,"primary");
            for(const auto& observer:observers)invoke(observer,"presentation");
        };
    }
};

// Production composition of the existing host/observer. The Session remains
// gameplay authority; this owns only output, listener facts and diagnostics.
class RuntimeSessionAudioV1 final {
    struct Context;
    std::shared_ptr<Context> context_;
    std::unique_ptr<RuntimeAudioHostV1> host_;
    std::unique_ptr<RuntimeCombatAudioV1> combat_;
    std::unique_ptr<RuntimeAttackSoundV1> attack_;
    CombatSessionStepObserver attack_step_observer_;
    StepEntryPresentationObserversV1 step_entry_presenters_;
    std::weak_ptr<CombatSession> bound_;
    dh2::character::CharacterCombatSoundTablesV2 tables_;
    dh2::data::AnimationTables animations_;
    dh2::data::ItemTable items_;
    bool attack_tables_ready_{};
    dh2::audio::AudioListenerRowV38 listener_;
    RetainedFrameAudioClock clock_{};
    struct FaeryPreSoundRequestV1 {
        ActorId caster{};
        std::array<float,3> position{};
        std::size_t target_count{};
        std::vector<std::string> labels;
    };
    std::vector<FaeryPreSoundRequestV1> faery_pre_pending_;
    std::uint32_t activity_sequence_{};
    std::uint64_t started_{},dispatched_{},diagnostics_{};
    bool focused_{},minimized_{},activity_known_{};
    std::string level_music_name_,level_music_error_;
    // P17 SAFEZONE: the level's own track and its safezone track (empty when the level has none).
    std::string level_music_base_,level_music_safezone_;
    bool level_music_in_safe_zone_{};
    bool level_music_revive_pending_{};
    // v20 SCRIPTSVC: Level::Update starts the level track once (latch this+324) and SetInSafeZoneMusic/revive start it
    // explicitly. The start is pending from those events until it succeeds; it is not re-asserted every frame, so a
    // scripted PlayMusic holds until the next source event.
    bool level_music_start_pending_{};
    std::ostream& log_;
public:
    explicit RuntimeSessionAudioV1(std::ostream&);
    ~RuntimeSessionAudioV1();
    bool start(const std::string& exact_assets,
               const std::vector<std::uint8_t>& legacy_sounds,int listener_index,
               const std::shared_ptr<CombatSession>&,bool focused,bool minimized,
               std::string&,const std::string& gameplay_metadata_root={});
    bool bind(const std::shared_ptr<CombatSession>&,std::string&);
    bool add_step_entry_presentation_observer(CombatSessionStepObserver,std::string&);
    void clear_step_entry_presentation_observers() noexcept;
    // Primary audio observer runs first, then retained presenters in insertion
    // order. Each exception is isolated and reported without escaping Session.
    CombatSessionStepObserver compose_step_entry_observer(CombatSessionStepObserver audio_first);
    void unbind() noexcept;
    bool window_activity(bool focused,bool minimized,std::string&);
    // P15 FAERYSOUND (B050): original Celest/Hotty OnPreSkill_ PlaySound3D labels.
    // Queued by the Faery cast (every cast, empty target list included) and
    // submitted by flush_faery_pre_sounds on the same frame's device clock.
    void queue_faery_pre_sounds(ActorId caster,const std::array<float,3>& position,
        std::size_t target_count,std::vector<std::string> labels);
    // clock==nullptr (no device clock this frame) drops the queue with a logged status.
    bool flush_faery_pre_sounds(const RetainedFrameAudioClock* clock,std::string& error);
    // Original LevelConfig `music` (LevelMusicNamesV1). Playback starts from
    // after_update once the window is focused and not minimised, as the
    // original Level::Update path plays it on the first updated frame. An empty
    // name requests nothing.
    bool set_level_music(const std::string& name,std::string& error);
    // P17 SAFEZONE: names the level's safezone track (LevelMusicNamesV1::safezone); empty keeps the level track.
    void set_level_music_safezone(const std::string& name);
    // P17 SAFEZONE: Enter (true) switches to the safezone track, Leave (false) back to the level track. The
    // after_update start gate performs the switch (ordinal change). Without a safezone track the level track stays.
    bool set_level_music_safe_zone(bool entering,std::string& error);
    // Original PlayerManager::ReviveLocalPlayers: StopAllMusic(2) then
    // PlayMusic(level music, 1000). Restart is deferred until output is focused.
    // Not yet reached by the normal EXE: no global-death/revive flow is wired.
    bool on_local_players_revived(std::string& error);
    // Original MenuMainMenu::Hide StopMusic(1000) on the way out of gameplay.
    // Clears the track so after_update cannot restart it before teardown.
    bool on_return_to_menu(std::string& error);
    // I024 DEATH: source SetSoundVolume(VolumeFX) for the death mute (true) and the revive restore (false).
    bool set_fx_muted(bool muted,std::string& error);
    // Publish actual camera/player and window state before the Session update.
    // A missing device sample returns nullptr; it never supplies a private clock.
    const RetainedFrameAudioClock* before_update(const Camera&,bool focused,
        bool minimized,bool minimal_randoms,std::uint64_t frame,std::string&);
    bool after_update(std::string&);
    // B048: ItemObject drop/pickup Play3D through this session's live host.
    bool submit_world_item_sound(std::int32_t source_ordinal,
        const std::array<float,3>& position,WorldItemSoundResultV1& result,std::string& error);
    // v20 SCRIPTSVC: compiled-bank Script_PlaySound / Script_StopSound. Music uses the shared PlayMusic/StopMusic voice
    // (the same owner as level music); plain uses the generated Play row. Missing original assets are logged and silent.
    bool script_play_music(std::int32_t ordinal,bool change,int fade_ms,std::string& error);
    bool script_stop_music(int fade_ms,std::string& error);
    bool script_play_plain(std::int32_t ordinal,int delay_ms,std::string& error);
    // B111: NativePlaySoundFX(name) from an authored in-game menu handler (e.g. "MenuSelect"): plain Play of the named
    // generated source sound. Unknown names are a source no-op (returns true).
    bool play_named_plain(const char* name,std::string& error);
    // B154: LuaScript::_PlayMusic 0x37e420 (AI script PlayMusic(name, fade)): GetMemberIDByString<Sounds>(name), then
    // VoxSoundManager::PlayMusic(id, change=1, 0, fade) - the same owner as script_play_music. Unknown names are a no-op.
    bool play_named_music(const char* name,int fade_ms,std::string& error);
    // Typed diagnostics for original plain Play; retains ordinal/UID/own token.
    NamedUiCueResultV1 play_named_plain_result(const char* name,std::string& error);
    bool script_stop_plain(std::int32_t ordinal,int fade_ms,std::string& error);
    // v20.1 SOUNDS (B090/B099/B104): NativePlaySoundFX -> VoxSoundManager::PlayMenu of a named Arrays::Sounds row
    // (plain emitter, same as Script_PlaySound). Reports actual command submission separately from source no-ops.
    NamedUiCueResultV1 play_named_ui_sound(const char* name,std::string& error);
    bool shutdown(std::string&);
    void summary() const;
};
}
