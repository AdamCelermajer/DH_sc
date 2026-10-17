#pragma once

// Host-only compatibility for the pinned Box2D 2.0.1 headers when compiled
// with MinGW. Android/NDK builds do not use this header: older MinGW libstdc++
// does not expose the POSIX finite() spelling used by this upstream release.
#include <cmath>
#include <cstring>

#if defined(__MINGW32__) && !defined(finite)
#define finite(value) (std::isfinite(value))
#endif
