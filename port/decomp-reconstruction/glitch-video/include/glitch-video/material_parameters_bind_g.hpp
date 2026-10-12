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

#include "glitch-video/material_parameters_part_1a.hpp"
#include "glitch-video/material_parameters_part_1b.hpp"
#include "glitch-video/material_parameters_typed.hpp"

namespace glitch::video::detail {

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

}  // namespace glitch::video::detail
