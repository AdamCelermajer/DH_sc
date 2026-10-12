#include "net-session-online/c_messaging.hpp"

CMessaging*& CMessaging::Instance() {
    static CMessaging* instance = nullptr;
    return instance;
}

// 0x80a710 _ZN10CMessaging13IsInitializedEv
std::uint32_t CMessaging::IsInitialized() {
    CMessaging* instance = Instance();
    if (instance == nullptr) {
        return 0;
    }
    return instance->initialized_ ? 1u : 0u;
}

// 0x80a738 _ZN10CMessaging9TerminateEv
void CMessaging::Terminate() {
    CMessaging* instance = Instance();
    if (instance != nullptr) {
        delete instance;
        Instance() = nullptr;
    }
}

// 0x80a77c _ZN10CMessaging6UpdateEv
int CMessaging::Update() { return 0; }

void CMessaging::SetInstanceForTest(CMessaging* instance, bool initialized) {
    Instance() = instance;
    if (instance != nullptr) {
        instance->initialized_ = initialized;
    }
}
