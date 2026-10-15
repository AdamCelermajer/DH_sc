#pragma once
// Process-wide effect counters for the Player skill/faery audit probe
// (skill-matrix and faery-skill). They only count; no gameplay branch reads
// them. Each counter is bumped where the source effect is actually delivered
// (a native returned success, an FX owner matched/launched an instance, the
// Player VM dispatched OnSkill), so the probe can tell a script that ran and
// did nothing from one whose effect reached the world.
#include <cstdint>
#include <string>

namespace dh2::native::player_skill_effect_audit_v1 {

struct Counters {
    std::uint32_t on_skill_dispatched = 0;   // CharAISkillScript::OnSkill ran (Lua status >= 0)
    std::uint32_t on_skill_failed = 0;       // OnSkill callback returned a host/Lua error
    std::uint32_t native_calls = 0;          // every Player skill VM native call
    std::uint32_t native_failed = 0;         // native returned nonzero (script saw an error)
    std::uint32_t attacks = 0;               // SkillCombatRoll returned source attack amounts
    std::uint32_t attack_hits = 0;           // ...of which the amount is positive (damage delivered)
    std::uint32_t damage_natives = 0;        // DealDamages / Attack / SkillCombatRoll calls that succeeded
    std::uint32_t projectiles = 0;           // SpawnSkillProjectile succeeded
    std::uint32_t buffs = 0;                 // CreateBuff / ApplyBuff succeeded
    std::uint32_t fx_requests = 0;           // PlayFX native reached the FX owner
    std::uint32_t fx_matched = 0;            // FX owner matched an AnimatedEffectTable row
    std::uint32_t fx_launched = 0;           // FX owner created and started an instance
    std::uint32_t sound_requests = 0;        // PlaySound3D native reached the sound owner
    std::uint32_t sound_playable = 0;        // sound name resolved to a playable source row
    std::string last_error;                  // last native/OnSkill failure text
};

inline Counters& counters() {
    static Counters value;
    return value;
}

inline void reset_counters() { counters() = Counters{}; }

}  // namespace dh2::native::player_skill_effect_audit_v1
