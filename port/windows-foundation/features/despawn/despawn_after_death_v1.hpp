#pragma once

// P16 DESPAWN: automatic despawn after death, driven through the original actor lifecycle.
//
// Original rules (IDA pseudocode-all.c + character-monster-state-ownership/registration-plan.json):
//   - CSDead::OnEvent 0x3c4c3c, event 34 (death animation end): SetPhysicalObject(nullptr) (the body leaves the physical
//     world), then, unless the character's persistence predicate (vtbl+40) holds, CharTimers TMR_Start(Despawn_Delay =
//     CharacterDesign 2000 ms, event 46) and flags328 = 64.
//   - CSDead::OnFocus 0x3c4d50: the same timer when the death animation is not pending.
//   - Registered transition: Dead(12) --event 46--> Despawn(2) (registration-plan row 12).
//   - CSDespawn::OnFocus 0x3c32fc: flags328 = 512, the Despawn clip (AnimTable "Despawn"). CSDespawn registers event 34
//     (clip end) and 64 -> Limbus(0).
//   - CSDespawn::OnBlur 0x3c3794: a summoned actor (CharType 5) or a non-respawnable one is deleted (ObjectBase::Delete);
//     a respawnable one goes to Limbus. The port's summoned actors are the spawn-pool slots (released on Limbus).
//
// Port phases (one record per tracked actor, transient, never persisted):
//   dying      the death pose is playing (no timer yet)
//   corpse     body released; Despawn_Delay timer running (the source raises event 46 when it expires)
//   despawning Despawn clip playing (lifecycle state 2); its end is the lifecycle's Limbus transition
// An actor with no Despawn clip in its data goes straight from the timer to Limbus (no clip is invented).
// Done: the record is removed. Summoned actors also release their pool slot, so a later spawn reuses it.
//
// Owners are injected through Services so the state machine is testable without the EXE.

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace dh::foundation::despawn {

enum class Phase : std::uint8_t { dying, corpse, despawning };

struct Record {
    Phase phase = Phase::dying;
    std::uint32_t delay_ms = 0;     // CharacterDesign.Despawn_Delay for this actor (source constant)
    std::uint32_t remaining_ms = 0; // timer left while corpse
    bool summoned = false;   // pool slot: released when the despawn completes
    bool has_clip = false;   // profile has a Despawn clip in the original data
};

struct Services {
    // Source SetPhysicalObject(nullptr) at the death-animation end: lifecycle remove_physical.
    std::function<bool(std::uint64_t actor, std::string& error)> release_body;
    // Lifecycle state 2 (Despawn) with its clip: OriginalActorLifecycle::despawn.
    std::function<bool(std::uint64_t actor, std::string& error)> play_clip;
    // No Despawn clip: lifecycle Limbus directly (hidden).
    std::function<bool(std::uint64_t actor, std::string& error)> hide;
    // True when the lifecycle has reached Limbus after the Despawn clip (clip finished).
    std::function<bool(std::uint64_t actor, bool& done, std::string& error)> finished;
    // Summoned actors: free the pool slot after the actor is hidden.
    std::function<bool(std::uint64_t actor, std::string& error)> release_slot;
    std::function<void(const std::string& line)> log;
};

class DespawnAfterDeathV1 {
public:
    // Starts tracking a dead lifecycle actor in the dying phase. Refused for a duplicate.
    bool track(std::uint64_t actor, bool summoned, bool has_clip, std::uint32_t delay_ms, std::string& error);
    // The death animation has ended (source event 34): release the body and start the delay timer.
    bool death_ended(std::uint64_t actor, Services& services, std::string& error);
    // Advances every corpse timer. Expired timers (source event 46) start the Despawn clip or hide the actor.
    bool advance(std::uint32_t elapsed_ms, Services& services, std::string& error);
    // Completes despawns whose clip finished; releases pool slots of summoned actors.
    bool poll(Services& services, std::string& error);
    bool tracked(std::uint64_t actor) const noexcept { return records_.count(actor) != 0; }
    const Record* record(std::uint64_t actor) const noexcept;
    std::size_t size() const noexcept { return records_.size(); }
    // Transient state is dropped with the world (load, reset). No owner is touched.
    void clear() noexcept { records_.clear(); }

private:
    std::map<std::uint64_t, Record> records_;
};

} // namespace dh::foundation::despawn
