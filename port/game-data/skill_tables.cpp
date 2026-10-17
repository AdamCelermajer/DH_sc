#include "skill_tables.hpp"

#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>

namespace dh2::data {
namespace {
constexpr std::size_t kMaximumInput = 8u * 1024u * 1024u;
constexpr std::uint32_t kMaximumRows = 4096;
constexpr std::uint32_t kMaximumStringBytes = 1u * 1024u * 1024u;

struct Reader {
    Bytes bytes;
    std::size_t offset = 0;

    explicit Reader(Bytes input) : bytes(input) {
        if (!input.data || input.size < 4 || input.size > kMaximumInput)
            throw std::runtime_error("Skill/faery input outside size limit");
    }

    void require(std::size_t amount) const {
        if (offset > bytes.size || amount > bytes.size - offset)
            throw std::runtime_error("Truncated skill/faery table");
    }

    std::uint32_t word() {
        require(4);
        const auto* p = bytes.data + offset;
        offset += 4;
        return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
               (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
    }

    std::int32_t integer() {
        const auto bits = word();
        std::int32_t value = 0;
        static_assert(sizeof(value) == sizeof(bits), "cache integer is 32 bits");
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    }

    bool boolean() {
        require(1);
        const auto value = bytes.data[offset++];
        if (value > 1) throw std::runtime_error("Invalid serialized skill boolean");
        return value != 0;
    }

    std::uint32_t count() {
        const auto value = word();
        if (value > kMaximumRows) throw std::runtime_error("Skill/faery row count outside limit");
        return value;
    }

    std::string raw_string() {
        const auto length = word();
        if (length > kMaximumStringBytes)
            throw std::runtime_error("Skill/faery string outside size limit");
        return raw_bytes(length);
    }

    std::string raw_bytes(std::size_t length) {
        if (length > kMaximumStringBytes)
            throw std::runtime_error("Skill/faery string outside size limit");
        require(length);
        std::string result(reinterpret_cast<const char*>(bytes.data + offset), length);
        offset += length;
        return result;
    }

    std::vector<std::string> names() {
        const auto size = count();
        if (size == 0) throw std::runtime_error("Empty skill/faery name section");
        std::vector<std::string> result;
        result.reserve(size);
        std::set<std::string> seen;
        for (std::uint32_t n = 0; n < size; ++n) {
            auto value = raw_string();
            if (value.empty() || !seen.insert(value).second)
                throw std::runtime_error("Empty or duplicate skill/faery table name");
            for (const unsigned char ch : value)
                if (ch < 0x21 || ch > 0x7e)
                    throw std::runtime_error("Non-ASCII skill/faery table identifier");
            result.push_back(std::move(value));
        }
        return result;
    }

    std::vector<std::vector<std::string>> sections() {
        std::vector<std::vector<std::string>> result;
        while (offset != bytes.size) result.push_back(names());
        return result;
    }

    std::vector<std::int32_t> integer_vector() {
        const auto size = count();
        if (size > (bytes.size - offset) / 4)
            throw std::runtime_error("Truncated skill/faery integer list");
        std::vector<std::int32_t> result;
        result.reserve(size);
        for (std::uint32_t n = 0; n < size; ++n) result.push_back(integer());
        return result;
    }

    void finish() const {
        if (offset != bytes.size) throw std::runtime_error("Unexpected skill/faery table suffix");
    }
};

void require_names(const std::vector<std::string>& actual,
                   const std::vector<std::string>& expected, const char* what) {
    if (actual != expected) throw std::runtime_error(what);
}

std::vector<IntegerListRow> read_lists(Reader& reader,
                                      const std::vector<std::string>& names) {
    if (reader.count() != names.size()) throw std::runtime_error("Skill/faery list count differs from names");
    std::vector<IntegerListRow> result;
    result.reserve(names.size());
    for (const auto& name : names)
        result.push_back(IntegerListRow{name, reader.integer_vector()});
    return result;
}

std::vector<SkillRow> read_skills(Reader& reader,
                                  const std::vector<std::string>& names) {
    if (reader.count() != names.size()) throw std::runtime_error("Skill count differs from names");
    std::vector<SkillRow> result;
    result.reserve(names.size());
    for (const auto& name : names) {
        SkillRow row{};
        row.table_name = name;
        row.anim = reader.integer();
        row.anim_is_moving = reader.boolean();
        row.display_props = reader.integer_vector();
        row.elemental_type = reader.integer();
        row.fairie_dependant_text = reader.boolean();
        row.flags = reader.integer();
        row.level = reader.integer();
        row.script_length = reader.integer();
        if (row.script_length < 0 || static_cast<std::uint32_t>(row.script_length) > kMaximumStringBytes)
            throw std::runtime_error("Invalid serialized skill Script length");
        row.script = reader.raw_bytes(static_cast<std::size_t>(row.script_length));
        if (row.script.size() != static_cast<std::size_t>(row.script_length))
            throw std::runtime_error("Skill Script length differs from serialized bytes");
        row.skill_assignable = reader.boolean();
        row.skill_curr_level = reader.integer();
        row.skill_description = reader.integer();
        row.skill_icon_length = reader.integer();
        if (row.skill_icon_length < 0 || static_cast<std::uint32_t>(row.skill_icon_length) > kMaximumStringBytes)
            throw std::runtime_error("Invalid serialized skill SkillIcon length");
        row.skill_icon = reader.raw_bytes(static_cast<std::size_t>(row.skill_icon_length));
        if (row.skill_icon.size() != static_cast<std::size_t>(row.skill_icon_length))
            throw std::runtime_error("SkillIcon length differs from serialized bytes");
        row.skill_name = reader.integer();
        row.skill_next_level = reader.integer();
        row.type = reader.integer();
        result.push_back(std::move(row));
    }
    return result;
}

std::vector<FaeryRow> read_faeries(Reader& reader,
                                   const std::vector<std::string>& names) {
    if (reader.count() != names.size()) throw std::runtime_error("Faery count differs from names");
    std::vector<FaeryRow> result;
    result.reserve(names.size());
    for (const auto& name : names) {
        FaeryRow row{};
        row.table_name = name;
        row.description = reader.integer();
        row.elemental = reader.integer();
        row.model_file = reader.integer();
        row.name_id = reader.integer();
        row.spell_script_length = reader.integer();
        if (row.spell_script_length < 0 ||
            static_cast<std::uint32_t>(row.spell_script_length) > kMaximumStringBytes)
            throw std::runtime_error("Invalid serialized Faery SpellScript length");
        row.spell_script = reader.raw_bytes(static_cast<std::size_t>(row.spell_script_length));
        if (row.spell_script.size() != static_cast<std::size_t>(row.spell_script_length))
            throw std::runtime_error("Faery SpellScript length differs from serialized bytes");
        row.spell_type = reader.integer();
        row.type = reader.integer();
        result.push_back(std::move(row));
    }
    return result;
}
}  // namespace

bool load_skill_tables(Bytes records, Bytes names, Bytes schema,
                       SkillTables& output, std::string& error) {
    error.clear();
    try {
        Reader data(records), keys(names), layout(schema);
        const auto name_sections = keys.sections();
        const auto schema_sections = layout.sections();
        keys.finish();
        layout.finish();
        if (name_sections.size() != 2 || schema_sections.size() != 2)
            throw std::runtime_error("Unexpected skill table section count");
        require_names(schema_sections[0], {"List"}, "SkillList schema differs");
        require_names(schema_sections[1],
            {"Anim", "AnimIsMoving", "DisplayProps", "ElementalType", "FairieDependantText",
             "Flags", "Level", "Script", "SkillAssignable", "SkillCurrLevel", "SkillDescription",
             "SkillIcon", "SkillName", "SkillNextLevel", "Type"}, "Skill schema differs");

        SkillTables next;
        next.skill_lists = read_lists(data, name_sections[0]);
        next.skills = read_skills(data, name_sections[1]);
        data.finish();
        output = std::move(next);
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

bool load_faery_tables(Bytes records, Bytes names, Bytes schema,
                       FaeryTables& output, std::string& error) {
    error.clear();
    try {
        Reader data(records), keys(names), layout(schema);
        const auto name_sections = keys.sections();
        const auto schema_sections = layout.sections();
        keys.finish();
        layout.finish();
        if (name_sections.size() != 2 || schema_sections.size() != 3)
            throw std::runtime_error("Unexpected faery table section count");
        require_names(schema_sections[0],
            {"Description", "Elemental", "ModelFile", "Name", "SpellScript", "SpellType", "Type"},
            "Faery schema differs");
        require_names(schema_sections[1], {"List"}, "FaeryList schema differs");
        require_names(schema_sections[2], {"List"}, "FaerySpellList schema differs");

        FaeryTables next;
        next.faery_lists = read_lists(data, name_sections[0]);
        next.faeries = read_faeries(data, name_sections[1]);
        data.finish();
        output = std::move(next);
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

}  // namespace dh2::data
