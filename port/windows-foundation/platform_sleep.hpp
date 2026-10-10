#pragma once

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <thread>

namespace dh::foundation {

// B066: Windows rounds every sleep up to the 15.6 ms scheduler tick unless the process asks for 1 ms timers.
// sleep(1) per frame then cost a whole tick (after a vsync-blocking swap this halved the frame rate to ~30 fps).
// Defined in the platform layer (platform_win32.cpp: timeBeginPeriod(1), loaded dynamically; no-op on Linux).
void platform_enable_precise_timers() noexcept;

inline void platform_sleep_milliseconds(unsigned milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// B066: frame pacing against an absolute deadline. begin() at the top of the loop, wait() at the bottom: it sleeps only
// the REMAINDER of the frame period (zero when a vsync-blocking swap already used it), with a short yield-spin for
// accuracy. Target defaults to 60 fps; DH_FPS_CAP=<n> overrides (0 = uncapped).
class FramePacer {
public:
    FramePacer() {
        platform_enable_precise_timers();
        double fps=60.0;
        if(const char* v=std::getenv("DH_FPS_CAP")){if(*v)fps=std::strtod(v,nullptr);}
        periodSeconds_=fps>0.0?1.0/fps:0.0;
    }
    void begin() noexcept {start_=clock::now();}
    // Returns the milliseconds actually slept/spun.
    double wait() noexcept {
        if(periodSeconds_<=0.0)return 0.0;
        const auto began=clock::now();
        const auto deadline=start_+std::chrono::duration_cast<clock::duration>(std::chrono::duration<double>(periodSeconds_));
        for(;;){
            const auto now=clock::now();
            if(now>=deadline)break;
            if(deadline-now>std::chrono::microseconds(2500))std::this_thread::sleep_for(std::chrono::milliseconds(1));
            else std::this_thread::yield();
        }
        return std::chrono::duration<double,std::milli>(clock::now()-began).count();
    }
private:
    using clock=std::chrono::steady_clock;
    clock::time_point start_=clock::now();
    double periodSeconds_=1.0/60.0;
};

} // namespace dh::foundation

// The shared Windows-era host uses GetTickCount for monotonic millisecond
// timestamps. Preserve its 32-bit wrap semantics on Linux without changing
// gameplay callers.
#if !defined(_WIN32)
inline std::uint32_t GetTickCount() noexcept {
    using namespace std::chrono;
    return static_cast<std::uint32_t>(duration_cast<milliseconds>(
        steady_clock::now().time_since_epoch()).count());
}
#endif
