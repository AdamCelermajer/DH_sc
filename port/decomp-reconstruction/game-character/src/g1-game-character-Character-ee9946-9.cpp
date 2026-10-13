// Batch g1-game-character-Character-ee9946-9: twenty Character functions of the setup and combat-roll part of the class.
// The combat rolls are F_SpellAttack, F_SkillAttack and F_MeleeAttack. The setup chain is InitAll, InitPost, InitFinal,
// InitSpawned, InitCam, InitCoopCam, InitPhysicalObject, InitLightSetAndMaterial and RegisterCharacterFXTable. The
// equipment and skill setup is _InitEquipment, _InitHpMp, _InitSkillsSlots and _InitSounds. The rest are
// InitializePlayerSavegame, UpdateInventoryLocalization, SafeGetCharPropsId and SafeGetCharPropsTemplateId.
// Declarations come from dh2/Character.hpp, which this batch does not change (every member and method used here is
// already declared there).
//
// Several bodies are PARTIAL. Each PARTIAL comment names the missing piece; the ledger rows repeat it:
//  - gAssertLevel 2: the original stores 0 to address 0 (a deliberate crash). That store is not reproduced; level 1
//    prints the assert text to stderr as the original does.
//  - PlayerManager: dh2/PlayerManager.hpp cannot be included together with Character.hpp (its StreamBuffer needs
//    net-transport's IStreamBase, which pydata's stream_reader.hpp also defines). Every branch that tests
//    PlayerManager::IsLocalPlayer, GetLocalPlayer or GetPlayerByCharacter is therefore not taken, and its body is not run.
//  - Name tables and undeclared members or globals: Arrays::*::m_memberNames (only in member_names.hpp, which conflicts
//    with the structs headers), VisualFXManager's instance (Singleton<VisualFXManager>::s_inst at ELF 0x9a1cf0),
//    VoxSoundManager, PyDataConstants' accessor, Level's difficulty, the scene node's RootSceneNode, the LightSetManager
//    path, the VisualObject light filter (std::vector<bool> at +0x44, declared as void* unk_44), and the CharProperties
//    words at +0x3c..+0x44 (the scale).

#include <cstdint>
#include <cstdio>
#include <string>

#include "dh2/Character.hpp"
#include "dh2/AnchorGroup.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ItemInstance.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/Random.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/v2Controller.hpp"
#include "pydata-accessors/w7_arrays27.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

// gAssertLevel is defined by core-platform (memory.cpp) and declared in core-platform/memory.hpp. That header is not
// included here: it pulls core-platform's glitch_core stand-in, which clashes with the glitch-shared value types that
// dh2/AnchorGroup.hpp brings in. The declaration below is the same one.
extern int gAssertLevel;

namespace {

// Original file names of the ASSERT calls (copied verbatim from the listing's string literals).
constexpr const char* kFormulasFile =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Formulas.cpp";
constexpr const char* kInitializationFile =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character_Initialization.cpp";

// ASSERT(expr) at file:line. gAssertLevel 1 prints the message to stderr (the third _iob entry of the original).
// gAssertLevel 2 stores 0 to address 0 in the original; that store is not reproduced (PARTIAL in each caller).
void ReportAssertLevel1(const char* expr, const char* file, int line) {
    if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, file, line);
    }
}

// abs32 of the original. The negation wraps for the most negative value, as the two's complement negation does.
int Abs32(int value) {
    return value < 0 ? static_cast<int>(0u - static_cast<unsigned int>(value)) : value;
}

// The isTracingChar_* debug switch read. Every caller in this batch drops the result, as the original does.
void TouchDebugSwitch(const char* name) {
    DebugSwitches::GetInstance()->load();
    (void)DebugSwitches::GetInstance()->GetSwitch(name);
}

}  // namespace

// 0x3b3004 _ZN9Character13F_SpellAttackERNS_12AttackResultEPS_S2_ii
// Character::F_SpellAttack(Character::AttackResult&, Character*, Character*, int, int)
// PARTIAL: the gAssertLevel 2 store of the two null checks is not reproduced (see ReportAssertLevel1). Reproduced: the
// level 1 messages, the isTracingChar_Attack switch read and the roll with flags b | 0x1005554A, weapon -1, c = a, d = 0.
void Character::F_SpellAttack(AttackResult& result, Character* attacker, Character* target, int a, int b) {
    if (!attacker) {
        ReportAssertLevel1("attacker", kFormulasFile, 633);
    }
    if (!target) {
        ReportAssertLevel1("defender", kFormulasFile, 634);
    }
    TouchDebugSwitch("isTracingChar_Attack");
    _F_CalculateResult(result, attacker, target, b | 0x1005554A, -1, a, 0);
}

// 0x3b31a4 _ZN9Character13F_SkillAttackERNS_12AttackResultEPS_S2_ii
// Character::F_SkillAttack(Character::AttackResult&, Character*, Character*, int, int)
// PARTIAL: the gAssertLevel 2 store of the two null checks is not reproduced. Reproduced: the level 1 messages, the
// switch read, the weapon slot (2 when flag 0x4000000 is set, else 1), the weapon word (Structs::Item m_w37, +0x94, or -1
// with nothing equipped) and the roll with flags a | 0x8000000, c = b, d = 0.
void Character::F_SkillAttack(AttackResult& result, Character* attacker, Character* target, int a, int b) {
    if (!attacker) {
        ReportAssertLevel1("attacker", kFormulasFile, 613);
    }
    if (!target) {
        ReportAssertLevel1("defender", kFormulasFile, 614);
    }
    const int flags = a | 0x8000000;
    TouchDebugSwitch("isTracingChar_Attack");
    const unsigned int slot = (flags & 0x4000000) != 0 ? 2u : 1u;
    int weapon = -1;
    if (ItemInstance* equipped = attacker->GetEquippedItem(slot)) {
        weapon = equipped->GetItem()->m_w37;
    }
    _F_CalculateResult(result, attacker, target, flags, weapon, b, 0);
}

// 0x3b3368 _ZN9Character15F_MeleeAttackERNS_12AttackResultEPS_S2_bb
// Character::F_MeleeAttack(Character::AttackResult&, Character*, Character*, bool, bool)
// PARTIAL: (1) the gAssertLevel 2 store of the two null checks is not reproduced. (2) When b is false the original's flag
// word is the link-time address of a string literal (MOVEQ R7, R3 in the listing: aZn6glitch5scen_399+0x12), not a flag.
// That value is not recoverable from the pseudocode; the placeholder 0 stands in for it in that case only. (3) The
// SnS_CleanUp class application when b is set (a name search in Arrays::ClassTable::m_memberNames, then PROPS_ApplyClass
// with useTemp false) is not reproduced: the name table is declared only in pydata-accessors/member_names.hpp, which cannot
// be included with Character.hpp. Reproduced: the switch read, the weapon slot (2 when a is set), the weapon word, flag
// 0x4000000 when a is set, and the roll with c = -1, d = 0.
void Character::F_MeleeAttack(AttackResult& result, Character* attacker, Character* target, bool a, bool b) {
    if (!attacker) {
        ReportAssertLevel1("attacker", kFormulasFile, 584);
    }
    if (!target) {
        ReportAssertLevel1("defender", kFormulasFile, 585);
    }
    TouchDebugSwitch("isTracingChar_Attack");
    const unsigned int slot = a ? 2u : 1u;
    int flags = b ? 0x0005554A : 0;  // PARTIAL: 0 stands in for the link-time address when b is false (see above)
    int weapon = -1;
    if (ItemInstance* equipped = attacker->GetEquippedItem(slot)) {
        weapon = equipped->GetItem()->m_w37;
    }
    // PARTIAL: SnS_CleanUp class application when b is set (see above).
    if (a) {
        flags |= 0x4000000;
    }
    _F_CalculateResult(result, attacker, target, flags, weapon, -1, 0);
}

// 0x3b35f0 _ZN9Character7InitAllEv
// Character::InitAll(void)
// Reproduced: InitPost (vtable slot +0x1c) then InitFinal (vtable slot +0x58), both as virtual calls.
void Character::InitAll() {
    InitPost();
    InitFinal();
}

// 0x3b36b0 _ZN9Character22InitializePlayerSavegameEv
// Character::InitializePlayerSavegame(void)
// Reproduced: the PlayerSavegame is created into m_savegame (+0x14e8), then SG_SetPlayer(this). The original allocates 408
// bytes with operator new(size, 0) and calls the constructor; the host object is sized by its own type.
void Character::InitializePlayerSavegame() {
    m_savegame = new PlayerSavegame();
    SG_SetPlayer(this);
}

// 0x3b36e4 _ZN9Character26UpdateInventoryLocalizationEv
// Character::UpdateInventoryLocalization(void)
// Reproduced: ItemInventory::UpdateLocalizationForItems on the ItemInventory base (+0x37c).
void Character::UpdateInventoryLocalization() {
    ItemInventory::UpdateLocalizationForItems();
}

// 0x3b36ec _ZN9Character26SafeGetCharPropsTemplateIdEv
// Character::SafeGetCharPropsTemplateId(void)
// PARTIAL. Reproduced: when m_charTemplate is empty the cached m_charPropsTemplateId (+0x13ca) is returned. The original's
// string test compares the string's start and finish pointers, which is the empty test of the host std::string.
// Not reproduced: the name search of m_charTemplate in Arrays::Charater_Templates::m_memberNames (the first match or -1,
// cached into m_charPropsTemplateId). That name table is declared only in pydata-accessors/member_names.hpp, which cannot be
// included with Character.hpp. The cached value is returned unchanged in the non-empty case.
int Character::SafeGetCharPropsTemplateId() {
    return m_charPropsTemplateId;
}

// 0x3b379c _ZN9Character11InitSpawnedEiRK7Point3DIfE
// Character::InitSpawned(int, Point3D<float> const&)
// Reproduced from the listing's call arguments (the Hex-Rays prototype of GameObject::SetPosition is one argument; the
// listing passes m_initialPosition and true). Sets the character property id (+0x13c8), m_isSpawned, m_inZone, the
// initial position, then runs InitPost, InitFinal, SetVisible(true) and CharStateMachine::SM_SetSpawnState(false, false).
void Character::InitSpawned(int charPropsId, const Point3D<float>& pos) {
    m_charPropsId = static_cast<std::int16_t>(charPropsId);
    m_isSpawned = 1;
    m_inZone = 1;
    SetInitialPosition(pos);
    SetPosition(m_initialPosition, true);
    InitPost();
    InitFinal();
    SetVisible(true);
    CharStateMachine::SM_SetSpawnState(false, false);
}

// 0x3b3928 _ZN9Character11InitCoopCamEv
// Character::InitCoopCam(void)
// Reproduced: a new AnchorGroup on this character (AnchorType 1, the value the listing passes) becomes the camera anchor.
// The original allocates 0x1c bytes with operator new(size, 0) before the constructor call.
void Character::InitCoopCam() {
    AnchorGroup* anchor = new AnchorGroup(this, static_cast<AnchorBase::AnchorType>(1));
    SetCameraAnchorObject(anchor);
}

// 0x3b395c _ZN9Character14_InitEquipmentEv
// Character::_InitEquipment(void)
// PARTIAL: the entry gate is not evaluable here. It is `!GetOnline()-><offline flag> || PlayerManager::GetPlayerByCharacter(
// this, 0)->+0x66c == 1`; COnline's flag has no getter and PlayerManager cannot be included. The body is therefore not run.
// Also missing from the body: the item-9 property read (CharProperties::_GetProperty on the m_props sheet at +0xa94; the
// method and member are private in dh2/CharProperties.hpp), and the branch on the word at Character +0x39c (the
// ItemInventory +0x20 word, m_gold in dh2/ItemInventory.hpp), whose meaning is not confirmed. The EquipItemAuto loop over
// IsItemEquippable slots is part of the body too.
void Character::_InitEquipment() {
}

// 0x3b3a70 _ZN9Character10_InitHpMpEv
// Character::_InitHpMp(void)
// Reproduced: RegenHP(-1), then RegenMP(-1).
void Character::_InitHpMp() {
    RegenHP(-1);
    RegenMP(-1);
}

// 0x3b3a90 _ZN9Character16_InitSkillsSlotsEv
// Character::_InitSkillsSlots(void)
// Reproduced: when SG_HasSkillSlots is false, clear slot 0 and swap the equipment set, clear slot 0 again and swap back,
// then add skill 0 (IncSkill(0, false)) when its level is 0.
void Character::_InitSkillsSlots() {
    if (SG_HasSkillSlots()) {
        return;
    }
    SG_SetSkillInSlot(0, 0);
    ItemInventory::SwapEquipmentSet();
    SG_SetSkillInSlot(0, 0);
    ItemInventory::SwapEquipmentSet();
    if (SG_GetSkillLevel(0) == 0) {
        IncSkill(0, false);
    }
}

// 0x3b3b00 _ZN9Character11_InitSoundsEv
// Character::_InitSounds(void)
// PARTIAL: the four loops load the sound ids of the Structs::CharSounds row (counts m_w1, m_w3, m_w5, m_w7 with arrays
// m_w2, m_w4, m_w6, m_w8) through VoxSoundManager::LoadSound on VoxSoundManager::s_instance. No header declares
// VoxSoundManager's instance or LoadSound (vox-audio has no header; core-platform/w7_global_rem.hpp only forward-declares
// the type), so nothing is loaded.
void Character::_InitSounds() {
}

// 0x3b3d38 _ZN9Character17SafeGetCharPropsIdEv
// Character::SafeGetCharPropsId(void)
// PARTIAL. (1) The gAssertLevel 2 store at Character_Initialization.cpp:756 is not reproduced; the assert cannot fire for a
// valid table (the index is a remainder of the count). (2) Not reproduced: every name search of Arrays::CharacterTable::
// m_memberNames ("KnightPlayerBase" for a player whose class is -1, and m_charPropsName for a character without a
// template); that table is declared only in pydata-accessors/member_names.hpp, which cannot be included with Character.hpp.
// Those branches leave the cached id unchanged. Reproduced: the cached id (+0x13c8), the player class read (SG_Load(1),
// SG_GetPlayerClass, SG_SetPlayerClass when the class is not -1), and the template branch: the Random draw (Random::s_seed,
// LCG 59051 / 177149 mod 0xdaf26b, debug counter [0]) and the Structs::CharInfoName index into the template row. That
// branch takes its template id from SafeGetCharPropsTemplateId, which is a stub (see there), so it is only as good as that id.
int Character::SafeGetCharPropsId() {
    if (m_charPropsId != -1) {
        return m_charPropsId;
    }
    if (IsPlayer()) {
        SG_Load(1);
        const int playerClass = SG_GetPlayerClass();
        m_charPropsId = static_cast<std::int16_t>(playerClass);
        if (playerClass != -1) {
            SG_SetPlayerClass(playerClass);
        }
        // PARTIAL: playerClass == -1 needs the KnightPlayerBase name search (see above); SG_SetPlayerClass is not called then.
        return m_charPropsId;
    }
    if (m_charTemplate.empty()) {
        // PARTIAL: the m_charPropsName search (see above). The cached id is returned as it is.
        return m_charPropsId;
    }
    const int templateId = SafeGetCharPropsTemplateId();
    if (templateId < 0) {
        return m_charPropsId;
    }
    const auto* row = reinterpret_cast<const Structs::CharTemplate*>(Arrays::Charater_Templates::members) + templateId;
    const std::uint32_t count = row->m_w1;
    if (count == 0) {
        return m_charPropsId;
    }
    Random::s_seed = (59051u * Random::s_seed + 177149u) % 0xDAF26Bu;
    const int index = Abs32(static_cast<int>(Random::s_seed % count));
    ++Random::s_debugRandomCounters[0];
    if (count <= static_cast<std::uint32_t>(index)) {
        ReportAssertLevel1("random >= 0 && random < (int)charTemplate.CharInfoSize", kInitializationFile, 756);
    }
    const auto* infos = reinterpret_cast<const Structs::CharInfoName*>(row->m_w2);
    m_charPropsId = static_cast<std::int16_t>(infos[index].m_w1);
    return m_charPropsId;
}

// 0x3b4088 _ZN9Character18InitPhysicalObjectEv
// Character::InitPhysicalObject(void)
// PARTIAL. The leaf order of the original is kept. No physics object is created: each leaf builds a POCharacter-family
// object on the physics world (Application +0x44, the global dword_99F770, which no header declares), and POVillager and
// POMonster have no header. Reproduced: the one leaf that builds nothing, SetPhysicalObject(nullptr, false) for a
// non-monster that is none of the others.
void Character::InitPhysicalObject() {
    if (IsInvisibleMan()) {
        // PARTIAL: POCharacter(world, this, 0, 256, 0, 0, 0) with the POFaerie vtable; SetPhysicalObject(obj, true).
        return;
    }
    if (m_static) {
        // PARTIAL: PhysicalObject(world, this, 1, 0, 0, 0, 0, 0, 2, 0xFFFF, 1) with the static vtable; SetPhysicalObject(obj, true).
        return;
    }
    if (IsNPC()) {
        // PARTIAL: POCharacter(world, this, -4, 1024, 0xD1F, 0) with the POVillager vtable (no header); SetPhysicalObject(obj, true).
        return;
    }
    if (IsPlayer()) {
        // PARTIAL: POCharacter(world, this, -1, 4, 0xD7F, 1) with the player vtable; SetPhysicalObject(obj, true).
        return;
    }
    if (IsFollower() || IsSummoned()) {
        // PARTIAL: POCharacter(world, this, -2, 8, 0xD3B, 0) with the POFollower vtable; SetPhysicalObject(obj, true).
        return;
    }
    if (IsFaerie()) {
        // PARTIAL: POCharacter(world, this, 0, 256, 0, 0) with the POFaerie vtable; SetPhysicalObject(obj, true).
        return;
    }
    if (!IsMonster()) {
        SetPhysicalObject(nullptr, false);
        return;
    }
    // PARTIAL: POCharacter(world, this, 2, 16, 0xD3F, 0) with the POMonster vtable (no header); SetPhysicalObject(obj, true).
}

// 0x3b4738 _ZN9Character23RegisterCharacterFXTableEv
// Character::RegisterCharacterFXTable(void)
// PARTIAL: the three VisualFXManager::RegisterFXSetToLoad calls (footprint, blood, blood death; each when the id is >= 0)
// run on Singleton<VisualFXManager>::s_inst (ELF 0x9a1cf0). No header declares that instance, so they are not made.
// Reproduced: the three id reads and the isTracingChar_Init switch read.
void Character::RegisterCharacterFXTable() {
    const int fxFootprint = GetFXFootprint();
    const int fxBlood = GetFXBlood();
    const int fxBloodDeath = GetFXBloodDeath();
    TouchDebugSwitch("isTracingChar_Init");
    (void)fxFootprint;
    (void)fxBlood;
    (void)fxBloodDeath;
}

// 0x3b4834 _ZN9Character20InitLightSetAndMaterialEv
// Character::InitLightSetAndMaterial(void)
// PARTIAL. Not reproduced: the light set id store (VisualObject +0x40, declared unk_40) from LightSetManager::
// GetLightSetIdFromName("MonsterLight") on the light manager reached from the Application singleton (no header path), and the
// std::vector<bool> light filter copy into VisualObject +0x44 (declared void* unk_44 in dh2/VisualObject.hpp). Reproduced:
// the five material applications, in the original's order, on the visual object (+0x2d8).
void Character::InitLightSetAndMaterial() {
    VisualObject* visual = m_visualObject;
    if (visual == nullptr) {
        return;
    }
    visual->ApplyLightSet();
    visual->ApplyMaterial();
    visual->ApplyMaterialBaseParam();
    visual->ApplyShadowMaterial();
    visual->ApplyXrayMaterial();
}

// 0x3b4978 _ZN9Character9InitFinalEv
// Character::InitFinal(void)
// PARTIAL. Reproduced: the one-time guard (+0x1395), the spawn probability test against GameObject::m_spawnProb, the
// GameObject::InitFinal base call, and the CharAI::OnInitFinal call (CharAI vtable slot +0x10) on the CharAI base.
// Not reproduced (each needs PlayerManager or the light manager):
//  - Faerie or follower: the offset vector (GameObject::GetLookAtVec of the local player's object at +0x660 of the local
//    player record, plus its GetTargetPosition) passed to GameObject::SetPosition(vector, true).
//  - The light set name ("SceneLight" for a player, "MonsterLight" otherwise) and its store (VisualObject +0x40).
//  - Local player only: CharAI::UpdateAllSkills, CharProperties::RecalcProperties(true), the PROPS word 194 clamp into
//    ItemInventory::m_maxPotions (+0x2c, word +0x3a8) and Character::SG_Save.
void Character::InitFinal() {
    if (m_initFinalDone != 0) {
        return;
    }
    m_initFinalDone = 1;
    if (!(CheckSpawnProbability() < m_spawnProb)) {
        return;
    }
    GameObject::InitFinal();
    // PARTIAL: faerie or follower offset vector (see above).
    // PARTIAL: light set name and id store (see above).
    OnInitFinal();
    // PARTIAL: local player block (see above).
}

// 0x3b4bc4 _ZN9Character7InitCamEv
// Character::InitCam(void)
// PARTIAL. Reproduced: the UseStaticCamera debug switch read. Not reproduced: the static branch (AnchorBase built with a
// zero-argument constructor, which dh2/AnchorBase.hpp does not declare), and the forward branch (AnchorForward with the
// ForwardCamera_Max_Distance, ForwardCamera_Distance_PerSec and ForwardCamera_Threshold constants, read through
// PyDataConstants::getConstant, a class no header declares; the fifth AnchorForward argument is not in the listing).
// The camera anchor is therefore never set here.
void Character::InitCam() {
    DebugSwitches::GetInstance()->load();
    const bool useStaticCamera = DebugSwitches::GetInstance()->GetSwitch("UseStaticCamera");
    (void)useStaticCamera;
}

// 0x3b4d60 _ZN9Character8InitPostEv
// Character::InitPost(void)
// PARTIAL. Reproduced, in the original's order: the one-time guard (+0x1394), the spawn probability test, the starting
// waypoint move (ObjectManager::GetObjectByName with the ObjectBase word +0x64, active false, no type, then Cmd_MoveTo on
// the controller), the character property id and the base property load with RecalcProperties, the model name assign
// (GameObject m_dae), GameObject::InitPost, the MeetCondition split (false: SetVisible(false) and ObjectBase::Delete),
// SG_Load(2), RegisterCharacterFXTable, CharAnimator::SetAnimationSet, _InitSounds, the properties 210 and 211 into
// unk_1440 and unk_1444, the initial position (SetInitialPosition(m_position), SetPosition(m_initialPosition, true)), the
// rotation copy, the mesh box, Revive(nullptr, true) and _InitHpMp.
// Not reproduced:
//  - Scale (+0x120..+0x128): the CharProperties words at Character +0x59c, +0x5a0 and +0x5a4 (0.009f, 0.009f, 0.01f) are
//    not named by dh2/CharProperties.hpp.
//  - The script gate: it needs PlayerManager::IsLocalPlayer. Its false branch sets CharAI m_stagedLoad (+0x24), which is
//    private in dh2/CharAI.hpp, so the InitScriptProcess and AddToGroup calls (AddToGroup takes the flag as a second
//    argument in the listing; the header declares one parameter) are not made.
//  - m_selfFX (+0x1484): VisualFXManager's instance is not declared.
//  - The IsPlayer block: the scene node's automatic culling (RootSceneNode is incomplete in VisualObject.hpp), SG_Load(4),
//    the difficulty from the current Level (Level.hpp conflicts with Character.hpp), the local player's equipment, skill
//    slots and gear properties (PlayerManager), the maximum gold of 999999999 (ItemInventory::m_maxGold), the
//    m_maxPotions clamp, the target-circle and cursor FX (VisualFXManager), and AddMultiplayerHighlight.
void Character::InitPost() {
    if (m_initPostDone != 0) {
        return;
    }
    m_initPostDone = 1;
    if (!(GameObject::CheckSpawnProbability() < m_spawnProb)) {
        return;
    }
    TouchDebugSwitch("isTracingChar_Init");
    if (!m_startingWaypoint.empty()) {
        ObjectHandle waypoint = dword_99F764->GetObjectByName(m_startingWaypoint.c_str(), unk_64, false, nullptr);
        if (waypoint.GetObject(false) != nullptr) {
            GameObject* target = waypoint;
            if (target != nullptr) {
                m_controller->Cmd_MoveTo(target);
            }
        }
    }
    SafeGetCharPropsId();
    int loadId = -1;
    if (m_charPropsId != -1) {
        TouchDebugSwitch("isTracingChar_Init");
        loadId = m_charPropsId;
    }
    CharProperties::LoadBaseProperties(loadId);
    CharProperties::RecalcProperties(true);
    if (const char* modelName = GetCharModelName()) {
        m_dae.assign(modelName);
    }
    TouchDebugSwitch("isTracingChar_Init");
    // PARTIAL: scale from the CharProperties words (see above).
    GameObject::InitPost();
    if (!MeetCondition()) {
        SetVisible(false);
        ObjectBase::Delete();
        return;
    }
    SG_Load(2);
    // PARTIAL: script gate and m_selfFX (see above).
    RegisterCharacterFXTable();
    TouchDebugSwitch("isTracingChar_Init");
    CharAnimator::SetAnimationSet();
    TouchDebugSwitch("isTracingChar_Init");
    _InitSounds();
    TouchDebugSwitch("isTracingChar_Init");
    // PARTIAL: IsPlayer block (see above).
    // The original reads CharProperties::_GetProperty(m_props, 210 and 211) (m_props is private in the header). PROPS_Get
    // reads m_props only, per its declaration note; that equivalence is assumed here and is the one PARTIAL point of this read.
    unk_1440 = static_cast<float>(PROPS_Get(210, false));
    unk_1444 = static_cast<float>(PROPS_Get(211, false));
    SetInitialPosition(m_position);
    SetPosition(m_initialPosition, true);
    m_initialRotation = m_rotation;
    if (m_visualObject != nullptr) {
        m_visualObject->ApplyMeshBox();
    }
    Revive(nullptr, true);
    _InitHpMp();
    // PARTIAL: CharAI script start and group add (see above).
    TouchDebugSwitch("isTracingChar_Init");
}
