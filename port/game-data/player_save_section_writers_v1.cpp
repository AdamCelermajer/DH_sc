#include "player_save_section_writers_v1.hpp"

#include <array>
#include <cstring>
#include <limits>

namespace dh2::data::player_save_section_writers_v1 {
namespace {

// IDA: PlayerSavegame::__SaveCurrentFaery@0x468bf0,
// __SaveFaeries@0x468f18, and __SaveFastTravelList@0x469cdc. The source
// registers these at mask 4 in PlayerSavegame::_Load@0x464f4c; its
// _InitFaeries@0x4694c8 initializes five entries for each of three slots.

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
        if (error.empty()) error = "source save-section stream write failed";
        return false;
    }
    return true;
}

bool word(const WriteServicesV1& stream, std::uint32_t value,
          std::string& error) {
    const std::uint8_t bytes[]{std::uint8_t(value), std::uint8_t(value >> 8),
                              std::uint8_t(value >> 16),
                              std::uint8_t(value >> 24)};
    return emit(stream, bytes, sizeof(bytes), error);
}

bool halfword(const WriteServicesV1& stream, std::uint16_t value,
              std::string& error) {
    const std::uint8_t bytes[]{std::uint8_t(value), std::uint8_t(value >> 8)};
    return emit(stream, bytes, sizeof(bytes), error);
}

bool source_string(const WriteServicesV1& stream, const char* bytes,
                   std::size_t size, std::string& error) {
    if (size >= std::size_t(std::numeric_limits<std::uint32_t>::max())) {
        error = "source save-section string exceeds 32-bit length";
        return false;
    }
    const auto length = static_cast<std::uint32_t>(size + 1);
    if (!word(stream, length, error)) return false;
    if (size && !emit(stream, reinterpret_cast<const std::uint8_t*>(bytes),
                      size, error))
        return false;
    const std::uint8_t terminator = 0;
    return emit(stream, &terminator, 1, error);
}

bool initialized_faery(const PlayerSavegameV1& save, std::size_t difficulty,
                       std::string& error) {
    if (save.faeries_initialized()[difficulty]) return true;
    error = "source faery pointer assertion boundary: initialize faeries first";
    return false;
}

Status write_current_faery(const PlayerSavegameV1& save,
                           const WriteServicesV1& stream,
                           std::string& error) {
    const auto& current = save.current_faeries();
    for (std::size_t difficulty = 0; difficulty < current.size(); ++difficulty) {
        if (!initialized_faery(save, difficulty, error))
            return Status::source_assertion_boundary;
        if (!word(stream, static_cast<std::uint32_t>(current[difficulty]), error))
            return Status::failed;
    }
    return Status::complete;
}

Status write_faeries(const PlayerSavegameV1& save,
                     const WriteServicesV1& stream, std::string& error) {
    constexpr std::uint32_t source_count = 5;
    const auto& current = save.current_faeries();
    const auto& faeries = save.faeries();
    for (std::size_t difficulty = 0; difficulty < faeries.size(); ++difficulty) {
        if (!initialized_faery(save, difficulty, error))
            return Status::source_assertion_boundary;
        if (!word(stream, static_cast<std::uint32_t>(current[difficulty]), error) ||
            !word(stream, source_count, error))
            return Status::failed;
        for (const auto& faery : faeries[difficulty]) {
            // The source writes the U16 at entry+2 before the signed byte at
            // entry+0; the intervening padding byte is not serialized.
            if (!halfword(stream, faery.level, error)) return Status::failed;
            const std::uint8_t state = faery.state;
            if (!emit(stream, &state, 1, error)) return Status::failed;
        }
    }
    return Status::complete;
}

Status write_fast_travel(const PlayerSavegameV1& save,
                         const WriteServicesV1& stream,
                         std::string& error) {
    std::array<char, 64> text{};
    for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
        const auto* words = save.source_fast_travel_bits(difficulty);
        if (!words) {
            error = "source fast-travel bitset backing unavailable";
            return Status::source_assertion_boundary;
        }
        // std::bitset<64>::_M_copy_to_string writes the high bit first.
        for (std::uint32_t position = 0; position < 64; ++position) {
            const auto bit = 63u - position;
            text[position] = ((*words)[bit >> 5] &
                              (std::uint32_t{1} << (bit & 31)))
                                 ? '1'
                                 : '0';
        }
        if (!source_string(stream, text.data(), text.size(), error))
            return Status::failed;
    }
    return Status::complete;
}

}  // namespace

Status write_section_v1(const char* tag, const PlayerSavegameV1& save,
                        const WriteServicesV1& stream, std::string& error) {
    if (overlaps(&error, sizeof(error), &save, sizeof(save)) ||
        overlaps(&error, sizeof(error), &stream, sizeof(stream)))
        return Status::invalid_argument;
    error.clear();
    if (!tag || !stream.write) {
        error = "source save-section tag and stream writer are required";
        return Status::invalid_argument;
    }

    const bool cfee = std::strcmp(tag, "CFEE") == 0;
    const bool faes = std::strcmp(tag, "FAES") == 0;
    const bool ftvl = std::strcmp(tag, "FTVL") == 0;
    if (!cfee && !faes && !ftvl) {
        error = "no writer for requested source save-section tag";
        return Status::unsupported_tag;
    }

    try {
        if (cfee) return write_current_faery(save, stream, error);
        if (faes) return write_faeries(save, stream, error);
        return write_fast_travel(save, stream, error);
    } catch (...) {
        if (error.empty()) error = "source save-section stream threw";
        return Status::failed;
    }
}

}  // namespace dh2::data::player_save_section_writers_v1
