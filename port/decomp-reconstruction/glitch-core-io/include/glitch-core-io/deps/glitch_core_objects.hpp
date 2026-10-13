// Minimal stand-ins, local to glitch-core-io, for the glitch and glitch::video/scene types the io
// code uses. The glitch::core value types are in glitch-shared/glitch_core.hpp (see glitch_core.hpp).
#pragma once

#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>

namespace glitch {
// Reference-counted base; the counter is at +0x04 in the original.
class IReferenceCounted {
public:
    virtual ~IReferenceCounted() = default;
    // Called by drop() right before the object is deleted (vtable +0x08 in the original).
    virtual void onDelete() {}
    // Decrements the count; on zero calls onDelete() and deletes the object.
    // Returns true when the object was deleted. 0x31d584 _ZNK6glitch17IReferenceCounted4dropEv
    bool drop() const;
    mutable int referenceCounter = 1;  // +0x04
};

// Event receiver interface (only its destructor is used by the io code).
class IEventReceiver {
public:
    virtual ~IEventReceiver() = default;  // 0x31d8cc (D1)
};
}  // namespace glitch

namespace glitch::video {
// Stand-in for the driver interface; only its reference counting is used here.
class IVideoDriver : public glitch::IReferenceCounted {};
// Reference-counted video objects held through boost::intrusive_ptr by the io code.
// ITexture keeps a slot sub-type at +0x38; its low two bits pick the parameter code 12..15 that a
// texture is stored under (see material_parameters.hpp, PtrTraits<ITexture>).
class ITexture : public glitch::IReferenceCounted {
public:
    std::uint32_t slotType = 0;  // +0x38
};
class CLight : public glitch::IReferenceCounted {};
// Video mode list of a device (glitch::video::CVideoModeList); only its storage is needed here.
class CVideoModeList {
public:
    CVideoModeList() = default;
};

// CMaterial and CMaterialRenderer are not reference-counted through IReferenceCounted:
// the count is their first member (+0x00), and the release calls the destructor
// directly (no vtable is assumed). The intrusive_ptr release below deletes the object
// when the last use goes; the original also frees it with CustomFree (not modelled).
class CMaterial {
public:
    mutable int referenceCounter = 1;  // +0x00
};
class CMaterialRenderer {
public:
    mutable int referenceCounter = 1;  // +0x00
};

// One stream entry of a CVertexStreams table: 16 bytes in the original (glitch-video's
// SVertexStream). The buffer is a shared_ptr here (the original counts it); this header cannot
// use the counting intrusive_ptr, which includes it.
class IBuffer;
struct SVertexStream {
    std::shared_ptr<IBuffer> buffer;  // +0x00
    std::uint32_t offset = 0;     // +0x04
    std::uint16_t attribute = 0;  // +0x08
    std::uint16_t field0a = 0;    // +0x0a, value type: index into SVertexAttributeTypeInspection::ValueTypeSize
    std::uint16_t field0c = 0;    // +0x0c, component count
    std::uint16_t field0e = 0;    // +0x0e, per-vertex stride
};

// CVertexStreams is released like CMaterial: the count is at +0x00 and the last release
// deletes the object (0x5a10fc; the original calls its destructor first, not modelled).
class CVertexStreams {
public:
    mutable int referenceCounter = 1;  // +0x00
    // SVertexStream entries, 16 bytes each, at +0x14 in the original (its end pointer is at +0x10).
    std::vector<SVertexStream> streams;
    std::uint32_t field08 = 0;  // +0x08, set by loadVS to the vertex count (glitch-video field08)
    std::uint8_t count0c = 0;   // +0x0c, count of the trailing block (24 * count0c + 24 bytes)
};
inline void intrusive_ptr_add_ref(const CVertexStreams* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const CVertexStreams* p) {
    if (--p->referenceCounter == 0) delete p;
}

// Reference-counted video objects that only need the +0x04 drop() release here.
class IShader : public glitch::IReferenceCounted {};      // 0x5b9d6c
class IShaderCode : public glitch::IReferenceCounted {};  // 0x5b9dfc
class IBuffer : public glitch::IReferenceCounted {        // 0x637b8c
public:
    std::uint8_t* data = nullptr;  // +0x08, the mapped or private data (glitch-video IBuffer::m_data)
    std::uint32_t size = 0;        // +0x0c, byte size of data
    std::vector<std::uint8_t> storage;  // stand-in only: owns the bytes that data points to
};
class CImage : public glitch::IReferenceCounted {};       // 0x603414

// Found by argument-dependent lookup from boost::intrusive_ptr (0x310be8, 0x351e3c,
// 0x3522b8 release through these).
inline void intrusive_ptr_add_ref(const CMaterial* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const CMaterial* p) {
    if (--p->referenceCounter == 0) delete p;
}
inline void intrusive_ptr_add_ref(const CMaterialRenderer* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const CMaterialRenderer* p) {
    if (--p->referenceCounter == 0) delete p;
}
}  // namespace glitch::video

namespace glitch::scene {
// Mesh buffer stand-in for the intrusive_ptr release at 0x637bac (drop() at +0x04).
class CMeshBuffer : public glitch::IReferenceCounted {};
}  // namespace glitch::scene
