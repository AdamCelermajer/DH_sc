// glitch::video::CMaterialRendererManager. Only the parts that the port can express are
// reconstructed here: the two lookups that take a temporary id, the creation-state slot, and
// the constructor. The technique, render-pass and parameter tables of the original sit on
// SIDedCollection and SSharedString red-black trees, which are not in the port.
#pragma once

#include <cstdint>

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
        // 0x5d7c5c. Partial: the list heads are not built; the name pointer and the word at +0x04 are set.
        explicit SCreationState(char* name);

        char* m_name = nullptr;      // +0x00
        uint32_t m_inProgress04 = 0; // +0x04, non-zero while a technique is open (isCreatingTechnique)
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
