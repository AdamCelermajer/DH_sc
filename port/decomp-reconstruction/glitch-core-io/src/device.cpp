#include "glitch-core-io/os/device.hpp"

#include "glitch-core-io/os/logging.hpp"

#include <cstring>
#include <string>

namespace glitch {

// 0x672040 _ZN6glitch7IDeviceC1ERKNS_19SCreationParametersE (C2 at 0x67228c is the same body)
// Re-read from the listing: the original zeroes +0x10..+0x24, stores the vtable and the count 1,
// copies the word at parameters +0x28 into +0x28 (the event receiver - the port's memcpy of the
// 0x58-byte block covers it, which is what the old note asked for), clears the logger at +0x2c,
// builds the CVideoModeList at +0x3c, then allocates and constructs the collada resource manager
// (+0x08), the animation streaming manager (+0x0c) and the timer (+0x20), attaches
// glitch::os::Printer::Logger (adding it a use and passing the receiver to CLogger::setReceiver),
// stores the logger into the Printer global, takes the file system from the CIrrFactory instance
// (+0x34), logs the "Glitch Engine version " + "0.1.0.2" text at ELL_INFORMATION and runs
// checkVersion on the parameters string at +0x3c.
// PARTIAL, missing item: none of those collaborators are declared in the port (collada's
// CResFileManager / CAnimationStreamingManager, glitch::os::Timer, CLogger and CIrrFactory), so the
// constructor only copies the parameters block here.
IDevice::IDevice(const SCreationParameters& parameters) {
    std::memcpy(creationParameters.bytes, parameters.bytes, sizeof(creationParameters.bytes));
}

// 0x67142c _ZN6glitch7IDevice15isExitRequestedEv
bool IDevice::isExitRequested() {
    return false;
}

// 0x671438 _ZN6glitch7IDevice16setScreenCaptureEb
void IDevice::setScreenCapture(bool /*enable*/) {}

// 0x67143c _ZN6glitch7IDevice16getVideoModeListEv
video::CVideoModeList* IDevice::getVideoModeList() {
    return &videoModeList;
}

// 0x671494 _ZNK6glitch7IDevice12isFullscreenEv
// The flag is byte 0x0e of the creation parameters (original +0x6a).
bool IDevice::isFullscreen() const {
    return creationParameters.bytes[0x0e] != 0;
}

// 0x67149c _ZNK6glitch7IDevice14getColorFormatEv
int IDevice::getColorFormat() const {
    return 5;
}

// 0x6714c8 _ZN6glitch7IDevice16setEventReceiverEPNS_14IEventReceiverE
// Re-read from the listing: receiver stored at +0x28; then, when the logger at +0x2c is non-null,
// CLogger::setReceiver (0x6a093c) is called with it; then, when the pointer at +0x18 is non-null,
// the virtual at that object's vtable +0x34 is called with the receiver.
// PARTIAL, missing item: glitch::CLogger is not declared in the port (the +0x2c member is a void*)
// and the type of the object at +0x18 is not recovered (the header keeps +0x18 as void*), so neither
// call can be written. The store is all that can be reproduced.
void IDevice::setEventReceiver(IEventReceiver* receiver) {
    eventReceiver = receiver;
}

// 0x671e84 _ZN6glitch7IDevice12checkVersionEPKc
// The library version is "0.1.0.2". A mismatch logs one warning line (the original builds the
// message through std::string appends and logs it through Printer::log at warning level).
bool IDevice::checkVersion(const char* compiledVersion) {
    static const char kLibraryVersion[] = "0.1.0.2";
    if (std::strcmp(kLibraryVersion, compiledVersion) == 0) return true;
    std::string message = "Warning: The library version of the Glitch Engine (";
    message += kLibraryVersion;
    message += ") does not match the version the application was compiled with (";
    message += compiledVersion;
    message += "). This may cause problems.";
    os::Printer::log(message.c_str(), os::ELL_WARNING);
    return false;
}

}  // namespace glitch
