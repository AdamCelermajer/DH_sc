// Companion of batch g1-game-items-save-pack3-27fb18-0: Objective_MoveInZone::DBG_TraceDetailedObjectiveInformation (0x47b3dc).
// It reads Arrays::LevelList::size, which is declared in pydata-accessors/w7_arrays_00.hpp. That header conflicts with
// w7_arrays_02.hpp (Arrays::DesignSettingsTable, used by the main file), so the trace is kept apart.
// Layout and declarations come from dh2/Objective_MoveInZone.hpp.

#include <cstdint>
#include <cstdio>

#include "dh2/Application.hpp"
#include "dh2/Objective_EventReceiver.hpp"
#include "dh2/Objective_MoveInZone.hpp"
#include "pydata-accessors/structs_records.hpp"  // Structs::v2QuestObjectiveStub
#include "pydata-accessors/w7_arrays_00.hpp"     // Arrays::LevelList::size
#include "pydata-accessors/w7_pydata12.hpp"      // PyDataConstants::getConstantName

namespace {

// The listing prints Arrays::LevelList::m_memberNames[id] for an in-range level id. That table is declared in
// member_names.hpp, which cannot be included with structs_records.hpp, so an in-range id prints this placeholder.
// PARTIAL: replace with the table name when the pydata headers are unified.
constexpr const char kUnreadLevelName[] = "?";

}  // namespace

// 0x47b3dc Objective_MoveInZone::DBG_TraceDetailedObjectiveInformation(__sFILE*) const
// Prints the header line, the type name, the object and the level, then calls
// Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation for the base lines.
// PARTIAL: the level name (see kUnreadLevelName). The out-of-range test uses Arrays::LevelList::size as the listing does.
// The "Object" line prints the row's name pointer (+0x1c, m_w7) with %d, as the listing does.
int Objective_MoveInZone::DBG_TraceDetailedObjectiveInformation(__sFILE* stream) const {
    std::FILE* out = reinterpret_cast<std::FILE*>(stream);
    const Structs::v2QuestObjectiveStub* row = m_pyData;  // +0x0c

    static_cast<void>(std::fwrite("\t  v2QuestMoveInZone\n", 1, 0x15, out));
    const char* constantName =
        ApplicationGetInstance().m_pyDataConstants->getConstantName("v2QuestObjectiveType", row->m_w1);  // row +0x04
    std::fprintf(out, "\t    Type        : %s\n", constantName);
    std::fprintf(out, "\t    Object      : %d\n", static_cast<int>(reinterpret_cast<std::uintptr_t>(row->m_w7)));

    const std::int32_t levelId = row->m_w8;  // row +0x20
    const bool outOfRange = levelId < 0 || static_cast<std::uint32_t>(levelId) >= Arrays::LevelList::size;
    const char* levelName = outOfRange ? "na" : kUnreadLevelName;
    std::fprintf(out, "\t    Level (OID) : %s\n\n", levelName);

    return Objective_EventReceiver::DBG_TraceDetailedObjectiveInformation(stream);
}
