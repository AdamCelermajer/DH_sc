// Helper unit of the batch r3b-game-character-Character-ee9946-0 (Character::GetCharModelId, ELF 0x3a31e8): the row count of
// Arrays::ModelDict. pydata-accessors/w7_arrays_00.hpp declares ModelDict but clashes with w7_arrays_02.hpp, which the
// Character unit includes, so the count is read in this unit and passed on.
#include <cstdint>

#include "pydata-accessors/w7_arrays_00.hpp"

std::uint32_t ModelDictRowCount() {
    return Arrays::ModelDict::size;
}
