// Track B batch e2-pydata-accessors-Structs-66b9bc-1 (category pydata-accessors, owner Structs, part 2 of 2).
// The read(IStreamBase*) bodies of 12 Structs record classes. Each body follows the Hex-Rays order of
// reads, frees and allocations. The element arrays use the block of the Arrays readers
// (e2-pydata-accessors-Arrays-b90cc9-0.cpp): an 8-byte header (stride word, count word), then the elements,
// built by pydata::NewAndReadElementTable (element_table.hpp). The elements are host objects constructed in place
// (so the element's default member initialisers give the zeroed buffer pointers that the listing clears:
// v2QuestObjectiveStub +0x14 and +0x1c), reached through a typed pointer, and the stride word holds the original
// 32-bit stride (Structs::ListingStride, struct_strides.hpp: 36 for LootEntry, 16 for v2ConditionStub and
// v2QuestRewardStub, 44 for v2QuestObjectiveStub). A host table is never addressed with those strides.

#include <cstddef>
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"
#include "pydata-accessors/element_table.hpp"
#include "pydata-accessors/struct_strides.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_structs_1.hpp"
#include "pydata-accessors/w7_structs_2.hpp"

namespace Structs {

// 0x4fcff0 _ZN7Structs21FastTravelDestination4readEP11IStreamBase
void FastTravelDestination::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc, length of the string
    if (m_w4 != nullptr) CustomFree(m_w4);
    char* text = static_cast<char*>(::operator new[](static_cast<std::size_t>(m_w3) + 1, kPydataBufferHint));
    m_w4 = text;  // +0x10
    StreamReader::readStringEx(stream, text, m_w3);
    text[m_w3] = '\0';
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
}

// 0x4fe794 _ZN7Structs4Loot4readEP11IStreamBase
void Loot::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    // Table A: LootEntry elements (listing stride 36).
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc, count of table A
    pydata::DeleteElementTable<LootEntry>(m_w4);
    m_w4 = pydata::NewAndReadElementTable<LootEntry>(ListingStride::LootEntry, m_w3, stream);
    // Table B: LootEntry elements (listing stride 36).
    m_w5 = StreamReader::readAs<std::uint32_t>(stream);  // +0x14, count of table B
    pydata::DeleteElementTable<LootEntry>(m_w6);
    m_w6 = pydata::NewAndReadElementTable<LootEntry>(ListingStride::LootEntry, m_w5, stream);
    // Table C: plain int32 words (stride 4), no element header.
    m_w7 = StreamReader::readAs<std::uint32_t>(stream);  // +0x1c, count of table C
    if (m_w8 != nullptr) CustomFree(m_w8);
    m_w8 = ::operator new[](static_cast<std::size_t>(m_w7) * 4, kPydataBufferHint);
    auto* words = static_cast<std::int32_t*>(m_w8);
    for (std::uint32_t i = 0; i < m_w7; ++i) {
        words[i] = StreamReader::readAs<std::int32_t>(stream);
    }
}

// 0x4fec08 _ZN7Structs9LootEntry4readEP11IStreamBase
void LootEntry::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
}

// 0x50358c _ZN7Structs16v2QuestObjective4readEP11IStreamBase
void v2QuestObjective::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x504818 _ZN7Structs20v2QuestObjectiveStub4readEP11IStreamBase
void v2QuestObjectiveStub::read(IStreamBase* stream) {
    v2QuestObjective::read(stream);  // base read first (+0x4..+0xc)
    m_w4 = StreamReader::readAs<std::uint32_t>(stream);  // +0x10, length of m_w5
    if (m_w5 != nullptr) CustomFree(m_w5);
    char* first = static_cast<char*>(::operator new[](static_cast<std::size_t>(m_w4) + 1, kPydataBufferHint));
    m_w5 = first;  // +0x14
    StreamReader::readStringEx(stream, first, m_w4);
    first[m_w4] = '\0';
    m_w6 = StreamReader::readAs<std::uint32_t>(stream);  // +0x18, length of m_w7
    if (m_w7 != nullptr) CustomFree(m_w7);
    char* second = static_cast<char*>(::operator new[](static_cast<std::size_t>(m_w6) + 1, kPydataBufferHint));
    m_w7 = second;  // +0x1c
    StreamReader::readStringEx(stream, second, m_w6);
    second[m_w6] = '\0';
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
}

// 0x504a94 _ZN7Structs7v2Quest4readEP11IStreamBase
void v2Quest::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    // Table 1: v2ConditionStub elements (listing stride 16).
    m_w5 = StreamReader::readAs<std::uint32_t>(stream);  // +0x14
    pydata::DeleteElementTable<v2ConditionStub>(m_w6);
    m_w6 = pydata::NewAndReadElementTable<v2ConditionStub>(ListingStride::v2ConditionStub, m_w5, stream);
    // Table 2: v2QuestObjectiveStub elements (listing stride 44).
    m_w7 = StreamReader::readAs<std::uint32_t>(stream);  // +0x1c
    pydata::DeleteElementTable<v2QuestObjectiveStub>(m_w8);
    m_w8 = pydata::NewAndReadElementTable<v2QuestObjectiveStub>(ListingStride::v2QuestObjectiveStub, m_w7, stream);
    // Table 3: v2QuestRewardStub elements (listing stride 16).
    m_w9 = StreamReader::readAs<std::uint32_t>(stream);  // +0x24
    pydata::DeleteElementTable<v2QuestRewardStub>(m_w10);
    m_w10 = pydata::NewAndReadElementTable<v2QuestRewardStub>(ListingStride::v2QuestRewardStub, m_w9, stream);
    // Table 4: v2QuestRewardStub elements (listing stride 16).
    m_w11 = StreamReader::readAs<std::uint32_t>(stream);  // +0x2c
    pydata::DeleteElementTable<v2QuestRewardStub>(m_w12);
    m_w12 = pydata::NewAndReadElementTable<v2QuestRewardStub>(ListingStride::v2QuestRewardStub, m_w11, stream);
    // Table 5: v2QuestRewardStub elements (listing stride 16).
    m_w13 = StreamReader::readAs<std::uint32_t>(stream);  // +0x34
    pydata::DeleteElementTable<v2QuestRewardStub>(m_w14);
    m_w14 = pydata::NewAndReadElementTable<v2QuestRewardStub>(ListingStride::v2QuestRewardStub, m_w13, stream);
    // Two embedded objective stubs, then the scripts block.
    m_sub60.v2QuestObjectiveStub::read(stream);  // +0x3c
    m_sub104.v2QuestObjectiveStub::read(stream);  // +0x68
    m_w37 = StreamReader::readAs<std::int32_t>(stream);  // +0x94
    m_w38 = StreamReader::readAs<bool>(stream);  // +0x98, one byte
    m_w39 = StreamReader::readAs<std::int32_t>(stream);  // +0x9c
    m_sub160.v2QuestScripts::read(stream);  // +0xa0
    m_w69 = StreamReader::readAs<std::int32_t>(stream);  // +0x114
    m_w70 = StreamReader::readAs<std::int32_t>(stream);  // +0x118
}

// 0x5053fc _ZN7Structs13v2QuestReward4readEP11IStreamBase
void v2QuestReward::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
}

// 0x50589c _ZN7Structs17v2QuestRewardStub4readEP11IStreamBase
void v2QuestRewardStub::read(IStreamBase* stream) {
    v2QuestReward::read(stream);  // base read first (+0x4)
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x505b28 _ZN7Structs11v2Condition4readEP11IStreamBase
void v2Condition::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
}

// 0x50600c _ZN7Structs15v2ConditionStub4readEP11IStreamBase
void v2ConditionStub::read(IStreamBase* stream) {
    v2Condition::read(stream);  // base read first (+0x4)
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x50637c _ZN7Structs5Faery4readEP11IStreamBase
void Faery::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::uint32_t>(stream);  // +0x14, length of m_w6
    if (m_w6 != nullptr) CustomFree(m_w6);
    char* text = static_cast<char*>(::operator new[](static_cast<std::size_t>(m_w5) + 1, kPydataBufferHint));
    m_w6 = text;  // +0x18
    StreamReader::readStringEx(stream, text, m_w5);
    text[m_w5] = '\0';
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
}

// 0x506f3c _ZN7Structs7AIProps4readEP11IStreamBase
void AIProps::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<bool>(stream);  // +0x10, one byte
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    StreamReader::readAs<float>(stream, &m_w6);  // +0x18
    StreamReader::readAs<float>(stream, &m_w7);  // +0x1c
    StreamReader::readAs<float>(stream, &m_w8);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::uint32_t>(stream);  // +0x28, length of m_w11
    if (m_w11 != nullptr) CustomFree(m_w11);
    char* text = static_cast<char*>(::operator new[](static_cast<std::size_t>(m_w10) + 1, kPydataBufferHint));
    m_w11 = text;  // +0x2c
    StreamReader::readStringEx(stream, text, m_w10);
    text[m_w10] = '\0';
    m_w12 = StreamReader::readAs<std::int32_t>(stream);  // +0x30
    m_w13 = StreamReader::readAs<std::int32_t>(stream);  // +0x34
    m_w14 = StreamReader::readAs<std::int32_t>(stream);  // +0x38
    StreamReader::readAs<float>(stream, &m_w15);  // +0x3c
    StreamReader::readAs<float>(stream, &m_w16);  // +0x40
}

}  // namespace Structs
