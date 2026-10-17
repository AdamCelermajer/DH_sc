#pragma once

#include "data.hpp"

namespace dh2::data {

struct IntegerListRow {
    std::string name;
    std::vector<std::int32_t> members;
};

// Owned projection of the serialized Skill record. `script_length` and
// `skill_icon_length` preserve the source length words independently from
// their host-owned byte strings. No ARM32 pointer or object layout is exposed.
struct SkillRow {
    std::string table_name;
    std::int32_t anim = 0;
    bool anim_is_moving = false;
    std::vector<std::int32_t> display_props;
    std::int32_t elemental_type = 0;
    bool fairie_dependant_text = false;
    std::int32_t flags = 0;
    std::int32_t level = 0;
    std::int32_t script_length = 0;
    std::string script;
    bool skill_assignable = false;
    std::int32_t skill_curr_level = 0;
    std::int32_t skill_description = 0;
    std::int32_t skill_icon_length = 0;
    std::string skill_icon;
    std::int32_t skill_name = 0;
    std::int32_t skill_next_level = 0;
    std::int32_t type = 0;
};

// Serialized `Name` is a signed source selector, not a pointer. The row's
// readable table name is kept separately as `table_name`.
struct FaeryRow {
    std::string table_name;
    std::int32_t description = 0;
    std::int32_t elemental = 0;
    std::int32_t model_file = 0;
    std::int32_t name_id = 0;
    std::int32_t spell_script_length = 0;
    std::string spell_script;
    std::int32_t spell_type = 0;
    std::int32_t type = 0;
};

struct SkillTables {
    std::vector<IntegerListRow> skill_lists;
    std::vector<SkillRow> skills;
};

struct FaeryTables {
    std::vector<IntegerListRow> faery_lists;
    std::vector<FaeryRow> faeries;
};

// Decode each table's actual cache record, parallel names sections and schema
// sections. Loads are transactional: malformed input leaves `output` intact.
bool load_skill_tables(Bytes records, Bytes names, Bytes schema,
                       SkillTables& output, std::string& error);
bool load_faery_tables(Bytes records, Bytes names, Bytes schema,
                       FaeryTables& output, std::string& error);

}  // namespace dh2::data
