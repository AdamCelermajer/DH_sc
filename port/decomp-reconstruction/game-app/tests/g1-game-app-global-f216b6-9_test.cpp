// Test for batch g1-game-app-global-f216b6-9. Covers the quest state predicates (IsNonDebugQuestClosedQuest,
// IsNonDebugQuestActiveButNotClosedQuest, the IsDebugQuest thunk), the hint-page pick of NativeGetLoadingTipStrID
// and the credit value of NativeGetCreditMovement. Exit status 0 means every check passed.
//
// Quest::IsDebug and IsNonDebugQuest are defined in other batches, so this test defines them with the bodies the
// pseudocode gives (IsNonDebugQuest is !Quest::IsDebug). Quest is never constructed: the predicates read only
// m_state (+0x00) and pass the address on, so the test uses a raw buffer for each quest.
#include <cstdint>
#include <cstdio>

#include "dh2/Quest.hpp"
#include "dh2/global/native_app_bindings.hpp"
#include "dh2/global/quest_predicates.hpp"

namespace {

int g_failures = 0;

// The one quest that IsDebug reports as a debug quest. Null means no quest is a debug quest.
const Quest* g_debugQuest = nullptr;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

struct FakeQuest {
    alignas(Quest) unsigned char bytes[sizeof(Quest)];
};

// Stores the state at +0x00 (m_state) of the buffer and returns it as a Quest pointer.
const Quest* MakeQuest(FakeQuest& storage, int state) {
    Quest* quest = reinterpret_cast<Quest*>(storage.bytes);
    quest->m_state = state;
    return quest;
}

}  // namespace

// Defined elsewhere in the tree (Quest.cpp, and the batch that covers 0x43b4dc). Bodies as in the pseudocode.
bool Quest::IsDebug() const {
    return this == g_debugQuest;
}

bool IsNonDebugQuest(const Quest* quest) {
    return !quest->IsDebug();
}

namespace {

void TestNonDebugStatePredicates() {
    FakeQuest storage;
    g_debugQuest = nullptr;
    for (int state = 0; state <= 13; ++state) {
        const Quest* quest = MakeQuest(storage, state);
        Check(IsNonDebugQuestClosedQuest(quest) == (state > 12),
              "IsNonDebugQuestClosedQuest is true only above QUEST_CLOSED (states 13)");
        Check(IsNonDebugQuestActiveButNotClosedQuest(quest) == (state >= 6 && state <= 12),
              "IsNonDebugQuestActiveButNotClosedQuest covers states 6 to 12");
    }
}

void TestDebugQuestRejected() {
    FakeQuest storage;
    const Quest* quest = MakeQuest(storage, 9);

    g_debugQuest = quest;
    Check(IsDebugQuest(quest) == 1, "IsDebugQuest forwards to Quest::IsDebug and reports true");
    Check(IsNonDebugQuestClosedQuest(quest) == 0, "a debug quest is never a closed non-debug quest");
    Check(IsNonDebugQuestActiveButNotClosedQuest(quest) == 0, "a debug quest is never an active non-debug quest");

    g_debugQuest = nullptr;
    Check(IsDebugQuest(quest) == 0, "IsDebugQuest reports false for a non-debug quest");
    Check(IsNonDebugQuestActiveButNotClosedQuest(quest) == 1, "state 9 is active-but-not-closed when not debug");
    g_debugQuest = nullptr;
}

void TestPickHintPageOffset() {
    // Expected values come from the 32-bit LCG (59051 * seed + 177149) % 0xDAF26B with wraparound.
    std::uint32_t seed = 1;
    std::uint32_t offset = PickHintPageOffset(seed, 5);
    Check(offset == 0 && seed == 236200u, "seed 1 with 5 pages: seed becomes 236200, row 0, offset 0");

    offset = PickHintPageOffset(seed, 10);
    Check(offset == 36 && seed == 1302343u, "seed 236200 with 10 pages: seed becomes 1302343, row 3, offset 36");

    const std::uint32_t before = seed;
    offset = PickHintPageOffset(seed, 0);
    Check(offset == 0 && seed == before, "an empty table gives offset 0 and leaves the seed alone");
}

void TestCreditMovementValue() {
    Check(CreditMovementValue(true, 0) == 2.0, "HTC device: credit movement is 2.0");
    Check(CreditMovementValue(true, 500) == 2.0, "HTC device ignores the frame time");
    Check(CreditMovementValue(false, 0) == 1.0, "frame time 0 gives 1");
    Check(CreditMovementValue(false, 24) == 1.0, "24 ms gives 1");
    Check(CreditMovementValue(false, 25) == 2.0, "25 ms gives 2");
    Check(CreditMovementValue(false, 50) == 3.0, "50 ms gives 3");
    Check(CreditMovementValue(false, -1) == 171798692.0, "a negative frame time divides as unsigned");
}

}  // namespace

int main() {
    TestNonDebugStatePredicates();
    TestDebugQuestRejected();
    TestPickHintPageOffset();
    TestCreditMovementValue();
    if (g_failures == 0) {
        std::printf("g1-game-app-global-f216b6-9 test: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
