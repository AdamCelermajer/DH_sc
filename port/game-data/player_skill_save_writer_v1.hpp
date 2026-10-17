#pragma once

#include "player_savegame_v1.hpp"

#include <string>

namespace dh2::data::player_skill_save_writer_v1 {

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    source_assertion_boundary,
    failed,
};

struct WriteServicesV1 {
    void* context{};
    // The source performs one stream write per count, string length, string
    // payload, level, map key, and map value. A failed write retains its
    // already accepted prefix.
    bool (*write)(void*, Bytes, std::string&){};
};

// Source PlayerSavegame::__SaveSkills at libDungeonHunter2.so 0x469e6c.
// Skill names are borrowed from the decoded Skills table; all mutable skill
// levels and both slot maps come from this one canonical Save projection.
Status write_section_v1(const PlayerSavegameV1& save,
                        const SkillTables& tables,
                        const WriteServicesV1& stream,
                        std::string& error);

}  // namespace dh2::data::player_skill_save_writer_v1
