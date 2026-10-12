// Batch e1-glitch-video-glitch_video_CMaterialRendererManager_SCreationS-0: SCreationState::addRenderPass.
#include <cstring>

#include "glitch-video/material_renderer_manager.hpp"

// glitch-core-io/core/process_buffer.hpp is not on the glitch-video include path, so the three process
// buffer declarations used here are repeated exactly as that header declares them. Replace this block with
// the include once the category include paths allow it.
namespace glitch::core {
struct SScopedEnableProcessBufferHeapExcess {
    explicit SScopedEnableProcessBufferHeapExcess(bool enable);
    bool previous = false;  // +0x00
};
int* allocProcessBuffer(int size);            // 0x5345f4
void setProcessBufferHeapExcessEnabled(bool enabled);  // 0x534268
}  // namespace glitch::core

namespace glitch::video {

// Number of 8-byte binding entries for a shader. The original reads four u16 fields of IShader: the
// count is (u16 +0x36 + u16 +0x2e) - (u16 +0x34 + u16 +0x2c), truncated to u16 (asm of 0x5d93e8, entry).
// The port's IShader does not name those fields yet, so this helper has no body. It must be defined
// with the shader layout before addRenderPass can run.
uint16_t shaderBindingEntryCount(const glitch::IReferenceCounted* shader);

namespace {

using SCreationState = CMaterialRendererManager::SCreationState;
using SPassNode = CMaterialRendererManager::SCreationState::SPassNode;

// Fills a pass record the way the reuse path of the original does (LABEL_6 in the pseudocode). The old
// shader is released before the new one is counted.
static_assert(sizeof(detail::renderpass::SRenderState) == sizeof(SCreationState::SRenderPass::stateWords),
              "the pass keeps the 32-byte render state block as eight words");

void fillPass(SCreationState::SRenderPass& pass, glitch::IReferenceCounted* shader,
              const detail::renderpass::SRenderState& state, int* bindings) {
    if (pass.shader != nullptr) {
        pass.shader->drop();
    }
    std::memcpy(pass.stateWords, &state, sizeof pass.stateWords);
    pass.shader = shader;
    if (shader != nullptr) {
        ++shader->referenceCounter;
    }
    pass.bindings = bindings;
    pass.unknown28 = 0;
    pass.unknown2c = 0;
    pass.unknown2e = 0;
    pass.flag30 = 1;
}

// Appends a new node at the tail of the pass list (the insert at end of the original). The node is
// allocated with new here; the original allocates it from the process buffer. The original copies a
// temporary record and drops the temporary's count; the net effect on the shader count is one reference
// held by the node, which is what this writes.
void appendPass(SCreationState& state, glitch::IReferenceCounted* shader,
                const detail::renderpass::SRenderState& renderState, int* bindings) {
    // Both insert paths of the original enable heap excess for the allocation and restore it after.
    // The original passes an undefined argument here; the port passes true, which is what the
    // decompiled callee does whatever the argument is.
    glitch::core::SScopedEnableProcessBufferHeapExcess guard(true);

    SPassNode* node = new SPassNode();
    fillPass(node->pass, shader, renderState, bindings);

    SPassNode* tail = state.m_passSentinel.prev;
    node->next = &state.m_passSentinel;
    node->prev = tail;
    tail->next = node;
    state.m_passSentinel.prev = node;

    glitch::core::setProcessBufferHeapExcessEnabled(guard.previous);
}

}  // namespace

// 0x5d93e8 _ZN6glitch5video24CMaterialRendererManager14SCreationState13addRenderPassERKN5boost13intrusive_ptrIKNS0_7IShaderEEERKNS0_6detail10renderpass12SRenderStateERKNSA_8material12SRenderStateE
// Partial, for three reasons:
//   1. shaderBindingEntryCount has no body yet (IShader fields at +0x2c, +0x2e, +0x34 and +0x36 are not named).
//   2. The pass-list sentinel (+0x10/+0x14) must be linked to itself by the constructor. The existing
//      constructor (material_renderer_manager.cpp) links the pink sentinel only.
//   3. The symbol has a third parameter, the material::SRenderState const&. The body never reads it (the
//      entry code loads only the first two arguments), so it is not a parameter here.
// The destructor (material_renderer_manager.cpp) does not release the pass nodes yet.
void CMaterialRendererManager::SCreationState::addRenderPass(glitch::IReferenceCounted* shader,
                                                             const detail::renderpass::SRenderState& state) {
    // Binding block for the shader: 8 bytes per entry, zeroed. A null block is not cleared.
    const uint16_t entryCount = shaderBindingEntryCount(shader);
    int* bindings = nullptr;
    if (entryCount != 0) {
        bindings = glitch::core::allocProcessBuffer(8 * entryCount);
    }
    if (bindings != nullptr) {
        std::memset(bindings, 0, static_cast<size_t>(8) * entryCount);
    }
    m_bindingTotal += entryCount;

    // m_pinkCounter24 at +0x24 is the pass count of the technique being built (addPinkBind reads it as
    // count minus one).
    if (m_passSentinel.next == &m_passSentinel) {
        // Empty list: append, and the cursor points at the first node.
        appendPass(*this, shader, state, bindings);
        m_passCursor = m_passSentinel.next;
    } else if (m_pinkCounter24 == 0) {
        // No passes in the technique yet: reuse the first node, which the cursor now points at.
        m_passCursor = m_passSentinel.next;
        fillPass(m_passCursor->pass, shader, state, bindings);
    } else if (m_passCursor != m_passSentinel.prev) {
        // The cursor is not at the tail: reuse the node after it.
        m_passCursor = m_passCursor->next;
        fillPass(m_passCursor->pass, shader, state, bindings);
    } else {
        // The cursor is at the tail: append, and the cursor moves to the new node (the old tail's next).
        appendPass(*this, shader, state, bindings);
        m_passCursor = m_passCursor->next;
    }
    ++m_pinkCounter24;
}

}  // namespace glitch::video
