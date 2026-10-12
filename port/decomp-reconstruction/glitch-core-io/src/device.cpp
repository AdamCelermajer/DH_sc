#include "glitch-core-io/os/device.hpp"

#include <cstring>

namespace glitch {

// 0x672040 _ZN6glitch7IDeviceC1ERKNS_19SCreationParametersE (C2 at 0x67228c is the same body)
// PARTIAL: the original also copies the receiver from the parameters block (+0x28, a 32-bit
// pointer) into the device. That read is not modelled here, so the receiver starts null.
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

}  // namespace glitch
