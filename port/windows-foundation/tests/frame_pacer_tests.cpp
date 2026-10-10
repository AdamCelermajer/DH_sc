// B066: the loop pacer sleeps only the remainder of the frame period, accurately, and not at all when the frame
// already took longer (e.g. a vsync-blocking swap).
#include "../platform_sleep.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace dh::foundation;
using clock_type = std::chrono::steady_clock;
static void check(bool value, const char* message) { if (!value) throw std::runtime_error(message); }
static double since(clock_type::time_point start) { return std::chrono::duration<double, std::milli>(clock_type::now() - start).count(); }

int main() { try {
#if defined(_WIN32)
    _putenv("DH_FPS_CAP=100");   // 10 ms period
#else
    setenv("DH_FPS_CAP", "100", 1);
#endif
    FramePacer pacer;
    // 1. Fast frame: the pacer fills the period (10 ms), accurately (the old sleep(1) overshot to a 15.6 ms tick).
    double worst = 0, best = 1e9;
    for (int i = 0; i < 30; ++i) {
        const auto start = clock_type::now(); pacer.begin();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        pacer.wait();
        const double elapsed = since(start);
        worst = std::max(worst, elapsed); best = std::min(best, elapsed);
    }
    std::cout << "paced frame best=" << best << " worst=" << worst << " ms (target 10)\n";
    check(best >= 9.9, "Pacer released the frame early");
    check(worst < 13.0, "Pacer overslept (timer tick not honoured)");
    // 2. Slow frame (already past the period, as after a vsync wait): no extra sleep at all.
    {
        const auto start = clock_type::now(); pacer.begin();
        std::this_thread::sleep_for(std::chrono::milliseconds(14));
        const double waited = pacer.wait();
        check(waited < 1.0, "Pacer added a sleep after a frame that already exceeded the period");
        check(since(start) < 20.0, "Slow frame was stretched");
    }
    std::cout << "PASS frame pacer\n";
    return 0;
} catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; } }
