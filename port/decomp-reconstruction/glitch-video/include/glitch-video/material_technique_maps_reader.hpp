// glitch::video::IMaterialTechniqueMapsReader: base of the material technique map loader.
// Only the state set by the constructor and setError is laid out here. The virtual error
// hook (vtable +0x28) and the parsing paths are not reconstructed yet.
#pragma once

#include <cstdint>

namespace glitch::video {

class CMaterialRendererManager;

class IMaterialTechniqueMapsReader {
public:
    // E_MAP_GROUP values 0 and 1 index the two map-group slots (see endMapGroup, not reconstructed).
    using E_MAP_GROUP = int32_t;

    // 0x5df8ec (C1) and 0x5df94c (C2): both bodies are the same. The vtable pointer is implicit.
    explicit IMaterialTechniqueMapsReader(CMaterialRendererManager& manager);
    virtual ~IMaterialTechniqueMapsReader() = default;

    // 0x5df8c8: sets the error flag (this +0x16). The original then tail-calls the virtual error hook at
    // vtable +0x28 with (this, message, detail) when the message is non-null, and returns its result; that
    // slot is the 11th virtual, so the eight virtuals before it are needed to declare it (see the ledger row).
    void setError(const char* message, const char* detail);

    CMaterialRendererManager* m_manager = nullptr;  // +0x04
    int32_t m_mapIds[2] = {-1, -1};                 // +0x08 and +0x0c, one id per map group, -1 when unset
    uint16_t m_unknown10 = 0xFFFF;                  // +0x10, not identified
    bool m_mapGroupActive[2] = {false, false};      // +0x12 and +0x13, cleared by endMapGroup
    bool m_unknown14 = false;                       // +0x14, not identified
    bool m_unknown15 = true;                        // +0x15, not identified
    bool m_error = false;                           // +0x16, set by setError
    const void* m_unknown18 = nullptr;              // +0x18, printError passes it to Printer::log
};

}  // namespace glitch::video
