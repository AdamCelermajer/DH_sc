#pragma once
// P17 TRANSITIONS: generic level exit, checkpoint and quest-gated zones for ANY loaded level.
//
// Original rules (IDA, libDungeonHunter2.so, see TRANSITIONS-report.md for addresses):
// - TriggerZoneExitLevel::Update (0039c33c): while the local host touches the zone and
//   GameObject::MeetCondition holds, it shows the fast-travel prompt (DisplayFastTravel with the
//   LevelList display name, the level name and the entry id). Leaving the zone hides it.
//   The travel itself is NativeGoToZone(levelName, entryId) -> Application::LoadLevel (0045cba0 for
//   Script_ChangeLevel, which has no prompt). Both save every player before the load.
// - CheckpointZone::OnCollisionBegins (00395f04): a collision BEGIN by the local player saves the
//   checkpoint (Level::CheckpointSave -> SG_SaveCheckpoint + LevelSavegame::SaveCheckPoint).
// - QuestMoveInZone::OnCollisionBegins (00396120) raises the MoveInZone quest event. The port already
//   builds those zones in features/quest_runtime/quest_zones_v1, so this registry delegates them.
//
// Nothing here names a level, position or script. Targets are resolved through the level table
// (LevelList rows) and entry points through the SpawnPoint declarations of the level being left or
// entered. Conditions go through ConditionSource, which the CONDITIONS session replaces.

#include "../../actor_definitions.hpp"
#include "../campaign_host/trigger_zones.hpp"
#include "../../../game-data/level_tables.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::transitions {

enum class ZoneKind : std::uint8_t {
    level_exit,        // TriggerZoneExitLevel: prompt while inside, travel on confirm
    checkpoint,        // CheckpointZone: save on collision begin
    delegated_quest,   // QuestMoveInZone: owned by quest_zones_v1
    unsupported,       // gametype known to the registry but not built here (reason kept)
};
const char* zone_kind_name(ZoneKind kind) noexcept;

// One row of the LevelList table: `name` is the authored levelName, `file` the bare level file name.
struct LevelRow {
    std::string name;
    std::string file;   // e.g. 003_darkwood.mlx
    // LevelList display text id (LevelList+36, the word the original passes to StringManager). -1 when unknown.
    std::int32_t display_string_id = -1;
};
std::vector<LevelRow> level_rows(const dh2::data::LevelTables& tables);
const LevelRow* find_level(const std::vector<LevelRow>& rows, const std::string& name) noexcept;
// Asset-relative level path used by the loaders (the same form as --level): data/scene/<file>.
std::string level_uri(const LevelRow& row);

// Condition interface. CONDITIONS owns the evaluator; until it lands, LoggedConditionStub answers
// true for every name and logs each name once, so the gate is visible in the run log.
class ConditionSource {
public:
    virtual ~ConditionSource() = default;
    virtual bool condition_active(const std::string& name) = 0;
};
class LoggedConditionStub final : public ConditionSource {
public:
    bool condition_active(const std::string& name) override;
    const std::set<std::string>& logged() const noexcept { return logged_; }
private:
    std::set<std::string> logged_;
};

// P17 CONDITIONS: a ConditionSource answered by a callback (the authored v2Conditions evaluator, bound in main).
class FunctionConditionSource final : public ConditionSource {
public:
    explicit FunctionConditionSource(std::function<bool(const std::string&)> fn) : fn_(std::move(fn)) {}
    bool condition_active(const std::string& name) override { return fn_ ? fn_(name) : false; }
private:
    std::function<bool(const std::string&)> fn_;
};

struct ExitZone {
    ZoneKind kind = ZoneKind::unsupported;
    std::string name;             // authored prim name
    std::string gametype;         // authored class
    std::string source;           // declaration source path
    std::string target_name;      // LevelList name (level_exit)
    std::string target_file;      // asset-relative level path (level_exit, empty when unresolved)
    // Procedural targets (.rule.xml) have no path in the Windows EXE yet (LEVEL-COVERAGE gap 7).
    // The zone is built and reported; the transition is refused at run time, never half-loaded.
    bool target_procedural = false;
    int entry_id = 0;             // entrypointID (level_exit)
    std::string fast_travel;      // authored fasttravel id (display/unlock key, informational here)
    std::string condition;        // authored condition name; empty = unconditional
    // Translation from the original placed declaration. CheckpointZone stores
    // this exact source zone position as the checkpoint key; it is distinct
    // from both the oriented contact-box center and the player's revive point.
    std::array<float, 3> source_position{};
    campaign_host::TriggerZoneBox box;
    bool inside = false;          // last fed contact fact
    bool built = false;           // false = reported in notices(), never fed
    std::string note;             // reason when not built
};

// Class registry: authored gametype -> builder. A builder fills the zone or returns false with a reason.
using ZoneBuilder = std::function<bool(const ActorDefinition& declaration,
                                       const std::vector<LevelRow>& levels,
                                       ExitZone& zone,
                                       std::string& reason)>;

class ZoneClassRegistry {
public:
    ZoneClassRegistry();   // registers TriggerZoneExitLevel, CheckpointZone and QuestMoveInZone
    void add(const std::string& gametype, ZoneBuilder builder);
    const ZoneBuilder* find(const std::string& gametype) const noexcept;
private:
    std::map<std::string, ZoneBuilder> builders_;
};

// Begin-of-contact event for this frame (for level_exit: the prompt appears, for checkpoint: save).
struct ZoneEvent {
    std::size_t zone = 0;
    ZoneKind kind = ZoneKind::unsupported;
};

class ExitZoneSet {
public:
    // Builds the zones of the loaded level from its declarations. Never fails on a single zone;
    // every rejected zone is kept in notices() with its reason. Returns false only on a bad input.
    bool build(const std::vector<ActorDefinition>& declarations,
               const std::vector<LevelRow>& levels,
               const ZoneClassRegistry& registry,
               std::string& error);
    // One frame of player contact. The first call only records the state (no event on arrival).
    // level_exit zones stay armed while the player is inside; checkpoint zones emit on the begin edge.
    std::vector<ZoneEvent> update(const std::array<float, 3>& player, bool alive);
    const std::vector<ExitZone>& zones() const noexcept { return zones_; }
    const std::vector<std::string>& notices() const noexcept { return notices_; }
    // Index of the first zone with this authored name, or npos.
    std::size_t find(const std::string& name) const noexcept;
    std::size_t built_count(ZoneKind kind) const noexcept;
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
private:
    std::vector<ExitZone> zones_;
    std::vector<std::string> notices_;
    bool primed_ = false;
};

// Entry point resolution: the first SpawnPoint declaration (list order, i.e. placement order) whose
// entrypointID equals entry_id. Mirrors Level::_LoadPlayer, which places the player at the first
// matching SpawnPoint and records the entry on the character.
struct SpawnResolution {
    bool found = false;
    std::array<float, 3> position{};
    float heading_radians = 0.0f;   // yaw of the placed X axis (SpawnPoint::PlaceObject sets the rotation)
    std::string name;
    std::size_t candidates = 0;     // how many SpawnPoints carry this entry id
};
SpawnResolution resolve_entry_spawn(const std::vector<ActorDefinition>& declarations, int entry_id);

// A transition the frame loop has decided on and the session loop carries out.
struct PendingTransition {
    std::string zone_name;
    ZoneKind source = ZoneKind::level_exit;
    std::string target_name;
    std::string target_file;
    int entry_id = 0;
};

} // namespace dh::foundation::transitions
