#include "quest_npc_marker_v1.hpp"

namespace dh::foundation::quest_runtime {

void quest_npc_markers_for_row_v1(const dh2::data::QuestDefinitionV51& definition, QuestStateV1 state,
                                  const QuestRowProgressV1* progress, std::int32_t quest_row,
                                  std::vector<QuestNpcMarkerV1>& out) {
    const auto s = std::int32_t(state);
    const bool secondary = definition.priority == 1;
    const auto add = [&](const dh2::data::QuestObjectiveDefinitionV51& objective) {
        if (objective.type != std::int32_t(QuestObjectiveTypeV1::talk_to_npc)) return;
        out.push_back({objective.oid1, secondary, quest_row});
    };
    if (s == 2 || s == 3) {
        if (!(progress && progress->accept.completed)) add(definition.accept);
    } else if (s == 5 || s == 6) {
        for (std::size_t i = 0; i < definition.objectives.size(); ++i) {
            if (progress && i < progress->objectives.size() && progress->objectives[i].completed) continue;
            add(definition.objectives[i]);
        }
    }
}

std::vector<QuestNpcMarkerV1> quest_npc_markers_v1(const QuestTableV1& table, const QuestRuntimeV1& runtime) {
    std::vector<QuestNpcMarkerV1> out;
    for (std::int32_t row = 0; row < std::int32_t(table.rows().size()); ++row) {
        QuestStateV1 state{};
        if (!runtime.state_of(row, state)) continue;
        quest_npc_markers_for_row_v1(table.rows()[std::size_t(row)], state, runtime.progress_of(row), row, out);
    }
    return out;
}

} // namespace dh::foundation::quest_runtime
