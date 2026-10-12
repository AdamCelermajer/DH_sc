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

    // Initialize creates the singleton once; GetInstance returns the same one.
    CHECK(CMessaging::Initialize() == 0);
    CHECK(CMessaging::IsInitialized() == 1);
    CMessaging* first = CMessaging::GetInstance();
    CHECK(first != nullptr);
    CHECK(CMessaging::GetInstance() == first);
    CHECK(CMessaging::Initialize() == 0);
    CHECK(CMessaging::GetInstance() == first);
    CMessaging::Terminate();
    CHECK(CMessaging::IsInitialized() == 0);

    // Lost and acknowledged packet callbacks do nothing when the packet map is empty.
    CMessaging::Initialize();
    CMessaging::sProcessLostPacket(3, 4);
    CMessaging::sProcessAcknowledgedPacket(3, 4);
    CMessaging::Terminate();

    std::printf("%s\n", failures ? "c_messaging: FAILED" : "c_messaging: ok");
    return failures != 0;
}
