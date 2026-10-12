// glitch::IDevice: the device interface. Only the base members the queued functions use are
// declared. Layout from the original offsets (kept as comments):
//   +0x28 event receiver, +0x2c logger (pointer, non-null when a logger is attached),
//   +0x3c video mode list, +0x5c creation parameters (0x58 bytes, copied by the constructor).
// The creation parameters' fullscreen flag is byte 0x0e of that block (original +0x6a).
// The window-offset fields read by postMouseEventFromUser are not modelled.
#pragma once

#include <cstdint>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch {

// Creation parameters, kept as the 0x58-byte block the constructor copies.
struct SCreationParameters {
    unsigned char bytes[0x58];
};

class IDevice : public IReferenceCounted {
public:
    // 0x672040 (C1), 0x67228c (C2): copies the parameters and clears the members.
    explicit IDevice(const SCreationParameters& parameters);

    // 0x67142c: the base device never requests exit.
    virtual bool isExitRequested();
    // 0x671438: no-op in the base device.
    virtual void setScreenCapture(bool enable);
    // 0x67143c: the video mode list (original +0x3c).
    virtual video::CVideoModeList* getVideoModeList();
    // 0x671494: the fullscreen flag from the creation parameters (original +0x6a).
    virtual bool isFullscreen() const;
    // 0x67149c: the colour format the base device reports (constant 5).
    virtual int getColorFormat() const;
    // 0x6714c8: stores the receiver (+0x28), passes it to the logger when one is attached and
    // to the object at +0x18 (vtable +0x34, not reconstructed).
    // PARTIAL: the +0x18 object is not modelled; see src/device.cpp.
    virtual void setEventReceiver(IEventReceiver* receiver);
    // 0x671e84: true when the application's compiled version string equals the library version
    // (0.1.0.2). Otherwise it logs a warning at ELL_WARNING and returns false.
    bool checkVersion(const char* compiledVersion);

    // +0x28
    IEventReceiver* eventReceiver = nullptr;
    // +0x2c: logger, null when none is attached (CLogger is not reconstructed)
    void* logger = nullptr;
    // +0x3c
    video::CVideoModeList videoModeList;
    // +0x5c
    SCreationParameters creationParameters;
};

}  // namespace glitch
