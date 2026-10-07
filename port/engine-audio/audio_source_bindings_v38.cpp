#include "audio_source_bindings_v38.hpp"
#include <cstring>
#include <unordered_set>

namespace dh2::audio {
namespace {
struct Stream {
    const std::uint8_t* data;
    std::size_t size, position{};
    bool word(std::uint32_t& value) noexcept {
        if (!data || position > size || size-position < 4) return false;
        value = std::uint32_t(data[position]) | (std::uint32_t(data[position+1]) << 8)
              | (std::uint32_t(data[position+2]) << 16) | (std::uint32_t(data[position+3]) << 24);
        position += 4;
        return true;
    }
};
std::int32_t signed_word(std::uint32_t value) noexcept {
    std::int32_t result;
    std::memcpy(&result, &value, sizeof result);
    return result;
}
}
bool AudioSourceBindingsV38::load(const std::uint8_t* records, std::size_t records_size,
                                 const std::uint8_t* names, std::size_t names_size,
                                 std::string& error) {
    Stream binary{records, records_size}, labels{names, names_size};
    std::uint32_t count{}, name_count{};
    if (!binary.word(count) || count > 65536 || !labels.word(name_count) || count != name_count
        || records_size-4 != std::size_t(count)*8) {
        error = "Required matching generated Sounds UID/event and name streams";
        return false;
    }
    std::vector<AudioSoundAutoGenV34> next_rows;
    std::vector<std::string> next_names;
    std::unordered_set<std::string> unique;
    next_rows.reserve(count); next_names.reserve(count);
    for (std::uint32_t i=0; i<count; ++i) {
        std::uint32_t uid{}, event{}, length{};
        if (!binary.word(uid) || !binary.word(event) || signed_word(uid) < -1 || event > 1
            || !labels.word(length) || !length || length > 4096
            || labels.position > labels.size || length > labels.size-labels.position) {
            error = "Required complete source SoundAutoGen UID/event/name row";
            return false;
        }
        // Original readNames allocates length+1 and appends NUL. Cache names
        // have length bytes without terminator. Embedded NUL is not a name.
        const char* text = reinterpret_cast<const char*>(labels.data+labels.position);
        if (std::memchr(text, 0, length)) {
            error = "Required source name without embedded terminator";
            return false;
        }
        std::string name(text, length);
        if (!unique.insert(name).second) {
            error = "Required unique source Sounds name order";
            return false;
        }
        labels.position += length;
        next_rows.push_back({signed_word(uid), signed_word(event)});
        next_names.push_back(std::move(name));
    }
    if (labels.position != labels.size) {
        error = "Required exact generated Sounds name stream extent";
        return false;
    }
    rows_ = std::move(next_rows); names_ = std::move(next_names); error.clear();
    return true;
}
const AudioSoundAutoGenV34* AudioSourceBindingsV38::row(std::int32_t id) const noexcept {
    return id >= 0 && std::size_t(id) < rows_.size() ? &rows_[std::size_t(id)] : nullptr;
}
std::int32_t AudioSourceBindingsV38::source_id(const std::string& name) const noexcept {
    for (std::size_t i=0; i<names_.size(); ++i)
        if (names_[i] == name) return static_cast<std::int32_t>(i);
    return -1;
}
}
