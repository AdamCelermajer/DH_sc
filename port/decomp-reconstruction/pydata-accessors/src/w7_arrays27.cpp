// Arrays::X::read(IStreamBase*) for slice arrays_27 (27 tables). Declarations are in
// include/pydata-accessors/w7_arrays27.hpp. Each function carries its address and mangled symbol.
//
// Shared body (ReadArrayTable), from the Hex-Rays pseudocode of every row:
//   1. count = readAs<uint32>(stream)
//   2. Table::finalize() on the old table
//   3. size = count; one block of Stride * count + 8 bytes; header words (Stride, count); members = block + 8
//   4. each element gets its vptr (the Elem class's vtable address point) and the listed zeroed words
//   5. each element's read slot (+0xc) runs in order, with the stream
// The stride of each table is sizeof(element) (host), per the port's element ABI decision. The ARM listing
// stride is kept in each table's comment as a reference. Listing offsets of zeroed words are element-relative
// (result[k] in the listing is byte 4k - 8). LevelList and FastTravelList keep the listing stride: their host
// classes are smaller than a zeroed offset, so widening would write past the element (both are partial).
// The finalize helpers in w7_arrays_01.cpp and w7_arrays_00.cpp still walk the listing stride; they must use
// the same sizeof value for each table before these reads are used.
#include "pydata-accessors/w7_arrays27.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"
#include "pydata-accessors/w7_arrays_01.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

namespace {

// The vtable address point of Elem: the first word of a polymorphic object (Itanium ABI).
template <class Elem>
const void* VtableOf() {
    Elem tmp;
    const void* vptr = nullptr;
    std::memcpy(&vptr, &tmp, sizeof vptr);
    return vptr;
}

inline pydata::ArrayElement* ElementAt(std::uint8_t* p) {
    return reinterpret_cast<pydata::ArrayElement*>(p);
}

template <class Elem, std::size_t Stride, class Finalize>
void ReadArrayTable(std::uint8_t*& members, std::uint32_t& size, IStreamBase* stream, Finalize finalize,
                    std::initializer_list<std::size_t> zeroed) {
    const std::uint32_t count = StreamReader::readAs<std::uint32_t>(stream);
    finalize();
    size = count;
    std::uint8_t* block = static_cast<std::uint8_t*>(MemoryHintState(Stride * count + 8, 1));
    const std::uint32_t header_stride = Stride;
    std::memcpy(block, &header_stride, sizeof header_stride);
    std::memcpy(block + 4, &count, sizeof count);
    members = block + 8;
    const void* vptr = VtableOf<Elem>();
    for (std::uint32_t i = 0; i < count; ++i) {
        std::uint8_t* element = members + Stride * i;
        std::memcpy(element, &vptr, sizeof vptr);
        for (std::size_t offset : zeroed) {
            const std::uint32_t zero = 0;
            std::memcpy(element + offset, &zero, sizeof zero);
        }
    }
    for (std::uint32_t i = 0; i < size; ++i) {
        ElementAt(members + Stride * i)->read(stream);
    }
}

}  // namespace

namespace Arrays {

// Static storage of the 21 tables declared in this slice (the 6 tables of w7_arrays_00.hpp own theirs).
std::uint8_t* ItemPowerTable::members = nullptr;
std::uint32_t ItemPowerTable::size = 0;
std::uint8_t* HintPages::members = nullptr;
std::uint32_t HintPages::size = 0;
std::uint8_t* HelpPages::members = nullptr;
std::uint32_t HelpPages::size = 0;
std::uint8_t* TriggerTraps::members = nullptr;
std::uint32_t TriggerTraps::size = 0;
std::uint8_t* TriggerPlates::members = nullptr;
std::uint32_t TriggerPlates::size = 0;
std::uint8_t* TriggerObjects::members = nullptr;
std::uint32_t TriggerObjects::size = 0;
std::uint8_t* TimerTraps::members = nullptr;
std::uint32_t TimerTraps::size = 0;
std::uint8_t* ProjectileTraps::members = nullptr;
std::uint32_t ProjectileTraps::size = 0;
std::uint8_t* OpenableContainers::members = nullptr;
std::uint32_t OpenableContainers::size = 0;
std::uint8_t* LiftableObjects::members = nullptr;
std::uint32_t LiftableObjects::size = 0;
std::uint8_t* GameObjectDamager::members = nullptr;
std::uint32_t GameObjectDamager::size = 0;
std::uint8_t* ExplosiveTraps::members = nullptr;
std::uint32_t ExplosiveTraps::size = 0;
std::uint8_t* Doors::members = nullptr;
std::uint32_t Doors::size = 0;
std::uint8_t* DestructibleContainers::members = nullptr;
std::uint32_t DestructibleContainers::size = 0;
std::uint8_t* FontPalette::members = nullptr;
std::uint32_t FontPalette::size = 0;
std::uint8_t* FootstepEffectTable::members = nullptr;
std::uint32_t FootstepEffectTable::size = 0;
std::uint8_t* CharEffectTable::members = nullptr;
std::uint32_t CharEffectTable::size = 0;
std::uint8_t* AnimatedEffectTable::members = nullptr;
std::uint32_t AnimatedEffectTable::size = 0;
std::uint8_t* Dialogs::members = nullptr;
std::uint32_t Dialogs::size = 0;
std::uint8_t* DialogActors::members = nullptr;
std::uint32_t DialogActors::size = 0;
std::uint8_t* GameOptionTable::members = nullptr;
std::uint32_t GameOptionTable::size = 0;

// ---- Tables declared in w7_arrays_00.hpp (free functions; see the header) ----

// 0x4b9ab0 _ZN6Arrays15ProjectileTable4readEP11IStreamBase
// Element Structs::Projectile (host 80 bytes, listing stride 72). Zeroed words: none.
void ReadProjectileTable(IStreamBase* stream) {
    ReadArrayTable<Structs::Projectile, sizeof(Structs::Projectile)>(ProjectileTable::members, ProjectileTable::size, stream,
                                            [] { ProjectileTable::finalize(); }, {});
}

// 0x4b9d40 _ZN6Arrays13MerchantTable4readEP11IStreamBase
// Element Structs::Merchant (host 32, listing stride 20). Zeroed: +0x10.
void ReadMerchantTable(IStreamBase* stream) {
    ReadArrayTable<Structs::Merchant, sizeof(Structs::Merchant)>(MerchantTable::members, MerchantTable::size, stream,
                                          [] { MerchantTable::finalize(); }, {16});
}

// 0x4b9fe0 _ZN6Arrays12ItemTypeList4readEP11IStreamBase
// Element Structs::ItemTypeListList (host 24, listing stride 12). Zeroed: +0x8.
void ReadItemTypeList(IStreamBase* stream) {
    ReadArrayTable<Structs::ItemTypeListList, sizeof(Structs::ItemTypeListList)>(ItemTypeList::members, ItemTypeList::size, stream,
                                                  [] { ItemTypeList::finalize(); }, {8});
}

// 0x4ba648 _ZN6Arrays20ItemAudioVisualTable4readEP11IStreamBase
// Element Structs::ItemAudioVisual (host 32, listing stride 20). Zeroed: +0x10.
void ReadItemAudioVisualTable(IStreamBase* stream) {
    ReadArrayTable<Structs::ItemAudioVisual, sizeof(Structs::ItemAudioVisual)>(ItemAudioVisualTable::members, ItemAudioVisualTable::size, stream,
                                                 [] { ItemAudioVisualTable::finalize(); }, {16});
}

// 0x4ba794 _ZN6Arrays9LevelList4readEP11IStreamBase
// Element Structs::LevelDeclaration (host 24, listing stride 72). Zeroed: +0xc, +0x20. Partial: the class declares
// no read slot, so the vptr +0xc dispatch has no target in the port.
void ReadLevelList(IStreamBase* stream) {
    ReadArrayTable<Structs::LevelDeclaration, 72>(LevelList::members, LevelList::size, stream,
                                                  [] { LevelList::finalize(); }, {12, 32});
}

// 0x4ba8e4 _ZN6Arrays14FastTravelList4readEP11IStreamBase
// Element Structs::FastTravelDestination (host 16, listing stride 28). Zeroed: +0x10. Partial: the class declares
// no read slot (as ReadLevelList).
void ReadFastTravelList(IStreamBase* stream) {
    ReadArrayTable<Structs::FastTravelDestination, 28>(FastTravelList::members, FastTravelList::size, stream,
                                                       [] { FastTravelList::finalize(); }, {16});
}

// ---- Tables declared in this slice (finalize from w7_arrays_01.hpp) ----

// 0x4bab7c _ZN6Arrays14ItemPowerTable4readEP11IStreamBase
// Element Structs::ItemPowerRef (host 56, listing stride 40). Zeroed: +0x10.
void ItemPowerTable::read(IStreamBase* stream) {
    ReadArrayTable<Structs::ItemPowerRef, sizeof(Structs::ItemPowerRef)>(members, size, stream,
                                              [] { FinalizeItemPowerTable(members, size); }, {16});
}

// 0x4bae14 _ZN6Arrays9HintPages4readEP11IStreamBase
// Element Structs::HintPage (host 16, listing stride 12). Zeroed: none.
void HintPages::read(IStreamBase* stream) {
    ReadArrayTable<Structs::HintPage, sizeof(Structs::HintPage)>(members, size, stream, [] { FinalizeHintPages(members, size); }, {});
}

// 0x4baf58 _ZN6Arrays9HelpPages4readEP11IStreamBase
// Element Structs::HelpPage (host 16, listing stride 12). Zeroed: none.
void HelpPages::read(IStreamBase* stream) {
    ReadArrayTable<Structs::HelpPage, sizeof(Structs::HelpPage)>(members, size, stream, [] { FinalizeHelpPages(members, size); }, {});
}

// 0x4bb09c _ZN6Arrays12TriggerTraps4readEP11IStreamBase
// Element Structs::TriggerTrap (host 40, listing stride 28). Zeroed: +0x10.
void TriggerTraps::read(IStreamBase* stream) {
    ReadArrayTable<Structs::TriggerTrap, sizeof(Structs::TriggerTrap)>(members, size, stream,
                                             [] { FinalizeTriggerTraps(members, size); }, {16});
}

// 0x4bb1e8 _ZN6Arrays13TriggerPlates4readEP11IStreamBase
// Element Structs::TriggerPlate (host 40, listing stride 32). Zeroed: none.
void TriggerPlates::read(IStreamBase* stream) {
    ReadArrayTable<Structs::TriggerPlate, sizeof(Structs::TriggerPlate)>(members, size, stream,
                                              [] { FinalizeTriggerPlates(members, size); }, {});
}

// 0x4bb320 _ZN6Arrays14TriggerObjects4readEP11IStreamBase
// Element Structs::TriggerObject (host 32, listing stride 24). Zeroed: +0xc.
void TriggerObjects::read(IStreamBase* stream) {
    ReadArrayTable<Structs::TriggerObject, sizeof(Structs::TriggerObject)>(members, size, stream,
                                               [] { FinalizeTriggerObjects(members, size); }, {12});
}

// 0x4bb46c _ZN6Arrays10TimerTraps4readEP11IStreamBase
// Element Structs::TimerTrap (host 40, listing stride 32). Zeroed: +0x14.
void TimerTraps::read(IStreamBase* stream) {
    ReadArrayTable<Structs::TimerTrap, sizeof(Structs::TimerTrap)>(members, size, stream,
                                           [] { FinalizeTimerTraps(members, size); }, {20});
}

// 0x4bb5ac _ZN6Arrays15ProjectileTraps4readEP11IStreamBase
// Element Structs::ProjectileTrap (host 48, listing stride 40). Zeroed: +0x1c.
void ProjectileTraps::read(IStreamBase* stream) {
    ReadArrayTable<Structs::ProjectileTrap, sizeof(Structs::ProjectileTrap)>(members, size, stream,
                                                [] { FinalizeProjectileTraps(members, size); }, {28});
}

// 0x4bb6f8 _ZN6Arrays18OpenableContainers4readEP11IStreamBase
// Element Structs::OpenableContainer (host 56, listing stride 40). Zeroed: +0x18.
void OpenableContainers::read(IStreamBase* stream) {
    ReadArrayTable<Structs::OpenableContainer, sizeof(Structs::OpenableContainer)>(members, size, stream,
                                                   [] { FinalizeOpenableContainers(members, size); }, {24});
}

// 0x4bb844 _ZN6Arrays15LiftableObjects4readEP11IStreamBase
// Element Structs::LiftableObject (host 16, listing stride 8). Zeroed: none.
void LiftableObjects::read(IStreamBase* stream) {
    ReadArrayTable<Structs::LiftableObject, sizeof(Structs::LiftableObject)>(members, size, stream,
                                               [] { FinalizeLiftableObjects(members, size); }, {});
}

// 0x4bb978 _ZN6Arrays17GameObjectDamager4readEP11IStreamBase
// Element Structs::GameObjectDamage (host 32, listing stride 24). Zeroed: none.
void GameObjectDamager::read(IStreamBase* stream) {
    ReadArrayTable<Structs::GameObjectDamage, sizeof(Structs::GameObjectDamage)>(members, size, stream,
                                                  [] { FinalizeGameObjectDamager(members, size); }, {});
}

// 0x4bbabc _ZN6Arrays14ExplosiveTraps4readEP11IStreamBase
// Element Structs::ExplosiveTrap (host 40, listing stride 24). Zeroed: +0x10.
void ExplosiveTraps::read(IStreamBase* stream) {
    ReadArrayTable<Structs::ExplosiveTrap, sizeof(Structs::ExplosiveTrap)>(members, size, stream,
                                               [] { FinalizeExplosiveTraps(members, size); }, {16});
}

// 0x4bbc04 _ZN6Arrays5Doors4readEP11IStreamBase
// Element Structs::Door (host 40, listing stride 24). Zeroed: +0x8.
void Doors::read(IStreamBase* stream) {
    ReadArrayTable<Structs::Door, sizeof(Structs::Door)>(members, size, stream, [] { FinalizeDoors(members, size); }, {8});
}

// 0x4bbd50 _ZN6Arrays22DestructibleContainers4readEP11IStreamBase
// Element Structs::DestructibleContainer (host 80, listing stride 68). Zeroed: +0x30.
void DestructibleContainers::read(IStreamBase* stream) {
    ReadArrayTable<Structs::DestructibleContainer, sizeof(Structs::DestructibleContainer)>(members, size, stream,
                                                       [] { FinalizeDestructibleContainers(members, size); }, {48});
}

// 0x4bbe9c _ZN6Arrays11FontPalette4readEP11IStreamBase
// Element Structs::FontColorDef (host 16, listing stride 12). Zeroed: none.
void FontPalette::read(IStreamBase* stream) {
    ReadArrayTable<Structs::FontColorDef, sizeof(Structs::FontColorDef)>(members, size, stream, [] { FinalizeFontPalette(members, size); }, {});
}

// 0x4bc278 _ZN6Arrays19FootstepEffectTable4readEP11IStreamBase
// Element Structs::FootstepEffect (host 56, listing stride 32). Zeroed: +0xc, +0x14, +0x1c.
void FootstepEffectTable::read(IStreamBase* stream) {
    ReadArrayTable<Structs::FootstepEffect, sizeof(Structs::FootstepEffect)>(members, size, stream,
                                                [] { FinalizeFootstepEffectTable(members, size); }, {12, 20, 28});
}

// 0x4bc3c0 _ZN6Arrays15CharEffectTable4readEP11IStreamBase
// Element Structs::CharEffect (host 32, listing stride 24). Zeroed: none.
void CharEffectTable::read(IStreamBase* stream) {
    ReadArrayTable<Structs::CharEffect, sizeof(Structs::CharEffect)>(members, size, stream,
                                            [] { FinalizeCharEffectTable(members, size); }, {});
}

// 0x4bc504 _ZN6Arrays19AnimatedEffectTable4readEP11IStreamBase
// Element Structs::AnimFXTpl (host 40, listing stride 24). Zeroed: +0x10.
void AnimatedEffectTable::read(IStreamBase* stream) {
    ReadArrayTable<Structs::AnimFXTpl, sizeof(Structs::AnimFXTpl)>(members, size, stream,
                                           [] { FinalizeAnimatedEffectTable(members, size); }, {16});
}

// 0x4bc64c _ZN6Arrays7Dialogs4readEP11IStreamBase
// Element Structs::DialogStepList (host 24, listing stride 12). Zeroed: +0x8.
void Dialogs::read(IStreamBase* stream) {
    ReadArrayTable<Structs::DialogStepList, sizeof(Structs::DialogStepList)>(members, size, stream, [] { FinalizeDialogs(members, size); }, {8});
}

// 0x4bc798 _ZN6Arrays12DialogActors4readEP11IStreamBase
// Element Structs::DialogActor (host 32, listing stride 20). Zeroed: +0xc.
void DialogActors::read(IStreamBase* stream) {
    ReadArrayTable<Structs::DialogActor, sizeof(Structs::DialogActor)>(members, size, stream,
                                             [] { FinalizeDialogActors(members, size); }, {12});
}

// 0x4bc8e0 _ZN6Arrays15GameOptionTable4readEP11IStreamBase
// Element Structs::GameOption (host 40, listing stride 32). Zeroed: none.
void GameOptionTable::read(IStreamBase* stream) {
    ReadArrayTable<Structs::GameOption, sizeof(Structs::GameOption)>(members, size, stream,
                                            [] { FinalizeGameOptionTable(members, size); }, {});
}

}  // namespace Arrays
