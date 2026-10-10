#pragma once

#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/items.hpp"
#include "../../combat_session.hpp"
#include "retained_frame_audio_clock.hpp"
#include <functional>
#include <set>
#include <string>
#include <tuple>

namespace dh::foundation::audio {

using RuntimeAttackSoundSubmitV1=std::function<bool(ActorId,std::int32_t,
    const std::array<float,3>&,std::int64_t,std::string&)>;
using RuntimeAttackSoundReadyV1=std::function<bool()>;

enum class RuntimeAttackSoundStatusV1 {
    not_applicable,
    dispatched,
    unavailable_clock,
    required_owner_unavailable,
    playback_diagnostic
};

struct RuntimeAttackSoundDiagnosticV1 {
    ActorId actor=invalid_actor_id;
    std::uint64_t occurrence=0,update_serial=0;
    std::int64_t sequence_id=-1;
    std::uint32_t step=0;
    std::int32_t sound_id=-1;
    RuntimeAttackSoundStatusV1 status=RuntimeAttackSoundStatusV1::not_applicable;
    std::string detail;
};

// Reads the actual AnimationTables row named by the retained hierarchy entry
// and sends its source sound through the existing RuntimeAudioHost/V42 output.
// Equipment Swoosh selection uses only copied same-session ItemRecord facts;
// this observer owns neither another clock nor gameplay state.
class RuntimeAttackSoundV1 final {
    RuntimeAttackSoundSubmitV1 submit_;
    RuntimeAttackSoundReadyV1 ready_;
    CombatSession& session_;
    const dh2::data::AnimationTables& animations_;
    const dh2::data::ItemTable& items_;
    std::function<void(const RuntimeAttackSoundDiagnosticV1&)> diagnostic_;
    using OccurrenceKey=std::tuple<std::uintptr_t,ActorId,std::uint64_t>;
    std::set<OccurrenceKey> delivered_;

    void report(const RuntimeAttackSoundDiagnosticV1&) const noexcept;
    void dispatch(const CombatSessionStepEntry&);
public:
    RuntimeAttackSoundV1(RuntimeAttackSoundSubmitV1,RuntimeAttackSoundReadyV1,CombatSession&,
        const dh2::data::AnimationTables&,const dh2::data::ItemTable&,
        std::function<void(const RuntimeAttackSoundDiagnosticV1&)> diagnostic={});
    CombatSessionStepObserver step_entry_observer();
};

} // namespace dh::foundation::audio
