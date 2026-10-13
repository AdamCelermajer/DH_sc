// Test for batch g1-game-app-pack4-c4671c-0. Checks the parts of the script command group that run without the engine:
// Init and Update do nothing, every IsBlocking returns false, the five empty Execute bodies make no DebugSwitches call,
// and the three PARTIAL Execute bodies make only the isTracingScriptCmd trace probe (DebugSwitches::GetInstance, then
// GetSwitch with that name). Exit status 0 means every check passed.
//
// The DebugSwitches members are stubbed below (the game-app DebugSwitches batch defines them in the game build). The
// stub object is never constructed and the stubs never touch it, so only the calls and their arguments are observed.
#include <cstdio>
#include <string>

#include "dh2/DebugSwitches.hpp"
#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/Script_AIChangeScript.hpp"
#include "dh2/Script_AIDoSkill.hpp"
#include "dh2/Script_AIEnableTimer.hpp"
#include "dh2/Script_AIResetTimer.hpp"
#include "dh2/Script_AISetHP.hpp"
#include "dh2/Script_AISetInt.hpp"
#include "dh2/Script_ActivateTriggerPlate.hpp"
#include "dh2/Script_AutoEquip.hpp"

// Test-only stubs for the DebugSwitches members the batch calls.
namespace {
alignas(DebugSwitches) unsigned char g_switchStorage[sizeof(DebugSwitches)];
int g_loads = 0;
int g_switchReads = 0;
std::string g_lastSwitch;
}  // namespace

DebugSwitches* DebugSwitches::GetInstance() {
    ++g_loads;
    return reinterpret_cast<DebugSwitches*>(g_switchStorage);
}

bool DebugSwitches::GetSwitch(const std::string& name) {
    ++g_switchReads;
    g_lastSwitch = name;
    return false;
}

namespace {

int g_failures = 0;

void Check(bool ok, const char* name, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s: %s\n", name, what);
        ++g_failures;
    }
}

// Init and Update are inherited by every command. They must do nothing and make no DebugSwitches call.
void TestInitUpdate() {
    Script_AISetHP cmd{};
    ScriptCmdImpl& base = cmd;
    const int loads = g_loads;
    const int reads = g_switchReads;
    base.Init();
    base.Update();
    Check(g_loads == loads && g_switchReads == reads, "ScriptCmdImpl", "Init or Update made a DebugSwitches call");
}

template <class T>
void CheckNotBlocking(const char* name) {
    T cmd{};
    const ScriptCmdImpl& base = cmd;
    Check(!base.IsBlocking(), name, "IsBlocking is not false");
}

void TestIsBlocking() {
    CheckNotBlocking<Script_AIChangeScript>("Script_AIChangeScript");
    CheckNotBlocking<Script_AIDoSkill>("Script_AIDoSkill");
    CheckNotBlocking<Script_AIEnableTimer>("Script_AIEnableTimer");
    CheckNotBlocking<Script_AIResetTimer>("Script_AIResetTimer");
    CheckNotBlocking<Script_AISetHP>("Script_AISetHP");
    CheckNotBlocking<Script_AISetInt>("Script_AISetInt");
    CheckNotBlocking<Script_ActivateTriggerPlate>("Script_ActivateTriggerPlate");
    CheckNotBlocking<Script_AutoEquip>("Script_AutoEquip");
}

// The empty Execute bodies: no DebugSwitches call, whatever the arguments.
template <class T>
void CheckEmptyExecute(const char* name) {
    T cmd{};
    ScriptCmdImpl& base = cmd;
    const int loads = g_loads;
    const int reads = g_switchReads;
    base.Execute(false, 3);
    base.Execute(true, -1);
    Check(g_loads == loads && g_switchReads == reads, name, "empty Execute made a DebugSwitches call");
}

void TestEmptyExecutes() {
    CheckEmptyExecute<Script_AIChangeScript>("Script_AIChangeScript");
    CheckEmptyExecute<Script_AIDoSkill>("Script_AIDoSkill");
    CheckEmptyExecute<Script_AIEnableTimer>("Script_AIEnableTimer");
    CheckEmptyExecute<Script_AIResetTimer>("Script_AIResetTimer");
    CheckEmptyExecute<Script_AISetHP>("Script_AISetHP");
}

// The PARTIAL Execute bodies: exactly one load and one GetSwitch of "isTracingScriptCmd" per call.
template <class T>
void CheckTraceProbe(const char* name) {
    T cmd{};
    ScriptCmdImpl& base = cmd;
    const int loads = g_loads;
    const int reads = g_switchReads;
    base.Execute(true, 7);
    Check(g_loads == loads + 1, name, "trace probe did not load the switches once");
    Check(g_switchReads == reads + 1, name, "trace probe did not read exactly one switch");
    Check(g_lastSwitch == "isTracingScriptCmd", name, "trace probe read the wrong switch");
}

void TestTraceProbes() {
    CheckTraceProbe<Script_AISetInt>("Script_AISetInt");
    CheckTraceProbe<Script_ActivateTriggerPlate>("Script_ActivateTriggerPlate");
    CheckTraceProbe<Script_AutoEquip>("Script_AutoEquip");
}

}  // namespace

int main() {
    TestInitUpdate();
    TestIsBlocking();
    TestEmptyExecutes();
    TestTraceProbes();
    if (g_failures != 0) {
        std::printf("%d check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("g1-game-app-pack4-c4671c-0: all checks passed\n");
    return 0;
}
