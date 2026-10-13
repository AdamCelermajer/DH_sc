// Element tables owned by Structs records (Loot, v2Quest, ItemPowerRef, AnimTpl, ClassFuncList, CharTemplate, ...).
// Same rules as array_strides_test.cpp: a table is read from packed fields, kept as host objects constructed in
// place at sizeof(Elem), reached through typed pointers, and its stride word holds the 32-bit stride of the Hex-Rays
// body (Structs::ListingStride). The destroy loops of the owners used to step by that 32-bit stride (or, in
// w7_structs_4, by 16) over tables that the reads had built at the host stride; every row below reads a table and
// destroys it two ways (finalize, then the destructor of the next read), which corrupted the heap before.
#include "table_test_support.hpp"

#include "pydata-accessors/struct_strides.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_structs_1.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_4.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

// Reads an OWNER from a stream that invents every value, checks the table behind OWNER::PTR (count word CNT) and
// its stride word, then finalizes (the pointer and count are cleared, as the listing does), reads again into the
// same object (the read frees the old table first) and lets the destructor free the second table.
#define OWNER_ROW(OWNER, PTR, CNT, ELEM, STRIDE)                                                              \
    do {                                                                                                      \
        CHECK(Structs::ListingStride::ELEM == (STRIDE));                                                      \
        Structs::OWNER o;                                                                                     \
        CounterStream s1(0, false);                                                                           \
        o.read(&s1);                                                                                          \
        CHECK(o.PTR != nullptr);                                                                              \
        if (o.PTR != nullptr) {                                                                               \
            CHECK(pydata::ElementTableListingStride(o.PTR) == (STRIDE));                                      \
            CHECK(pydata::ElementTableCount(o.PTR) == o.CNT);                                                 \
        }                                                                                                     \
        o.finalize();                                                                                         \
        CHECK(o.PTR == nullptr && o.CNT == 0);                                                                \
        CounterStream s2(0, false);                                                                           \
        o.read(&s2);                                                                                          \
        CounterStream s3(0, false);                                                                           \
        o.read(&s3); /* the second read destroys the first table */                                           \
        CHECK(o.PTR != nullptr);                                                                              \
    } while (0)

static void ConditionalWordsOfTheOwners() {
    OWNER_ROW(LangSheetList, m_w2, m_w1, LangSheet, 20);
    OWNER_ROW(v2Event, m_w5, m_w4, v2QuestObjectiveStub, 44);
    OWNER_ROW(v2CondAnd, m_w2, m_w1, v2ConditionStub, 16);
    OWNER_ROW(SpawnGroup, m_w4, m_w3, SpawnInfo, 16);
    OWNER_ROW(Merchant, m_w4, m_w3, MerchandiseListEntry, 12);
    OWNER_ROW(TileOffsetList, m_w2, m_w1, TileOffset, 8);
    OWNER_ROW(NumProbList, m_w2, m_w1, NumProb, 8);
    OWNER_ROW(ItemListEntryList, m_w2, m_w1, ItemListEntry, 12);
    OWNER_ROW(Loot, m_w4, m_w3, LootEntry, 36);
    OWNER_ROW(Loot, m_w6, m_w5, LootEntry, 36);
    OWNER_ROW(AnimFXTpl, m_w4, m_w3, AnimFX, 48);
    OWNER_ROW(DialogStepList, m_w2, m_w1, DialogStep, 20);
    OWNER_ROW(ConditionalDialog, m_w2, m_w1, DialogCondition, 16);
    OWNER_ROW(CharTemplate, m_w2, m_w1, CharInfoName, 8);
    OWNER_ROW(StatAutoAssignTargetList, m_w2, m_w1, StatAutoAssignTarget, 16);
    OWNER_ROW(ClassFuncList, m_w2, m_w1, ClassFunc, 24);
    OWNER_ROW(AnimTpl, m_w3, m_w2, AnimStep, 56);
    OWNER_ROW(AIFactions, m_w2, m_w1, AIFaction, 12);
    OWNER_ROW(ItemPowerEntryList, m_w2, m_w1, ItemPowerEntry, 12);
    OWNER_ROW(ItemPowerRef, m_w4, m_w3, AttrBonusEntry, 16);
}

static void V2QuestFiveTables() {
    // v2Quest owns five tables: v2ConditionStub (16), v2QuestObjectiveStub (44) and v2QuestRewardStub three times (16).
    Structs::v2Quest q;
    CounterStream s(0, false);
    q.read(&s);
    CHECK(q.m_w6 != nullptr && pydata::ElementTableListingStride(q.m_w6) == 16);
    CHECK(q.m_w8 != nullptr && pydata::ElementTableListingStride(q.m_w8) == 44);
    CHECK(q.m_w10 != nullptr && pydata::ElementTableListingStride(q.m_w10) == 16);
    CHECK(q.m_w12 != nullptr && pydata::ElementTableListingStride(q.m_w12) == 16);
    CHECK(q.m_w14 != nullptr && pydata::ElementTableListingStride(q.m_w14) == 16);
    CHECK(pydata::ElementTableCount(q.m_w6) == q.m_w5);
    CHECK(pydata::ElementTableCount(q.m_w8) == q.m_w7);
    CHECK(pydata::ElementTableCount(q.m_w10) == q.m_w9);
    CHECK(pydata::ElementTableCount(q.m_w12) == q.m_w11);
    CHECK(pydata::ElementTableCount(q.m_w14) == q.m_w13);
    if (sizeof(void*) == 8) {
        CHECK(sizeof(Structs::v2ConditionStub) > Structs::ListingStride::v2ConditionStub);
        CHECK(sizeof(Structs::v2QuestObjectiveStub) > Structs::ListingStride::v2QuestObjectiveStub);
    }
    q.finalize();
    CHECK(q.m_w6 == nullptr && q.m_w8 == nullptr && q.m_w10 == nullptr && q.m_w12 == nullptr && q.m_w14 == nullptr);
    CHECK(q.m_w5 == 0 && q.m_w7 == 0 && q.m_w9 == 0 && q.m_w11 == 0 && q.m_w13 == 0);
    CounterStream s2(0, false);
    q.read(&s2);  // read again, then the destructor frees the second set
}

static void DestructorOfAnUnreadRecordIsSafe() {
    // The destructors test each table pointer (the listing: `if (v1)`); an unread record owns no table.
    { Structs::v2Quest q; }
    { Structs::Loot l; }
    { Structs::AnimTpl a; }
    { Structs::ClassFuncList c; }
    { Structs::ItemPowerRef r; }
    CHECK(true);
}

static void ItemPowerFamilyFinalizeThenDestroy() {
    // A derived ItemPower record frees its table in finalize and clears the pointer, so the destructors (whose
    // base repeats the loop, as the listing does) find nothing left to free. Elements are AttrBonusEntry, listing
    // stride 16, host objects larger than that.
    Structs::ItemPowerAR a;
    CounterStream s(0, false);
    a.read(&s);
    CHECK(a.m_w4 != nullptr && pydata::ElementTableListingStride(a.m_w4) == 16);
    CHECK(pydata::ElementTableCount(a.m_w4) == a.m_w3);
    if (sizeof(void*) == 8) CHECK(sizeof(Structs::AttrBonusEntry) > Structs::ListingStride::AttrBonusEntry);
    a.finalize();
    CHECK(a.m_w4 == nullptr && a.m_w3 == 0);
}

static void PackedLangSheetList() {
    // Explicit decode: LangSheetList = count, then per element two (length, text) pairs; listing stride 20, file
    // bytes per element = 4 + len + 4 + len.
    BufferStream s;
    s.u32(2);
    s.u32(2).str("en").u32(3).str("one");
    s.u32(4).str("fran").u32(5).str("deux!");
    Structs::LangSheetList l;
    l.read(&s);
    CHECK(s.pos == s.bytes.size());
    CHECK(l.m_w1 == 2 && l.m_w2 != nullptr);
    if (l.m_w2 == nullptr) return;
    CHECK(pydata::ElementTableListingStride(l.m_w2) == 20);
    const auto* e = static_cast<const Structs::LangSheet*>(l.m_w2);
    CHECK(e[0].m_w1 == 2 && std::memcmp(e[0].m_w2, "en", 3) == 0 && e[0].m_w3 == 3 && std::memcmp(e[0].m_w4, "one", 4) == 0);
    CHECK(e[1].m_w1 == 4 && std::memcmp(e[1].m_w2, "fran", 5) == 0 && e[1].m_w3 == 5 && std::memcmp(e[1].m_w4, "deux!", 6) == 0);
}

static void PackedAnimTpl() {
    // AnimTpl = int, count, AnimStep table, int. AnimStep = 4 bools/ints, a count + int32 array, ... (listing 56,
    // host larger). Two steps with empty arrays; the surrounding words come back unchanged.
    BufferStream s;
    s.i32(77).u32(2);
    for (int e = 0; e < 2; ++e) {
        s.u8(1).i32(10 * e + 1).i32(10 * e + 2).i32(10 * e + 3).u8(0).i32(10 * e + 4).u8(1);  // +0x4 .. +0x1c
        s.u32(2).i32(100 * e + 1).i32(100 * e + 2);                                          // +0x20 count, +0x24 words
        s.i32(10 * e + 5).i32(10 * e + 6).f32(2.5f + e).u8(e);                               // +0x28 .. +0x34
    }
    s.i32(88);
    Structs::AnimTpl t;
    t.read(&s);
    CHECK(s.pos == s.bytes.size());
    CHECK(t.m_w1 == 77 && t.m_w2 == 2 && t.m_w4 == 88);
    CHECK(t.m_w3 != nullptr);
    if (t.m_w3 == nullptr) return;
    CHECK(pydata::ElementTableListingStride(t.m_w3) == 56);
    const auto* st = static_cast<const Structs::AnimStep*>(t.m_w3);
    for (int e = 0; e < 2; ++e) {
        CHECK(st[e].m_w1 == true && st[e].m_w2 == 10 * e + 1 && st[e].m_w3 == 10 * e + 2 && st[e].m_w4 == 10 * e + 3);
        CHECK(st[e].m_w5 == false && st[e].m_w6 == 10 * e + 4 && st[e].m_w7 == true);
        CHECK(st[e].m_w8 == 2 && st[e].m_w9 != nullptr);
        if (st[e].m_w9 != nullptr) {
            const auto* w = static_cast<const std::int32_t*>(st[e].m_w9);
            CHECK(w[0] == 100 * e + 1 && w[1] == 100 * e + 2);
        }
        CHECK(st[e].m_w10 == 10 * e + 5 && st[e].m_w11 == 10 * e + 6 && st[e].m_w12 == 2.5f + e && st[e].m_w13 == (e != 0));
    }
}

int main() {
    ConditionalWordsOfTheOwners();
    V2QuestFiveTables();
    DestructorOfAnUnreadRecordIsSafe();
    ItemPowerFamilyFinalizeThenDestroy();
    PackedLangSheetList();
    PackedAnimTpl();
    if (failures == 0) std::printf("struct_tables_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
