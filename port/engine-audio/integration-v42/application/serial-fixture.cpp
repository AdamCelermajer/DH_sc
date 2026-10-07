#include "application_audio_serial_v42.hpp"
#include <iostream>
#include <stdexcept>
int main() {
    dh2::android_audio::ApplicationAudioSerialV42 serial;unsigned checks{};
    auto check=[&](bool value){++checks;if(!value)throw std::runtime_error("application owner serial assertion");};
    bool closing=false;
    auto close=[&](std::uint64_t owner){if(!serial.matches(owner))return false;closing=true;return true;};
    check(serial.current()==0);check(!close(0)&&!closing);
    const auto first=serial.reserve();check(first==1);check(serial.reserve()==first);
    check(close(first));serial.clear();closing=false;check(!close(first)&&!closing);
    const auto next=serial.reserve();check(next==2);check(!close(first)&&!closing);
    check(!close(0)&&!closing);check(close(next));
    for(unsigned i=0;i<1000;++i) {
        const auto stale=serial.current();serial.clear();closing=false;
        const auto actual=serial.reserve();check(actual>stale);
        check(!close(stale)&&!closing);check(serial.current()==actual);
    }
    std::cout<<"PASS "<<checks<<" owner-serial checks; reserved identity stable and stale/zero close has no effect\n";
}
