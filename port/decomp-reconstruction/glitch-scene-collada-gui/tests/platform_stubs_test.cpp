// Checks for the platform stand-ins (logger, OS operator, Android device stubs) and
// the particle-system heap helpers.
#include "glitch-scene-collada-gui/platform_stubs.hpp"
#include "glitch-scene-collada-gui/ps_types.hpp"

#include <cstdio>

using namespace glitch;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    {
        IObject object;
        (void)object;
        CLogger logger;
        CHECK(logger.getLogLevel() == ELL_DEBUG);
        logger.setLogLevel(ELL_WARNING);
        CHECK(logger.getLogLevel() == ELL_WARNING);
        int receiverTag = 0;
        logger.setReceiver(reinterpret_cast<IEventReceiver*>(&receiverTag));
        CHECK(logger.receiver == reinterpret_cast<IEventReceiver*>(&receiverTag));
        ILogger base;
        (void)base;
        IOSOperator osBase;
        (void)osBase;
    }

    {
        COSOperator os;
        CHECK(os.getOperationSystemVersion() == nullptr);
        static const wchar_t kVersion[] = L"test";
        os.osVersion = kVersion;
        CHECK(os.getOperationSystemVersion() == kVersion);
        CHECK(os.getTextFromClipboard() == nullptr);
        std::uint32_t mhz = 7, total = 1, avail = 2;
        CHECK(!os.getProcessorSpeedMHz(&mhz));
        CHECK(!os.getSystemMemory(&total, &avail));
        os.copyToClipboard("ignored");
    }

    {
        CAndroidOSDevice device;
        CHECK(device.createWindow());
        CHECK(device.isWindowActive() && device.isWindowFocused());
        CHECK(!device.isWindowMinimized());
        CHECK(device.getVideoModeList() == nullptr);
        CAndroidOSDevice::CCursorControl cursor;
        CHECK(!cursor.isVisible());
        cursor.visible = true;
        CHECK(cursor.isVisible());
    }

    {
        void* block = glitch::ps::PS_NEW(64);
        CHECK(block != nullptr);
        glitch::ps::PS_FREE(block);
    }

    std::printf("%s\n", failures ? "platform_stubs: FAILED" : "platform_stubs: ok");
    return failures != 0;
}
