// vox::DriverInterface base defaults and the driver-side hooks the engine calls.
#include <cstdio>

#include "vox-audio/vox/driver.hpp"

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

// A concrete driver: the base leaves Suspend and Resume pure, so the test driver
// records how often the engine calls them.
class CountingDriver : public vox::DriverInterface {
public:
    void Suspend() override { ++suspends; }
    void Resume() override { ++resumes; }
    int suspends = 0;
    int resumes = 0;
};

// Source driver with no overrides: the base bodies apply.
class PlainSource : public vox::DriverSourceInterface {};
}  // namespace

int main() {
    CountingDriver driver;
    vox::DriverInterface* base = &driver;
    base->Suspend();
    base->Suspend();
    base->Resume();
    CHECK(driver.suspends == 2);
    CHECK(driver.resumes == 1);

    // Base defaults: SetOutputMode reports 0, GetOutputMode reports 1.
    CHECK(base->SetOutputMode(static_cast<vox::VoxOutputMode>(2)) == 0);
    CHECK(base->GetOutputMode() == 1);

    PlainSource source;
    CHECK(source.AllowBufferReference() == 0);

    std::printf("driver_test: %s\n", g_failures == 0 ? "ok" : "FAILED");
    return g_failures == 0 ? 0 : 1;
}
