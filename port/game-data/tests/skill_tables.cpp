#include "../skill_tables.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace data = dh2::data;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read_file(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    require(bool(stream), "cannot open cache input");
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}

std::uint32_t read_word(const std::vector<std::uint8_t>& value, std::size_t offset) {
    require(offset <= value.size() && value.size() - offset >= 4, "test cache offset outside input");
    return std::uint32_t(value[offset]) | (std::uint32_t(value[offset + 1]) << 8) |
           (std::uint32_t(value[offset + 2]) << 16) | (std::uint32_t(value[offset + 3]) << 24);
}

void quote(const std::string& value) {
    const char hex[] = "0123456789abcdef";
    std::cout << '"';
    for (const unsigned char ch : value) {
        if (ch == '"' || ch == '\\') std::cout << '\\' << static_cast<char>(ch);
        else if (ch < 0x20) std::cout << "\\u00" << hex[ch >> 4] << hex[ch & 15];
        else std::cout << static_cast<char>(ch);
    }
    std::cout << '"';
}

template<class Load, class Table, class NameOf>
void reject_preserving(Load load, const std::vector<std::uint8_t>& records,
                       data::Bytes names, data::Bytes schema, Table& table,
                       const std::string& retained_name, const NameOf& get_name,
                       unsigned& rejected) {
    std::string error;
    require(!load(bytes(records), names, schema, table, error) && !error.empty(),
            "malformed cache accepted");
    require(get_name(table) == retained_name,
            "failed load replaced existing owned table");
    ++rejected;
}

int main(int argc, char** argv) {
    try {
        require(argc == 7, "pass skill records/names/schema and faery records/names/schema");
        const auto skill_records = read_file(argv[1]);
        const auto skill_names = read_file(argv[2]);
        const auto skill_schema = read_file(argv[3]);
        const auto faery_records = read_file(argv[4]);
        const auto faery_names = read_file(argv[5]);
        const auto faery_schema = read_file(argv[6]);

        data::SkillTables skills;
        data::FaeryTables faeries;
        std::string error;
        require(data::load_skill_tables(bytes(skill_records), bytes(skill_names), bytes(skill_schema), skills, error),
                error.c_str());
        require(data::load_faery_tables(bytes(faery_records), bytes(faery_names), bytes(faery_schema), faeries, error),
                error.c_str());
        require(skills.skill_lists.size() == 36 && skills.skills.size() == 127 &&
                faeries.faery_lists.size() == 4 && faeries.faeries.size() == 16,
                "cache table dimensions differ");
        require(skills.skill_lists[3].name == "DEFAULT" && skills.skill_lists[3].members.empty(),
                "authored empty DEFAULT skill list differs");
        require(faeries.faery_lists[0].name == "DEFAULT" &&
                faeries.faery_lists[0].members == std::vector<std::int32_t>{2, 4, 5, 6, 3},
                "authored DEFAULT faery list differs");
        const std::vector<std::string> fake_names = {
            "Fake_Celest", "Fake_Rocky", "Fake_Wetty", "Fake_Windy", "Fake_Hotty"};
        for (std::size_t i = 0; i < fake_names.size(); ++i) {
            const auto index = static_cast<std::size_t>(faeries.faery_lists[0].members[i]);
            require(faeries.faeries[index].table_name == fake_names[i] &&
                    faeries.faeries[index].spell_script_length == 0 &&
                    faeries.faeries[index].spell_script.empty(),
                    "DEFAULT faery null-script gate differs");
        }

        unsigned rejection_cases = 0;
        const auto skill_name = skills.skills.front().table_name;
        const auto faery_name = faeries.faeries.front().table_name;
        const auto first_skill = [](const data::SkillTables& table) {
            return table.skills.empty() ? std::string{} : table.skills.front().table_name;
        };
        const auto first_faery = [](const data::FaeryTables& table) {
            return table.faeries.empty() ? std::string{} : table.faeries.front().table_name;
        };
        auto bad = skill_records;
        reject_preserving(data::load_skill_tables, std::vector<std::uint8_t>{},
                          bytes(skill_names), bytes(skill_schema), skills, skill_name, first_skill, rejection_cases);
        for (const auto length : {3u, 4u, static_cast<unsigned>(skill_records.size() - 1)}) {
            std::vector<std::uint8_t> truncated(skill_records.begin(), skill_records.begin() + length);
            reject_preserving(data::load_skill_tables, truncated, bytes(skill_names), bytes(skill_schema),
                              skills, skill_name, first_skill, rejection_cases);
        }
        bad.push_back(0);
        reject_preserving(data::load_skill_tables, bad, bytes(skill_names), bytes(skill_schema),
                          skills, skill_name, first_skill, rejection_cases);
        auto wrong_skill_schema = skill_schema;
        wrong_skill_schema.back() ^= 1;
        reject_preserving(data::load_skill_tables, skill_records, bytes(skill_names), bytes(wrong_skill_schema),
                          skills, skill_name, first_skill, rejection_cases);
        auto wrong_skill_count = skill_records;
        wrong_skill_count[0] = 0xff;
        wrong_skill_count[1] = 0xff;
        reject_preserving(data::load_skill_tables, wrong_skill_count, bytes(skill_names), bytes(skill_schema),
                          skills, skill_name, first_skill, rejection_cases);
        std::size_t first_skill_offset = 4;
        for (std::size_t i = 0; i < skills.skill_lists.size(); ++i)
            first_skill_offset += 4u + std::size_t(read_word(skill_records, first_skill_offset)) * 4u;
        first_skill_offset += 4; // SkillTable row count
        auto invalid_skill_bool = skill_records;
        invalid_skill_bool[first_skill_offset + 4] = 2;
        reject_preserving(data::load_skill_tables, invalid_skill_bool, bytes(skill_names), bytes(skill_schema),
                          skills, skill_name, first_skill, rejection_cases);
        const auto first_display_count = read_word(skill_records, first_skill_offset + 5);
        const auto script_length_offset = first_skill_offset + 22u + std::size_t(first_display_count) * 4u;
        auto invalid_script_length = skill_records;
        invalid_script_length[script_length_offset] = 0xff;
        invalid_script_length[script_length_offset + 1] = 0xff;
        invalid_script_length[script_length_offset + 2] = 0xff;
        invalid_script_length[script_length_offset + 3] = 0xff;
        reject_preserving(data::load_skill_tables, invalid_script_length, bytes(skill_names), bytes(skill_schema),
                          skills, skill_name, first_skill, rejection_cases);

        auto wrong_faery_schema = faery_schema;
        wrong_faery_schema.back() ^= 1;
        reject_preserving(data::load_faery_tables, faery_records, bytes(faery_names), bytes(wrong_faery_schema),
                          faeries, faery_name, first_faery, rejection_cases);
        auto truncated_faery = faery_records;
        truncated_faery.pop_back();
        reject_preserving(data::load_faery_tables, truncated_faery, bytes(faery_names), bytes(faery_schema),
                          faeries, faery_name, first_faery, rejection_cases);
        auto suffixed_faery = faery_records;
        suffixed_faery.push_back(0);
        reject_preserving(data::load_faery_tables, suffixed_faery, bytes(faery_names), bytes(faery_schema),
                          faeries, faery_name, first_faery, rejection_cases);
        auto invalid_faery_count = faery_records;
        invalid_faery_count[0] = 0xff;
        invalid_faery_count[1] = 0xff;
        reject_preserving(data::load_faery_tables, invalid_faery_count, bytes(faery_names), bytes(faery_schema),
                          faeries, faery_name, first_faery, rejection_cases);
        std::size_t first_faery_offset = 4;
        for (std::size_t i = 0; i < faeries.faery_lists.size(); ++i)
            first_faery_offset += 4u + std::size_t(read_word(faery_records, first_faery_offset)) * 4u;
        first_faery_offset += 4; // FaeryTable row count
        auto invalid_faery_length = faery_records;
        for (unsigned i = 0; i < 4; ++i) invalid_faery_length[first_faery_offset + 16 + i] = 0xff;
        reject_preserving(data::load_faery_tables, invalid_faery_length, bytes(faery_names), bytes(faery_schema),
                          faeries, faery_name, first_faery, rejection_cases);

        std::cout << "{\"validation\":\"PASS\",\"rejection_cases\":" << rejection_cases
                  << ",\"skill_lists\":[";
        for (std::size_t i = 0; i < skills.skill_lists.size(); ++i) {
            if (i) std::cout << ',';
            const auto& row = skills.skill_lists[i];
            std::cout << "{\"name\":"; quote(row.name); std::cout << ",\"members\":[";
            for (std::size_t j = 0; j < row.members.size(); ++j) {
                if (j) std::cout << ',';
                std::cout << row.members[j];
            }
            std::cout << "]}";
        }
        std::cout << "],\"skills\":[";
        for (std::size_t i = 0; i < skills.skills.size(); ++i) {
            if (i) std::cout << ',';
            const auto& row = skills.skills[i];
            std::cout << "{\"table_name\":"; quote(row.table_name);
            std::cout << ",\"anim\":" << row.anim << ",\"anim_is_moving\":" << (row.anim_is_moving ? "true" : "false")
                      << ",\"display_props\":[";
            for (std::size_t j = 0; j < row.display_props.size(); ++j) {
                if (j) std::cout << ',';
                std::cout << row.display_props[j];
            }
            std::cout << "],\"elemental_type\":" << row.elemental_type
                      << ",\"fairie_dependant_text\":" << (row.fairie_dependant_text ? "true" : "false")
                      << ",\"flags\":" << row.flags << ",\"level\":" << row.level
                      << ",\"script_length\":" << row.script_length << ",\"script\":";
            quote(row.script);
            std::cout << ",\"skill_assignable\":" << (row.skill_assignable ? "true" : "false")
                      << ",\"skill_curr_level\":" << row.skill_curr_level
                      << ",\"skill_description\":" << row.skill_description
                      << ",\"skill_icon_length\":" << row.skill_icon_length << ",\"skill_icon\":";
            quote(row.skill_icon);
            std::cout << ",\"skill_name\":" << row.skill_name << ",\"skill_next_level\":" << row.skill_next_level
                      << ",\"type\":" << row.type << '}';
        }
        std::cout << "],\"faery_lists\":[";
        for (std::size_t i = 0; i < faeries.faery_lists.size(); ++i) {
            if (i) std::cout << ',';
            const auto& row = faeries.faery_lists[i];
            std::cout << "{\"name\":"; quote(row.name); std::cout << ",\"members\":[";
            for (std::size_t j = 0; j < row.members.size(); ++j) {
                if (j) std::cout << ',';
                std::cout << row.members[j];
            }
            std::cout << "]}";
        }
        std::cout << "],\"faeries\":[";
        for (std::size_t i = 0; i < faeries.faeries.size(); ++i) {
            if (i) std::cout << ',';
            const auto& row = faeries.faeries[i];
            std::cout << "{\"table_name\":"; quote(row.table_name);
            std::cout << ",\"description\":" << row.description << ",\"elemental\":" << row.elemental
                      << ",\"model_file\":" << row.model_file << ",\"name_id\":" << row.name_id
                      << ",\"spell_script_length\":" << row.spell_script_length << ",\"spell_script\":";
            quote(row.spell_script);
            std::cout << ",\"spell_type\":" << row.spell_type << ",\"type\":" << row.type << '}';
        }
        std::cout << "]}\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "skill tables: " << failure.what() << '\n';
        return 1;
    }
}
