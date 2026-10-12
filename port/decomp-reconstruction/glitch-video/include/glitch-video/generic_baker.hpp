// glitch::video::CGenericBaker: batch baker built on an IShader. Layout from the constructor
// (0x609d28 C1, 0x609db4 C2): vtable at +0x00, reference word at +0x04, shader at +0x08,
// vertex attribute map at +0x0c (reference counted through its first word), +0x10 halfword
// set to -1, +0x14 word zeroed. The other methods (initParameters, configureAppendBuffer,
// getBatchMaterial, isCompatible, bake) need CMaterial and CAppendMeshBuffer and are not here.
#pragma once

#include <cstdint>

namespace glitch::video {

class IShader;
class CVertexAttributeMap;

class CGenericBaker {
public:
    // 0x609d28 (C1) / 0x609db4 (C2). The attribute map starts unset, as the null-streams branch
    // of the CVertexAttributeMap streams constructor does.
    explicit CGenericBaker(const IShader* shader);

    // The destructor (not reconstructed in this slice) is expected to release the attribute map;
    // until it is written the map reference is not released here.
    virtual ~CGenericBaker() = default;

    uint32_t m_refCount = 0;                       // +0x04
    const IShader* m_shader = nullptr;             // +0x08
    CVertexAttributeMap* m_attributeMap = nullptr; // +0x0c, owned, first word is its reference count
    uint16_t m_unknown10 = 0xffff;                 // +0x10
    uint32_t m_unknown14 = 0;                      // +0x14
};

}  // namespace glitch::video
