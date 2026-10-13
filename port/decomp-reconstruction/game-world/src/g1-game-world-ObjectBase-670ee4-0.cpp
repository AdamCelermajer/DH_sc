// Batch g1-game-world-ObjectBase-670ee4-0 (part 1 of 2): 20 ObjectBase methods. Covered: the type tests, the
// updating and visible flags, the enable and disable transitions, the net and handle accessors, Delete, the
// culling and disable-condition tests, and the stream pair. Declarations come from dh2/ObjectBase.hpp.
//
// Layout used here (original 32-bit offsets, kept in ObjectBase.hpp):
//   +0x80 m_visible, +0x81 m_markedForDeletion, +0x82 m_deleteMode, +0x85 unk_85 (the updating flag),
//   +0x86 m_cullState, +0x8a m_enabled, +0x8c m_enableCond, +0xb0 m_disableCond, +0x2c m_handle,
//   +0x110 m_netOwnerId, +0x118 unk_118.

#include <cstdint>

#include "dh2/ObjectBase.hpp"
#include "dh2/Application.hpp"
#include "dh2/ConditionData.hpp"
#include "dh2/IStreamBase.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/global/ctrl_camera.hpp"            // ApplicationGetInstance (the Application singleton)
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764, the ObjectManager instance

// 0x33dcc8 _ZNK10ObjectBase12IsGameObjectEv
bool ObjectBase::IsGameObject() const {
    return false;
}

// 0x33dcd0 _ZNK10ObjectBase11IsCharacterEv
bool ObjectBase::IsCharacter() const {
    return false;
}

// 0x33dcd8 _ZNK10ObjectBase8IsPlayerEv
bool ObjectBase::IsPlayer() const {
    return false;
}

// 0x33dce0 _ZNK10ObjectBase6IsDeadEv
bool ObjectBase::IsDead() const {
    return false;
}

// 0x33dce8 _ZNK10ObjectBase11IsUpdatableEv
bool ObjectBase::IsUpdatable() const {
    return false;
}

// 0x33dcf0 _ZN10ObjectBase11setUpdatingEb
void ObjectBase::setUpdating(bool updating) {
    unk_85 = static_cast<std::uint8_t>(updating);  // +0x85: the argument is stored as is
}

// 0x33dcf8 _ZN10ObjectBase10SetVisibleEb
void ObjectBase::SetVisible(bool visible) {
    // +0x80 gets m_enabled (+0x8a) when visible is set, and 0 otherwise.
    m_visible = visible && m_enabled;
}

// 0x33dd08 _ZN10ObjectBase25PopulateOutgoingNetStructEb
void ObjectBase::PopulateOutgoingNetStruct(bool force) {
    (void)force;  // empty in the original
}

// 0x33dd0c _ZN10ObjectBase26InterpretIncomingNetStructEb
void ObjectBase::InterpretIncomingNetStruct(bool force) {
    (void)force;  // empty in the original
}

// 0x33dd10 _ZNK10ObjectBase17IsRemotelyUpdatedEv
bool ObjectBase::IsRemotelyUpdated() const {
    if (m_netOwnerId == -1) {  // +0x110
        return unk_118 != 0;   // +0x118
    }
    return true;
}

// 0x33dd2c _ZN10ObjectBase9GetHandleEv
ObjectHandle ObjectBase::GetHandle() {
    // Refresh the cache generation (ObjectManager +0x78), then return a copy of the handle (12 bytes).
    m_handle->m_generation = static_cast<std::uint32_t>(dword_99F764->m_generation);
    return *m_handle;
}

// 0x33dd70 _ZNK10ObjectBase9GetHandleEv
ObjectHandle ObjectBase::GetHandle() const {
    // Same code as the non-const form in the original. The handle is reached through the pointer at +0x2c.
    m_handle->m_generation = static_cast<std::uint32_t>(dword_99F764->m_generation);
    return *m_handle;
}

// 0x33ddb4 _ZN10ObjectBase6DeleteEv
void ObjectBase::Delete() {
    m_deleteMode = 2;         // +0x82 (stored first, as in the original)
    m_markedForDeletion = 1;  // +0x81
}

// 0x33ddc8 _ZN10ObjectBase9SetEnableEb
void ObjectBase::SetEnable(bool enable) {
    if (m_enabled == enable) {  // +0x8a unchanged: nothing else runs
        return;
    }
    m_enabled = enable;
    if (enable) {
        Enabled();   // vtable +0x44
    } else {
        Disabled();  // vtable +0x48
    }
}

// 0x33de04 _ZN10ObjectBase7EnabledEv
void ObjectBase::Enabled() {
    SetVisible(true);   // vtable +0x40
    setUpdating(true);  // vtable +0x3c
}

// 0x33de34 _ZN10ObjectBase8DisabledEv
void ObjectBase::Disabled() {
    setUpdating(false);  // vtable +0x3c
    SetVisible(false);   // vtable +0x40
}

// 0x33de90 _ZN10ObjectBase23TestCullingBeforeUpdateERK4aabbIfE
// PARTIAL: waits on the aabb<float> definition (forward-declared only, no header defines its six floats) and on the
// COnline flag at +0x05 (GetOnline() returns COnline*, global/net_singletons.hpp, and no header declares that flag).
// The frustum slot is known now: ICameraSceneNode::getViewFrustum (scene/ICameraSceneNode.hpp, +0x144). Reproduced:
// the +0x86 steps (0 becomes 1 on the first call; any value other than 1 returns true at once) and the
// no-level exit, which sets +0x86 to 2. Missing, in the original order:
//   1. The online gate. When the online flag of GetOnline() is set (COnline +0x05, private, no accessor), the
//      original calls the virtual IsRemotelyUpdated() (vtable +0x54) and discards the result.
//   2. The frustum test. The box is six floats (min x, y, z, then max x, y, z; aabb<float> is forward-declared
//      only). The camera is Level +0x128 (CameraLevel*), then CameraBase +0x08 (the glitch camera node, m_camera).
//      The node's getViewFrustum returns a SViewFrustum*, whose planes start at +0x0c, 16 bytes each (normal, then
//      D). Each plane takes the box corner by the sign of its normal, and a corner with a positive plane distance
//      rejects the object (returns 0).
// Until the frustum test is written the function never culls: it returns true and leaves +0x86 at 1.
bool ObjectBase::TestCullingBeforeUpdate(const aabb<float>& box) {
    (void)box;
    if (m_cullState == 0) {  // +0x86
        m_cullState = 1;
        return true;
    }
    if (m_cullState != 1) {
        return true;
    }
    if (ApplicationGetInstance().GetCurrentLevel() == nullptr) {
        m_cullState = 2;
        return true;
    }
    // PARTIAL: the online gate and the frustum test are missing (see above).
    return true;
}

// 0x33e0f8 _ZN10ObjectBase11DeserializeEP11IStreamBase
void ObjectBase::Deserialize(IStreamBase* stream) {
    // The original calls IStreamBase::readAs<bool>(bool&) for +0x80 and again for +0x8a (asm at 0x33e0f8).
    stream->readAs<bool>(m_visible);  // +0x80
    stream->readAs<bool>(m_enabled);  // +0x8a
    m_enableCond.SetAsTested(false);  // +0x8c
    m_disableCond.SetAsTested(false); // +0xb0
}

// 0x33e1f0 _ZN10ObjectBase9SerializeEP11IStreamBase
void ObjectBase::Serialize(IStreamBase* stream) {
    // The original calls IStreamBase::writeAs<bool>(bool const&) for +0x80 and +0x8a (asm at 0x33e1f0).
    stream->writeAs<bool>(m_visible);  // +0x80
    stream->writeAs<bool>(m_enabled);  // +0x8a
}

// 0x33e61c _ZN10ObjectBase20TestDisableConditionEb
// PARTIAL: the gate needs dh2/PlayerManager.hpp (GetLocalPlayer on dword_99F76C) and dh2/Character.hpp (m_savegame)
// in one translation unit, and those two cannot share one (CustomFree is redefined in pydata-accessors/deps/stream_reader.hpp).
// PlayerInfo +0x660 (Character* unk_660) and PlayerSavegame::m_questSynced (+0x14) are declared. The original first looks up the local player (GetLocalPlayer(0, true) on
// dword_99F76C). It reads the pointer at PlayerInfo +0x660, which is the Character (see GameObject::IsPlayerNearby
// in game-world/ledger.d). It then reads Character::m_savegame (+0x14e8, declared in Character.hpp) and that
// PlayerSavegame's m_questSynced (+0x14, declared). When the savegame is null or not quest synced, the function
// returns m_enabled without checking the condition. That gate is not reproduced: the body below assumes it is
// open and checks the disable condition.
bool ObjectBase::TestDisableCondition(bool markTested) {
    if (m_disableCond.m_tested || m_disableCond.m_list == nullptr) {  // +0xd0 (m_tested), +0xcc (m_list)
        return m_enabled;
    }
    if (!m_disableCond.IsTrue()) {
        SetEnable(false);
        if (markTested) {
            m_disableCond.SetAsTested(true);
        }
    }
    return m_enabled;
}
