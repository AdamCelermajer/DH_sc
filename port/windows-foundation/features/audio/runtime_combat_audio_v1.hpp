#pragma once

#include "retained_frame_audio_clock.hpp"
#include "../../combat_session.hpp"
#include "../../../level-world/character_combat_sound_tables_v2.hpp"
#include "../../../engine-audio/integration-v42/audio_native_session_v42.hpp"
#include <functional>
#include <array>
#include <deque>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <vector>

namespace dh::foundation::audio {

enum class RuntimeCombatAudioStatusV1 {
    not_applicable,
    dispatched,
    source_name_miss,
    unavailable_clock,
    required_owner_unavailable,
    playback_diagnostic
};

struct RuntimeCombatAudioDiagnosticV1 {
    ActorId actor=invalid_actor_id;
    ActorId target=invalid_actor_id;
    std::uint32_t event_index=0;
    std::string event_name;
    RuntimeCombatAudioStatusV1 status=RuntimeCombatAudioStatusV1::not_applicable;
    std::int32_t source_id=-1;
    std::string detail;
};

// CharSounds row selection and source random state are explicit caller-owned
// facts. ActorId is the stable identity shared by this retained CombatSession
// and its audio source-command provider; it is not a native Character pointer.
struct RuntimeCombatAudioServicesV1 {
    // Optional explicit source override for isolated failure fixtures or a
    // caller that owns the original Character cache identity. Production
    // default reads CharSounds from the same CombatSession resolved sheet.
    std::function<bool(ActorId,std::int32_t&,std::string&)> cached_char_sound_id;
    std::function<bool(std::uint32_t&,std::string&)> minimal_randoms;
    std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> random;
    std::function<void(const RuntimeCombatAudioDiagnosticV1&)> diagnostic;
};

// Mirrors Character::GetCharSoundsId's source field: resolved property 8
// (Sounds). Out-of-range fallback remains the table owner's Get(row) rule.
bool runtime_combat_audio_char_sound_id_v1(const CombatSession&,ActorId,
    std::int32_t&,std::string& error);

// Sidecar for the existing CombatSession and its existing AudioNativeSessionV42.
// It never owns a mixer/output and never turns audio delivery into a gameplay
// callback result. Combat sounds dispatch synchronously from the existing
// retained-event observer boundary, after gameplay and before motion.
class RuntimeCombatAudioV1 {
    dh2::audio::AudioNativeSessionV42& audio_;
    CombatSession& combat_;
    const dh2::character::CharacterCombatSoundTablesV2& tables_;
    RuntimeCombatAudioServicesV1 services_;
    std::weak_ptr<const void> actor_lease_;
    using OccurrenceKey=std::tuple<std::uintptr_t,std::uint64_t,ActorId,std::string,
        std::uint32_t,std::uint32_t,std::int32_t,std::string,std::uint32_t>;
    using UpdateKey=std::tuple<std::uintptr_t,std::uint64_t>;
    std::set<OccurrenceKey> delivered_occurrences_;
    std::deque<OccurrenceKey> delivered_order_;
    std::set<std::size_t> consumed_resolution_indices_,consumed_damage_indices_;
    UpdateKey current_update_{};
    bool has_update_=false;

    bool current_owners(std::string&) const;
    RuntimeCombatAudioDiagnosticV1 dispatch_hit(const PlayableCombatResolution&,
        const DamageEvent&,const RetainedAnimationEvent&,std::uint32_t,
        const std::array<float,3>& target_position,const RetainedFrameAudioClock&);
    void report(const RuntimeCombatAudioDiagnosticV1&) const;
public:
    RuntimeCombatAudioV1(dh2::audio::AudioNativeSessionV42&,CombatSession&,
        const dh2::character::CharacterCombatSoundTablesV2&,
        RuntimeCombatAudioServicesV1);

    RetainedFrameAudioObserver retained_event_observer();
};

} // namespace dh::foundation::audio
