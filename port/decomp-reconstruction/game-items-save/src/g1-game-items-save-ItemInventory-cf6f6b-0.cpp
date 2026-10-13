// Batch g1-game-items-save-ItemInventory-cf6f6b-0: the item-list, equipment-set, gold and query methods
// of ItemInventory. Declarations and layout come from dh2/ItemInventory.hpp.
//
// Not in this file yet: GetNumPotions, FindItem, IsItemEquipped, both IsItemEquippable overloads,
// UpdateLocalizationForItems and _HasItemInstanceLike read ItemInstance members, and dh2/ItemInstance.hpp
// does not exist yet. SetGold and TrophyCheckArmorSet are partial: their trophy unlocks are left out.

#include "dh2/ItemInventory.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "core-platform/memory.hpp"
#include "dh2/DebugSwitches.hpp"

namespace {

// The original's ASSERT(expr) at ItemInventory.cpp:line. gAssertLevel 2 stops the program (the original
// writes to address 0 to force a crash), 1 prints the message and carries on, 0 stays silent.
void ReportItemInventoryAssert(const char* expr, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr,
                     "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Items\\ItemInventory.cpp", line);
    }
}

}  // namespace

// 0x3fc608 _ZNK13ItemInventory11GetNumItemsEv
int ItemInventory::GetNumItems() const {
    return static_cast<int>(m_items.size());
}

// 0x3fc61c _ZN13ItemInventory7GetItemEj
ItemInstance* ItemInventory::GetItem(unsigned int idx) {
    if (idx < m_items.size()) {
        return m_items[idx]->m_item;
    }
    return nullptr;
}

// 0x3fc63c _ZNK13ItemInventory12GetItemIndexEPK12ItemInstance
int ItemInventory::GetItemIndex(const ItemInstance* item) const {
    const std::size_t count = m_items.size();
    if (count != 0) {
        if (m_items[0]->m_item == item) {
            return 0;
        }
        for (std::size_t i = 1; i != count; ++i) {
            if (m_items[i]->m_item == item) {
                return static_cast<int>(i);
            }
        }
    }
    return -1;
}

// 0x3fc6a0 _ZN13ItemInventory18GetCurrentSkillSetEi
// The skill set is always 0 in this build.
int ItemInventory::GetCurrentSkillSet(int /*slot*/) {
    return 0;
}

// 0x3fc6a8 _ZNK13ItemInventory18GetCurrentEquipSetEi
// Negative slots and slots 1 and 2 report the current equipment set (m_equipSet, +0x2e). Every other slot,
// including 0 and 3 and up, reports set 0. The original's (unsigned)(slot - 1) <= 1 test is the same check.
int ItemInventory::GetCurrentEquipSet(int slot) const {
    if (slot < 0 || static_cast<unsigned int>(slot) - 1u <= 1u) {
        return m_equipSet;
    }
    return 0;
}

// 0x3fc6c8 _ZN13ItemInventory16SwapEquipmentSetEv
void ItemInventory::SwapEquipmentSet() {
    m_equipSet = static_cast<std::int8_t>((m_equipSet + 1) % 2);
}

// 0x3fcff8 _ZN13ItemInventory19TrophyCheckArmorSetEv
// PARTIAL: the armour gate is rebuilt. The unlock dispatch is not, because it needs the game-app helpers
// TrophyCheckArmorSet{Phantasm,Rage,Angelic,Decay,Shadow,Marksman,Lordly,Jester,Archmage}(ItemInventory*)
// (their return type is not recorded yet) and dh2/TrophyManager.hpp (not written yet).
void ItemInventory::TrophyCheckArmorSet() {
    // Slots 3, 4, 8, 0 and 7 must all be taken before any trophy is checked.
    if (!IsEquipmentSlotTaken(3) || !IsEquipmentSlotTaken(4) || !IsEquipmentSlotTaken(8) ||
        !IsEquipmentSlotTaken(0) || !IsEquipmentSlotTaken(7)) {
        return;
    }
    // Not rebuilt: the original reads m_character->m_charPropsId (+0x13c8) and picks one helper:
    // 291 Phantasm, 264 Rage, 265 Angelic, 292 Decay, 326 Marksman, 327 Shadow, otherwise none.
    // A helper result other than -1 goes to TrophyManager::UnlockTrophy(TrophyManager::s_instance, id) and
    // returns. A -1 result, or no matching helper, falls through to Lordly, then Jester, then Archmage.
}

// 0x3fda20 _ZNK13ItemInventory20IsItemEquippedInSlotEjj
bool ItemInventory::IsItemEquippedInSlot(unsigned int itemIdx, unsigned int slot) const {
    if (itemIdx >= m_items.size()) {
        ReportItemInventoryAssert("itemIdx < m_items.size()", 616);
    }
    // The original does not guard itemIdx here either: with gAssertLevel 0 it reads past m_items.
    const int set = GetCurrentEquipSet(static_cast<int>(slot));
    return static_cast<int>(m_items[itemIdx]->m_equipSlot[set]) == static_cast<int>(slot);
}

// 0x3fdbf8 _ZNK13ItemInventory7GetItemEj
ItemInstance* ItemInventory::GetItem(unsigned int idx) const {
    if (idx < m_items.size()) {
        return m_items[idx]->m_item;
    }
    ReportItemInventoryAssert("itemIdx < m_items.size()", 555);
    return nullptr;
}

// 0x3fe330 _ZNK13ItemInventory15IsInventoryFullEv
bool ItemInventory::IsInventoryFull() const {
    // The original loads the singleton directly (DebugSwitches::load on s_inst). GetInstance returns that
    // singleton. Its first-use load runs before our load(), so the file is read once more on first use.
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    const bool infiniteSwitch = switches->GetSwitch("InfiniteInventory");
    return !infiniteSwitch && !m_infiniteInventory && m_items.size() > 99;
}

// 0x3fdfd8 _ZN13ItemInventory7SetGoldEi
// PARTIAL: the gold clamp and the amount assert are rebuilt. The trophy section is not. It needs
// dh2/TrophyManager.hpp (TrophyManager::s_instance and UnlockTrophy, not written yet) and the local-player
// test PlayerManager::IsLocalPlayer on the global at 0x99f76c (the global has no name yet).
void ItemInventory::SetGold(int amount) {
    if (amount < 0) {
        ReportItemInventoryAssert("amount >= 0", 322);
    }
    m_gold = amount > m_maxGold ? m_maxGold : amount;
    // Not rebuilt: when m_character->IsPlayer() and the local-player test hold, m_gold above 9999 unlocks
    // "gear_10kgold", above 99999 also "gear_100kgold", above 999999 also "gear_1mgold", each through
    // Arrays::GetMemberIDByString<Arrays::TrophyTable> and TrophyManager::UnlockTrophy.
}

// 0x3fe164 _ZN13ItemInventory7AddGoldEi
void ItemInventory::AddGold(int amount) {
    const int gold = m_gold;
    if (amount < 0) {
        if (gold >= -amount) {
            SetGold(gold + amount);
            return;
        }
        amount = -gold;
    }
    if (amount > 0) {
        const int room = m_maxGold - gold;
        if (amount > room) {
            // The original masks room with ~(room >> 31), which is max(room, 0).
            amount = room < 0 ? 0 : room;
        }
    }
    SetGold(gold + amount);
}

// 0x3fe1a8 _ZN13ItemInventory14TransferGoldToEiRS_
void ItemInventory::TransferGoldTo(int amount, ItemInventory& dst) {
    dst.AddGold(amount);
    AddGold(-amount);
}
