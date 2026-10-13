// glitch::video::CVideoModeList: the list of display modes and the desktop mode.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-video/deps/glitch_core.hpp"

namespace glitch::video {

struct SVideoMode {
    core::dimension2d<uint32_t> Resolution;  // 8 bytes
    uint32_t Depth = 0;                      // 4 bytes; 12 bytes per entry in the original
};

// 0x6dac5c: the order heapsink<SVideoMode> (sort.hpp) uses: width, then height, then depth, each
// compared as a signed int (the listing uses signed branches on the three words).
inline bool operator<(const SVideoMode& a, const SVideoMode& b) {
    const int32_t aw = static_cast<int32_t>(a.Resolution.Width);
    const int32_t bw = static_cast<int32_t>(b.Resolution.Width);
    if (aw != bw) return aw < bw;
    const int32_t ah = static_cast<int32_t>(a.Resolution.Height);
    const int32_t bh = static_cast<int32_t>(b.Resolution.Height);
    if (ah != bh) return ah < bh;
    return static_cast<int32_t>(a.Depth) < static_cast<int32_t>(b.Depth);
}

class CVideoModeList {
public:
    CVideoModeList();
    // Port helper (not an original function): fills the list the way the
    // enumeration code does, so the accessors can be tested.
    CVideoModeList(std::vector<SVideoMode> modes, core::dimension2d<uint32_t> desktopResolution,
                   uint32_t desktopDepth);
    virtual ~CVideoModeList() = default;

    // 0x6dae90: appends a mode unless the same resolution and depth are already listed.
    // Partial: the original heapsorts the list after an insert with an operator< on
    // SVideoMode that is not recovered, so the list is only appended here.
    void addMode(const core::dimension2d<uint32_t>& resolution, uint32_t depth);
    uint32_t getVideoModeCount() const;
    core::dimension2d<uint32_t> getVideoModeResolution(int index) const;
    uint32_t getVideoModeDepth(int index) const;
    const core::dimension2d<uint32_t>& getDesktopResolution() const;
    uint32_t getDesktopDepth() const;

private:
    uint32_t m_unknown04 = 1;                 // +0x04, set to 1 by both constructors; purpose not identified
    std::vector<SVideoMode> m_modes;          // +0x08 begin, +0x0c end, +0x10 capacity end
    core::dimension2d<uint32_t> m_desktopResolution;  // +0x14
    uint32_t m_desktopDepth = 0;              // +0x1c
};

}  // namespace glitch::video
