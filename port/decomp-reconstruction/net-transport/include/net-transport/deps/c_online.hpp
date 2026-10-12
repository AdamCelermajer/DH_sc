// Stand-in for COnline, the online service that supplies the clock and disconnect reports.
// Owned by another category; declared here so the connection code compiles on its own.
// The connection code reads slot 0 as the clock (in seconds-free milliseconds, 32-bit).
// NOTE: the vtable dump names slot 0 DetectDisconnect; the clock reading is kept as in
// the decompile and is unresolved.
#pragma once

#include <chrono>
#include <cstdint>

class COnline {
public:
    virtual ~COnline() = default;

    // Vtable +0x00: the 32-bit millisecond clock.
    virtual std::int32_t GetTime() const {
        auto now = std::chrono::steady_clock::now().time_since_epoch();
        return static_cast<std::int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
    }

    // _ZN7COnline16ReportDisconnectEii (name from the call sites). Stand-in: nothing reported.
    void ReportDisconnect(int reason, int detail) const {
        (void)reason;
        (void)detail;
    }

    // +0x20: reference time read by CConnection::ProcessPing (name not recovered).
    std::int32_t ping_reference_ = 0;
};

// GetOnline() (no arguments in the original; the decompile passes stray registers).
inline COnline& GetOnline() {
    static COnline instance;
    return instance;
}
