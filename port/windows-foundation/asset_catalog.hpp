#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
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

    // P17 PROCGEN: in-memory document served under a relative path (generated
    // procedural levels). It shadows a file of the same path, is never written
    // to disk, and is returned by resolve() and read() like a real file.
    void set_virtual_file(const std::filesystem::path& relative, const std::vector<std::uint8_t>& bytes);
    bool has_virtual_file(const std::filesystem::path& relative) const;

private:
    std::filesystem::path root_;
    std::map<std::string, std::vector<std::uint8_t>> virtual_files_;
};

} // namespace dh::foundation
