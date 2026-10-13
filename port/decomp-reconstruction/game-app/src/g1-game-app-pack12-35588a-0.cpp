// Batch g1-game-app-pack12-35588a-0 (18 ELF functions in 10 owner classes): the Execute and IsBlocking bodies of the
// Script_StopSound, Script_UnEquipHands, Script_UnEquipItemFromSlot, Script_UnlockCharacter, Script_Wait,
// Script_WaitCamera and Script_WaitDialog commands, SearchFailCache::add, and the TestCharPropId and TestCharTemplate
// predicates.
//
// Every Execute of this group first reads the isTracingScriptCmd debug switch and discards the result. That read is
// kept (TraceScriptCmd): DebugSwitches::GetInstance() loads the switch file on first use, and GetSwitch stores a
// missing key as false, so the lookup has side effects.
//
// PARTIAL, for a shared reason. A command's argument record (ScriptCmdImpl::m_args) is a polymorphic object whose
// class is not declared anywhere in the tree. Its fields (the cue id, the slot, the duration, the character name)
// are only known by their offsets in the listing, and reading them would need pointer arithmetic on raw offsets.
// The layout request is in the ledger notes. Each partial body keeps the parts that do not depend on the record.
//   - Script_StopSound::Execute also needs VoxSoundManager::s_instance, Stop and StopMusic. The vox-audio header
//     declares none of them.
//   - Script_UnEquipHands, Script_UnEquipItemFromSlot and Script_UnlockCharacter resolve their character by name
//     through ObjectManager::GetObjectByName on the ObjectManager global dword_99F764, which has no accessor in the
//     tree. The character-side work is in the [[maybe_unused]] helpers below, ready for the lookup.
//   - Script_Wait::Execute does not copy the duration (record +0x08) into m_duration.
//   - Script_WaitCamera::IsBlocking needs the byte at +0x84 of the level's camera (CameraLevel, no header in the tree).
//   - Script_WaitDialog::IsBlocking needs Level::hasActiveDialog. dh2/Level.hpp cannot be included in the same
//     translation unit as dh2/Character.hpp, which this file needs for the TestChar* predicates: both headers define a
//     class IStreamBase (pydata-accessors/deps/stream_reader.hpp and net-transport/deps/i_stream_base.hpp). This is a
//     header conflict, not a change in this file. Character.hpp is kept because it serves more of the batch.
//
// Original functions are called through the same accessors the other game-app sources use: ApplicationGetInstance()
// (dh2/global/ctrl_camera.hpp) for Singleton<Application>::s_inst, and the ScriptCmdImpl slots.

#include "dh2/Script_StopSound.hpp"
#include "dh2/Script_UnEquipHands.hpp"
#include "dh2/Script_UnEquipItemFromSlot.hpp"
#include "dh2/Script_UnlockCharacter.hpp"
#include "dh2/Script_Wait.hpp"
#include "dh2/Script_WaitCamera.hpp"
#include "dh2/Script_WaitDialog.hpp"
#include "dh2/SearchFailCache.hpp"
#include "dh2/TestCharPropId.hpp"
#include "dh2/TestCharTemplate.hpp"

#include <string>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/v2Controllable.hpp"
#include "dh2/v2Controller.hpp"
#include "dh2/global/ctrl_camera.hpp"

namespace {

// The isTracingScriptCmd read that starts every Execute in this batch. The result is discarded.
void TraceScriptCmd() {
    (void)DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));
}

// The equipment part of Script_UnEquipHands::Execute (0x45ded0), for a resolved character. It stores the item index
// of the item in hand 1 at +0x14f4 and of the item in hand 2 at +0x14f8, then unequips slots 1 and 2 through the
// Character virtual UnEquipItemFromSlot (vtable byte +0x144). The inventory is the ItemInventory base at +0x37c.
[[maybe_unused]] void UnEquipBothHands(Character& character) {
    ItemInstance* hand1 = character.ItemInventory::GetEquippedItem(1);
    character.unk_14f4 = character.ItemInventory::GetItemIndex(hand1);
    ItemInstance* hand2 = character.ItemInventory::GetEquippedItem(2);
    character.unk_14f8 = character.ItemInventory::GetItemIndex(hand2);
    character.UnEquipItemFromSlot(1);
    character.UnEquipItemFromSlot(2);
}

// The per-character part of Script_UnlockCharacter::Execute (0x45dc80): clears the flag at +0x08 of the character's
// controller. The controller is the v2Controllable::m_controller at +0x378 of the character.
[[maybe_unused]] void UnlockCharacterController(Character& character) {
    character.v2Controllable::m_controller->m_blocked = false;
}

// The "All" branch of Script_UnlockCharacter::Execute: clears the global controller block.
[[maybe_unused]] void UnlockAllControllers() {
    v2Controller::s_blocked = false;
}

}  // namespace

// 0x455658 Script_StopSound::IsBlocking() const
bool Script_StopSound::IsBlocking() const {
    return false;
}

// 0x45fe2c Script_StopSound::Execute(bool, int)
// PARTIAL. Only the isTracingScriptCmd read is rebuilt. Missing: the record's cue id (+0x08, int), the flag at +0x0c
// (nonzero selects VoxSoundManager::StopMusic(cue)), and the second argument at +0x10 for VoxSoundManager::Stop(
// s_instance, +0x10, cue) on the sound path. VoxSoundManager::s_instance, Stop and StopMusic are not declared in
// vox-audio/include/vox-audio/vox/sound_manager.hpp.
void Script_StopSound::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
}

// 0x455800 Script_UnEquipHands::IsBlocking() const
bool Script_UnEquipHands::IsBlocking() const {
    return false;
}

// 0x45ded0 Script_UnEquipHands::Execute(bool, int)
// PARTIAL. Only the isTracingScriptCmd read is rebuilt. Missing: the character name (record +0x0c, const char*) and the
// lookup ObjectManager::GetObjectByName(name, module, false, nullptr) on the ObjectManager global dword_99F764, which
// has no accessor in the tree. When the handle resolves to a Character, the original calls UnEquipBothHands on it
// (see the helper above).
void Script_UnEquipHands::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
}

// 0x4557f8 Script_UnEquipItemFromSlot::IsBlocking() const
bool Script_UnEquipItemFromSlot::IsBlocking() const {
    return false;
}

// 0x45da8c Script_UnEquipItemFromSlot::Execute(bool, int)
// PARTIAL. Only the isTracingScriptCmd read is rebuilt. Missing: the slot (record +0x08, int) and the character name
// (record +0x10, const char*), and the lookup ObjectManager::GetObjectByName(name, module, false, nullptr) on the
// ObjectManager global dword_99F764. When the handle resolves to a Character, the original calls its virtual
// UnEquipItemFromSlot(slot) (vtable byte +0x144, Character::UnEquipItemFromSlot).
void Script_UnEquipItemFromSlot::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
}

// 0x45574c Script_UnlockCharacter::IsBlocking() const
bool Script_UnlockCharacter::IsBlocking() const {
    return false;
}

// 0x45dc80 Script_UnlockCharacter::Execute(bool, int)
// PARTIAL. Only the isTracingScriptCmd read is rebuilt. Missing: the character name (record +0x0c, const char*), which
// selects the branch: "All" clears v2Controller::s_blocked (UnlockAllControllers), any other name is looked up through
// ObjectManager::GetObjectByName on the ObjectManager global dword_99F764, and a resolved Character has its controller
// flag cleared (UnlockCharacterController).
void Script_UnlockCharacter::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
}

// 0x455754 Script_Wait::IsBlocking() const
bool Script_Wait::IsBlocking() const {
    return m_elapsed < m_duration;
}

// 0x4597b4 Script_Wait::Update()
void Script_Wait::Update() {
    const int elapsed = m_elapsed;
    m_elapsed = ApplicationGetInstance().GetDt() + elapsed;
}

// 0x45c438 Script_Wait::Execute(bool, int)
// PARTIAL. The elapsed counter is reset and the trace is read. Missing: m_duration is copied from the record's int at
// +0x08 (record type not declared), so m_duration keeps its previous value here.
void Script_Wait::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
    m_elapsed = 0;
}

// 0x45932c Script_WaitCamera::IsBlocking() const
// PARTIAL. The original returns the byte at +0x84 of the current level's camera (Level +0x128, a CameraLevel*) when both
// exist, and false otherwise. dh2/Level.hpp cannot share a translation unit with dh2/Character.hpp (both pull in a class
// IStreamBase, from pydata-accessors/deps/stream_reader.hpp and net-transport/deps/i_stream_base.hpp), and CameraLevel
// has no header in the tree. The current level is requested as in the listing; the function returns false as a
// placeholder.
bool Script_WaitCamera::IsBlocking() const {
    (void)ApplicationGetInstance().GetCurrentLevel();
    return false;
}

// 0x45c5e8 Script_WaitCamera::Execute(bool, int)
void Script_WaitCamera::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
}

// 0x4598d8 Script_WaitDialog::IsBlocking() const
// PARTIAL. The original returns Level::hasActiveDialog() of the current level. That call needs dh2/Level.hpp, which
// cannot share a translation unit with dh2/Character.hpp (both define a class IStreamBase, see Script_WaitCamera::
// IsBlocking). The level is requested as in the listing, and the function returns false as a placeholder. Once Level.hpp
// can be included, the call is level->hasActiveDialog() with no null check, as in the original.
bool Script_WaitDialog::IsBlocking() const {
    (void)ApplicationGetInstance().GetCurrentLevel();
    return false;
}

// 0x45c4d8 Script_WaitDialog::Execute(bool, int)
void Script_WaitDialog::Execute(bool skipping, int module) {
    (void)skipping;
    (void)module;
    TraceScriptCmd();
}

// 0x52a504 SearchFailCache::add(SearchFailCache::Entry &)
// The original keeps a vector of 12-byte entries. When more than 10 entries are stored, it sets the end pointer back to
// the begin pointer, which empties the vector and keeps its storage. It then copies the entry to the end, or grows the
// vector when the storage is full. std::vector::clear and push_back do both steps.
void SearchFailCache::add(Entry& entry) {
    if (m_entries.size() > 10) {
        m_entries.clear();
    }
    m_entries.push_back(entry);
}

// 0x47b820 TestCharPropId::Compare(int, Character *)
// Static in the header: the id is the first argument (r0), the character the second (r1). r2 is not read.
bool TestCharPropId::Compare(int propId, Character* character) {
    return character != nullptr && character->SafeGetCharPropsId() == propId;
}

// 0x47b05c TestCharTemplate::Compare(int, Character *)
// Static in the header: the template id is the first argument (r0), the character the second (r1). r2 is not read.
bool TestCharTemplate::Compare(int templateId, Character* character) {
    return character != nullptr && character->SafeGetCharPropsTemplateId() == templateId;
}
