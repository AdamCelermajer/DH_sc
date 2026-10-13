// Structs::X::read(IStreamBase*) for batch e1-pydata-accessors-Structs-0 (20 functions, class Structs).
// Declarations are in include/pydata-accessors/structs_records.hpp, w7_structs_2.hpp and w7_structs_5.hpp.
// Layout comments give the original 32-bit offsets. Each read follows the Hex-Rays body in order: the
// stream reads, the frees before each reallocation, and the allocation and its element loop.
//
// Element tables (ItemListEntryList, CharTemplate, ClassFuncList, AnimTpl) hold the element class found from
// the vtable of the table's element pointer: ItemListEntry (listing stride 12), CharInfoName (8), ClassFunc (24)
// and AnimStep (56). They are built and destroyed through pydata::NewAndReadElementTable / DeleteElementTable
// (element_table.hpp): host objects constructed in place at sizeof(Elem), reached through a typed pointer, with the
// original 32-bit stride (Structs::ListingStride, struct_strides.hpp) written to the stride word. The table header
// (stride, count) sits before the members, as in the original, so the destructor loop reads the count from the block.
#include <cstddef>
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"
#include "pydata-accessors/element_table.hpp"
#include "pydata-accessors/struct_strides.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

namespace {

// Original int32 word table: free the old block if set, allocate count * 4 bytes (hint 1), read each word.
// The count word has already been stored by the caller.
void ReadInt32Words(IStreamBase* stream, void*& words, std::uint32_t count) {
    if (words != nullptr) CustomFree(words);
    words = MemoryHintState(sizeof(std::int32_t) * static_cast<std::size_t>(count), 1);
    std::int32_t* const values = static_cast<std::int32_t*>(words);
    for (std::uint32_t i = 0; i < count; ++i) {
        values[i] = StreamReader::readAs<std::int32_t>(stream);
    }
}

// Original length-prefixed string field: the length word is already stored. The old buffer is freed, a buffer
// of len + 1 bytes is allocated (hint 1) and stored, len bytes are read (hint 0), and buf[len] is set to NUL.
void ReadStringField(IStreamBase* stream, std::uint32_t len, void*& buf) {
    if (buf != nullptr) CustomFree(buf);
    buf = MemoryHintState(static_cast<std::size_t>(len) + 1, 1);
    char* const chars = static_cast<char*>(buf);
    StreamReader::readStringEx(stream, chars, len);
    chars[len] = '\0';
}

}  // namespace

namespace Structs {

// 0x4dbb5c Structs::Inventory::read(IStreamBase*)
void Inventory::read(IStreamBase* stream) {
    StreamReader::readAs<std::int16_t>(stream, &m_w1);  // +0x4
}

// 0x4dc790 Structs::ItemListEntryList::read(IStreamBase*)
void ItemListEntryList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, element count
    pydata::DeleteElementTable<ItemListEntry>(m_w2);  // +0x8
    m_w2 = pydata::NewAndReadElementTable<ItemListEntry>(ListingStride::ItemListEntry, m_w1, stream);  // +0x8
}

// 0x4dcd38 Structs::CharTemplate::read(IStreamBase*)
void CharTemplate::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, element count
    pydata::DeleteElementTable<CharInfoName>(m_w2);  // +0x8
    m_w2 = pydata::NewAndReadElementTable<CharInfoName>(ListingStride::CharInfoName, m_w1, stream);  // +0x8
}

// 0x4dcfec Structs::ClassFuncList::read(IStreamBase*)
void ClassFuncList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, element count
    pydata::DeleteElementTable<ClassFunc>(m_w2);  // +0x8
    m_w2 = pydata::NewAndReadElementTable<ClassFunc>(ListingStride::ClassFunc, m_w1, stream);  // +0x8
}

// 0x4dd5a8 Structs::v2QuestScripts::read(IStreamBase*)
void v2QuestScripts::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadStringField(stream, m_w1, m_w2);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    ReadStringField(stream, m_w3, m_w4);  // +0x10
    m_w5 = StreamReader::readAs<std::uint32_t>(stream);  // +0x14
    ReadStringField(stream, m_w5, m_w6);  // +0x18
    m_w7 = StreamReader::readAs<std::uint32_t>(stream);  // +0x1c
    ReadStringField(stream, m_w7, m_w8);  // +0x20
    m_w9 = StreamReader::readAs<std::uint32_t>(stream);  // +0x24
    ReadStringField(stream, m_w9, m_w10);  // +0x28
    m_w11 = StreamReader::readAs<std::uint32_t>(stream);  // +0x2c
    ReadStringField(stream, m_w11, m_w12);  // +0x30
    m_w13 = StreamReader::readAs<std::uint32_t>(stream);  // +0x34
    ReadStringField(stream, m_w13, m_w14);  // +0x38
    m_w15 = StreamReader::readAs<std::uint32_t>(stream);  // +0x3c
    ReadStringField(stream, m_w15, m_w16);  // +0x40
    m_w17 = StreamReader::readAs<std::uint32_t>(stream);  // +0x44
    ReadStringField(stream, m_w17, m_w18);  // +0x48
    m_w19 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4c
    ReadStringField(stream, m_w19, m_w20);  // +0x50
    m_w21 = StreamReader::readAs<std::uint32_t>(stream);  // +0x54
    ReadStringField(stream, m_w21, m_w22);  // +0x58
    m_w23 = StreamReader::readAs<std::uint32_t>(stream);  // +0x5c
    ReadStringField(stream, m_w23, m_w24);  // +0x60
    m_w25 = StreamReader::readAs<std::uint32_t>(stream);  // +0x64
    ReadStringField(stream, m_w25, m_w26);  // +0x68
    m_w27 = StreamReader::readAs<std::uint32_t>(stream);  // +0x6c
    ReadStringField(stream, m_w27, m_w28);  // +0x70
}

// 0x4ea830 Structs::SkillList::read(IStreamBase*)
void SkillList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, count
    ReadInt32Words(stream, m_w2, m_w1);  // +0x8
}

// 0x4eab9c Structs::FaeryList::read(IStreamBase*)
void FaeryList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, count
    ReadInt32Words(stream, m_w2, m_w1);  // +0x8
}

// 0x4eb4d8 Structs::WorldMapLocker::read(IStreamBase*)
void WorldMapLocker::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
}

// 0x4eb5a8 Structs::WldMapLocation::read(IStreamBase*)
void WldMapLocation::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc, count
    ReadInt32Words(stream, m_w4, m_w3);  // +0x10
}

// 0x4ebeb0 Structs::Skill::read(IStreamBase*)
void Skill::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<bool>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc, count
    ReadInt32Words(stream, m_w4, m_w3);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<bool>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::uint32_t>(stream);  // +0x24, length
    ReadStringField(stream, m_w9, m_w10);  // +0x28
    m_w11 = StreamReader::readAs<bool>(stream);  // +0x2c
    m_w12 = StreamReader::readAs<std::int32_t>(stream);  // +0x30
    m_w13 = StreamReader::readAs<std::int32_t>(stream);  // +0x34
    m_w14 = StreamReader::readAs<std::uint32_t>(stream);  // +0x38, length
    ReadStringField(stream, m_w14, m_w15);  // +0x3c
    m_w16 = StreamReader::readAs<std::int32_t>(stream);  // +0x40
    m_w17 = StreamReader::readAs<std::int32_t>(stream);  // +0x44
    m_w18 = StreamReader::readAs<std::int32_t>(stream);  // +0x48
}

// 0x4ec47c Structs::ItemBase::read(IStreamBase*)
void ItemBase::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, length
    ReadStringField(stream, m_w1, m_w2);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<bool>(stream);  // +0x1c
    StreamReader::readAs<float>(stream, &m_w8);  // +0x20
    StreamReader::readAs<float>(stream, &m_w9);  // +0x24
    StreamReader::readAs<float>(stream, &m_w10);  // +0x28
    StreamReader::readAs<float>(stream, &m_w11);  // +0x2c
    StreamReader::readAs<float>(stream, &m_w12);  // +0x30
    StreamReader::readAs<float>(stream, &m_w13);  // +0x34
    StreamReader::readAs<float>(stream, &m_w14);  // +0x38
    StreamReader::readAs<float>(stream, &m_w15);  // +0x3c
    StreamReader::readAs<float>(stream, &m_w16);  // +0x40
}

// 0x4ec9f0 Structs::AnimStep::read(IStreamBase*)
void AnimStep::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<bool>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<bool>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<bool>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::uint32_t>(stream);  // +0x20, count
    ReadInt32Words(stream, m_w9, m_w8);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    m_w11 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c
    StreamReader::readAs<float>(stream, &m_w12);  // +0x30
    m_w13 = StreamReader::readAs<bool>(stream);  // +0x34
}

// 0x4ed280 Structs::ItemListEntry::read(IStreamBase*)
void ItemListEntry::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    StreamReader::readAs<std::int16_t>(stream, &m_w2);  // +0x8
    StreamReader::readAs<std::int8_t>(stream, &m_off10);  // +0xa
}

// 0x4ef05c Structs::CamAnimSet::read(IStreamBase*)
void CamAnimSet::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4, count
    ReadInt32Words(stream, m_w2, m_w1);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
}

// 0x4ef2f0 Structs::AnimTpl::read(IStreamBase*)
void AnimTpl::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::uint32_t>(stream);  // +0x8, element count
    pydata::DeleteElementTable<AnimStep>(m_w3);  // +0xc
    // The original's vptr loop also zeroes element +0x24 (AnimStep::m_w9, the int32 array pointer); construction gives
    // it its default nullptr.
    m_w3 = pydata::NewAndReadElementTable<AnimStep>(ListingStride::AnimStep, m_w2, stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
}

// 0x4ef64c Structs::CharAnim::read(IStreamBase*)
void CharAnim::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    m_w11 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c
    m_w12 = StreamReader::readAs<std::int32_t>(stream);  // +0x30
    m_w13 = StreamReader::readAs<std::int32_t>(stream);  // +0x34
    m_w14 = StreamReader::readAs<std::int32_t>(stream);  // +0x38
    m_w15 = StreamReader::readAs<std::int32_t>(stream);  // +0x3c
    m_w16 = StreamReader::readAs<std::uint32_t>(stream);  // +0x40, count
    ReadInt32Words(stream, m_w17, m_w16);  // +0x44
    m_w18 = StreamReader::readAs<std::int32_t>(stream);  // +0x48
    m_w19 = StreamReader::readAs<std::int32_t>(stream);  // +0x4c
    m_w20 = StreamReader::readAs<std::int32_t>(stream);  // +0x50
    m_w21 = StreamReader::readAs<std::int32_t>(stream);  // +0x54
    m_w22 = StreamReader::readAs<std::int32_t>(stream);  // +0x58
    m_w23 = StreamReader::readAs<std::int32_t>(stream);  // +0x5c
    m_w24 = StreamReader::readAs<std::int32_t>(stream);  // +0x60
    m_w25 = StreamReader::readAs<std::int32_t>(stream);  // +0x64
    m_w26 = StreamReader::readAs<std::int32_t>(stream);  // +0x68
    m_w27 = StreamReader::readAs<std::int32_t>(stream);  // +0x6c
    m_w28 = StreamReader::readAs<std::int32_t>(stream);  // +0x70
    m_w29 = StreamReader::readAs<std::int32_t>(stream);  // +0x74
    m_w30 = StreamReader::readAs<std::int32_t>(stream);  // +0x78
    m_w31 = StreamReader::readAs<std::int32_t>(stream);  // +0x7c
    m_w32 = StreamReader::readAs<std::int32_t>(stream);  // +0x80
    m_w33 = StreamReader::readAs<std::uint32_t>(stream);  // +0x84, count
    ReadInt32Words(stream, m_w34, m_w33);  // +0x88
    m_w35 = StreamReader::readAs<std::int32_t>(stream);  // +0x8c
    m_w36 = StreamReader::readAs<std::int32_t>(stream);  // +0x90
    m_w37 = StreamReader::readAs<std::int32_t>(stream);  // +0x94
    m_w38 = StreamReader::readAs<std::int32_t>(stream);  // +0x98
    m_w39 = StreamReader::readAs<std::int32_t>(stream);  // +0x9c
}

// 0x4f19dc Structs::ClassFunc::read(IStreamBase*)
void ClassFunc::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4f1c90 Structs::CharInfoName::read(IStreamBase*)
void CharInfoName::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
}

// 0x4fc068 Structs::Item::read(IStreamBase*)
void Item::read(IStreamBase* stream) {
    ItemBase::read(stream);  // base read first
    m_w17 = StreamReader::readAs<std::int32_t>(stream);  // +0x44
    m_w18 = StreamReader::readAs<std::int32_t>(stream);  // +0x48
    m_w19 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4c, length
    ReadStringField(stream, m_w19, m_w20);  // +0x50
    m_w21 = StreamReader::readAs<std::int32_t>(stream);  // +0x54
    m_w22 = StreamReader::readAs<std::int32_t>(stream);  // +0x58
    m_w23 = StreamReader::readAs<std::int32_t>(stream);  // +0x5c
    m_w24 = StreamReader::readAs<std::int32_t>(stream);  // +0x60
    m_w25 = StreamReader::readAs<std::int32_t>(stream);  // +0x64
    m_w26 = StreamReader::readAs<std::int32_t>(stream);  // +0x68
    m_w27 = StreamReader::readAs<std::int32_t>(stream);  // +0x6c
    m_w28 = StreamReader::readAs<std::int32_t>(stream);  // +0x70
    m_w29 = StreamReader::readAs<std::int32_t>(stream);  // +0x74
    m_w30 = StreamReader::readAs<std::int32_t>(stream);  // +0x78
    m_w31 = StreamReader::readAs<std::int32_t>(stream);  // +0x7c
    m_w32 = StreamReader::readAs<std::int32_t>(stream);  // +0x80
    m_w33 = StreamReader::readAs<std::int32_t>(stream);  // +0x84
    m_w34 = StreamReader::readAs<std::int32_t>(stream);  // +0x88
    m_w35 = StreamReader::readAs<std::int32_t>(stream);  // +0x8c
    m_w36 = StreamReader::readAs<std::int32_t>(stream);  // +0x90
    m_w37 = StreamReader::readAs<std::int32_t>(stream);  // +0x94
    m_w38 = StreamReader::readAs<std::int32_t>(stream);  // +0x98
    m_w39 = StreamReader::readAs<std::int32_t>(stream);  // +0x9c
    m_w40 = StreamReader::readAs<std::int32_t>(stream);  // +0xa0
}

// 0x4fca84 Structs::LevelDeclaration::read(IStreamBase*)
void LevelDeclaration::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<bool>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::uint32_t>(stream);  // +0x8, length
    ReadStringField(stream, m_w2, m_w3);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<bool>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::uint32_t>(stream);  // +0x1c, length
    ReadStringField(stream, m_w7, m_w8);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    m_w11 = StreamReader::readAs<std::int32_t>(stream);  // +0x2c
    m_w12 = StreamReader::readAs<std::int32_t>(stream);  // +0x30
    m_w13 = StreamReader::readAs<std::int32_t>(stream);  // +0x34
    m_w14 = StreamReader::readAs<std::int32_t>(stream);  // +0x38
    m_w15 = StreamReader::readAs<std::int32_t>(stream);  // +0x3c
    m_w16 = StreamReader::readAs<std::int32_t>(stream);  // +0x40
    m_w17 = StreamReader::readAs<std::int32_t>(stream);  // +0x44
}

}  // namespace Structs
