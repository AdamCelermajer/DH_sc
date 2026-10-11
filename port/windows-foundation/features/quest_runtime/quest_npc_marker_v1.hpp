#pragma once
// B087: quest-giver "!" markers over NPC heads. Source: Objective_TalkToNPC::InstallObjectiveMarker (IDA 0x47d8e8) /
// RemoveObjectiveMarker (0x47da6c). A TalkToNPC objective whose quest is in a marker-installing state attaches an
// AnimatedFX (DesignSettings QuestMarker / QuestMarkerSecondary) to the first Character whose CharacterTable row equals
// the objective's oid1. Pure selection logic; the renderer owns the visual.
#include "quest_runtime_v1.hpp"
#include <cstdint>
#include <vector>

namespace dh::foundation::quest_runtime {

struct QuestNpcMarkerV1 {
    std::int32_t npc_row{-1};   // CharacterTable row (objective oid1)
    bool secondary{false};      // quest priority == 1 (Secondary): DesignSettings QuestMarkerSecondary, else QuestMarker
    std::int32_t quest_row{-1};
};

// Markers one quest contributes in `state`. Accept objective: states 2..3 (pre_available/available; SetState case 2/3
// markers); authored objectives: states 5..6 (pre_active, Compile at active), not yet completed. The completed variants
// (state > 7, DesignSettings QuestMarkerCompleted/SecondaryCompleted) are -1 in the shipped table: no marker.
void quest_npc_markers_for_row_v1(const dh2::data::QuestDefinitionV51& definition, QuestStateV1 state,
                                  const QuestRowProgressV1* progress, std::int32_t quest_row,
                                  std::vector<QuestNpcMarkerV1>& out);

// All rows of the runtime's table.
std::vector<QuestNpcMarkerV1> quest_npc_markers_v1(const QuestTableV1& table, const QuestRuntimeV1& runtime);

} // namespace dh::foundation::quest_runtime
