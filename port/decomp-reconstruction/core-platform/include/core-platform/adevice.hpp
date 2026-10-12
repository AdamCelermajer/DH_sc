// ADevice: the Android device-information class. Most of its methods call the Java
// side through JNI and are not built for the Windows target (see the ledger); the
// Windows build reads device facts through Device (device.hpp).
#pragma once

#include <cstdint>

class ADevice {
public:
    // 0x52eea8: milliseconds since the epoch (partial: the original's high word is
    // the compiler's division scratch and is not reproduced).
    static std::int64_t currentTime();
};
