// glitch::video::detail::IMaterialParameters: the typed parameter accessors of the global material
// parameter manager (CGlobalMaterialParameterManager), CMaterial and CMaterialRenderer. The three
// owners keep their parameter tables in different layouts, so one template serves all of them through
// a storage policy (GlobalParameterStorage, MaterialParameterStorage, RendererParameterStorage). The
// member templates below are the instances queued for this category; each address is listed in the
// map that follows and its status is in ledger.csv.
//
// Overloads of each owner (T is the value type, the type code is in ParamType<T>):
//   getParameter(id, index, T&)      indexed read, needs index < count
//   getParameter(id, T*, stride)     array read of all count elements
//   setParameter(id, index, T const&) indexed write
//   setParameter(id, T const*, stride) array write of all count elements
//   getParameterCvt / setParameterCvt   the same four shapes behind the Convertions gate. A stored
//                                       value of another type is converted when the gate table allows
//                                       it (int <-> float, packed colour <-> float colour, and so on).
//
// Original 32-bit offsets (kept as comments, never used for pointer arithmetic):
//   GlobalParameterStorage (CGlobalMaterialParameterManager): SIDedCollection of SShaderParameterDef
//     at +0x00..+0x26 (vector of 20-byte entries at +0x18), value buffer pointer at +0x2c.
//   MaterialParameterStorage (CMaterial): block pointer at +0x04, dirty-range markers at +0x0c and
//     +0x10 (set to -1 by the array setters, and by an indexed setter that changes the value), block
//     header with count (uint16) at +0x0e and the 16-byte entries at +0x20, values stored inline at
//     +0x20 of the material.
//   RendererParameterStorage (CMaterialRenderer): count (uint16) at +0x0e, entry table at +0x20,
//     value buffer pointer at +0x24.
//
// Stride rule of the array forms: a stride of 0 or the element size copies the elements contiguously
// (the original's memcpy path); any other stride steps the caller's side by that many bytes. A stride
// of 0 with a stored type other than the accessor's own type does nothing and reports success.
//
// Convertions gate (SShaderParameterTypeInspection::Convertions, 19 words at ELF 0x8e2c58, read from
// recovered/native/ida-asm/asm_chunk_018.asm): the stored type code must index the table and carry the
// accessor's bit. Accessor bits: int 0x2, float 0x20, vector2d<int> 0x4, vector3d<int> 0x8,
// vector4d<int> 0x10, vector2d<float> 0x40, vector3d<float> 0x80, vector4d<float> 0x100, SColor 0x10000,
// SColorf 0x20000. Only the accessor's own type and the conversions listed in CvtCodec are accepted.
//
// Original quirks kept as they are in the listing: the indexed Cvt forms check index < count and then
// read or write the entry's first element (the index is not added to the offset); the CMaterial setters
// report success after the gate even when the value is unchanged.
//
// Address map: each queued body of this template, with its instance in parentheses.
//   getParameter(id, index, T&)
//     CGM: int 0x5c3b40, v2i 0x5c3a94, v3i 0x5c39dc, v4i 0x5bfb5c, f 0x5bfabc, v2f 0x5bfa10, v3f 0x5bf958, v4f 0x5bf89c, SColor 0x5bf738, SColorf 0x5bf68c
//     CMat: int 0x5c6e24, v2i 0x5c6e84, v3i 0x5c6ef0, v4i 0x5c6f68, f 0x5c6fe4, v2f 0x5c7044, v3f 0x5c70b0, v4f 0x5c7128, SColor 0x5c71a4, SColorf 0x5c7208
//     CMR: int 0x5cfc88, v2i 0x5cfce4, v3i 0x5cfd44, v4i 0x5cfdb0, f 0x5cfe20, v2f 0x5cfe74, v3f 0x5cfed4, v4f 0x5cff40, SColor 0x5cffb0, SColorf 0x5d0010
//   getParameter(id, T*, stride)
//     CGM: int 0x5c2c80, v2i 0x5c2b8c, v3i 0x5c2a98, v4i 0x5c29a4, f 0x5c28c4, v2f 0x5c27d0, v3f 0x5c26dc, v4f 0x5c25e8, SColor 0x5c23e0, SColorf 0x5c22e8
//     CMat: int 0x5c8d8c, v2i 0x5c8cdc, v3i 0x5c8c28, v4i 0x5c8b74, f 0x5c8ad4, v2f 0x5c8a24, v3f 0x5c8970, v4f 0x5c88bc, SColor 0x5c8804, SColorf 0x5c874c
//     CMR: int 0x5d1b24, v2i 0x5d1a74, v3i 0x5d19c4, v4i 0x5d1914, f 0x5d1878, v2f 0x5d17c8, v3f 0x5d1718, v4f 0x5d1668, SColor 0x5d15b4, SColorf 0x5d1500
//   setParameter(id, index, T const&)
//     CGM: int 0x5c4e70, v2i 0x5c4dc4, v3i 0x5c4d0c, v4i 0x5c4c50, f 0x5c4bb0, v2f 0x5c4b04, v3f 0x5c4a4c, v4f 0x5c4990, SColor 0x5c47fc, SColorf 0x5c4754
//     CMat: int 0x5c623c, v2i 0x5c62b4, v3i 0x5c6350, v4i 0x5c6408, f 0x5c64d4, v2f 0x5c6564, v3f 0x5c660c, v4f 0x5c66d4, SColor 0x5c67b4, SColorf 0x5cabf8
//     CMR: int 0x5cf05c, v2i 0x5cf0b8, v3i 0x5cf118, v4i 0x5cf18c, f 0x5cf1fc, v2f 0x5cf250, v3f 0x5cf2b0, v4f 0x5cf324, SColor 0x5cf394, SColorf 0x5cf3f4
//   setParameter(id, T const*, stride)
//     CGM: int 0x5c083c, v2i 0x5c0748, v3i 0x5c0654, v4i 0x5c0560, f 0x5c0480, v2f 0x5c038c, v3f 0x5c0298, v4f 0x5c01a4, SColor 0x5c00ac, SColorf 0x5bffb4
//     CMat: int 0x5ca260, v2i 0x5ca1a4, v3i 0x5ca0e4, v4i 0x5ca024, f 0x5c9f78, v2f 0x5c9ebc, v3f 0x5c9dfc, v4f 0x5c9d3c, SColor 0x5c9c78, SColorf 0x5c9bb4
//     CMR: int 0x5d2e7c, v2i 0x5d2dcc, v3i 0x5d2d1c, v4i 0x5d2c6c, f 0x5d2bd0, v2f 0x5d2b20, v3f 0x5d2a70, v4f 0x5d29c0, SColor 0x5d290c, SColorf 0x5d2858
//   getParameterCvt(id, index, T&)
//     CGM: int 0x5bf5bc, v2i 0x5bf4fc, v3i 0x5bf434, v4i 0x5bf360, f 0x5bf288, v2f 0x5bf1c8, v3f 0x5bf100, v4f 0x5bef98, SColor 0x5be8e4, SColorf 0x5be794
//     CMat: int 0x5c7274, v2i 0x5c7310, v3i 0x5c739c, v4i 0x5c7430, f 0x5c74d0, v2f 0x5c7574, v3f 0x5c7600, v4f 0x5c7694, SColor 0x5c77c8, SColorf 0x5c7954
//     CMR: int 0x5d0078, v2i 0x5d0110, v3i 0x5d0198, v4i 0x5d0228, f 0x5d02c4, v2f 0x5d0364, v3f 0x5d03ec, v4f 0x5d047c, SColor 0x5d05ac, SColorf 0x5d0734
//   getParameterCvt(id, T*, stride)
//     CGM: int 0x5c2084, v2i 0x5c1f60, v3i 0x5c1e34, v4i 0x5c1d08, f 0x5c1bbc, v2f 0x5c1a98, v3f 0x5c196c, v4f 0x5c1718, SColor 0x5c14b8, SColorf 0x5c12b0
//     CMat: int 0x5c8670, v2i 0x5c8584, v3i 0x5c8490, v4i 0x5c839c, f 0x5c8284, v2f 0x5c8198, v3f 0x5c80a4, v4f 0x5c7e90, SColor 0x5c7c64, SColorf 0x5c7a90
//     CMR: int 0x5d1428, v2i 0x5d1340, v3i 0x5d1250, v4i 0x5d1160, f 0x5d104c, v2f 0x5d0f64, v3f 0x5d0e74, v4f 0x5d0c64, SColor 0x5d0a3c, SColorf 0x5d086c
//   setParameterCvt(id, index, T const&)
//     CGM: int 0x5c4688, v2i 0x5c45c8, v3i 0x5c4500, v4i 0x5c442c, f 0x5c4358, v2f 0x5c4298, v3f 0x5c41d0, v4f 0x5c4054, SColor 0x5c3d5c, SColorf 0x5c3be0
//     CMat: int 0x5c6834, v2i 0x5c6904, v3i 0x5c69c4, v4i 0x5c6a9c, f 0x5c6b8c, v2f 0x5c6c60, v3f 0x5c6d34, v4f 0x5ce768, SColor 0x5cad38, SColorf 0x5cb6e8
//     CMR: int 0x5cf458, v2i 0x5cf4ec, v3i 0x5cf574, v4i 0x5cf604, f 0x5cf6a0, v2f 0x5cf73c, v3f 0x5cf7c4, v4f 0x5cf854, SColor 0x5cf998, SColorf 0x5cfb44
//   setParameterCvt(id, T const*, stride)
//     CGM: int 0x5bfe68, v2i 0x5bfd44, v3i 0x5bfc18, v4i 0x5c38b0, f 0x5c3764, v2f 0x5c3640, v3f 0x5c3514, v4f 0x5c3308, SColor 0x5c30b0, SColorf 0x5c2e9c
//     CMat: int 0x5c9a88, v2i 0x5c9988, v3i 0x5c9880, v4i 0x5c9778, f 0x5c964c, v2f 0x5c954c, v3f 0x5c9444, v4f 0x5c9258, SColor 0x5c9020, SColorf 0x5c8e2c
//     CMR: int 0x5d2744, v2i 0x5d265c, v3i 0x5d256c, v4i 0x5d247c, f 0x5d2368, v2f 0x5d2280, v3f 0x5d2190, v4f 0x5d1fbc, SColor 0x5d1d9c, SColorf 0x5d1bc0
//   Queued instances added in this pass (the address of each body, by member):
//   setPointer: CGM CLight: 0x5be68c, 0x5c5be0; CGM ITexture: 0x5c3f40, 0x5c48a0; CMR CLight: 0x5d4f34, 0x5d5174; CMR ITexture: 0x5d691c, 0x5d76e0; CMat CLight: 0x5cc1b8, 0x5ce95c; CMat ITexture: 0x5cd324, 0x5cec4c
//   getPointer: CGM CLight: 0x5c58ec, 0x5c5a40; CGM ITexture: 0x5beaa4, 0x5bf7dc; CMR CLight: 0x5d50f0, 0x5d7578; CMR ITexture: 0x5d68a0, 0x5d7848; CMat CLight: 0x5cc3b4, 0x5ceae0; CMat ITexture: 0x5cdabc, 0x5cedd0
//   setPointerArray: CGM CLight: 0x5c568c; CGM ITexture: 0x5c0c94; CMR CLight: 0x5d5004; CMR ITexture: 0x5d599c; CMat CLight: 0x5cc2b8; CMat ITexture: 0x5cc8b4
//   getPointerArray: CGM CLight: 0x5c2198; CGM ITexture: 0x5c24d8; CMR CLight: 0x5d5378; CMR ITexture: 0x5d52ac; CMat CLight: 0x5cc50c; CMat ITexture: 0x5cc43c
//   setPointerArrayCvt: CGM CLight: 0x5c540c; CGM ITexture: 0x5c0bd4; CMR CLight: 0x5d56d8; CMR ITexture: 0x5d57e4; CMat CLight: 0x5cce20; CMat ITexture: 0x5cc820
//   getPointerArrayCvt: CGM CLight: 0x5c518c; CGM ITexture: 0x5c4fa8; CMR CLight: 0x5d55cc; CMR ITexture: 0x5d5da4; CMat CLight: 0x5cd0d8; CMat ITexture: 0x5ccc7c
//   setParameterElement: CGM: 0x5beb84, 0x5becc4, 0x5c0aa8; CMR: 0x5d404c, 0x5d4154, 0x5d425c; CMat: 0x5caf68, 0x5cb098, 0x5cb1c4
//   setParameterAt: CMat: 0x5cac8c
//   The CMaterial SColor Cvt body (0x5cad38) performs that helper compare and write inline
//   (CvtCodec<SColor>::same and store). setParameterAt is the free function of that name.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/core/sided_collection.hpp"
#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::video::detail {

// SShaderParameterDef: the 16-byte value of a shader parameter definition (the entry value of the
// global manager's collection; the CMaterial and CMaterialRenderer tables use the same record).
struct SShaderParameterDef {
    const void* sharedString = nullptr;  // +0x00: shared-string reference, non-null when defined
    std::uint16_t unknown04 = 0;         // +0x04: not read by the accessors
    std::uint8_t type = 0;               // +0x06: value type code (see ParamType)
    std::uint32_t count = 0;             // +0x08: number of elements
    std::uint32_t offset = 0;            // +0x0c: byte offset of the first element in the owner's values
};

// Validity of a collection entry value. The collection's entries are unset when the value has no
// shared-string reference.
struct SShaderParamTraits {
    static bool isSet(const SShaderParameterDef& v) { return v.sharedString != nullptr; }
    static int useCount(const SShaderParameterDef&) { return 1; }
};

// Type code and element size of each value type. The codes are the ones tested by the pseudocode
// (char at entry+6). Only the scalar, vector and colour types of the queue are listed here.
template <class T>
struct ParamType;

template <>
struct ParamType<int> {
    static constexpr std::uint8_t code = 1;
    static constexpr std::size_t bytes = 4;
};
template <>
struct ParamType<glitch::core::vector2d<int>> {
    static constexpr std::uint8_t code = 2;
    static constexpr std::size_t bytes = 8;
};
template <>
struct ParamType<glitch::core::vector3d<int>> {
    static constexpr std::uint8_t code = 3;
    static constexpr std::size_t bytes = 12;
};
template <>
struct ParamType<glitch::core::vector4d<int>> {
    static constexpr std::uint8_t code = 4;
    static constexpr std::size_t bytes = 16;
};
template <>
struct ParamType<float> {
    static constexpr std::uint8_t code = 5;
    static constexpr std::size_t bytes = 4;
};
template <>
struct ParamType<glitch::core::vector2d<float>> {
    static constexpr std::uint8_t code = 6;
    static constexpr std::size_t bytes = 8;
};
template <>
struct ParamType<glitch::core::vector3d<float>> {
    static constexpr std::uint8_t code = 7;
    static constexpr std::size_t bytes = 12;
};
template <>
struct ParamType<glitch::core::vector4d<float>> {
    static constexpr std::uint8_t code = 8;
    static constexpr std::size_t bytes = 16;
};
template <>
struct ParamType<glitch::video::SColor> {
    static constexpr std::uint8_t code = 16;
    static constexpr std::size_t bytes = 4;
};
template <>
struct ParamType<glitch::video::SColorf> {
    static constexpr std::uint8_t code = 17;
    static constexpr std::size_t bytes = 16;
};

// Storage of IMaterialParameters<CGlobalMaterialParameterManager, SEmptyBase>.
struct GlobalParameterStorage {
    using Collection = glitch::core::detail::SIDedCollection<SShaderParameterDef, std::uint16_t, false,
                                                             glitch::core::detail::sidedcollection::SEmptyProperties,
                                                             SShaderParamTraits>;
    // The entry for id, or nullptr when the id is out of range or the entry is unset. The collection
    // returns its shared invalid entry for an out-of-range id, which is unset.
    const SShaderParameterDef* definition(std::uint16_t id) const {
        const auto* entry = defs.get(id);
        return SShaderParamTraits::isSet(entry->value) ? &entry->value : nullptr;
    }
    SShaderParameterDef* definition(std::uint16_t id) {
        auto* entry = defs.get(id);
        return SShaderParamTraits::isSet(entry->value) ? &entry->value : nullptr;
    }
    const std::uint8_t* data(std::uint32_t off) const { return values.data() + off; }  // +0x2c
    std::uint8_t* data(std::uint32_t off) { return values.data() + off; }
    // The global manager keeps no dirty range.
    void noteIndexed(bool) {}
    void noteArray() {}

    Collection defs;                   // +0x00..+0x26
    std::vector<std::uint8_t> values;  // +0x2c: value buffer
};

// Block header of a CMaterial (the parameter table of its ISharedMemoryBlockHeader).
struct SParameterBlock {
    std::uint16_t count = 0;                        // +0x0e
    std::vector<SShaderParameterDef> entries;       // +0x20: 16-byte entries
};

// Storage of IMaterialParameters<CMaterial, ISharedMemoryBlockHeader<CMaterial>>.
struct MaterialParameterStorage {
    const SShaderParameterDef* definition(std::uint16_t id) const {
        if (!block || id >= block->count || id >= block->entries.size()) return nullptr;
        return &block->entries[id];
    }
    SShaderParameterDef* definition(std::uint16_t id) {
        if (!block || id >= block->count || id >= block->entries.size()) return nullptr;
        return &block->entries[id];
    }
    const std::uint8_t* data(std::uint32_t off) const { return inlineValues.data() + off; }  // +0x20 + off
    std::uint8_t* data(std::uint32_t off) { return inlineValues.data() + off; }
    // An indexed setter marks the range only when the stored value changes (the ARM compares first).
    void noteIndexed(bool changed) {
        if (changed) markDirty();
    }
    // The array setters always mark the range (the ARM stores -1 to +0x0c and +0x10 after the gate).
    void noteArray() { markDirty(); }
    void markDirty() {
        dirtyFirst = -1;
        dirtyLast = -1;
    }

    SParameterBlock* block = nullptr;  // +0x04: not owned here
    std::int32_t dirtyFirst = -1;      // +0x0c
    std::int32_t dirtyLast = -1;       // +0x10
    std::vector<std::uint8_t> inlineValues;  // +0x20: values follow the header in the material
};

// Storage of IMaterialParameters<CMaterialRenderer, ISharedMemoryBlockHeader<CMaterialRenderer>>.
struct RendererParameterStorage {
    const SShaderParameterDef* definition(std::uint16_t id) const {
        if (id >= count || id >= entries.size()) return nullptr;
        return &entries[id];
    }
    SShaderParameterDef* definition(std::uint16_t id) {
        if (id >= count || id >= entries.size()) return nullptr;
        return &entries[id];
    }
    const std::uint8_t* data(std::uint32_t off) const { return values.data() + off; }  // +0x24
    std::uint8_t* data(std::uint32_t off) { return values.data() + off; }
    // The renderer keeps no dirty range.
    void noteIndexed(bool) {}
    void noteArray() {}

    std::uint16_t count = 0;                   // +0x0e
    std::vector<SShaderParameterDef> entries;  // +0x20: entry table
    std::vector<std::uint8_t> values;          // +0x24: value buffer
};

// Float helpers of the original, as the listing uses them.
inline float bitsToFloat(std::uint32_t bits) {
    float f;
    std::memcpy(&f, &bits, sizeof f);
    return f;
}

// Byte to float: the byte is converted to float and multiplied by 1/255 (constant 0x3B808081).
inline float byteToFloat(std::uint8_t b) { return static_cast<float>(b) * bitsToFloat(0x3B808081u); }

// Float to byte: the value is multiplied by 255.0 (0x437F0000) and converted with __aeabi_f2uiz, which
// saturates (NaN and negative values give 0, values of 2^32 and above give UINT32_MAX); the byte is the
// low 8 bits of that.
inline std::uint32_t f2uiz(float v) {
    if (!(v > 0.0f)) return 0;
    if (v >= 4294967296.0f) return 0xFFFFFFFFu;
    return static_cast<std::uint32_t>(v);
}
inline std::uint8_t floatToByte(float v) {
    return static_cast<std::uint8_t>(f2uiz(v * bitsToFloat(0x437F0000u)));
}

// Float to int32 with __aeabi_f2iz saturation (truncation toward zero; NaN gives 0).
inline std::int32_t f2iz(float v) {
    if (!(v == v)) return 0;
    if (v >= 2147483648.0f) return 0x7FFFFFFF;
    if (v <= -2147483648.0f) return static_cast<std::int32_t>(0x80000000u);
    return static_cast<std::int32_t>(v);
}

// SShaderParameterTypeInspection::Convertions (19 words, ELF 0x8e2c58): the gate bits per stored type code.
inline constexpr std::uint32_t kConvertions[19] = {
    0x1,    0x22,    0x4,    0x8,    0x10,   0x22,   0x40,    0x80,    0x30100, 0x200,
    0x400,  0x800,   0x1000, 0x2000, 0x4000, 0x8000, 0x30100, 0x30100, 0x40000};

// SShaderParameterTypeInspection::ValueTypeSize (20 bytes): the element size of each stored type code.
inline constexpr std::uint8_t kValueTypeSize[20] = {1, 4, 8, 12, 16, 4, 8, 12, 16, 16,
                                                    36, 4, 4, 4, 4, 4, 4, 16, 4, 0};

// True when the stored type code passes the Convertions gate of the accessor (Codec::kGate). A code
// above the table reads past it in the original; the port refuses it (see NOTES.md).
template <class Codec>
bool convertible(std::uint8_t code) {
    return code < 19 && (kConvertions[code] & Codec::kGate) != 0;
}

// Value equality used by the indexed setters of CMaterial (see MaterialParameterStorage). Integers
// compare bitwise (the same as ==); floats compare with == (fcmpeq), so NaN never matches.
template <class T>
struct ParamEquals {
    static bool equal(const T& a, const T& b) { return std::memcmp(&a, &b, sizeof(T)) == 0; }
};
template <>
struct ParamEquals<float> {
    static bool equal(float a, float b) { return a == b; }
};
template <>
struct ParamEquals<glitch::core::vector2d<float>> {
    static bool equal(const glitch::core::vector2d<float>& a, const glitch::core::vector2d<float>& b) {
        return a.X == b.X && a.Y == b.Y;
    }
};
template <>
struct ParamEquals<glitch::core::vector3d<float>> {
    static bool equal(const glitch::core::vector3d<float>& a, const glitch::core::vector3d<float>& b) {
        return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
    }
};
template <>
struct ParamEquals<glitch::core::vector4d<float>> {
    static bool equal(const glitch::core::vector4d<float>& a, const glitch::core::vector4d<float>& b) {
        return a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;
    }
};

// SColorf::equals (ELF 0x5caa98): one component matches when the new value lies within 1e-6 (0x358637BD)
// of the stored one, on both sides. Called as equals(stored, value).
inline bool withinTolerance(float stored, float value) {
    const float tol = bitsToFloat(0x358637BDu);
    return value <= stored + tol && value >= stored - tol;
}
template <>
struct ParamEquals<glitch::video::SColorf> {
    static bool equal(const glitch::video::SColorf& a, const glitch::video::SColorf& b) {
        return withinTolerance(a.r, b.r) && withinTolerance(a.g, b.g) && withinTolerance(a.b, b.b) &&
               withinTolerance(a.a, b.a);
    }
};

// Conversions of one value type T with the stored representation (type code c, element size
// kValueTypeSize[c]). Each codec mirrors the branch chain of the matching Hex-Rays body: the gate bit,
// the stored types it converts, and the rule for a stored type it does not name (nothing is written).
// `same` reports whether a stored element already holds the value, which is the CMaterial compare
// that decides whether the dirty range is marked.
template <class T>
struct CvtCodec;

// int (gate 0x2): int stored as is, float stored converted with f2iz. The array read copies ints only.
template <>
struct CvtCodec<int> {
    static constexpr std::uint32_t kGate = 0x2;
    static constexpr std::uint8_t kSame = 1;
    static bool arrayLoads(std::uint8_t c) { return c == kSame; }
    static void load(const std::uint8_t* p, std::uint8_t c, int& out) {
        if (c == kSame) {
            std::memcpy(&out, p, sizeof out);
        } else if (c == 5) {
            float f;
            std::memcpy(&f, p, sizeof f);
            out = f2iz(f);
        }
    }
    static void store(std::uint8_t* p, std::uint8_t c, int v) {
        if (c == kSame) {
            std::memcpy(p, &v, sizeof v);
        } else if (c == 5) {
            const float f = static_cast<float>(v);
            std::memcpy(p, &f, sizeof f);
        }
    }
    static bool same(const std::uint8_t* p, std::uint8_t c, int v) {
        if (c == kSame) {
            int s;
            std::memcpy(&s, p, sizeof s);
            return s == v;
        }
        if (c == 5) {
            float s;
            std::memcpy(&s, p, sizeof s);
            return s == static_cast<float>(v);
        }
        return true;
    }
};

// float (gate 0x20): int stored converted to float; float stored as is. A float stored into an int slot
// goes through f2iz.
template <>
struct CvtCodec<float> {
    static constexpr std::uint32_t kGate = 0x20;
    static constexpr std::uint8_t kSame = 5;
    static bool arrayLoads(std::uint8_t) { return true; }
    static void load(const std::uint8_t* p, std::uint8_t c, float& out) {
        if (c == 1) {
            std::int32_t i;
            std::memcpy(&i, p, sizeof i);
            out = static_cast<float>(i);
        } else if (c == kSame) {
            std::memcpy(&out, p, sizeof out);
        }
    }
    static void store(std::uint8_t* p, std::uint8_t c, float v) {
        if (c == 1) {
            const std::int32_t i = f2iz(v);
            std::memcpy(p, &i, sizeof i);
        } else if (c == kSame) {
            std::memcpy(p, &v, sizeof v);
        }
    }
    static bool same(const std::uint8_t* p, std::uint8_t c, float v) {
        if (c == 1) {
            std::int32_t s;
            std::memcpy(&s, p, sizeof s);
            return s == f2iz(v);
        }
        if (c == kSame) {
            float s;
            std::memcpy(&s, p, sizeof s);
            return s == v;
        }
        return true;
    }
};

// Vector types that convert nothing: the stored type must be the accessor's own type.
template <class T, std::uint32_t Gate>
struct CvtSameCodec {
    static constexpr std::uint32_t kGate = Gate;
    static constexpr std::uint8_t kSame = ParamType<T>::code;
    static bool arrayLoads(std::uint8_t) { return true; }
    static void load(const std::uint8_t* p, std::uint8_t c, T& out) {
        if (c == kSame) std::memcpy(&out, p, sizeof(T));
    }
    static void store(std::uint8_t* p, std::uint8_t c, const T& v) {
        if (c == kSame) std::memcpy(p, &v, sizeof(T));
    }
    static bool same(const std::uint8_t* p, std::uint8_t c, const T& v) {
        if (c != kSame) return true;
        T s;
        std::memcpy(&s, p, sizeof(T));
        return ParamEquals<T>::equal(s, v);
    }
};
template <>
struct CvtCodec<glitch::core::vector2d<int>> : CvtSameCodec<glitch::core::vector2d<int>, 0x4> {};
template <>
struct CvtCodec<glitch::core::vector3d<int>> : CvtSameCodec<glitch::core::vector3d<int>, 0x8> {};
template <>
struct CvtCodec<glitch::core::vector4d<int>> : CvtSameCodec<glitch::core::vector4d<int>, 0x10> {};
template <>
struct CvtCodec<glitch::core::vector2d<float>> : CvtSameCodec<glitch::core::vector2d<float>, 0x40> {};
template <>
struct CvtCodec<glitch::core::vector3d<float>> : CvtSameCodec<glitch::core::vector3d<float>, 0x80> {};

// vector4d<float> (gate 0x100): packed colour (16) as bytes, float4 (8) and SColorf (17) as floats.
template <>
struct CvtCodec<glitch::core::vector4d<float>> {
    static constexpr std::uint32_t kGate = 0x100;
    static constexpr std::uint8_t kSame = 8;
    static bool arrayLoads(std::uint8_t) { return true; }
    static void load(const std::uint8_t* p, std::uint8_t c, glitch::core::vector4d<float>& out) {
        if (c == 16) {
            out.X = byteToFloat(p[0]);
            out.Y = byteToFloat(p[1]);
            out.Z = byteToFloat(p[2]);
            out.W = byteToFloat(p[3]);
        } else if (c == kSame || c == 17) {
            std::memcpy(&out, p, sizeof out);
        }
    }
    static void store(std::uint8_t* p, std::uint8_t c, const glitch::core::vector4d<float>& v) {
        if (c == 16) {
            p[0] = floatToByte(v.X);
            p[1] = floatToByte(v.Y);
            p[2] = floatToByte(v.Z);
            p[3] = floatToByte(v.W);
        } else if (c == kSame || c == 17) {
            std::memcpy(p, &v, sizeof v);
        }
    }
    static bool same(const std::uint8_t* p, std::uint8_t c, const glitch::core::vector4d<float>& v) {
        glitch::core::vector4d<float> s;
        if (c == 16) {
            return p[0] == floatToByte(v.X) && p[1] == floatToByte(v.Y) && p[2] == floatToByte(v.Z) &&
                   p[3] == floatToByte(v.W);
        }
        if (c == 17) {
            std::memcpy(&s, p, sizeof s);
            return withinTolerance(s.X, v.X) && withinTolerance(s.Y, v.Y) && withinTolerance(s.Z, v.Z) &&
                   withinTolerance(s.W, v.W);
        }
        if (c == kSame) {
            std::memcpy(&s, p, sizeof s);
            return ParamEquals<glitch::core::vector4d<float>>::equal(s, v);
        }
        return true;
    }
};

// SColor (gate 0x10000): the packed bytes as stored (16); float colours (8, 17) converted to bytes.
// The bytes of an SColor are its memory image, so the conversions work on the bytes.
template <>
struct CvtCodec<glitch::video::SColor> {
    static constexpr std::uint32_t kGate = 0x10000;
    static constexpr std::uint8_t kSame = 16;
    static bool arrayLoads(std::uint8_t) { return true; }
    static float floatAt(const std::uint8_t* p, int i) {
        float f;
        std::memcpy(&f, p + 4 * i, sizeof f);
        return f;
    }
    static void load(const std::uint8_t* p, std::uint8_t c, glitch::video::SColor& out) {
        if (c == kSame) {
            std::memcpy(&out, p, sizeof out);
        } else if (c == 8 || c == 17) {
            std::uint8_t b[4];
            for (int i = 0; i < 4; ++i) b[i] = floatToByte(floatAt(p, i));
            std::memcpy(&out, b, sizeof out);
        }
    }
    static void store(std::uint8_t* p, std::uint8_t c, const glitch::video::SColor& v) {
        if (c == kSame) {
            std::memcpy(p, &v, sizeof v);
        } else if (c == 8 || c == 17) {
            std::uint8_t b[4];
            std::memcpy(b, &v, sizeof b);
            for (int i = 0; i < 4; ++i) {
                const float f = byteToFloat(b[i]);
                std::memcpy(p + 4 * i, &f, sizeof f);
            }
        }
    }
    static bool same(const std::uint8_t* p, std::uint8_t c, const glitch::video::SColor& v) {
        std::uint8_t b[4];
        std::memcpy(b, &v, sizeof b);
        if (c == kSame) return std::memcmp(p, &v, sizeof v) == 0;
        if (c == 17) {
            for (int i = 0; i < 4; ++i) {
                if (!withinTolerance(floatAt(p, i), byteToFloat(b[i]))) return false;
            }
            return true;
        }
        if (c == 8) {
            for (int i = 0; i < 4; ++i) {
                if (floatAt(p, i) != byteToFloat(b[i])) return false;
            }
            return true;
        }
        return true;
    }
};

// SColorf (gate 0x20000): packed colour (16) converted to floats; float colours (8, 17) as stored.
template <>
struct CvtCodec<glitch::video::SColorf> {
    static constexpr std::uint32_t kGate = 0x20000;
    static constexpr std::uint8_t kSame = 17;
    static bool arrayLoads(std::uint8_t) { return true; }
    static void load(const std::uint8_t* p, std::uint8_t c, glitch::video::SColorf& out) {
        if (c == 16) {
            out.r = byteToFloat(p[0]);
            out.g = byteToFloat(p[1]);
            out.b = byteToFloat(p[2]);
            out.a = byteToFloat(p[3]);
        } else if (c == kSame || c == 8) {
            std::memcpy(&out, p, sizeof out);
        }
    }
    static void store(std::uint8_t* p, std::uint8_t c, const glitch::video::SColorf& v) {
        if (c == 16) {
            p[0] = floatToByte(v.r);
            p[1] = floatToByte(v.g);
            p[2] = floatToByte(v.b);
            p[3] = floatToByte(v.a);
        } else if (c == kSame || c == 8) {
            std::memcpy(p, &v, sizeof v);
        }
    }
    static bool same(const std::uint8_t* p, std::uint8_t c, const glitch::video::SColorf& v) {
        glitch::video::SColorf s;
        if (c == 16) {
            return p[0] == floatToByte(v.r) && p[1] == floatToByte(v.g) && p[2] == floatToByte(v.b) &&
                   p[3] == floatToByte(v.a);
        }
        if (c == kSame) {
            std::memcpy(&s, p, sizeof s);
            return ParamEquals<glitch::video::SColorf>::equal(s, v);
        }
        if (c == 8) {
            std::memcpy(&s, p, sizeof s);
            return s.r == v.r && s.g == v.g && s.b == v.b && s.a == v.a;
        }
        return true;
    }
};

// Base type and element count used by setParameterElement (ValueTypeBaseType and ValueTypeArraySize,
// neither table is in the container). The base types are read from the pseudocode gates: 0 for bytes,
// 1 for int, 5 for float. Counts are the components of each type; the matrix (11) has 16 floats. Codes
// not listed here are refused. Code 9 and 10 are not confirmed (see NOTES.md).
inline std::uint8_t valueBaseType(std::uint8_t code) {
    switch (code) {
        case 1: case 2: case 3: case 4:
            return 1;
        case 5: case 6: case 7: case 8: case 10: case 11: case 17:
            return 5;
        case 16:
            return 0;
        default:
            return 0xFF;
    }
}
inline std::uint32_t valueArraySize(std::uint8_t code) {
    switch (code) {
        case 1: case 5: return 1;
        case 2: case 6: return 2;
        case 3: case 7: return 3;
        case 4: case 8: case 16: case 17: return 4;
        case 10: return 9;
        case 11: return 16;
        default: return 0;
    }
}

// Element traits of setParameterElement: the base type each value must have.
template <class T>
struct ElementTraits;
template <>
struct ElementTraits<std::uint8_t> {
    static constexpr std::uint8_t kBase = 0;
};
template <>
struct ElementTraits<int> {
    static constexpr std::uint8_t kBase = 1;
};
template <>
struct ElementTraits<float> {
    static constexpr std::uint8_t kBase = 5;
};

// Pointer-valued parameters. The original stores a 4-byte pointer per element and keeps a use on the
// object; the port uses sizeof(void*) bytes per element and the same use counting (see
// boost/intrusive_ptr.hpp). PtrTraits gives the stored type codes each object kind accepts.
template <class P>
struct PtrTraits;

// ITexture: codes 12..15 (the four switch cases). A non-null value must be stored under its own
// sub-type code, 12 + (slotType & 3); a null value accepts any of the four codes.
template <>
struct PtrTraits<boost::intrusive_ptr<glitch::video::ITexture>> {
    using Object = glitch::video::ITexture;
    static bool storedOk(std::uint8_t c) { return c - 12u <= 3u; }
    static bool setOk(std::uint8_t c, const Object* v) {
        return v ? c == 12u + (v->slotType & 3u) : storedOk(c);
    }
};

// CLight: code 18 only (the Convertions bit 0x40000 selects that code alone).
template <>
struct PtrTraits<boost::intrusive_ptr<glitch::video::CLight>> {
    using Object = glitch::video::CLight;
    static bool storedOk(std::uint8_t c) { return c == 18; }
    static bool setOk(std::uint8_t c, const Object*) { return c == 18; }
};

inline constexpr std::size_t kPtrSlot = sizeof(void*);

template <class T>
T* loadRaw(const std::uint8_t* slot) {
    T* p;
    std::memcpy(&p, slot, sizeof p);
    return p;
}
template <class T>
void storeRaw(std::uint8_t* slot, T* p) {
    std::memcpy(slot, &p, sizeof p);
}
// The slot takes a use on the new object and drops the one it held (boost::intrusive_ptr assignment).
template <class T>
void assignSlot(std::uint8_t* slot, T* value) {
    T* old = loadRaw<T>(slot);
    if (value) boost::intrusive_ptr_add_ref(value);
    storeRaw(slot, value);
    if (old) boost::intrusive_ptr_release(old);
}

// CMaterial::setParameterAt<SColorf>(SColorf* dst, SColor const& src) (0x5cac8c). Each byte of the
// colour becomes byte * (1/255); the dirty range is marked when the stored value differs beyond the
// SColorf::equals tolerance. The source's first byte is red, the fourth is alpha.
inline void setParameterAt(MaterialParameterStorage& storage, glitch::video::SColorf* dst,
                           const glitch::video::SColor& src) {
    const std::uint8_t* b = reinterpret_cast<const std::uint8_t*>(&src);
    const glitch::video::SColorf value{byteToFloat(b[0]), byteToFloat(b[1]), byteToFloat(b[2]),
                                       byteToFloat(b[3])};
    if (!ParamEquals<glitch::video::SColorf>::equal(*dst, value)) storage.markDirty();
    *dst = value;
}

template <class Storage>
class IMaterialParameters {
public:
    Storage storage;

    // getParameter(id, index, T&) : indexed read. 0x5c3b40 (CGM int) and the other indexed readers.
    template <class T>
    bool getParameter(std::uint16_t id, std::uint32_t index, T& out) const {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || def->type != ParamType<T>::code || index >= def->count) return false;
        std::memcpy(&out, storage.data(def->offset + index * static_cast<std::uint32_t>(ParamType<T>::bytes)),
                    sizeof(T));
        return true;
    }

    // getParameter(id, T*, stride) : array read of all elements into the caller's array.
    template <class T>
    bool getParameter(std::uint16_t id, T* out, int stride) const {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || def->type != ParamType<T>::code) return false;
        copyElements(reinterpret_cast<std::uint8_t*>(out), arrayStep(stride, ParamType<T>::bytes),
                     storage.data(def->offset), static_cast<std::ptrdiff_t>(ParamType<T>::bytes), def->count,
                     ParamType<T>::bytes);
        return true;
    }

    // setParameter(id, index, T const&) : indexed write. The value is always written; on CMaterial the
    // dirty range is marked only when the stored value differs.
    template <class T>
    bool setParameter(std::uint16_t id, std::uint32_t index, const T& value) {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || def->type != ParamType<T>::code || index >= def->count) return false;
        std::uint8_t* dst = storage.data(def->offset + index * static_cast<std::uint32_t>(ParamType<T>::bytes));
        T current{};
        std::memcpy(&current, dst, sizeof(T));
        storage.noteIndexed(!ParamEquals<T>::equal(current, value));
        std::memcpy(dst, &value, sizeof(T));
        return true;
    }

    // setParameter(id, T const*, stride) : array write of all elements from the caller's array.
    template <class T>
    bool setParameter(std::uint16_t id, const T* src, int stride) {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || def->type != ParamType<T>::code) return false;
        storage.noteArray();
        copyElements(storage.data(def->offset), static_cast<std::ptrdiff_t>(ParamType<T>::bytes),
                     reinterpret_cast<const std::uint8_t*>(src), arrayStep(stride, ParamType<T>::bytes),
                     def->count, ParamType<T>::bytes);
        return true;
    }

    // getParameterCvt(id, index, T&). Checks index < count, then reads the entry's first element: the
    // original does not add the index to the offset (the ARM listing reuses the index register).
    template <class T>
    bool getParameterCvt(std::uint16_t id, std::uint32_t index, T& out) const {
        using C = CvtCodec<T>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !convertible<C>(def->type) || index >= def->count) return false;
        C::load(storage.data(def->offset), def->type, out);
        return true;
    }

    // getParameterCvt(id, T*, stride). A stored type that is not converted by this accessor copies
    // nothing for an int array read; a zero stride copies only the accessor's own type.
    template <class T>
    bool getParameterCvt(std::uint16_t id, T* out, int stride) const {
        using C = CvtCodec<T>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !convertible<C>(def->type)) return false;
        const std::uint8_t c = def->type;
        if (!C::arrayLoads(c)) return true;
        if (stride == 0 && c != C::kSame) return true;
        const std::ptrdiff_t step = arrayStep(stride, sizeof(T));
        const std::uint8_t* src = storage.data(def->offset);
        const std::size_t srcStep = kValueTypeSize[c];
        auto* dst = reinterpret_cast<std::uint8_t*>(out);
        for (std::uint32_t i = 0; i < def->count; ++i) {
            T v{};
            C::load(src + i * srcStep, c, v);
            std::memcpy(dst + static_cast<std::ptrdiff_t>(i) * step, &v, sizeof(T));
        }
        return true;
    }

    // setParameterCvt(id, index, T const&). Checks index < count, then writes the entry's first element.
    // The dirty range is marked when the stored value differs (CMaterial), before the write.
    template <class T>
    bool setParameterCvt(std::uint16_t id, std::uint32_t index, const T& value) {
        using C = CvtCodec<T>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !convertible<C>(def->type) || index >= def->count) return false;
        std::uint8_t* dst = storage.data(def->offset);
        storage.noteIndexed(!C::same(dst, def->type, value));
        C::store(dst, def->type, value);
        return true;
    }

    // setParameterCvt(id, T const*, stride). The dirty range is marked before the stride rules, as the
    // ARM does after the gate. A zero stride copies only the accessor's own type.
    template <class T>
    bool setParameterCvt(std::uint16_t id, const T* src, int stride) {
        using C = CvtCodec<T>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !convertible<C>(def->type)) return false;
        const std::uint8_t c = def->type;
        storage.noteArray();
        if (stride == 0 && c != C::kSame) return true;
        const std::ptrdiff_t step = arrayStep(stride, sizeof(T));
        const auto* in = reinterpret_cast<const std::uint8_t*>(src);
        std::uint8_t* dst = storage.data(def->offset);
        const std::size_t dstStep = kValueTypeSize[c];
        for (std::uint32_t i = 0; i < def->count; ++i) {
            T v{};
            std::memcpy(&v, in + static_cast<std::ptrdiff_t>(i) * step, sizeof(T));
            C::store(dst + i * dstStep, c, v);
        }
        return true;
    }

    // setParameterElement<T>(id, index, component, value): one component of an element. The base type
    // must match and the component must be inside the value type. A matrix (type 11) is a pointer slot:
    // the matrix is allocated on first use with the identity and its flag is cleared. A byte element
    // is addressed by index + component (the original's byte form), int and float by 4 * (index +
    // component). The matrix allocation is not released by this class (partial, see NOTES.md).
    template <class T>
    bool setParameterElement(std::uint16_t id, std::uint32_t index, std::uint8_t component, T value) {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || valueBaseType(def->type) != ElementTraits<T>::kBase ||
            component >= valueArraySize(def->type) || index >= def->count) {
            return false;
        }
        std::uint8_t* slot = storage.data(def->offset);
        if (def->type == 11) {
            auto* matrix = loadRaw<glitch::core::CMatrix4<float>>(slot);
            if (!matrix) {
                matrix = new glitch::core::CMatrix4<float>(glitch::core::CMatrix4<float>::EM4CONST_IDENTITY);
                storeRaw(slot, matrix);
            }
            const float v = static_cast<float>(value);
            storage.noteIndexed(matrix->M[component] != v);
            matrix->flag = false;
            matrix->M[component] = v;
            return true;
        }
        std::uint8_t* dst;
        if constexpr (sizeof(T) == 1) {
            dst = storage.data(def->offset + index + component);
            storage.noteIndexed(*dst != static_cast<std::uint8_t>(value));
            *dst = static_cast<std::uint8_t>(value);
        } else {
            dst = storage.data(def->offset + 4u * (index + component));
            const T current = [&] {
                T t;
                std::memcpy(&t, dst, sizeof t);
                return t;
            }();
            storage.noteIndexed(current != value);
            std::memcpy(dst, &value, sizeof value);
        }
        return true;
    }

    // getParameter(id, index, Ptr&) and getParameterCvt(id, index, Ptr&) for intrusive_ptr slots.
    // Both forms are the same here: the Cvt gate for pointers is the stored-code check alone.
    template <class T>
    bool getPointer(std::uint16_t id, std::uint32_t index, boost::intrusive_ptr<T>& out) const {
        using Traits = PtrTraits<boost::intrusive_ptr<T>>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !Traits::storedOk(def->type) || index >= def->count) return false;
        out = loadRaw<T>(storage.data(def->offset + static_cast<std::uint32_t>(kPtrSlot) * index));
        return true;
    }

    // getParameter(id, Ptr*, stride): element i is written to out + i * stride (0 means the element size).
    // The original copies the raw slots when the stride is 0 or 4; the port assigns each element so the
    // uses stay balanced (see NOTES.md).
    template <class T>
    bool getPointerArray(std::uint16_t id, boost::intrusive_ptr<T>* out, int stride) const {
        using Traits = PtrTraits<boost::intrusive_ptr<T>>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !Traits::storedOk(def->type)) return false;
        const std::ptrdiff_t step = stride == 0 ? static_cast<std::ptrdiff_t>(sizeof(out[0])) : stride;
        auto* dst = reinterpret_cast<std::uint8_t*>(out);
        const std::uint8_t* src = storage.data(def->offset);
        for (std::uint32_t i = 0; i < def->count; ++i) {
            auto& element = *reinterpret_cast<boost::intrusive_ptr<T>*>(dst + static_cast<std::ptrdiff_t>(i) * step);
            element = loadRaw<T>(src + i * kPtrSlot);
        }
        return true;
    }

    // getParameterCvt(id, Ptr*, stride): as getPointerArray, but a zero stride copies nothing.
    template <class T>
    bool getPointerArrayCvt(std::uint16_t id, boost::intrusive_ptr<T>* out, int stride) const {
        if (stride == 0) {
            const SShaderParameterDef* def = storage.definition(id);
            return def && PtrTraits<boost::intrusive_ptr<T>>::storedOk(def->type);
        }
        return getPointerArray(id, out, stride);
    }

    // setParameter(id, index, Ptr const&) / setParameterCvt(id, index, Ptr const&). On CMaterial the
    // dirty range is marked when the stored object changes.
    template <class T>
    bool setPointer(std::uint16_t id, std::uint32_t index, const boost::intrusive_ptr<T>& value) {
        using Traits = PtrTraits<boost::intrusive_ptr<T>>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !Traits::setOk(def->type, value.get()) || index >= def->count) return false;
        std::uint8_t* slot = storage.data(def->offset + static_cast<std::uint32_t>(kPtrSlot) * index);
        storage.noteIndexed(loadRaw<T>(slot) != value.get());
        assignSlot(slot, value.get());
        return true;
    }

    // setParameter(id, Ptr const*, stride): stride 0 means the element size. Marks the range first.
    template <class T>
    bool setPointerArray(std::uint16_t id, const boost::intrusive_ptr<T>* src, int stride) {
        using Traits = PtrTraits<boost::intrusive_ptr<T>>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !Traits::storedOk(def->type)) return false;
        storage.noteArray();
        const std::ptrdiff_t step = stride == 0 ? static_cast<std::ptrdiff_t>(sizeof(src[0])) : stride;
        const auto* in = reinterpret_cast<const std::uint8_t*>(src);
        std::uint8_t* dst = storage.data(def->offset);
        for (std::uint32_t i = 0; i < def->count; ++i) {
            const auto& element = *reinterpret_cast<const boost::intrusive_ptr<T>*>(in + static_cast<std::ptrdiff_t>(i) * step);
            assignSlot(dst + i * kPtrSlot, element.get());
        }
        return true;
    }

    // setParameterCvt(id, Ptr const*, stride): a zero stride marks the range and copies nothing.
    template <class T>
    bool setPointerArrayCvt(std::uint16_t id, const boost::intrusive_ptr<T>* src, int stride) {
        using Traits = PtrTraits<boost::intrusive_ptr<T>>;
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || !Traits::storedOk(def->type)) return false;
        storage.noteArray();
        if (stride == 0) return true;
        return setPointerArray(id, src, stride);
    }

private:
    // Copies count elements of bytes each. dstStep and srcStep are byte steps between elements, so a
    // step equal to bytes is the contiguous copy of the original's memcpy path.
    static void copyElements(std::uint8_t* dst, std::ptrdiff_t dstStep, const std::uint8_t* src,
                             std::ptrdiff_t srcStep, std::uint32_t count, std::size_t bytes) {
        for (std::uint32_t i = 0; i < count; ++i) {
            std::memcpy(dst + static_cast<std::ptrdiff_t>(i) * dstStep,
                        src + static_cast<std::ptrdiff_t>(i) * srcStep, bytes);
        }
    }

    // The element step of an array argument: stride 0 means the elements are contiguous.
    static std::ptrdiff_t arrayStep(int stride, std::size_t bytes) {
        return stride == 0 ? static_cast<std::ptrdiff_t>(bytes) : static_cast<std::ptrdiff_t>(stride);
    }
};

using GlobalMaterialParameters = IMaterialParameters<GlobalParameterStorage>;    // CGlobalMaterialParameterManager
using MaterialParameters = IMaterialParameters<MaterialParameterStorage>;       // CMaterial
using RendererParameters = IMaterialParameters<RendererParameterStorage>;       // CMaterialRenderer

}  // namespace glitch::video::detail
