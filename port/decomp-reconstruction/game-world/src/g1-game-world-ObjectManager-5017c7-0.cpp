// Batch g1-game-world-ObjectManager-5017c7-0 (game-world, owner ObjectManager, part 1 of 4): the per-frame and
// online walks over the object lists, the room list, the network id lookups, the AI re-init and the network level
// hooks. Declarations come from dh2/ObjectManager.hpp.
//
// Owner-header edits made in this batch (small, in place): DoRemoteUpdateUpdate returns void (the float the pseudocode
// returns is the register that held this); the list at +0x24 holds RoomZone* and the list at +0x68 holds Module*
// (the pseudocode dereferences the nodes as those types); the m_initialized comment now matches the binary.
//
// Layout facts used here (original 32-bit offsets, the names come from dh2/ObjectBase.hpp and dh2/ObjectManager.hpp):
//   ObjectManager +0x24 unk_24 RoomZone* list      +0x2c m_updateList ObjectBase* list (AI init)
//   ObjectManager +0x80 m_roomZones list of room object lists
//   ObjectManager +0x100 unk_100 ObjectBase* list  (online objects: flush, remote update, net-id lookup, delete)
//   ObjectManager +0x68 unk_68 Module* list        (fog colour)
//   ObjectManager +0xf8 unk_f8                     (zeroed by UpdateRooms and ForceEverythingVisible)
//   ObjectManager +0x1ac m_initialized             (NetworkInitLevel sets 1, NetworkUnInitLevel sets 0)
//   ObjectBase +0x100 m_netStruct, +0x108 unk_108 (the network id), +0x110 m_netOwnerId, +0x114 unk_114 (timer),
//   +0x118 unk_118, +0x119 m_netUpdateRequested, +0x8c m_enableCond, +0xb0 m_disableCond
//   Character +0x11c (ObjectBase unk_11c, resend counter), +0x1480 m_isDummy
//
// Status: 15 bodies are reconstructed. 5 are PARTIAL and each names what it waits on:
//   IsRemotePlayerOfMember (the player record's +0x198 pointer has no declared member),
//   IsObjectSerializable (its last branch calls IsRemotePlayerOfMember),
//   NetworkInitLevel (COnline +0x05 is protected and the packet callbacks are non-static members),
//   GetDynamicFogColorAtPos (Module::UNINIT_FOG_COLOR is declared as one word, the test reads three floats),
//   GetObjectsByType (ObjectListItem has no layout).
// No standalone test: every body calls the engine (virtual calls, list walks over engine objects, RNG, packets),
// and linking this translation unit needs the whole engine.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <list>

// Character.hpp comes first: it includes net-transport's net_struct.hpp with the two clashing deps names renamed (see
// its own note), so the net headers below can follow without redefining CustomFree.
#include "dh2/Character.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/ConditionData.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Module.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Point3D.hpp"
#include "dh2/Random.hpp"
#include "dh2/RoomZone.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"          // dword_99F76C, the PlayerManager instance
#include "dh2/global/net_singletons.hpp"             // GetOnline
#include "net-session-online/c_matching_base.hpp"    // CMatching::Get, CMatching::IsServer
#include "net-session-online/c_online.hpp"           // COnline::IsServer
#include "net-transport/net_struct.hpp"              // NetStruct::Resend (already included via Character.hpp)
#include "net-transport/packet_manager.hpp"          // CPacketManager::UnregisterPacketSlot

// The global assert level (gAssertLevel at 0x99f914). core-platform/memory.hpp declares it too, but that header's glitch
// stand-ins clash with the glitch scene headers that Character.hpp pulls in, so it is declared here.
extern int gAssertLevel;

// Runtime support (<global>), declared here as in GameObject-5d2019-3.cpp: dh2/global/runtime_support.hpp includes
// core-platform/memory.hpp, which clashes with the same glitch headers.
void PushProfilingContext(const char* name);  // 0x3136b4
void PopProfilingContext(const char* name);   // 0x3136b8

namespace {

// The source path the original's ASSERT messages print (ObjectManager.cpp in the Visual Studio project).
const char kObjectManagerSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\ObjectManager\\ObjectManager.cpp";

// The original ASSERT(expression) at line. Level 2 stops the program (the original stores to address 0 to crash);
// level 1 prints the message and the function goes on.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kObjectManagerSource, line);
    }
}

}  // namespace

// 0x34024c ObjectManager::Draw2D(void)
void ObjectManager::Draw2D() {
}

// 0x340250 ObjectManager::DoOnlineStateFlush(void)
// Clears the network update request (+0x119) of every object on the online list.
void ObjectManager::DoOnlineStateFlush() {
    for (ObjectBase* object : unk_100) {
        object->m_netUpdateRequested = 0;  // +0x119
    }
}

// 0x340274 ObjectManager::DoRemoteUpdateUpdate(float)
// For each remotely updated object on the online list: the timer (+0x114) grows by dt while it is below 50.0. At 50.0
// or more the timer is reset to 0 and the owner id (+0x110) is set to -1. The pseudocode returns a float, which is the
// register that held this (the list head or the last timer); nothing reads it, so the function returns void.
void ObjectManager::DoRemoteUpdateUpdate(float dt) {
    for (ObjectBase* object : unk_100) {
        if (!object->IsRemotelyUpdated()) {
            continue;
        }
        if (object->unk_114 < 50.0f) {  // +0x114
            object->unk_114 += dt;
        } else {
            object->unk_114 = 0.0f;
            object->m_netOwnerId = -1;  // +0x110
        }
    }
}

// 0x3402f4 ObjectManager::GetNetworkIdByObjectBase(ObjectBase const*)
int ObjectManager::GetNetworkIdByObjectBase(ObjectBase const* object) {
    if (object) {
        return object->unk_108;  // +0x108, the network id
    }
    return -1;
}

// 0x340304 ObjectManager::DBG_DumpRoomObjects(void)
// The release build keeps only the walk over the room object lists (+0x80); the print calls are absent from the binary.
void ObjectManager::DBG_DumpRoomObjects() {
    for (std::list<GameObject*>* roomObjects : m_roomZones) {
        for (GameObject* object : *roomObjects) {
            (void)object;
        }
    }
}

// 0x34064c ObjectManager::DoCharAIInit(void)
// For each Character on the update list (+0x2c): its AI scripts are cleaned up and initialised again.
void ObjectManager::DoCharAIInit() {
    for (ObjectBase* object : m_updateList) {
        if (object && object->IsCharacter()) {
            Character* character = static_cast<Character*>(object);
            character->CharAI::AI_ScriptCleanUp();
            character->CharAI::AI_ScriptInit();
        }
    }
}

// 0x3406ac ObjectManager::ObjectReInit(void)
// For each object on the update list: both conditions (+0x8c, +0xb0) are marked untested, and each Character then
// has its AI scripts cleaned up and initialised again.
void ObjectManager::ObjectReInit() {
    for (ObjectBase* object : m_updateList) {
        if (object) {
            object->m_enableCond.SetAsTested(false);   // +0x8c
            object->m_disableCond.SetAsTested(false);  // +0xb0
            if (object->IsCharacter()) {
                Character* character = static_cast<Character*>(object);
                character->CharAI::AI_ScriptCleanUp();
                character->CharAI::AI_ScriptInit();
            }
        }
    }
}

// 0x34072c ObjectManager::DeleteRandomOnlineObject(void)
// The pseudocode takes an int and a bool and returns an int. The int is unused. The bool is passed through as the third
// argument of Random::GetRandom, which takes two, so it has no effect. The return value is either the GetRandom result
// or r0 left by ObjectBase::Delete (void), so the function returns void.
// The random draw is made even when the list is empty (range -4), so the generator advances as in the original. The
// first four entries of the list are never chosen: the target index is roll + 4, with roll in [0, count - 4).
void ObjectManager::DeleteRandomOnlineObject() {
    const int count = static_cast<int>(unk_100.size());
    const unsigned int roll = Random::GetRandom(count - 4, false);  // ordinary stream
    const unsigned int target = roll + 4;
    unsigned int index = 0;
    for (ObjectBase* object : unk_100) {
        if (index == target) {
            object->Delete();
            return;
        }
        ++index;
    }
}

// 0x3407a0 ObjectManager::GetObjectHandleByNetworkId(int)
// The first object on the online list whose network id (+0x108) equals networkId, as a handle. Otherwise an empty handle.
ObjectHandle ObjectManager::GetObjectHandleByNetworkId(int networkId) {
    for (ObjectBase* object : unk_100) {
        if (object && object->unk_108 == networkId) {
            return object->GetHandle();
        }
    }
    return ObjectHandle(nullptr);
}

// 0x340848 ObjectManager::CanSendUpdate(ObjectBase*)
bool ObjectManager::CanSendUpdate(ObjectBase* object) {
    if (!object || object->IsRemotelyUpdated() || !object->m_netStruct) {
        return false;
    }
    ObjectHandle handle = object->GetHandle();
    Character* character = handle;  // ObjectHandle::operator Character*
    if (!character || character->unk_11c > 0) {
        return true;
    }
    if (!character->CanUpdate() || character->m_isDummy != 0 || character->IsMerchant() ||
        (character->IsDead() && character->SM_IsInLimbus() && character->CanRespawn())) {
        return false;
    }
    return !character->IsPlayer();
}

// 0x340948 ObjectManager::IsRemotePlayerOfMember(int,Character*)
// PARTIAL: the original walks the member's player ids (CNetPlayerManager::GetPlayerIdListByMemberId on GetNetPlayerMgr()).
// For each id it takes PlayerManager::GetPlayerByInternalID(dword_99F76C, id, false) and reads the pointer at the player
// record's +0x198 (PlayerInfo +408), then returns true when that object's network id (+0x108) equals the character's.
// PlayerInfo has no declared member at +0x198 (layout request in the ledger), so the walk is not written and every
// call returns false.
bool ObjectManager::IsRemotePlayerOfMember(int member, Character* character) {
    (void)member;
    if (!character) {
        return false;
    }
    return false;
}

// 0x3409f4 ObjectManager::IsObjectSerializable(ObjectBase*,int)
// PARTIAL: every branch matches the pseudocode except the last one, which returns !IsRemotePlayerOfMember(mode, character).
// That call is a partial stub (see IsRemotePlayerOfMember), so the result of this function is only as good as that one.
bool ObjectManager::IsObjectSerializable(ObjectBase* object, int mode) {
    if (!object) {
        return false;
    }
    if (GetOnline()->IsServer()) {
        const int owner = object->m_netOwnerId;  // +0x110
        if (owner != -1 && mode != owner) {
            return true;
        }
    }
    if (!object->m_netUpdateRequested || !object->m_netStruct || object->IsRemotelyUpdated() || object->unk_118 ||
        mode == object->m_netOwnerId) {
        return false;
    }
    ObjectHandle handle = object->GetHandle();
    Character* character = handle;  // ObjectHandle::operator Character*
    if (!character) {
        return true;
    }
    if (character->unk_11c > 0) {
        object->m_netStruct->Resend();
        if (character->unk_11c > 0) {
            character->unk_11c = character->unk_11c - 1;
        }
        return true;
    }
    if (!character->CanUpdate() || character->m_isDummy != 0 || character->IsMerchant() ||
        (character->IsDead() && character->SM_IsInLimbus() && character->CanRespawn())) {
        return false;
    }
    if (!character->IsPlayer()) {
        return true;
    }
    if (!CMatching::Get()->CMatching::IsServer() &&
        dword_99F76C->GetPlayerByCharacter(character, false)->unk_66c != 1) {
        return false;
    }
    return !IsRemotePlayerOfMember(mode, character);
}

// 0x340bc4 ObjectManager::NetworkUnInitLevel(void)
void ObjectManager::NetworkUnInitLevel() {
    CPacketManager::UnregisterPacketSlot(3);
    m_initialized = 0;  // +0x1ac
}

// 0x340be0 ObjectManager::NetworkInitLevel(void)
// PARTIAL: the original registers packet slot 3 when the online-game flag of GetOnline() (COnline +0x05, protected, with
// no public getter) is set: CPacketManager::RegisterPacketSlot(3, sWritePacketData, sReadPacketData,
// sProcessAcknowledgedPacket, sProcessLostPacket). That registration is not written. The four callbacks are declared as
// non-static members of ObjectManager, and RegisterPacketSlot takes plain function pointers, so they need static
// declarations first (layout request in the ledger).
void ObjectManager::NetworkInitLevel() {
    m_initialized = 1;  // +0x1ac
}

// 0x340c54 ObjectManager::GetObjectByPtr(ObjectBase*)
// The handle of the object, or an empty handle when the object is null. The manager itself is not used.
ObjectHandle ObjectManager::GetObjectByPtr(ObjectBase* object) {
    if (object) {
        return object->GetHandle();
    }
    return ObjectHandle();
}

// 0x34163c ObjectManager::UpdateRooms(void)
void ObjectManager::UpdateRooms() {
    PushProfilingContext("ObjectManager::UpdateRooms");
    unk_f8 = 0;
    for (RoomZone* zone : unk_24) {
        zone->Update();
    }
    PopProfilingContext("ObjectManager::UpdateRooms");
}

// 0x34169c ObjectManager::ForceEverythingVisible(void)
void ObjectManager::ForceEverythingVisible() {
    unk_f8 = 0;
    for (RoomZone* zone : unk_24) {
        zone->Activate();
    }
}

// 0x3419a8 ObjectManager::GetDynamicFogColorAtPos(Point3D<float> const&,float)
// Blends the fog colours of the modules on the list (+0x68) by distance. Each module within radius * 100 contributes its
// fog colour scaled by (range - distance) / range. Each component is then capped at 255.0. Lower values are not clamped.
// PARTIAL: the original skips a module whose fog colour equals Module::UNINIT_FOG_COLOR, a Point3D comparison against the
// three floats at that symbol. Module declares the symbol as one std::int32_t, so the test is not written and every module
// in range contributes (layout request in the ledger).
Point3D<float> ObjectManager::GetDynamicFogColorAtPos(Point3D<float> const& pos, float radius) {
    Point3D<float> color{};
    const float scaledRadius = radius * 100.0f;
    for (Module* module : unk_68) {
        const float dx = module->m_position.X - pos.X;
        const float dy = module->m_position.Y - pos.Y;
        const float dz = module->m_position.Z - pos.Z;
        const float distance = std::sqrt((dx * dx + dy * dy) + dz * dz);
        if (scaledRadius < distance) {
            continue;
        }
        const float weight = (scaledRadius - distance) / scaledRadius;
        color.X = color.X + weight * module->m_fogColor.X;
        color.Y = color.Y + weight * module->m_fogColor.Y;
        color.Z = color.Z + weight * module->m_fogColor.Z;
    }
    if (color.X > 255.0f) {
        color.X = 255.0f;
    }
    if (color.Y > 255.0f) {
        color.Y = 255.0f;
    }
    if (color.Z > 255.0f) {
        color.Z = 255.0f;
    }
    return color;
}

// 0x3426b0 ObjectManager::GetObjectsByType(char const*,std::list<Character*>&) const
// PARTIAL, body not written: the original walks the object map (m_objectMap, std::map<int, ObjectListItem>) in order.
// It compares PropertyMap::GetThisClassName of each object with the type and appends each match, as a Character*
// resolved from the map key, to the list. ObjectListItem has no declared layout (layout request in the ledger), so the
// walk is not written and the list is left unchanged.
void ObjectManager::GetObjectsByType(char const* type, std::list<Character*>& out) const {
    (void)type;
    (void)out;
}

// 0x3427a0 ObjectManager::AddRoomObjects(std::list<GameObject*>*)
// Registers a room object list. Null and an already registered list each raise the original ASSERT (which stops at
// level 2 and prints at level 1). The list is then appended to m_roomZones.
void ObjectManager::AddRoomObjects(std::list<GameObject*>* objects) {
    if (!objects) {
        ReportAssert("ro", 2313);
    }
    if (std::find(m_roomZones.begin(), m_roomZones.end(), objects) != m_roomZones.end()) {
        ReportAssert("std::find(m_roomObjectsList.begin(), m_roomObjectsList.end(), ro) == m_roomObjectsList.end()", 2314);
    }
    m_roomZones.push_back(objects);
}
