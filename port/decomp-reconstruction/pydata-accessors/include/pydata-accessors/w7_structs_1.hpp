#pragma once

// Structs records for slice structs_1 (see NOTES.md, "Structs records" and "Thunks: read and finalize forwarders").
// Quest rewards (v2QuestReward family), LangSheetList/LangSheet, the quest objective family
// (v2QuestObjective family), v2Event and v2Quest. Field names are m_wN (word N, offset 4*N); the
// original 32-bit offset is in each comment. Freed pointers are void* or char* (the CustomFree argument).
//
// Bases come from structs_records.hpp (v2QuestReward, v2QuestObjective) and array_element.hpp
// (LangSheet). Classes in this header are defined here; do not include member_names.hpp in the same
// translation unit, because it declares the name-table structs Structs::v2Quest, Structs::v2Event and
// Structs::LangSheetList (see the notes on this slice in the ledger).
//
// "partial" methods are declared here with their stores; the base read call is not declared (no header
// defines v2QuestReward::read or v2QuestObjective::read), so it is omitted (see the ledger notes).

#include <cstddef>
#include <cstdint>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/deps/stream_reader.hpp"
#include "pydata-accessors/structs_records.hpp"

namespace Structs {

// ---- Quest rewards: base v2QuestReward (structs_records.hpp) ----

class v2QuestRewardGiveXP : public v2QuestReward {
public:
    ~v2QuestRewardGiveXP() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(v2QuestRewardGiveXP* self);

class v2QuestRewardGiveGold : public v2QuestReward {
public:
    ~v2QuestRewardGiveGold() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(v2QuestRewardGiveGold* self);

class v2QuestRewardGiveCharProps : public v2QuestReward {
public:
    ~v2QuestRewardGiveCharProps() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(v2QuestRewardGiveCharProps* self);

// Read and finalize are forwarders (thunks) to v2QuestRewardGiveCharProps.
class v2QuestRewardGiveSkillPoint : public v2QuestRewardGiveCharProps {
public:
    ~v2QuestRewardGiveSkillPoint() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestRewardGiveSkillPoint* self);

class v2QuestRewardGiveStatPoint : public v2QuestRewardGiveCharProps {
public:
    ~v2QuestRewardGiveStatPoint() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestRewardGiveStatPoint* self);

class v2QuestRewardGiveMaxMPBonus : public v2QuestRewardGiveCharProps {
public:
    ~v2QuestRewardGiveMaxMPBonus() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestRewardGiveMaxMPBonus* self);

class v2QuestRewardGiveLoot : public v2QuestReward {
public:
    ~v2QuestRewardGiveLoot() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(v2QuestRewardGiveLoot* self);

class v2QuestRewardConsumeLoot : public v2QuestReward {
public:
    ~v2QuestRewardConsumeLoot() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::int32_t m_w2 = 0;  // +0x8
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(v2QuestRewardConsumeLoot* self);

// ---- LangSheetList and its elements ----

// Element of LangSheetList. The element base is pydata::ArrayElement (vptr at +0x0).
class LangSheet : public pydata::ArrayElement {
public:
    ~LangSheet() override;
    void finalize() override;
    void read(IStreamBase* stream) override;  // partial: see ledger
    std::uint32_t m_w1 = 0;  // +0x4
    char* m_w2 = nullptr;    // +0x8
    std::uint32_t m_w3 = 0;  // +0xc
    char* m_w4 = nullptr;    // +0x10
};
void DeletingDestructor(LangSheet* self);

class LangSheetList {
public:
    virtual ~LangSheetList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;     // +0x4  element count
    std::uint8_t* m_w2 = nullptr;  // +0x8  members (element block; header at members - 8 / - 4)
};
void DeletingDestructor(LangSheetList* self);

// ---- Quest objectives: base v2QuestObjective (structs_records.hpp) ----

// Objectives with two strings (length word, buffer) at +0x10 and +0x18 and three int32 at +0x20..+0x28.
class v2QuestMoveInZone : public v2QuestObjective {
public:
    ~v2QuestMoveInZone() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestMoveInZone* self);

class v2QuestGatherLoot : public v2QuestObjective {
public:
    ~v2QuestGatherLoot() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestGatherLoot* self);

class v2QuestClearEnemyTemplate : public v2QuestObjective {
public:
    ~v2QuestClearEnemyTemplate() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestClearEnemyTemplate* self);

class v2QuestKillEnemyTemplate : public v2QuestObjective {
public:
    ~v2QuestKillEnemyTemplate() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestKillEnemyTemplate* self);

class v2QuestClearEnemies : public v2QuestObjective {
public:
    ~v2QuestClearEnemies() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestClearEnemies* self);

class v2QuestKillEnemies : public v2QuestObjective {
public:
    ~v2QuestKillEnemies() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestKillEnemies* self);

// Base of the six objectives below. Its field layout is the same as the one above.
class v2QuestInteractWithDoNotUseThisObjectiveDirectly : public v2QuestObjective {
public:
    ~v2QuestInteractWithDoNotUseThisObjectiveDirectly() override;
    void finalize() override;
    virtual void read(IStreamBase* stream);  // partial: base read call omitted
    std::uint32_t m_w4 = 0;  // +0x10
    char* m_w5 = nullptr;    // +0x14
    std::uint32_t m_w6 = 0;  // +0x18
    char* m_w7 = nullptr;    // +0x1c
    std::int32_t m_w8 = 0;   // +0x20
    std::int32_t m_w9 = 0;   // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(v2QuestInteractWithDoNotUseThisObjectiveDirectly* self);

// The six classes below add no fields. Their finalize and destructor release m_w5 and m_w7 themselves,
// then call the base. Their read is a forwarder to the base read.
class v2QuestTalkToNPC : public v2QuestInteractWithDoNotUseThisObjectiveDirectly {
public:
    ~v2QuestTalkToNPC() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestTalkToNPC* self);

class v2QuestPickedUpLiftable : public v2QuestInteractWithDoNotUseThisObjectiveDirectly {
public:
    ~v2QuestPickedUpLiftable() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestPickedUpLiftable* self);

class v2QuestTriggerOn : public v2QuestInteractWithDoNotUseThisObjectiveDirectly {
public:
    ~v2QuestTriggerOn() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestTriggerOn* self);

class v2QuestTriggerPlate : public v2QuestInteractWithDoNotUseThisObjectiveDirectly {
public:
    ~v2QuestTriggerPlate() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestTriggerPlate* self);

class v2QuestOpenGameObject : public v2QuestInteractWithDoNotUseThisObjectiveDirectly {
public:
    ~v2QuestOpenGameObject() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestOpenGameObject* self);

class v2QuestDestroyGameObject : public v2QuestInteractWithDoNotUseThisObjectiveDirectly {
public:
    ~v2QuestDestroyGameObject() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(v2QuestDestroyGameObject* self);

// ---- v2Event and v2Quest (no base in this slice) ----

// The element class of v2Event's table (44-byte stride, vtable off_96A030) is not in any header.
// The table is walked with the listing stride; its elements are not created by read (see ledger).
class v2Event {
public:
    virtual ~v2Event();
    virtual void finalize();
    virtual void read(IStreamBase* stream);  // partial: element creation missing
    std::int32_t m_w1 = 0;    // +0x4
    std::uint32_t m_w2 = 0;   // +0x8  length of m_w3
    char* m_w3 = nullptr;     // +0xc
    std::uint32_t m_w4 = 0;   // +0x10 element count
    std::uint8_t* m_w5 = nullptr;  // +0x14 members (element block)
};
void DeletingDestructor(v2Event* self);

// Only finalize is in this slice (no destructor or read). Five element tables, with listing strides
// 16, 44, 16, 16, 16; the element classes are not in any header.
class v2Quest {
public:
    virtual void finalize();
    std::uint32_t m_w5 = 0;     // +0x14 size of table 1
    std::uint8_t* m_w6 = nullptr;  // +0x18 members of table 1 (stride 16)
    std::uint32_t m_w7 = 0;     // +0x1c size of table 2
    std::uint8_t* m_w8 = nullptr;  // +0x20 members of table 2 (stride 44)
    std::uint32_t m_w9 = 0;     // +0x24 size of table 3
    std::uint8_t* m_w10 = nullptr; // +0x28 members of table 3 (stride 16)
    std::uint32_t m_w11 = 0;    // +0x2c size of table 4
    std::uint8_t* m_w12 = nullptr; // +0x30 members of table 4 (stride 16)
    std::uint32_t m_w13 = 0;    // +0x34 size of table 5
    std::uint8_t* m_w14 = nullptr; // +0x38 members of table 5 (stride 16)
};

}  // namespace Structs
