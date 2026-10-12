#include "glitch-core-io/os/logging.hpp"

namespace glitch::os {

ILogger* Printer::Logger = nullptr;
float Timer::VirtualTimerSpeed = 0.f;  // placeholder: the original initial value is not recovered
int Timer::VirtualTimerStopCounter = 0;

// 0x60ad80 _ZN6glitch2os7Printer11setLogLevelENS_10ELOG_LEVELE
void Printer::setLogLevel(ELOG_LEVEL level) {
    if (Logger) Logger->setLogLevel(level);
}

// 0x60adc0 _ZN6glitch2os7Printer11getLogLevelEv
ELOG_LEVEL Printer::getLogLevel() {
    return Logger ? Logger->getLogLevel() : ELL_NONE;
}

// 0x60aca0 _ZN6glitch2os7Printer3logEPKcNS_10ELOG_LEVELE
void Printer::log(const char* text, ELOG_LEVEL level) {
    if (Logger) Logger->log(text, level);
}

// 0x60ace8 _ZN6glitch2os7Printer3logEPKcS3_NS_10ELOG_LEVELE
void Printer::log(const char* text, const char* hint, ELOG_LEVEL level) {
    if (Logger) Logger->log(text, hint, level);
}

// 0x60ad38 _ZN6glitch2os7Printer3logEPKwNS_10ELOG_LEVELE
void Printer::log(const wchar_t* text, ELOG_LEVEL level) {
    if (Logger) Logger->log(text, level);
}

// 0x60ae9c _ZN6glitch2os5Timer8getSpeedEv
float Timer::getSpeed() { return VirtualTimerSpeed; }

// 0x60aebc _ZN6glitch2os5Timer9isStoppedEv
bool Timer::isStopped() { return VirtualTimerStopCounter != 0; }

}  // namespace glitch::os
