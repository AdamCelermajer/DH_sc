#include "glitch-scene-collada-gui/platform_stubs.hpp"

namespace glitch {

// 0x6a092c _ZNK6glitch7CLogger11getLogLevelEv
ELOG_LEVEL CLogger::getLogLevel() const {
    return level;
}

// 0x6a0934 _ZN6glitch7CLogger11setLogLevelENS_10ELOG_LEVELE
void CLogger::setLogLevel(ELOG_LEVEL value) {
    level = value;
}

// 0x6a093c _ZN6glitch7CLogger11setReceiverEPNS_14IEventReceiverE
void CLogger::setReceiver(IEventReceiver* value) {
    receiver = value;
}

// 0x6a0c80 _ZNK6glitch11COSOperator25getOperationSystemVersionEv
const wchar_t* COSOperator::getOperationSystemVersion() const {
    return osVersion;
}

// 0x6a0c88 _ZNK6glitch11COSOperator15copyToClipboardEPKc
void COSOperator::copyToClipboard(const char*) const {}

// 0x6a0c8c _ZNK6glitch11COSOperator20getTextFromClipboardEv
const char* COSOperator::getTextFromClipboard() const {
    return nullptr;
}

// 0x6a0c94 _ZNK6glitch11COSOperator20getProcessorSpeedMHzEPj
bool COSOperator::getProcessorSpeedMHz(std::uint32_t*) const {
    return false;
}

// 0x6a0c9c _ZNK6glitch11COSOperator15getSystemMemoryEPjS1_
bool COSOperator::getSystemMemory(std::uint32_t*, std::uint32_t*) const {
    return false;
}

// 0x6a00a0 _ZNK6glitch16CAndroidOSDevice14CCursorControl9isVisibleEv
bool CAndroidOSDevice::CCursorControl::isVisible() const {
    return visible;
}

// 0x6a0334 _ZN6glitch16CAndroidOSDevice14CCursorControl10setVisibleEb
// The original has no null check on the owning device; one is added here.
void CAndroidOSDevice::CCursorControl::setVisible(bool value) {
    visible = value;
    if (device != nullptr) {
        device->setCursorVisible(value);
    }
}

// 0x6a0164 _ZN6glitch16CAndroidOSDevice14CCursorControl11getPositionEv
core::position2d<std::int32_t> CAndroidOSDevice::CCursorControl::getPosition() const {
    return core::position2d<std::int32_t>{positionX, positionY};
}

// 0x6a02a4 _ZN6glitch16CAndroidOSDevice11closeDeviceEv
void CAndroidOSDevice::closeDevice() {
    running = false;
}

// 0x6a02b0 _ZN6glitch16CAndroidOSDevice12createWindowEv
bool CAndroidOSDevice::createWindow() {
    return true;
}

// 0x6a02b8 _ZN6glitch16CAndroidOSDevice9setResizeEii
void CAndroidOSDevice::setResize(int, int) {}

// 0x6a02bc _ZN6glitch16CAndroidOSDevice5flushEv
void CAndroidOSDevice::flush() {}

// 0x6a02c0 _ZN6glitch16CAndroidOSDevice5yieldEv
void CAndroidOSDevice::yield() {}

// 0x6a02c4 _ZN6glitch16CAndroidOSDevice16setWindowCaptionEPKw
void CAndroidOSDevice::setWindowCaption(const wchar_t*) {}

// 0x6a02c8 _ZNK6glitch16CAndroidOSDevice14isWindowActiveEv
bool CAndroidOSDevice::isWindowActive() const {
    return true;
}

// 0x6a02d0 _ZNK6glitch16CAndroidOSDevice15isWindowFocusedEv
bool CAndroidOSDevice::isWindowFocused() const {
    return true;
}

// 0x6a02d8 _ZNK6glitch16CAndroidOSDevice17isWindowMinimizedEv
bool CAndroidOSDevice::isWindowMinimized() const {
    return false;
}

// 0x6a02e0 _ZN6glitch16CAndroidOSDevice12postKeyEventEPvRNS_6SEventEb
void CAndroidOSDevice::postKeyEvent(void*, SEvent&, bool) {}

// 0x6a02e4 _ZN6glitch16CAndroidOSDevice14postMouseEventEPvRNS_6SEventE
void CAndroidOSDevice::postMouseEvent(void*, SEvent&) {}

// 0x6a02e8 _ZN6glitch16CAndroidOSDevice18storeMouseLocationEv
void CAndroidOSDevice::storeMouseLocation() {}

// 0x6a02ec _ZN6glitch16CAndroidOSDevice16setMouseLocationEii
void CAndroidOSDevice::setMouseLocation(int, int) {}

// 0x6a0330 _ZN6glitch16CAndroidOSDevice16setCursorVisibleEb
void CAndroidOSDevice::setCursorVisible(bool) {}

// 0x6a0340 _ZN6glitch16CAndroidOSDevice12initKeycodesEv
void CAndroidOSDevice::initKeycodes() {}

// 0x6a0344 _ZN6glitch16CAndroidOSDevice13setResizeAbleEb
void CAndroidOSDevice::setResizeAble(bool) {}

// 0x6a0348 _ZN6glitch16CAndroidOSDevice16getVideoModeListEv
video::IVideoModeList* CAndroidOSDevice::getVideoModeList() {
    return nullptr;
}

}  // namespace glitch
