// rnd::Direction's static tables. The class has been declared in dh2/rnd/Direction.hpp for a while but
// nothing defined its data, so every row that reads a direction's opposite was blocked on an undefined
// symbol rather than on missing information.
//
// Both tables are read straight out of libDungeonHunter2.so's data:
//
//   sDirections - 0x956AE4, four 16-byte records, stride 0x10:
//     { id 0, "north", x  0, y -1 }
//     { id 1, "east",  x  1, y  0 }
//     { id 2, "south", x  0, y  1 }
//     { id 3, "west",  x -1, y  0 }
//   The four name strings are at 0x8d4dc0, 0x8d4dc8, 0x8d4dd0 and 0x8d4dd8.
//
//   DIRECTION_OPPOSITES - 0x999880, four int32: 2, 3, 0, 1. The address was not found from a data xref:
//   the only xref to the name string `_ZN3rnd19DIRECTION_OPPOSITESE` (at 0x121282) is an ELF symbol-table
//   entry at 0x56d98, and that Elf32_Sym's st_value field, at entry offset +0x04, is 0x999880. So the
//   mapping is north(0) -> south(2), east(1) -> west(3), south(2) -> north(0), west(3) -> east(1).
#include "dh2/rnd/Direction.hpp"

namespace rnd {

const Direction Direction::sDirections[4] = {
    {0, "north", 0, -1},
    {1, "east", 1, 0},
    {2, "south", 0, 1},
    {3, "west", -1, 0},
};

const std::int32_t DIRECTION_OPPOSITES[4] = {2, 3, 0, 1};

}  // namespace rnd
