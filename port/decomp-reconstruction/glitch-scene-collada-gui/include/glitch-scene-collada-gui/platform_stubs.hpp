// glitch base object, logger, OS operator and Android device stand-ins. The bodies
// are the original's stubs (empty, constant returns) and single-field accessors. The
// Windows port uses its own device, so these are kept for the record of the queue.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch {

struct SEvent;
class IEventReceiver;
class IDevice;

namespace video {
class IVideoModeList;
}

class IObject {
public:
    // 0x6a118c _ZN6glitch7IObjectC2Ev (empty)
    // 0x6a1190 _ZN6glitch7IObjectC1Ev (empty)
    IObject() = default;
    // 0x6a1194 _ZN6glitch7IObjectD2Ev (empty)
    // 0x6a1198 _ZN6glitch7IObjectD1Ev (empty)
    ~IObject() = default;
};

// Log levels in the order of upstream ILogger.h.
enum ELOG_LEVEL : std::uint32_t {
    ELL_DEBUG = 0,
    ELL_INFORMATION,
    ELL_WARNING,
    ELL_ERROR,
    ELL_NONE,
};

class ILogger {
public:
    // 0x6a08b0 _ZN6glitch7ILoggerD1Ev (empty D1)
    ~ILogger() = default;
};

class CLogger {
public:
    // 0x6a092c _ZNK6glitch7CLogger11getLogLevelEv
    ELOG_LEVEL getLogLevel() const;
    // 0x6a0934 _ZN6glitch7CLogger11setLogLevelENS_10ELOG_LEVELE
    void setLogLevel(ELOG_LEVEL level);
    // 0x6a093c _ZN6glitch7CLogger11setReceiverEPNS_14IEventReceiverE
    void setReceiver(IEventReceiver* receiver);
    // 0x6a0944 _ZN6glitch7CLoggerD1Ev (empty D1)
    ~CLogger() = default;

    // The base words at +0x00..+0x07 are not mapped.
    ELOG_LEVEL level{};                  // +0x08
    IEventReceiver* receiver = nullptr;  // +0x0c
};

namespace os {

// glitch::os::Printer as the Android device calls it. The original's log takes the
// glitch::ELOG_LEVEL enum above (mangled NS_10ELOG_LEVEL). Only the two-argument
// overload is declared here.
class Printer {
public:
    // 0x60aca0 _ZN6glitch2os7Printer3logEPKcNS_10ELOG_LEVELE
    static void log(const char* text, ELOG_LEVEL level);
};

}  // namespace os

class IOSOperator {
public:
    // 0x6a0c7c _ZN6glitch11IOSOperatorD1Ev (empty D1)
    ~IOSOperator() = default;
};

class COSOperator {
public:
    // 0x6a0c80 _ZNK6glitch11COSOperator25getOperationSystemVersionEv
    // The deprecated upstream signature returns a wide string.
    const wchar_t* getOperationSystemVersion() const;
    // 0x6a0c88 _ZNK6glitch11COSOperator15copyToClipboardEPKc (empty)
    void copyToClipboard(const char*) const;
    // 0x6a0c8c _ZNK6glitch11COSOperator20getTextFromClipboardEv (null)
    const char* getTextFromClipboard() const;
    // 0x6a0c94 _ZNK6glitch11COSOperator20getProcessorSpeedMHzEPj (returns false)
    bool getProcessorSpeedMHz(std::uint32_t* mhz) const;
    // 0x6a0c9c _ZNK6glitch11COSOperator15getSystemMemoryEPjS1_ (returns false)
    bool getSystemMemory(std::uint32_t* totalBytes, std::uint32_t* availableBytes) const;

    const wchar_t* osVersion = nullptr;  // +0x4c
};

// The Android device class. Its cursor control is a nested class with the visible
// flag at +0x34.
class CAndroidOSDevice {
public:
    class CCursorControl {
    public:
        // 0x6a00a0 _ZNK6glitch16CAndroidOSDevice14CCursorControl9isVisibleEv
        bool isVisible() const;
        // 0x6a0334 _ZN6glitch16CAndroidOSDevice14CCursorControl10setVisibleEb
        // Stores the flag, then passes it to the owning device (+0x30).
        void setVisible(bool value);
        // 0x6a0164 _ZN6glitch16CAndroidOSDevice14CCursorControl11getPositionEv
        // The two position words are copied out unchanged; int elements are assumed.
        core::position2d<std::int32_t> getPosition() const;
        // 0x6a0350 _ZN6glitch16CAndroidOSDevice14CCursorControlD1Ev (empty D1)
        ~CCursorControl() = default;

        std::uint8_t pad_0000[0x08] = {};   // +0x00..+0x07 (not mapped)
        std::int32_t positionX = 0;         // +0x08
        std::int32_t positionY = 0;         // +0x0c
        std::uint8_t pad_0010[0x20] = {};   // +0x10..+0x2f (not mapped)
        CAndroidOSDevice* device = nullptr;  // +0x30 (owning device)
        bool visible = false;                // +0x34
    };

    // 0x6a02a4 _ZN6glitch16CAndroidOSDevice11closeDeviceEv
    // Clears the running flag at +0x109.
    void closeDevice();
    // 0x6a02b0 _ZN6glitch16CAndroidOSDevice12createWindowEv (returns true)
    bool createWindow();
    // 0x6a02b8 _ZN6glitch16CAndroidOSDevice9setResizeEii (empty)
    void setResize(int width, int height);
    // 0x6a02bc _ZN6glitch16CAndroidOSDevice5flushEv (empty)
    void flush();
    // 0x6a02c0 _ZN6glitch16CAndroidOSDevice5yieldEv (empty)
    void yield();
    // 0x6a02c4 _ZN6glitch16CAndroidOSDevice16setWindowCaptionEPKw (empty)
    void setWindowCaption(const wchar_t*);
    // 0x6a02c8 _ZNK6glitch16CAndroidOSDevice14isWindowActiveEv (returns true)
    bool isWindowActive() const;
    // 0x6a02d0 _ZNK6glitch16CAndroidOSDevice15isWindowFocusedEv (returns true)
    bool isWindowFocused() const;
    // 0x6a02d8 _ZNK6glitch16CAndroidOSDevice17isWindowMinimizedEv (returns false)
    bool isWindowMinimized() const;
    // 0x6a02e0 _ZN6glitch16CAndroidOSDevice12postKeyEventEPvRNS_6SEventEb (empty)
    void postKeyEvent(void*, SEvent&, bool);
    // 0x6a02e4 _ZN6glitch16CAndroidOSDevice14postMouseEventEPvRNS_6SEventE (empty)
    void postMouseEvent(void*, SEvent&);
    // 0x6a02e8 _ZN6glitch16CAndroidOSDevice18storeMouseLocationEv (empty)
    void storeMouseLocation();
    // 0x6a02ec _ZN6glitch16CAndroidOSDevice16setMouseLocationEii (empty)
    void setMouseLocation(int x, int y);
    // 0x6a0330 _ZN6glitch16CAndroidOSDevice16setCursorVisibleEb (empty)
    void setCursorVisible(bool visible);
    // 0x6a0340 _ZN6glitch16CAndroidOSDevice12initKeycodesEv (empty)
    void initKeycodes();
    // 0x6a0344 _ZN6glitch16CAndroidOSDevice13setResizeAbleEb (empty)
    void setResizeAble(bool);
    // 0x6a0348 _ZN6glitch16CAndroidOSDevice16getVideoModeListEv (null)
    video::IVideoModeList* getVideoModeList();
    // 0x6a0454 _ZN6glitch16CAndroidOSDevice12createDriverEv
    // Picks the video driver from the type word at +0x5c: 1 OpenGL ES 2, 0 null driver,
    // 128 and 256 are not available on this platform. The driver is stored at +0x10 and
    // returned. On failure the error is logged and nullptr is returned.
    // The IDevice argument of the pseudocode is not read; see the source for the open points.
    void* createDriver();

    std::uint8_t pad_0000[0x10] = {};   // +0x00..+0x0f (not mapped)
    void* unk_0010 = nullptr;           // +0x10 video driver pointer (driver type not mapped)
    std::uint8_t pad_0018[0x44] = {};   // +0x18..+0x5b (not mapped)
    std::int32_t unk_005c = 0;          // +0x5c driver type selector (1 OpenGL ES 2, 0 null)
    std::uint8_t pad_0060[0xa9] = {};   // +0x60..+0x108 (not mapped)
    bool running = false;               // +0x109 (cleared by closeDevice; initial value not mapped)
};

}  // namespace glitch
