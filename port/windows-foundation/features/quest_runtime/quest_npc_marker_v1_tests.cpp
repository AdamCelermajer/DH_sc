#include "quest_npc_marker_v1.hpp"
#include <cstdio>
#include <cstdlib>
using namespace dh::foundation::quest_runtime;
static void check(bool ok, const char* m) { if (!ok) { std::printf("FAIL %s\n", m); std::exit(1); } }
int main() {
    dh2::data::QuestDefinitionV51 d;
    d.accept.type = 5; d.accept.oid1 = 365; d.priority = 1;
    d.objectives.resize(2); d.objectives[0].type = 5; d.objectives[0].oid1 = 41; d.objectives[1].type = 10;
    std::vector<QuestNpcMarkerV1> m;
    quest_npc_markers_for_row_v1(d, QuestStateV1::available, nullptr, 7, m);
    check(m.size() == 1 && m[0].npc_row == 365 && m[0].secondary && m[0].quest_row == 7, "available: accept talk marker");
    m.clear(); quest_npc_markers_for_row_v1(d, QuestStateV1::locked, nullptr, 7, m);
    check(m.empty(), "locked: none");
    m.clear(); quest_npc_markers_for_row_v1(d, QuestStateV1::active, nullptr, 7, m);
    check(m.size() == 1 && m[0].npc_row == 41, "active: talk objective only (kill objective ignored)");
    QuestRowProgressV1 p; p.objectives.resize(2); p.objectives[0].completed = true;
    m.clear(); quest_npc_markers_for_row_v1(d, QuestStateV1::active, &p, 7, m);
    check(m.empty(), "completed talk objective: marker removed");
    m.clear(); quest_npc_markers_for_row_v1(d, QuestStateV1::completed, nullptr, 7, m);
    check(m.empty(), "completed state: no marker (Completed effect -1)");
    d.priority = 0; m.clear(); quest_npc_markers_for_row_v1(d, QuestStateV1::available, nullptr, 1, m);
    check(m.size() == 1 && !m[0].secondary, "primary variant");
    std::printf("quest_npc_marker_v1 OK\n");
}
