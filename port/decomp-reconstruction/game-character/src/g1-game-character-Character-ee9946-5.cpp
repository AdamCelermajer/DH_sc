// Batch g1-game-character-Character-ee9946-5: the Character destructor, the script-process unload, the constructor,
// the equipment wrappers and requirement check, the skill reload, the property declaration, the object-of-interest
// update, the cached-OID insertion, the hit handler, the draw overlay and the skin refresh.
// Declarations come from dh2/Character.hpp. Partial bodies name what is missing in a PARTIAL comment; the ledger rows in
// game-character/ledger.d/g1-game-character-Character-ee9946-5.csv repeat it.

#include <cstdint>
#include <map>
#include <string>

#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/v2Controller.hpp"
#include "pydata-accessors/w7_arrays27.hpp"

// 0x3a7b24 Character::UnLoadScriptProcess(std::priv::_Rb_tree_iterator<std::pair<int const, Character*>, ...>, bool)
// The original takes the iterator by value (the ELF symbol has no R). STLport's iterator has a user-provided copy
// constructor, so the ABI passes a pointer to the caller's temporary. When the iterator is end(), the original searches
// s_concurrentAI from the first entry for the one whose value is this, and writes the found entry (or end() when there
// is none) to that temporary. Every original caller passes a fresh end() copy and does not read it again, so the
// write-back is not visible to the caller. It erases the entry when the iterator is not end(), then unloads the CharAI
// script process with the flag.
void Character::UnLoadScriptProcess(std::map<int, Character*>::iterator it, bool force) {
    if (it == s_concurrentAI.end()) {
        auto found = s_concurrentAI.begin();
        while (found != s_concurrentAI.end() && found->second != this) {
            ++found;
        }
        it = found;
    }
    if (it != s_concurrentAI.end()) {
        s_concurrentAI.erase(it);
    }
    CharAI::AIUnLoadScriptProcess(force);
}

// 0x3a7c84 Character::~Character()
// 0x3a7dfc Character::~Character()  (same body: the listing types the Clean argument differently; defined once here)
// The original stores each base's vptr (off_965F38 at +0 ... off_966254 at +0x560), calls Clean(), then destroys
// m_netOut and m_netIn, the strings, CharProperties, CharStateMachine, CharAnimator, CharAI, CharTimers, ItemInventory,
// v2Controllable and GameObject. Members and bases of Character are destroyed by the compiler in reverse declaration
// order, which is that order, so the body is Clean() alone.
Character::~Character() {
    Clean();
}

// 0x3a9340 _ZN9CharacterC2EN10ObjectBase6GO_IDSE  Character::Character(ObjectBase::GO_IDS)  (C2, base-object constructor)
// 0x3aa1b4 _ZN9CharacterC1EN10ObjectBase6GO_IDSE  Character::Character(ObjectBase::GO_IDS)  (C1, complete-object constructor;
// same body, defined once here)
// Reconstructed. The stores at ObjectBase +0x104 (unk_104, the incoming NetStruct pointer, &m_netIn) and at +0x380
// (ItemInventory::m_character, the owner) are now declared in their headers and written here. The original's
// v2Controllable null check (a dead branch: the controllable is this + 0x374) is not reproduced.
Character::Character(GO_IDS id)
    : GameObject(id) {
    // Base constructors of v2Controllable, ItemInventory, CharTimers, CharAI, CharAnimator, CharStateMachine and
    // CharProperties run before this body, as they do in the original.
    unk_28 = 1;
    m_validateDirection = 1;
    unk_85 = 1;
    unk_f8 = 1;

    m_initPostDone = 0;
    m_initFinalDone = 0;
    m_needsNetPlacement = 1;
    unk_1397 = -1;
    m_charPropsId = -1;
    m_charPropsTemplateId = -1;
    m_aiStateVisible = 1;
    m_autoSpawn = 1;
    m_spawnDelay = 0;
    unk_1438 = 0;
    m_lowHpSoundArmed = 1;
    m_dead = 0;
    m_killer = nullptr;
    m_initialPosition = Point3D<float>{};
    m_initialRotation = Point3D<float>{};
    m_savedPoint = Point3D<float>{};
    m_revivePosition = Point3D<float>{};
    m_isDummy = 0;
    m_isSpawned = 0;
    m_selfFX = nullptr;
    m_selfFXBaseId = 0;
    m_stateFX = nullptr;
    m_stateFXId = 0;
    m_targetCircleFX = nullptr;
    m_targetCircleIndex = -1;
    m_cursorFX = nullptr;
    m_multiplayerHighlightFX = nullptr;
    m_objectOfInterest = nullptr;
    m_ooiInteractionType = -1;
    m_ooiRefreshTimer = 0;
    m_ooiPriority = 0;
    m_ooiChanged = 0;
    m_spotTargetPos = Point3D<float>{};
    m_spotTargetValidPos = Point3D<float>{};
    m_spotTargetPending = 0;
    m_spotTargetFXId = -1;
    m_spotTargetFX = nullptr;
    unk_14d0 = 0;
    unk_14d4 = 0;
    unk_14d8 = 0;
    unk_14dc = 0;
    m_timerUtil = nullptr;
    m_isNetSpawned = 0;
    m_netSpawnAcked = 0;
    m_savegame = nullptr;
    unk_14ec = nullptr;
    m_freeMana = 0;
    unk_14f4 = -1;
    unk_14f8 = -1;
    m_combatTextCooldown = -1.0f;
    m_pendingGoldText = -1;
    m_pendingXPText = -1;

    m_netStruct = &m_netOut;
    unk_104 = &m_netIn;
    m_targetList.SetRefObject(this);

    // The original allocates a 16-byte v2Controller (vptr, m_controllable, m_blocked, m_character = 0) and installs it.
    // Its null check on the controllable is dead: the address is this + 0x374 and cannot be null.
    v2Controller* controller = new v2Controller(this);
    v2Controllable::SetController(controller);
    m_controller->m_character = this;

    CharTimers::SetCharacter(this);
    CharAI::SetCharacter(this);
    CharAnimator::SetCharacter(this);
    CharStateMachine::SetCharacter(this);
    CharProperties::SetCharacter(this);
    ItemInventory::m_character = this;
    for (int state = 0; state != 20; ++state) {
        CharStateMachine::RegisterState(state);
    }
}

// 0x3a7de0 Character::~Character()
// Deleting destructor (D0): the compiler generates it for delete. The original calls the destructor above and frees
// the object with CustomFree. Not rebuilt.

// 0x3a7dfc Character::~Character()
// Same body as 0x3a7c84 (see above).

// 0x3a7f10 Character::InterpretIncomingNetStruct(bool)
// PARTIAL. Implemented: the early return for m_isDummy, and the buff removal for a forced update of a monster. Not
// reproduced: the position push to the v2Controllable (virtual slot 28 of m_controller->m_controllable, with six words
// taken from the value words of the incoming NetStruct members at their +0x20 offsets; no value accessor is declared
// for those members), the placement block (IsPlayer, ForceUpdatePosition, the hosting-player check and
// Level::PlaceFaeryAndFollowers), the HP block (the incoming HP value drives PROPS 36 and the revive or kill path,
// with the killer looked up through ObjectManager::GetObjectHandleByNetworkId), the state block (the incoming state
// selects the limbus, pre-spawn or idle state), and the NetStructMember change tracking that clears applied values.
void Character::InterpretIncomingNetStruct(bool force) {
    if (m_isDummy) {
        return;
    }
    if (force && IsMonster()) {
        CharProperties::PROPS_RemoveAllBuffs();
    }
}

// 0x3a84f4 Character::Draw() const
// PARTIAL. The three DebugSwitches are queried in the original order, and each query reloads the switch file first. The
// debug drawing is not reproduced: the melee radius box, the path (PFObject::DBG_DrawPath on the base at +0x1c8) and
// the screen text, which is drawn through a renderer reached from dword_99F73C. The static text-object caches the
// original destroys when a switch is off are not held here.
void Character::Draw() const {
    GameObject::Draw();
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("isDisplayingMeleeRadius")) {
        // PARTIAL: melee radius box not drawn.
    }
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("isDisplayingPath")) {
        // PARTIAL: path not drawn.
    }
    DebugSwitches::GetInstance()->load();
    if (DebugSwitches::GetInstance()->GetSwitch("isDisplayingPositions")) {
        // PARTIAL: position text not drawn.
    }
}

// 0x3a8bc4 Character::HitFor(unsigned int, GameObject*)
// PARTIAL. Implemented: the early return when the character is already dead. Not reproduced: the damage sign choice
// (the GOD_Monster debug switch, the local player, the online flag at GetOnline() +5 and the network mode in
// PlayerManager), the hit-point update through PROPS 36 with the kill or revive branch (Cmd_Kill, the CharAI and state
// checks), the OneShotKill option and its PROPS reset, the low-hp sound through VoxSoundManager, and the power trophies
// (Arrays::GetMemberIDByString on the power_*dam names, TrophyManager::UnlockTrophy, and power_onehit for bosses). The
// original returns the hit target, which the void declaration drops.
void Character::HitFor(unsigned int damage, GameObject* attacker) {
    (void)damage;
    (void)attacker;
    if (IsDead()) {
        return;
    }
    // PARTIAL: see the comment above.
}

// 0x3a999c Character::INV_UpdateSkin()
// PARTIAL. The original walks the nine equipment slots and sets the modular skin of each equipped item, or its weapon
// skin when the category has no modular node, from the item name. The name is the string at +0x50 of the item's
// Arrays::ItemTable row; Structs::Item exposes no named host field for it, so the body is absent.
void Character::INV_UpdateSkin() {
}

// 0x3a9d10 Character::INV_CheckItemsRequirements()
// Walks the equipment slots. An equipped item that no longer meets its requirements is unequipped, which sets the
// changed flag. A changed pass updates the gear properties, runs the check again (the original calls itself), refreshes
// the skin and returns ValidateHPMP's result. An unchanged pass returns the last requirement result.
int Character::INV_CheckItemsRequirements() {
    int result = ItemInventory::GetNumEquipmentSlots();
    const unsigned int slotCount = static_cast<unsigned int>(result);
    if (result == 0) {
        return result;
    }
    unsigned int slot = 0;
    bool unequipped = false;
    do {
        bool reachedEnd = false;
        for (;;) {
            ItemInstance* item = ItemInventory::GetEquippedItem(slot);
            result = INV_DoesMeetRequirements(item);
            if (!result) {
                break;
            }
            if (++slot == slotCount) {
                reachedEnd = true;
                break;
            }
        }
        if (reachedEnd) {
            break;
        }
        const unsigned int failedSlot = slot++;
        ItemInventory::UnEquipItemFromSlot(failedSlot, -1);
        unequipped = true;
    } while (slot != slotCount);
    if (unequipped) {
        CharProperties::UpdateGearsProperties();
        INV_CheckItemsRequirements();
        INV_UpdateSkin();
        return ValidateHPMP();
    }
    return result;
}

// 0x3a9db4 Character::ReloadSkills()
// PARTIAL. The buffs are removed, the savegame skills are loaded, the AI and properties are recalculated, and the
// requirements are checked, as in the original. The IsSpecTime flag is computed from the player level and class ids
// (level above 11, class 0x107, 0x145 or 0x122). The final menu callback is not reproduced: RenderFX::InvokeASCallback
// is not declared in dh2/RenderFX.hpp, and the RenderFX pointer (MenuManager::GetInstance() +0xf4, then +0x138) is not
// declared on MultiMenuManager. The flag is computed and dropped.
void Character::ReloadSkills() {
    CharProperties::PROPS_RemoveAllBuffs();
    SG_Load(32);
    CharAI::AI_ReloadSkills();
    CharAI::UpdateAllSkills();
    CharProperties::RecalcProperties(true);
    INV_CheckItemsRequirements();
    bool specTime = false;
    if (SG_GetPlayerLevel() > 11) {
        const int playerClass = SG_GetPlayerClass();
        specTime = playerClass == 0x107 || playerClass == 0x145 || playerClass == 0x122;
    }
    (void)specTime;
    // PARTIAL: menu callback "_root.menu_CharacterMenu" / "IsSpecTime" not reproduced.
}

// 0x3a9eb0 Character::UnEquipItemFromSlot(unsigned int)
int Character::UnEquipItemFromSlot(unsigned int slot) {
    ItemInventory::UnEquipItemFromSlot(slot, -1);
    CharProperties::UpdateGearsProperties();
    INV_CheckItemsRequirements();
    INV_UpdateSkin();
    return ValidateHPMP();
}

// 0x3a9ee8 Character::EquipSlotAuto(unsigned int)
int Character::EquipSlotAuto(unsigned int slot) {
    const int result = ItemInventory::EquipSlotAuto(slot, this);
    CharProperties::UpdateGearsProperties();
    INV_CheckItemsRequirements();
    INV_UpdateSkin();
    ValidateHPMP();
    return result;
}

// 0x3a9f30 Character::EquipItemToSlot(unsigned int, unsigned int)
int Character::EquipItemToSlot(unsigned int item, unsigned int slot) {
    ItemInventory::EquipItemToSlot(item, slot);
    CharProperties::UpdateGearsProperties();
    INV_CheckItemsRequirements();
    INV_UpdateSkin();
    return ValidateHPMP();
}

// 0x3a9f6c Character::UnEquipItemAuto(unsigned int)
int Character::UnEquipItemAuto(unsigned int item) {
    ItemInventory::UnEquipItemAuto(item);
    CharProperties::UpdateGearsProperties();
    INV_CheckItemsRequirements();
    INV_UpdateSkin();
    return ValidateHPMP();
}

// 0x3a9fa8 Character::EquipItemAuto(unsigned int)
// Unlike the slot wrappers, this one does not run the requirement check.
int Character::EquipItemAuto(unsigned int item) {
    const int result = ItemInventory::EquipItemAuto(item);
    CharProperties::UpdateGearsProperties();
    INV_UpdateSkin();
    ValidateHPMP();
    return result;
}

// 0x3a9fe4 Character::DeclareProperties()
// PARTIAL. The string properties and the two bool properties are declared in the original order. The bool properties
// use the three-argument AddProperty<bool>. The listing calls the clone _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_.clone.21
// (0x3a92b4) with no default argument; that clone stores the default byte 1 (MOV R2,#1; STRB R2,[R4,#0x20]), so true is passed.
// Not declared: "spawn_delay" and "spawn_view_radius". The original builds those as hand-made Property objects of 40 and
// 36 bytes, with member offsets 0x1430 and 0x1438 from the PropertyMap base and default word 0. Their Property subclass
// vtables (off_965EB0 and off_964898) are not named, and neither size matches SimpleTypeProperty<int> or <float> (0x24).
// The original's return value, the PropertyMap slot of its last property, is dropped because the override is void.
void Character::DeclareProperties() {
    GameObject::DeclareProperties();
    AddProperty<std::string>("charpropsname", m_charPropsName);
    AddProperty<std::string>("char_template", m_charTemplate);
    AddProperty<std::string>("ai_state", m_aiState);
    AddProperty<bool>("ai_state_visible", m_aiStateVisible, true);
    AddProperty<std::string>("starting_waypoint", m_startingWaypoint);
    AddProperty<std::string>("char_group", m_charGroup);
    AddProperty<std::string>("char_group_role", m_charGroupRole);
    AddProperty<bool>("auto_spawn", m_autoSpawn, true);
    // PARTIAL: "spawn_delay" and "spawn_view_radius" (Property objects, see above) are not declared.
}

// 0x3ab674 Character::AddCharOIDToCache(int, unsigned int)
// An id outside the character table is ignored. An id already in s_cachedCharOIDs keeps the larger of its stored flags
// and the new flags. A new id is inserted with the new flags.
// PARTIAL: the preload that follows an insertion is not reproduced. It reads the model id at byte +16 of the 900-byte
// CharacterTable row and the model name at +8 of the 12-byte ModelDict row, then calls
// AssetManager::preloadSceneNode with the 0x7FFFFFFF option. The table rows are not decoded into host elements yet.
// The two empty std::string temporaries of the original have no effect and are not reproduced.
void Character::AddCharOIDToCache(int oid, unsigned int flags) {
    if (oid < 0 || static_cast<std::uint32_t>(oid) >= Arrays::CharacterTable::size) {
        return;
    }
    auto found = s_cachedCharOIDs.find(oid);
    if (found != s_cachedCharOIDs.end()) {
        if (found->second < flags) {
            found->second = flags;
        }
        return;
    }
    s_cachedCharOIDs.emplace(oid, flags);
    // PARTIAL: model preload not reproduced (see above).
}

// 0x3abb9c Character::UpdateObjectOfInterest()
// PARTIAL. Implemented: the changed flag reset. Not reproduced: the refresh timer (the frame time from
// ApplicationGetInstance() is subtracted from m_ooiRefreshTimer; the search runs when it expires and resets it to 500,
// and the original returns early before any change while it is positive), the validity test of the current object (a
// virtual call at vtable byte offset 0x8c, which the Character header does not declare; it clears the object when the
// test fails, or when the object is marked for deletion and the test passes), and the target search itself (an
// ObjectSearcher::TargetList over the room objects with the CharacterDesign.OOI_Distance constant, the
// GetInteractionType choice at virtual slot 144, and the result queue). Until those are reproduced the object of interest
// is left as it is and the changed flag is only reset.
void Character::UpdateObjectOfInterest() {
    m_ooiChanged = 0;
    // PARTIAL: see above.
}
