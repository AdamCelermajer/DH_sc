// Track B batch e2-pydata-accessors-Arrays-b90cc9-0 (category pydata-accessors, class Arrays).
// Element-table read bodies (Arrays::X::read(IStreamBase*)) for SkillTable, SkillListTable, NumProbArray,
// InventoryTable and ItemBonusAttrMonopoly. The element classes and their finalize bodies are in the w7
// slices (w7_arrays_00.cpp). The other seven rows of the batch are partial and have no body here; the
// comment block at the end says why.

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <new>

#include "pydata-accessors/deps/stream_reader.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_structs_1.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

namespace {

// The shared body of the Arrays::X::read family (Hex-Rays shape, identical in all five):
//   1. count = readAs<uint32>(stream);
//   2. Table::finalize() (frees the old elements), then size = count;
//   3. block = new[](stride * count + 8): header word stride, header word count, members = block + 8;
//   4. each element gets its vtable (here: placement-new of Elem), then the listed words are zeroed;
//   5. each element's read slot (+0xc) runs in order, with the stream.
// The in-memory stride is sizeof(Elem) (wave 7d decision). The listing offsets in `zeroed` are kept from the
// ARM listing; a listed offset is zeroed only when it lies inside the host element (offset + 4 <= sizeof).
template <class Table, class Elem>
void ReadElementTable(IStreamBase* stream, std::initializer_list<std::size_t> zeroed) {
    const std::uint32_t count = StreamReader::readAs<std::uint32_t>(stream);
    Table::finalize();
    Table::size = count;

    constexpr std::size_t kStride = sizeof(Elem);
    auto* block = static_cast<std::uint8_t*>(MemoryHintState(kStride * count + 8, 1));
    const std::uint32_t header_stride = static_cast<std::uint32_t>(kStride);
    std::memcpy(block, &header_stride, sizeof header_stride);  // +0x0 of the block
    std::memcpy(block + 4, &count, sizeof count);              // +0x4 of the block
    Table::members = block + 8;

    for (std::uint32_t i = 0; i < count; ++i) {
        std::uint8_t* element = Table::members + kStride * i;
        new (element) Elem();  // vptr of Elem, the other words default-initialised
        for (std::size_t offset : zeroed) {
            if (offset + sizeof(std::uint32_t) > sizeof(Elem)) continue;  // listing offset past the host element
            const std::uint32_t zero = 0;
            std::memcpy(element + offset, &zero, sizeof zero);
        }
    }

    for (std::uint32_t i = 0; i < Table::size; ++i) {
        reinterpret_cast<Elem*>(Table::members + kStride * i)->read(stream);
    }
}

}  // namespace

namespace Arrays {

// 0x4b9810 _ZN6Arrays10SkillTable4readEP11IStreamBase
// Element Structs::Skill (vtable "vtable for Structs::Skill", listing stride 76). Zeroed words: +0x10, +0x28, +0x3c.
void SkillTable::read(IStreamBase* stream) {
    ReadElementTable<SkillTable, Structs::Skill>(stream, {0x10, 0x28, 0x3c});
}

// 0x4b9964 _ZN6Arrays14SkillListTable4readEP11IStreamBase
// Element Structs::SkillList (listing stride 12). Zeroed word: +0x8.
void SkillListTable::read(IStreamBase* stream) {
    ReadElementTable<SkillListTable, Structs::SkillList>(stream, {0x8});
}

// 0x4b9bf4 _ZN6Arrays12NumProbArray4readEP11IStreamBase
// Element Structs::NumProbList (listing stride 12). Zeroed word: +0x8.
void NumProbArray::read(IStreamBase* stream) {
    ReadElementTable<NumProbArray, Structs::NumProbList>(stream, {0x8});
}

// 0x4ba3c8 _ZN6Arrays14InventoryTable4readEP11IStreamBase
// Element Structs::Inventory (listing stride 8; only the vptr is written per element, no zeroed word).
void InventoryTable::read(IStreamBase* stream) {
    ReadElementTable<InventoryTable, Structs::Inventory>(stream, {});
}

// 0x4baa30 _ZN6Arrays21ItemBonusAttrMonopoly4readEP11IStreamBase
// Element Structs::ItemBonusAttrList (listing stride 12, confirmed by its vtable). Zeroed word: +0x8.
void ItemBonusAttrMonopoly::read(IStreamBase* stream) {
    ReadElementTable<ItemBonusAttrMonopoly, Structs::ItemBonusAttrList>(stream, {0x8});
}

// ---------------------------------------------------------------------------------------------
// Not defined in this file. Each row is partial in the ledger; the notes there say what is missing.
//
// 0x4b9e8c Arrays::LootTable::read(IStreamBase*): element Structs::Loot (vtable confirmed, listing stride 36,
//   finalize at sizeof(Structs::Loot)). Structs::Loot declares no read, so the element read slot (+0xc) has no
//   port body. Zeroed words +0x10, +0x18, +0x20 are known.
// 0x4ba12c Arrays::ItemTable::read(IStreamBase*): element Structs::Item (listing stride 164). ItemTable has no
//   members/size/finalize declared in the w7 layout (member_names.hpp declares only m_memberNames), and
//   ItemTable::finalize is not defined (see tb_accessors-Arrays-0.cpp).
// 0x4ba27c Arrays::ItemList::read(IStreamBase*): element is Structs::ItemListEntryList by its vtable, not
//   Structs::ItemListEntry as the name-match assumed. ItemList::finalize (w7_arrays_00.cpp) steps by
//   sizeof(Structs::ItemListEntry), so the two disagree and no read body is written until that is settled.
// 0x4ba4fc Arrays::DropTilePriorityTable::read(IStreamBase*): element Structs::TileOffsetList (vtable confirmed).
//   DropTilePriorityTable::finalize is still on the listing stride 12, which is smaller than the host element.
// 0x4bacc8 Arrays::ItemPowerList::read(IStreamBase*): element Structs::ItemPowerEntryList (vtable confirmed).
//   No Arrays::ItemPowerList members/size statics are declared (w7_arrays_01 finalize takes them by reference
//   and uses the listing stride 12).
// 0x4bbfe0 Arrays::FaeryTable::read(IStreamBase*): element Structs::Faery (vtable confirmed). No members/size
//   statics are declared, and w7_arrays_01 finalize still uses the listing stride 36.
// 0x4bc12c Arrays::FaeryListTable::read(IStreamBase*): element Structs::FaeryList (vtable confirmed). No
//   members/size statics are declared, and w7_arrays_01 finalize still uses the listing stride 12.
// ---------------------------------------------------------------------------------------------

}  // namespace Arrays
