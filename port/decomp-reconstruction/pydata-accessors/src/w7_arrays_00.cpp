// Arrays::X::finalize() for the 33 tables of slice arrays_00. One shared body (FinalizeArrayTable):
//   1. if members is set and size is nonzero, the element vtable slot +0x8 (finalize) runs on each element,
//      in order;
//   2. if members is still set, the element vtable slot +0x0 (D1, the virtual destructor) runs from the last
//      element back to the first, then the block at members - 8 is freed (CustomFree);
//   3. members is cleared.
// The stride of each table is sizeof(its element class) in the port. The ARM listing stride of each table is
// kept in the comment on its line (e.g. "listing stride 72"). Hex-Rays gives the listing strides; the ARM
// listing was checked for v2Quests (284) and InventoryTable (8).
#include <cstring>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_structs_1.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

namespace {

template <std::size_t Stride>
void FinalizeArrayTable(std::uint8_t*& members, std::uint32_t& size) {
    if (members == nullptr) return;
    if (size != 0) {
        for (std::uint32_t i = 0; i < size; ++i) {
            reinterpret_cast<pydata::ArrayElement*>(members + Stride * i)->finalize();
        }
    }
    if (members != nullptr) {
        std::uint32_t count;
        std::memcpy(&count, members - 4, sizeof count);
        std::uint8_t* last = members + Stride * count;
        if (last != members) {
            for (;;) {
                std::uint8_t* cur = last - Stride;
                reinterpret_cast<pydata::ArrayElement*>(cur)->~ArrayElement();
                last = members;
                if (members == cur) break;
                last = cur;
            }
        }
        CustomFree(last - 8);
    }
    members = nullptr;
}

}  // namespace

namespace Arrays {

std::uint8_t* Sounds::members = nullptr;
std::uint32_t Sounds::size = 0;
std::uint8_t* StrID_Languages::members = nullptr;
std::uint32_t StrID_Languages::size = 0;
std::uint8_t* GameObjectDict::members = nullptr;
std::uint32_t GameObjectDict::size = 0;
std::uint8_t* ProjectileDict::members = nullptr;
std::uint32_t ProjectileDict::size = 0;
std::uint8_t* EffectDict::members = nullptr;
std::uint32_t EffectDict::size = 0;
std::uint8_t* AnimDict::members = nullptr;
std::uint32_t AnimDict::size = 0;
std::uint8_t* ModelDict::members = nullptr;
std::uint32_t ModelDict::size = 0;
std::uint8_t* WorldMapLockers::members = nullptr;
std::uint32_t WorldMapLockers::size = 0;
std::uint8_t* WorldMap::members = nullptr;
std::uint32_t WorldMap::size = 0;
std::uint8_t* v2Quests::members = nullptr;
std::uint32_t v2Quests::size = 0;
std::uint8_t* v2Events::members = nullptr;
std::uint32_t v2Events::size = 0;
std::uint8_t* v2Conditions::members = nullptr;
std::uint32_t v2Conditions::size = 0;
std::uint8_t* TrophyTable::members = nullptr;
std::uint32_t TrophyTable::size = 0;
std::uint8_t* SpawnGroups::members = nullptr;
std::uint32_t SpawnGroups::size = 0;
std::uint8_t* Sounds_bak::members = nullptr;
std::uint32_t Sounds_bak::size = 0;
std::uint8_t* SoundGroupsRouting::members = nullptr;
std::uint32_t SoundGroupsRouting::size = 0;
std::uint8_t* SoundBankPlayback::members = nullptr;
std::uint32_t SoundBankPlayback::size = 0;
std::uint8_t* Listeners::members = nullptr;
std::uint32_t Listeners::size = 0;
std::uint8_t* CharSoundsTable::members = nullptr;
std::uint32_t CharSoundsTable::size = 0;
std::uint8_t* SkillTable::members = nullptr;
std::uint32_t SkillTable::size = 0;
std::uint8_t* SkillListTable::members = nullptr;
std::uint32_t SkillListTable::size = 0;
std::uint8_t* ProjectileTable::members = nullptr;
std::uint32_t ProjectileTable::size = 0;
std::uint8_t* NumProbArray::members = nullptr;
std::uint32_t NumProbArray::size = 0;
std::uint8_t* MerchantTable::members = nullptr;
std::uint32_t MerchantTable::size = 0;
std::uint8_t* LootTable::members = nullptr;
std::uint32_t LootTable::size = 0;
std::uint8_t* ItemTypeList::members = nullptr;
std::uint32_t ItemTypeList::size = 0;
std::uint8_t* ItemList::members = nullptr;
std::uint32_t ItemList::size = 0;
std::uint8_t* InventoryTable::members = nullptr;
std::uint32_t InventoryTable::size = 0;
std::uint8_t* DropTilePriorityTable::members = nullptr;
std::uint32_t DropTilePriorityTable::size = 0;
std::uint8_t* ItemAudioVisualTable::members = nullptr;
std::uint32_t ItemAudioVisualTable::size = 0;
std::uint8_t* LevelList::members = nullptr;
std::uint32_t LevelList::size = 0;
std::uint8_t* FastTravelList::members = nullptr;
std::uint32_t FastTravelList::size = 0;
std::uint8_t* ItemBonusAttrMonopoly::members = nullptr;
std::uint32_t ItemBonusAttrMonopoly::size = 0;

// 0x4a38a4 _ZN6Arrays6Sounds8finalizeEv
// listing stride 12; element Structs::SoundAutoGen
void Sounds::finalize() { FinalizeArrayTable<sizeof(Structs::SoundAutoGen)>(members, size); }

// 0x4a3a24 _ZN6Arrays15StrID_Languages8finalizeEv
// listing stride 12; element Structs::LangSheetList
void StrID_Languages::finalize() { FinalizeArrayTable<sizeof(Structs::LangSheetList)>(members, size); }

// 0x4a3ba4 _ZN6Arrays14GameObjectDict8finalizeEv
// listing stride 12; element Structs::ColladaFile
void GameObjectDict::finalize() { FinalizeArrayTable<sizeof(Structs::ColladaFile)>(members, size); }

// 0x4a3d24 _ZN6Arrays14ProjectileDict8finalizeEv
// listing stride 12; element Structs::ColladaFile
void ProjectileDict::finalize() { FinalizeArrayTable<sizeof(Structs::ColladaFile)>(members, size); }

// 0x4a3ea4 _ZN6Arrays10EffectDict8finalizeEv
// listing stride 12; element Structs::ColladaFile
void EffectDict::finalize() { FinalizeArrayTable<sizeof(Structs::ColladaFile)>(members, size); }

// 0x4a4024 _ZN6Arrays8AnimDict8finalizeEv
// listing stride 12; element Structs::ColladaFile
void AnimDict::finalize() { FinalizeArrayTable<sizeof(Structs::ColladaFile)>(members, size); }

// 0x4a41a4 _ZN6Arrays9ModelDict8finalizeEv
// listing stride 12; PARTIAL: no element class declared for ModelDict, stride left at the listing value
void ModelDict::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a4324 _ZN6Arrays15WorldMapLockers8finalizeEv
// listing stride 12; element Structs::WorldMapLocker
void WorldMapLockers::finalize() { FinalizeArrayTable<sizeof(Structs::WorldMapLocker)>(members, size); }

// 0x4a44a4 _ZN6Arrays8WorldMap8finalizeEv
// listing stride 20; element Structs::WldMapLocation
void WorldMap::finalize() { FinalizeArrayTable<sizeof(Structs::WldMapLocation)>(members, size); }

// 0x4a4624 _ZN6Arrays8v2Quests8finalizeEv
// listing stride 284; element Structs::v2Quest
void v2Quests::finalize() { FinalizeArrayTable<sizeof(Structs::v2Quest)>(members, size); }

// 0x4a47a4 _ZN6Arrays8v2Events8finalizeEv
// listing stride 24; element Structs::v2Event
void v2Events::finalize() { FinalizeArrayTable<sizeof(Structs::v2Event)>(members, size); }

// 0x4a4924 _ZN6Arrays12v2Conditions8finalizeEv
// listing stride 16; element Structs::v2CondAnd
void v2Conditions::finalize() { FinalizeArrayTable<sizeof(Structs::v2CondAnd)>(members, size); }

// 0x4a4a98 _ZN6Arrays11TrophyTable8finalizeEv
// listing stride 32; element Structs::Trophy
void TrophyTable::finalize() { FinalizeArrayTable<sizeof(Structs::Trophy)>(members, size); }

// 0x4a4c0c _ZN6Arrays11SpawnGroups8finalizeEv
// listing stride 20; element Structs::SpawnGroup
void SpawnGroups::finalize() { FinalizeArrayTable<sizeof(Structs::SpawnGroup)>(members, size); }

// 0x4a4d8c _ZN6Arrays10Sounds_bak8finalizeEv
// listing stride 44; element Structs::Sound
void Sounds_bak::finalize() { FinalizeArrayTable<sizeof(Structs::Sound)>(members, size); }

// 0x4a4f0c _ZN6Arrays18SoundGroupsRouting8finalizeEv
// listing stride 16; element Structs::SoundGroup
void SoundGroupsRouting::finalize() { FinalizeArrayTable<sizeof(Structs::SoundGroup)>(members, size); }

// 0x4a5080 _ZN6Arrays17SoundBankPlayback8finalizeEv
// listing stride 12; element Structs::SoundBank
void SoundBankPlayback::finalize() { FinalizeArrayTable<sizeof(Structs::SoundBank)>(members, size); }

// 0x4a5200 _ZN6Arrays9Listeners8finalizeEv
// listing stride 28; element Structs::Listener
void Listeners::finalize() { FinalizeArrayTable<sizeof(Structs::Listener)>(members, size); }

// 0x4a5380 _ZN6Arrays15CharSoundsTable8finalizeEv
// listing stride 40; element Structs::CharSounds
void CharSoundsTable::finalize() { FinalizeArrayTable<sizeof(Structs::CharSounds)>(members, size); }

// 0x4a5500 _ZN6Arrays10SkillTable8finalizeEv
// listing stride 76; element Structs::Skill
void SkillTable::finalize() { FinalizeArrayTable<sizeof(Structs::Skill)>(members, size); }

// 0x4a5680 _ZN6Arrays14SkillListTable8finalizeEv
// listing stride 12; element Structs::SkillList
void SkillListTable::finalize() { FinalizeArrayTable<sizeof(Structs::SkillList)>(members, size); }

// 0x4a5800 _ZN6Arrays15ProjectileTable8finalizeEv
// listing stride 72; element Structs::Projectile
void ProjectileTable::finalize() { FinalizeArrayTable<sizeof(Structs::Projectile)>(members, size); }

// 0x4a5980 _ZN6Arrays12NumProbArray8finalizeEv
// listing stride 12; element Structs::NumProbList
void NumProbArray::finalize() { FinalizeArrayTable<sizeof(Structs::NumProbList)>(members, size); }

// 0x4a5b00 _ZN6Arrays13MerchantTable8finalizeEv
// listing stride 20; element Structs::Merchant
void MerchantTable::finalize() { FinalizeArrayTable<sizeof(Structs::Merchant)>(members, size); }

// 0x4a5c80 _ZN6Arrays9LootTable8finalizeEv
// listing stride 36; element Structs::Loot
void LootTable::finalize() { FinalizeArrayTable<sizeof(Structs::Loot)>(members, size); }

// 0x4a5e00 _ZN6Arrays12ItemTypeList8finalizeEv
// listing stride 12; element Structs::ItemTypeListList
void ItemTypeList::finalize() { FinalizeArrayTable<sizeof(Structs::ItemTypeListList)>(members, size); }

// 0x4a6100 _ZN6Arrays8ItemList8finalizeEv
// listing stride 12; element Structs::ItemListEntry (matched by name; the element is not confirmed in the ledger)
void ItemList::finalize() { FinalizeArrayTable<sizeof(Structs::ItemListEntry)>(members, size); }

// 0x4a6280 _ZN6Arrays14InventoryTable8finalizeEv
// listing stride 8; element Structs::Inventory
void InventoryTable::finalize() { FinalizeArrayTable<sizeof(Structs::Inventory)>(members, size); }

// 0x4a63f4 _ZN6Arrays21DropTilePriorityTable8finalizeEv
// listing stride 12; PARTIAL: no element class declared for DropTilePriorityTable, stride left at the listing value
void DropTilePriorityTable::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a6574 _ZN6Arrays20ItemAudioVisualTable8finalizeEv
// listing stride 20; element Structs::ItemAudioVisual
void ItemAudioVisualTable::finalize() { FinalizeArrayTable<sizeof(Structs::ItemAudioVisual)>(members, size); }

// 0x4a66f4 _ZN6Arrays9LevelList8finalizeEv
// listing stride 72; element Structs::LevelDeclaration
void LevelList::finalize() { FinalizeArrayTable<sizeof(Structs::LevelDeclaration)>(members, size); }

// 0x4a6874 _ZN6Arrays14FastTravelList8finalizeEv
// listing stride 28; element Structs::FastTravelDestination
void FastTravelList::finalize() { FinalizeArrayTable<sizeof(Structs::FastTravelDestination)>(members, size); }

// 0x4a69f4 _ZN6Arrays21ItemBonusAttrMonopoly8finalizeEv
// listing stride 12; element Structs::ItemBonusAttrList (matched by name; the element is not confirmed in the ledger)
void ItemBonusAttrMonopoly::finalize() { FinalizeArrayTable<sizeof(Structs::ItemBonusAttrList)>(members, size); }

}  // namespace Arrays
