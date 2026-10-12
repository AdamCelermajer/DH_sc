// Stand-in for ConfigFile as ALicenseCheck uses it (Decode and the mParams table).
// The real class lives in core-platform, whose header only has Encode and Seed.
// Decode is not reconstructed, and mParams holds the three 255-byte response
// codes that the original fills at run time. Here they stay zeroed.
#pragma once

class ConfigFile {
public:
    ConfigFile() = default;   // 0x89bed0 (core-platform owns the real ctor)
    ~ConfigFile() = default;  // 0x89bed8

    // ConfigFile::Decode(this, buffer, key, pid, server). Not reconstructed.
    void Decode(const char* /*buffer*/, char* /*key*/, char* /*pid*/, char* /*server*/) {}

    // ConfigFile::mParams: three 255-byte slots, filled at run time by the original.
    static inline char mParams[765] = {};
};
