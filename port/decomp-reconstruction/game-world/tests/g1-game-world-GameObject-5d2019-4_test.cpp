// Test for batch g1-game-world-GameObject-5d2019-4.
//
// Covers the two parts of the unit that run without the Lua layer: GameObject::_PopTargetList (the state change on the
// target queue at +0x304) and GameObject::LoadExternalScript (the script lifecycle at +0x300). The object is zero-filled
// and never constructed, as in the other GameObject tests. The queue is constructed in place on that storage and
// destroyed at the end.
//
// The LuaScript constructor, Load and destructor, and PFObject::IsInWater / IsOverAHole (called by the partial bindings
// in the unit) are stubs here. They record calls and never reach the engine.
//
// _PopTargetList takes an Arguments and a ReturnValues, which have no declarations in the tree. The test passes references
// into unused storage. The function never reads them.

#include <cstdio>
#include <cstring>
#include <deque>
#include <new>
#include <queue>

#include "dh2/GameObject.hpp"
#include "dh2/LuaScript.hpp"

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                             \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

int g_luaCtorCalls = 0;
int g_luaDtorCalls = 0;
int g_loadResult = 1;

using TargetQueue = std::priority_queue<ObjectSearcher::TargetInfo, std::deque<ObjectSearcher::TargetInfo>,
                                        ObjectSearcher::TargetSorter>;

// Nearest target first: the comparator says a is "less" (lower priority) when it is farther away.
bool ByDistance(const ObjectSearcher::TargetInfo& a, const ObjectSearcher::TargetInfo& b) {
    return a.distance > b.distance;
}

alignas(GameObject) unsigned char g_storage[sizeof(GameObject)];
alignas(16) unsigned char g_argsStorage[16];
alignas(16) unsigned char g_retStorage[16];

GameObject* Storage() {
    return reinterpret_cast<GameObject*>(g_storage);
}

const sfc::script::lua::Arguments& Args() {
    return *reinterpret_cast<const sfc::script::lua::Arguments*>(g_argsStorage);
}

sfc::script::lua::ReturnValues& Ret() {
    return *reinterpret_cast<sfc::script::lua::ReturnValues*>(g_retStorage);
}

}  // namespace

// Stubs for the engine code the unit calls.
LuaScript::LuaScript(bool /*skipBindings*/) {
    ++g_luaCtorCalls;
}

int LuaScript::Load(const char* /*file*/) {
    return g_loadResult;
}

LuaScript::~LuaScript() {
    ++g_luaDtorCalls;
}

bool PFObject::IsInWater() const {
    return false;
}

bool PFObject::IsOverAHole() const {
    return false;
}

int main() {
    // _PopTargetList: pops the nearest entry of a non-empty queue and does nothing on an empty one.
    std::memset(g_storage, 0, sizeof g_storage);
    GameObject* go = Storage();
    TargetQueue* queue = new (static_cast<void*>(&go->m_targetList.m_results))
        TargetQueue(ObjectSearcher::TargetSorter(&ByDistance));

    GameObject::_PopTargetList(Args(), Ret(), go);  // empty: no effect
    EXPECT(queue->empty());

    const float distances[3] = {3.0f, 1.0f, 2.0f};
    for (float d : distances) {
        queue->push(ObjectSearcher::TargetInfo{nullptr, d, 0.0f, 0, 0.0f});
    }
    EXPECT(queue->size() == 3);
    EXPECT(queue->top().distance == 1.0f);

    GameObject::_PopTargetList(Args(), Ret(), go);
    EXPECT(queue->size() == 2);
    EXPECT(queue->top().distance == 2.0f);

    GameObject::_PopTargetList(Args(), Ret(), go);
    EXPECT(queue->size() == 1);
    EXPECT(queue->top().distance == 3.0f);

    GameObject::_PopTargetList(Args(), Ret(), go);
    EXPECT(queue->empty());

    GameObject::_PopTargetList(Args(), Ret(), go);  // empty again: no effect
    EXPECT(queue->empty());
    queue->~TargetQueue();

    // LoadExternalScript: the script is created and named, released when no file is given, and released when Load fails.
    std::memset(g_storage, 0, sizeof g_storage);
    go = Storage();
    EXPECT(go->m_script == nullptr);

    g_loadResult = 1;
    go->LoadExternalScript("data/scripts/a.lua", "a");
    EXPECT(go->m_script != nullptr);
    EXPECT(g_luaCtorCalls == 1);
    EXPECT(go->m_script != nullptr && go->m_script->m_scriptDirectory == "a");

    go->LoadExternalScript(nullptr, nullptr);  // no file: the current script is released
    EXPECT(go->m_script == nullptr);
    EXPECT(g_luaDtorCalls == 1);

    g_loadResult = 0;  // Load fails: the new script is released at once
    go->LoadExternalScript("data/scripts/bad.lua", nullptr);
    EXPECT(go->m_script == nullptr);
    EXPECT(g_luaCtorCalls == 2);
    EXPECT(g_luaDtorCalls == 2);

    go->LoadExternalScript("", "x");  // empty file name: nothing is created
    EXPECT(go->m_script == nullptr);
    EXPECT(g_luaCtorCalls == 2);

    if (g_failures == 0) {
        std::printf("GameObject-5d2019-4: all checks passed\n");
        return 0;
    }
    std::printf("GameObject-5d2019-4: %d failures\n", g_failures);
    return 1;
}
