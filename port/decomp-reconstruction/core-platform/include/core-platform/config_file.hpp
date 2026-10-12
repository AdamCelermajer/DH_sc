// ConfigFile: encrypted configuration reader. Only the trivial methods of the
// original are reconstructed; Decode and XOR are not (see NOTES.md).
#pragma once

class ConfigFile {
public:
    ConfigFile() = default;   // 0x89bed0 / 0x89bed4
    ~ConfigFile() = default;  // 0x89bed8 / 0x89bedc (compiler-generated)

    int Encode(const char* key, const char* name, const char* value, char* out);  // 0x89bee0
    int Seed();                                                                    // 0x89bee8
};
