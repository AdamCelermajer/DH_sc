#pragma once
#include "runtime_quest_menu_v1.hpp"
namespace dh::foundation {
struct RuntimeQuestMenuSourceArtV1 {
    std::vector<HudGeometryBatch> page_art;
    std::vector<character_menu::MenuSolidBatch> page_solids;
    std::vector<character_menu::MenuTextField> page_fields;
    std::array<std::vector<HudGeometryBatch>,2> row_art;
    std::array<std::vector<character_menu::MenuSolidBatch>,2> row_solids;
    std::array<std::vector<character_menu::MenuTextField>,2> row_fields;
    std::vector<HudGeometryBatch> current_marker_art;
    std::vector<character_menu::MenuSolidBatch> current_marker_solids;
    std::vector<HudGeometryBatch> activate_art;
    std::vector<character_menu::MenuSolidBatch> activate_solids;
    std::vector<character_menu::MenuTextField> activate_fields;
    std::array<std::array<float,6>,2> row_parent_matrices;
};
const RuntimeQuestMenuSourceArtV1& original_runtime_quest_menu_art_v1();
}
