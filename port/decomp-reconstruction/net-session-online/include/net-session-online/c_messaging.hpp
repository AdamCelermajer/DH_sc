// CMessaging: process-wide singleton that owns the send and receive queues.
// Only the singleton accessors are reconstructed so far. The constructor,
// queue handling and packet callbacks are still queued (see NOTES.md).
#pragma once

#include <cstdint>

class CMessaging {
public:
    // 0x80a710 _ZN10CMessaging13IsInitializedEv: 0 when no instance; otherwise the initialized flag at +0x04.
    static std::uint32_t IsInitialized();
    // 0x80a738 _ZN10CMessaging9TerminateEv: deletes the instance through its virtual destructor.
    static void Terminate();
    // 0x80a77c _ZN10CMessaging6UpdateEv: the original returns 0 with no work.
    static int Update();

    virtual ~CMessaging() = default;  // D0/D1/D2 are queued

    // Test hook: installs an instance for IsInitialized/Terminate tests.
    static void SetInstanceForTest(CMessaging* instance, bool initialized);

private:
    static CMessaging*& Instance();
    bool initialized_ = false;  // +0x04
};
