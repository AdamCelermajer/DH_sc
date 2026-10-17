#include "player_skill_save_writer_v1.hpp"

#include <cstring>
#include <limits>
#include <map>
#include <vector>

namespace dh2::data::player_skill_save_writer_v1 {
namespace {

// IDA: PlayerSavegame::_Load@0x464f4c registers SKIL with __SaveSkills and
// passes this as the callback context. __SaveSkills@0x469e6c writes through
// IStreamBase::writeAs; Android's ARM stream representation is little-endian.

bool overlaps(const void* a, std::size_t a_size,
              const void* b, std::size_t b_size) noexcept {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (a_size > UINTPTR_MAX - x || b_size > UINTPTR_MAX - y) return true;
    return a_size && b_size && x < y + b_size && y < x + a_size;
}

bool emit(const WriteServicesV1& stream, const std::uint8_t* bytes,
          std::size_t size, std::string& error) {
    if (!stream.write(stream.context, {bytes, size}, error)) {
        if (error.empty()) error = "source SKIL stream write failed";
        return false;
    }
    return true;
}

bool word(const WriteServicesV1& stream, std::uint32_t value,
          std::string& error) {
    const std::uint8_t bytes[]{std::uint8_t(value),
                              std::uint8_t(value >> 8),
                              std::uint8_t(value >> 16),
                              std::uint8_t(value >> 24)};
    return emit(stream, bytes, sizeof(bytes), error);
}

bool halfword(const WriteServicesV1& stream, std::uint16_t value,
              std::string& error) {
    const std::uint8_t bytes[]{std::uint8_t(value),
                              std::uint8_t(value >> 8)};
    return emit(stream, bytes, sizeof(bytes), error);
}

bool source_string(const WriteServicesV1& stream, const char* value,
                   std::size_t size, std::string& error) {
    // IStreamBase::writeAs(string) writes a signed int for strlen + 1, then
    // emits the bytes and trailing NUL in one call (0x461668).
    constexpr auto source_max =
        static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max());
    if (size >= source_max) {
        error = "SKIL source string length exceeds signed 32-bit range";
        return false;
    }
    const auto count = static_cast<std::uint32_t>(size + 1);
    if (!word(stream, count, error)) return false;
    return emit(stream, reinterpret_cast<const std::uint8_t*>(value),
                size + 1, error);
}

bool source_count_fits(std::size_t count) noexcept {
    return count <=
           static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max());
}

}  // namespace

Status write_section_v1(const PlayerSavegameV1& save,
                        const SkillTables& tables,
                        const WriteServicesV1& stream,
                        std::string& error) {
    if (overlaps(&error, sizeof(error), &save, sizeof(save)) ||
        overlaps(&error, sizeof(error), &tables, sizeof(tables)) ||
        overlaps(&error, sizeof(error), &stream, sizeof(stream)))
        return Status::invalid_argument;
    error.clear();
    if (!stream.write) {
        error = "source SKIL stream writer is required";
        return Status::invalid_argument;
    }

    // __SaveSkills asserts m_skills before writing. No stream bytes are
    // emitted when the source skills array has not reached _InitSkills.
    if (!save.skills_initialized()) {
        error = "source m_skills assertion boundary: initialize skills first";
        return Status::source_assertion_boundary;
    }

    const auto& skills = save.skills();
    if (!source_count_fits(skills.size())) {
        error = "SKIL source skill count exceeds signed 32-bit range";
        return Status::source_assertion_boundary;
    }

    try {
        if (!word(stream, static_cast<std::uint32_t>(skills.size()), error))
            return Status::failed;

        for (const auto& skill : skills) {
            if (skill.id < 0 ||
                static_cast<std::size_t>(skill.id) >= tables.skills.size()) {
                error = "SKIL source SkillTable name dependency unavailable for id " +
                        std::to_string(skill.id);
                return Status::source_assertion_boundary;
            }
            const auto& name = tables.skills[static_cast<std::size_t>(skill.id)]
                                   .table_name;
            const auto* c_name = name.c_str();
            const auto name_size = std::strlen(c_name);
            if (!source_string(stream, c_name, name_size, error) ||
                !halfword(stream, skill.level, error))
                return Status::failed;
        }

        // The original has exactly two std::map<int,int> slot sets. Its
        // in-order traversal emits each set's node count, then key and value.
        for (const auto& slots : save.skill_slots()) {
            if (!source_count_fits(slots.size())) {
                error = "SKIL source slot count exceeds signed 32-bit range";
                return Status::source_assertion_boundary;
            }
            if (!word(stream, static_cast<std::uint32_t>(slots.size()), error))
                return Status::failed;
            for (const auto& entry : slots) {
                if (!word(stream, static_cast<std::uint32_t>(entry.first), error) ||
                    !word(stream, entry.second, error))
                    return Status::failed;
            }
        }
    } catch (...) {
        if (error.empty()) error = "source SKIL writer dependency threw";
        return Status::failed;
    }
    error.clear();
    return Status::complete;
}

}  // namespace dh2::data::player_skill_save_writer_v1
