// Batch g1-game-character-CharAIScript-ef5739-0 (part 1 of 1 of CharAIScript): the constructors and destructors,
// the state entry type CharAIScript::_State, the owner and script setup (SetCharacter), the four CallState*
// dispatchers and the two Lua callbacks. Declarations and layout come from dh2/CharAIScript.hpp.
//
// Offsets in comments are the original 32-bit offsets inside CharAIScript (this). The state table at +0x9c is a
// host std::map, not the STLport tree the original builds, so the tree-header stores in the original constructor
// and destructor are not copied; the std::map member does that work.
//
// The deleting destructor at 0x3d92d4 is not written (compiler-generated; see the ledger).
//
// Two gaps keep the Lua functions PARTIAL:
//   - dh2/sfc/script/lua/Binder.hpp is absent, so the two bindFunction registrations are not made.
//   - dh2/sfc/script/lua/Arguments.hpp and Value.hpp are absent, so the Lua argument decoding in the two
//     callbacks is not written.

#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/CharAIScript.hpp"
#include "dh2/Character.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel). Declared here instead of through core-platform/memory.hpp,
// which redefines glitch::core types that glitch-shared also defines (see game-anim AnimatedFX).
extern int gAssertLevel;

namespace {

// Source name the original passes to its ASSERT, as the other game-character sources spell it.
const char* const kCharAIScriptCpp =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\AI\\CharAIScript.cpp";

}  // namespace

// ---- CharAIScript::_State (one registered AI state, 0x60 bytes in the original) ----

// 0x3d9684 CharAIScript::_State::_State(void)
// The four strings are constructed empty. The original's STLport buffer setup has no equivalent here.
CharAIScript::_State::_State()
    : m_update(), m_conditions(), m_init(), m_post() {}

// 0x3d9648 CharAIScript::_State::_State(CharAIScript::_State const&)
// Copies the strings in declaration order (+0x00, +0x18, +0x30, +0x48).
CharAIScript::_State::_State(const _State& other)
    : m_update(other.m_update),
      m_conditions(other.m_conditions),
      m_init(other.m_init),
      m_post(other.m_post) {}

// 0x3d91b8 CharAIScript::_State::~_State(void)
// The original destroys the strings from +0x48 down to +0x00. Implicit member destruction runs in reverse
// declaration order, which is the same sequence.
CharAIScript::_State::~_State() = default;

// ---- CharAIScript constructors and destructors ----

// 0x3d8f44 CharAIScript::CharAIScript(bool)   (C1, complete object)
// 0x3d8fb0 CharAIScript::CharAIScript(bool)   (C2, base object; same body)
// PARTIAL: the field stores are complete (m_character = 0 at +0x98, m_currentState = 0 at +0xb4, state table empty).
// The path with skipBindings == false calls CharAIScriptBindFunction, which is partial (waits on the Binder).
// unk_b8, unk_bc and unk_c0 are not set here, as in the original. The AISExternal constructor clears them, and
// CharAIScript is abstract, so no bare CharAIScript object exists to read them.
CharAIScript::CharAIScript(bool skipBindings)
    : LuaScript(skipBindings),
      m_character(nullptr),
      m_currentState(nullptr) {
    if (!skipBindings) {
        CharAIScriptBindFunction();
    }
}

// 0x3d926c CharAIScript::~CharAIScript()   (D1, complete object)
// 0x3d92f0 CharAIScript::~CharAIScript()   (D2, base object; same body)
// The original erases the state table and resets its header before LuaScript::~LuaScript runs. clear() does the
// same; the LuaScript base destructor runs after the members here, as in the original.
CharAIScript::~CharAIScript() {
    m_states.clear();
}

// 0x3d92d4 CharAIScript::~CharAIScript()   (D0, deleting destructor) is not written: it is compiler-generated for
// delete. Its body is the D1 destructor followed by CustomFree(this), which the delete expression supplies.

// ---- Owner and script setup ----

// 0x3d90f8 CharAIScript::SetCharacter(Character*)
// ASSERT(c) at CharAIScript.cpp:131. gAssertLevel 2 stores 0 to address 0 in the original, and gAssertLevel 1 prints
// the message. At every level the original then reads the vptr of the null Character and faults, so the rebuild
// aborts at that point, before any state is written.
void CharAIScript::SetCharacter(Character* c) {
    if (c == nullptr) {
        if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "c", kCharAIScriptCpp, 131);
        }
        std::abort();
    }
    m_character = c;                                                   // +0x98
    // Slot +0x0c of the Character vtable is createBindings, called with the Binder embedded at +0x10.
    c->createBindings(*reinterpret_cast<sfc::script::lua::Binder*>(m_binder));
    m_scriptDirectory = "data/scripts/ai/";                            // +0x68, LuaScript::m_scriptDirectory
}

// ---- Lua script bindings ----

// 0x3d8ec8 CharAIScript::CharAIScriptBindFunction(void)
// PARTIAL: not written. The original calls sfc::script::lua::Binder::bindFunction on the Binder at +0x10 (this + 16)
// twice, both with userData = this: "RegisterAIState" with CharAIScript::_RegisterState, then "ChangeAIState" with
// CharAIScript::_ChangeState. The Binder has no declaration in this tree (dh2/sfc/script/lua/Binder.hpp is absent),
// so the two registrations are not made.
void CharAIScript::CharAIScriptBindFunction() {}

// 0x3d8f2c CharAIScript::BindFunction(void)
// PARTIAL: LuaScript::BindFunction runs first, as in the original. The second step is CharAIScriptBindFunction,
// which is not written, so the Lua functions are not registered.
void CharAIScript::BindFunction() {
    LuaScript::BindFunction();
    CharAIScriptBindFunction();
}

// ---- State dispatch ----
// Each dispatcher does nothing without a current state. Otherwise it runs one Lua function name of that state
// through LuaScript::Call(const char*). The strings are read at call time.

// 0x3d8e78 CharAIScript::CallStatePost(void)
// Runs m_post (state +0x48, original string data at +0x5c of the state).
void CharAIScript::CallStatePost() {
    if (m_currentState != nullptr) {
        LuaScript::Call(m_currentState->m_post.c_str());
    }
}

// 0x3d8e8c CharAIScript::CallStateInit(void)
// Runs m_init (state +0x30, original string data at +0x44).
void CharAIScript::CallStateInit() {
    if (m_currentState != nullptr) {
        LuaScript::Call(m_currentState->m_init.c_str());
    }
}

// 0x3d8ea0 CharAIScript::CallStateConditions(void)
// Runs m_conditions (state +0x18, original string data at +0x2c).
void CharAIScript::CallStateConditions() {
    if (m_currentState != nullptr) {
        LuaScript::Call(m_currentState->m_conditions.c_str());
    }
}

// 0x3d8eb4 CharAIScript::CallStateUpdate(void)
// Runs m_update (state +0x00, original string data at +0x14). The original's LuaScript* return value is the
// tail-call result of LuaScript::Call and is not used by any caller, so the function is void.
void CharAIScript::CallStateUpdate() {
    if (m_currentState != nullptr) {
        LuaScript::Call(m_currentState->m_update.c_str());
    }
}

// ---- Lua callbacks ----

// 0x3d9710 CharAIScript::_ChangeState(sfc::script::lua::Arguments const&,sfc::script::lua::ReturnValues &,void *)
// PARTIAL: not written. The Lua argument types have no declaration in this tree, so the argument count and the
// string value cannot be read. The steps, for the record: the binding acts only when it receives exactly one
// argument (the argument vector holds 112-byte values, and the count is compared through its inverse); that
// argument is the state name; the name is looked up in the state table (self, +0x9c); when found,
// CallStatePost(self) runs, m_currentState is set to the found state (+0xb4), and CallStateInit(self) runs. A
// missing name or a count other than one does nothing.
void CharAIScript::_ChangeState([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                [[maybe_unused]] void* self) {}

// 0x3da144 CharAIScript::_RegisterState(sfc::script::lua::Arguments const&,sfc::script::lua::ReturnValues &,void *)
// PARTIAL: not written, for the same reason as _ChangeState. The steps, for the record: the binding acts only when
// the argument count is 2 to 5 and every argument has Lua type tag 4 (string). The first string is the state name;
// the state table entry for it is found or created (operator[], default _State). Arguments 1 to count-1 assign, in
// order, m_update (+0x00), m_conditions (+0x18), m_init (+0x30) and m_post (+0x48); fields without an argument keep
// their previous values.
void CharAIScript::_RegisterState([[maybe_unused]] const sfc::script::lua::Arguments& args,
                                  [[maybe_unused]] sfc::script::lua::ReturnValues& ret,
                                  [[maybe_unused]] void* self) {}
