#include "net-session-online/c_room_attributes.hpp"
#include "net-session-online/c_room_search_filter.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Default construction: no attribute is active, every value is zero.
    CRoomAttributes empty;
    CHECK(empty.IntActiveMask() == 0);
    CHECK(empty.BinActiveMask() == 0);
    for (int i = 0; i < CRoomAttributes::kIntAttributeCount; ++i) {
        CHECK(!empty.IsAttributeActiveInt(i));
        CHECK(empty.GetAttributeInt(i) == 0);
    }

    // Set and erase an int attribute: value and active bit move together.
    CRoomAttributes room;
    room.SetAttributeInt(3, 77);
    CHECK(room.IsAttributeActiveInt(3));
    CHECK(room.GetAttributeInt(3) == 77);
    CHECK(room.IntActiveMask() == (1u << 3));
    room.EraseAttributeInt(3);
    CHECK(!room.IsAttributeActiveInt(3));
    CHECK(room.GetAttributeInt(3) == 0);

    // Bin attribute round trip: size is reported, bytes come back.
    const std::uint8_t name[] = {'a', 'b', 'c'};
    room.SetAttributeBin(1, name, 3);
    CHECK(room.IsAttributeActiveBin(1));
    std::uint8_t out[8] = {};
    CHECK(room.GetAttributeBin(1, out, sizeof out) == 3);
    CHECK(out[0] == 'a' && out[1] == 'b' && out[2] == 'c');

    // Copy constructor and copy assignment keep every active attribute.
    CRoomAttributes copy(room);
    CHECK(copy.GetAttributeInt(3) == 0);
    CHECK(copy.IsAttributeActiveBin(1));
    room.SetAttributeInt(5, 9);
    CRoomAttributes assigned;
    assigned = room;
    CHECK(assigned.GetAttributeInt(5) == 9);
    CHECK(assigned.IntActiveMask() == room.IntActiveMask());

    // Copy with flag 0 copies no int attributes (group flag 0x1 gates them).
    CRoomAttributes target;
    target.CopyWithFlags(room, 0);
    CHECK(!target.IsAttributeActiveInt(5));
    CHECK(!target.IsAttributeActiveBin(1));  // bin 1 and 2 are gated by flag 0x8, which is absent

    // Clear empties everything.
    room.Clear();
    CHECK(room.IntActiveMask() == 0 && room.BinActiveMask() == 0);
    CHECK(room.GetAttributeInt(5) == 0);

    // Search filter: an empty filter matches every room.
    CRoomSearchFilter filter;
    CHECK(filter.TestRoomAttributes(empty));

    // Int filter needs an active attribute with a matching value.
    CRoomAttributes game;
    game.SetAttributeInt(2, 10);
    CRoomSearchFilter::tSearchFilterInt intFilter;
    intFilter.attribute = 2;
    intFilter.op = CRoomSearchFilter::kOpGreaterEqual;
    intFilter.value = 10;
    filter.IntFilters().push_back(intFilter);
    CHECK(filter.TestRoomAttributes(game));
    intFilter.value = 11;
    filter.IntFilters().back() = intFilter;
    CHECK(!filter.TestRoomAttributes(game));
    CHECK(!filter.TestRoomAttributes(empty));  // inactive attribute fails the filter

    // Operator table from TestIntValues.
    CHECK(CRoomSearchFilter::TestIntValues(4, 4, CRoomSearchFilter::kOpEqual));
    CHECK(!CRoomSearchFilter::TestIntValues(4, 5, CRoomSearchFilter::kOpEqual));
    CHECK(CRoomSearchFilter::TestIntValues(4, 5, CRoomSearchFilter::kOpLess));
    CHECK(!CRoomSearchFilter::TestIntValues(4, 5, 42));  // unknown operator never matches

    // Copy keeps the two vectors.
    CRoomSearchFilter copied(filter);
    CHECK(copied.IntFilters().size() == 1);
    CHECK(copied.IntFilters()[0].value == 11);

    std::printf("%s\n", failures ? "c_room_attributes_test: FAILED" : "c_room_attributes_test: ok");
    return failures != 0;
}
