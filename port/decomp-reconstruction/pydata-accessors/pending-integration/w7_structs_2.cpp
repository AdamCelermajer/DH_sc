// Structs::X destructors, finalize and read for slice structs_2 (pydata-accessors wave 7).
// Bodies follow the Hex-Rays pseudocode (tools/decomp_extract.py --address). Each function carries its
// address and mangled symbol. Declarations are in include/pydata-accessors/w7_structs_2.hpp.
//
// Element arrays: a pointer field points at element data. The 8 bytes before it hold the stride and the
// count (the listing's new[](stride * count + 8) block). An element is a pydata::ArrayElement: its vptr
// is the first word, slot 0 is the destructor, slot 2 finalize, slot 3 read. The stride is the listing's,
// widened to sizeof(element) when the host object is larger (host pointers are 8 bytes).
//
// Omitted bodies (see the ledger notes): ItemBase::read (0x4ec47c) is not in the port, so the read bodies
// of the ItemBase-derived records skip its call. The read of v2CondAnd (0x505970) and the thunk of
// v2QuestAutomatic::read (0x504a90) need v2QuestObjectiveStub::read or the element's read slot, which the
// port does not declare. PlayEffect and ExecScript are declared in script_cmd.hpp without these methods.
#include "pydata-accessors/w7_structs_2.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace Structs {
namespace {

pydata::ArrayElement* ElementAt(std::uint8_t* p) {
    return reinterpret_cast<pydata::ArrayElement*>(p);
}

// Allocates count elements of the given stride (the listing's new[](stride * count + 8, 1)). The block is
// zeroed, the header words are written, and the returned pointer is block + 8.
std::uint8_t* AllocElements(std::size_t stride, std::uint32_t count) {
    const std::size_t total = stride * count + 8;
    auto* block = static_cast<std::uint8_t*>(MemoryHintState(total, 1));
    std::memset(block, 0, total);
    const std::uint32_t header_stride = static_cast<std::uint32_t>(stride);
    std::memcpy(block, &header_stride, sizeof header_stride);
    std::memcpy(block + 4, &count, sizeof count);
    return block + 8;
}

// The listing's loop: from the last element back to the first, each element's slot 0 (D1), then the
// header block (data - 8) is freed. Does nothing for a null pointer.
void DestroyElements(void* data) {
    if (data == nullptr) return;
    auto* first = static_cast<std::uint8_t*>(data);
    std::uint32_t stride = 0;
    std::uint32_t count = 0;
    std::memcpy(&stride, first - 8, sizeof stride);
    std::memcpy(&count, first - 4, sizeof count);
    for (std::uint32_t i = count; i > 0; --i) {
        ElementAt(first + static_cast<std::size_t>(stride) * (i - 1))->~ArrayElement();
    }
    CustomFree(first - 8);
}

// The vptr of Elem: the first word of a polymorphic object (Itanium ABI).
template <class Elem>
const void* VtableOf() {
    Elem tmp;
    const void* vptr = nullptr;
    std::memcpy(&vptr, &tmp, sizeof vptr);
    return vptr;
}

template <class Elem>
std::size_t ElementStride(std::size_t listing) {
    return sizeof(Elem) > listing ? sizeof(Elem) : listing;
}

// Allocates count elements of Elem, sets each element's vptr, then calls each element's read slot (+0xc)
// in order, as the listing's read bodies do.
template <class Elem>
void* ReadElementArray(IStreamBase* stream, std::uint32_t count, std::size_t listing_stride) {
    const std::size_t stride = ElementStride<Elem>(listing_stride);
    std::uint8_t* data = AllocElements(stride, count);
    const void* vptr = VtableOf<Elem>();
    for (std::uint32_t i = 0; i < count; ++i) {
        std::memcpy(data + stride * i, &vptr, sizeof vptr);
    }
    for (std::uint32_t i = 0; i < count; ++i) {
        ElementAt(data + stride * i)->read(stream);
    }
    return data;
}

// new[](4 * count, 1) of int32 words, each read with readAs<int> in order.
void* ReadInt32Array(IStreamBase* stream, std::uint32_t count) {
    auto* data = static_cast<std::int32_t*>(MemoryHintState(std::size_t{4} * count, 1));
    for (std::uint32_t i = 0; i < count; ++i) {
        data[i] = StreamReader::readAs<std::int32_t>(stream);
    }
    return data;
}

// The string read of the listing: free the old buffer if any (not zeroed), allocate len + 1, read len
// bytes with hint 0, then store the terminating zero at buf[len].
void ReadStringField(IStreamBase* stream, void*& slot, std::uint32_t len) {
    if (slot != nullptr) CustomFree(slot);
    auto* buf = static_cast<char*>(MemoryHintState(std::size_t{len} + 1, 1));
    slot = buf;
    StreamReader::readStringEx(stream, buf, len, 0);
    buf[len] = 0;
}

// Shared bodies of the ItemBase-derived records (WeaponRef, RangedWeaponRef, ShieldRef, JewelryRef,
// ArmorRef, Valuable, Potion, GoldItem, Item). All of them use m_w1, m_w2 (ItemBase), m_w19, m_w20.

// finalize, before ItemBase::finalize: frees m_w2 and m_w20 and zeroes the words the listing zeroes.
template <class T>
void FinalizeItemRefWords(T& self) {
    if (self.m_w2 != nullptr) {  // +0x8
        CustomFree(self.m_w2);
        self.m_w1 = 0;  // +0x4
        self.m_w2 = nullptr;
    }
    if (self.m_w20 != nullptr) {  // +0x50
        CustomFree(self.m_w20);
        self.m_w19 = 0;  // +0x4c
        self.m_w20 = nullptr;
    }
}

// destructor body: frees m_w2 and m_w20 (no zeroing), the base destructor runs after it.
template <class T>
void ReleaseItemRefWords(T& self) {
    if (self.m_w2 != nullptr) CustomFree(self.m_w2);  // +0x8
    if (self.m_w20 != nullptr) CustomFree(self.m_w20);  // +0x50
}

// read body of the family, without the ItemBase::read call (not in the port).
template <class T>
void ReadItemRefWords(T& self, IStreamBase* stream) {
    self.m_w17 = StreamReader::readAs<std::int32_t>(stream);  // +0x44
    self.m_w18 = StreamReader::readAs<std::int32_t>(stream);  // +0x48
    self.m_w19 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4c
    ReadStringField(stream, self.m_w20, self.m_w19);  // +0x50
    self.m_w21 = StreamReader::readAs<std::int32_t>(stream);  // +0x54
    self.m_w22 = StreamReader::readAs<std::int32_t>(stream);  // +0x58
    self.m_w23 = StreamReader::readAs<std::int32_t>(stream);  // +0x5c
    self.m_w24 = StreamReader::readAs<std::int32_t>(stream);  // +0x60
    self.m_w25 = StreamReader::readAs<std::int32_t>(stream);  // +0x64
    self.m_w26 = StreamReader::readAs<std::int32_t>(stream);  // +0x68
    self.m_w27 = StreamReader::readAs<std::int32_t>(stream);  // +0x6c
    self.m_w28 = StreamReader::readAs<std::int32_t>(stream);  // +0x70
    self.m_w29 = StreamReader::readAs<std::int32_t>(stream);  // +0x74
    self.m_w30 = StreamReader::readAs<std::int32_t>(stream);  // +0x78
    self.m_w31 = StreamReader::readAs<std::int32_t>(stream);  // +0x7c
    self.m_w32 = StreamReader::readAs<std::int32_t>(stream);  // +0x80
    self.m_w33 = StreamReader::readAs<std::int32_t>(stream);  // +0x84
    self.m_w34 = StreamReader::readAs<std::int32_t>(stream);  // +0x88
    self.m_w35 = StreamReader::readAs<std::int32_t>(stream);  // +0x8c
    self.m_w36 = StreamReader::readAs<std::int32_t>(stream);  // +0x90
    self.m_w37 = StreamReader::readAs<std::int32_t>(stream);  // +0x94
    self.m_w38 = StreamReader::readAs<std::int32_t>(stream);  // +0x98
    self.m_w39 = StreamReader::readAs<std::int32_t>(stream);  // +0x9c
    self.m_w40 = StreamReader::readAs<std::int32_t>(stream);  // +0xa0
}

}  // namespace

// ---------------------------------------------------------------- v2QuestAutomatic (base v2QuestObjectiveStub)

// 0x4d0054 _ZN7Structs16v2QuestAutomatic8finalizeEv
void v2QuestAutomatic::finalize() {
    if (m_w5 != nullptr) {  // +0x14
        CustomFree(m_w5);
        m_w4 = 0;  // +0x10
        m_w5 = nullptr;
    }
    if (m_w7 != nullptr) {  // +0x1c
        CustomFree(m_w7);
        m_w6 = 0;  // +0x18
        m_w7 = nullptr;
    }
    v2QuestObjectiveStub::finalize();
}

// 0x4d01e0 _ZN7Structs16v2QuestAutomaticD2Ev
// 0x4d016c _ZN7Structs16v2QuestAutomaticD1Ev
// The base destructor (implicit, runs after this body) frees m_w5 and m_w7 again, as the listing does.
v2QuestAutomatic::~v2QuestAutomatic() {
    if (m_w5 != nullptr) CustomFree(m_w5);  // +0x14
    if (m_w7 != nullptr) CustomFree(m_w7);  // +0x1c
}

// 0x4d01c4 _ZN7Structs16v2QuestAutomaticD0Ev
void DeletingDestructor(v2QuestAutomatic* self) {
    self->v2QuestAutomatic::~v2QuestAutomatic();
    CustomFree(self);
}

// ---------------------------------------------------------------- v2Quest (no base in the listing)

// 0x4d06c0 _ZN7Structs7v2QuestD2Ev
// 0x4d04dc _ZN7Structs7v2QuestD1Ev
// The listing calls the three subobject destructors (+0xa0, +0x68, +0x3c) after the array frees. The port
// declares them as members in that order, so the compiler destroys them in the same order after this body.
v2Quest::~v2Quest() {
    DestroyElements(m_w6);   // +0x18, stride 16
    DestroyElements(m_w8);   // +0x20, stride 44
    DestroyElements(m_w10);  // +0x28, stride 16
    DestroyElements(m_w12);  // +0x30, stride 16
    DestroyElements(m_w14);  // +0x38, stride 16
}

// 0x4d06a4 _ZN7Structs7v2QuestD0Ev
void DeletingDestructor(v2Quest* self) {
    self->v2Quest::~v2Quest();
    CustomFree(self);
}

// ---------------------------------------------------------------- v2CondAnd (no base)

// 0x4d09b4 _ZN7Structs9v2CondAnd8finalizeEv
void v2CondAnd::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        DestroyElements(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4d0ab0 _ZN7Structs9v2CondAndD2Ev
// 0x4d0a18 _ZN7Structs9v2CondAndD1Ev
v2CondAnd::~v2CondAnd() {
    DestroyElements(m_w2);  // +0x8, v2ConditionStub elements, stride 16
}

// 0x4d0a94 _ZN7Structs9v2CondAndD0Ev
void DeletingDestructor(v2CondAnd* self) {
    self->v2CondAnd::~v2CondAnd();
    CustomFree(self);
}

// ---------------------------------------------------------------- SpawnGroup (no base)

// 0x4d0b2c _ZN7Structs10SpawnGroup8finalizeEv
void SpawnGroup::finalize() {
    if (m_w4 != nullptr) {  // +0x10
        DestroyElements(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
}

// 0x4eb244 _ZN7Structs10SpawnGroup4readEP11IStreamBase
void SpawnGroup::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<bool>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    if (m_w4 != nullptr) DestroyElements(m_w4);  // +0x10
    m_w4 = ReadElementArray<SpawnInfo>(stream, m_w3, 16);
}

// 0x4d0c28 _ZN7Structs10SpawnGroupD2Ev
// 0x4d0b90 _ZN7Structs10SpawnGroupD1Ev
SpawnGroup::~SpawnGroup() {
    DestroyElements(m_w4);  // +0x10, SpawnInfo elements, stride 16
}

// 0x4d0c0c _ZN7Structs10SpawnGroupD0Ev
void DeletingDestructor(SpawnGroup* self) {
    self->SpawnGroup::~SpawnGroup();
    CustomFree(self);
}

// ---------------------------------------------------------------- CharSounds (no base)

// 0x4d0ca4 _ZN7Structs10CharSounds8finalizeEv
void CharSounds::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        CustomFree(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
    if (m_w4 != nullptr) {  // +0x10
        CustomFree(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
    if (m_w6 != nullptr) {  // +0x18
        CustomFree(m_w6);
        m_w5 = 0;  // +0x14
        m_w6 = nullptr;
    }
    if (m_w8 != nullptr) {  // +0x20
        CustomFree(m_w8);
        m_w7 = 0;  // +0x1c
        m_w8 = nullptr;
    }
}

// 0x4eade4 _ZN7Structs10CharSounds4readEP11IStreamBase
void CharSounds::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
    m_w2 = ReadInt32Array(stream, m_w1);
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
    m_w4 = ReadInt32Array(stream, m_w3);
    m_w5 = StreamReader::readAs<std::uint32_t>(stream);  // +0x14
    if (m_w6 != nullptr) CustomFree(m_w6);  // +0x18
    m_w6 = ReadInt32Array(stream, m_w5);
    m_w7 = StreamReader::readAs<std::uint32_t>(stream);  // +0x1c
    if (m_w8 != nullptr) CustomFree(m_w8);  // +0x20
    m_w8 = ReadInt32Array(stream, m_w7);
    m_w9 = StreamReader::readAs<bool>(stream);  // +0x24
    m_off37 = StreamReader::readAs<bool>(stream);  // +0x25
}

// 0x4d0dac _ZN7Structs10CharSoundsD2Ev
// 0x4d0d20 _ZN7Structs10CharSoundsD1Ev
CharSounds::~CharSounds() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
    if (m_w6 != nullptr) CustomFree(m_w6);  // +0x18
    if (m_w8 != nullptr) CustomFree(m_w8);  // +0x20
}

// 0x4d0d90 _ZN7Structs10CharSoundsD0Ev
void DeletingDestructor(CharSounds* self) {
    self->CharSounds::~CharSounds();
    CustomFree(self);
}

// ---------------------------------------------------------------- Sound (no base)

// 0x4d0e1c _ZN7Structs5Sound8finalizeEv
void Sound::finalize() {
    if (m_w4 != nullptr) {  // +0x10
        CustomFree(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
}

// 0x4eb89c _ZN7Structs5Sound4readEP11IStreamBase
void Sound::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    ReadStringField(stream, m_w4, m_w3);  // +0x10
    m_w5 = StreamReader::readAs<std::int32_t>(stream);  // +0x14
    m_w6 = StreamReader::readAs<std::int32_t>(stream);  // +0x18
    m_w7 = StreamReader::readAs<std::int32_t>(stream);  // +0x1c
    m_w8 = StreamReader::readAs<std::int32_t>(stream);  // +0x20
    m_w9 = StreamReader::readAs<std::int32_t>(stream);  // +0x24
    m_w10 = StreamReader::readAs<std::int32_t>(stream);  // +0x28
}

// 0x4d0ea0 _ZN7Structs5SoundD2Ev
// 0x4d0e44 _ZN7Structs5SoundD1Ev
Sound::~Sound() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4d0e84 _ZN7Structs5SoundD0Ev
void DeletingDestructor(Sound* self) {
    self->Sound::~Sound();
    CustomFree(self);
}

// ---------------------------------------------------------------- SoundGroup (no base)

// 0x4d0ee0 _ZN7Structs10SoundGroup8finalizeEv
void SoundGroup::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        CustomFree(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4eb784 _ZN7Structs10SoundGroup4readEP11IStreamBase
void SoundGroup::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadStringField(stream, m_w2, m_w1);  // +0x8
    m_w3 = StreamReader::readAs<std::int32_t>(stream);  // +0xc
}

// 0x4d0f64 _ZN7Structs10SoundGroupD2Ev
// 0x4d0f08 _ZN7Structs10SoundGroupD1Ev
SoundGroup::~SoundGroup() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4d0f48 _ZN7Structs10SoundGroupD0Ev
void DeletingDestructor(SoundGroup* self) {
    self->SoundGroup::~SoundGroup();
    CustomFree(self);
}

// ---------------------------------------------------------------- HideHUD (base HideFlash)

// 0x4d30ec _ZN7Structs7HideHUD8finalizeEv
void HideHUD::finalize() {
    if (m_w4 != nullptr) {  // +0x10, HideFlash field
        CustomFree(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
    HideFlash::finalize();
}

// 0x5021b4 _ZN7Structs7HideHUD4readEP11IStreamBase
// Forwarder: the listing is a 4-byte thunk to HideFlash::read (b #0x...), which is the whole body.
void HideHUD::read(IStreamBase* stream) {
    HideFlash::read(stream);
}

// 0x4d322c _ZN7Structs7HideHUDD2Ev
// 0x4d31c8 _ZN7Structs7HideHUDD1Ev
// The base destructor (implicit, runs after this body) frees m_w4 again, as the listing does.
HideHUD::~HideHUD() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4d3210 _ZN7Structs7HideHUDD0Ev
void DeletingDestructor(HideHUD* self) {
    self->HideHUD::~HideHUD();
    CustomFree(self);
}

// ---------------------------------------------------------------- ShowHUD (base ShowFlash)

// 0x4d32a4 _ZN7Structs7ShowHUD8finalizeEv
void ShowHUD::finalize() {
    if (m_w4 != nullptr) {  // +0x10, ShowFlash field
        CustomFree(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
    ShowFlash::finalize();
}

// 0x5022e0 _ZN7Structs7ShowHUD4readEP11IStreamBase
// Forwarder: the listing is a 4-byte thunk to ShowFlash::read, which is the whole body.
void ShowHUD::read(IStreamBase* stream) {
    ShowFlash::read(stream);
}

// 0x4d33e4 _ZN7Structs7ShowHUDD2Ev
// 0x4d3380 _ZN7Structs7ShowHUDD1Ev
// The base destructor (implicit, runs after this body) frees m_w4 again, as the listing does.
ShowHUD::~ShowHUD() {
    if (m_w4 != nullptr) CustomFree(m_w4);  // +0x10
}

// 0x4d33c8 _ZN7Structs7ShowHUDD0Ev
void DeletingDestructor(ShowHUD* self) {
    self->ShowHUD::~ShowHUD();
    CustomFree(self);
}

// ---------------------------------------------------------------- SceneFile (no base)

// 0x4d3ab8 _ZN7Structs9SceneFile8finalizeEv
void SceneFile::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        CustomFree(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4dd4ec _ZN7Structs9SceneFile4readEP11IStreamBase
// The listing returns the readStringEx result (a bool); the port's read returns void.
void SceneFile::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    ReadStringField(stream, m_w2, m_w1);  // +0x8
}

// 0x4d3b3c _ZN7Structs9SceneFileD2Ev
// 0x4d3ae0 _ZN7Structs9SceneFileD1Ev
SceneFile::~SceneFile() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4d3b20 _ZN7Structs9SceneFileD0Ev
void DeletingDestructor(SceneFile* self) {
    self->SceneFile::~SceneFile();
    CustomFree(self);
}

// ---------------------------------------------------------------- ItemTypeListList (no base)

// 0x4d3b7c _ZN7Structs16ItemTypeListList8finalizeEv
void ItemTypeListList::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        CustomFree(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4dd2c4 _ZN7Structs16ItemTypeListList4readEP11IStreamBase
void ItemTypeListList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
    auto* bytes = static_cast<std::int8_t*>(MemoryHintState(m_w1, 1));
    m_w2 = bytes;
    for (std::uint32_t i = 0; i < m_w1; ++i) {
        bytes[i] = StreamReader::readAs<std::int8_t>(stream);
    }
}

// 0x4d3c00 _ZN7Structs16ItemTypeListListD2Ev
// 0x4d3ba4 _ZN7Structs16ItemTypeListListD1Ev
ItemTypeListList::~ItemTypeListList() {
    if (m_w2 != nullptr) CustomFree(m_w2);  // +0x8
}

// 0x4d3be4 _ZN7Structs16ItemTypeListListD0Ev
void DeletingDestructor(ItemTypeListList* self) {
    self->ItemTypeListList::~ItemTypeListList();
    CustomFree(self);
}

// ---------------------------------------------------------------- Merchant (no base)

// 0x4d3c40 _ZN7Structs8Merchant8finalizeEv
void Merchant::finalize() {
    if (m_w4 != nullptr) {  // +0x10
        DestroyElements(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
}

// 0x4ff604 _ZN7Structs8Merchant4readEP11IStreamBase
void Merchant::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::int32_t>(stream);  // +0x4
    m_w2 = StreamReader::readAs<std::int32_t>(stream);  // +0x8
    m_w3 = StreamReader::readAs<std::uint32_t>(stream);  // +0xc
    if (m_w4 != nullptr) DestroyElements(m_w4);  // +0x10
    m_w4 = ReadElementArray<MerchandiseListEntry>(stream, m_w3, 12);
}

// 0x4d3d44 _ZN7Structs8MerchantD2Ev
// 0x4d3ca8 _ZN7Structs8MerchantD1Ev
Merchant::~Merchant() {
    DestroyElements(m_w4);  // +0x10, MerchandiseListEntry elements, stride 12
}

// 0x4d3d28 _ZN7Structs8MerchantD0Ev
void DeletingDestructor(Merchant* self) {
    self->Merchant::~Merchant();
    CustomFree(self);
}

// ---------------------------------------------------------------- TileOffsetList (no base)

// 0x4d3dc4 _ZN7Structs14TileOffsetList8finalizeEv
void TileOffsetList::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        DestroyElements(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4dc4e0 _ZN7Structs14TileOffsetList4readEP11IStreamBase
void TileOffsetList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    if (m_w2 != nullptr) DestroyElements(m_w2);  // +0x8
    m_w2 = ReadElementArray<TileOffset>(stream, m_w1, 8);
}

// 0x4d3ec0 _ZN7Structs14TileOffsetListD2Ev
// 0x4d3e28 _ZN7Structs14TileOffsetListD1Ev
TileOffsetList::~TileOffsetList() {
    DestroyElements(m_w2);  // +0x8, TileOffset elements, stride 8
}

// 0x4d3ea4 _ZN7Structs14TileOffsetListD0Ev
void DeletingDestructor(TileOffsetList* self) {
    self->TileOffsetList::~TileOffsetList();
    CustomFree(self);
}

// ---------------------------------------------------------------- NumProbList (no base)

// 0x4d3f3c _ZN7Structs11NumProbList8finalizeEv
void NumProbList::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        DestroyElements(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4dc638 _ZN7Structs11NumProbList4readEP11IStreamBase
void NumProbList::read(IStreamBase* stream) {
    m_w1 = StreamReader::readAs<std::uint32_t>(stream);  // +0x4
    if (m_w2 != nullptr) DestroyElements(m_w2);  // +0x8
    m_w2 = ReadElementArray<NumProb>(stream, m_w1, 8);
}

// 0x4d4038 _ZN7Structs11NumProbListD2Ev
// 0x4d3fa0 _ZN7Structs11NumProbListD1Ev
NumProbList::~NumProbList() {
    DestroyElements(m_w2);  // +0x8, NumProb elements, stride 8
}

// 0x4d401c _ZN7Structs11NumProbListD0Ev
void DeletingDestructor(NumProbList* self) {
    self->NumProbList::~NumProbList();
    CustomFree(self);
}

// ---------------------------------------------------------------- ItemListEntryList (no base)

// 0x4d40b4 _ZN7Structs17ItemListEntryList8finalizeEv
void ItemListEntryList::finalize() {
    if (m_w2 != nullptr) {  // +0x8
        DestroyElements(m_w2);
        m_w1 = 0;  // +0x4
        m_w2 = nullptr;
    }
}

// 0x4d41b8 _ZN7Structs17ItemListEntryListD2Ev
// 0x4d411c _ZN7Structs17ItemListEntryListD1Ev
ItemListEntryList::~ItemListEntryList() {
    DestroyElements(m_w2);  // +0x8, ItemListEntry elements, stride 12
}

// 0x4d419c _ZN7Structs17ItemListEntryListD0Ev
void DeletingDestructor(ItemListEntryList* self) {
    self->ItemListEntryList::~ItemListEntryList();
    CustomFree(self);
}

// ---------------------------------------------------------------- Loot (no base)

// 0x4d4238 _ZN7Structs4Loot8finalizeEv
void Loot::finalize() {
    if (m_w4 != nullptr) {  // +0x10
        DestroyElements(m_w4);
        m_w3 = 0;  // +0xc
        m_w4 = nullptr;
    }
    if (m_w6 != nullptr) {  // +0x18
        DestroyElements(m_w6);
        m_w5 = 0;  // +0x14
        m_w6 = nullptr;
    }
    if (m_w8 != nullptr) {  // +0x20
        CustomFree(m_w8);
        m_w7 = 0;  // +0x1c
        m_w8 = nullptr;
    }
}

// 0x4d4414 _ZN7Structs4LootD2Ev
// 0x4d4318 _ZN7Structs4LootD1Ev
Loot::~Loot() {
    DestroyElements(m_w4);  // +0x10, stride 36
    DestroyElements(m_w6);  // +0x18, stride 36
    if (m_w8 != nullptr) CustomFree(m_w8);  // +0x20
}

// 0x4d43f8 _ZN7Structs4LootD0Ev
void DeletingDestructor(Loot* self) {
    self->Loot::~Loot();
    CustomFree(self);
}

// ---------------------------------------------------------------- ItemBase family (base ItemBase)

// 0x4d451c _ZN7Structs9WeaponRef8finalizeEv
void WeaponRef::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4f7b18 _ZN7Structs9WeaponRef4readEP11IStreamBase
// Partial: the ItemBase::read call (0x4ec47c, not in the port) is omitted. Words 17..40 are read here.
void WeaponRef::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4a08 _ZN7Structs9WeaponRefD2Ev
// 0x4d4994 _ZN7Structs9WeaponRefD1Ev
// The base destructor (implicit, runs after this body) frees m_w2 again, as the listing does.
WeaponRef::~WeaponRef() {
    ReleaseItemRefWords(*this);
}

// 0x4d49ec _ZN7Structs9WeaponRefD0Ev
void DeletingDestructor(WeaponRef* self) {
    self->WeaponRef::~WeaponRef();
    CustomFree(self);
}

// 0x4d4568 _ZN7Structs15RangedWeaponRef8finalizeEv
void RangedWeaponRef::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4f83c0 _ZN7Structs15RangedWeaponRef4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void RangedWeaponRef::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4ad4 _ZN7Structs15RangedWeaponRefD2Ev
// 0x4d4a60 _ZN7Structs15RangedWeaponRefD1Ev
RangedWeaponRef::~RangedWeaponRef() {
    ReleaseItemRefWords(*this);
}

// 0x4d4ab8 _ZN7Structs15RangedWeaponRefD0Ev
void DeletingDestructor(RangedWeaponRef* self) {
    self->RangedWeaponRef::~RangedWeaponRef();
    CustomFree(self);
}

// 0x4d45b4 _ZN7Structs9ShieldRef8finalizeEv
void ShieldRef::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4f8c68 _ZN7Structs9ShieldRef4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void ShieldRef::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4ba0 _ZN7Structs9ShieldRefD2Ev
// 0x4d4b2c _ZN7Structs9ShieldRefD1Ev
ShieldRef::~ShieldRef() {
    ReleaseItemRefWords(*this);
}

// 0x4d4b84 _ZN7Structs9ShieldRefD0Ev
void DeletingDestructor(ShieldRef* self) {
    self->ShieldRef::~ShieldRef();
    CustomFree(self);
}

// 0x4d4600 _ZN7Structs10JewelryRef8finalizeEv
void JewelryRef::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4f9510 _ZN7Structs10JewelryRef4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void JewelryRef::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4c6c _ZN7Structs10JewelryRefD2Ev
// 0x4d4bf8 _ZN7Structs10JewelryRefD1Ev
JewelryRef::~JewelryRef() {
    ReleaseItemRefWords(*this);
}

// 0x4d4c50 _ZN7Structs10JewelryRefD0Ev
void DeletingDestructor(JewelryRef* self) {
    self->JewelryRef::~JewelryRef();
    CustomFree(self);
}

// 0x4d464c _ZN7Structs8ArmorRef8finalizeEv
void ArmorRef::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4f9db8 _ZN7Structs8ArmorRef4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void ArmorRef::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4698 _ZN7Structs7BeltRef8finalizeEv
void BeltRef::finalize() {
    ArmorRef::finalize();
}

// 0x4fa660 _ZN7Structs7BeltRef4readEP11IStreamBase
// Forwarder to ArmorRef::read (partial, see above).
void BeltRef::read(IStreamBase* stream) {
    ArmorRef::read(stream);
}

// 0x4d46e4 _ZN7Structs7HelmRef8finalizeEv
void HelmRef::finalize() {
    ArmorRef::finalize();
}

// 0x4fa664 _ZN7Structs7HelmRef4readEP11IStreamBase
// Forwarder to ArmorRef::read (partial, see above).
void HelmRef::read(IStreamBase* stream) {
    ArmorRef::read(stream);
}

// 0x4d4730 _ZN7Structs8GloveRef8finalizeEv
void GloveRef::finalize() {
    ArmorRef::finalize();
}

// 0x4fa668 _ZN7Structs8GloveRef4readEP11IStreamBase
// Forwarder to ArmorRef::read (partial, see above).
void GloveRef::read(IStreamBase* stream) {
    ArmorRef::read(stream);
}

// 0x4d477c _ZN7Structs8BootsRef8finalizeEv
void BootsRef::finalize() {
    ArmorRef::finalize();
}

// 0x4fa66c _ZN7Structs8BootsRef4readEP11IStreamBase
// Forwarder to ArmorRef::read (partial, see above).
void BootsRef::read(IStreamBase* stream) {
    ArmorRef::read(stream);
}

// 0x4d47c8 _ZN7Structs8Valuable8finalizeEv
void Valuable::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4fa670 _ZN7Structs8Valuable4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void Valuable::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4814 _ZN7Structs6Potion8finalizeEv
void Potion::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4faf18 _ZN7Structs6Potion4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void Potion::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d4860 _ZN7Structs8GoldItem8finalizeEv
void GoldItem::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

// 0x4fb7c0 _ZN7Structs8GoldItem4readEP11IStreamBase
// Partial: the ItemBase::read call is omitted (see WeaponRef::read).
void GoldItem::read(IStreamBase* stream) {
    ReadItemRefWords(*this, stream);
}

// 0x4d48ac _ZN7Structs4Item8finalizeEv
void Item::finalize() {
    FinalizeItemRefWords(*this);
    ItemBase::finalize();
}

}  // namespace Structs
