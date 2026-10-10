#include "pc_skill_hud_projection_v1.hpp"
#include "runtime_skill_cast_prepare_v1.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
using Raw = std::vector<std::uint8_t>;
Raw read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("missing original HUD source table: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        require(argc == 2, "source SkillTables asset directory argument required");
        const std::string root = argv[1];
        auto skill_data = read(root + "/skills_pyarray.bin");
        auto skill_names = read(root + "/skills_pyarraynames.bin");
        auto skill_schema = read(root + "/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        std::string error;
        require(skill_owner.load({skill_data.data(), skill_data.size()},
                                 {skill_names.data(), skill_names.size()},
                                 {skill_schema.data(), skill_schema.size()}, error),
                error.c_str());
        const auto skills = skill_owner.borrow();

        auto character_data = read(root + "/character_properties_pyarray.bin");
        auto character_names = read(root + "/character_properties_pyarraynames.bin");
        auto character_schema = read(root + "/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        require(dh2::data::load_characters({character_data.data(), character_data.size()},
                                           {character_names.data(), character_names.size()},
                                           {character_schema.data(), character_schema.size()},
                                           characters, error), error.c_str());
        const auto class_row = std::find(characters.names.begin(), characters.names.end(),
                                         "RoguePlayerBase");
        const auto tree_field = std::find(characters.fields.begin(), characters.fields.end(),
                                          "SkillTree");
        require(class_row != characters.names.end() && tree_field != characters.fields.end(),
                "Original Rogue class row/SkillTree field missing");
        const int list_id = characters.rows[static_cast<std::size_t>(class_row - characters.names.begin())]
                                         [static_cast<std::size_t>(tree_field - characters.fields.begin())];
        require(list_id >= 0 && static_cast<std::size_t>(list_id) < skills.lists().size(),
                "Original Rogue SkillTree does not resolve to SkillTables");

        CharacterState state;
        state.id = "same-source-pc-hud-player";
        state.class_id = "RoguePlayerBase";
        state.source_skill_slots_known = true;
        const auto& source_list = skills.lists()[static_cast<std::size_t>(list_id)];
        require(!source_list.empty(), "Original Rogue SkillList is empty");
        int jumpkick_id = source_list.front();
        require(jumpkick_id >= 0 && static_cast<std::size_t>(jumpkick_id) < skills.skill_names().size(),
                "Original Rogue slot0 SkillTable ID is invalid");
        require(skills.skill_names()[static_cast<std::size_t>(jumpkick_id)] == "JumpKick",
                "Original Rogue source slot0 is no longer JumpKick");
        for (const auto table_id : source_list) {
            require(table_id >= 0 && static_cast<std::size_t>(table_id) < skills.skill_names().size(),
                    "Original Rogue SkillList contains an invalid SkillTable row");
            state.skills.push_back({skills.skill_names()[static_cast<std::size_t>(table_id)], 0});
        }
        state.skills[0].rank = 1;
        state.skill_slots = {{0, 0, 0}};
        const auto saved_slots_before = state.skill_slots;
        const auto saved_rows_before = state.skills;

        const std::array<PcSkillHudSourceStatusV1, 3> runtime_status{{
            {0, 12, true}, {1, 0, false}, {2, std::nullopt, std::nullopt}}};
        RuntimeSkillCastReceiptV1 cast;
        cast.actor = 51;
        cast.generation = 7;
        cast.phase = RuntimeSkillCastPhaseV1::prepared_pending_use;
        cast.equipment_set = 0;
        cast.source_slot = 0;
        cast.saved_skill_row = 0;
        PcSkillHudFrameV1 frame;
        require(project_pc_skill_hud_v1(state, 51, characters, skills, 0,
                    runtime_status, &cast, frame, error), error.c_str());

        require(frame.source_actor == 51 && frame.character_state_id == state.id &&
                frame.equipment_set == 0,
                "PC HUD frame lost the same source actor/state/set identity");
        const auto& left = frame.left_middle_right[0];
        const auto& middle = frame.left_middle_right[1];
        const auto& right = frame.left_middle_right[2];
        require(left.physical_position == 0 && left.pc_key_number == 1 &&
                left.key_label == "1" && left.source_slot == 2 && !left.assigned,
                "Physical left HUD cell did not project key1/logical source slot2");
        require(middle.physical_position == 1 && middle.pc_key_number == 2 &&
                middle.key_label == "2" && middle.source_slot == 0 && middle.assigned &&
                middle.saved_skill_row == 0 && middle.saved_rank == 1 &&
                middle.source_skill_name == "JumpKick" &&
                middle.source_icon_key == skills.skills()[static_cast<std::size_t>(jumpkick_id)].icon &&
                middle.source_cooldown_frame == 12 && middle.source_usable == true &&
                middle.cast_in_progress && middle.cast_generation == 7,
                "Physical middle HUD cell did not show the actual key2/source-slot0 JumpKick source state");
        require(right.physical_position == 2 && right.pc_key_number == 3 &&
                right.key_label == "3" && right.source_slot == 1 && !right.assigned &&
                right.source_cooldown_frame == 0 && right.source_usable == false,
                "Physical right HUD cell did not project key3/logical source slot1");

        for (std::uint32_t physical = 0; physical < 3; ++physical) {
            std::uint32_t key_number = 99;
            require(pc_skill_hud_key_for_hit_v1(frame, physical, key_number, error), error.c_str());
            require(key_number == physical + 1,
                    "Confirmed physical HUD shape hit did not emit its matching PC key number");
            std::uint32_t source_slot = 99;
            require(pc_skill_number_to_source_slot_v1(key_number, source_slot, error), error.c_str());
            require(source_slot == frame.left_middle_right[physical].source_slot,
                    "HUD hit plus the single production pre-cast mapping selects a different source cell");
        }
        std::uint32_t key = 99;
        require(!pc_skill_hud_key_for_hit_v1(frame, 3, key, error) && key == 99,
                "Out-of-layout HUD hit fabricated a PC key number");
        require(state.skill_slots.size() == saved_slots_before.size() &&
                state.skill_slots[0].equipment_set == saved_slots_before[0].equipment_set &&
                state.skill_slots[0].slot == saved_slots_before[0].slot &&
                state.skill_slots[0].saved_skill_row == saved_slots_before[0].saved_skill_row &&
                state.skills[0].id == saved_rows_before[0].id &&
                state.skills[0].rank == saved_rows_before[0].rank,
                "HUD projection mutated the current saved assignment or rank");

        RuntimeSkillCastReceiptV1 foreign_cast = cast;
        foreign_cast.actor = 52;
        PcSkillHudFrameV1 unchanged;
        unchanged.source_actor = 123;
        require(!project_pc_skill_hud_v1(state, 51, characters, skills, 0,
                    runtime_status, &foreign_cast, unchanged, error) &&
                unchanged.source_actor == 123,
                "Foreign coordinator receipt was accepted or partially replaced HUD frame");
        std::cout << "pc_skill_hud_projection_v1_tests PASS: actual Rogue SkillTables row/icon; "
                     "left/middle/right labels1/2/3 -> source slots2/0/1; middle key2 JumpKick; "
                     "shape hits emit keys once; saved rows unchanged\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
