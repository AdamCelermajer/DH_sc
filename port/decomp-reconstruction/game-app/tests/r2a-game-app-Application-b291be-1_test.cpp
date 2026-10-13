// Test for batch r2a-game-app-Application-b291be-1 (Application::Application, 0x32d980; the body is shared with 0x32d79c
// and lives in g1-game-app-Application-b291be-2.cpp, which this test links). It checks the state the constructor leaves:
// the stored values, the null managers, the flags, the empty strings and the receiver list, the vptr, the first-run byte
// FirstTime, and the five job threads (CustomAlloc(0x10) then updateJob_thread(n), n = 1..5, in that order, the first
// stored at +0x04 and the others in the file-scope globals). Exit status 0 means every check passed.
//
// Link: check_tu.sh --test <this file> g1-game-app-Application-b291be-2.cpp
// The constructor's source file also holds the other Application methods, which reference engine and game code. That code
// is defined below as no-op stubs under the Itanium names. The stubs abort if called, and the constructor never calls them.
// The globals FirstTime and m_updateJobThread1..4 are defined by game-app/src/Globals.cpp in the original; the test
// defines them so the batch source links alone.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <new>
#include <string>

#include "dh2/Application.hpp"
#include "dh2/global/Globals.hpp"

// Defined by the globals batch in the original.
bool FirstTime = false;
updateJob_thread* m_updateJobThread1 = nullptr;
updateJob_thread* m_updateJobThread2 = nullptr;
updateJob_thread* m_updateJobThread3 = nullptr;
updateJob_thread* m_updateJobThread4 = nullptr;

namespace {

constexpr int kJobCount = 5;

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// What the constructor asked the allocator and the job thread constructor for, in call order.
int g_allocCalls = 0;
unsigned g_allocSizes[kJobCount] = {};
int g_jobCalls = 0;
int g_jobArgs[kJobCount] = {};
void* g_jobSelf[kJobCount] = {};

// Application storage. It is filled with a pattern before construction, so a member the constructor does not store
// keeps the pattern, and a member it stores shows the stored value.
alignas(Application) unsigned char g_storage[sizeof(Application)];

}  // namespace

// The allocator of the job threads. Defined here with C++ linkage, as the declaration in core-platform/memory.hpp.
void* CustomAlloc(unsigned size) {
    if (g_allocCalls < kJobCount) {
        g_allocSizes[g_allocCalls] = size;
    }
    ++g_allocCalls;
    return std::malloc(size);
}

// The debug output function that other Application methods call. The constructor never calls it.
void _DEBUG_OUT(const char*, ...) {}

extern "C" {

// Constructor of updateJob_thread, called by placement new. The test records the object and the argument.
void _ZN16updateJob_threadC1Ei(void* self, int argument) {
    if (g_jobCalls < kJobCount) {
        g_jobSelf[g_jobCalls] = self;
        g_jobArgs[g_jobCalls] = argument;
    }
    ++g_jobCalls;
}

// Application methods (other functions of the batch source). Never called from the constructor.
void _ZN11Application12IsInitFinishEv() { std::abort(); }
void _ZN11Application13SendGLHiScoreEv() { std::abort(); }
void _ZN11Application13ShowStatubBarEb() { std::abort(); }
void _ZN11Application14SetOrientationEib() { std::abort(); }
void _ZN11Application19IsInIGPOrGameCenterEv() { std::abort(); }
void _ZN11Application9ComputeDtEv() { std::abort(); }
void _ZNK11Application15GetCurrentLevelEv() { std::abort(); }

// Other classes' methods.
void _ZN11MenuManager11GetInstanceEv() { std::abort(); }
void _ZN11MenuManager13GetMenuByNameEPKc() { std::abort(); }
void _ZN11MenuManager5ResetEv() { std::abort(); }
void _ZN11MenuManager7PopMenuEPKc() { std::abort(); }
void _ZN11MenuManagerC1Ev() { std::abort(); }
void _ZN12EventManager6UpdateEd() { std::abort(); }
void _ZN12InputManager11GetInstanceEv() { std::abort(); }
void _ZN12InputManager24GetFirstConnectedGamepadEv() { std::abort(); }
void _ZN12StateMachine11SwitchStateEP9StateBaseb() { std::abort(); }
void _ZN12StateMachine5SleepEv() { std::abort(); }
void _ZN12StateMachine6UpdateEd() { std::abort(); }
void _ZN12StateMachine6WakeUpEv() { std::abort(); }
void _ZN12StateMachine9PushStateEP9StateBaseb() { std::abort(); }
void _ZNK12StateMachine4DrawEv() { std::abort(); }
void _ZN13DebugSwitches11GetInstanceEv() { std::abort(); }
void _ZN13DebugSwitches4loadEv() { std::abort(); }
void _ZN13DebugSwitches9GetSwitchERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE() { std::abort(); }
void _ZN13DebugSwitches9SetSwitchERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb() { std::abort(); }
void _ZN13ObjectManager19DBG_DumpRoomObjectsEv() { std::abort(); }
void _ZN13PlayerManager16RemoveAllPlayersEv() { std::abort(); }
void _ZN15SavegameManager12saveSettingsEv() { std::abort(); }
void _ZN15TouchScreenBase13ProcessEventsEv() { std::abort(); }
void _ZN15TouchScreenBase6updateEd() { std::abort(); }
void _ZN5Level13SG_SavePlayerEib() { std::abort(); }
void _ZN5Level18SG_SaveLocalPlayerEb() { std::abort(); }
void _ZN5Level9QuickSaveEb() { std::abort(); }
void _ZNK5Level36SG_DBG_TraceDetailedQuestInformationEi() { std::abort(); }
void _ZNK8MenuBase9IsVisibleEv() { std::abort(); }

// Static data of other classes, and the vtable of Application (the constructor stores its address). The test never
// reads the storage of the first two.
alignas(16) unsigned char _ZN11GSFlashMenu6s_instE[256];
bool _ZN8MenuBase11s_igmOpenedE = false;
void* _ZTV11Application[8] = {};

// Stubs for the external symbols of the other batch functions (Pause, ResetTouch, LoadLevel, GoToMainMenu, Update),
// which the constructor never calls. They abort if called.
void _ZN11Application12IsOnlineGameEv() { std::abort(); }
void _Z9GetOnlinev() { std::abort(); }
void _ZN7COnline14ReceivePacketsEv() { std::abort(); }
void _ZN7COnline11SendPacketsEv() { std::abort(); }
void _ZN6Device17IsHighPerformanceEv() { std::abort(); }
void _ZN12PerfCounters6UpdateEf() { std::abort(); }
void _ZN12PerfCounters15SetCounterValueERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEfff() { std::abort(); }
void _ZNK15TouchScreenBase14getTouchIDListB5cxx11Ev() { std::abort(); }
void _ZN7GSLevel9LoadLevelEPKcijjjbbii() { std::abort(); }
void _ZN5Level13ResetIsLoadedEv() { std::abort(); }
void _ZN17PlayerStatManager5ResetEv() { std::abort(); }
void _ZN13PlayerManager6UpdateEv() { std::abort(); }
void _ZN13TrophyManager20UnlockTrophiesGLLiveEv() { std::abort(); }
alignas(16) unsigned char _ZN9SingletonI12PerfCountersE6s_instE[256];
alignas(16) unsigned char _ZN17PlayerStatManager6s_instE[256];
void* _ZN13TrophyManager10s_instanceE = nullptr;

}  // extern "C"

namespace {

void TestStoredValues(const Application* app) {
    Check(app->unk_6c == 30, "unk_6c (+0x6c) is 30");
    Check(app->m_dtFactor == 1.0f, "m_dtFactor (+0x9c) is 1.0");
    Check(app->m_pauseTime == -1, "m_pauseTime (+0xa0) is -1");
    Check(app->m_lastTime == 0, "m_lastTime (+0x88) is 0");
    Check(app->m_frameCount == 0, "m_frameCount (+0x74) is 0");
}

void TestFlags(const Application* app) {
    Check(app->unk_78 == false, "unk_78 (+0x78) is 0");
    Check(app->unk_79 == false, "unk_79 (+0x79) is 0");
    Check(app->unk_7a == false, "unk_7a (+0x7a) is 0");
    Check(app->m_paused == false, "m_paused (+0xa4) is 0");
    Check(app->m_pauseMenuPushed == false, "m_pauseMenuPushed (+0xa5) is 0");
    Check(app->m_resumeMusic == false, "m_resumeMusic (+0xa6) is 0");
    Check(app->unk_a7 == false, "unk_a7 (+0xa7) is 0");
    Check(app->unk_a8 == false, "unk_a8 (+0xa8) is 0");
    Check(app->m_sceneUpdated == false, "m_sceneUpdated (+0xa9) is 0");
    Check(app->m_goToMainMenuPending == false, "m_goToMainMenuPending (+0xaa) is 0");
    Check(app->unk_ab == true, "unk_ab (+0xab) is 1");
    Check(app->unk_ad == false, "unk_ad (+0xad) is 0");
    Check(app->unk_b4 == true, "unk_b4 (+0xb4) is 1");
    Check(app->unk_ed == false, "unk_ed (+0xed) is 0");
    Check(app->unk_ee == false, "unk_ee (+0xee) is 0");
    Check(app->unk_10e == false, "unk_10e (+0x10e) is 0");
    Check(app->unk_10f == true, "unk_10f (+0x10f) is 1");
}

void TestNullManagers(const Application* app) {
    Check(app->m_device == nullptr, "m_device (+0x10) is null");
    Check(app->m_eventManager == nullptr, "m_eventManager (+0x14) is null");
    Check(app->m_stateMachine == nullptr, "m_stateMachine (+0x18) is null");
    Check(app->unk_1c == nullptr, "unk_1c (+0x1c) is null");
    Check(app->m_touchScreen == nullptr, "m_touchScreen (+0x20) is null");
    Check(app->m_accelerometer == nullptr, "m_accelerometer (+0x24) is null");
    Check(app->m_dataReloaderManager == nullptr, "m_dataReloaderManager (+0x28) is null");
    Check(app->m_pyDataConstants == nullptr, "m_pyDataConstants (+0x2c) is null");
    Check(app->m_pyDataArrays == nullptr, "m_pyDataArrays (+0x30) is null");
    Check(app->m_stringManager == nullptr, "m_stringManager (+0x34) is null");
    Check(app->m_objectManager == nullptr, "m_objectManager (+0x38) is null");
    Check(app->m_luaManager == nullptr, "m_luaManager (+0x3c) is null");
    Check(app->m_playerManager == nullptr, "m_playerManager (+0x40) is null");
    Check(app->m_physicalWorld == nullptr, "m_physicalWorld (+0x44) is null");
    Check(app->m_console == nullptr, "m_console (+0x48) is null");
    Check(app->m_savegameManager == nullptr, "m_savegameManager (+0x4c) is null");
    Check(app->m_zoomHandler == nullptr, "m_zoomHandler (+0x50) is null");
}

void TestContainers(const Application* app) {
    Check(app->m_eventReceivers.empty(), "m_eventReceivers (+0x08) is an empty list");
    Check(app->m_uncompiledData.empty(), "m_uncompiledData (+0xbc) is an empty string");
    Check(app->unk_d4.empty(), "unk_d4 (+0xd4) is an empty string");
}

void TestJobThreads(const Application* app) {
    Check(g_allocCalls == kJobCount, "five job threads are allocated");
    Check(g_jobCalls == kJobCount, "five job threads are constructed");
    for (int i = 0; i < kJobCount && i < g_allocCalls; ++i) {
        Check(g_allocSizes[i] == 0x10, "each job thread block is 0x10 bytes");
    }
    for (int i = 0; i < kJobCount && i < g_jobCalls; ++i) {
        Check(g_jobArgs[i] == i + 1, "job thread n is constructed with argument n");
    }
    Check(g_jobSelf[0] != nullptr && static_cast<void*>(app->m_updateJobThread0) == g_jobSelf[0],
          "m_updateJobThread0 (+0x04) holds job thread 1");
    Check(static_cast<void*>(m_updateJobThread1) == g_jobSelf[1], "m_updateJobThread1 holds job thread 2");
    Check(static_cast<void*>(m_updateJobThread2) == g_jobSelf[2], "m_updateJobThread2 holds job thread 3");
    Check(static_cast<void*>(m_updateJobThread3) == g_jobSelf[3], "m_updateJobThread3 holds job thread 4");
    Check(static_cast<void*>(m_updateJobThread4) == g_jobSelf[4], "m_updateJobThread4 holds job thread 5");
}

void TestFirstTime() {
    Check(FirstTime == true, "FirstTime is set to 1");
    Check(*reinterpret_cast<const unsigned char*>(&FirstTime) == 1, "FirstTime is stored as one byte");
}

void TestVptr(const Application* app) {
    // Itanium layout: the vptr points at the first virtual slot, two pointers past the start of the vtable.
    void* const* vptr = *reinterpret_cast<void* const* const*>(app);
    Check(vptr == reinterpret_cast<void* const*>(&_ZTV11Application[2]), "vptr points at the Application vtable");
}

}  // namespace

int main() {
    std::memset(g_storage, 0xAB, sizeof g_storage);
    Application* app = new (g_storage) Application();

    TestStoredValues(app);
    TestFlags(app);
    TestNullManagers(app);
    TestContainers(app);
    TestJobThreads(app);
    TestFirstTime();
    TestVptr(app);

    if (g_failures == 0) {
        std::printf("r2a-game-app-Application-b291be-1_test: all checks passed\n");
        return 0;
    }
    std::printf("r2a-game-app-Application-b291be-1_test: %d failure(s)\n", g_failures);
    return 1;
}
