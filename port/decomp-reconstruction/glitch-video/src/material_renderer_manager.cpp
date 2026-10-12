#include "glitch-video/material_renderer_manager.hpp"

namespace glitch::video {

// 0x5d7cf4 _ZNK6glitch5video24CMaterialRendererManager12getTechniqueENS1_12STemporaryIDE
// Partial: the meaning of the flag word at +0x90 is not identified.
int32_t CMaterialRendererManager::getTechnique(STemporaryID id) const {
    return (m_unknown90 != 0 && id.value != 0) ? static_cast<int32_t>(id.value) : 0;
}

// 0x5d7d14 _ZNK6glitch5video24CMaterialRendererManager15getParameterDefENS1_12STemporaryIDE
// Partial: same test as getTechnique; see the note there.
int32_t CMaterialRendererManager::getParameterDef(STemporaryID id) const {
    return (m_unknown90 != 0 && id.value != 0) ? static_cast<int32_t>(id.value) : 0;
}

}  // namespace glitch::video
