#pragma once

#include "data.hpp"
#include "player_savegame_v1.hpp"

#include <string>

namespace dh2::data::player_save_section_writers_v1 {

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    unsupported_tag,
    source_assertion_boundary,
    failed,
};

struct WriteServicesV1 {
    void* context{};
    // The sink copies bytes synchronously. A failed write retains any prefix
    // it already accepted, matching the source stream's non-transactional path.
    bool (*write)(void*, Bytes, std::string&){};
};

// Source-backed writers for CFEE, FAES and FTVL over the one canonical Save.
// No copies of the faery/current-faery/fast-travel stores are created. CFEE
// and FAES require PlayerSavegameV1::initialize_faeries() first; FTVL is
// backed by the Save's six constructor-initialized bitset words.
Status write_section_v1(const char* tag, const PlayerSavegameV1& save,
                        const WriteServicesV1& stream, std::string& error);

}  // namespace dh2::data::player_save_section_writers_v1
