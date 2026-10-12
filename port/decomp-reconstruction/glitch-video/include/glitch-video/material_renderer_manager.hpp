// glitch::video::CMaterialRendererManager. Only the parts that the port can express are
// reconstructed here: the two lookups that take a temporary id, the creation-state slot, and
// the constructor. The technique, render-pass and parameter tables of the original sit on
// SIDedCollection and SSharedString red-black trees, which are not in the port.
#pragma once

#include <cstdint>

#include "glitch-shared/reference_counted.hpp"
#include "glitch-video/tb/renderpass_srenderstate.hpp"

namespace glitch::video {

class IVideoDriver;

class CMaterialRendererManager {
public:
    // Temporary id, passed by value as one word in the original.
    struct STemporaryID {
        uint32_t value = 0;
    };

    // The state of the technique being defined (SCreationState in the original). Its list heads
    // (+0x08..+0x34) are not modelled; only the name and the in-progress word are kept.
    class SCreationState {
    public:
        // One pink-bind record (the list node payload at node +0x08 in the original).
        struct SPinkBind {
            STemporaryID id;     // +0x08 (node +0x08)
            uint8_t flag = 0;    // +0x0c (node +0x0c), counter byte minus one
            uint16_t value = 0;  // +0x0e (node +0x0e), the ushort argument
        };
        // List node: next at +0x00, prev at +0x04, record at +0x08 (16 bytes in this port).
        struct SPinkBindNode {
            SPinkBindNode* next = nullptr;  // +0x00
            SPinkBindNode* prev = nullptr;  // +0x04
            SPinkBind data;                 // +0x08
        };

        // 0x5d7c5c. Partial: the constructor sets the name and the word at +0x04. The list heads are
        // set up on first use by addPinkBind (the original sets them in the constructor).
        explicit SCreationState(char* name);
        ~SCreationState();
        SCreationState(const SCreationState&) = delete;
        SCreationState& operator=(const SCreationState&) = delete;

        // 0x5d9788. Adds a pink bind. Fills the node under the cursor when one exists, otherwise
        // appends a new node at the tail and moves the cursor to the end sentinel.
        void addPinkBind(STemporaryID id, uint16_t value);

        // One pending render pass: the 52-byte SRenderPass record of the original (see
        // material_renderer_manager_layout.hpp, "SRenderPass").
        struct SRenderPass {
            uint32_t stateWords[8] = {};                  // +0x00..+0x1f, the 8 words copied from the state argument
                                                          // (the renderpass::SRenderState block, see tb/renderpass_srenderstate.hpp)
            glitch::IReferenceCounted* shader = nullptr;  // +0x20, counted
            int* bindings = nullptr;                      // +0x24, 8 bytes per entry, process-buffer block
            uint32_t unknown28 = 0;                       // +0x28, meaning unknown
            uint16_t unknown2c = 0;                       // +0x2c, meaning unknown
            uint16_t unknown2e = 0;                       // +0x2e, meaning unknown
            uint8_t flag30 = 1;                           // +0x30, one when written by addRenderPass
        };
        // Pending pass list node: next at +0x00, prev at +0x04, record at +0x08.
        struct SPassNode {
            SPassNode* next = nullptr;  // +0x00
            SPassNode* prev = nullptr;  // +0x04
            SRenderPass pass;           // +0x08
        };

        // 0x5d93e8. Partial: see the notes in glitch-video/src/tb_video-glitch_video_CMaterialRendererManager_SCreationS-0.cpp
        // (the shader binding count and the list sentinel setup are not reconstructed yet).
        void addRenderPass(glitch::IReferenceCounted* shader, const detail::renderpass::SRenderState& state);

        char* m_name = nullptr;      // +0x00
        uint32_t m_inProgress04 = 0; // +0x04, non-zero while a technique is open (isCreatingTechnique)
        // +0x24 byte, the pink counter. The record flag is this counter minus one. Other fields
        // of the original (+0x08..+0x23) are not modelled.
        uint8_t m_pinkCounter24 = 0;
        // +0x28 (next) and +0x2c (prev): the sentinel of the pink-bind list. The sentinel is the end
        // of the list. Initially both point at the sentinel itself.
        SPinkBindNode m_pinkSentinel;
        // +0x30: the cursor used by addPinkBind (points at a node or at the sentinel).
        SPinkBindNode* m_pinkCursor = nullptr;
        // +0x10 (next) and +0x14 (prev): sentinel of the pending pass list. The constructor must link it
        // to itself, as the pink sentinel is linked; the port's constructor does not do that yet.
        SPassNode m_passSentinel;
        // +0x18: pass-list cursor (node reused by the next addRenderPass, or the last node written).
        SPassNode* m_passCursor = nullptr;
        // +0x54: running total of 8-byte binding entries of the open technique (addRenderPass adds to it).
        uint32_t m_bindingTotal = 0;
    };

    // 0x5d8f24 (C1) / 0x5d8fa0 (C2). Partial: the SIDedCollection base and the -1 fill of the
    // +0x2c table are not modelled.
    explicit CMaterialRendererManager(IVideoDriver* driver);

    // 0x5d7cf4: returns the id when a creation state is open and the id is not zero, otherwise 0.
    int32_t getTechnique(STemporaryID id) const;
    // 0x5d7d14: the same test and result as getTechnique.
    int32_t getParameterDef(STemporaryID id) const;

    // 0x5db15c. Partial: deletes the creation state; the process-buffer release is a plain delete.
    void clearCreationState();
    // 0x5dc058. Partial: the warning log for a non-null name is not written.
    bool isCreatingTechnique(const char* name) const;

    IVideoDriver* m_driver = nullptr;               // +0x28
    SCreationState* m_creationState = nullptr;      // +0x90, open technique state (null when none)
};

}  // namespace glitch::video
