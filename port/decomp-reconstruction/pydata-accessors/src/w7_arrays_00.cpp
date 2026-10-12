// Arrays::X::finalize() for the 33 tables of slice arrays_00. One shared body (FinalizeArrayTable):
//   1. if members is set and size is nonzero, the element vtable slot +0x8 (finalize) runs on each element,
//      in order;
//   2. if members is still set, the element vtable slot +0x0 (D1, the virtual destructor) runs from the last
//      element back to the first, then the block at members - 8 is freed (CustomFree);
//   3. members is cleared.
// The stride is per table (the constant in each function's call). Hex-Rays gives every
// stride; the ARM listing was checked for v2Quests (284) and InventoryTable (8).
#include <cstring>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"

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
void Sounds::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a3a24 _ZN6Arrays15StrID_Languages8finalizeEv
void StrID_Languages::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a3ba4 _ZN6Arrays14GameObjectDict8finalizeEv
void GameObjectDict::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a3d24 _ZN6Arrays14ProjectileDict8finalizeEv
void ProjectileDict::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a3ea4 _ZN6Arrays10EffectDict8finalizeEv
void EffectDict::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a4024 _ZN6Arrays8AnimDict8finalizeEv
void AnimDict::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a41a4 _ZN6Arrays9ModelDict8finalizeEv
void ModelDict::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a4324 _ZN6Arrays15WorldMapLockers8finalizeEv
void WorldMapLockers::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a44a4 _ZN6Arrays8WorldMap8finalizeEv
void WorldMap::finalize() { FinalizeArrayTable<20>(members, size); }

// 0x4a4624 _ZN6Arrays8v2Quests8finalizeEv
void v2Quests::finalize() { FinalizeArrayTable<284>(members, size); }

// 0x4a47a4 _ZN6Arrays8v2Events8finalizeEv
void v2Events::finalize() { FinalizeArrayTable<24>(members, size); }

// 0x4a4924 _ZN6Arrays12v2Conditions8finalizeEv
void v2Conditions::finalize() { FinalizeArrayTable<16>(members, size); }

// 0x4a4a98 _ZN6Arrays11TrophyTable8finalizeEv
void TrophyTable::finalize() { FinalizeArrayTable<32>(members, size); }

// 0x4a4c0c _ZN6Arrays11SpawnGroups8finalizeEv
void SpawnGroups::finalize() { FinalizeArrayTable<20>(members, size); }

// 0x4a4d8c _ZN6Arrays10Sounds_bak8finalizeEv
void Sounds_bak::finalize() { FinalizeArrayTable<44>(members, size); }

// 0x4a4f0c _ZN6Arrays18SoundGroupsRouting8finalizeEv
void SoundGroupsRouting::finalize() { FinalizeArrayTable<16>(members, size); }

// 0x4a5080 _ZN6Arrays17SoundBankPlayback8finalizeEv
void SoundBankPlayback::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a5200 _ZN6Arrays9Listeners8finalizeEv
void Listeners::finalize() { FinalizeArrayTable<28>(members, size); }

// 0x4a5380 _ZN6Arrays15CharSoundsTable8finalizeEv
void CharSoundsTable::finalize() { FinalizeArrayTable<40>(members, size); }

// 0x4a5500 _ZN6Arrays10SkillTable8finalizeEv
void SkillTable::finalize() { FinalizeArrayTable<76>(members, size); }

// 0x4a5680 _ZN6Arrays14SkillListTable8finalizeEv
void SkillListTable::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a5800 _ZN6Arrays15ProjectileTable8finalizeEv
void ProjectileTable::finalize() { FinalizeArrayTable<72>(members, size); }

// 0x4a5980 _ZN6Arrays12NumProbArray8finalizeEv
void NumProbArray::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a5b00 _ZN6Arrays13MerchantTable8finalizeEv
void MerchantTable::finalize() { FinalizeArrayTable<20>(members, size); }

// 0x4a5c80 _ZN6Arrays9LootTable8finalizeEv
void LootTable::finalize() { FinalizeArrayTable<36>(members, size); }

// 0x4a5e00 _ZN6Arrays12ItemTypeList8finalizeEv
void ItemTypeList::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a6100 _ZN6Arrays8ItemList8finalizeEv
void ItemList::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a6280 _ZN6Arrays14InventoryTable8finalizeEv
void InventoryTable::finalize() { FinalizeArrayTable<8>(members, size); }

// 0x4a63f4 _ZN6Arrays21DropTilePriorityTable8finalizeEv
void DropTilePriorityTable::finalize() { FinalizeArrayTable<12>(members, size); }

// 0x4a6574 _ZN6Arrays20ItemAudioVisualTable8finalizeEv
void ItemAudioVisualTable::finalize() { FinalizeArrayTable<20>(members, size); }

// 0x4a66f4 _ZN6Arrays9LevelList8finalizeEv
void LevelList::finalize() { FinalizeArrayTable<72>(members, size); }

// 0x4a6874 _ZN6Arrays14FastTravelList8finalizeEv
void FastTravelList::finalize() { FinalizeArrayTable<28>(members, size); }

// 0x4a69f4 _ZN6Arrays21ItemBonusAttrMonopoly8finalizeEv
void ItemBonusAttrMonopoly::finalize() { FinalizeArrayTable<12>(members, size); }

}  // namespace Arrays
