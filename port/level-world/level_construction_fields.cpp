#include "level_construction_fields.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace dh2::level_construction_fields {
namespace {

constexpr std::size_t kSourceBufferBytes = 1024;

bool has_embedded_nul(std::string_view value) noexcept {
    for (const char ch : value) {
        if (ch == '\0') return true;
    }
    return false;
}

bool safe_level_file(const std::string& value) noexcept {
    // strcpy into the original 1024-byte stack buffer writes the terminator as
    // well; a 1023-byte value is the largest source-safe row.
    return value.size() < kSourceBufferBytes &&
           value.find('\0') == std::string::npos;
}

void ascii_lower(char* bytes, std::size_t size) noexcept {
    for (std::size_t i = 0; i < size; ++i) {
        const auto ch = static_cast<unsigned char>(bytes[i]);
        if (ch >= static_cast<unsigned char>('A') &&
            ch <= static_cast<unsigned char>('Z')) {
            bytes[i] = static_cast<char>(ch + ('a' - 'A'));
        }
    }
}

}  // namespace

Status initialize(const data::LevelTables* tables,
                  std::string_view incoming_level_file,
                  std::int32_t constructor_difficulty_118,
                  State* output,
                  Result* result) noexcept {
    if (!tables || !output || !result ||
        tables->levels.size() > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) ||
        has_embedded_nul(incoming_level_file)) {
        return Status::invalid_argument;
    }

    // The source's strcpy has no bound check. Reject such cache data as a port
    // guard instead of reproducing a stack overwrite.
    for (const auto& row : tables->levels) {
        if (!safe_level_file(row.level_file)) return Status::unsafe_level_file;
    }

    State candidate{};
    candidate.level_list_index_3c = -1;
    candidate.hub_40 = -1;
    candidate.is_random_e8 = 0;
    candidate.difficulty_118 = constructor_difficulty_118;

    Result report{};
    report.status = Status::no_match;
    report.selected_index = -1;

    std::array<char, kSourceBufferBytes> buffer{};
    for (std::size_t index = 0; index < tables->levels.size(); ++index) {
        const auto& row = tables->levels[index];
        const auto length = row.level_file.size();
        for (std::size_t i = 0; i < length; ++i) buffer[i] = row.level_file[i];
        buffer[length] = '\0';
        ascii_lower(buffer.data(), length);
        ++report.rows_examined;

        const std::string_view needle(buffer.data(), length);
        // strstr(haystack, "") returns haystack, including for an empty row.
        if (incoming_level_file.find(needle) == std::string_view::npos) continue;

        candidate.level_list_index_3c = static_cast<std::int32_t>(index);
        candidate.hub_40 = row.hub;
        candidate.is_random_e8 = row.is_random ? 1 : 0;
        report.status = Status::selected;
        report.selected_index = static_cast<std::int32_t>(index);
        break;
    }

    *output = candidate;
    *result = report;
    return report.status;
}

}  // namespace dh2::level_construction_fields
