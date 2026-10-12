#include "glitch-video/video_mode_list.hpp"

#include <utility>

namespace glitch::video {

// 0x6dab0c _ZN6glitch5video14CVideoModeListC1Ev
// 0x6daabc _ZN6glitch5video14CVideoModeListC2Ev
CVideoModeList::CVideoModeList() = default;

CVideoModeList::CVideoModeList(std::vector<SVideoMode> modes,
                               core::dimension2d<uint32_t> desktopResolution, uint32_t desktopDepth)
    : m_modes(std::move(modes)), m_desktopResolution(desktopResolution), m_desktopDepth(desktopDepth) {}

// 0x6dae50 _ZN6glitch5video14CVideoModeListD1Ev
// The original frees the mode array; the std::vector member does the same.
// (The deleting variant _ZN6glitch5video14CVideoModeListD0Ev is compiler generated.)

// 0x6dab74 _ZNK6glitch5video14CVideoModeList17getVideoModeCountEv
uint32_t CVideoModeList::getVideoModeCount() const {
    return static_cast<uint32_t>(m_modes.size());
}

// 0x6dab9c _ZNK6glitch5video14CVideoModeList22getVideoModeResolutionEi
// Original checks index <= count, which reads one element past the end.
// This port uses index < count and returns an empty size for anything outside.
core::dimension2d<uint32_t> CVideoModeList::getVideoModeResolution(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < m_modes.size()) {
        return m_modes[static_cast<size_t>(index)].Resolution;
    }
    return {};
}

// 0x6dac00 _ZNK6glitch5video14CVideoModeList17getVideoModeDepthEi
uint32_t CVideoModeList::getVideoModeDepth(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < m_modes.size()) {
        return m_modes[static_cast<size_t>(index)].Depth;
    }
    return 0;
}

// 0x6dac4c _ZNK6glitch5video14CVideoModeList20getDesktopResolutionEv
const core::dimension2d<uint32_t>& CVideoModeList::getDesktopResolution() const {
    return m_desktopResolution;
}

// 0x6dac54 _ZNK6glitch5video14CVideoModeList15getDesktopDepthEv
uint32_t CVideoModeList::getDesktopDepth() const { return m_desktopDepth; }

}  // namespace glitch::video
