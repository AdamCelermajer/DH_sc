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
