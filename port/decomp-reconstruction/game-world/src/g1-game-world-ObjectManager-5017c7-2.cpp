// ObjectManager, part 3 of 4 (batch g1-game-world-ObjectManager-5017c7-2).
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original
// 32-bit ones. The object id map is std::map<int, ObjectListItem> m_objectMap (dh2/ObjectListItem.hpp): an entry holds the
// object's name and the object pointer.

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
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
#include "dh2/Module.hpp"
#include "dh2/ObjectListItem.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"
#include "dh2/global/object_factory.hpp"
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

// objectCreationMap (0x95c800): the 33 {type name, factory} pairs GetNewObject scans in order. "Block" shares the Module factory and "Player" the Character factory.
struct ObjectCreationEntry {
    const char* typeName;
    ObjectBase* (*factory)();
};

const ObjectCreationEntry objectCreationMap[33] = {
    {"Decor", &GetNewInstance<Decor>},
    {"AnimatedDecor", &GetNewInstance<AnimatedDecor>},
    {"Dummy", &GetNewInstance<Dummy>},
    {"Floor", &GetNewInstance<Floor>},
    {"LightPoint", &GetNewInstance<LightPoint>},
    {"ColBox", &GetNewInstance<ColBox>},
    {"Billboard", &GetNewInstance<Billboard>},
    {"Module", &GetNewInstance<Module>},
    {"Block", &GetNewInstance<Module>},
    {"Character", &GetNewInstance<Character>},
    {"Player", &GetNewInstance<Character>},
    {"RoomZone", &GetNewInstance<RoomZone>},
    {"QuestMoveInZone", &GetNewInstance<QuestMoveInZone>},
    {"CheckpointZone", &GetNewInstance<CheckpointZone>},
    {"TriggerObject", &GetNewInstance<TriggerObject>},
    {"TriggerPlate", &GetNewInstance<TriggerPlate>},
    {"TriggerZone", &GetNewInstance<TriggerZone>},
    {"TriggerZoneExitLevel", &GetNewInstance<TriggerZoneExitLevel>},
    {"TriggerTrap", &GetNewInstance<TriggerTrap>},
    {"TimerTrap", &GetNewInstance<TimerTrap>},
    {"ProjectileTrap", &GetNewInstance<ProjectileTrap>},
    {"Door", &GetNewInstance<Door>},
    {"SpawnPoint", &GetNewInstance<SpawnPoint>},
    {"SpawnSpot", &GetNewInstance<SpawnSpot>},
    {"LiftableObject", &GetNewInstance<LiftableObject>},
    {"OpenableContainer", &GetNewInstance<OpenableContainer>},
    {"SlotContainer", &GetNewInstance<SlotContainer>},
    {"DestructibleContainer", &GetNewInstance<DestructibleContainer>},
    {"Item", &GetNewInstance<ItemObject>},
    {"Projectile", &GetNewInstance<Projectile>},
    {"LaserTypeProjectile", &GetNewInstance<LaserTypeProjectile>},
    {"SoundEmitter", &GetNewInstance<SoundEmitter>},
    {"LevelConfig", &GetNewInstance<LevelConfig>},
};

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
// PARTIAL. Written: the first half, which draws every visible object of m_objectMap in key order (Draw() on the ObjectBase,
// vtable +0x30, when m_visible is set; the map entry's object is at value +0x18). Not written: the render-state call the
// original makes before the loop (vtable slot +0x6c of the scene manager's driver at dword_99F73C + 16, not declared), and
// the debug culling-box overlay (DebugSwitches "isDisplayingCullingBoxes"). The overlay needs the glitch render backend:
// IVideoDriver setMaterial, CMaterialRendererManager, the plane3d intersection helpers and DBG_DisplayPointAsBox, none of
// which have a header in the tree.
void ObjectManager::Draw3D() {
    for (auto& entry : m_objectMap) {
        ObjectBase* object = entry.second.m_object;
        if (object != nullptr && object->m_visible) {
            object->Draw();
        }
    }
}

// 0x348ea4 ObjectManager::Remove(ObjectHandle)
// Unlinks the object everywhere: its room (RoomZone::RemoveObject) and the no-room list, the first entry of the list at +0x90, every entry of the update list (+0x2c) and of the list at +0x70,
// and for a Character the character list (+0x60) and its AI group (CharAI::RemoveFromGroup); a Module (GO_IDS 5) leaves the module list (+0x68). Online, the entry of the online list
// (+0x100) that is the map entry's object is removed, and its network id is remembered in unk_120 unless it is a player character. Then the object count (unk_50) drops. An object flagged
// unk_2fc (GameObject +0x2fc) is handed to AddOrphanRenderObjectToDelete; any other is deleted through its virtual destructor. The map entry is erased and the generation (+0x78) is raised.
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

    base = handle.GetObject(false);
    if (base != nullptr && base->m_goId == 5) {
        EraseAll(unk_68, static_cast<Module*>(base));
    }

    if (OnlineGameFlagUnavailable()) {
        const int key = static_cast<int>(handle.m_id);
        for (auto it = unk_100.begin(); it != unk_100.end(); ++it) {
            ObjectBase* listed = *it;
            if (listed == m_objectMap[key].m_object) {
                ObjectHandle listedHandle = listed->GetHandle();
                Character* listedCharacter = listedHandle;
                if (listedCharacter == nullptr || !listedCharacter->IsPlayer()) {
                    unk_120.push_back(static_cast<std::int16_t>(listed->unk_108));
                }
                unk_100.erase(it);
                break;
            }
        }
    }

    --unk_50;
    const int key = static_cast<int>(handle.m_id);
    if (object != nullptr && object->unk_2fc != 0) {
        AddOrphanRenderObjectToDelete(m_objectMap[key].m_object);
    } else {
        ObjectBase* entryObject = m_objectMap[key].m_object;
        if (entryObject != nullptr) {
            delete entryObject;
        }
    }
    auto entry = m_objectMap.find(key);
    if (entry != m_objectMap.end()) {
        m_objectMap.erase(entry);
    }
    ++m_generation;
}

// 0x349240 ObjectManager::FakeRemove(ObjectHandle)
// The same unlinks as Remove, with the object marked for deletion first (+0x81) and, for a Character, Character::Clean after leaving its group; the module list (+0x68, GO_IDS 5), the
// update list and the online list (+0x100, every entry) are cleaned too. The map entry is erased, and then (as the listing does) an entry for the id is created again by operator[]
// and its object, which is null, is passed to AddOrphanRenderObjectToDelete (which ignores null): the render object is not queued.
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

    base = handle.GetObject(false);
    if (base != nullptr && base->m_goId == 5) {
        EraseAll(unk_68, static_cast<Module*>(base));
    }

    ObjectBase* online = handle.GetObject(false);
    EraseAll(unk_100, online);

    const int key = static_cast<int>(handle.m_id);
    auto entry = m_objectMap.find(key);
    if (entry != m_objectMap.end()) {
        m_objectMap.erase(entry);
    }
    AddOrphanRenderObjectToDelete(m_objectMap[key].m_object);
}

// 0x3496b8 ObjectManager::Flush(void)
// Every Character is told to flush its target list and is deleted (ObjectBase::Delete); the AI pointers of the characters are then updated. Every object of the map is deleted
// and its entry nulled, and the map is emptied. Every list, map and counter is reset, the no-room list is registered as a room list again, the map gets its entry for key 0, the key counter
// restarts at 1, the orphan render objects are deleted, and the packet bookkeeping maps are emptied. The original calls _UpdatePointers with a null this when a list entry is null; the
// character list never holds null, so that case is not written.
void ObjectManager::Flush() {
    for (Character* character : m_characters) {
        character->GameObject::FlushTargetList();
        character->ObjectBase::Delete();
    }
    for (Character* character : m_characters) {
        static_cast<CharAI*>(character)->_UpdatePointers();
    }

    for (auto& entry : m_objectMap) {
        if (entry.second.m_object != nullptr) {
            delete entry.second.m_object;
            entry.second.m_object = nullptr;
        }
    }
    unk_90.clear();
    m_objectMap.clear();
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
    static_cast<void>(m_objectMap[0]);

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
// Both copies (D1 and D2) have one body: Flush(), then the lists and maps are cleared, which the implicit member destruction does.

ObjectManager::~ObjectManager() {
    Flush();
}

// 0x349e70 ObjectManager::~ObjectManager()
// Deleting destructor (D0): compiler-generated for a virtual destructor, which runs ~ObjectManager and then frees
// the object. Not rebuilt.

// 0x34a1e8 ObjectManager::ObjectManager(void)
// 0x34a404 ObjectManager::ObjectManager(void)
// Both copies (C1 and C2) have one body: the lists and maps start empty, the scalar members (next key, counters, generation, phase, +0xf8, the initialized byte) are zeroed, and
// Flush() sets up the rest (the room list holding the no-room list, the map entry for key 0, the key counter at 1).
ObjectManager::ObjectManager()
    : m_nextKey(0),
      unk_50(0),
      unk_54(0),
      unk_58(0),
      unk_5c(0),
      m_generation(0),
      unk_7c(0),
      unk_f8(0),
      unk_fc(0),
      unk_fd(0),
      unk_138(0),
      unk_13c(0),
      unk_140(0),
      unk_160(0),
      m_initialized(0) {
    Flush();
}

// 0x34a620 ObjectManager::Update(float)
// One frame of the object manager, in the profiling scope "ObjectManager::Update". Online, unk_fc and unk_fd are set. When the current level exists, is loaded, is not in the game view and the
// game is offline nothing more happens. Then: UpdateRooms, DoRemoteUpdateUpdate, ProcessNextGameObjectToStartUpdate; the spawn list (+0x34) is appended to the update list (+0x2c) and emptied;
// the objects marked for deletion (+0x3c) are visited: one with a lock count or a delete delay is left (the delay counts down), one that is not deferred for an online member is removed (a
// player character with FakeRemove, anything else with Remove) and leaves the list. The condition list (+0x44) tests the activate and deactivate conditions of the objects that still have an untested one.
// The counters unk_58 and unk_5c are reset. Every object of the update list is then handled: a Character updates its AI pointers; an object that is updating and enabled (or remotely updated in an
// online game) is dropped to the deletion list when marked for deletion, otherwise Update runs (unk_88 cleared first) and the list at +0x70 follows the unk_88 flag (unk_89 holds the last
// value); any other object loses its cull state and a Character unloads its script process. Last the debug switches TraceUpdateGameObjectOnce and Give50Potions are cleared.
void ObjectManager::Update(float dt) {
    PushProfilingContext("ObjectManager::Update");
    if (GetOnlineGameFlag()) {
        unk_fd = 1;
        unk_fc = 1;
    }
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level != nullptr && level->m_isLoaded && !level->m_isInGameView && !GetOnlineGameFlag()) {
        PopProfilingContext("ObjectManager::Update");
        return;
    }
    UpdateRooms();
    DoRemoteUpdateUpdate(dt);
    ProcessNextGameObjectToStartUpdate();
    m_updateList.splice(m_updateList.end(), m_spawnList);

    for (auto it = unk_3c.begin(); it != unk_3c.end();) {
        ObjectBase* object = *it;
        if (object->m_lockCount == 0 && object->m_deleteMode == 0 && !IsOnlineDeferred(object)) {
            if (object->IsCharacter() && object->IsPlayer()) {
                FakeRemove(ObjectHandle(object));
            } else {
                Remove(ObjectHandle(object));
            }
            it = unk_3c.erase(it);
            continue;
        }
        if (object->m_deleteMode != 0) {
            object->m_deleteMode = static_cast<std::uint8_t>(object->m_deleteMode - 1);
        }
        ++it;
    }

    for (ObjectBase* object : m_conditionList) {
        const bool enablePending = !object->m_enableCond.m_tested && object->m_enableCond.m_list != nullptr;
        const bool disablePending = !object->m_disableCond.m_tested && object->m_disableCond.m_list != nullptr;
        if (enablePending || disablePending) {
            object->TestEnableCondition(true);
            object->TestDisableCondition(true);
        }
    }

    unk_58 = 0;
    unk_5c = 0;
    for (auto it = m_updateList.begin(); it != m_updateList.end();) {
        ObjectBase* object = *it;
        if (object == nullptr) {
            ++it;
            continue;
        }
        if (object->IsCharacter()) {
            static_cast<Character*>(object)->UpdateAIPointers();
        }
        if ((object->unk_85 != 0 && object->m_enabled) || (GetOnlineGameFlag() && object->IsRemotelyUpdated())) {
            if (object->m_markedForDeletion != 0) {
                MarkForDeletion(object);
                it = m_updateList.erase(it);
                continue;
            }
            object->unk_88 = 0;
            object->Update();
            ObjectHandle handle = object->GetHandle();
            if (handle.GetObject(false) != nullptr) {
                if (object->unk_89 != object->unk_88) {
                    object->unk_89 = object->unk_88;
                    Character* character = handle;
                    if (object->unk_88 != 0) {
                        unk_70.push_back(character);
                    } else {
                        EraseAll(unk_70, character);
                    }
                }
            }
        } else {
            object->m_cullState = 0;
            if (object->IsCharacter()) {
                static_cast<Character*>(object)->UnLoadScriptProcess(Character::s_concurrentAI.end(), false);
            }
        }
        ++it;
    }

    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->SetSwitch("TraceUpdateGameObjectOnce", false);
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->SetSwitch("Give50Potions", false);
    PopProfilingContext("ObjectManager::Update");
}

// 0x34aca0 ObjectManager::GetObjectByName(char const*,int,bool,char const*)
// A name starting with "Player", "PlayerCharacter_" or "LocalPlayer" ignores the flags (-1). The name "HighestThreatPlayer" is answered by GetHighestThreatPlayer(type, flags, active), the type
// argument naming the player. Otherwise the object map is walked in key order: an entry with an object whose flags value (ObjectBase +0x64) equals the flags (or the flags are -1, or the
// object is global, +0x87) matches when its entry name equals the name, or when the name is "Player" or "LocalPlayer" and the object is the local player's character (the first local PlayerInfo,
// PlayerInfo +0x660). A match returns a handle holding the entry's key. With no match, an active lookup takes the next key (m_nextKey, +0x4c), creates the entry with that name and returns its
// handle; an inactive one reads the isTracingObjectManager switch and returns an empty handle.
ObjectHandle ObjectManager::GetObjectByName(char const* name, int flags, bool active, char const* type) {
    if (std::strncmp(name, "Player", 6) == 0 || std::strncmp(name, "PlayerCharacter_", 16) == 0 ||
        std::strncmp(name, "LocalPlayer", 11) == 0) {
        flags = -1;
    }
    if (std::strcmp(name, "HighestThreatPlayer") == 0) {
        return GetHighestThreatPlayer(type, flags, active);
    }
    ObjectHandle handle;
    for (const auto& entry : m_objectMap) {
        ObjectBase* object = entry.second.m_object;
        if (object == nullptr || !(flags == -1 || flags == object->unk_64 || object->m_isGlobal)) {
            continue;
        }
        bool match = std::strcmp(entry.second.m_name.c_str(), name) == 0;
        if (!match && (std::strcmp(name, "Player") == 0 || std::strcmp(name, "LocalPlayer") == 0)) {
            ObjectHandle objectHandle = object->GetHandle();
            Character* character = objectHandle;
            match = dword_99F76C->GetLocalPlayer(0, true)->unk_660 == character;
        }
        if (match) {
            handle.m_id = static_cast<std::uint32_t>(entry.first);
            return handle;
        }
    }
    if (active) {
        const int key = m_nextKey++;
        m_objectMap[key].m_name = name;
        handle.m_id = static_cast<std::uint32_t>(key);
        return handle;
    }
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch("isTracingObjectManager");
    return handle;
}

// 0x34b064 ObjectManager::GetHighestThreatPlayer(char const*,int,bool)
// Looks the named object up (GetObjectByName(name, flags, false)), takes the Character's CharAI highest aggro target and returns that object's handle. Without a target the
// isTracingObjectManager switch is read and an empty handle comes back. The active argument is not read (the original's third explicit argument is ignored). A name that does not resolve
// leaves the original dereferencing a null Character (it crashes); here it takes the empty-handle path.
ObjectHandle ObjectManager::GetHighestThreatPlayer(char const* name, int flags, bool active) {
    (void)active;
    ObjectHandle player = GetObjectByName(name, flags, false, nullptr);
    Character* character = player;
    if (character != nullptr) {
        Character* aggro = character->AI_GetHighestAggro();
        if (aggro != nullptr) {
            return GetObjectByPtr(aggro);
        }
    }
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch("isTracingObjectManager::");
    return ObjectHandle(nullptr);
}

// 0x34b270 ObjectManager::Add(ObjectBase*,char const*,char const*,int,bool)
// Asserts that the object (line 1064) and the name (line 1065) are present. The name is looked up with the create flag (GetObjectByName(name, flags, true)); when it already resolves to an object the
// new object is deleted and the existing handle is returned. Otherwise the object becomes the map entry's object, the object count (unk_50) rises, the handle (with the object cached) is copied
// into the object's own handle (ObjectBase +0x2c), the name, game type (type) and flags (ObjectBase +0x64) are set, a Character is appended to the character list (+0x60), a Module (GO_IDS 5 with the game
// type "Module") to the module list (+0x68), and with the active flag the object gets its network id (AssignObjectNetworkId).
ObjectHandle ObjectManager::Add(ObjectBase* object, char const* name, char const* type, int flags, bool active) {
    if (object == nullptr) {
        ReportAssert("obj", 1064);
    }
    if (name == nullptr) {
        ReportAssert("name", 1065);
    }
    ObjectHandle handle = GetObjectByName(name, flags, true, nullptr);
    if (handle.GetObject(false) != nullptr) {
        if (object != nullptr) {
            delete object;
        }
        return handle;
    }
    m_objectMap[static_cast<int>(handle.m_id)].m_object = object;
    ++unk_50;
    handle.m_cached = object;
    *object->m_handle = handle;
    object->SetName(name);
    object->m_gameType.assign(type);
    object->unk_64 = flags;
    {
        ObjectHandle objectHandle = object->GetHandle();
        Character* character = objectHandle;
        if (character != nullptr) {
            m_characters.push_back(character);
        }
    }
    {
        ObjectHandle objectHandle = object->GetHandle();
        ObjectBase* resolved = objectHandle.GetObject(false);
        if (resolved != nullptr && resolved->m_goId == 5 && object->m_gameType == "Module") {
            unk_68.push_back(static_cast<Module*>(resolved));
        }
    }
    if (active) {
        AssignObjectNetworkId(object);
    }
    return handle;
}

// 0x34b520 ObjectManager::GetNewObject(char const*,char const*,int,bool)
// Scans objectCreationMap (33 type names with their factories) for the first entry equal to the type. Its factory builds the object, which gets the table's type name as its class name
// (PropertyMap +0x1c) and is registered with Add(object, name, type, flags, active). A factory that returns null fails ASSERT(obj) at line 1310 and the scan goes on. Without a match the
// isTracingObjectManager switch is read and an empty handle is returned.
ObjectHandle ObjectManager::GetNewObject(char const* type, char const* name, int flags, bool active) {
    for (const ObjectCreationEntry& entry : objectCreationMap) {
        if (std::strcmp(type, entry.typeName) != 0) {
            continue;
        }
        ObjectBase* object = entry.factory();
        if (object != nullptr) {
            static_cast<PropertyMap*>(object)->m_className = entry.typeName;
            return Add(object, name, type, flags, active);
        }
        ReportAssert("obj", 1310);
    }
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch("isTracingObjectManager");
    return ObjectHandle();
}
