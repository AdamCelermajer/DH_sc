#include "runtime_skill_activation_v1.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
using Bytes = std::vector<std::uint8_t>;
Bytes read(const std::string& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("missing source table: " + path);
    return {std::istreambuf_iterator<char>(stream), {}};
}
void check(bool value, int line) {
    if (!value) throw std::runtime_error("runtime skill activation test failed at line " +
                                         std::to_string(line));
}
#define CHECK(value) check((value), __LINE__)

bool same_state(const CharacterState& a, const CharacterState& b) {
    if (a.class_id != b.class_id || a.stats.level != b.stats.level ||
        a.stats.resource != b.stats.resource || a.source_skill_points != b.source_skill_points ||
        a.skills.size() != b.skills.size() || a.skill_slots.size() != b.skill_slots.size()) return false;
    for (std::size_t i = 0; i < a.skills.size(); ++i)
        if (a.skills[i].id != b.skills[i].id || a.skills[i].rank != b.skills[i].rank) return false;
    for (std::size_t i = 0; i < a.skill_slots.size(); ++i)
        if (a.skill_slots[i].equipment_set != b.skill_slots[i].equipment_set ||
            a.skill_slots[i].slot != b.skill_slots[i].slot ||
            a.skill_slots[i].saved_skill_row != b.skill_slots[i].saved_skill_row) return false;
    return true;
}
} // namespace

int main(int argc, char** argv) {
    try {
        CHECK(argc == 2);
        const std::string root = argv[1];
        auto skill_data = read(root + "/original-cache/data/pydata/skills_pyarray.bin");
        auto skill_names = read(root + "/original-cache/data/pydata/skills_pyarraynames.bin");
        auto skill_schema = read(root + "/original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        std::string error;
        CHECK(skill_owner.load({skill_data.data(), skill_data.size()},
                               {skill_names.data(), skill_names.size()},
                               {skill_schema.data(), skill_schema.size()}, error));
        const auto tables = skill_owner.borrow();

        auto character_data = read(root + "/original-cache/data/pydata/character_properties_pyarray.bin");
        auto character_names = read(root + "/original-cache/data/pydata/character_properties_pyarraynames.bin");
        auto character_schema = read(root + "/original-cache/data/pydata/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        CHECK(dh2::data::load_characters({character_data.data(), character_data.size()},
                                         {character_names.data(), character_names.size()},
                                         {character_schema.data(), character_schema.size()},
                                         characters, error));

        const auto class_it = std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase");
        const auto tree_it = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        CHECK(class_it != characters.names.end() && tree_it != characters.fields.end());
        const auto class_row = static_cast<std::size_t>(class_it - characters.names.begin());
        const auto tree_column = static_cast<std::size_t>(tree_it - characters.fields.begin());
        const int authored_list = characters.rows[class_row][tree_column];
        CHECK(authored_list >= 0 && static_cast<std::size_t>(authored_list) < tables.lists().size());
        CHECK(tables.lists()[static_cast<std::size_t>(authored_list)].size() > 0);

        CharacterState state;
        state.id = "activation-state-fixture";
        state.class_id = "KnightPlayerBase";
        state.stats.level = 1;
        state.stats.resource = 0; // Generic resource is not silently treated as native MP.
        state.source_skill_slots_known = true;
        for (const auto id : tables.lists()[static_cast<std::size_t>(authored_list)]) {
            CHECK(id >= 0 && static_cast<std::size_t>(id) < tables.skill_names().size());
            state.skills.push_back({tables.skill_names()[static_cast<std::size_t>(id)], 1});
        }

        SkillVisualRequestV1 request;
        CHECK(resolve_skill_visual_request_v1(state, characters, tables, 0, request, error));
        CHECK(request.class_skill_position == 0 && request.active_skill_list_id == authored_list);
        CHECK(request.skill_table_id == 7 && request.saved_skill_row == 0 && request.saved_rank == 1);
        CHECK(request.animation_sequence_id == 347 && request.source_skill_name == "BashDown");
        CHECK(request.source_script == "prince_warrior_bashdown");

        const auto before = state;
        CharacterState unlearned = state;
        unlearned.skills[0].rank = 0;
        CHECK(!resolve_skill_visual_request_v1(unlearned, characters, tables, 0, request, error));
        CHECK(error.find("learned source skill") != std::string::npos);
        CharacterState rank_unknown = state;
        rank_unknown.skills.clear();
        rank_unknown.source_skill_slots_known = false;
        CHECK(!resolve_skill_visual_request_v1(rank_unknown, characters, tables, 0, request, error));
        CHECK(error.find("exact saved CharacterState rank") != std::string::npos);
        CHECK(!resolve_skill_visual_request_v1(state, characters, tables, 999, request, error));
        CHECK(same_state(state, before));

        SkillCastAnimationOutcomeV1 outcome{};
        SkillCastAdmissionFactV1 unknown;
        CHECK(resolve_skill_cast_admission_v1(unknown, outcome, error));
        CHECK(outcome == SkillCastAnimationOutcomeV1::source_admission_unknown && error.empty());
        SkillCastAdmissionFactV1 rejected{SkillCastAdmissionStatusV1::rejected,
                                          "source OnSkillCheck", "HasMana returned false"};
        CHECK(resolve_skill_cast_admission_v1(rejected, outcome, error));
        CHECK(outcome == SkillCastAnimationOutcomeV1::source_rejected && error.empty());
        SkillCastAdmissionFactV1 accepted{SkillCastAdmissionStatusV1::accepted,
                                          "source OnSkillCheck", {}};
        CHECK(resolve_skill_cast_admission_v1(accepted, outcome, error));
        CHECK(outcome == SkillCastAnimationOutcomeV1::source_admission_accepted && error.empty());
        accepted.actor = 17;
        accepted.character_state_id = request.character_state_id;
        accepted.class_id = request.class_id;
        accepted.active_skill_list_id = request.active_skill_list_id;
        accepted.class_skill_position = request.class_skill_position;
        accepted.skill_table_id = request.skill_table_id;
        accepted.saved_rank = request.saved_rank;
        CHECK(validate_skill_cast_admission_binding_v1(request, accepted, 17, error));
        auto foreign_admission = accepted;
        ++foreign_admission.saved_rank;
        CHECK(!validate_skill_cast_admission_binding_v1(request, foreign_admission, 17, error));
        CHECK(error.find("does not match") != std::string::npos);
        SkillCastAdmissionFactV1 forged{SkillCastAdmissionStatusV1::accepted, {}, {}};
        CHECK(!resolve_skill_cast_admission_v1(forged, outcome, error));
        CHECK(error.find("named source admission provider") != std::string::npos);
        CHECK(same_state(state, before));

        std::cout << "PASS source SkillTree position0 -> BashDown row7/root347; learned-rank gates; "
                     "unknown/rejected source cast admission stays non-playing; no state mutation; "
                     "native gameplay admission remains external\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
