#include "intro_stream_v1.hpp"

#include "inflate_raw_v1.hpp"

#include <cstring>
#include <string>
#include <utility>

namespace dh::foundation::startup {
namespace {

constexpr char kMagic[8] = {'D', 'H', '2', 'I', 'N', 'T', 'R', '1'};
constexpr std::uint32_t kVersion = 1;
constexpr std::size_t kHeaderBytes = 8 + 4 * 5;  // magic, version, w, h, fps, count
constexpr std::uint32_t kMaxFrames = 1u << 20;
constexpr std::uint32_t kMaxDimension = 4096;

std::uint32_t read_u32(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16) |
           (std::uint32_t(p[3]) << 24);
}

} // namespace

bool IntroStreamV1::open(std::vector<std::uint8_t> bytes, std::string& error) {
    valid_ = false;
    offset_.clear();
    size_.clear();
    previous_.clear();
    next_ = 0;
    bytes_ = std::move(bytes);
    if (bytes_.size() < kHeaderBytes) { error = "intro stream: file too small"; return false; }
    if (std::memcmp(bytes_.data(), kMagic, sizeof kMagic) != 0) { error = "intro stream: bad magic"; return false; }
    const std::uint8_t* h = bytes_.data() + 8;
    if (read_u32(h) != kVersion) { error = "intro stream: unsupported version"; return false; }
    IntroStreamInfo info;
    info.width = read_u32(h + 4);
    info.height = read_u32(h + 8);
    info.fps = read_u32(h + 12);
    info.frame_count = read_u32(h + 16);
    if (info.width == 0 || info.height == 0 || info.width > kMaxDimension || info.height > kMaxDimension) {
        error = "intro stream: invalid dimensions";
        return false;
    }
    if (info.fps == 0 || info.frame_count == 0 || info.frame_count > kMaxFrames) {
        error = "intro stream: invalid fps or frame count";
        return false;
    }
    const std::size_t tableBytes = std::size_t(info.frame_count) * 8;
    if (bytes_.size() < kHeaderBytes + tableBytes) { error = "intro stream: truncated frame table"; return false; }
    data_start_ = kHeaderBytes + tableBytes;
    const std::size_t dataBytes = bytes_.size() - data_start_;
    const std::size_t rawFrame = std::size_t(info.width) * info.height * 2;
    offset_.resize(info.frame_count);
    size_.resize(info.frame_count);
    const std::uint8_t* table = bytes_.data() + kHeaderBytes;
    for (std::uint32_t i = 0; i < info.frame_count; ++i) {
        offset_[i] = read_u32(table + i * 8);
        size_[i] = read_u32(table + i * 8 + 4);
        if (size_[i] == 0 || std::size_t(offset_[i]) + size_[i] > dataBytes) {
            error = "intro stream: frame " + std::to_string(i) + " outside the file";
            return false;
        }
        // A raw deflate stream can expand, but a frame can never be larger than
        // its own raw size by more than deflate's worst case (stored blocks).
        if (size_[i] > rawFrame + rawFrame / 1000 + 64) {
            error = "intro stream: frame " + std::to_string(i) + " larger than raw size";
            return false;
        }
    }
    info_ = info;
    previous_.assign(std::size_t(info.width) * info.height, 0);
    valid_ = true;
    error.clear();
    return true;
}

void IntroStreamV1::rewind() noexcept {
    next_ = 0;
    previous_.assign(previous_.size(), 0);
}

bool IntroStreamV1::next_frame(std::vector<std::uint8_t>& rgba, std::string& error) {
    if (at_end()) { error = "intro stream: end of stream"; return false; }
    const std::uint32_t i = next_;
    const std::size_t count = std::size_t(info_.width) * info_.height;
    std::vector<std::uint8_t> raw;
    if (!inflate_raw_v1(bytes_.data() + data_start_ + offset_[i], size_[i], raw, count * 2, error)) {
        error = "intro stream: frame " + std::to_string(i) + ": " + error;
        valid_ = false;  // a corrupt frame ends the stream; the host falls through
        return false;
    }
    if (raw.size() != count * 2) {
        error = "intro stream: frame " + std::to_string(i) + " has the wrong size";
        valid_ = false;
        return false;
    }
    for (std::size_t p = 0; p < count; ++p) {
        const std::uint16_t delta = static_cast<std::uint16_t>(raw[p * 2] | (raw[p * 2 + 1] << 8));
        previous_[p] = static_cast<std::uint16_t>(previous_[p] ^ delta);
    }
    rgb565_to_rgba8(previous_.data(), count, rgba);
    ++next_;
    error.clear();
    return true;
}

void rgb565_to_rgba8(const std::uint16_t* pixels, std::size_t count, std::vector<std::uint8_t>& rgba) {
    rgba.resize(count * 4);
    for (std::size_t p = 0; p < count; ++p) {
        const unsigned v = pixels[p];
        const unsigned r5 = (v >> 11) & 31u, g6 = (v >> 5) & 63u, b5 = v & 31u;
        rgba[p * 4 + 0] = static_cast<std::uint8_t>((r5 << 3) | (r5 >> 2));
        rgba[p * 4 + 1] = static_cast<std::uint8_t>((g6 << 2) | (g6 >> 4));
        rgba[p * 4 + 2] = static_cast<std::uint8_t>((b5 << 3) | (b5 >> 2));
        rgba[p * 4 + 3] = 255;
    }
}

} // namespace dh::foundation::startup
