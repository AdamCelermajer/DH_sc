#pragma once

// Portable intro picture stream (Preview 15). Produced offline by
// tools/convert_intro_video.ps1 from original-media/intro.mp4 (ffmpeg at
// asset-prep time only; no FFmpeg or Media Foundation at runtime).
//
// Why not pl_mpeg: the single-file MPEG-1 decoder is not present in this
// workspace and cannot be downloaded offline. Fallback chosen: per-frame
// XOR delta against the previous frame in RGB565, deflate-compressed, decoded
// by inflate_raw_v1.hpp. Plain C++, no platform calls.
//
// File layout (all integers little-endian):
//   char     magic[8] = "DH2INTR1"
//   u32      version = 1
//   u32      width, height          (640 x 360)
//   u32      fps                    (24)
//   u32      frame_count
//   u32      frame_offset[frame_count], u32 frame_size[frame_count]
//            (offsets relative to the start of the frame data region,
//             which begins right after the table)
//   bytes    frame data: raw deflate stream per frame. Inflated bytes are
//            width*height u16 values = RGB565 of (frame XOR previous frame),
//            previous of frame 0 is zero. Each u16 is little-endian.

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation::startup {

struct IntroStreamInfo {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t fps = 0;
    std::uint32_t frame_count = 0;
};

class IntroStreamV1 {
public:
    // Parses the header and frame table. Rejects bad magic/version, zero size,
    // frame ranges outside the file, and frames larger than the raw size.
    bool open(std::vector<std::uint8_t> bytes, std::string& error);

    const IntroStreamInfo& info() const noexcept { return info_; }
    bool valid() const noexcept { return valid_; }
    std::uint32_t next_index() const noexcept { return next_; }
    bool at_end() const noexcept { return !valid_ || next_ >= info_.frame_count; }
    void rewind() noexcept;

    // Decodes the next frame into RGBA8 (width*height*4 bytes, row 0 = top).
    // Returns false at end of stream or on a corrupt frame (error is set).
    bool next_frame(std::vector<std::uint8_t>& rgba, std::string& error);

private:
    std::vector<std::uint8_t> bytes_;
    IntroStreamInfo info_;
    std::size_t data_start_ = 0;
    std::vector<std::uint32_t> offset_;
    std::vector<std::uint32_t> size_;
    std::vector<std::uint16_t> previous_;
    std::uint32_t next_ = 0;
    bool valid_ = false;
};

// Expands RGB565 (already reconstructed, one u16 per pixel) to RGBA8 with
// alpha 255. Replicates the top bits into the low bits for full-range values.
void rgb565_to_rgba8(const std::uint16_t* pixels, std::size_t count, std::vector<std::uint8_t>& rgba);

} // namespace dh::foundation::startup
