#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::mods {
enum class Lookup { absent, loaded, rejected };
// Reads an optional override below an app-owned root. Existing invalid files
// fail explicitly; they never silently fall back to a different asset.
Lookup read(const std::string& root, const std::string& relative,
            std::vector<std::uint8_t>& bytes, std::string& error);
bool valid_path(const std::string& relative);
}
