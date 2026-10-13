// Batch g1-game-world-GameObject-5d2019-4 (GameObject, part 5 of 8).
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. Layout and declarations: dh2/GameObject.hpp.
// Offsets in comments are the original 32-bit ones.
//
// The Lua statics take (const Arguments&, ReturnValues&, void* self). The listings name the first pointer `this`,
// the second `a2` and the third `a3`, and type them shifted by one. The self object is the third argument: a3 + 0x1c8
// is m_pfObject and a3 + 0x304 is m_targetList. The bodies use the header's names.
//
// BLOCKED ON THE LUA LAYER. The sfc::script::lua wrapper has no declarations in the tree: there is no dh2 header for
// Arguments (operator[], size), Value (the getters) or ReturnValues (the pushes). Every binding below that reads an
// argument or pushes a result is therefore partial. Its body does the part that needs only GameObject state, and its
// comment gives the rest of the control flow from the listing. The Value tag word (read at Value +4) is 1 for bool,
// 2 for pointer, 3 for number, 4 for string and 7 for userdata; each listing pairs these tags with the matching getter.
//
// Fully reconstructed here: _PopTargetList (no arguments). LoadExternalScript is partial only for its Binder call.

#include <cstring>
#include <string>

#include "dh2/GameObject.hpp"
#include "dh2/LuaScript.hpp"

// 0x38eab4 GameObject::_IsInWater(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Reads the PFObject water flag (floor flag bit 1 of m_pfObject at +0x1c8). The push is missing: the listing
// calls ReturnValues::pushBoolean(ret, flag), and ReturnValues has no declaration.
void GameObject::_IsInWater(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    const bool inWater = static_cast<GameObject*>(self)->m_pfObject.IsInWater();
    (void)inWater;  // pushBoolean(ret, inWater) not declared
}

// 0x38ead4 GameObject::_IsOverAHole(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Reads the PFObject hole flag (floor flag bit 0 of m_pfObject at +0x1c8). The push is missing (see _IsInWater).
void GameObject::_IsOverAHole(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    const bool overHole = static_cast<GameObject*>(self)->m_pfObject.IsOverAHole();
    (void)overHole;  // pushBoolean(ret, overHole) not declared
}

// 0x38eaf4 GameObject::_GetSelf(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The listing pushes the self object as userdata: ReturnValues::pushUserData(ret, self). Not declared.
void GameObject::_GetSelf(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    (void)self;  // pushUserData(ret, self) not declared
}

// 0x38eb68 GameObject::_GetTargetListTop(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The empty test and the top entry read are reconstructed (the queue's front is *begin, as in the listing).
// The pushes are missing: an empty queue pushes a null pointer (pushPointer). Otherwise the listing pushes, in order,
// the TargetInfo object (pushUserData), distance (pushNumber), angle in radians times 57.296 (degrees, pushNumber),
// bit 0 of isCharacter (pushBoolean) and lateral (pushNumber). ReturnValues has no declaration.
void GameObject::_GetTargetListTop(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    const auto& results = static_cast<GameObject*>(self)->m_targetList.m_results;
    if (results.empty()) {
        return;  // pushPointer(ret, nullptr) not declared
    }
    const ObjectSearcher::TargetInfo& top = results.top();
    (void)top;  // the five pushes listed above are not declared
}

// 0x38ebe4 GameObject::_GetID(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The listing pushes the self pointer: ReturnValues::pushPointer(ret, self). Not declared.
void GameObject::_GetID(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    (void)self;  // pushPointer(ret, self) not declared
}

// 0x38ebf0 GameObject::_GetName(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The listing pushes the object name: ReturnValues::pushString(ret, +0x44). The word at +0x44 is the data
// pointer of m_name (the std::string at +0x30, as PFObject::m_debugName is set from it in GameObject::InitFinal).
void GameObject::_GetName(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    const char* name = static_cast<GameObject*>(self)->m_name.c_str();
    (void)name;  // pushString(ret, name) not declared
}

// 0x38ef60 GameObject::LoadExternalScript(char const*, char const*)
// PARTIAL. Reconstructed: the release of the current script (vtable +0x04 is the deleting destructor, so delete),
// the new LuaScript (operator new(152) then LuaScript(0): bindings are built), the name stored at LuaScript +0x68 (the
// listing assigns the second argument into the string at +0x68, which LuaScript.hpp names m_scriptDirectory; the name
// of that field needs checking), Load(file), and the release again when Load returns 0.
// Missing: the call createBindings(script's binder) (vtable +0x0c with the binder at LuaScript +0x10). LuaScript.hpp keeps
// the binder as 12 opaque bytes (the 32-bit size) and sfc::script::lua::Binder has no declaration, so the call is not made.
void GameObject::LoadExternalScript(const char* file, const char* name) {
    if (m_script != nullptr) {
        delete m_script;
        m_script = nullptr;
    }
    if (file != nullptr && *file != '\0') {
        m_script = new LuaScript(false);
        // createBindings(binder of m_script) belongs here (see the comment above).
        if (name != nullptr) {
            m_script->m_scriptDirectory.assign(name);
        }
        if (m_script->Load(file) == 0) {
            delete m_script;
            m_script = nullptr;
        }
    }
}

// 0x38fbb8 GameObject::_PopTargetList(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Reconstructed. Removes the top entry of the target queue (m_targetList at +0x304) when it is not empty. The listing
// tests the deque's size as the distance between its begin and end iterators, which is the same as empty(). The
// arguments and the return values are not used.
void GameObject::_PopTargetList(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    auto& results = static_cast<GameObject*>(self)->m_targetList.m_results;
    if (!results.empty()) {
        results.pop();
    }
}

// 0x38fbf8 GameObject::_TargetListResort(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With at least one argument and args[0] a number (tag 3), the listing calls TargetList::Resort(int(args[0]))
// on the target list at +0x304. The argument checks are missing.
void GameObject::_TargetListResort(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (number) -> m_targetList.Resort(sortType) not available
}

// 0x38fc74 GameObject::_DropFX(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With args[0] a pointer (tag 2), the listing copies the pointer (getPointer) into a local AnimatedFX* and calls
// VisualFXManager::DropAnimatedFX(&Singleton<VisualFXManager>::s_inst, &local). The singleton is not declared (no
// instance accessor in VisualFXManager.hpp), and the argument checks are missing.
void GameObject::_DropFX(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (pointer) -> VisualFXManager instance DropAnimatedFX(&local) not available
}

// 0x38fcf0 GameObject::_TargetListSearch(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written: every branch reads arguments. The control flow from the listing, for the next worker:
//  - Needs at least one argument and args[0] a number (radius). With two or more, args[1] must be a number (cone angle
//    in degrees). The search then uses half the angle in radians (angle * 0.017453 * 0.5). With one argument the half
//    angle is 3.1416.
//  - Origin: self m_position (+0x160, +0x164, +0x168) by default.
//    * Three or more arguments with args[2] userdata: the origin is that object's m_position. Options start at index 3.
//    * Five or more arguments with args[2..4] numbers: if args[5] is a true bool, the origin is
//      m_position + args[2] * (K x L) + args[3] * L + args[4] * K, where L is GetLookAtVec() of self and K is Vec3f_K,
//      which is (0, 0, 1) (static initializer at 0x312e00). Otherwise the origin is (args[2], args[3], args[4]).
//      Options start at index 6.
//    * Anything else: options start at index count - 1 and the origin stays at m_position.
//  - The source list is one of four. Three are built on the stack for each call: CharacterList over ObjectManager
//    m_characters (ObjectManager +0x60), PlayerList over the PlayerManager (dword_99F76C, index 0), and RoomObjectList over
//    the ObjectManager +0x80 list. The fourth is the BackupObjectList from TargetList::GetResultsBackup(name).
//  - Option at the start index: a true bool selects the backup named "default"; a string selects the backup of that name.
//    With no option, the default source is PlayerList when TargetList::m_filterMask has bit 0x80, else CharacterList when
//    TargetList::m_arg3 is 2, else RoomObjectList.
//  - Then TargetList::Search(origin, radius, halfAngle, source) on the target list at +0x304.
// The ObjectManager +0x80 member is declared as a RoomZone list in ObjectManager.hpp, while RoomObjectList expects a list
// of object lists: that conflict needs settling before the search body can be written.
void GameObject::_TargetListSearch(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // Argument-driven search: see the control flow above.
}

// 0x39040c GameObject::_EnableCollisions(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written: every branch reads arguments. From the listing:
//  - args[0] a bool (tag 1) sets the flag, otherwise nothing happens.
//  - args[1] a true bool (only when there are two arguments): if the physical object (m_physicalObject at +0x2dc) is set,
//    flag ? resetFilter() : setFilter(m_group, m_category, m_mask & 0xFFE3, false). Nothing else happens.
//  - Otherwise: flag ? EnableCollisions() : DisableCollisions() on self.
void GameObject::_EnableCollisions(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // Argument-driven: see the control flow above.
}

// 0x390548 GameObject::_SetTargetListSorting(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With args[0] a number (tag 3) as the mode i, the listing pops every entry of the target queue (while its
// begin and end iterators differ) and installs the comparator for the mode: 1 _sortClosest, 2 _sortFrontal, other
// _sortNoSort. TargetList::Resort(int) pops, installs the sorter and rebuilds the heap, which is the same thing on an
// empty queue, so the body is probably m_targetList.Resort(i). Confirm the Resort body before using it.
void GameObject::_SetTargetListSorting(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // Argument-driven: see the note above.
}

// 0x390624 GameObject::_SetTargetListObjectFilter(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With args[0] a number (tag 3), the listing stores (int) args[0] at TargetList +0x38 (self +0x33c). That is
// TargetList::m_arg3, the third constructor int, which _TargetListSearch compares with 2.
void GameObject::_SetTargetListObjectFilter(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (number) -> m_targetList.m_arg3 not available
}

// 0x390690 GameObject::_SetTargetListCharacterFilter(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With args[0] a number (tag 3), the listing stores (int) args[0] at TargetList +0x34 (self +0x338). That is
// TargetList::m_filterMask, which _TargetListSearch tests with bit 0x80.
void GameObject::_SetTargetListCharacterFilter(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (number) -> m_targetList.m_filterMask not available
}

// 0x3906fc GameObject::_DealDamages(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. From the listing:
//  - Needs more than two arguments, args[0] userdata (tag 7) and args[1] a number (tag 3), and args[1] below
//    Arrays::GameObjectDamager::size (no declaration of that table).
//  - Reads the userdata's object handle and converts it to Character*. The listing's first argument to
//    ObjectBase::GetHandle is the handle buffer, so the receiver is not clear from the listing. Check in IDA.
//  - Character: F_GOAttack(result, self, character, uint(args[1]), 0), F_ApplyResult(result, self, character, 0), then
//    pushInteger(ret, first word of the result).
//  - Otherwise, if the userdata's virtual slot at +0x90 (index 36) returns 8, call slot +0x98 with self and push false.
//    Those slots are not declared in sfc::script::lua::UserData (slots 0 to 3 only).
void GameObject::_DealDamages(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // Argument-driven: see the control flow above.
}

// 0x39088c GameObject::_MarkAsSwimming(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With args[0] a bool (tag 1), the listing calls PFObject::SetSwimming(flag) on m_pfObject (+0x1c8). The
// bool read (getBool) is missing.
void GameObject::_MarkAsSwimming(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (bool) -> m_pfObject.SetSwimming(flag) not available
}

// 0x390900 GameObject::_MarkAsFlying(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. As _MarkAsSwimming, with PFObject::SetFlying(flag) on m_pfObject (+0x1c8).
void GameObject::_MarkAsFlying(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (bool) -> m_pfObject.SetFlying(flag) not available
}

// 0x390974 GameObject::_SetFXEndPoint(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. Not written. From the listing, with args[0] a pointer (tag 2) and either two or four arguments:
//  - Two arguments: args[1] must be userdata (tag 7). The end point is that object's target position (GetTargetPosition()).
//  - Four arguments: args[1..3] must all be numbers (tag 3). The end point is (args[1], args[2], args[3]).
//  - Then AnimatedFX::SetEndPoint(pointer, &end). Other argument counts and tags do nothing.
void GameObject::_SetFXEndPoint(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // Argument-driven: see the control flow above.
}

// 0x390b3c GameObject::_SetMaxPath(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. With args[0] a number (tag 3): when self is a character (the virtual IsCharacter(), vtable +0x24) and
// Character::IsFollower() holds, m_maxPath (+0x26c) becomes (uint) args[0], and 100 when that is above 100. The
// argument read (getUInteger) is missing.
void GameObject::_SetMaxPath(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* /*self*/) {
    // args[0] (number) -> m_maxPath with the IsCharacter / IsFollower gate not available
}
