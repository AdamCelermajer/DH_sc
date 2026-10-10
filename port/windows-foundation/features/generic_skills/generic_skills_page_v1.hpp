#pragma once

#include "../../character_state.hpp"
#include "../character_menu/skill_page_text_projection_v1.hpp"
#include "../../../game-data/data.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace dh::foundation::generic_skills {

// A portable projection of the authored Skills page. CharacterState is the
// only progression/slot owner; the table borrows supply authored list order,
// SkillTable IDs, icon keys and requirements. Localized prose and progression
// policy remain source-provider responsibilities.
struct RowV1 {
    int position = 0;       // index in the active source SkillList
    int table_id = -1;       // global original SkillTable row
    std::string source_name; // original skill dictionary token, not localized text
    std::string source_icon; // original SkillIcon key
    int required_level = 0;  // original SkillTable word 8, interpreted signed
    std::optional<std::uint32_t> saved_rank;
    std::optional<std::uint32_t> saved_skill_row;
    std::optional<std::string> localized_name;
    std::optional<std::string> description;
};

struct SlotV1 {
    unsigned equipment_set = 0;
    unsigned slot = 0;
    std::optional<unsigned> saved_skill_row;
    std::optional<int> class_skill_position;
};

struct ViewV1 {
    int character_row = -1;
    int authored_skill_list_id = -1;
    int active_skill_list_id = -1;
    bool active_list_from_saved_rows = false;
    bool skill_points_known = false;
    std::optional<std::uint32_t> skill_points;
    bool slots_source_known = false;
    std::vector<RowV1> rows;
    std::vector<SlotV1> slots;
};

struct ServicesV1 {
    // Returns the original StringManager/localized projections for one real
    // source SkillTable row. No fabricated English fallback is supplied.
    std::function<bool(const CharacterState&, int table_id,
                       std::optional<std::string>& localized_name,
                       std::optional<std::string>& description,
                       std::string& error)> localized_text;

    // Character::IsSkillEquippable has owner-dependent availability in
    // addition to SkillAssignable/rank checks. This provider must evaluate
    // that source availability against the same CharacterState.
    std::function<bool(const CharacterState&, int table_id, int position,
                       bool& available, std::string& error)> can_assign;

    // Native progression policy (difficulty, level caps, source points and
    // CanIncrementSkill) is not inferred here. The provider must mutate this
    // exact shared state, or return false without changing it.
    std::function<bool(CharacterState&, int list_id, int position, int table_id,
                       std::string& error)> train;

    // Optional same-native-owner NativeGetSkillDetails result for the two
    // authored current/next fields. The provider must return exact source
    // SkillCurrLevel/SkillNextLevel ParseEx strings and echo request identity;
    // UI code never derives those values from a formula or static description.
    std::function<bool(const CharacterState&,
                       const character_menu::SkillPageCurrentNextRequestV1&,
                       character_menu::SkillPageCurrentNextTextV1&,
                       std::string& error)> skill_level_text;
};

class PageV1 {
    CharacterState* state_ = nullptr;
    const dh2::data::CharacterTable* characters_ = nullptr;
    dh2::data::SkillTables::Borrow tables_;
    ServicesV1 services_;
    std::optional<int> selected_position_;
    std::optional<int> selected_list_;

    bool resolve(ViewV1&, std::string&) const;
public:
    PageV1(CharacterState& state, const dh2::data::CharacterTable& characters,
           dh2::data::SkillTables::Borrow tables, ServicesV1 services = {});

    bool view(ViewV1&, std::string&) const;
    bool select(int class_skill_position, std::string&);
    std::optional<int> selected_position() const noexcept { return selected_position_; }
    bool assign(unsigned equipment_set, unsigned source_slot, std::string&);
    bool train(std::string&);
};

} // namespace dh::foundation::generic_skills
