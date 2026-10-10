#include "asset_catalog.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace dh::foundation {
namespace {
namespace fs = std::filesystem;

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

bool within(const fs::path& root, const fs::path& candidate) {
    auto r = root.begin();
    auto c = candidate.begin();
    for (; r != root.end(); ++r, ++c) {
        if (c == candidate.end()) return false;
#ifdef _WIN32
        if (lower(r->generic_string()) != lower(c->generic_string())) return false;
#else
        if (*r != *c) return false;
#endif
    }
    return true;
}

bool looks_like_assets(const fs::path& path) {
    std::error_code ec;
    if (!fs::is_directory(path, ec)) return false;
    for (const char* folder : {"models", "worlds", "animations", "data"}) {
        if (fs::is_directory(path / folder, ec)) return true;
        ec.clear();
    }
    return false;
}
} // namespace

AssetCatalog::AssetCatalog(fs::path root) {
    std::error_code ec;
    root_ = fs::canonical(root, ec);
    if (ec || !fs::is_directory(root_))
        throw std::runtime_error("Asset root is not an existing directory: " + root.string());
}

fs::path AssetCatalog::discover_root(fs::path start) {
    std::error_code ec;
    start = fs::absolute(start, ec);
    if (ec) throw std::runtime_error("Cannot resolve asset search directory");
    if (fs::is_regular_file(start, ec)) start = start.parent_path();
    start = start.lexically_normal();
    for (;;) {
        for (const fs::path& candidate : {
                 start,
                 start / "assets",
                 start / "port/android-native/app/src/main/assets"}) {
            if (looks_like_assets(candidate)) return fs::canonical(candidate);
        }
        const auto parent = start.parent_path();
        if (parent.empty() || parent == start) break;
        start = parent;
    }
    throw std::runtime_error("No extracted asset folder found; supply --assets PATH");
}

fs::path AssetCatalog::resolve(const fs::path& relative) const {
    if (relative.empty() || relative.has_root_path())
        throw std::invalid_argument("Asset path must be a nonempty relative path");
    for (const auto& component : relative) {
        if (component == "..") throw std::invalid_argument("Asset path cannot traverse parents");
    }
    std::error_code ec;
    const auto path = fs::canonical(root_ / relative, ec);
    if (ec) throw std::runtime_error("Asset not found: " + relative.generic_string());
    if (!within(root_, path)) throw std::invalid_argument("Asset path escapes asset root");
    if (!fs::is_regular_file(path))
        throw std::runtime_error("Asset is not a regular file: " + relative.generic_string());
    return path;
}

std::vector<std::uint8_t> AssetCatalog::read(const fs::path& relative) const {
    const auto path = resolve(relative);
    const auto count = fs::file_size(path);
    if (count > std::numeric_limits<std::size_t>::max() ||
        count > static_cast<std::uintmax_t>(std::numeric_limits<std::streamsize>::max()))
        throw std::runtime_error("Asset is too large to read: " + relative.generic_string());
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("Cannot open asset: " + relative.generic_string());
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(count));
    if (count && !stream.read(reinterpret_cast<char*>(bytes.data()),
                              static_cast<std::streamsize>(count)))
        throw std::runtime_error("Cannot read complete asset: " + relative.generic_string());
    return bytes;
}

std::vector<AssetEntry> AssetCatalog::inventory() const {
    std::vector<AssetEntry> entries;
    for (const auto& entry : fs::recursive_directory_iterator(root_)) {
        if (!entry.is_regular_file()) continue;
        const auto canonical = fs::canonical(entry.path());
        if (!within(root_, canonical)) continue;
        entries.push_back({entry.path().lexically_relative(root_), entry.file_size()});
    }
    std::sort(entries.begin(), entries.end(), [](const AssetEntry& a, const AssetEntry& b) {
        return a.relative_path.generic_string() < b.relative_path.generic_string();
    });
    return entries;
}

std::vector<AssetEntry> AssetCatalog::find(const std::string& name) const {
    auto entries = inventory();
    const auto needle = lower(name);
    entries.erase(std::remove_if(entries.begin(), entries.end(), [&](const AssetEntry& entry) {
        return lower(entry.relative_path.generic_string()).find(needle) == std::string::npos;
    }), entries.end());
    return entries;
}

std::vector<AssetEntry> AssetCatalog::original_levels() const {
    auto entries = inventory();
    entries.erase(std::remove_if(entries.begin(), entries.end(), [](const AssetEntry& entry) {
        const auto path = lower(entry.relative_path.generic_string());
        const auto ext = lower(entry.relative_path.extension().string());
        return path.rfind("worlds/", 0) != 0 && ext != ".dwld" && ext != ".dact";
    }), entries.end());
    return entries;
}

std::vector<AssetEntry> AssetCatalog::prince_resources() const {
    auto entries = inventory();
    entries.erase(std::remove_if(entries.begin(), entries.end(), [](const AssetEntry& entry) {
        const auto name = lower(entry.relative_path.filename().string());
        return name.find("prince") == std::string::npos && name.rfind("mc_", 0) != 0;
    }), entries.end());
    return entries;
}
} // namespace dh::foundation
