// ADevice: the Android device-information class. Most of its methods call the Java
// side through JNI and are not built for the Windows target (see the ledger); the
// Windows build reads device facts through Device (device.hpp).
#pragma once

#include <cstdint>

class ADevice {
public:
    // 0x52eea8: milliseconds since the epoch in the low 32 bits (usec / 1000 plus
    // 1000 * sec, 32-bit arithmetic as in the listing).
    static int currentTime();
};
