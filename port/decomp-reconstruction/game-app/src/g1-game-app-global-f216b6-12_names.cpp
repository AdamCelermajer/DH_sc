// Helper unit of the batch r3b-game-app-global-f216b6-7 (NativeGoToZone, ELF 0x4422b8): the Arrays::LevelList name lookup.
//
// The name table Arrays::LevelList::m_memberNames is declared in pydata-accessors/member_names.hpp, which cannot share a
// unit with pydata-accessors/w7_arrays_00.hpp (Arrays::LevelList::size and ::members). The caller passes the row count
// from that header. The lookup is the original's loop: the first of the first count names equal to the name, or -1.

#include <cstring>

#include "pydata-accessors/member_names.hpp"

int FindLevelListRow(const char* name, int count) {
    const char* const* names = Arrays::LevelList::m_memberNames.names;
    for (int i = 0; i < count; ++i) {
        if (std::strcmp(name, names[i]) == 0) {
            return i;
        }
    }
    return -1;
}
