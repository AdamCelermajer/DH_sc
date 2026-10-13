// Batch g1-game-items-save-ItemObject-eb7cc3-1 (owner ItemObject, part 2 of 2).
// Constructor, destructors, InitOnce, DeclareProperties, the auto-pickup hack, the loot drop path (DropLootTable,
// GetInventoryToDrop, DropInventory, DropAndAwardLoot, _GetRandomDropPos), _GetNextPlayerQuestRRId and Interact.
// Reconstructed from the Hex-Rays pseudocode and the assembly of libDungeonHunter2.so.
//
// Shared conventions in this file:
//  * The owner header ItemObject.hpp is not edited (part 1 of the class is written in parallel).
//  * PlayerManager reads sit in g1-game-items-save-ItemObject-eb7cc3-1_players.cpp (namespace ItemObjectPlayers); see the
//    note at the top of that file for why.
//  * Asserts follow the other batches: gAssertLevel 2 aborts (the original writes to address 0), 1 prints the ASSERT line.
//    Where the original goes on after an ASSERT with a null dropper (it dereferences it next), this port returns.
//  * Arrays::ItemAudioVisualTable rows are read at the host element size (Structs::ItemAudioVisual), not the 20-byte file
//    stride (TRACK-B decision 3), the same way the loot tables are read in the ItemInventory batch.
//  * Point3D<float>::normalize (0x34d0b0) is reproduced in NormalizeDirection below: the full dh2/Point3D.hpp declares a
//    second Point3D template and cannot share a translation unit with GameObject.hpp (core-platform/deps/game_types.hpp).
//  * Vtable slots are written as the virtual members they are: +0x98 Interact, +0x28 IsPlayer, +0x24 IsCharacter.

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ItemInstance.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/ItemManager.hpp"
#include "dh2/ItemObject.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/Random.hpp"
#include "dh2/SWFAnimToolTip.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/Vec3f_K.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"   // Arrays::ItemAudioVisualTable, Arrays::TrophyTable
#include "pydata-accessors/w7_pydata12.hpp"    // PyDataConstants::getConstant
#include "pydata-accessors/w7_structs_2.hpp"   // Structs::Item
#include "pydata-accessors/w7_structs_3.hpp"   // Structs::ItemAudioVisual
// gAssertLevel: declared by w7_pydata12.hpp. CustomFree: the inline in pydata-accessors/deps/stream_reader.hpp (the same
// call the original makes). core-platform/memory.hpp is not included: its glitch_core copy clashes with VisualObject.hpp.

// PlayerManager reads, defined in g1-game-items-save-ItemObject-eb7cc3-1_players.cpp.
namespace ItemObjectPlayers {
Character* QuestHolderOfPlayer(int friendlyId);
int NumPlayerCharacters();
int PlayerNumberOf(const Character* character);
Character* LocalPlayerCharacter();
bool IsLocalPlayerCharacter(const Character* character);
void IncrementStatFour(int playerNumber);
}  // namespace ItemObjectPlayers

namespace {

// ItemObject.cpp: the file name the original's ASSERT prints.
const char kItemObjectSource[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Items\\ItemObject.cpp";

// The original's ASSERT(expr) at ItemObject.cpp:line. Level 2 stops the program, level 1 prints the line and goes on.
void ReportItemObjectAssert(const char* expr, int line) {
    if (gAssertLevel == 2) {
        std::abort();  // the original writes to address 0 here
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, kItemObjectSource, line);
    }
}

// ItemObject::_DoAutoPickupHack / DropInventory: 0x3ec504's pickup-lock values. DropInventory sets the lock to 5000 ms.
constexpr std::int16_t kDropPickupLockMs = 5000;

// Interact: the item-type value of the potion stack (ItemInventory::m_potion is the item of type 14).
constexpr int kPotionItemType = 14;

// Interact: the Character's gold/inventory counter for the "picked up 300 drops" trophy.
constexpr int kPickedUpCounterProp = 223;

// Interact: the Character property that, when it reaches 300, unlocks the "picked_up_300_drops" trophy.
constexpr int kPickedUp300Threshold = 300;

// Point3D<float>::normalize (0x34d0b0): divides each component by the Euclidean length. Operator/= (0x34d04c) divides
// the three components in order; the length is sqrtf(((x*x) + (y*y)) + (z*z)) in the binary's order.
void NormalizeDirection(float& x, float& y, float& z) {
    const float length = std::sqrt((x * x + y * y) + z * z);
    x = x / length;
    y = y / length;
    z = z / length;
}

// PARTIAL. _GetNextPlayerQuestRRId walks the quest-gathered item list of the holder (Character +0x3ac, the
// ItemInventory::m_activeQuestGatheredItemIds list) and compares the ItemObject pointer `this` with each node's payload
// word (node +0x08). The ItemInventory header types that payload as QuestGatheredItem::m_itemId (an int item id), so the
// type of the compared value is unresolved. The match is not reconstructed: no holder matches here.
bool QuestListHoldsItem(const Character* holder) {
    (void)holder;
    return false;
}

}  // namespace

// 0x3ec320 ItemObject::DeclareProperties()
// Thunk to GameObject::DeclareProperties (0x38cee8). TRACK-B decision 4: the thunk is forwarded, not reimplemented.
void ItemObject::DeclareProperties() {
    GameObject::DeclareProperties();
}

// 0x3ec324 ItemObject::ItemObject(ObjectBase::GO_IDS) (C1). 0x3ec3cc (C2) has the same body; one definition covers both.
// The vtable stores (primary, PropertyMap +0x04, ISerializable +0x24, ItemInventory +0x374) are the compiler's.
ItemObject::ItemObject(ObjectBase::GO_IDS id)
    : GameObject(id), ItemInventory() {
    m_audioVisualIndex = -1;       // +0x3ac (-1 until InitOnce)
    m_speed = 2.5f;                // +0x3b0 (1075838976 = 0x40200000)
    m_spawnSoundId = -1;           // +0x3b4
    m_pickupSoundId = -1;          // +0x3b6
    m_pickupLockMs = -1;           // +0x3b8
    m_receiver = nullptr;          // +0x3bc
    m_pickupLockRRId = -1;         // +0x3c0
    m_tooltipCharacter = nullptr;  // +0x3c4
    m_tooltip = nullptr;           // +0x3c8
    m_glowFX = nullptr;            // +0x3cc
    unk_85 = 1;                    // +0x85 (ObjectBase updating flag, setUpdating stores here)
    m_zoningEnabled = 0;           // +0x2ee (GameObject::m_zoningEnabled, GameObject's own default is 1)
}

// 0x3ecd54 ItemObject::~ItemObject() (D1). 0x3ece00 (D2) has the same body; one definition covers both.
// The members are destroyed after the body: ItemInventory (+0x374) and GameObject, in that order.
ItemObject::~ItemObject() {
    if (m_tooltip != nullptr) {
        m_tooltip->~SWFAnimToolTip();
        CustomFree(m_tooltip);
        m_tooltip = nullptr;
    }
    HideGlow();
}

// 0x3ecde4 ItemObject::~ItemObject() (D0, deleting destructor): compiler generated. The original is ~ItemObject() followed
// by CustomFree(this). No body is written; the host delete expression produces it.

// 0x3ece80 ItemObject::InitOnce(int)
void ItemObject::InitOnce(int audioVisualIndex) {
    m_audioVisualIndex = static_cast<std::int16_t>(audioVisualIndex);  // +0x3ac
    m_dae = "data/3D/GameObjects/itemdrops.bdae";                     // +0x290 (GameObject::m_dae)
    m_speed = 6.0f;                                                   // +0x3b0 (1086324736 = 0x40C00000)

    if (audioVisualIndex < 0 ||
        static_cast<std::uint32_t>(audioVisualIndex) >= Arrays::ItemAudioVisualTable::size) {
        m_xrefObject = "dummy_itemdrop_bag";                          // +0x2a8 (GameObject::m_xrefObject)
        GameObject::InitPost();
        if (m_visualObject != nullptr) {                              // +0x2d8
            m_visualObject->ApplyMeshBox();
        }
        return;
    }

    // Row stride is the host size of Structs::ItemAudioVisual (decision 3); the mesh name is the row's m_w4 (+0x10).
    const Structs::ItemAudioVisual* row =
        reinterpret_cast<const Structs::ItemAudioVisual*>(Arrays::ItemAudioVisualTable::members) + audioVisualIndex;
    const char* name = static_cast<const char*>(row->m_w4);
    m_xrefObject.assign(name, std::strlen(name));                     // +0x2a8
    GameObject::InitPost();
    if (m_visualObject != nullptr) {
        m_visualObject->ApplyMeshBox();
    }
}

// 0x3ec474 ItemObject::_DoAutoPickupHack(ItemObject*, GameObject const*, GameObject const*)
// The header names the second argument `dropper`, the third `killer`. The body reads only the second: it calls Interact
// on it when the item's pick-up type is "Automatic". DropAndAwardLoot passes its killer in that slot. The first and third
// arguments are not read by the body (the original's register r1 and r3 values are never used).
// The original's `this == nullptr` test is dropped: this is never null in a member call.
// The original returns the PickUpType constant in r0 on this path; no caller reads it, so the function returns void.
void ItemObject::_DoAutoPickupHack(ItemObject* /*unk_item*/, const GameObject* dropper, const GameObject* /*killer*/) {
    ItemInstance* item = ItemInventory::GetItem(0);
    if (item != nullptr) {
        const int pickUpType = item->GetPickUpType();
        const int automatic = Singleton<Application>::s_inst.m_pyDataConstants->getConstant("PickUpType", "Automatic");
        if (pickUpType == automatic && dropper != nullptr) {
            Interact(const_cast<GameObject*>(dropper));  // vtable +0x98
        }
    }
}

// 0x3ec504 ItemObject::_GetNextPlayerQuestRRId(int)
// PARTIAL. The round-robin search is reconstructed: it reads the player count (PlayerManager +0x6c4), the player at the
// round-robin index, and the holder's Character (player record +0x660), and it updates s_playerQuestRRId as the pseudocode
// does. The match on each holder's quest list is unresolved (QuestListHoldsItem, see there), so no holder matches.
// The exit test compares the index with the global s_playerQuestRRId, which the loop does not change, so the loop checks
// one player and stops, as the pseudocode does. With no player characters the modulo is by zero, as in the original.
// The argument is unread in the pseudocode.
Character* ItemObject::_GetNextPlayerQuestRRId(int /*unk_arg*/) {
    const int count = ItemObjectPlayers::NumPlayerCharacters();
    int playerIndex = s_playerQuestRRId;
    for (;; playerIndex = (playerIndex + 1) % count) {
        Character* holder = ItemObjectPlayers::QuestHolderOfPlayer(playerIndex);
        if (QuestListHoldsItem(holder)) {
            s_playerQuestRRId = (playerIndex + 1) % count;
            return holder;
        }
        if (playerIndex == s_playerQuestRRId) {
            break;
        }
    }
    s_playerQuestRRId = (playerIndex + 1) % count;
    return nullptr;
}

// 0x3ec668 ItemObject::_GetRandomDropPos(Point3D<float>&, GameObject const*, GameObject const*)
// With a target, the drop point is origin + dir * (150 + rand(200)) + (dir x K) * (rand(300) - 150), where dir is the
// normalised (target - origin) and K = Vec3f_K = (0, 0, 1). Without a target, x and y move by rand(500) - 250 each and z
// keeps the origin's value. Random::GetRandom(range, false) is the ordinary stream (clone .clone.17 uses s_seed); the
// ranges 200, 300 and 500 are the immediates of the three calls (IDA prints them as dword_C8 and elf_hash_bucket+N, with
// elf_hash_bucket at 0xDC).
// The return value is the x component on the target path and the new y on the other path, as the pseudocode returns.
float ItemObject::_GetRandomDropPos(Point3D<float>& position, const GameObject* origin, const GameObject* target) {
    if (target != nullptr) {
        float dirX = target->m_position.X - origin->m_position.X;
        float dirY = target->m_position.Y - origin->m_position.Y;
        float dirZ = target->m_position.Z - origin->m_position.Z;
        NormalizeDirection(dirX, dirY, dirZ);

        const float kX = Vec3f_K.X;
        const float kY = Vec3f_K.Y;
        const float kZ = Vec3f_K.Z;

        const float distance = static_cast<float>(static_cast<int>(Random::GetRandom(200, false)) + 150);
        const float scaledX = dirX * distance;
        const float scaledY = dirY * distance;
        const float scaledZ = dirZ * distance;

        const float lateral = static_cast<float>(static_cast<int>(Random::GetRandom(300, false)) - 150);
        const float y = (lateral * ((dirZ * kX) - (kZ * dirX)) + scaledY) + origin->m_position.Y;
        const float x = (lateral * ((dirY * kZ) - (dirZ * kY)) + scaledX) + origin->m_position.X;
        const float z = (lateral * ((kY * dirX) - (dirY * kX)) + scaledZ) + origin->m_position.Z;

        position.X = x;
        position.Y = y;
        position.Z = z;
        return x;
    }

    position.X = origin->m_position.X;
    position.Y = origin->m_position.Y;
    position.Z = origin->m_position.Z;
    position.X = static_cast<float>(static_cast<int>(Random::GetRandom(500, false)) - 250) + position.X;
    const float y = position.Y + static_cast<float>(static_cast<int>(Random::GetRandom(500, false)) - 250);
    position.Y = y;
    return y;
}

// 0x3ec8a0 ItemObject::DropAndAwardLoot(ItemInventory&, GameObject const*, GameObject const*, int)
// Spawns each item of the inventory at a random drop point from the dropper, owned by the local player's character, and
// gives it to the killer through _DoAutoPickupHack. The fourth argument is not read by the pseudocode.
void ItemObject::DropAndAwardLoot(ItemInventory& inventory, const GameObject* dropper, const GameObject* killer,
                                  int /*unk_arg*/) {
    Point3D<float> dropPos{};
    while (inventory.GetNumItems() != 0) {
        _GetRandomDropPos(dropPos, dropper, killer);
        (void)inventory.GetItem(0);  // the original calls GetItem here and discards the result
        Character* owner = ItemObjectPlayers::LocalPlayerCharacter();
        ItemObject* item = Singleton<ItemManager>::s_inst.Spawn(inventory, 0, dropper, dropper->m_position, dropPos, owner);
        // The first and third arguments are unread by _DoAutoPickupHack; the original passes its dropper and an
        // uninitialised register there.
        item->_DoAutoPickupHack(nullptr, killer, nullptr);
    }
}

// 0x3ec974 ItemObject::DropInventory(ItemInventory&, GameObject const*, GameObject const*, Character const*)
// Spawns each item of the inventory at a random drop point from the dropper. When the dropper is a Character, each item
// gets a 5000 ms pick-up lock for that character's player number.
void ItemObject::DropInventory(ItemInventory& inventory, const GameObject* dropper, const GameObject* killer,
                               const Character* owner) {
    if (dropper == nullptr) {
        ReportItemObjectAssert("dropper", 255);
        return;  // PORT: the original dereferences the null dropper in the next call
    }
    Point3D<float> dropPos{};
    while (inventory.GetNumItems() != 0) {
        for (;;) {
            _GetRandomDropPos(dropPos, dropper, killer);
            ItemObject* item =
                Singleton<ItemManager>::s_inst.Spawn(inventory, 0, dropper, dropper->m_position, dropPos, owner);
            if (!dropper->IsCharacter()) {
                break;
            }
            const Character* dropperCharacter = static_cast<const Character*>(dropper);
            item->m_pickupLockMs = kDropPickupLockMs;                                    // +0x3b8
            item->m_pickupLockRRId = static_cast<std::int16_t>(                           // +0x3c0
                ItemObjectPlayers::PlayerNumberOf(dropperCharacter));
            if (inventory.GetNumItems() == 0) {
                return;
            }
        }
    }
}

// 0x3ecae4 ItemObject::GetInventoryToDrop(ItemInventory&, int, GameObject const*, int)
// Adds the loot table rows of lootTableId to the inventory. When the dropper is a live (+0xf4 == 0) Character, its two
// CharProperties (195 and 196, read from the m_props sheet at +0xa94) are passed as the item and quantity arguments.
void ItemObject::GetInventoryToDrop(ItemInventory& inventory, int lootTableId, const GameObject* dropper, int unk_arg) {
    if (dropper != nullptr) {
        const ObjectHandle handle = dropper->GetHandle();
        Character* object = static_cast<Character*>(handle.GetObject(false));
        if (object != nullptr && static_cast<int>(object->m_goId) == 0) {  // +0xf4 is ObjectBase::m_goId
            const int property195 = object->_GetProperty(object->m_props, 195);
            const int property196 = object->_GetProperty(object->m_props, 196);
            inventory.AddLoot(lootTableId, property195, property196, unk_arg, false);
            return;
        }
    }
    inventory.AddLoot(lootTableId, 0, 0, unk_arg, false);
}

// 0x3ecba0 ItemObject::DropLootTable(int, GameObject const*, GameObject const*, int, bool)
// The loot drops when the killer is a player-controlled Character (IsPlayer) that is the dropper, or when the dropper is a
// boss or mini boss. The bool argument is not read by the pseudocode.
void ItemObject::DropLootTable(int lootTableId, const GameObject* dropper, const GameObject* killer, int fixedNumPowers,
                               bool /*unk_flag*/) {
    Character* killerCharacter = nullptr;
    if (killer != nullptr) {
        const ObjectHandle handle = killer->GetHandle();
        Character* object = static_cast<Character*>(handle.GetObject(false));
        killerCharacter = object;
        if (object == nullptr || static_cast<int>(object->m_goId) != 0) {
            killerCharacter = nullptr;
        }
    }

    Character* dropperCharacter = nullptr;
    bool drop = false;
    if (dropper != nullptr) {
        const ObjectHandle handle = dropper->GetHandle();
        Character* object = static_cast<Character*>(handle.GetObject(false));
        if (object != nullptr && static_cast<int>(object->m_goId) == 0) {
            dropperCharacter = object;
            drop = dropperCharacter->IsBoss() || dropperCharacter->IsMiniBoss();
        }
    }
    if (!drop && killerCharacter != nullptr &&
        (killerCharacter->IsPlayer() || dropperCharacter == killerCharacter)) {
        drop = true;
    }

    if (drop) {
        ItemInventory loot;
        GetInventoryToDrop(loot, lootTableId, killer, fixedNumPowers);
        DropAndAwardLoot(loot, dropper, killer, fixedNumPowers);
    }
}

// 0x3eccc8 ItemObject::DropLootTable(int, GameObject const*, GameObject const*, Character const*, int)
// Rolls the loot table into a local inventory and drops it at the dropper for the owner Character.
void ItemObject::DropLootTable(int lootTableId, const GameObject* dropper, const GameObject* killer,
                               const Character* owner, int unk_arg) {
    if (killer != nullptr) {
        // The original looks the killer's handle up and drops the result. The lookup is kept; its value is not used.
        (void)killer->GetHandle().GetObject(false);
    }
    ItemInventory loot;
    GetInventoryToDrop(loot, lootTableId, killer, unk_arg);
    DropInventory(loot, dropper, killer, owner);
}

// 0x3ed144 ItemObject::Interact(GameObject*)
// PARTIAL. The gating, the item choice, the inventory transfer, the trophy and the pick-up of the item are reconstructed.
// Not reconstructed (each is named at its point below):
//  (a) the message texts (font colour from Arrays::FontPalette, the item name, StringManager parse) and the StatusMsg queue
//      (MenuMessageManager<StatusMsg,4>::EnqueueMessage; dh2/StatusMsg.hpp is absent and the pseudocode's one-argument call
//      has no declared overload);
//  (b) the tutorial script start (its online test reads COnline::isOnlineGame_ at +0x05, a private member with no accessor;
//      the savegame byte at +0x2a is SavegameManager::m_tutorials[1], not named);
//  (c) the online CMsgControllerAction message (CMessaging / CMessage / COnline layout);
//  (d) VoxSoundManager::Play3D (not declared in vox-audio);
//  (e) the loot_orb_fx effect id: the AnimatedEffectTable name search is not reconstructed, so no effect plays.
void ItemObject::Interact(GameObject* interactor) {
    if (HasBeenLooted()) {
        return;
    }
    ObjectHandle interactorHandle = interactor->GetHandle();
    Character* character = interactorHandle.operator Character*();
    if (character == nullptr) {
        return;
    }
    if (m_receiver != nullptr && character != m_receiver) {
        return;
    }
    if (m_pickupLockRRId != -1) {
        if (m_pickupLockRRId == ItemObjectPlayers::PlayerNumberOf(character) && m_pickupLockMs > 0) {
            return;
        }
    }
    if (!character->IsPlayer()) {
        return;
    }

    ItemInventory& charInventory = *character;
    ItemInstance* item = ItemInventory::GetItem(0);
    const int savedOption = Singleton<Application>::s_inst.GetSavedOption("AutoTransmute");
    const Structs::Item* row = item->GetItem();
    const bool pickUpTypeUnset = row->m_w26 == -1;   // Structs::Item +0x68
    const int itemType = row->m_w22;                 // Structs::Item +0x58

    // The pick-up step of the original (the LABEL_14 block). The potion stack is only taken while it has room.
    auto pickUp = [&]() {
        if (itemType != kPotionItemType || charInventory.GetNumPotions() < charInventory.m_maxPotions) {
            // PARTIAL (a): the pick-up message text and StatusMsg queue, and the tutorial start (b), are not reconstructed.
            DebugSwitches::s_inst.load();
            (void)DebugSwitches::s_inst.GetSwitch(std::string("isTracingItemPickedUp"));
            TransferInventoryTo(charInventory, false, true);
            character->PROPS_AddInt(kPickedUpCounterProp, 1);
            if (character->PROPS_GetInt(kPickedUpCounterProp, false) >= kPickedUp300Threshold &&
                character->IsPlayer() && ItemObjectPlayers::IsLocalPlayerCharacter(character)) {
                // PARTIAL (f): the original searches Arrays::TrophyTable::m_memberNames for "picked_up_300_drops" and
                // unlocks that row (TrophyManager::UnlockTrophy). The name table is in member_names.hpp, which clashes with
                // the element tables (w7_arrays_00.hpp) and with PlayerManager.hpp in any translation unit that this batch
                // can use, so the unlock is not made.
            }
        }
    };

    if (pickUpTypeUnset) {
        pickUp();
    } else if (static_cast<unsigned int>(item->GetNumPowers()) < static_cast<unsigned int>(savedOption)) {
        // Auto-transmute: the item is turned into gold and the gold is handed to the character.
        // PARTIAL: the message text (StrID ITEMS_AUTO_TRANSMUTE, StringManager::parse with the item name) is not reconstructed.
        const int gold = character->INV_TransmuteItem(item, true);
        (void)gold;
        const unsigned int transferred = static_cast<unsigned int>(TransferItemTo(0, charInventory, false, true));
        character->INV_TransmuteItem(transferred, false);
    } else if (!charInventory.IsInventoryFull()) {
        pickUp();
    } else {
        // PARTIAL (a): the GAMEPLAYMENUS_INVENTORY_FULL message is not queued (see the note above).
    }

    // LABEL_21: the item is gone when the inventory is empty, and only then is the object removed.
    if (HasBeenLooted()) {
        // PARTIAL (c), (d): the online CMsgControllerAction and the Play3D pick-up sound are not reconstructed here.
        if (m_tooltip != nullptr) {  // +0x3c8
            m_tooltip->~SWFAnimToolTip();
            CustomFree(m_tooltip);
            m_tooltip = nullptr;
        }
        HideGlow();
        if (character->m_objectOfInterest == this) {  // Character +0x14a4
            character->m_objectOfInterest = nullptr;
        }
        // PARTIAL (e): VisualFXManager::PlayAnimFXSet(loot_orb_fx, character position) is not called (see the note above).
        Singleton<ItemManager>::s_inst.DeSpawn(this);
        ItemObjectPlayers::IncrementStatFour(ItemObjectPlayers::PlayerNumberOf(character));
    }
}
