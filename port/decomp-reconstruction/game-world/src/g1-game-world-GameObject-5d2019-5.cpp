// Batch g1-game-world-GameObject-5d2019-5 (GameObject, part 6 of 8).
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. Layout and declarations: dh2/GameObject.hpp.
// Offsets in comments are the original 32-bit ones.
//
// The Lua statics take (const Arguments&, ReturnValues&, void* self). The listings name the first pointer `this`, the
// second `a2` and the third `a3`, and type them shifted by one: `this` is the argument list, `a2` the return values and
// `a3` the GameObject. The bodies use the header's names. Value tags as in batch -4: 1 bool, 2 pointer, 3 number,
// 4 string, 7 userdata. A Value is 0x70 bytes, so args.size() is (end - begin) / 0x70 in the listings.
//
// BLOCKED ON THE LUA LAYER. sfc::script::lua::Arguments, Value and ReturnValues still have no declarations in the tree
// (batch -4 says the same). Every binding below reads an argument or pushes a result, so all fifteen are partial. Their
// bodies are empty. Each comment gives the control flow from the listing for the worker who has the Lua layer.
//
// Reconstructed without Lua access: GetTargetPosition, SetDestination, IsAtDestination, UpdateRotation, SetRotation.

#include <cmath>

#include "dh2/Application.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ctrl_camera.hpp"  // Application& ApplicationGetInstance()

// 0x390bd4 GameObject::_TargetListBackup(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: with no arguments, or with args[0] not a string (tag 4), the target list at +0x304
// backs up its results under 'default'. With a string, it backs them up under that name (getString). The
// TargetList::BackupResults(const char*) call is declared; the argument reads are not.
void GameObject::_TargetListBackup(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                   void* /*self*/) {}

// 0x390c50 GameObject::_SummonTimerTrap(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked, and TimerTrap has no class header (TimerTrap::Create is not declared). Listing: needs more than
// one argument, args[0] a number below Arrays::TimerTraps::size and args[1] a number below
// Arrays::GameObjectDamager::size. The trap is TimerTrap::Create(self, int(args[0]), int(args[1]), r3), where the
// listing never sets r3 (check the asm). With more than two arguments, args[2] as userdata puts the trap at that
// object's m_position (+0x160). Otherwise, with more than four arguments and args[2..4] numbers, the trap goes to
// (args[2], args[3], args[4]). Either way SetPosition(trap, pos, true). Pushes the trap as userdata.
void GameObject::_SummonTimerTrap(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                  void* /*self*/) {}

// 0x390f1c GameObject::_IsInRange(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: needs more than one argument. args[0] is a string (object found by
// ObjectManager::GetObjectByName) or userdata (tag 7), else nothing is pushed. args[1] is a number radius r, and the
// gate also reads args[2]'s tag (check Arguments::operator[] for a short list). Both physical objects present (+0x2dc):
// in range when r >= distance(centres) - radius(self) - radius(target), using PhysicalObject::getRadius. Otherwise the
// absolute boxes (m_absAabb at +0x12c, grown by r on each side) must overlap. The box test reads the target without a
// null check. When in range: with exactly three arguments it pushes (args[2] <= dot / |d|), where d = target - self and
// dot uses GetLookAtVec(); with any other count it pushes true. Out of range pushes false.
// (The radius test also needs the target's PhysicalObject flag byte at +0x10 to be set.)
void GameObject::_IsInRange(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                            void* /*self*/) {}

// 0x391438 GameObject::_GetDistanceBetween(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: needs more than one argument, and args[0] and args[1] both strings. Each is looked up
// with ObjectManager::GetObjectByName (dword_99F764). Pushes -1.0 when either object is missing, else the squared 3D
// distance between their m_position (+0x160). ObjectManager::GetObjectByName is declared; the instance is not defined.
void GameObject::_GetDistanceBetween(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                     void* /*self*/) {}

// 0x391670 GameObject::_SummonTriggerTrap(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked, and TriggerTrap has no class header (TriggerTrap::Create is not declared). Same control flow as
// _SummonTimerTrap with Arrays::TriggerTraps::size for args[0], TriggerTrap::Create in place of TimerTrap::Create and the
// same position rules for the trap.
void GameObject::_SummonTriggerTrap(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                    void* /*self*/) {}

// 0x39193c GameObject::_Summon(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Callees without a matching declaration: Character::CreateNPC (no overload takes the listing's
// (int, 0, nullptr, flag): the header has (const char*, const char*, bool, bool) and (int, const char*, bool)), RoomZone
// and its AddInitialObject (no class header), OnlineSingleton<OnlineGameState>, and CMessaging::SendMsg (the header
// declares SendMsgToServer only).
// Listing: args[0] a number below Arrays::CharacterTable::size (the NPC type). args[1] a bool (the spawn-state flag).
// The point is the object's m_position, or the absolute position of the visual's summon_spot node when it has one.
// The rotation is the object's m_rotation. With userdata at args[2], the point and rotation come from that object. With
// more than four arguments and args[2..4] numbers, the point is those numbers, or, with a true bool at args[5], an offset
// from self: args[2] * (Vec3f_K x GetLookAtVec()) + args[3] * look + args[4] * Vec3f_K. A forward-animation time comes
// from args[2] when exactly three arguments are given. The point is floor-corrected by PFWorld::GetFloorHeightAt, and
// self's position is used when there is no floor. Then CreateNPC(type), SetInitialPosition, SetPosition, SetRotation,
// Character::m_isNetSpawned (+0x14e4) set to 1, and either the room zone's AddInitialObject or ObjectManager::AddNoRoomObject with
// ZoneEntered. SM_SetSpawnState(0, 0) runs when the flag is set. pushUserData(npc). A positive time is forwarded to the
// NPC visual's node with RootSceneNode::ForwardAnim. Online, the NPC gets a network id, its owner id and time copied from
// self, and a CMsgSpawnObject message is sent when the online state is 3 or 4.
void GameObject::_Summon(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* /*self*/) {}

// 0x39212c GameObject::_PlaySound3D(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: args[0] is read as a string (no tag test). Its index in Arrays::Sounds (strcmp over the
// member names) is the sound id, or -1 when it is not found or the table is empty. The position is self's m_position,
// unless there are at least four arguments and args[1..3] are numbers, which give the position. Then
// VoxSoundManager::Play3D(s_instance, id, position, 0, COERCE_FLOAT(1), -1.0, -1.0). COERCE_FLOAT(1) is the float with
// bit pattern 1, not 1.0. VoxSoundManager::Play3D is not declared in the tree (SoundEmitter.hpp mentions it only in a
// comment). The listing's return value is ignored by the Lua caller.
void GameObject::_PlaySound3D(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                              void* /*self*/) {}

// 0x3923cc GameObject::_PlayFX(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: args[0] a number below Arrays::AnimatedEffectTable::size is the effect id. With no more
// than three arguments: VisualFXManager::PlayAnimFXSet(id, self, 0). Otherwise the position is self's m_position unless
// args[1..3] are numbers, and the call is PlayAnimFXSet(id as float, &position, 0, 0). The header's PlayAnimFXSet
// overloads take (id, position, origin|object, ...), so the listing's argument order needs checking. The function returns
// the id, which the Lua caller ignores. The Singleton<VisualFXManager> instance is not declared.
void GameObject::_PlayFX(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* /*self*/) {}

// 0x392620 GameObject::_GrabFX(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked, and the Singleton<VisualFXManager> instance is not declared. Listing: args[0] a number below
// Arrays::AnimatedEffectTable::size is the effect id (float to unsigned). Pushes the pointer returned by
// VisualFXManager::GrabAnimFX(id, self), which the header declares as GrabAnimFX(int, GameObject*).
void GameObject::_GrabFX(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* /*self*/) {}

// 0x392714 GameObject::_RegisterSummon(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: args[0] a number below Arrays::CharacterTable::size, read as an object id (int).
// Flags are args[1] when there are two or more arguments and args[1] is a number, else 1. Then
// Character::AddCharOIDToCache(oid, flags, third). The listing passes a third word computed from the argument count,
// while the header declares AddCharOIDToCache(int, unsigned int) only. The third word is not resolved.
void GameObject::_RegisterSummon(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                 void* /*self*/) {}

// 0x392848 GameObject::_GetObjectByName(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: with exactly one argument that is a string, ObjectManager::GetObjectByName(name, -1,
// false, nullptr) on the ObjectManager instance (dword_99F764, declared in global/ObjectManagerInstance.hpp), then the
// handle converted to GameObject* is pushed as userdata (null when not found). Other argument shapes push nothing.
void GameObject::_GetObjectByName(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                  void* /*self*/) {}

// 0x3928f0 GameObject::_SetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: with one argument, a string (named object), a pointer (tag 2) or userdata (tag 7)
// selects an object. Other tags do nothing. With that object: SetPosition(self, object.m_position, true), the PFObject
// position (self m_pfObject.m_pos, +0x1e0) is copied from the object, then ForceUpdatePosition(). With exactly three
// arguments, all numbers: SetPosition(self, (x, y, z), true), m_pfObject.m_pos = (x, y, z), then ForceUpdatePosition().
// Any other argument count does nothing. Names resolve through ObjectManager::GetObjectByName.
void GameObject::_SetPosition(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                              void* /*self*/) {}

// 0x392b80 GameObject::_TargetListSearchRect(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: needs more than one argument, args[0] a number (length) and args[1] a number (width).
// The origin is self's m_position by default. With userdata at args[2] it is that object's m_position (options then start
// at index 3). With args[2..4] numbers it is those values. With more than five arguments and a true bool at args[5] it is
// self's m_position offset along GetLookAtVec() and Vec3f_K (options then start at index 6). The options (from the
// option index) choose the source. A true bool selects the backup named 'default' (GetResultsBackup). A string selects the
// backup of that name. Otherwise the default source is the player list when TargetList m_filterMask (+0x338) has 0x80,
// else the character list (ObjectManager +0x60) when m_arg3 (+0x33c) is 2, else the room object list (ObjectManager
// +0x80). Then TargetList::SearchRect(origin, length, width, source). Nothing happens unless args[1] is a number. Vec3f_K
// is not declared. The CharacterList and PlayerList classes are declared in ObjectSearcher.hpp (the PlayerManager is
// dword_99F76C, declared in global/IsMultiplayerGame.hpp). The room object list at ObjectManager +0x80 conflicts with the
// header, which declares m_roomZones there (the batch -4 conflict).
void GameObject::_TargetListSearchRect(const sfc::script::lua::Arguments& /*args*/,
                                       sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {}

// 0x393310 GameObject::_Rand(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: no arguments, or three or more, give the range 100 from 0. One argument, a number n,
// gives [0, n). Two numbers, a and b, give [a, b - a). Any other shape pushes nothing (one argument not a number, or two
// with a non-number). Outside online play it pushes Random::GetRandom(range) + base as an integer. Online, self's
// ObjectBase unk_fc (+0xfc) word is loaded into Random::s_seed, GetRandom runs on the synchronized stream and the seed is
// stored back to +0xfc. The listing's GetRandom call passes three words (the header declares GetRandom(range, synced)),
// and GetOnline() reads COnline +5.
void GameObject::_Rand(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                       void* /*self*/) {}

// 0x393444 GameObject::_GetDistanceFrom(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Lua-blocked. Listing: needs at least one argument. args[0] a string (object by name) or userdata (tag 7),
// other tags push nothing. Pushes the squared 3D distance from that object's m_position to self's m_position, or -1.0
// when the object is missing.
void GameObject::_GetDistanceFrom(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                                  void* /*self*/) {}

// 0x3935dc GameObject::GetTargetPosition() const
// The target node's position (m_targetNodePos, +0x184) when there is a node (m_targetNode, +0x180) and the object is
// visible (m_visible, the byte at +0x80 of ObjectBase). Otherwise m_position (+0x160).
const Point3D<float>& GameObject::GetTargetPosition() const {
    if (m_targetNode != nullptr && m_visible) {
        return m_targetNodePos;
    }
    return m_position;
}

// 0x393600 GameObject::SetDestination(Point3D<float> const&)
// Copies the destination into m_destination (+0x1a8, three floats).
void GameObject::SetDestination(const Point3D<float>& dest) {
    m_destination = dest;
}

// 0x39361c GameObject::IsAtDestination() const
// Within 80 units in the XY plane: the squared distance is below 6400. The target is the end of the PFObject path
// (m_pfObject.m_pathEnd, +0x208) when the waypoint list (PFObject +0x38, the list's sentinel at GameObject +0x200) is not
// empty. Otherwise it is m_destination (+0x1a8). The listing walks the waypoint list to its sentinel with no side effect,
// which is not reproduced.
bool GameObject::IsAtDestination() const {
    const Point3D<float>& target = m_pfObject.m_path.empty() ? m_destination : m_pfObject.m_pathEnd;
    const float dx = target.X - m_position.X;
    const float dy = target.Y - m_position.Y;
    return (dx * dx + dy * dy) < 6400.0;
}

namespace {

// Single-precision constants of UpdateRotation, by the bit pattern the listing loads (MOVW/MOVT). kFloatPi is the float
// nearest pi, 0x40490FDB; the float nearest the literal 3.1416 is 0x40490FF9 and is a different limit, so it is not used.
constexpr float kFloatPi = 3.14159274f;           // 0x40490FDB, the wrap limit (fcmpgt above, fcmplt below with -kFloatPi)
constexpr float kFloatTwoPi = 6.28318548f;        // 0x40C90FDB, the wrap amount (2 * kFloatPi)
constexpr float kRotationStepScale = 12.566371f;  // 0x41490FDB, the step per unit speed (4 * kFloatPi)
constexpr float kTimeScale = 0.001f;              // 0x3A83126F, frame time from milliseconds

}  // namespace

// 0x393710 GameObject::UpdateRotation()
// Turns m_rotation.Z (+0x174) toward m_targetYaw (+0x178) by GetRotationSpeed() (vtable +0xac, -1 by default). The
// listing is single precision throughout (__aeabi_fmul, fsub, fadd, fcmpgt, fcmplt), so every operation below is float.
// Step = (speed * kRotationStepScale) * (float(dt) * kTimeScale), dt in ms from Application::GetDt. The difference
// m_targetYaw - m_rotation.Z is wrapped by kFloatTwoPi when it is above kFloatPi, or else when it is below -kFloatPi.
// When |diff| is below the step, or the speed is below zero, the rotation snaps to m_targetYaw. Otherwise it moves one
// step toward the target, and m_turnPositive (+0x17c) records the direction: 1 when diff is not below zero, else 0.
// Last, when a visual is attached and IsUpdatingVisualWithRotation() (vtable +0x70) is set, the visual syncs its rotation.
// The comparisons are written as the listing's float helpers test them, so a NaN takes the same branch as the original.
// The listing is typed float. The value it leaves in the return register is a bool bit pattern or SyncRotation's result,
// and no caller reads it, so the method is void as declared in the header.
void GameObject::UpdateRotation() {
    const float speed = GetRotationSpeed();
    if (!(speed < 0.0f)) {  // fcmplt(speed, 0) is false: the object turns
        const int dt = ApplicationGetInstance().GetDt();
        const float cur = m_rotation.Z;
        const float step =
            (speed * kRotationStepScale) * (static_cast<float>(static_cast<unsigned int>(dt)) * kTimeScale);
        float diff = m_targetYaw - cur;
        if (diff > kFloatPi) {
            diff = diff - kFloatTwoPi;
        } else if (diff < -kFloatPi) {
            diff = diff + kFloatTwoPi;
        }
        if (std::fabs(diff) < step) {  // fcmplt(|diff|, step) is true: snap
            m_rotation.Z = m_targetYaw;
        } else if (!(diff < 0.0f)) {
            m_rotation.Z = cur + step;
            m_turnPositive = 1;
        } else {
            m_rotation.Z = cur - step;
            m_turnPositive = 0;
        }
    } else {
        m_rotation.Z = m_targetYaw;
    }
    if (m_visualObject != nullptr && IsUpdatingVisualWithRotation()) {
        m_visualObject->SyncRotation();
    }
}

// 0x3938a0 GameObject::SetRotation(Point3D<float> const&)
// Stores the rotation in m_rotation (+0x16c..+0x174) and its Z in m_targetYaw (+0x178). When a visual is attached and
// IsUpdatingVisualWithRotation() is set, the visual syncs its rotation.
void GameObject::SetRotation(const Point3D<float>& rotation) {
    m_rotation = rotation;
    m_targetYaw = rotation.Z;
    if (m_visualObject != nullptr && IsUpdatingVisualWithRotation()) {
        m_visualObject->SyncRotation();
    }
}
