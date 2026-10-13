// ObjectManager, part 3 of 4 (batch g1-game-world-ObjectManager-5017c7-2).
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original
// 32-bit ones. Functions that need a field or a class that is not in the tree yet are marked partial in the
// ledger; the blocking dependency is named next to each of them.
//
// Two blockers recur in this file:
//   - ObjectListItem (the value type of std::map<int, ObjectListItem> m_objectMap) has no definition in the
//     tree (only a forward declaration in ObjectManager.hpp). Any operation on m_objectMap, and the implicit
//     destruction of ObjectManager's members, needs the complete type. So the constructors and destructors
//     are not written here, and the m_objectMap steps of Flush, Remove, FakeRemove, GetObjectByName and Add
//     are left out.
//   - COnline::isOnlineGame_ (COnline +0x05) is protected with no getter. As in GameObject-2, the flag reads as
//     offline through OnlineGameFlagUnavailable(), so the online branches are not taken.

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <list>
#include <string>
#include <vector>

#include "dh2/Application.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/RoomZone.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/global/net_singletons.hpp"
#include "net-session-online/c_matching_base.hpp"
#include "net-transport/net_bit_stream.hpp"

namespace {

// COnline::isOnlineGame_ (+0x05) has no getter in net-session-online/c_online.hpp. Until one exists this
// reads as offline, so every online-only branch in this file is skipped.
bool OnlineGameFlagUnavailable() { return GetOnlineGameFlag(); }

// Removes every node whose value equals `value`. Matches the original loops that unlink each match.
template <typename T>
void EraseAll(std::list<T>& list, const T& value) {
    list.remove(value);
}

// Removes only the first node whose value equals `value`. Matches the original loops that break after one unlink.
template <typename T>
void EraseFirst(std::list<T>& list, const T& value) {
    auto it = std::find(list.begin(), list.end(), value);
    if (it != list.end()) {
        list.erase(it);
    }
}

}  // namespace

// 0x34735c ObjectManager::LoadGameObjectNetData(int,int,NetBitStream &)
// PARTIAL. Written: the level check, the stream header with its unk_b0, unk_c8 and unk_e0 bookkeeping, and the
// packet-validity test. NOT WRITTEN: the per-entry loop (entry count from ReadU32(16)) and the trailing set
// updates. The loop needs three things that are not recovered: (1) the NetStruct virtual slots +0x14 and +0x1c
// (Load, Erase in net_struct.hpp; the Hex-Rays calls pass the stream the way a read does, so the names are not
// trusted) and (2) the object construction ids: the Character and Trigger constructors show no GO_IDS argument in
// the pseudocode and the Container id is not in object_factory.hpp. The Door case is GetNewInstance<Door>, which
// matches the pseudocode's Door::Door(obj, 2). Once the loop is written the sets unk_17c and unk_194 and the
// cleanup run.
void ObjectManager::LoadGameObjectNetData(int objectId, int member, NetBitStream& stream) {
    Level* currentLevel = ApplicationGetInstance().GetCurrentLevel();
    if (currentLevel == nullptr ||
        static_cast<int>(stream.ReadByte(8)) != currentLevel->m_levelListIndex) {
        return;
    }

    // v7 and v79 in the pseudocode. The header bit is clear: the id is -1 and nothing is booked.
    std::int16_t headerByte = -1;
    bool headerPresent = false;
    if (stream.ReadBit() != 0) {
        headerByte = static_cast<std::int16_t>(stream.ReadByte(8));
        // unk_b0 is the highest header byte seen for this object, unk_e0 counts the repeats of that byte, and
        // unk_c8 is reset to -1 whenever a new highest byte arrives. A missing key is booked as -1 before the
        // compare (LABEL_149 in the pseudocode), so a first-seen object with header byte 0 still takes the reset
        // branch. operator[] alone would book 0 there and skip the reset.
        if (unk_b0.find(objectId) == unk_b0.end()) {
            unk_b0[objectId] = -1;
        }
        if (unk_b0[objectId] < headerByte) {
            unk_b0[objectId] = headerByte;
            unk_e0[objectId] = 0;
            unk_c8[objectId] = -1;
        }
        headerPresent = true;
        if (unk_b0[objectId] == headerByte) {
            ++unk_e0[objectId];
        }
    }

    const std::uint32_t entryCount = stream.ReadU32(16);
    // Hex-Rays leaves IsPacketValid unset when entryCount is zero. It is false here.
    bool isPacketValid = false;
    if (entryCount != 0) {
        isPacketValid = IsPacketValid(objectId, member);
        if (headerPresent) {
            isPacketValid = (unk_b0[objectId] == headerByte);
        }
        // NOT WRITTEN: the entry loop (see the row note).
    }
    (void)isPacketValid;
}

// 0x347d90 ObjectManager::sReadPacketData(int,int,NetBitStream &)
// Static. The pseudocode's first parameter is the object id (the Hex-Rays "this"). It forwards to the instance.
void ObjectManager::sReadPacketData(int objectId, int member, NetBitStream& stream) {
    if (stream.ReadBit() != 0) {
        if (dword_99F764 != nullptr) {
            dword_99F764->LoadGameObjectNetData(objectId, member, stream);
        }
    }
}

// 0x347dec ObjectManager::IsOnlineDeferred(ObjectBase *)
// True when the object is listed in unk_108 under one of the connected member ids. operator[] inserts an empty
// list for a missing member id, as the original does.
bool ObjectManager::IsOnlineDeferred(ObjectBase* object) {
    const std::vector<int> members = CMatching::Get()->ConnectedMemberIds();
    for (int memberId : members) {
        std::list<ObjectBase*>& deferred = unk_108[static_cast<std::int16_t>(memberId)];
        for (ObjectBase* entry : deferred) {
            if (entry == object) {
                return true;
            }
        }
    }
    return false;
}

// 0x347fd0 ObjectManager::ForceFullUpdate(void)
// PARTIAL. The original returns at once unless COnline::isOnlineGame_ is set, so the offline path is exact. The
// online path is NOT WRITTEN. It needs (1) the OnlineGameState state read at OnlineSingleton<OnlineGameState>
// +0x34 (the state test (state - 3) <= 1 picks between two paths; online_game_state.hpp has no state getter),
// (2) the unk_108 clear, and (3) the CMatching member list walk that feeds unk_17c and unk_194.
void ObjectManager::ForceFullUpdate() {
    if (!OnlineGameFlagUnavailable()) {
        return;
    }
    // NOT WRITTEN: the online path (see the row note).
}

// 0x348644 ObjectManager::ProcessLostPacket(int,int)
// When the packet id has an entry in unk_98 that lists the member, the whole map is cleared and a full update
// is forced.
void ObjectManager::ProcessLostPacket(int packetId, int member) {
    auto found = unk_98.find(packetId);
    if (found == unk_98.end()) {
        return;
    }
    const std::list<int>& members = found->second;
    if (std::find(members.begin(), members.end(), member) == members.end()) {
        return;
    }
    if (!unk_98.empty()) {
        unk_98.clear();
    }
    ForceFullUpdate();
}

// 0x348724 ObjectManager::sProcessLostPacket(int,int)
// Static. Forwards to the instance.
void ObjectManager::sProcessLostPacket(int packetId, int member) {
    if (dword_99F764 != nullptr) {
        dword_99F764->ProcessLostPacket(packetId, member);
    }
}

// 0x348988 ObjectManager::Draw3D(void)
// PARTIAL, body not written. The first half draws every visible object in m_objectMap in key order (Draw() on
// the ObjectBase, vtable +0x30, when m_visible is set). It needs the ObjectListItem definition for the object
// pointer. The second half is the debug culling-box overlay (DebugSwitches "isDisplayingCullingBoxes"). It
// needs the glitch render backend: IVideoDriver setMaterial, CMaterialRendererManager, the plane3d
// intersection helpers and DBG_DisplayPointAsBox, none of which have a header in the tree.
void ObjectManager::Draw3D() {
    // NOT WRITTEN: see the row note.
}

// 0x348ea4 ObjectManager::Remove(ObjectHandle)
// PARTIAL. Written: the room, no-room and update-list unlinks, the character unlinks with CharAI::RemoveFromGroup,
// the unk_50 decrement and the generation bump. NOT WRITTEN: (1) the unk_68 unlink for object
// type 5 (layout request: the original links ObjectBase* into unk_68 here and in Add, but the header declares it
// as std::list<Module*>), (2) the online branch that moves Player objects into unk_120 (offline only, so
// skipped), and (3) the m_objectMap erase and the AddOrphanRenderObjectToDelete / delete step (ObjectListItem).
void ObjectManager::Remove(ObjectHandle handle) {
    GameObject* object = handle;
    if (object != nullptr) {
        if (RoomZone* zone = object->m_roomZone) {
            zone->RemoveObject(object);
        }
        RemoveNoRoomObject(object);
        EraseFirst(unk_90, object);
    }

    ObjectBase* base = handle.GetObject(false);
    EraseAll(m_updateList, base);

    Character* character = handle;
    EraseAll(unk_70, character);
    if (character != nullptr) {
        EraseAll(m_characters, character);
        character->CharAI::RemoveFromGroup();
    }

    // NOT WRITTEN: the unk_68 unlink for object type 5 (layout request, see the row note).
    // NOT WRITTEN: the online branch (offline only, skipped).

    --unk_50;
    // NOT WRITTEN: the m_objectMap erase and the orphan or delete step (ObjectListItem).
    ++m_generation;
}

// 0x349240 ObjectManager::FakeRemove(ObjectHandle)
// PARTIAL. Same unlinks as Remove, plus the mark for deletion (ObjectBase::m_markedForDeletion, +0x81) and the
// Character::Clean call. NOT WRITTEN: the unk_68 unlink for object type 5 (layout request, as for Remove) and
// the m_objectMap erase with AddOrphanRenderObjectToDelete (ObjectListItem).
void ObjectManager::FakeRemove(ObjectHandle handle) {
    GameObject* object = handle;
    object->m_markedForDeletion = 1;
    if (RoomZone* zone = object->m_roomZone) {
        zone->RemoveObject(object);
    }
    RemoveNoRoomObject(object);
    EraseFirst(unk_90, object);

    ObjectBase* base = handle.GetObject(false);
    EraseAll(m_updateList, base);

    Character* character = handle;
    EraseAll(unk_70, character);
    if (character != nullptr) {
        EraseAll(m_characters, character);
        character->CharAI::RemoveFromGroup();
        character->Clean();
    }

    // NOT WRITTEN: the unk_68 unlink for object type 5 (layout request).

    ObjectBase* online = handle.GetObject(false);
    EraseAll(unk_100, online);

    // NOT WRITTEN: the m_objectMap erase and AddOrphanRenderObjectToDelete (ObjectListItem).
}

// 0x3496b8 ObjectManager::Flush(void)
// PARTIAL. Written: the character pass (FlushTargetList, Delete, then CharAI::_UpdatePointers on each character's
// CharAI base) and every list, map and counter reset except the m_objectMap steps. NOT WRITTEN: the m_objectMap
// pass that deletes each object and nulls its entry, the m_objectMap clear, and the entry for key 0 that the
// original inserts (ObjectListItem). The original calls _UpdatePointers with a null this when a list entry is
// null. The character list never holds null, so that case is not written.
void ObjectManager::Flush() {
    for (Character* character : m_characters) {
        character->GameObject::FlushTargetList();
        character->ObjectBase::Delete();
    }
    for (Character* character : m_characters) {
        static_cast<CharAI*>(character)->_UpdatePointers();
    }

    // NOT WRITTEN: the m_objectMap delete pass and clear (ObjectListItem).

    unk_90.clear();
    m_characters.clear();
    unk_68.clear();
    unk_100.clear();
    unk_120.clear();
    unk_108.clear();
    unk_164.clear();
    unk_17c.clear();
    unk_194.clear();
    unk_128.clear();
    unk_130.clear();
    m_roomZones.clear();
    m_noRoomObjects.clear();
    AddRoomObjects(&m_noRoomObjects);
    unk_138 = 0;
    unk_13c = 0;
    unk_140 = 0;
    unk_148.clear();
    unk_160 = 0;

    // NOT WRITTEN: the m_objectMap[0] entry the original inserts here (ObjectListItem).

    m_nextKey = 1;
    unk_58 = 0;
    unk_50 = 0;
    unk_54 = 0;
    unk_3c.clear();
    m_updateList.clear();
    m_conditionList.clear();
    m_spawnList.clear();
    unk_70.clear();
    unk_24.clear();
    unk_7c = 0;
    FlushAllOrphanRenderObjects();
    unk_98.clear();
    unk_b0.clear();
    unk_c8.clear();
    unk_e0.clear();
}

// 0x349b14 ObjectManager::~ObjectManager()
// 0x349e8c ObjectManager::~ObjectManager()
// NOT WRITTEN (both copies, D1 and D2 with the same body). The body is Flush() followed by clears of the maps
// and lists. The members are destroyed implicitly after the body, and that implicit destruction of
// std::map<int, ObjectListItem> m_objectMap needs the complete ObjectListItem type, which is not in the tree.
// See the ledger rows for the blocker.

// 0x349e70 ObjectManager::~ObjectManager()
// Deleting destructor (D0): compiler-generated for a virtual destructor, which runs ~ObjectManager and then frees
// the object. Not rebuilt.

// 0x34a1e8 ObjectManager::ObjectManager(void)
// 0x34a404 ObjectManager::ObjectManager(void)
// NOT WRITTEN (both copies with the same body). The body zeroes the scalar members, sets the list and map
// sentinels, and ends with Flush(). The member constructors and the implicit destruction on an exception need
// the complete ObjectListItem type, which is not in the tree. See the ledger rows for the blocker.

// 0x34a620 ObjectManager::Update(float)
// PARTIAL, body not written. The original runs in this order: the online flags (offline stand-in), the level
// gate (Level +324 and +408, not named in Level.hpp), UpdateRooms, DoRemoteUpdateUpdate, ProcessNextGameObjectToStartUpdate,
// the splice of m_spawnList into m_updateList, the m_conditionList test, the unk_17c-driven character pass with
// MarkForDeletion, FakeRemove and Remove, the AI pointer updates, the concurrent-AI unload and the two debug
// switch resets. Blocked on the GameObject fields that the condition walk tests and that GameObject.hpp does not
// name, on the Level flags +324 and +408, and on the iterator form of Character::UnLoadScriptProcess.
void ObjectManager::Update(float dt) {
    (void)dt;
    // NOT WRITTEN: see the row note.
}

// 0x34aca0 ObjectManager::GetObjectByName(char const*,int,bool,char const*)
// PARTIAL. Written: the flag rewrite for the player names (Player, PlayerCharacter_, LocalPlayer sets flags to
// -1). NOT WRITTEN: the m_objectMap walk (ObjectListItem: name at +0x14, object at +0x18), the "HighestThreatPlayer"
// dispatch (the Hex-Rays argument order for the GetHighestThreatPlayer call does not match the header), the new
// entry path with m_nextKey, and the tracing switch. The result is a null handle until the walk is written.
ObjectHandle ObjectManager::GetObjectByName(char const* name, int flags, bool active, char const* type) {
    (void)active;
    (void)type;
    if (std::strncmp(name, "Player", 6) == 0 || std::strncmp(name, "PlayerCharacter_", 16) == 0 ||
        std::strncmp(name, "LocalPlayer", 11) == 0) {
        flags = -1;
    }
    (void)flags;
    // NOT WRITTEN: the m_objectMap walk and the entry handling (ObjectListItem).
    return ObjectHandle();
}

// 0x34b064 ObjectManager::GetHighestThreatPlayer(char const*,int,bool)
// PARTIAL. The body follows the pseudocode: look the named object up, take its CharAI highest aggro target, and
// return that object's handle, or a null handle with the tracing switch read. Two gaps: (1) the header declares
// (name, int flags, bool active) while the pseudocode takes a name and one 4-byte flag argument, so the active
// argument is unused here; (2) when the name does not resolve, the original calls AI_GetHighestAggro on address
// 968 (a null deref), which is not reproduced. The lookup goes through the partial GetObjectByName.
ObjectHandle ObjectManager::GetHighestThreatPlayer(char const* name, int flags, bool active) {
    (void)active;
    ObjectHandle player = GetObjectByName(name, flags, false, nullptr);
    Character* character = player;
    Character* aggro = character->AI_GetHighestAggro();
    if (aggro != nullptr) {
        return GetObjectByPtr(aggro);
    }
    DebugSwitches::s_inst.load();
    DebugSwitches::s_inst.GetSwitch("isTracingObjectManager::");
    return ObjectHandle(nullptr);
}

// 0x34b270 ObjectManager::Add(ObjectBase *,char const*,char const*,int,bool)
// PARTIAL, body not written. The original asserts that the object and the name are present, reserves a handle with
// GetObjectByName(name, flags, true), deletes the new object when the name already resolves, and otherwise stores
// the object in m_objectMap (ObjectListItem), sets its name, type and flags, appends it to unk_70 and to the
// Module list (unk_68, layout request), and assigns a network id when active is set.
ObjectHandle ObjectManager::Add(ObjectBase* object, char const* name, char const* type, int flags, bool active) {
    (void)object;
    (void)name;
    (void)type;
    (void)flags;
    (void)active;
    // NOT WRITTEN: see the row note.
    return ObjectHandle();
}

// 0x34b520 ObjectManager::GetNewObject(char const*,char const*,int,bool)
// PARTIAL, body not written. The original scans a 33-entry table of {type name, factory} pairs (objectCreationMap,
// not declared anywhere in the tree), calls the matching factory, and passes the new object to Add. When no entry
// matches it reads the tracing switch and returns a null handle.
ObjectHandle ObjectManager::GetNewObject(char const* type, char const* name, int flags, bool active) {
    (void)type;
    (void)name;
    (void)flags;
    (void)active;
    // NOT WRITTEN: see the row note.
    return ObjectHandle();
}
