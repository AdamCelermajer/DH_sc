// Structs::X destructors, finalize and read for slice structs_5 (hand-written from the pseudocode; see
// include/pydata-accessors/w7_structs_5.hpp). Layout comments give the original 32-bit offsets.
#include "pydata-accessors/w7_structs_5.hpp"

#include <cstddef>
#include <cstring>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/structs_records.hpp"

namespace {

using pydata::ArrayElement;

constexpr std::size_t kNoZeroWord = static_cast<std::size_t>(-1);

// The vptr of Elem: the first word of a polymorphic object (the same convention as w7_arrays_02.cpp).
template <class Elem>
const void* VtableOf() {
    Elem tmp;
    const void* vptr = nullptr;
    std::memcpy(&vptr, &tmp, sizeof vptr);
    return vptr;
}

// Original element-table destructor loop: from the last element to the first, each destroyed through
// vtable slot 0 (the non-deleting destructor), then the block at members - 8 is freed.
void DestroyElements(void* members, std::size_t stride) {
    std::uint8_t* const first = static_cast<std::uint8_t*>(members);
    std::uint32_t count = 0;
    std::memcpy(&count, first - 4, sizeof count);
    std::uint8_t* cur = first + stride * count;
    while (cur != first) {
        cur -= stride;
        reinterpret_cast<ArrayElement*>(cur)->~ArrayElement();
    }
    CustomFree(first - 8);
}

// Original operator new[](stride * count + 8, 1): header words (stride, count), members = block + 8.
void* AllocElements(std::uint32_t count, std::size_t stride) {
    std::uint8_t* const block = static_cast<std::uint8_t*>(MemoryHintState(stride * count + 8, 1));
    const std::uint32_t header_stride = static_cast<std::uint32_t>(stride);
    std::memcpy(block, &header_stride, sizeof header_stride);
    std::memcpy(block + 4, &count, sizeof count);
    return block + 8;
}

// Original vptr loop: the first word of each element is the vtable of Elem.
template <class Elem>
void InstallVtables(void* members, std::uint32_t count, std::size_t stride) {
    const void* vptr = VtableOf<Elem>();
    for (std::uint32_t i = 0; i < count; ++i) {
        std::memcpy(static_cast<std::uint8_t*>(members) + stride * i, &vptr, sizeof vptr);
    }
}

// Original zeroing of one pointer word of each element (ConditionalDialog +0xc, AnimFXTpl +0x2c, both char*
// buffers). The offset is the host offsetof of that member, and the clear covers sizeof(void*) bytes.
void ZeroElementWord(void* members, std::uint32_t count, std::size_t stride, std::size_t offset) {
    const void* zero = nullptr;
    for (std::uint32_t i = 0; i < count; ++i) {
        std::memcpy(static_cast<std::uint8_t*>(members) + stride * i + offset, &zero, sizeof zero);
    }
}

// Original read loop: each element reads itself through vtable slot 3 (read), in order.
void ReadElements(void* members, std::uint32_t count, std::size_t stride, IStreamBase* stream) {
    for (std::uint32_t i = 0; i < count; ++i) {
        reinterpret_cast<ArrayElement*>(static_cast<std::uint8_t*>(members) + stride * i)->read(stream);
    }
}

// Original element-table read after the count word: destroy the old elements, allocate the new block,
// install the element vtables, zero the word at zero_offset when given, then read every element.
template <class Elem>
void BuildAndReadTable(IStreamBase* stream, std::uint32_t count, void*& members, std::size_t stride,
                       std::size_t zero_offset) {
    if (members != nullptr) DestroyElements(members, stride);
    members = AllocElements(count, stride);
    InstallVtables<Elem>(members, count, stride);
    if (zero_offset != kNoZeroWord) ZeroElementWord(members, count, stride, zero_offset);
    ReadElements(members, count, stride, stream);
}

// Original string read: free the old buffer, allocate length + 1 (hint 1), read `length` raw bytes,
// then NUL-terminate. The readStringEx 4th argument is 0 (the value the script_cmd string reads use).
void ReadBuffer(IStreamBase* stream, char*& buf, std::uint32_t length) {
    if (buf != nullptr) CustomFree(buf);
    buf = static_cast<char*>(MemoryHintState(static_cast<std::size_t>(length) + 1, 1));
    StreamReader::readStringEx(stream, buf, length, 0);
    buf[length] = '\0';
}

// Original int32 table read: free the old array, allocate 4 * count bytes, read count int32 words.
void ReadWords(IStreamBase* stream, void*& words, std::uint32_t count) {
    if (words != nullptr) CustomFree(words);
    words = MemoryHintState(4 * static_cast<std::size_t>(count), 1);
    auto* values = static_cast<std::int32_t*>(words);
    for (std::uint32_t i = 0; i < count; ++i) {
        values[i] = StreamReader::readAs<std::int32_t>(stream);
    }
}

}  // namespace

namespace Structs {

// 0x4d9abc _ZN7Structs18ItemPowerEntryListD1Ev
// 0x4d9b58 _ZN7Structs18ItemPowerEntryListD2Ev
ItemPowerEntryList::~ItemPowerEntryList() {
    if (m_w2 != nullptr) DestroyElements(m_w2, sizeof(ItemPowerEntry));  // +0x8, listing stride 12
}

// 0x4d9b3c _ZN7Structs18ItemPowerEntryListD0Ev
void DeletingDestructor(ItemPowerEntryList* self) {
    self->ItemPowerEntryList::~ItemPowerEntryList();
    CustomFree(self);
}

// 0x4d9a54 _ZN7Structs18ItemPowerEntryList8finalizeEv
void ItemPowerEntryList::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, sizeof(ItemPowerEntry));
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4dc8fc _ZN7Structs18ItemPowerEntryList4readEP11IStreamBase
void ItemPowerEntryList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    BuildAndReadTable<ItemPowerEntry>(stream, m_w1, m_w2, sizeof(ItemPowerEntry), kNoZeroWord);  // +0x8
}

// 0x4d9c00 _ZN7Structs11TriggerTrapD1Ev
// 0x4d9c5c _ZN7Structs11TriggerTrapD2Ev
TriggerTrap::~TriggerTrap() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4d9c40 _ZN7Structs11TriggerTrapD0Ev
void DeletingDestructor(TriggerTrap* self) {
    self->TriggerTrap::~TriggerTrap();
    CustomFree(self);
}

// 0x4d9bd8 _ZN7Structs11TriggerTrap8finalizeEv
void TriggerTrap::finalize() {
    if (m_w4 != nullptr) {
        CustomFree(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
}

// 0x4fd21c _ZN7Structs11TriggerTrap4readEP11IStreamBase
void TriggerTrap::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    ReadBuffer(stream, m_w4, m_w3);  // +0x10, length +0xc
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
}

// 0x4d9cc4 _ZN7Structs13TriggerObjectD1Ev
// 0x4d9d20 _ZN7Structs13TriggerObjectD2Ev
TriggerObject::~TriggerObject() {
    if (m_w3 != nullptr) CustomFree(m_w3);  // +0xc
}

// 0x4d9d04 _ZN7Structs13TriggerObjectD0Ev
void DeletingDestructor(TriggerObject* self) {
    self->TriggerObject::~TriggerObject();
    CustomFree(self);
}

// 0x4d9c9c _ZN7Structs13TriggerObject8finalizeEv
void TriggerObject::finalize() {
    if (m_w3 != nullptr) {
        CustomFree(m_w3);
        m_w2 = 0;
        m_w3 = nullptr;
    }
}

// 0x4fd448 _ZN7Structs13TriggerObject4readEP11IStreamBase
void TriggerObject::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::uint32_t>(stream);  // +0x8
    ReadBuffer(stream, m_w3, m_w2);  // +0xc, length +0x8
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4d9d88 _ZN7Structs9TimerTrapD1Ev
// 0x4d9de4 _ZN7Structs9TimerTrapD2Ev
TimerTrap::~TimerTrap() {
    if (m_w5 != nullptr) CustomFree(m_w5);  // +0x14
}

// 0x4d9dc8 _ZN7Structs9TimerTrapD0Ev
void DeletingDestructor(TimerTrap* self) {
    self->TimerTrap::~TimerTrap();
    CustomFree(self);
}

// 0x4d9d60 _ZN7Structs9TimerTrap8finalizeEv
void TimerTrap::finalize() {
    if (m_w5 != nullptr) {
        CustomFree(m_w5);
        m_w4 = 0;
        m_w5 = nullptr;
    }
}

// 0x4fd618 _ZN7Structs9TimerTrap4readEP11IStreamBase
void TimerTrap::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::uint32_t>(stream);  // +0x10
    ReadBuffer(stream, m_w5, m_w4);  // +0x14, length +0x10
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
}

// 0x4d9e4c _ZN7Structs14ProjectileTrapD1Ev
// 0x4d9ea8 _ZN7Structs14ProjectileTrapD2Ev
ProjectileTrap::~ProjectileTrap() {
    if (m_w7 != nullptr) CustomFree(m_w7);  // +0x1c
}

// 0x4d9e8c _ZN7Structs14ProjectileTrapD0Ev
void DeletingDestructor(ProjectileTrap* self) {
    self->ProjectileTrap::~ProjectileTrap();
    CustomFree(self);
}

// 0x4d9e24 _ZN7Structs14ProjectileTrap8finalizeEv
void ProjectileTrap::finalize() {
    if (m_w7 != nullptr) {
        CustomFree(m_w7);
        m_w6 = 0;
        m_w7 = nullptr;
    }
}

// 0x4fd8a0 _ZN7Structs14ProjectileTrap4readEP11IStreamBase
void ProjectileTrap::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<bool>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::uint32_t>(stream);  // +0x18
    ReadBuffer(stream, m_w7, m_w6);  // +0x1c, length +0x18
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
}

// 0x4d9f10 _ZN7Structs17OpenableContainerD1Ev
// 0x4d9f6c _ZN7Structs17OpenableContainerD2Ev
OpenableContainer::~OpenableContainer() {
    if (m_w6 != nullptr) CustomFree(m_w6);  // +0x18
}

// 0x4d9f50 _ZN7Structs17OpenableContainerD0Ev
void DeletingDestructor(OpenableContainer* self) {
    self->OpenableContainer::~OpenableContainer();
    CustomFree(self);
}

// 0x4d9ee8 _ZN7Structs17OpenableContainer8finalizeEv
void OpenableContainer::finalize() {
    if (m_w6 != nullptr) {
        CustomFree(m_w6);
        m_w5 = 0;
        m_w6 = nullptr;
    }
}

// 0x4fdb90 _ZN7Structs17OpenableContainer4readEP11IStreamBase
void OpenableContainer::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<bool>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::uint32_t>(stream);  // +0x14
    ReadBuffer(stream, m_w6, m_w5);  // +0x18, length +0x14
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
}

// 0x4d9fd4 _ZN7Structs13ExplosiveTrapD1Ev
// 0x4da030 _ZN7Structs13ExplosiveTrapD2Ev
ExplosiveTrap::~ExplosiveTrap() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4da014 _ZN7Structs13ExplosiveTrapD0Ev
void DeletingDestructor(ExplosiveTrap* self) {
    self->ExplosiveTrap::~ExplosiveTrap();
    CustomFree(self);
}

// 0x4d9fac _ZN7Structs13ExplosiveTrap8finalizeEv
void ExplosiveTrap::finalize() {
    if (m_w4 != nullptr) {
        CustomFree(m_w4);
        m_w3 = 0;
        m_w4 = nullptr;
    }
}

// 0x4fde80 _ZN7Structs13ExplosiveTrap4readEP11IStreamBase
void ExplosiveTrap::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    ReadBuffer(stream, m_w4, m_w3);  // +0x10, length +0xc
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4da098 _ZN7Structs4DoorD1Ev
// 0x4da0f4 _ZN7Structs4DoorD2Ev
Door::~Door() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4da0d8 _ZN7Structs4DoorD0Ev
void DeletingDestructor(Door* self) {
    self->Door::~Door();
    CustomFree(self);
}

// 0x4da070 _ZN7Structs4Door8finalizeEv
void Door::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4fe050 _ZN7Structs4Door4readEP11IStreamBase
void Door::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadBuffer(stream, m_w2, m_w1);  // +0x8, length +0x4
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4da15c _ZN7Structs21DestructibleContainerD1Ev
// 0x4da1b8 _ZN7Structs21DestructibleContainerD2Ev
DestructibleContainer::~DestructibleContainer() {
    if (m_w12 != nullptr) CustomFree(m_w12);  // +0x30
}

// 0x4da19c _ZN7Structs21DestructibleContainerD0Ev
void DeletingDestructor(DestructibleContainer* self) {
    self->DestructibleContainer::~DestructibleContainer();
    CustomFree(self);
}

// 0x4da134 _ZN7Structs21DestructibleContainer8finalizeEv
void DestructibleContainer::finalize() {
    if (m_w12 != nullptr) {
        CustomFree(m_w12);
        m_w11 = 0;
        m_w12 = nullptr;
    }
}

// 0x4fe220 _ZN7Structs21DestructibleContainer4readEP11IStreamBase
void DestructibleContainer::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<bool>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
    m_w11 = StreamReader::readAs<std::uint32_t>(stream);  // +0x2c
    ReadBuffer(stream, m_w12, m_w11);  // +0x30, length +0x2c
    m_w13 = StreamReader::readAs<std::int32_t>(stream);  // +0x34
    m_w14 = StreamReader::readAs<std::int32_t>(stream);  // +0x38
    m_w15 = StreamReader::readAs<std::int32_t>(stream);  // +0x3c
    m_w16 = StreamReader::readAs<std::int32_t>(stream);  // +0x40
}

// 0x4da220 _ZN7Structs14FaerySpellListD1Ev
// 0x4da27c _ZN7Structs14FaerySpellListD2Ev
FaerySpellList::~FaerySpellList() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4da260 _ZN7Structs14FaerySpellListD0Ev
void DeletingDestructor(FaerySpellList* self) {
    self->FaerySpellList::~FaerySpellList();
    CustomFree(self);
}

// 0x4da1f8 _ZN7Structs14FaerySpellList8finalizeEv
void FaerySpellList::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4eaa78 _ZN7Structs14FaerySpellList4readEP11IStreamBase
void FaerySpellList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadWords(stream, m_w2, m_w1);  // +0x8
}

// 0x4da4a4 _ZN7Structs14FootstepEffectD1Ev
// 0x4da520 _ZN7Structs14FootstepEffectD2Ev
FootstepEffect::~FootstepEffect() {
    if (m_w3 != nullptr) CustomFree(m_w3);  // +0xc
    if (m_w5 != nullptr) CustomFree(m_w5);  // +0x14
    if (m_w7 != nullptr) CustomFree(m_w7);  // +0x1c
}

// 0x4da504 _ZN7Structs14FootstepEffectD0Ev
void DeletingDestructor(FootstepEffect* self) {
    self->FootstepEffect::~FootstepEffect();
    CustomFree(self);
}

// 0x4da444 _ZN7Structs14FootstepEffect8finalizeEv
void FootstepEffect::finalize() {
    if (m_w3 != nullptr) {
        CustomFree(m_w3);
        m_w2 = 0;
        m_w3 = nullptr;
    }
    if (m_w5 != nullptr) {
        CustomFree(m_w5);
        m_w4 = 0;
        m_w5 = nullptr;
    }
    if (m_w7 != nullptr) {
        CustomFree(m_w7);
        m_w6 = 0;
        m_w7 = nullptr;
    }
}

// 0x506660 _ZN7Structs14FootstepEffect4readEP11IStreamBase
void FootstepEffect::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::uint32_t>(stream);  // +0x8
    ReadBuffer(stream, m_w3, m_w2);  // +0xc, length +0x8
    m_w4 = StreamReader::readAs<std::uint32_t>(stream);  // +0x10
    ReadWords(stream, m_w5, m_w4);  // +0x14
    m_w6 = StreamReader::readAs<std::uint32_t>(stream);  // +0x18
    ReadWords(stream, m_w7, m_w6);  // +0x1c
}

// 0x4da72c _ZN7Structs6AnimFXD1Ev
// 0x4da788 _ZN7Structs6AnimFXD2Ev
AnimFX::~AnimFX() {
    if (m_w11 != nullptr) CustomFree(m_w11);  // +0x2c
}

// 0x4da76c _ZN7Structs6AnimFXD0Ev
void DeletingDestructor(AnimFX* self) {
    self->AnimFX::~AnimFX();
    CustomFree(self);
}

// 0x4da704 _ZN7Structs6AnimFX8finalizeEv
void AnimFX::finalize() {
    if (m_w11 != nullptr) {
        CustomFree(m_w11);
        m_w10 = 0;
        m_w11 = nullptr;
    }
}

// 0x506990 _ZN7Structs6AnimFX4readEP11IStreamBase
void AnimFX::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<bool>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
    m_w4 = StreamReader::readAs<bool>(stream);  // +0x10
    m_off17 = StreamReader::readAs<bool>(stream);  // +0x11
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<bool>(stream);  // +0x20
    m_off33 = StreamReader::readAs<bool>(stream);  // +0x21
    m_w9 = StreamReader::readAs<float>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::uint32_t>(stream);  // +0x28
    ReadBuffer(stream, m_w11, m_w10);  // +0x2c, length +0x28
}

// 0x4da5e8 _ZN7Structs9AnimFXTplD1Ev
// 0x4da684 _ZN7Structs9AnimFXTplD2Ev
AnimFXTpl::~AnimFXTpl() {
    if (m_w4 != nullptr) DestroyElements(m_w4, sizeof(AnimFX));  // +0x10, listing stride 48
}

// 0x4da668 _ZN7Structs9AnimFXTplD0Ev
void DeletingDestructor(AnimFXTpl* self) {
    self->AnimFXTpl::~AnimFXTpl();
    CustomFree(self);
}

// 0x4da580 _ZN7Structs9AnimFXTpl8finalizeEv
void AnimFXTpl::finalize() {
    if (m_w4 != nullptr) {
        DestroyElements(m_w4, sizeof(AnimFX));
        m_w3 = 0;
        m_w4 = nullptr;
    }
}

// 0x4ed73c _ZN7Structs9AnimFXTpl4readEP11IStreamBase
// The decompiler's readAs calls for m_w1 carry a third argument that is a register residue, not a
// readAs parameter; it is dropped here.
void AnimFXTpl::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<bool>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    BuildAndReadTable<AnimFX>(stream, m_w3, m_w4, sizeof(AnimFX), offsetof(AnimFX, m_w11));  // +0x10, zeroes element +0x2c (m_w11)
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
}

// 0x4da830 _ZN7Structs14DialogStepListD1Ev
// 0x4da8cc _ZN7Structs14DialogStepListD2Ev
DialogStepList::~DialogStepList() {
    if (m_w2 != nullptr) DestroyElements(m_w2, sizeof(DialogStep));  // +0x8, listing stride 20
}

// 0x4da8b0 _ZN7Structs14DialogStepListD0Ev
void DeletingDestructor(DialogStepList* self) {
    self->DialogStepList::~DialogStepList();
    CustomFree(self);
}

// 0x4da7c8 _ZN7Structs14DialogStepList8finalizeEv
void DialogStepList::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, sizeof(DialogStep));
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4dca68 _ZN7Structs14DialogStepList4readEP11IStreamBase
void DialogStepList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    BuildAndReadTable<DialogStep>(stream, m_w1, m_w2, sizeof(DialogStep), kNoZeroWord);  // +0x8
}

// 0x4da9b0 _ZN7Structs17ConditionalDialogD1Ev
// 0x4daa48 _ZN7Structs17ConditionalDialogD2Ev
ConditionalDialog::~ConditionalDialog() {
    if (m_w2 != nullptr) DestroyElements(m_w2, sizeof(DialogCondition));  // +0x8, listing stride 16
}

// 0x4daa2c _ZN7Structs17ConditionalDialogD0Ev
void DeletingDestructor(ConditionalDialog* self) {
    self->ConditionalDialog::~ConditionalDialog();
    CustomFree(self);
}

// 0x4da94c _ZN7Structs17ConditionalDialog8finalizeEv
void ConditionalDialog::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, sizeof(DialogCondition));
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4dcbd4 _ZN7Structs17ConditionalDialog4readEP11IStreamBase
void ConditionalDialog::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    BuildAndReadTable<DialogCondition>(stream, m_w1, m_w2, sizeof(DialogCondition), offsetof(DialogCondition, m_w3));  // +0x8, zeroes element +0xc (m_w3)
}

// 0x4daaec _ZN7Structs15DialogConditionD1Ev
// 0x4dab48 _ZN7Structs15DialogConditionD2Ev
DialogCondition::~DialogCondition() {
    if (m_w3 != nullptr) CustomFree(m_w3);  // +0xc
}

// 0x4dab2c _ZN7Structs15DialogConditionD0Ev
void DeletingDestructor(DialogCondition* self) {
    self->DialogCondition::~DialogCondition();
    CustomFree(self);
}

// 0x4daac4 _ZN7Structs15DialogCondition8finalizeEv
void DialogCondition::finalize() {
    if (m_w3 != nullptr) {
        CustomFree(m_w3);
        m_w2 = 0;
        m_w3 = nullptr;
    }
}

// 0x506cb0 _ZN7Structs15DialogCondition4readEP11IStreamBase
void DialogCondition::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::uint32_t>(stream);  // +0x8
    ReadBuffer(stream, m_w3, m_w2);  // +0xc, length +0x8
}

// 0x4dabb0 _ZN7Structs11DialogActorD1Ev
// 0x4dac0c _ZN7Structs11DialogActorD2Ev
DialogActor::~DialogActor() {
    if (m_w3 != nullptr) CustomFree(m_w3);  // +0xc
}

// 0x4dabf0 _ZN7Structs11DialogActorD0Ev
void DeletingDestructor(DialogActor* self) {
    self->DialogActor::~DialogActor();
    CustomFree(self);
}

// 0x4dab88 _ZN7Structs11DialogActor8finalizeEv
void DialogActor::finalize() {
    if (m_w3 != nullptr) {
        CustomFree(m_w3);
        m_w2 = 0;
        m_w3 = nullptr;
    }
}

// 0x506dc8 _ZN7Structs11DialogActor4readEP11IStreamBase
void DialogActor::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::uint32_t>(stream);  // +0x8
    ReadBuffer(stream, m_w3, m_w2);  // +0xc, length +0x8
    m_w4 = StreamReader::readAs<std::int32_t>(stream);  // +0x10
}

// 0x4dac74 _ZN7Structs11ColladaFileD1Ev
// 0x4dacd0 _ZN7Structs11ColladaFileD2Ev
ColladaFile::~ColladaFile() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4dacb4 _ZN7Structs11ColladaFileD0Ev
void DeletingDestructor(ColladaFile* self) {
    self->ColladaFile::~ColladaFile();
    CustomFree(self);
}

// 0x4dac4c _ZN7Structs11ColladaFile8finalizeEv
void ColladaFile::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4dc424 _ZN7Structs11ColladaFile4readEP11IStreamBase
// partial: the original returns the bool result of readStringEx. The read here is void and the readStringEx
// stub returns void, so that result is not modelled.
void ColladaFile::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadBuffer(stream, m_w2, m_w1);  // +0x8, length +0x4
}

// 0x4dad74 _ZN7Structs12CharTemplateD1Ev
// 0x4dae0c _ZN7Structs12CharTemplateD2Ev
CharTemplate::~CharTemplate() {
    if (m_w2 != nullptr) DestroyElements(m_w2, 8);  // +0x8, stride 8
}

// 0x4dadf0 _ZN7Structs12CharTemplateD0Ev
void DeletingDestructor(CharTemplate* self) {
    self->CharTemplate::~CharTemplate();
    CustomFree(self);
}

// 0x4dad10 _ZN7Structs12CharTemplate8finalizeEv
void CharTemplate::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, 8);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4daeb0 _ZN7Structs12StatListListD1Ev
// 0x4daf0c _ZN7Structs12StatListListD2Ev
StatListList::~StatListList() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4daef0 _ZN7Structs12StatListListD0Ev
void DeletingDestructor(StatListList* self) {
    self->StatListList::~StatListList();
    CustomFree(self);
}

// 0x4dae88 _ZN7Structs12StatListList8finalizeEv
void StatListList::finalize() {
    if (m_w2 != nullptr) {
        CustomFree(m_w2);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4eacc0 _ZN7Structs12StatListList4readEP11IStreamBase
void StatListList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadWords(stream, m_w2, m_w1);  // +0x8
}

// 0x4dafb0 _ZN7Structs24StatAutoAssignTargetListD1Ev
// 0x4db048 _ZN7Structs24StatAutoAssignTargetListD2Ev
StatAutoAssignTargetList::~StatAutoAssignTargetList() {
    if (m_w2 != nullptr) DestroyElements(m_w2, sizeof(StatAutoAssignTarget));  // +0x8, listing stride 16
}

// 0x4db02c _ZN7Structs24StatAutoAssignTargetListD0Ev
void DeletingDestructor(StatAutoAssignTargetList* self) {
    self->StatAutoAssignTargetList::~StatAutoAssignTargetList();
    CustomFree(self);
}

// 0x4daf4c _ZN7Structs24StatAutoAssignTargetList8finalizeEv
void StatAutoAssignTargetList::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, sizeof(StatAutoAssignTarget));
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4dce90 _ZN7Structs24StatAutoAssignTargetList4readEP11IStreamBase
// Element stride is sizeof(StatAutoAssignTarget) (listing stride 16). StatAutoAssignTarget::read is defined in
// structs_records.cpp.
void StatAutoAssignTargetList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    BuildAndReadTable<StatAutoAssignTarget>(stream, m_w1, m_w2, sizeof(StatAutoAssignTarget), kNoZeroWord);  // +0x8
}

// 0x4db12c _ZN7Structs13ClassFuncListD1Ev
// 0x4db1c8 _ZN7Structs13ClassFuncListD2Ev
ClassFuncList::~ClassFuncList() {
    if (m_w2 != nullptr) DestroyElements(m_w2, 24);  // +0x8, stride 24
}

// 0x4db1ac _ZN7Structs13ClassFuncListD0Ev
void DeletingDestructor(ClassFuncList* self) {
    self->ClassFuncList::~ClassFuncList();
    CustomFree(self);
}

// 0x4db0c4 _ZN7Structs13ClassFuncList8finalizeEv
void ClassFuncList::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, 24);
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4db474 _ZN7Structs7AnimTplD1Ev
// 0x4db510 _ZN7Structs7AnimTplD2Ev
AnimTpl::~AnimTpl() {
    if (m_w3 != nullptr) DestroyElements(m_w3, 56);  // +0xc, stride 56
}

// 0x4db4f4 _ZN7Structs7AnimTplD0Ev
void DeletingDestructor(AnimTpl* self) {
    self->AnimTpl::~AnimTpl();
    CustomFree(self);
}

// 0x4db40c _ZN7Structs7AnimTpl8finalizeEv
void AnimTpl::finalize() {
    if (m_w3 != nullptr) {
        DestroyElements(m_w3, 56);
        m_w2 = 0;
        m_w3 = nullptr;
    }
}

// 0x4db6bc _ZN7Structs10AIFactionsD1Ev
// 0x4db758 _ZN7Structs10AIFactionsD2Ev
AIFactions::~AIFactions() {
    if (m_w2 != nullptr) DestroyElements(m_w2, sizeof(AIFaction));  // +0x8, listing stride 12
}

// 0x4db73c _ZN7Structs10AIFactionsD0Ev
void DeletingDestructor(AIFactions* self) {
    self->AIFactions::~AIFactions();
    CustomFree(self);
}

// 0x4db654 _ZN7Structs10AIFactions8finalizeEv
void AIFactions::finalize() {
    if (m_w2 != nullptr) {
        DestroyElements(m_w2, sizeof(AIFaction));
        m_w1 = 0;
        m_w2 = nullptr;
    }
}

// 0x4dd158 _ZN7Structs10AIFactions4readEP11IStreamBase
void AIFactions::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    BuildAndReadTable<AIFaction>(stream, m_w1, m_w2, sizeof(AIFaction), kNoZeroWord);  // +0x8
}

}  // namespace Structs
