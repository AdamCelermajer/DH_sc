#include "player_property_save_writer_v1.hpp"

#include <cstdint>
#include <cstring>

namespace dh2::data::player_property_save_writer_v1 {
namespace {

using player_save_section_writers_v1::Status;
using player_save_section_writers_v1::WriteServicesV1;

constexpr std::uint32_t kSourcePropertyCount = 224;

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
        if (error.empty()) error = "source PROP stream write failed";
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

}  // namespace

player_save_section_writers_v1::Status write_properties_v1(
    const PlayerSavegameV1& save, const PropertyView& properties,
    const WriteServicesV1& stream, std::string& error) {
    // The original callback asserts on its Save's missing m_player before it
    // writes anything. Keep that boundary explicit instead of inventing zeros.
    if (overlaps(&error, sizeof(error), &save, sizeof(save)) ||
        overlaps(&error, sizeof(error), &properties, sizeof(properties)) ||
        overlaps(&error, sizeof(error), &stream, sizeof(stream)))
        return Status::invalid_argument;
    error.clear();
    if (!stream.write) {
        error = "source PROP stream writer is required";
        return Status::invalid_argument;
    }
    if (!save.character()) {
        error = "source PROP Character assertion boundary";
        return Status::source_assertion_boundary;
    }
    if (properties.resolved &&
        overlaps(&error, sizeof(error), properties.resolved,
                 kSourcePropertyCount * sizeof(std::int32_t)))
        return Status::invalid_argument;
    if (!properties.resolved ||
        reinterpret_cast<std::uintptr_t>(properties.resolved) %
                alignof(std::int32_t) !=
            0 ||
        overlaps(properties.resolved,
                 kSourcePropertyCount * sizeof(std::int32_t), &save,
                 sizeof(save)) ||
        overlaps(properties.resolved,
                 kSourcePropertyCount * sizeof(std::int32_t), &properties,
                 sizeof(properties))) {
        error = "source PROP current CharacterProperties values required";
        return Status::source_assertion_boundary;
    }

    try {
        // IDA: PlayerSavegame::__SaveProperties at 0x4689d8 writes the fixed
        // count first, then _GetProperty-equivalent current values in table
        // order as int32 words, then Save+0x194 as one bool byte.
        if (!word(stream, kSourcePropertyCount, error)) return Status::failed;
        for (std::uint32_t i = 0; i < kSourcePropertyCount; ++i) {
            std::uint32_t bits = 0;
            std::memcpy(&bits, properties.resolved + i, sizeof(bits));
            if (!word(stream, bits, error)) return Status::failed;
        }
        const std::uint8_t saved = save.saved_properties_byte_194();
        if (!emit(stream, &saved, sizeof(saved), error)) return Status::failed;
        error.clear();
        return Status::complete;
    } catch (...) {
        if (error.empty()) error = "source PROP stream writer threw";
        return Status::failed;
    }
}

}  // namespace dh2::data::player_property_save_writer_v1
