// Test for batch g1-game-character-CharAIScript-ef5739-0.
//
// The four CallState* dispatchers and the _State copy and default constructors run without the engine.
// CharAIScript is abstract, so no object is constructed: the storage is zero-filled and only m_currentState
// (+0xb4) is written, as in the AISExternal test. The dispatchers are called qualified (non-virtual), so the
// zero vptr is never used. LuaScript::Call records the names it is given.
//
// The stubs below define the LuaScript members and gAssertLevel that the rebuilt CharAIScript code links against.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "dh2/CharAIScript.hpp"
#include "dh2/LuaScript.hpp"

// Global assert level, read by CharAIScript::SetCharacter (not called here).
int gAssertLevel = 0;

namespace {

std::vector<std::string> g_calls;
int g_failures = 0;

void Expect(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// Zero-filled storage for the CharAIScript that is never constructed. Only m_currentState is written.
alignas(CharAIScript) unsigned char g_storage[sizeof(CharAIScript)];

CharAIScript* MakeAI() {
    std::memset(g_storage, 0, sizeof(g_storage));
    return reinterpret_cast<CharAIScript*>(g_storage);
}

}  // namespace

// ---- LuaScript stubs ----

LuaScript::LuaScript(bool) {}
LuaScript::~LuaScript() {}
void LuaScript::BindFunction() {}
void LuaScript::Call(const char* funcName) { g_calls.emplace_back(funcName); }

// ---- Checks ----

int main() {
    // No current state: no dispatcher calls Lua.
    CharAIScript* ai = MakeAI();
    ai->m_currentState = nullptr;
    ai->CharAIScript::CallStatePost();
    ai->CharAIScript::CallStateInit();
    ai->CharAIScript::CallStateConditions();
    ai->CharAIScript::CallStateUpdate();
    Expect(g_calls.empty(), "no current state: nothing is called");

    // A current state with four distinct names: each dispatcher runs its own field.
    CharAIScript::_State st;
    st.m_update = "Upd";
    st.m_conditions = "Cond";
    st.m_init = "Init";
    st.m_post = "Post";
    ai->m_currentState = &st;

    g_calls.clear();
    ai->CharAIScript::CallStateInit();
    Expect(g_calls == std::vector<std::string>{"Init"}, "CallStateInit runs m_init");

    g_calls.clear();
    ai->CharAIScript::CallStateConditions();
    Expect(g_calls == std::vector<std::string>{"Cond"}, "CallStateConditions runs m_conditions");

    g_calls.clear();
    ai->CharAIScript::CallStateUpdate();
    Expect(g_calls == std::vector<std::string>{"Upd"}, "CallStateUpdate runs m_update");

    g_calls.clear();
    ai->CharAIScript::CallStatePost();
    Expect(g_calls == std::vector<std::string>{"Post"}, "CallStatePost runs m_post");

    // The names are read at call time: a change to the state shows in the next call.
    st.m_post = "Post2";
    g_calls.clear();
    ai->CharAIScript::CallStatePost();
    Expect(g_calls == std::vector<std::string>{"Post2"}, "CallStatePost reads m_post at call time");

    // Switching the current state to another one changes what the dispatchers run.
    CharAIScript::_State other;
    other.m_init = "Init2";
    ai->m_currentState = &other;
    g_calls.clear();
    ai->CharAIScript::CallStateInit();
    Expect(g_calls == std::vector<std::string>{"Init2"}, "CallStateInit follows the current state");

    // The copy constructor copies all four names; the default constructor leaves them empty.
    CharAIScript::_State copy(st);
    Expect(copy.m_update == "Upd" && copy.m_conditions == "Cond" && copy.m_init == "Init" && copy.m_post == "Post2",
           "_State copy keeps the four names");

    CharAIScript::_State empty;
    Expect(empty.m_update.empty() && empty.m_conditions.empty() && empty.m_init.empty() && empty.m_post.empty(),
           "_State default is empty");

    if (g_failures == 0) {
        std::printf("g1-game-character-CharAIScript-ef5739-0 test: OK\n");
    }
    return g_failures == 0 ? 0 : 1;
}
