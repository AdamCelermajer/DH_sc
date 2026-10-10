#include "procedural_asset_source_v1.hpp"
#include <algorithm>

namespace dh2::loader {

bool ProceduralAssetSourceV1::key(const std::string& input, std::string& out, std::string& error) {
    std::string next = input;
    std::replace(next.begin(), next.end(), '\\', '/');
    while (next.rfind("./", 0) == 0) next.erase(0, 2);
    // Same safe-name rule as ZipAssetPackV1::key: no empty, absolute, drive or parent segments.
    const bool absolute = !next.empty() && (next[0] == '/' || next.find(':') != std::string::npos);
    bool parent = false;
    std::size_t start = 0;
    for (;;) {
        const auto slash = next.find('/', start);
        const auto part = next.substr(start, slash == std::string::npos ? std::string::npos : slash - start);
        if (part == "..") parent = true;
        if (slash == std::string::npos) break;
        start = slash + 1;
    }
    if (next.empty() || absolute || parent || next.find('\0') != std::string::npos) {
        error = "invalid resource URI";
        return false;
    }
    for (auto& c : next) if (c >= 'A' && c <= 'Z') c = char(c - 'A' + 'a');
    out = std::move(next);
    error.clear();
    return true;
}

} // namespace dh2::loader
