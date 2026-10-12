// Owner G binding: IMaterialParameters<CGlobalMaterialParameterManager, SEmptyBase>.
//
// The port has no class of that name. material_parameters.hpp declares a different template
// (IMaterialParameters<Owner, bool>) for owners M and R. SGlobalParameterOwner is the Derived type of the
// typed mixin for G. It owns the global manager's table, which the two part files read in two forms:
//   - part 1a reads the 20-byte entry records (SParamEntry) through the GlobalParamTable view;
//   - part 1b reads the same entries as ParamEntry (set = ref != 0) inside GlobalParameterStore.
// Both copies of the entry table are kept in step by defineEntry, the only way to change an entry.
// The value buffer is shared: the 1a view points at store().values.
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

    // Store for the part 1b rows. The value buffer is store().values. Do not change store().entries
    // directly: it must match the table view, so use defineEntry.
    part1b::GlobalParameterStore& store() { return m_store; }
    const part1b::GlobalParameterStore& store() const { return m_store; }

private:
    part1b::GlobalParameterStore m_store;
    std::vector<part1a::SParamEntry> m_entries;  // same entries as m_store.entries, 20-byte records
};

}  // namespace glitch::video::detail
