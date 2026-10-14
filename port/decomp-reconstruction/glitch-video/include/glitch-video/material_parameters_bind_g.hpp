// Owner G binding: IMaterialParameters<CGlobalMaterialParameterManager, SEmptyBase>.
//
// The port has no class of that name. material_parameters.hpp declares a different template
// (IMaterialParameters<Owner, bool>) for owners M and R. SGlobalParameterOwner is the Derived type of the
// typed mixin for G. It owns the global manager's table: the entry list of part1b::GlobalParameterStore, whose
// records are part1a::SParamEntry (20 bytes). Part 1a reads that list through the GlobalParamTable view, part 1b
// through the store. The value buffer is shared: the view points at store().values. defineEntry is the only way
// to change an entry.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-core-io/core/sided_collection.hpp"
#include "glitch-video/material_parameters_part_1a.hpp"
#include "glitch-video/material_parameters_part_1b.hpp"
#include "glitch-video/material_parameters_typed.hpp"

namespace glitch::video {
namespace detail {

// Property record and value traits of the collection instance that stores the global manager's parameter
// definitions: glitch::core::detail::SIDedCollection<glitch::video::SShaderParameterDef, unsigned short, false,
// globalmaterialparametermanager::SPropeties, globalmaterialparametermanager::SValueTraits> (the object at the
// global manager's +0x00..+0x27, see global_parameter_manager.hpp). Namespace and spelling are the binary's
// (one 'r' in SPropeties).
namespace globalmaterialparametermanager {

// Props record of that instance; SIdValue derives from it and puts the uint16 id after it. The SIdValue
// constructors 0x5b9f64 (C2) and 0x5b9f74 (C1) are three instructions and share their body:
// STRH R2,[R0,#4] (the id) and STR R0,#1,[R0] (the literal 1), an 8-byte SIdValue record whose single Props word
// is the 1. Verified in both instantiations and against the getProperties 0x5b9f04/0x5b9f1c offsets.
struct SPropeties {
    int32_t unk00 = 1;  // +0x00: 1 after construction (the only Props write of the SIdValue constructor); meaning not recovered
};

// Value traits of the same instance. The shared template calls Traits::isSet and Traits::useCount on the stored
// value; for this instance they read the reference count of the 20-byte SShaderParameterDef's shared-string data
// pointer (glitch-shared/sid_collection.hpp). The port's SShaderParameterDef (shader_attributes.hpp) has no
// shared-string member yet, so the two members are NOT declared here: the collection functions that need them
// (insert 0x5bc16c, remove 0x5c0f70, removeAll(bool) 0x5c1104, SEntry::reset 0x5bc5c4, SEntry::set 0x5bc620)
// keep their ledger status and stay unwritten rather than silently mis-counting. The SIdValue constructor this
// declaration unblocks does not use the traits.
struct SValueTraits;

}  // namespace globalmaterialparametermanager

struct SGlobalParameterOwner : IMaterialParametersTyped<SGlobalParameterOwner> {
    // Defines entry id: value type code, element count and byte offset of element 0 in the value buffer.
    // Entries are never removed. Ids below id are created unset.
    void defineEntry(std::uint16_t id, std::uint8_t type, std::uint32_t count, std::uint32_t offset);

    // Non-owning view for the part 1a rows. The const getters never write through the value pointer.
    part1a::GlobalParamTable table() const;

    // Store for the part 1b rows. The value buffer is store().values. Change the entries only through defineEntry.
    part1b::GlobalParameterStore& store() { return m_store; }
    const part1b::GlobalParameterStore& store() const { return m_store; }

private:
    part1b::GlobalParameterStore m_store;
};

}  // namespace detail
}  // namespace glitch::video
