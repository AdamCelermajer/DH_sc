// Stand-in for glitch::os::ELOG_LEVEL and glitch::os::Printer::log (glitch-core-io/os/logging.hpp).
// The enum values and the log signature match that header; the definitions live in glitch-core-io
// (Printer::log at 0x60aca0). Used by glitch-video code that reports through the os logger.
#pragma once

namespace glitch::os {

enum ELOG_LEVEL { ELL_DEBUG = 0, ELL_INFORMATION = 1, ELL_WARNING = 2, ELL_ERROR = 3, ELL_NONE = 4 };

class Printer {
public:
    static void log(const char* text, ELOG_LEVEL level);  // 0x60aca0 (glitch-core-io)
    // printf-style forms. Defined in glitch-core-io (0x60afd4 / 0x60b034, partial there).
    static void logf(ELOG_LEVEL level, const char* format, ...);
};

}  // namespace glitch::os
