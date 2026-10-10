#include "android/asset_manager.h"

#include <algorithm>
#include <limits>
#include <system_error>
#include <vector>

namespace {
bool resolve_case_sensitive(AAssetManager& manager,
                            const char* requested,
                            std::filesystem::path& resolved) {
    if (!requested || !*requested) return false;
    const std::string name(requested);
    if (name.find('\0') != std::string::npos || name.find('\\') != std::string::npos ||
        name.find(':') != std::string::npos)
        return false;

    const auto relative = std::filesystem::u8path(name);
    if (relative.is_absolute() || relative.has_root_name() || relative.has_root_directory())
        return false;

    auto current = manager.root;
    for (const auto& component_path : relative) {
        const auto component = component_path.u8string();
        if (component.empty() || component == "." || component == "..") return false;
        std::filesystem::path next;
        {
            const auto key = current.u8string();
            std::lock_guard<std::mutex> lock(manager.cache_mutex);
            auto cached = manager.directory_entries.find(key);
            if (cached == manager.directory_entries.end()) {
                std::unordered_map<std::string, std::filesystem::path> entries;
                std::error_code ec;
                for (std::filesystem::directory_iterator it(current, ec), end; !ec && it != end; it.increment(ec))
                    entries.emplace(it->path().filename().u8string(), it->path());
                if (ec) return false;
                cached = manager.directory_entries.emplace(key, std::move(entries)).first;
            }
            const auto match = cached->second.find(component);
            if (match == cached->second.end()) return false;
            next = match->second;
        }
        current = std::move(next);
    }

    std::error_code ec;
    const auto canonical_root = std::filesystem::weakly_canonical(manager.root, ec);
    if (ec) return false;
    const auto canonical_file = std::filesystem::weakly_canonical(current, ec);
    if (ec) return false;
    auto root_part = canonical_root.begin();
    auto file_part = canonical_file.begin();
    for (; root_part != canonical_root.end(); ++root_part, ++file_part) {
        if (file_part == canonical_file.end() || *root_part != *file_part) return false;
    }
    resolved = canonical_file;
    return true;
}
}

AAssetManager* AAssetManager_createFromFilesystem(const char* root) {
    if (!root || !*root) return nullptr;
    std::error_code ec;
    auto canonical = std::filesystem::weakly_canonical(std::filesystem::u8path(root), ec);
    if (ec || !std::filesystem::is_directory(canonical, ec) || ec) return nullptr;
    auto* manager = new AAssetManager;
    manager->root = std::move(canonical);
    return manager;
}

void AAssetManager_delete(AAssetManager* manager) { delete manager; }

AAsset* AAssetManager_open(AAssetManager* manager, const char* filename, int) {
    if (!manager) return nullptr;
    std::filesystem::path path;
    if (!resolve_case_sensitive(*manager, filename, path)) return nullptr;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec) || ec) return nullptr;
    const auto length = std::filesystem::file_size(path, ec);
    if (ec || length > static_cast<std::uintmax_t>(std::numeric_limits<std::int64_t>::max()))
        return nullptr;

    auto asset = std::make_unique<AAsset>();
    asset->stream.open(path, std::ios::binary);
    if (!asset->stream) return nullptr;
    asset->length = static_cast<std::int64_t>(length);
    return asset.release();
}

std::int64_t AAsset_getLength64(AAsset* asset) {
    return asset ? asset->length : -1;
}

int AAsset_read(AAsset* asset, void* buffer, std::size_t count) {
    if (!asset || !buffer || count == 0) return 0;
    const auto bounded = static_cast<std::streamsize>(
        std::min<std::size_t>(count, static_cast<std::size_t>(std::numeric_limits<int>::max())));
    asset->stream.read(static_cast<char*>(buffer), bounded);
    return static_cast<int>(asset->stream.gcount());
}

void AAsset_close(AAsset* asset) { delete asset; }
