#include "path_case.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <system_error>

namespace dh::foundation {
namespace {
namespace fs = std::filesystem;

#ifndef _WIN32
std::string folded(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

struct Listing {
    fs::file_time_type stamp{};
    bool stamped = false;
    std::map<std::string, std::string> byFoldedName; // folded name -> first spelling (sorted, so deterministic)
};

std::shared_ptr<const Listing> listing_for(const fs::path& directory) {
    static std::mutex mutex;
    static std::map<std::string, std::shared_ptr<const Listing>> cache;
    std::error_code ec;
    const auto stamp = fs::last_write_time(directory, ec);
    const bool stamped = !ec;
    const std::string key = directory.string();
    std::lock_guard<std::mutex> lock(mutex);
    const auto found = cache.find(key);
    if (found != cache.end() && found->second->stamped == stamped && (!stamped || found->second->stamp == stamp))
        return found->second;
    auto fresh = std::make_shared<Listing>();
    fresh->stamp = stamp;
    fresh->stamped = stamped;
    std::vector<std::string> names;
    ec.clear();
    for (const auto& entry : fs::directory_iterator(directory, ec)) names.push_back(entry.path().filename().string());
    std::sort(names.begin(), names.end());
    for (const auto& name : names) fresh->byFoldedName.emplace(folded(name), name);
    cache[key] = fresh;
    return fresh;
}

// Resolves `relative` below `base`; `base` is kept exactly. Empty when a component has no match.
fs::path match_below(const fs::path& base, const fs::path& relative) {
    fs::path current = base;
    for (const auto& component : relative) {
        if (component.empty() || component == ".") continue;
        std::error_code ec;
        const fs::path exact = current / component;
        if (fs::exists(fs::symlink_status(exact, ec))) { current = exact; continue; }
        if (!fs::is_directory(current, ec)) return {};
        const auto listing = listing_for(current);
        const auto hit = listing->byFoldedName.find(folded(component.string()));
        if (hit == listing->byFoldedName.end()) return {};
        current /= hit->second;
    }
    return current;
}
#endif
} // namespace

fs::path find_path_ignoring_case(const fs::path& root, const fs::path& relative) {
#ifdef _WIN32
    std::error_code ec;
    const fs::path path = root / relative;
    return fs::exists(path, ec) ? path : fs::path();
#else
    return match_below(root, relative);
#endif
}

fs::path existing_path_ignoring_case(const fs::path& path) {
#ifdef _WIN32
    return path;
#else
    if (path.empty()) return path;
    std::error_code ec;
    if (fs::exists(fs::symlink_status(path, ec))) return path;
    // Keep the longest exact prefix, then match the rest component by component.
    fs::path prefix;
    auto it = path.begin();
    for (; it != path.end(); ++it) {
        const fs::path next = prefix.empty() && !path.has_root_path() ? *it : prefix / *it;
        if (it->empty() || !fs::exists(fs::symlink_status(next, ec))) break;
        prefix = next;
    }
    fs::path rest;
    for (; it != path.end(); ++it) rest /= *it;
    if (prefix.empty()) prefix = path.has_root_path() ? path.root_path() : fs::path(".");
    const auto matched = match_below(prefix, rest);
    if (matched.empty()) return path;
    return prefix == fs::path(".") && !path.has_root_path() ? matched.lexically_normal() : matched;
#endif
}

} // namespace dh::foundation
