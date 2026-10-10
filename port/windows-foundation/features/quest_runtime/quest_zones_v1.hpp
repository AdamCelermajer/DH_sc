#pragma once
// Quest trigger zones (Preview 16). The zone names are the MoveInZone objectives
// authored in the v2Quest table (str2 of accept and objective rows). Boxes are built
// from the declarations of the LOADED level for any map: a level without a declared
// zone simply has no quest zone, and nothing here names a level or a position.
//
// Box rule = the authored Block size (source default 200 per axis times the authored
// scale, so the half extent is 100 * |scale| about the placement translation), the same
// rule the campaign trigger zones use. Rotated blocks are reported and not built
// (the source rotation convention is not verified here).
//
// Entry is a rising edge of "player inside the box". The first update only records
// the state, so a player who starts inside a zone does not trigger it.
#include "quest_table_v1.hpp"
#include "quest_events_v1.hpp"

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation::quest_runtime {

// One loaded level declaration reduced to what a quest zone needs.
struct QuestZoneDeclarationV1 {
    std::string name;                       // authored prim name (e.g. _prim_WitchQuestStart)
    std::string shape;                      // authored "type" (only "Block" is built)
    std::array<float, 3> position{};        // world translation (module offset applied)
    std::array<float, 3> scale{1, 1, 1};
    std::array<float, 3> rotation{};
};

// Builds a declaration from the authored property map ("type", "scale", "rotation").
// Missing or malformed scale/rotation fall back to identity and are reported by the set.
QuestZoneDeclarationV1 make_quest_zone_declaration_v1(const std::string& name,
    const std::map<std::string, std::string>& properties,
    const std::array<float, 3>& position);

struct QuestZoneV1 {
    std::string name;
    std::array<float, 3> min{}, max{};
    bool inside{};
};

class QuestZoneSetV1 {
public:
    // Replaces the zone set. Names that no declaration provides are reported once in notes().
    // The level check is left to the runtime (MoveInZone oid1 = level row).
    void build(const QuestTableV1& table, const std::vector<QuestZoneDeclarationV1>& declarations);
    // Zone entries (rising edges) for one frame of player position. Returns zone names.
    std::vector<std::string> update(const std::array<float, 3>& player);
    const std::vector<QuestZoneV1>& zones() const noexcept { return zones_; }
    const std::vector<std::string>& notes() const noexcept { return notes_; }
private:
    std::vector<QuestZoneV1> zones_;
    std::vector<std::string> notes_;
    bool primed_{};
};

// The zone names authored by quest MoveInZone objectives (accept and objective rows), sorted and unique.
std::vector<std::string> quest_zone_names_v1(const QuestTableV1& table);

} // namespace dh::foundation::quest_runtime
