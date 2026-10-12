// glitch::os::Printer and glitch::os::Timer. Printer forwards to a global
// logger object (a static pointer, null when none is installed). Timer keeps
// its virtual-time state in statics.
#pragma once

namespace glitch::os {

enum ELOG_LEVEL { ELL_DEBUG = 0, ELL_INFORMATION = 1, ELL_WARNING = 2, ELL_ERROR = 3, ELL_NONE = 4 };

// Interface the Printer forwards to. Slot numbers from the original logger
// vtable are noted; the order here is not binary compatible.
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual ELOG_LEVEL getLogLevel() const = 0;                       // +0x0c
    virtual void setLogLevel(ELOG_LEVEL level) = 0;                   // +0x10
    virtual void log(const char* text, ELOG_LEVEL level) = 0;         // +0x14
    virtual void log(const char* text, const char* hint, ELOG_LEVEL level) = 0;  // +0x18
    virtual void log(const wchar_t* text, ELOG_LEVEL level) = 0;      // +0x20
};

class Printer {
public:
    static ILogger* Logger;  // _ZN6glitch2os7Printer6LoggerE

    static void setLogLevel(ELOG_LEVEL level);                        // 0x60ad80
    static ELOG_LEVEL getLogLevel();                                  // 0x60adc0 (ELL_NONE with no logger)
    static void log(const char* text, ELOG_LEVEL level);              // 0x60aca0
    static void log(const char* text, const char* hint, ELOG_LEVEL level);  // 0x60ace8
    static void log(const wchar_t* text, ELOG_LEVEL level);           // 0x60ad38
};

class Timer {
public:
    // Virtual timer state; initial values come from the original's static
    // initialisation and are not recovered.
    static float VirtualTimerSpeed;            // _ZN6glitch2os5Timer17VirtualTimerSpeedE
    static int VirtualTimerStopCounter;        // _ZN6glitch2os5Timer23VirtualTimerStopCounterE

    static float getSpeed();                   // 0x60ae9c
    static bool isStopped();                   // 0x60aebc
};

}  // namespace glitch::os
