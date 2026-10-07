#include "menu_frame_clock.hpp"
#include <cassert>
#include <cstdio>
#include <chrono>
#include <cstdint>
using dh2::android_ui::MenuFrameClock;
using std::chrono::nanoseconds;
int main() {
    for(const int hz:{60,90,120,144,240}) {
        MenuFrameClock clock;
        std::int64_t elapsed=0,delivered=0,old_delivered=0;
        for(int i=0;i<hz*10;++i) {
            const auto next=std::int64_t(i+1)*1000000000/hz;
            const auto dt=next-elapsed;
            delivered+=clock.advance(nanoseconds(dt));
            old_delivered+=dt/1000000;
            elapsed=next;
            assert(delivered==elapsed/1000000);
        }
        assert(delivered==10000);
        std::printf("%d Hz | ten seconds | delivered %lld ms | old truncation %lld ms\n",hz,(long long)delivered,(long long)old_delivered);
    }
    MenuFrameClock clock;
    assert(clock.advance(nanoseconds(999999))==0);
    assert(clock.advance(nanoseconds(1))==1);
    assert(clock.advance(nanoseconds(-1))==0);
    assert(clock.advance(std::chrono::seconds(15))==100);
    assert(clock.advance(nanoseconds(500000))==0);
    clock.reset();
    assert(clock.advance(nanoseconds(500000))==0);
    assert(clock.advance(nanoseconds(500000))==1);
    std::puts("PASS | fractional conservation, gap cap, reset and nonpositive time");
}
