#include "android/asset_manager.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    const auto root = fs::temp_directory_path() / "dh2-desktop-assets-test";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "worlds");
    {
        std::ofstream file(root / "worlds" / "Crypt.dwld", std::ios::binary);
        file.write("DH2\0asset", 9);
    }

    auto* manager = AAssetManager_createFromFilesystem(root.string().c_str());
    if (!manager) return 1;
    auto* asset = AAssetManager_open(manager, "worlds/Crypt.dwld", AASSET_MODE_BUFFER);
    if (!asset || AAsset_getLength64(asset) != 9) return 2;
    std::array<char, 9> bytes{};
    if (AAsset_read(asset, bytes.data(), bytes.size()) != 9 ||
        std::string(bytes.data(), bytes.size()) != std::string("DH2\0asset", 9)) return 3;
    AAsset_close(asset);

    if (AAssetManager_open(manager, "worlds/crypt.dwld", AASSET_MODE_BUFFER) ||
        AAssetManager_open(manager, "../outside", AASSET_MODE_BUFFER) ||
        AAssetManager_open(manager, root.string().c_str(), AASSET_MODE_BUFFER)) return 4;

    AAssetManager_delete(manager);
    fs::remove_all(root, ec);
    std::cout << "asset facade: binary reads, exact-case lookup, traversal rejection PASS\n";
    return 0;
}
