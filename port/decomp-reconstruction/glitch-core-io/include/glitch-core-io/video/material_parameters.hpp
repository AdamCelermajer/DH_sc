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
//   getParameterCvt / setParameterCvt   the same four shapes behind the Convertions gate (partial)
//
// Original 32-bit offsets (kept as comments, never used for pointer arithmetic):
//   GlobalParameterStorage (CGlobalMaterialParameterManager): SIDedCollection of SShaderParameterDef
//     at +0x00..+0x26 (vector of 20-byte entries at +0x18), value buffer pointer at +0x2c.
//   MaterialParameterStorage (CMaterial): block pointer at +0x04, dirty-range markers at +0x0c and
//     +0x10 (set to -1 by the array setters, and by an indexed setter that changes the value), block
//     header with count (uint16) at +0x0e and the 16-byte entries at +0x20, values stored inline at
//     +0x20 of the material. Indexed setters on CMaterial compare with the stored value first.
//   RendererParameterStorage (CMaterialRenderer): count (uint16) at +0x0e, entry table at +0x20,
//     value buffer pointer at +0x24.
//
// Stride rule of the array forms: a stride of 0 or the element size copies the elements contiguously
// (the original's memcpy path); any other stride steps the caller's side by that many bytes.
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
//   Not ledgered (blocked, see NOTES.md): setParameterElement (9), setParameterAt<SColorf> (1), and the
//   intrusive_ptr instances of CLight and ITexture (48).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

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
    static constexpr bool kSkipsUnchanged = false;  // indexed setters always write
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
    void noteWrite() {}

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
    static constexpr bool kSkipsUnchanged = true;  // indexed setters skip an equal value (return false)
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
    // Every setter resets the dirty-range markers (the ARM stores -1 to +0x0c and +0x10).
    void noteWrite() {
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
    static constexpr bool kSkipsUnchanged = false;  // indexed setters always write
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
    void noteWrite() {}

    std::uint16_t count = 0;                   // +0x0e
    std::vector<SShaderParameterDef> entries;  // +0x20: entry table
    std::vector<std::uint8_t> values;          // +0x24: value buffer
};

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
// Partial: SColorf::equals takes a tolerance in the original; the tolerance constant is not recovered,
// so the components are compared exactly.
template <>
struct ParamEquals<glitch::video::SColorf> {
    static bool equal(const glitch::video::SColorf& a, const glitch::video::SColorf& b) {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
};

// Copies count elements of bytes each. dstStep and srcStep are byte steps between elements, so a
// step equal to bytes is the contiguous copy of the original's memcpy path.
inline void copyElements(std::uint8_t* dst, std::ptrdiff_t dstStep, const std::uint8_t* src,
                         std::ptrdiff_t srcStep, std::uint32_t count, std::size_t bytes) {
    for (std::uint32_t i = 0; i < count; ++i) {
        std::memcpy(dst + static_cast<std::ptrdiff_t>(i) * dstStep,
                    src + static_cast<std::ptrdiff_t>(i) * srcStep, bytes);
    }
}

// The element step of an array argument: stride 0 means the elements are contiguous.
inline std::ptrdiff_t arrayStep(int stride, std::size_t bytes) {
    return stride == 0 ? static_cast<std::ptrdiff_t>(bytes) : static_cast<std::ptrdiff_t>(stride);
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

    // setParameter(id, index, T const&) : indexed write. On CMaterial an equal value is not written
    // and the call returns false (the original branches on the compare and returns 0 in that case).
    template <class T>
    bool setParameter(std::uint16_t id, std::uint32_t index, const T& value) {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || def->type != ParamType<T>::code || index >= def->count) return false;
        std::uint8_t* dst = storage.data(def->offset + index * static_cast<std::uint32_t>(ParamType<T>::bytes));
        if (Storage::kSkipsUnchanged) {
            T current{};
            std::memcpy(&current, dst, sizeof(T));
            if (ParamEquals<T>::equal(current, value)) return false;
        }
        storage.noteWrite();
        std::memcpy(dst, &value, sizeof(T));
        return true;
    }

    // setParameter(id, T const*, stride) : array write of all elements from the caller's array.
    template <class T>
    bool setParameter(std::uint16_t id, const T* src, int stride) {
        const SShaderParameterDef* def = storage.definition(id);
        if (!def || def->type != ParamType<T>::code) return false;
        storage.noteWrite();
        copyElements(storage.data(def->offset), static_cast<std::ptrdiff_t>(ParamType<T>::bytes),
                     reinterpret_cast<const std::uint8_t*>(src), arrayStep(stride, ParamType<T>::bytes),
                     def->count, ParamType<T>::bytes);
        return true;
    }

    // Partial: the original gates conversions on the SShaderParameterTypeInspection::Convertions table,
    // which is not in the container. Only reads and writes of the parameter's own type are implemented;
    // a value of another type is refused.
    template <class T>
    bool getParameterCvt(std::uint16_t id, std::uint32_t index, T& out) const {
        return getParameter(id, index, out);
    }
    template <class T>
    bool getParameterCvt(std::uint16_t id, T* out, int stride) const {
        return getParameter(id, out, stride);
    }
    template <class T>
    bool setParameterCvt(std::uint16_t id, std::uint32_t index, const T& value) {
        return setParameter(id, index, value);
    }
    template <class T>
    bool setParameterCvt(std::uint16_t id, const T* src, int stride) {
        return setParameter(id, src, stride);
    }
};

using GlobalMaterialParameters = IMaterialParameters<GlobalParameterStorage>;    // CGlobalMaterialParameterManager
using MaterialParameters = IMaterialParameters<MaterialParameterStorage>;       // CMaterial
using RendererParameters = IMaterialParameters<RendererParameterStorage>;       // CMaterialRenderer

}  // namespace glitch::video::detail
