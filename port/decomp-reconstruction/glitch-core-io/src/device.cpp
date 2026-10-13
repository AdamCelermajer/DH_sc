#include "glitch-core-io/os/device.hpp"

#include "glitch-core-io/os/logging.hpp"

#include <cstring>
#include <string>

namespace glitch {

// 0x672040 _ZN6glitch7IDeviceC1ERKNS_19SCreationParametersE (C2 at 0x67228c is the same body)
// PARTIAL: the original also copies the receiver from the parameters block (+0x28, a 32-bit
// pointer) into the device. That read is not modelled here, so the receiver starts null. It also
// builds the collada resource manager (0x2c bytes), the animation streaming manager (0x20 bytes),
// the os timer (8 bytes), joins the Printer logger, takes the CIrrFactory object and runs
// checkVersion on the version string at parameters +0x3c. None of these are declared in the port.
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
// PARTIAL: the receiver is stored. The logger call (CLogger::setReceiver, when a logger is
// attached) and the virtual call on the object at +0x18 (vtable +0x34) are not modelled.
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
