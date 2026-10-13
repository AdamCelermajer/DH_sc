// Batch g1-game-character-Character-ee9946-3: part 4 of the Character functions (state FX, spot target, interaction
// queries, inventory transmute and requirements, equipment, ranged attack, respawn, update gating, animation stance).
// Declarations come from dh2/Character.hpp. Partial bodies name the missing field, singleton or callee in a PARTIAL
// comment; the ledger rows in game-character/ledger.d/g1-game-character-Character-ee9946-3.csv repeat it, with the
// layout requests for the owner classes.

#include <cstdint>
#include <cstring>

#include "dh2/Character.hpp"
#include "dh2/ItemInstance.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/charai_ai.hpp"
#include "pydata-accessors/w7_pydata12.hpp"
#include "pydata-accessors/w7_structs_2.hpp"

namespace {

// CharProperties::PROPS_GetWalkSpeed (and the other float getters) return the bit pattern of a float. The bits are
// reinterpreted here, not converted.
float FloatFromBits(int bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

// The original does 32-bit wrapping integer arithmetic on these values. The helpers wrap the same way without signed
// overflow in C++.
std::int32_t WrapMul(std::int32_t a, std::int32_t b) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}

std::int32_t WrapShl(std::int32_t a, unsigned int n) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) << n);
}

std::int32_t WrapAdd(std::int32_t a, std::int32_t b) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
}

// PARTIAL placeholders. Each stands for state the tree does not expose yet; the value returned is the one the
// reconstructed branch sees when the bit or word is taken as clear or unset. See the ledger notes for each.

// CharStateMachine::m_stateFlags (+0x24, private to CharStateMachine, Character +0x520). UpdateStateFX tests bits
// 0x400 (scare) and 0x800 (stun); IsInteractive tests bit 0x2000. Not reachable from Character: taken as 0.
std::uint32_t CharStateMachineFlags() {
    return 0;
}

// COnline::isOnlineGame_ (+0x05, private, no getter). Taken as offline.
bool OnlineGameFlag() {
    return false;
}

// Arrays::DesignSettingsTable FX ids, read at byte offsets 116, 124 and 128 of the table members. The pydata Arrays
// element ABI is not ported, so no id is read. -1 means no FX.
int DesignSettingsFxId(unsigned int byteOffset) {
    (void)byteOffset;
    return -1;
}

}  // namespace

// 0x3a4470 _ZN9Character13UpdateStateFXEv
void Character::UpdateStateFX() {
    // PARTIAL: the state FX object m_stateFX (+0x148c) is not swapped. The original drops it and grabs the new AnimatedFX
    // through Singleton<VisualFXManager>::s_inst, sets its owner (+0x28), copies the position, makes it visible and
    // drives its animator with two virtual calls (+0x44, +0x40). None of that can be made: there is no VisualFXManager
    // instance accessor and no ISceneNodeAnimator declaration. The state category is still stored.
    if (IsDead()) {
        return;
    }
    // +0x1490 holds the state category (0 none, 1 slowed, 3 scared, 4 stunned), not an FX id as the header comment says.
    // Layout request: rename m_stateFXId to m_stateFXState (see the ledger).
    const int previous = m_stateFXId;
    struct Choice {
        int category;
        int fxId;
    };
    Choice choice{0, -1};
    if (previous > 4) {
        choice = {0, -1};
    } else if (CharStateMachine::SM_IsStunned(true) && (CharStateMachineFlags() & 0x800u) != 0) {
        choice = {4, DesignSettingsFxId(128)};
    } else if (previous > 3) {
        choice = {0, -1};
    } else if (CharStateMachine::SM_IsScared(true) && (CharStateMachineFlags() & 0x400u) != 0) {
        choice = {3, DesignSettingsFxId(116)};
    } else if (previous > 1 || !(FloatFromBits(CharProperties::PROPS_GetWalkSpeed()) < 1.0f)) {
        choice = {0, -1};
    } else {
        choice = {1, DesignSettingsFxId(124)};
    }
    // The original takes the none path (category 0) whenever the FX id is -1.
    if (choice.fxId == -1) {
        choice.category = 0;
    }
    if (m_stateFXId != choice.category) {
        m_stateFXId = choice.category;
    }
}

// 0x3a469c _ZN9Character16UpdateSpotTargetEv
void Character::UpdateSpotTarget() {
    // PARTIAL: the spot FX m_spotTargetFX (+0x14cc) is grabbed and dropped through Singleton<VisualFXManager>::s_inst,
    // and the queued position is checked with PFWorld::ValidatePosition on Singleton<PFWorld>::s_inst. Neither singleton
    // has an instance accessor in the tree. The AnimatedFX position words (unk_34..unk_3c) are unnamed. Only the
    // branches that need no call are reproduced, and no call is made in the others.
    if (m_spotTargetFX == nullptr) {
        // Original: when m_spotTargetPending is set, grab the FX (m_spotTargetFXId) and validate the position. Not made.
        return;
    }
    if (!m_spotTargetPending) {
        // Original: DropAnimatedFX(&Singleton<VisualFXManager>::s_inst, &m_spotTargetFX), then return. Not made.
        return;
    }
    // Original: ValidatePosition(m_spotTargetPos). When it fails, m_spotTargetPos becomes m_spotTargetValidPos. When it
    // passes, the valid position takes m_spotTargetPos, the FX position takes it too and AnimatedFX::SyncIrrData(false)
    // runs. Not made.
}

// 0x3a47e8 _ZNK9Character18GetInteractionTypeEP10GameObject
int Character::GetInteractionType(GameObject* other) const {
    if (other != nullptr && CharAI::AI_IsEnemy(other) && !other->IsPlayer()) {
        return 8;
    }
    if (IsSummoned() || IsDead()) {
        return -1;
    }
    if (IsMonster()) {
        return 8;
    }
    return 3;
}

// 0x3a4870 _ZNK9Character13IsInteractiveEP10GameObject
bool Character::IsInteractive(GameObject* other) const {
    if (IsDead() && other != nullptr && CharAI::AI_IsFriend(other) && !IsMonster()) {
        return true;
    }
    if (m_markedForDeletion != 0 || m_enabled == 0 || IsFaerie() || IsSummoned() || IsDead() ||
        (CharStateMachine::m_stateFlags & 0x2000u) == 0) {
        return false;
    }
    return CharAI::unk_4d;
}

// 0x3a4930 _ZN9Character24INV_DoesMeetRequirementsEP12ItemInstance
// PARTIAL: the original returns true for a remotely updated character of an online game (COnline::isOnlineGame_ at +0x05,
// private with no getter; OnlineGameFlag() is taken offline here). Otherwise it compares the five requirement words of the
// item record (Structs::Item m_w29..m_w33, +0x74..+0x84) with five words of this character's final sheet, each shifted
// right by 8: m_props.m_w20 (+0x1044) and m_props.m_w150..m_w153 (+0x124c..+0x1258).
bool Character::INV_DoesMeetRequirements(ItemInstance* item) {
    if ((OnlineGameFlag() && IsRemotelyUpdated()) || item == nullptr) {
        return true;
    }
    const Structs::Item* record = item->GetItem();
    return record->m_w29 <= (m_props.m_w20 >> 8) && record->m_w30 <= (m_props.m_w150 >> 8) &&
           record->m_w31 <= (m_props.m_w151 >> 8) && record->m_w32 <= (m_props.m_w152 >> 8) &&
           record->m_w33 <= (m_props.m_w153 >> 8);
}

// 0x3a49f0 _ZNK9Character8IsPlayerEv
bool Character::IsPlayer() const {
    const int charType = GetCharType();
    if (charType != 0) {
        return charType == 1;
    }
    // PARTIAL: the original reads a char* at +0x44 and tests strstr(name, "PlayerCharacter") == name. That pointer is
    // not a declared member (ObjectBase::m_name is a std::string at +0x30), so the object's name is read from m_name.
    const char* name = m_name.c_str();
    return std::strstr(name, "PlayerCharacter") == name;
}

// 0x3a4a3c _ZN9Character17INV_TransmuteItemEP12ItemInstanceb
int Character::INV_TransmuteItem(ItemInstance* item, bool a) {
    if (item == nullptr) {
        return 0;
    }
    // Word 197 is read from the final sheet. PROPS_Get(id, false) reads m_props, which is the word _GetProperty(m_props,
    // id) returns in the original.
    const std::int32_t property = CharProperties::PROPS_Get(197, false);
    const std::int32_t multiplier =
        ApplicationGetPyDataConstants()->getConstant("CharacterDesign", "TransmuteMultiplier");
    // ItemInstance +0x54 is m_value.
    const std::int32_t scaled = WrapMul(WrapShl(item->m_value, 8), WrapAdd(property, 256)) >> 8;
    std::int32_t gold = WrapMul(multiplier, scaled) >> 16;
    if (gold < 1) {
        gold = 1;
    }
    if (!a) {
        // ItemInstance +0x50 is m_qty.
        if (item->m_qty <= 1) {
            ItemInventory::RemoveItem(static_cast<unsigned int>(ItemInventory::GetItemIndex(item)));
        } else {
            item->AddQty(-1);
        }
        ItemInventory::AddGold(gold);
        CharProperties::PROPS_AddInt(213, 1);
        // PARTIAL: the original runs the gear_transmute trophy test here: when PlayerManager::IsLocalPlayer(dword_99F76C,
        // this) holds, PROPS_GetInt(213) >= 300 and IsPlayer(), it calls TrophyManager::s_instance->UnlockTrophy with
        // Arrays::GetMemberIDByString<Arrays::TrophyTable>("gear_transmute"). dh2/PlayerManager.hpp cannot be included in
        // this translation unit (its StreamBuffer member is abstract next to pydata's IStreamBase), so the local-player
        // test is not made and the trophy is not unlocked.
    }
    return gold;
}

// 0x3a4bac _ZNK9Character15GetRespawnDelayEv
int Character::GetRespawnDelay() const {
    // PROPS_GetInt(11, false) is the final-sheet word 11 shifted right by 8, as _GetProperty(m_props, 11) >> 8 is.
    if (m_isSpawned != 0) {
        return 0;
    }
    return 1000 * CharProperties::PROPS_GetInt(11, false);
}

// 0x3a4bec _ZN9Character17INV_TransmuteItemEjb
int Character::INV_TransmuteItem(unsigned int item, bool a) {
    ItemInstance* instance = ItemInventory::GetItem(item);
    return INV_TransmuteItem(instance, a);
}

// 0x3a4c14 _ZN9Character17EquipAllSlotsAutoEv
int Character::EquipAllSlotsAuto() {
    const int numSlots = ItemInventory::GetNumEquipmentSlots();
    // The original keeps its r0 in one register: the last call result. Its value is the result of the slot 0 step
    // (the last one), or the slot count when there are no slots.
    int result = numSlots;
    if (numSlots > 0) {
        for (int slot = 0; slot < numSlots; ++slot) {
            result = UnEquipItemFromSlot(static_cast<unsigned int>(slot));
        }
        for (int slot = numSlots - 1; slot >= 0; --slot) {
            EquipSlotAuto(static_cast<unsigned int>(slot));
        }
        for (int slot = numSlots - 1; slot >= 0; --slot) {
            const bool taken = ItemInventory::IsEquipmentSlotTaken(static_cast<unsigned int>(slot));
            result = static_cast<int>(taken);
            // The original leaves r0 as the object pointer for slots 1 and 2 when they are free. That value is always
            // overwritten by the slot 0 step, so the integer stays here.
            if (!taken && slot != 1 && slot != 2) {
                result = EquipSlotAuto(static_cast<unsigned int>(slot));
            }
        }
    }
    return result;
}

// 0x3a4cc0 _ZN9Character15AddItemInstanceEP12ItemInstancebb
int Character::AddItemInstance(ItemInstance* item, bool a, bool b) {
    return ItemInventory::AddItemInstance(item, a, b);
}

// 0x3a4cd0 _ZNK9Character14CanRangeAttackERiS0_S0_
// The ranged override is the final-sheet word m_props.m_w33 (+0x1078, -1 = none). Without an override the ItemInventory
// query runs. With one, a and b take m_props.m_w31 and m_w32 (+0x1070, +0x1074) shifted right by 8, c takes m_w33, and the
// result is true.
bool Character::CanRangeAttack(int& a, int& b, int& c) const {
    if (m_props.m_w33 == -1) {
        return ItemInventory::CanRangeAttack(a, b, c);
    }
    a = m_props.m_w31 >> 8;
    b = m_props.m_w32 >> 8;
    c = m_props.m_w33;
    return true;
}

// 0x3a4d3c _ZNK9Character14CanRangeAttackEv
bool Character::CanRangeAttack() const {
    if (m_props.m_w33 == -1) {
        return ItemInventory::CanRangeAttack();
    }
    return true;
}

// 0x3a4d5c _ZN9Character10RaiseEventEiPv
// PARTIAL: event 54 returns what CharProperties::BuffExpired returns. In the binary BuffExpired (0x3e123c) is an int function
// that ends with "return PROPS_DelBuff(...)", but dh2/CharProperties.hpp declares BuffExpired (line 81) and PROPS_DelBuff
// (line 79) void, so that value is not defined in this tree. Layout request: CharProperties::BuffExpired and PROPS_DelBuff
// return int. 0 is returned here.
int Character::RaiseEvent(int eventId, void* data) {
    if (eventId == 54) {
        CharProperties::BuffExpired(static_cast<CharTimers::Timer*>(data));
        return 0;
    }
    return CharAI::RaiseAIEvent(eventId, data);
}

// 0x3a4d78 _ZN9Character8InteractEP10GameObject
void Character::Interact(GameObject* other) {
    if (!CharStateMachine::SM_IsInteracting()) {
        // PARTIAL: the original raises the TalkToNPC quest event on the current level before it sets the interact state.
        // The level comes from Application::GetCurrentLevel on Singleton<Application>::s_inst, which has no instance
        // accessor in the tree, and the event object (IEvent vtable off_965E98, 0x14 bytes) is not declared. Also the
        // original reports a missing level through gAssertLevel (2 writes address 0 to crash, 1 prints). No event is
        // raised here.
    }
    CharStateMachine::SM_SetInteractState(3, true, other, false);
    // GameObject unk_2fa (+0x2fa) is the byte the original tests with the Character at +0x2fa.
    if (unk_2fa == 0) {
        RaiseEvent(5, other);
        if (IsMerchant()) {
            if (HasLoot() && other != nullptr) {
                // PARTIAL: the original resolves the other object to its Character through its ObjectHandle. When the
                // merchant list is empty it adds the Arrays::MerchantTable loot rows (rows not decoded yet), then calls
                // RenderFX::InvokeASCallback with "openMerchantMenu" and "AdditionnalInfosForMerchant" on the HUD root,
                // with the owner's player number (PlayerInfo +0x678, PlayerInfo not declared). Not made.
            }
        } else if (IsCleaner()) {
            if (other != nullptr) {
                // PARTIAL: the original calls RenderFX::InvokeASCallback "openCleanerMenu" on the HUD root with the
                // owner's player number (PlayerInfo +0x678). Not made: no RenderFX callback and no PlayerInfo.
            }
        }
    }
}

// 0x3a5184 _ZN9Character16GetImmunityTimerEi
TimerUtil* Character::GetImmunityTimer(int damageType) {
    // PARTIAL: the original indexes a TimerUtil* table with 4-byte stride from +0x14e0 (word damageType + 1336), creates
    // the TimerUtil when the entry is null and stores it there. That table overlaps the members from +0x14e4 on
    // (m_isNetSpawned, m_savegame, ...), so it cannot be typed on this layout, and the lazily created timer is not
    // stored. The pseudocode also shows the assert block without its condition, so that block is not reproduced.
    (void)damageType;
    return nullptr;
}

// 0x3a5248 _ZNK9Character10CanRespawnEv
bool Character::CanRespawn() const {
    if (m_isSpawned != 0 || PROPS_Get(11, false) <= 0) {
        return false;
    }
    if (CharAI::m_group != nullptr) {
        return CharAI::m_group->CanRespawn(CharAI::m_character);
    }
    return true;
}

// 0x3a52a4 _ZN9Character9CanUpdateEv
int Character::CanUpdate() {
    // The root node of the visual object (VisualObject m_node, +0x08) carries the cull-before-update flag (node +0x200,
    // RootSceneNode m_cullBeforeUpdate). The original clears it first and sets it at the end when the character is visible.
    RootSceneNode* root = (m_visualObject != nullptr) ? m_visualObject->m_node : nullptr;
    if (root != nullptr) {
        root->m_cullBeforeUpdate = false;
    }
    // PARTIAL: on the path where the character is not online-updated the original also returns 0 when the master and
    // local-player test (CharAI m_master at +0x50 against the local player's PlayerInfo +0x660; PlayerManager.hpp does not
    // compile in this unit) and the culling test (ObjectBase::TestCullingBeforeUpdate needs an aabb<float>, forward-declared
    // only here) both fail, and when the root node word +0x118 (glitch CRootSceneNode base, not restated) or unk_2fc is
    // zero. Those are not made. The online flag (COnline isOnlineGame_, private) is taken offline. The dead-and-hidden test
    // and the return values are reproduced.
    if (!OnlineGameFlag() || !IsRemotelyUpdated()) {
        if (IsDead() && !m_visible && !CanRespawn()) {
            return 0;
        }
    }
    if (root != nullptr && m_visible) {
        root->m_cullBeforeUpdate = true;
    }
    return 1;
}

// 0x3a53e0 _ZNK9Character13GetAnimStanceEv
int Character::GetAnimStance() const {
    // Stance ids: 3 staff, 4 bow, 2 dual wield, 1 two-hander, 5 no main-hand weapon, 0 none (null in the original).
    int stance = 0;
    if (IsPlayer()) {
        const ItemInventory& inventory = *this;
        if (inventory.HasStaff()) {
            stance = 3;
        } else if (inventory.HasBow()) {
            stance = 4;
        } else if (inventory.IsDualWielding()) {
            stance = 2;
        } else if (inventory.HasTwoHander(false)) {
            stance = 1;
        } else if (!inventory.HasMainHandWeapon()) {
            stance = 5;
        } else {
            stance = 0;
        }
    }
    const int count = ApplicationGetPyDataConstants()->getConstant("AnimStances", "COUNT_IPHONE");
    if (stance >= count) {
        return 0;
    }
    return stance;
}

// 0x3a54b4 _ZNK9Character22GetCharUniqueAnimSetIdEv
int Character::GetCharUniqueAnimSetId() const {
    const int animTableId = GetCharAnimTableId();
    return GetCharSkillListId() | static_cast<int>(static_cast<std::uint32_t>(animTableId) << 8);
}
