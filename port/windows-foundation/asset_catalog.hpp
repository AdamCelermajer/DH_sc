#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace dh::foundation {

struct AssetEntry {
    std::filesystem::path relative_path;
    std::uintmax_t bytes = 0;
};

// Reads extracted original content. ZIP extraction and game-format decoding
// belong outside this small filesystem boundary.
class AssetCatalog {
public:
    explicit AssetCatalog(std::filesystem::path root);
    static std::filesystem::path discover_root(
        std::filesystem::path start = std::filesystem::current_path());

    const std::filesystem::path& root() const noexcept { return root_; }
    std::filesystem::path resolve(const std::filesystem::path& relative) const;
    std::vector<std::uint8_t> read(const std::filesystem::path& relative) const;
    std::vector<AssetEntry> inventory() const;
    std::vector<AssetEntry> find(const std::string& name) const;
    std::vector<AssetEntry> original_levels() const;
    std::vector<AssetEntry> prince_resources() const;

private:
    std::filesystem::path root_;
};

} // namespace dh::foundation
