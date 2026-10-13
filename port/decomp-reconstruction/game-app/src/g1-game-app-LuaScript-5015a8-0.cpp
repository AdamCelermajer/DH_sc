// Batch g1-game-app-LuaScript-5015a8-0 (LuaScript, part 1 of 3).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// The virtual-function table and the integer table are std::map (the original uses STLport rb-trees with
// the same header layout), so the tree bookkeeping is the containers' own. Member offsets are recorded in
// dh2/LuaScript.hpp.
//
// Not in the tree yet: the sfc::script::lua headers (Arguments, ReturnValues, Instance, Binder). The Lua
// instance and binder are opaque bytes in LuaScript.hpp, so every body that calls into them is partial.
#include "dh2/LuaScript.hpp"

#include <string>

#include "dh2/Application.hpp"
#include "dh2/Level.hpp"
#include "dh2/LuaManager.hpp"
#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance(): the Singleton<Application>::s_inst object (0x99f72c)
#include "glitch-core-io/core/strings.hpp"

// Number of Call() invocations that reached Lua. Data, not a function; defined here because the Call bodies
// of this batch increment it.
int LuaScript::s_numCalls = 0;

// 0x37b568 LuaScript::_IncludePyCst(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Empty body (asm: BX LR).
void LuaScript::_IncludePyCst(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37b56c LuaScript::_IncludePyStruct(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Empty body (asm: BX LR).
void LuaScript::_IncludePyStruct(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37b570 LuaScript::_IncludePyArray(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Empty body (asm: BX LR).
void LuaScript::_IncludePyArray(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37b574 LuaScript::Load(char const*)
// Hands the file to LuaManager::AddFile with this script. The original passes the LuaManager from the global
// dword_99F768, which is Singleton<Application>::s_inst + 0x3c, that is Application::m_luaManager (Application.hpp
// +0x3c). It is read here through ApplicationGetInstance().
int LuaScript::Load(const char* file) {
    return ApplicationGetInstance().m_luaManager->AddFile(this, file);
}

// 0x37b5a0 LuaScript::BindFunction()
// PARTIAL. The original runs, in this order: Instance::includeBase, includeMath, includeTable and includeString
// on m_instance (+0x04), then Binder::bindFunction on m_binder (+0x10) once for each name below, and returns the
// result of the last call (the pseudocode types it int; the header declares void). Each callback gets this
// LuaScript as its void* argument. Waits on dh2/sfc/script/lua/Instance.hpp and dh2/sfc/script/lua/Binder.hpp,
// neither of which is in the tree. Nothing is bound yet.
// Binding table, in call order (name -> callback):
//   "Include" -> _Include                 "Trace" -> _Trace                     "SetInt" -> _SetInt
//   "GetInt" -> _GetInt                   "AddToVFTable" -> _AddToVFTable       "PushVFTable" -> _PushVFTable
//   "PopVFTable" -> _PopVFTable           "ToFixed" -> _ToFixed                 "FromFixed" -> _FromFixed
//   "MulFixed" -> _MulFixed               "DivFixed" -> _DivFixed               "Rand" -> _Rand
//   "RandF" -> _RandF                     "BitNot" -> _BitNot                   "BitAnd" -> _BitAnd
//   "BitOr" -> _BitOr                     "BitXOr" -> _BitXOr                   "GetPyCst" -> _GetPyCst
//   "GetPyStruct" -> _GetPyStruct         "GetPyOID" -> _GetPyOID               "CallPyScript" -> _CallPyScript
//   "GetNumPlayers" -> _GetNumPlayers     "GetHostPlayer" -> _GetHostPlayer     "GetHostPlayerLevel" -> _GetHostPlayerLevel
//   "GetHostPlayerDifficulty" -> _GetHostPlayerDifficulty
//   "GetCurrentLevelRange" -> _GetCurrentLevelRange                             "GetGameObjectsByType" -> _GetGameObjectsByType
//   "SetGameType" -> _SetGameType         "GetGameScript" -> _GetGameScript
//   "OnTargetDied" -> _IsPlayerCharacter  (the name is OnTargetDied in the original and the callback is _IsPlayerCharacter; kept as is)
//   "PlayMusic" -> _PlayMusic             "PlaySound" -> _PlaySound             "StopSound" -> _StopSound
void LuaScript::BindFunction() {
    // PARTIAL: nothing is bound. See the table above.
}

// 0x37be00 LuaScript::_PushVFTable(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// Starts a collection: the saved-entry map (+0x4c) is emptied when it holds anything, and m_vfTableCollecting
// (+0x64) is set. The arguments and the return values are not used. self is the LuaScript.
void LuaScript::_PushVFTable(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void* self) {
    LuaScript* script = static_cast<LuaScript*>(self);
    script->m_vfTableSaved.clear();     // +0x4c
    script->m_vfTableCollecting = true;  // +0x64
}

// 0x37c004 LuaScript::~LuaScript()  (D1, complete-object destructor)
// 0x37bec0 LuaScript::~LuaScript()  (D2, base-object destructor)
// PARTIAL. The two addresses have the same body and one C++ destructor covers both. The original empties unk_80
// (+0x80), the string m_scriptDirectory (+0x68), m_vfTableSaved (+0x4c), m_vfTable (+0x34) and m_ints (+0x1c),
// resets the Binder vptr at +0x10 to off_95B140, and calls sfc::script::lua::Instance::~Instance on m_instance
// (+0x04). The containers and the string are destroyed by the compiler here. The ~Instance call is missing
// (no Instance header in the tree; m_instance is opaque bytes).
LuaScript::~LuaScript() {}

// 0x37c148 LuaScript::~LuaScript()  (D0, deleting destructor)
// Skipped: compiler-generated deleting destructor. The original runs D1 (0x37c004) and then CustomFree(this). C++
// delete supplies it from the virtual destructor, so no body is written.

// 0x37c2a0 LuaScript::IsInVFTable(char const*) const
// Hashes the name and tests whether m_vfTable (+0x34) holds that hash. The original searches the rb-tree for the
// first key not below the hash and checks for equality, which is what find() does.
bool LuaScript::IsInVFTable(const char* funcName) const {
    return m_vfTable.find(glitch::core::hashString(funcName)) != m_vfTable.end();
}

// 0x37c314 LuaScript::_GetFuncName(char const*) const
// Returns the mapped name stored under the hash of funcName, or funcName itself when the hash is not in
// m_vfTable. The original returns the stored string's character data (node +0x28), which is c_str() here.
const char* LuaScript::_GetFuncName(const char* funcName) const {
    const auto entry = m_vfTable.find(glitch::core::hashString(funcName));
    return entry != m_vfTable.end() ? entry->second.c_str() : funcName;
}

// 0x37c390 LuaScript::Call(char const*, sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&)
// PARTIAL. The original clears the value vector of ret (ReturnValues +0x24), maps the name with _GetFuncName, runs
// sfc::script::lua::Instance::pCall(m_instance, name, args, ret), increments s_numCalls and returns pCall's result.
// Missing: the ReturnValues header, Instance::pCall. Done here: the name mapping and the counter. Returns 0 until
// pCall is wired.
int LuaScript::Call(const char* funcName, const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&) {
    const char* const name = _GetFuncName(funcName);
    (void)name;  // Instance::pCall(name, args, ret) is missing
    ++s_numCalls;
    return 0;
}

// 0x37c41c LuaScript::Call(char const*, sfc::script::lua::Arguments const&)
// PARTIAL. The original builds a ReturnValues on the stack (40 bytes), calls the three-argument Call with it and
// destroys it. There is no ReturnValues header in the tree, so the stack object cannot be declared and no call is made.
void LuaScript::Call(const char*, const sfc::script::lua::Arguments&) {}

// 0x37c494 LuaScript::Call(char const*, sfc::script::lua::ReturnValues&)
// PARTIAL. Same as the three-argument Call with no arguments: clears ret, maps the name, runs pCall, increments
// s_numCalls and returns pCall's result. Missing: the ReturnValues header and Instance::pCall. Done here: the name
// mapping and the counter. Returns 0 until pCall is wired.
int LuaScript::Call(const char* funcName, sfc::script::lua::ReturnValues&) {
    const char* const name = _GetFuncName(funcName);
    (void)name;  // Instance::pCall(name, ret) is missing
    ++s_numCalls;
    return 0;
}

// 0x37c514 LuaScript::Call(char const*)
// PARTIAL. Same as the two-argument Call: it needs a stack ReturnValues (see 0x37c41c), so no call is made.
void LuaScript::Call(const char*) {}

// 0x37c584 LuaScript::LuaScript(bool)  (C1, complete-object constructor)
// 0x37c674 LuaScript::LuaScript(bool)  (C2, base-object constructor)
// PARTIAL. The two addresses have the same body and one C++ constructor covers both. The original order: vptr;
// Instance::Instance on m_instance (+0x04); the Binder at +0x10 gets its vptr (off_95B140), its Instance pointer
// (this+4) and 0; the maps, the string and unk_80 start empty; m_vfTableCollecting (+0x64) is cleared; and
// BindFunction() runs when skipBindings is false. Missing here: the Instance constructor and the Binder setup
// (m_instance and m_binder are opaque bytes until the sfc::script::lua headers exist). The rest is done.
LuaScript::LuaScript(bool skipBindings) {
    m_vfTableCollecting = false;  // +0x64
    if (!skipBindings) {
        BindFunction();
    }
}

// 0x37c934 LuaScript::_GetGameScript(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original takes the current level (Application::GetCurrentLevel on Singleton<Application>::s_inst) and,
// when there is one, pushes Level::GetMPGameScript() (the const char* config field at +0x314) as a string into ret.
// Missing: dh2/Level.hpp declares GetMPGameScript as int, but the original returns a const char*. Also missing:
// ReturnValues::pushString (no dh2/sfc/script/lua/ReturnValues.hpp in the tree).
void LuaScript::_GetGameScript(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues& ret, void*) {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level != nullptr) {
        // PARTIAL: ret.pushString(level->GetMPGameScript()) is missing (see above).
    }
    (void)ret;
}

// 0x37ca60 LuaScript::_GetHostPlayer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original calls PlayerManager::GetHostingPlayer on the PlayerManager global (dword_99F76C, see
// dh2/global/IsMultiplayerGame.hpp), reads the pointer at PlayerInfo +0x660 (Hex-Rays *((UserData**)info + 408)) and
// pushes it as a user-data value into ret. Missing: dh2/PlayerInfo.hpp (absent; PlayerInfo is incomplete in
// dh2/PlayerManager.hpp) and ReturnValues::pushUserData (no ReturnValues header). Nothing is pushed.
void LuaScript::_GetHostPlayer(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues&, void*) {}

// 0x37cb8c LuaScript::_GetHostPlayerDifficulty(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL. The original pushes the current level's difficulty (Level +0x118, Level::m_difficulty) as an integer into
// ret, or 0 when there is no current level. The level lookup and the difficulty are done here. Missing:
// ReturnValues::pushInteger (no dh2/sfc/script/lua/ReturnValues.hpp in the tree), so nothing is pushed.
void LuaScript::_GetHostPlayerDifficulty(const sfc::script::lua::Arguments&, sfc::script::lua::ReturnValues& ret, void*) {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    const int difficulty = level != nullptr ? level->m_difficulty : 0;
    (void)ret;
    (void)difficulty;  // ret.pushInteger(difficulty) is missing
}
