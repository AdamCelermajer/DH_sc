#pragma once

// Structs::X records of slice structs_2 (pydata-accessors wave 7). Definitions are in
// src/w7_structs_2.cpp. Field names follow structs_records.hpp: m_wN is word N (offset 4*N), m_offN an
// unaligned offset. Each comment carries the original offset. Element arrays hold host records reached through
// typed pointers (element_table.hpp; NOTES.md, "Element tables"). See the notes in the .cpp for the omitted read bodies.
#include <cstdint>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/deps/stream_reader.hpp"
#include "pydata-accessors/name_table.hpp"
#include "pydata-accessors/script_cmd.hpp"
#include "pydata-accessors/structs_records.hpp"

namespace Structs {

class v2QuestAutomatic : public v2QuestObjectiveStub {
public:
    ~v2QuestAutomatic() override;
    void finalize() override;
    void read(IStreamBase* stream) override;  // 0x504a90, thunk: forwards to v2QuestObjectiveStub::read
    // NO members of its own. The original header (v2quests_pystruct.h) declares
    // `class v2QuestAutomatic : public v2QuestObjectiveStub` with ZERO fields, so sizeof stays at the
    // base's 0x2c. This class previously re-declared m_w4 and m_w6 here with `// +0x10` / `// +0x18`
    // comments: because v2QuestObjectiveStub already has those names at +0x10/+0x18, they were NEW
    // members SHADOWING the base ones, living at +0x2c/+0x30 and making sizeof 0x34. finalize then wrote
    // the shadowed members and left the real size words at +0x10/+0x18 stale. The listing at 0x4d0054
    // clears +0x10/+0x18 (CustomFree on +0x14/+0x1c, zero the +0x10/+0x18 size words) and then calls
    // v2QuestObjectiveStub::finalize(this) - i.e. the base words. Removing the two declarations makes the
    // unqualified m_w4/m_w6 in the body resolve to the base members, at the offsets their comments claim.
};
void DeletingDestructor(v2QuestAutomatic* self);




class v2CondAnd {
public:
    static constexpr int kCount = 2;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~v2CondAnd();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;  // +0x8, element array (v2ConditionStub, stride 16)
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(v2CondAnd* self);


class SpawnGroup {
public:
    static constexpr int kCount = 3;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~SpawnGroup();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    bool m_w1 = false;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, element count
    void* m_w4 = nullptr;  // +0x10, element array (SpawnInfo, stride 16)
};
void DeletingDestructor(SpawnGroup* self);


class CharSounds {
public:
    static constexpr int kCount = 6;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~CharSounds();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, count of m_w2
    void* m_w2 = nullptr;  // +0x8, int32 array
    std::uint32_t m_w3 = 0;  // +0xc, count of m_w4
    void* m_w4 = nullptr;  // +0x10, int32 array
    std::uint32_t m_w5 = 0;  // +0x14, count of m_w6
    void* m_w6 = nullptr;  // +0x18, int32 array
    std::uint32_t m_w7 = 0;  // +0x1c, count of m_w8
    void* m_w8 = nullptr;  // +0x20, int32 array
    bool m_w9 = false;  // +0x24
    bool m_off37 = false;  // +0x25
};
void DeletingDestructor(CharSounds* self);


class Sound {
public:
    static constexpr int kCount = 9;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~Sound();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, string length
    void* m_w4 = nullptr;  // +0x10, string buffer (len + 1 bytes)
    std::int32_t m_w5 = 0;  // +0x14
    std::int32_t m_w6 = 0;  // +0x18
    std::int32_t m_w7 = 0;  // +0x1c
    std::int32_t m_w8 = 0;  // +0x20
    std::int32_t m_w9 = 0;  // +0x24
    std::int32_t m_w10 = 0;  // +0x28
};
void DeletingDestructor(Sound* self);


class SoundGroup {
public:
    static constexpr int kCount = 2;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~SoundGroup();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, string length
    void* m_w2 = nullptr;  // +0x8, string buffer (len + 1 bytes)
    std::int32_t m_w3 = 0;  // +0xc
};
void DeletingDestructor(SoundGroup* self);


class HideHUD : public HideFlash {
public:
    ~HideHUD() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(HideHUD* self);


class ShowHUD : public ShowFlash {
public:
    ~ShowHUD() override;
    void finalize() override;
    void read(IStreamBase* stream) override;
};
void DeletingDestructor(ShowHUD* self);


class SceneFile {
public:
    virtual ~SceneFile();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, string length
    void* m_w2 = nullptr;  // +0x8, string buffer (len + 1 bytes)
};
void DeletingDestructor(SceneFile* self);


class ItemTypeListList {
public:
    static constexpr int kCount = 1;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~ItemTypeListList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, byte count of m_w2
    void* m_w2 = nullptr;  // +0x8, int8 array
};
void DeletingDestructor(ItemTypeListList* self);


class Merchant {
public:
    static constexpr int kCount = 3;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~Merchant();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, element count
    void* m_w4 = nullptr;  // +0x10, element array (MerchandiseListEntry, stride 12)
};
void DeletingDestructor(Merchant* self);


class TileOffsetList {
public:
    static constexpr int kCount = 1;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~TileOffsetList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;  // +0x8, element array (TileOffset, stride 8)
};
void DeletingDestructor(TileOffsetList* self);


class NumProbList {
public:
    static constexpr int kCount = 1;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~NumProbList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;  // +0x8, element array (NumProb, stride 8)
};
void DeletingDestructor(NumProbList* self);


class ItemListEntryList {
public:
    static constexpr int kCount = 1;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~ItemListEntryList();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::uint32_t m_w1 = 0;  // +0x4, element count
    void* m_w2 = nullptr;  // +0x8, element array (ItemListEntry, stride 12)
};
void DeletingDestructor(ItemListEntryList* self);


class Loot {
public:
    static constexpr int kCount = 5;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    virtual ~Loot();
    virtual void finalize();
    virtual void read(IStreamBase* stream);
    std::int32_t m_w1 = 0;  // +0x4
    std::int32_t m_w2 = 0;  // +0x8
    std::uint32_t m_w3 = 0;  // +0xc, element count of m_w4
    void* m_w4 = nullptr;  // +0x10, element array (LootEntry, stride 36)
    std::uint32_t m_w5 = 0;  // +0x14, element count of m_w6
    void* m_w6 = nullptr;  // +0x18, element array (LootEntry, stride 36)
    std::uint32_t m_w7 = 0;  // +0x1c, element count of m_w8 (int32 elements)
    void* m_w8 = nullptr;  // +0x20, int32 array
};
void DeletingDestructor(Loot* self);


class WeaponRef : public ItemBase {
public:
    ~WeaponRef() override;
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};
void DeletingDestructor(WeaponRef* self);


class RangedWeaponRef : public ItemBase {
public:
    ~RangedWeaponRef() override;
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};
void DeletingDestructor(RangedWeaponRef* self);


class ShieldRef : public ItemBase {
public:
    ~ShieldRef() override;
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};
void DeletingDestructor(ShieldRef* self);


class JewelryRef : public ItemBase {
public:
    ~JewelryRef() override;
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};
void DeletingDestructor(JewelryRef* self);


class ArmorRef : public ItemBase {
public:
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};


class Valuable : public ItemBase {
public:
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};


class Potion : public ItemBase {
public:
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};


class GoldItem : public ItemBase {
public:
    void finalize() override;
    void read(IStreamBase* stream);
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer (len + 1 bytes)
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};


class Item : public ItemBase {
public:
    static constexpr int kCount = 38;  // member names: the PyData name table of the class (member_names.hpp)
    static pydata::NameTable m_dataNames;
    void finalize() override;
    void read(IStreamBase* stream) override;
    // m_w1 (+0x4, zeroed by finalize) is ItemBase::m_w1: the record does not declare a second one.
    std::int32_t m_w17 = 0;  // +0x44
    std::int32_t m_w18 = 0;  // +0x48
    std::uint32_t m_w19 = 0;  // +0x4c, string length
    void* m_w20 = nullptr;  // +0x50, string buffer
    std::int32_t m_w21 = 0;  // +0x54
    std::int32_t m_w22 = 0;  // +0x58
    std::int32_t m_w23 = 0;  // +0x5c
    std::int32_t m_w24 = 0;  // +0x60
    std::int32_t m_w25 = 0;  // +0x64
    std::int32_t m_w26 = 0;  // +0x68
    std::int32_t m_w27 = 0;  // +0x6c
    std::int32_t m_w28 = 0;  // +0x70
    std::int32_t m_w29 = 0;  // +0x74
    std::int32_t m_w30 = 0;  // +0x78
    std::int32_t m_w31 = 0;  // +0x7c
    std::int32_t m_w32 = 0;  // +0x80
    std::int32_t m_w33 = 0;  // +0x84
    std::int32_t m_w34 = 0;  // +0x88
    std::int32_t m_w35 = 0;  // +0x8c
    std::int32_t m_w36 = 0;  // +0x90
    std::int32_t m_w37 = 0;  // +0x94
    std::int32_t m_w38 = 0;  // +0x98
    std::int32_t m_w39 = 0;  // +0x9c
    std::int32_t m_w40 = 0;  // +0xa0
};


class BeltRef : public ArmorRef {
public:
    void finalize() override;
    void read(IStreamBase* stream);
};


class HelmRef : public ArmorRef {
public:
    void finalize() override;
    void read(IStreamBase* stream);
};


class GloveRef : public ArmorRef {
public:
    void finalize() override;
    void read(IStreamBase* stream);
};


class BootsRef : public ArmorRef {
public:
    void finalize() override;
    void read(IStreamBase* stream);
};


}  // namespace Structs
