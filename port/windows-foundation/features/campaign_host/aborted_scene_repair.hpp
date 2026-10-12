#pragma once
// B145 PORT REPAIR (not original behaviour): one-time re-arm of a consumed save-blocked scene trigger whose scene never ran.
//
// A v21.3 build could abort a trigger scene after the trigger had already counted its activation (B141), and the next save kept
// "activated once" while the scene's spawn target was still unspawned. The original engine cannot write that state:
// Script_BlockSaveGame (0x45c8c0) makes LevelSavegame::Save a no-op for the whole scene and the scene's closing
// Script_SaveGame (0x45c994) runs only after Script_SpawnCharacter. So the combination below proves an aborted run, not a
// finished scene, and the scene is armed once more. Nothing here names a level, scene, trigger or actor: the scene is derived
// from the compiled script bank (OriginalCampaignRuntime::trigger_scene_facts) and the actors from live lifecycle facts.
#include "../../original_campaign_runtime.hpp"
#include <functional>
#include <iosfwd>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::campaign_host {

// Live facts of one named actor, supplied by the owner of the restored world.
struct SceneActorFact {
    bool found = false;          // resolves to a live session actor
    bool alive = false;          // not dead
    bool never_spawned = false;  // lifecycle PreSpawn17 record, not Limbus, physical body known absent
    bool at_anchor = false;      // still on its authored initial anchor
};
using SceneActorFactProvider = std::function<bool(const std::string& actor, SceneActorFact& fact, std::string& error)>;

inline constexpr const char* aborted_scene_rearm_repair_id = "B145.rearm_aborted_blocked_scene_v1";
// Identity stored in the save (component campaign_repairs_v1) once the repair re-armed this trigger.
std::string repair_stamp(const std::string& repair_id, const std::string& trigger_key);

enum class SceneRepairVerdict {
    rearm,                    // all admission conditions hold
    not_a_blocked_scene,      // no analyzable enter script that spawns an actor inside a Script_BlockSaveGame window
    not_consumed,             // the trigger can still fire by itself
    already_applied,          // the stamp is already in the save
    actor_unresolved,         // a spawn/kill target could not be resolved to a live actor
    spawn_target_not_pristine,// a spawn target is dead, already spawned, Limbus, or away from its anchor
    kill_target_gone,         // a Script_KillActor target of the scene is already dead
    provider_error
};
const char* verdict_name(SceneRepairVerdict verdict) noexcept;

struct SceneRepairDecision { SceneRepairVerdict verdict = SceneRepairVerdict::not_a_blocked_scene; std::string detail; };

// Pure decision for one trigger key (no mutation).
SceneRepairDecision decide_aborted_scene_rearm(const OriginalCampaignRuntime& runtime, const std::string& trigger_key,
                                               const std::set<std::string>& applied_stamps, const SceneActorFactProvider& facts);

// Evaluates every trigger with a restored activation count; re-arms (count, inside edge, delay) each admitted one and
// records its stamp. Returns the re-armed trigger keys. Writes one line per decision that is not the trivial no-op.
std::vector<std::string> rearm_aborted_scene_triggers(OriginalCampaignRuntime& runtime, std::set<std::string>& applied_stamps,
                                                      const SceneActorFactProvider& facts, std::ostream& log);

} // namespace dh::foundation::campaign_host
