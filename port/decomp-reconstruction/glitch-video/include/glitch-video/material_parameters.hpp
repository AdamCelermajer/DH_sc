// glitch::video::detail::IMaterialParameters<Owner, Header>: the typed setters
// and getters of a parameter block. The three original instances are
// IMaterialParameters<CGlobalMaterialParameterManager, SEmptyBase>,
// IMaterialParameters<CMaterial, ISharedMemoryBlockHeader<CMaterial>> and
// IMaterialParameters<CMaterialRenderer, ISharedMemoryBlockHeader<CMaterialRenderer>>.
// The value conversions are the same in all three. Only CMaterial tracks dirty
// words (+0x0c and +0x10); the other two owners store without marking.
//
// Matrix parameters (CMatrix4<float>) are held in a table of 4-byte slots in the
// original. Each slot holds a heap matrix, or null for the identity. The port keeps
// the slots in m_matrixSlots, keyed by the original byte offset of the slot. The
// accessors below are written for CMaterial and CMaterialRenderer only. The
// CGlobalMaterialParameterManager instance has no matrix accessors here.
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>

#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/material_parameters_typed.hpp"

namespace glitch::video {

class CGlobalMaterialParameterManager;
class CMaterial;
class CMaterialRenderer;

namespace detail {

// One entry of a parameter table, 16 bytes in the original. Only the fields the
// matrix accessors read are named. +0x00..+0x05 and +0x07 are not read here.
struct SParameterEntry {
    uint8_t unknown00[4] = {};  // +0x00
    uint16_t kind = 0;          // +0x04, 2: the hashed 4-byte words; 11 and 15 are not hashed (CMaterial::updateParametersHashCode)
    uint8_t type = 0;           // +0x06, 11 selects a matrix parameter
    uint8_t unknown07 = 0;      // +0x07
    uint32_t count = 0;         // +0x08, number of matrices in the entry
    uint32_t valueOffset = 0;   // +0x0c, byte offset of the first slot in the value area
};

// Header of a parameter table: count (uint16) at +0x0e and the entry array pointer at +0x20.
// CMaterial reaches this through its block pointer at +0x04. CMaterialRenderer holds it inline.
struct SParameterBlockHeader {
    uint16_t count = 0;                        // +0x0e
    const SParameterEntry* entries = nullptr;  // +0x20
};

// Matrix helpers of glitch::video::detail. getMatrixParameter copies the slot's matrix,
// or the identity when the slot is null. setMatrixParameter stores into the slot (see the
// .cpp for the identity and pool rules).
void getMatrixParameter(const core::CMatrix4<float>* slot, core::CMatrix4<float>& out);
void setMatrixParameter(core::CMatrix4<float>*& slot, const core::CMatrix4<float>& src);

// Defined in material_parameters_part_1c.hpp (which includes this header).
namespace part1c {
struct SParameterTable;
struct SPointerSlots;
}  // namespace part1c

template <typename Owner, bool kTrackDirty>
class IMaterialParameters : public IMaterialParametersTyped<IMaterialParameters<Owner, kTrackDirty>> {
public:
    using Base = IMaterialParametersTyped<IMaterialParameters<Owner, kTrackDirty>>;
    // The typed member templates and dispatchers come from the mixin (material_parameters_typed.hpp).
    using Base::getParameter;
    using Base::getParameterCvt;
    using Base::setParameter;
    using Base::setParameterCvt;

    // setDirty: CMaterial sets both dirty words to 0xffffffff; the others do nothing.
    void setDirty();

    // float -> int, and int -> float, with the dirty mark on change when tracked.
    void setParameterAt(int32_t* dst, float value);
    void setParameterAt(float* dst, int32_t value);
    // Colour and vector copies.
    void setParameterAt(core::vector4d<float>* dst, const SColorf& value);
    void setParameterAt(SColorf* dst, const core::vector4d<float>& value);

    void getParameterAt(const int32_t* src, float& value) const;
    void getParameterAt(const float* src, int32_t& value) const;
    void getParameterAt(const SColorf* src, core::vector4d<float>& value) const;
    void getParameterAt(const core::vector4d<float>* src, SColorf& value) const;

    // Matrix accessors (CMaterial and CMaterialRenderer; explicit specializations below).
    // Entry lookup: the entry for id, or null when id is out of range.
    const SParameterEntry* getParameterDef(uint16_t id) const;
    bool getParameter(uint16_t id, core::CMatrix4<float>* out, int stride) const;
    bool getParameter(uint16_t id, uint32_t index, core::CMatrix4<float>& out) const;
    bool setParameter(uint16_t id, const core::CMatrix4<float>* src, int stride);
    bool setParameter(uint16_t id, uint32_t index, const core::CMatrix4<float>& src);
    bool getParameter(uint16_t id, core::CMatrix4<float>& out) const;
    bool getParameterCvt(uint16_t id, uint32_t index, core::CMatrix4<float>& out) const;
    bool getParameterCvt(uint16_t id, core::CMatrix4<float>& out) const;

    // Test helpers, not in the original.
    uint32_t dirtyWord0() const { return m_dirty0; }
    uint32_t dirtyWord1() const { return m_dirty1; }

    // Test helper: the number of matrix slots that have been written.
    std::size_t matrixSlotCount() const { return m_matrixSlots.size(); }
    // Test helpers: the inline header (CMaterialRenderer) and the block pointer (CMaterial).
    SParameterBlockHeader& headerForTest() { return m_header; }
    void setBlockForTest(SParameterBlockHeader* block) { m_block = block; }

    // Views for the typed bindings of CMaterialRenderer (material_parameters_bind_r.cpp): the inline
    // header, the value buffer at +0x24 and the matrix slot map.
    const SParameterBlockHeader& parameterHeader() const { return m_header; }
    uint8_t* valueBuffer() const { return m_values; }
    void setValueBuffer(uint8_t* values) { m_values = values; }
    std::map<uint32_t, core::CMatrix4<float>*>& matrixSlots() { return m_matrixSlots; }

    // Views for the typed bindings of CMaterial (material_parameters_bind_m.cpp; explicit specializations
    // below). table() is the part 1c view: block header, value area, dirty words and pointer slots.
    part1c::SParameterTable table();
    part1c::SParameterTable table() const;
    // Part 1c rows that are not mixin members (defined in material_parameters_bind_m.cpp).
    template <typename T>
    int setParameterElement(uint16_t id, uint32_t index, uint8_t component, T value);
    void setParameterAt(SColorf* dst, const SColor& src);

private:
    uint32_t m_dirty0 = 0;  // +0x0c, set to 0xffffffff by setDirty (tracking owners)
    uint32_t m_dirty1 = 0;  // +0x10

    // CMaterial: block pointer at +0x04. Its count and entries are read through it.
    SParameterBlockHeader* m_block = nullptr;
    // CMaterialRenderer: count at +0x0e and entries at +0x20 of the object itself.
    SParameterBlockHeader m_header;
    // CMaterialRenderer: value buffer at +0x24. The owner sets it with setValueBuffer.
    uint8_t* m_values = nullptr;

    // Matrix slots by original byte offset (CMaterial: inline values at +0x20,
    // CMaterialRenderer: value buffer at +0x24). Null means identity.
    std::map<uint32_t, core::CMatrix4<float>*> m_matrixSlots;

    // CMaterial: pointer slots of the part 1c rows, created by table(). A shared pointer keeps the
    // destructor independent of the complete type of SPointerSlots.
    mutable std::shared_ptr<part1c::SPointerSlots> m_pointers;
};

// CMaterial typed views and the two non-mixin rows (defined in material_parameters_bind_m.cpp).
template <>
part1c::SParameterTable IMaterialParameters<CMaterial, true>::table();
template <>
part1c::SParameterTable IMaterialParameters<CMaterial, true>::table() const;
template <>
void IMaterialParameters<CMaterial, true>::setParameterAt(SColorf* dst, const SColor& src);

// Explicit specializations of the matrix accessors (defined in src/material_parameters.cpp).
template <>
const SParameterEntry* IMaterialParameters<CMaterial, true>::getParameterDef(uint16_t id) const;
template <>
bool IMaterialParameters<CMaterial, true>::getParameter(uint16_t id, core::CMatrix4<float>* out, int stride) const;
template <>
bool IMaterialParameters<CMaterial, true>::getParameter(uint16_t id, uint32_t index, core::CMatrix4<float>& out) const;
template <>
bool IMaterialParameters<CMaterial, true>::setParameter(uint16_t id, const core::CMatrix4<float>* src, int stride);
template <>
bool IMaterialParameters<CMaterial, true>::setParameter(uint16_t id, uint32_t index, const core::CMatrix4<float>& src);
template <>
bool IMaterialParameters<CMaterial, true>::getParameter(uint16_t id, core::CMatrix4<float>& out) const;
template <>
bool IMaterialParameters<CMaterial, true>::getParameterCvt(uint16_t id, uint32_t index, core::CMatrix4<float>& out) const;
template <>
bool IMaterialParameters<CMaterial, true>::getParameterCvt(uint16_t id, core::CMatrix4<float>& out) const;

template <>
const SParameterEntry* IMaterialParameters<CMaterialRenderer, false>::getParameterDef(uint16_t id) const;
template <>
bool IMaterialParameters<CMaterialRenderer, false>::getParameter(uint16_t id, core::CMatrix4<float>* out, int stride) const;
template <>
bool IMaterialParameters<CMaterialRenderer, false>::getParameter(uint16_t id, uint32_t index, core::CMatrix4<float>& out) const;
template <>
bool IMaterialParameters<CMaterialRenderer, false>::setParameter(uint16_t id, const core::CMatrix4<float>* src, int stride);
template <>
bool IMaterialParameters<CMaterialRenderer, false>::setParameter(uint16_t id, uint32_t index, const core::CMatrix4<float>& src);
template <>
bool IMaterialParameters<CMaterialRenderer, false>::getParameter(uint16_t id, core::CMatrix4<float>& out) const;
template <>
bool IMaterialParameters<CMaterialRenderer, false>::getParameterCvt(uint16_t id, uint32_t index, core::CMatrix4<float>& out) const;
template <>
bool IMaterialParameters<CMaterialRenderer, false>::getParameterCvt(uint16_t id, core::CMatrix4<float>& out) const;

}  // namespace detail

}  // namespace glitch::video
