#pragma once
#include "runtime_audio_host_v1.hpp"
#include "runtime_attack_sound_v1.hpp"
#include "../../renderer.hpp"
#include "../../../engine-audio/audio_listener_rows_v38.hpp"
#include "../../../game-data/data.hpp"
#include <ostream>
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
    std::uint32_t activity_sequence_{};
    std::uint64_t started_{},dispatched_{},diagnostics_{};
    bool focused_{},minimized_{},activity_known_{};
    std::string level_music_name_,level_music_error_;
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
    // Original LevelConfig `music` (LevelMusicNamesV1). Playback starts from
    // after_update once the window is focused and not minimised, as the
    // original Level::Update path plays it on the first updated frame. An empty
    // name requests nothing.
    bool set_level_music(const std::string& name,std::string& error);
    // Publish actual camera/player and window state before the Session update.
    // A missing device sample returns nullptr; it never supplies a private clock.
    const RetainedFrameAudioClock* before_update(const Camera&,bool focused,
        bool minimized,bool minimal_randoms,std::uint64_t frame,std::string&);
    bool after_update(std::string&);
    bool shutdown(std::string&);
    void summary() const;
};
}
