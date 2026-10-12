#include "core-platform/config_file.hpp"

// 0x89bee0 ConfigFile::Encode(char const*, char const*, char const*, char*)
// The original returns 0 without writing the output.
int ConfigFile::Encode(const char*, const char*, const char*, char*) { return 0; }

// 0x89bee8 ConfigFile::Seed()
int ConfigFile::Seed() { return 0; }
