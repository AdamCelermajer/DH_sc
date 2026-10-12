#include "glitch-video/material_renderer_manager.hpp"

#include <cstring>

namespace glitch::video {

// 0x5d7c5c _ZN6glitch5video24CMaterialRendererManager14SCreationStateC1EPc
// Partial: the list heads at +0x08..+0x34 are set by the original to point at themselves; they
// are not modelled here.
CMaterialRendererManager::SCreationState::SCreationState(char* name) : m_name(name) {}

// 0x5d8f24 _ZN6glitch5video24CMaterialRendererManagerC1EPNS0_12IVideoDriverE
// 0x5d8fa0 _ZN6glitch5video24CMaterialRendererManagerC2EPNS0_12IVideoDriverE
// Partial: the SIDedCollection base constructor and the memset of the id table (0xff, 0x22 bytes)
// are not reproduced.
CMaterialRendererManager::CMaterialRendererManager(IVideoDriver* driver) : m_driver(driver) {}

// 0x5d7cf4 _ZNK6glitch5video24CMaterialRendererManager12getTechniqueENS1_12STemporaryIDE
int32_t CMaterialRendererManager::getTechnique(STemporaryID id) const {
    return (m_creationState != nullptr && id.value != 0) ? static_cast<int32_t>(id.value) : 0;
}

// 0x5d7d14 _ZNK6glitch5video24CMaterialRendererManager15getParameterDefENS1_12STemporaryIDE
int32_t CMaterialRendererManager::getParameterDef(STemporaryID id) const {
    return (m_creationState != nullptr && id.value != 0) ? static_cast<int32_t>(id.value) : 0;
}

// 0x5db15c _ZN6glitch5video24CMaterialRendererManager18clearCreationStateEv
// Partial: the original runs the state destructor and releases the process buffer.
void CMaterialRendererManager::clearCreationState() {
    delete m_creationState;
    m_creationState = nullptr;
}

// 0x5dc058 _ZNK6glitch5video24CMaterialRendererManager19isCreatingTechniqueEPKc
// Partial: when no technique is open and a name is given, the original logs the name at level 3;
// the logger is not ported.
bool CMaterialRendererManager::isCreatingTechnique(const char* /*name*/) const {
    if (m_creationState == nullptr) {
        return false;
    }
    return m_creationState->m_inProgress04 != 0;
}

}  // namespace glitch::video
