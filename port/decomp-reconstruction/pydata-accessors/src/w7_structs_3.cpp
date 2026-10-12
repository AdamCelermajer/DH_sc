// Structs records, slice structs_3 (see include/pydata-accessors/w7_structs_3.hpp).
#include <cstring>

#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_2.hpp"  // Item family (superset layout)
#include "pydata-accessors/array_element.hpp"

namespace {

// Element of the ItemPowerRef table: AttrBonusEntry (its vtable is off_96B368, the vptr the listing stores
// into each element). The listing's stride is 16; the in-memory stride is sizeof(element).
constexpr std::size_t kItemPowerElementStride = sizeof(Structs::AttrBonusEntry);

// The vptr of AttrBonusEntry: the first word of a polymorphic object (Itanium ABI).
const void* AttrBonusEntryVptr() {
    Structs::AttrBonusEntry probe;
    const void* vptr = nullptr;
    std::memcpy(&vptr, &probe, sizeof vptr);
    return vptr;
}

// Original loop of ItemPowerRef::finalize (0x4d573c). Destroys the elements from the last to the first
// through vtable slot 0 (the virtual destructor), then frees the table block at members - 8. The caller
// clears its count and table pointer.
void ReleaseElementTable(void* members) {
    auto* header = static_cast<std::uint32_t*>(members) - 2;  // header[0] = stride, header[1] = count
    const std::uint32_t count = header[1];
    auto* bytes = static_cast<unsigned char*>(members);
    for (std::uint32_t i = count; i > 0; --i) {
        reinterpret_cast<Structs::AttrBonusEntry*>(bytes + kItemPowerElementStride * (i - 1))->~AttrBonusEntry();
    }
    CustomFree(header);
}

}  // namespace

namespace Structs {

// 0x4d4d38 _ZN7Structs8ArmorRefD2Ev
// 0x4d4e04 _ZN7Structs7BeltRefD2Ev
// 0x4d4ed0 _ZN7Structs7HelmRefD2Ev
// 0x4d4f9c _ZN7Structs8GloveRefD2Ev
// 0x4d5068 _ZN7Structs8BootsRefD2Ev
// 0x4d5134 _ZN7Structs8ValuableD2Ev
// 0x4d5200 _ZN7Structs6PotionD2Ev
// 0x4d52cc _ZN7Structs8GoldItemD2Ev
// 0x4d5398 _ZN7Structs4ItemD2Ev
// 0x4d5474 _ZN7Structs15ItemAudioVisualD2Ev
// 0x4d5418 _ZN7Structs15ItemAudioVisualD1Ev
ItemAudioVisual::~ItemAudioVisual() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4d5458 _ZN7Structs15ItemAudioVisualD0Ev
void DeletingDestructor(ItemAudioVisual* self) {
    self->ItemAudioVisual::~ItemAudioVisual();
    CustomFree(self);
}

// 0x4d53f0 _ZN7Structs15ItemAudioVisual8finalizeEv
void ItemAudioVisual::finalize() {
    if (m_w4 != nullptr) {
        CustomFree(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
}

// 0x4fc910 _ZN7Structs15ItemAudioVisual4readEP11IStreamBase
void ItemAudioVisual::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);   // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);   // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    if (m_w4 != nullptr) CustomFree(m_w4);               // +0x10
    m_w4 = MemoryHintState(static_cast<std::size_t>(m_w3) + 1, 1);
    // PARTIAL: the listing passes a 5th, 64-bit argument to readStringEx (high word 1, low word undefined in
    // the listing), and returns the readStringEx result as a bool. The stand-in readStringEx takes four
    // arguments and returns void, so neither is passed or returned.
    StreamReader::readStringEx(stream, static_cast<char*>(m_w4), m_w3, 0);
    static_cast<char*>(m_w4)[m_w3] = 0;
}

// 0x4d56fc _ZN7Structs17ItemBonusAttrListD2Ev
// 0x4d56a0 _ZN7Structs17ItemBonusAttrListD1Ev
ItemBonusAttrList::~ItemBonusAttrList() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4d56e0 _ZN7Structs17ItemBonusAttrListD0Ev
void DeletingDestructor(ItemBonusAttrList* self) {
    self->ItemBonusAttrList::~ItemBonusAttrList();
    CustomFree(self);
}

// 0x4d5678 _ZN7Structs17ItemBonusAttrList8finalizeEv
void ItemBonusAttrList::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4ea954 _ZN7Structs17ItemBonusAttrList4readEP11IStreamBase
void ItemBonusAttrList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4 (count)
    if (m_w2 != nullptr) CustomFree(m_w2);               // +0x8
    m_w2 = MemoryHintState(4 * static_cast<std::size_t>(m_w1), 1);
    auto* words = static_cast<std::int32_t*>(m_w2);
    for (std::uint32_t i = 0; i < m_w1; ++i) {
        words[i] = StreamReader::readAs<std::int32_t>(stream);
    }
}

// 0x4d573c _ZN7Structs12ItemPowerRef8finalizeEv
void ItemPowerRef::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
}

// 0x4ecdc8 _ZN7Structs12ItemPowerRef4readEP11IStreamBase
// The table elements are AttrBonusEntry (vtable off_96B368, see 0x4fef00). Each element's vptr is stored, then
// each element's read (vtable slot +0xc) runs in order, before the trailing words are read. Listing stride 16,
// in-memory stride sizeof(AttrBonusEntry).
void ItemPowerRef::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int8_t>(stream);   // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream); // +0xc
    if (m_w4 != nullptr) ReleaseElementTable(m_w4);     // +0x10 (old table)
    const std::uint32_t count = m_w3;
    const std::size_t stride = kItemPowerElementStride;
    auto* header = static_cast<std::uint32_t*>(MemoryHintState(stride * static_cast<std::size_t>(count) + 8, 1));
    header[0] = static_cast<std::uint32_t>(stride);  // element stride
    header[1] = count;                               // element count
    auto* data = reinterpret_cast<std::uint8_t*>(header + 2);
    const void* vptr = AttrBonusEntryVptr();
    for (std::uint32_t i = 0; i < count; ++i) {
        std::memcpy(data + stride * i, &vptr, sizeof vptr);
    }
    m_w4 = data;
    for (std::uint32_t i = 0; i < count; ++i) {
        reinterpret_cast<Structs::AttrBonusEntry*>(data + stride * i)->read(stream);
    }
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
}

// 0x4d5f38 _ZN7Structs11ItemPowerAR8finalizeEv
void ItemPowerAR::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1a0 _ZN7Structs11ItemPowerAR4readEP11IStreamBase
void ItemPowerAR::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d658c _ZN7Structs18ItemPowerAirDamage8finalizeEv
void ItemPowerAirDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1dc _ZN7Structs18ItemPowerAirDamage4readEP11IStreamBase
void ItemPowerAirDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d68ec _ZN7Structs17ItemPowerAllStats8finalizeEv
void ItemPowerAllStats::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1fc _ZN7Structs17ItemPowerAllStats4readEP11IStreamBase
void ItemPowerAllStats::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d607c _ZN7Structs14ItemPowerArmor8finalizeEv
void ItemPowerArmor::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1ac _ZN7Structs14ItemPowerArmor4readEP11IStreamBase
void ItemPowerArmor::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d59bc _ZN7Structs23ItemPowerAxeDamageBonus8finalizeEv
void ItemPowerAxeDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed16c _ZN7Structs23ItemPowerAxeDamageBonus4readEP11IStreamBase
void ItemPowerAxeDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d60e8 _ZN7Structs14ItemPowerBlock8finalizeEv
void ItemPowerBlock::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1b0 _ZN7Structs14ItemPowerBlock4readEP11IStreamBase
void ItemPowerBlock::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d61c0 _ZN7Structs17ItemPowerCritical8finalizeEv
void ItemPowerCritical::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1b8 _ZN7Structs17ItemPowerCritical4readEP11IStreamBase
void ItemPowerCritical::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5ecc _ZN7Structs11ItemPowerDR8finalizeEv
void ItemPowerDR::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed19c _ZN7Structs11ItemPowerDR4readEP11IStreamBase
void ItemPowerDR::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d58e4 _ZN7Structs26ItemPowerDaggerDamageBonus8finalizeEv
void ItemPowerDaggerDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed164 _ZN7Structs26ItemPowerDaggerDamageBonus4readEP11IStreamBase
void ItemPowerDaggerDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6010 _ZN7Structs15ItemPowerDamage8finalizeEv
void ItemPowerDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1a8 _ZN7Structs15ItemPowerDamage4readEP11IStreamBase
void ItemPowerDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6814 _ZN7Structs18ItemPowerDexterity8finalizeEv
void ItemPowerDexterity::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1f4 _ZN7Structs18ItemPowerDexterity4readEP11IStreamBase
void ItemPowerDexterity::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6154 _ZN7Structs14ItemPowerDodge8finalizeEv
void ItemPowerDodge::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1b4 _ZN7Structs14ItemPowerDodge4readEP11IStreamBase
void ItemPowerDodge::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6664 _ZN7Structs20ItemPowerEarthDamage8finalizeEv
void ItemPowerEarthDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1e4 _ZN7Structs20ItemPowerEarthDamage4readEP11IStreamBase
void ItemPowerEarthDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d64b4 _ZN7Structs23ItemPowerEarthDotDamage8finalizeEv
void ItemPowerEarthDotDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1d4 _ZN7Structs23ItemPowerEarthDotDamage4readEP11IStreamBase
void ItemPowerEarthDotDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6520 _ZN7Structs25ItemPowerElectricalDamage8finalizeEv
void ItemPowerElectricalDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1d8 _ZN7Structs25ItemPowerElectricalDamage4readEP11IStreamBase
void ItemPowerElectricalDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d67a8 _ZN7Structs18ItemPowerEndurance8finalizeEv
void ItemPowerEndurance::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1f0 _ZN7Structs18ItemPowerEndurance4readEP11IStreamBase
void ItemPowerEndurance::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d673c _ZN7Structs15ItemPowerEnergy8finalizeEv
void ItemPowerEnergy::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1ec _ZN7Structs15ItemPowerEnergy4readEP11IStreamBase
void ItemPowerEnergy::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d66d0 _ZN7Structs19ItemPowerFireDamage8finalizeEv
void ItemPowerFireDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1e8 _ZN7Structs19ItemPowerFireDamage4readEP11IStreamBase
void ItemPowerFireDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d63dc _ZN7Structs22ItemPowerFireDotDamage8finalizeEv
void ItemPowerFireDotDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1cc _ZN7Structs22ItemPowerFireDotDamage4readEP11IStreamBase
void ItemPowerFireDotDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6370 _ZN7Structs11ItemPowerHP8finalizeEv
void ItemPowerHP::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1c8 _ZN7Structs11ItemPowerHP4readEP11IStreamBase
void ItemPowerHP::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6298 _ZN7Structs16ItemPowerHPRegen8finalizeEv
void ItemPowerHPRegen::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1c0 _ZN7Structs16ItemPowerHPRegen4readEP11IStreamBase
void ItemPowerHPRegen::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5bd8 _ZN7Structs18ItemPowerLifeLeech8finalizeEv
void ItemPowerLifeLeech::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed180 _ZN7Structs18ItemPowerLifeLeech4readEP11IStreamBase
void ItemPowerLifeLeech::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6304 _ZN7Structs11ItemPowerMP8finalizeEv
void ItemPowerMP::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1c4 _ZN7Structs11ItemPowerMP4readEP11IStreamBase
void ItemPowerMP::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d622c _ZN7Structs16ItemPowerMPRegen8finalizeEv
void ItemPowerMPRegen::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1bc _ZN7Structs16ItemPowerMPRegen4readEP11IStreamBase
void ItemPowerMPRegen::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5950 _ZN7Structs24ItemPowerMaceDamageBonus8finalizeEv
void ItemPowerMaceDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed168 _ZN7Structs24ItemPowerMaceDamageBonus4readEP11IStreamBase
void ItemPowerMaceDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5b6c _ZN7Structs18ItemPowerManaLeech8finalizeEv
void ItemPowerManaLeech::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed17c _ZN7Structs18ItemPowerManaLeech4readEP11IStreamBase
void ItemPowerManaLeech::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5878 _ZN7Structs25ItemPowerRangeDamageBonus8finalizeEv
void ItemPowerRangeDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed160 _ZN7Structs25ItemPowerRangeDamageBonus4readEP11IStreamBase
void ItemPowerRangeDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5d1c _ZN7Structs18ItemPowerResistAir8finalizeEv
void ItemPowerResistAir::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed18c _ZN7Structs18ItemPowerResistAir4readEP11IStreamBase
void ItemPowerResistAir::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5c44 _ZN7Structs18ItemPowerResistAll8finalizeEv
void ItemPowerResistAll::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed184 _ZN7Structs18ItemPowerResistAll4readEP11IStreamBase
void ItemPowerResistAll::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5df4 _ZN7Structs20ItemPowerResistEarth8finalizeEv
void ItemPowerResistEarth::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed194 _ZN7Structs20ItemPowerResistEarth4readEP11IStreamBase
void ItemPowerResistEarth::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5cb0 _ZN7Structs25ItemPowerResistElectrical8finalizeEv
void ItemPowerResistElectrical::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed188 _ZN7Structs25ItemPowerResistElectrical4readEP11IStreamBase
void ItemPowerResistElectrical::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5e60 _ZN7Structs19ItemPowerResistFire8finalizeEv
void ItemPowerResistFire::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed198 _ZN7Structs19ItemPowerResistFire4readEP11IStreamBase
void ItemPowerResistFire::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5d88 _ZN7Structs20ItemPowerResistWater8finalizeEv
void ItemPowerResistWater::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed190 _ZN7Structs20ItemPowerResistWater4readEP11IStreamBase
void ItemPowerResistWater::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5b00 _ZN7Structs22ItemPowerSpellCritical8finalizeEv
void ItemPowerSpellCritical::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed178 _ZN7Structs22ItemPowerSpellCritical4readEP11IStreamBase
void ItemPowerSpellCritical::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5a94 _ZN7Structs19ItemPowerSpellToHit8finalizeEv
void ItemPowerSpellToHit::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed174 _ZN7Structs19ItemPowerSpellToHit4readEP11IStreamBase
void ItemPowerSpellToHit::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d580c _ZN7Structs25ItemPowerStaffDamageBonus8finalizeEv
void ItemPowerStaffDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed15c _ZN7Structs25ItemPowerStaffDamageBonus4readEP11IStreamBase
void ItemPowerStaffDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6880 _ZN7Structs17ItemPowerStrength8finalizeEv
void ItemPowerStrength::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1f8 _ZN7Structs17ItemPowerStrength4readEP11IStreamBase
void ItemPowerStrength::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5a28 _ZN7Structs25ItemPowerSwordDamageBonus8finalizeEv
void ItemPowerSwordDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed170 _ZN7Structs25ItemPowerSwordDamageBonus4readEP11IStreamBase
void ItemPowerSwordDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d5fa4 _ZN7Structs14ItemPowerToHit8finalizeEv
void ItemPowerToHit::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1a4 _ZN7Structs14ItemPowerToHit4readEP11IStreamBase
void ItemPowerToHit::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d57a0 _ZN7Structs24ItemPowerWandDamageBonus8finalizeEv
void ItemPowerWandDamageBonus::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed158 _ZN7Structs24ItemPowerWandDamageBonus4readEP11IStreamBase
void ItemPowerWandDamageBonus::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d65f8 _ZN7Structs20ItemPowerWaterDamage8finalizeEv
void ItemPowerWaterDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1e0 _ZN7Structs20ItemPowerWaterDamage4readEP11IStreamBase
void ItemPowerWaterDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

// 0x4d6448 _ZN7Structs23ItemPowerWaterDotDamage8finalizeEv
void ItemPowerWaterDotDamage::finalize() {
    if (m_w4 != nullptr) {
        ReleaseElementTable(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
    ItemPowerRef::finalize();
}

// thunk: b 0x4ecdc8, forwards to ItemPowerRef::read
// 0x4ed1d0 _ZN7Structs23ItemPowerWaterDotDamage4readEP11IStreamBase
void ItemPowerWaterDotDamage::read(IStreamBase* stream) {
    ItemPowerRef::read(stream);
}

}  // namespace Structs
