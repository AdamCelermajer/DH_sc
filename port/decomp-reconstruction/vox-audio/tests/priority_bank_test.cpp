// PriorityBankManager admission rules and removal.
#include <cstdio>

#include "vox-audio/vox/priority_bank.hpp"

namespace {
int g_failures = 0;
#define CHECK(cond)                                                       \
    do {                                                                  \
        if (!(cond)) {                                                    \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__,   \
                         __LINE__, #cond);                                \
            ++g_failures;                                                 \
        }                                                                 \
    } while (0)

int g_e1, g_e2, g_e3;
}  // namespace

int main() {
    vox::PriorityBankManager m(2);
    // Defaults: unlimited count, behaviour 3 (refuse when full, never full here).
    CHECK(m.CanAddEmitter(0, 5));
    CHECK(!m.CanAddEmitter(2, 5));   // index out of range
    CHECK(!m.CanAddEmitter(-1, 5));

    // Bank 1: max 1 entry, behaviour 1 (evict a lower priority).
    CHECK(m.SetPriorityBank(1, 0, 1, 1));
    CHECK(!m.CanAddEmitter(1, -1));  // below min priority 0
    CHECK(m.CanAddEmitter(1, 3));    // room: nothing in the bank yet
    CHECK(m.AddPriorityBank(0, 1, 0));  // bank 2: behaviour 0
    CHECK(m.CanAddEmitter(2, 0));

    // Fill bank 1 with a priority-4 emitter.
    vox::PriorityBankManager fill(1);
    CHECK(fill.SetPriorityBank(0, 0, 1, 2));
    CHECK(fill.CanAddEmitter(0, 9));  // empty bank: has room
    CHECK(!fill.RemoveEmitter(0, nullptr));
    CHECK(!fill.RemoveEmitter(0, &g_e1));  // not in the bank

    if (g_failures == 0) {
        std::printf("priority_bank_test: all checks passed\n");
        return 0;
    }
    std::printf("priority_bank_test: %d failures\n", g_failures);
    return 1;
}
