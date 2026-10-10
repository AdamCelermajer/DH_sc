#pragma once

// P16 DESPAWN: automatic despawn after death, driven through the original actor lifecycle (and, for authored monsters without
// a lifecycle record, through the same owner with population effects; see DESPAWN2 report).
//
// Original rules (IDA pseudocode-all.c + character-monster-state-ownership/registration-plan.json):
//   - CSDead::OnEvent 0x3c4c3c, event 34 (death animation end): SetPhysicalObject(nullptr) (the body leaves the physical
//     world), then, unless the character's persistence predicate (vtbl+40) holds, CharTimers TMR_Start(Despawn_Delay =
//     CharacterDesign 2000 ms, event 46) and flags328 = 64.
//   - CSDead::OnFocus 0x3c4d50: the same timer when the death animation is not pending.
//   - Registered transition: Dead(12) --event 46--> Despawn(2) (registration-plan row 12).
//   - CSDespawn::OnFocus 0x3c32fc: flags328 = 512, SM_SetAnim(-1): the state selects NO clip (the AnimTable "Despawn" slot is
//     not read by any state; only Idle and Reviving select AnimTable slots). CSDespawn registers event 34 and 64 -> Limbus(0).
//   - CSDespawn::OnBlur 0x3c3794: sets +1328 (respawn flag); a summoned actor (CharType 5) or a non-respawnable one is deleted
//     (ObjectBase::Delete); the target of the local player is cleared.
//   - CSLimbus::OnBlur 0x3c2be4: vtbl+64, the initial anchor position and rotation, Revive. CSLimbus::OnFocus 0x3c2e58:
//     flags328 = 0, vtbl+64, the respawn timer (event 47) when +1328 is set and GetRespawnDelay > 0, AI_ClearAllAggro.
//   - GetRespawnDelay 0x3a4bac = 1000 * (CharProperty 11 >> 8) ms (0 when the Character flag at +5249 is set); CanRespawn 0x3a5248 needs
//     property 11 > 0 and the group's permission (not modelled: no group table in this port).
//   - Fade: VisualObject::StartFadeOut/UpdateFadeOut are empty in this build. The body is removed by hiding it.
//
// Port phases (one record per tracked actor, transient, never persisted):
//   dying            the death pose is playing (no timer yet)
//   corpse           body released; Despawn_Delay timer running (the source raises event 46 when it expires)
//   despawning       the Despawn state with its clip (only when the clip option is on); its end is the Limbus transition
//   awaiting_respawn hidden in Limbus with the respawn timer (event 47) running; the respawn is at the initial anchor
// Done: the record is removed. Summoned actors release their pool slot (never respawn).
//
// Owners are injected through Services so the state machine is testable without the EXE.

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace dh::foundation::despawn {

enum class Phase : std::uint8_t { dying, corpse, despawning, awaiting_respawn };

struct Record {
    Phase phase = Phase::dying;
    std::uint32_t delay_ms = 0;     // CharacterDesign.Despawn_Delay for this actor (source constant)
    std::uint32_t remaining_ms = 0; // timer left while corpse / awaiting_respawn
    std::uint32_t respawn_ms = 0;   // GetRespawnDelay (0 = not respawnable)
    bool summoned = false;   // pool slot: released when the despawn completes, never respawns
    bool has_clip = false;   // profile has a Despawn clip AND the clip option is on
};

struct Services {
    // Source SetPhysicalObject(nullptr) at the death-animation end.
    std::function<bool(std::uint64_t actor, std::string& error)> release_body;
    // Source CSDespawn::OnFocus: enter Despawn (lifecycle state 2). With a clip, the clip plays.
    std::function<bool(std::uint64_t actor, std::string& error)> play_clip;
    // Source CSLimbus: hidden in Limbus (lifecycle state 0, or population effects for an authored actor).
    std::function<bool(std::uint64_t actor, std::string& error)> hide;
    // True when the Despawn clip has finished (the lifecycle reached Limbus).
    std::function<bool(std::uint64_t actor, bool& done, std::string& error)> finished;
    // Summoned actors: free the pool slot after the actor is hidden.
    std::function<bool(std::uint64_t actor, std::string& error)> release_slot;
    // Source event 47 at its expiry: revive at the initial anchor and show again (Limbus -> Spawn in the source).
    std::function<bool(std::uint64_t actor, std::string& error)> respawn;
    std::function<void(const std::string& line)> log;
};

class DespawnAfterDeathV1 {
public:
    // Starts tracking a dead actor in the dying phase. Refused for a duplicate.
    bool track(std::uint64_t actor, bool summoned, bool has_clip, std::uint32_t delay_ms, std::string& error,
               std::uint32_t respawn_ms = 0);
    // The death animation has ended (source event 34): release the body and start the delay timer.
    bool death_ended(std::uint64_t actor, Services& services, std::string& error);
    // Advances every timer. Expired corpse timers (event 46) enter Despawn; expired respawn timers (event 47) respawn.
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
