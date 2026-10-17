#include "mod_assets.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>

namespace dh2::mods {
bool valid_path(const std::string& value) {
    if (value.empty() || value.front() == '/' || value.back() == '/') return false;
    if (value.find_first_of("\\:\0", 0, 3) != std::string::npos) return false;
    std::size_t start = 0;
    while (start < value.size()) {
        const auto end = value.find('/', start);
        const auto part = value.substr(start, end == std::string::npos ? end : end-start);
        if (part.empty() || part == "." || part == "..") return false;
        if (end == std::string::npos) break;
        start = end + 1;
    }
    return true;
}
Lookup read(const std::string& root, const std::string& relative,
            std::vector<std::uint8_t>& bytes, std::string& error) {
    namespace fs = std::filesystem;
    error.clear();
    if (!valid_path(relative)) { error = "Invalid mod asset path"; return Lookup::rejected; }
    if (root.empty()) return Lookup::absent;
    std::error_code ec;
    const auto base = fs::weakly_canonical(root, ec);
    if (ec) { error = "Cannot resolve mod directory"; return Lookup::rejected; }
    const auto requested = fs::weakly_canonical(base / relative, ec);
    if (ec) { error = "Cannot resolve mod asset: " + relative; return Lookup::rejected; }
    // Component comparison prevents a symlink from escaping the mod directory.
    auto candidate = requested.begin();
    for (const auto& component : base) {
        if (candidate == requested.end() || *candidate++ != component) {
            error = "Mod asset escapes directory: " + relative; return Lookup::rejected;
        }
    }
    const auto status = fs::status(requested, ec);
    if (ec == std::errc::no_such_file_or_directory || (!ec && !fs::exists(status))) return Lookup::absent;
    if (ec || !fs::is_regular_file(status)) { error = "Mod asset is not a readable file: " + relative; return Lookup::rejected; }
    const auto size = fs::file_size(requested, ec);
    if (ec || size == 0 || size > 32*1024*1024) { error = "Mod asset size rejected: " + relative; return Lookup::rejected; }
    std::ifstream input(requested, std::ios::binary);
    std::vector<std::uint8_t> loaded(static_cast<std::size_t>(size));
    if (!input.read(reinterpret_cast<char*>(loaded.data()), loaded.size()) || input.peek() != std::ifstream::traits_type::eof()) {
        error = "Mod asset changed or could not be read: " + relative; return Lookup::rejected;
    }
    bytes = std::move(loaded);
    return Lookup::loaded;
}
}
