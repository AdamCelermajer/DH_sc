#include "net-session-online/c_messaging.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(CMessaging::IsInitialized() == 0);
    CHECK(CMessaging::Update() == 0);
    CMessaging::SetInstanceForTest(new CMessaging(), false);
    CHECK(CMessaging::IsInitialized() == 0);
    CMessaging::SetInstanceForTest(nullptr, false);

    CMessaging::SetInstanceForTest(new CMessaging(), true);
    CHECK(CMessaging::IsInitialized() == 1);
    CMessaging::Terminate();
    CHECK(CMessaging::IsInitialized() == 0);
    CMessaging::Terminate();  // no instance: no-op

    std::printf("%s\n", failures ? "c_messaging: FAILED" : "c_messaging: ok");
    return failures != 0;
}
