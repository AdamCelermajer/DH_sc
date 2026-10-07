#pragma once
#include "audio_catalog_v34.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::audio {
// Source registration4c0d80/4c0da0 selects the generated sdd streams.
// Source identity is the row index, not its soundpack label or UID.
class AudioSourceBindingsV38 {
public:
    static constexpr const char* records_uri = "data/pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin";
    static constexpr const char* names_uri = "data/pydata/sdd_dungeon_hunter_2_iphone_pyarraynames.bin";
    bool load(const std::uint8_t* records, std::size_t records_size,
              const std::uint8_t* names, std::size_t names_size,
              std::string& error);
    const std::vector<AudioSoundAutoGenV34>& rows() const noexcept { return rows_; }
    const std::vector<std::string>& names() const noexcept { return names_; }
    const AudioSoundAutoGenV34* row(std::int32_t source_id) const noexcept;
    std::int32_t source_id(const std::string& name) const noexcept;
private:
    std::vector<AudioSoundAutoGenV34> rows_;
    std::vector<std::string> names_;
};
}
