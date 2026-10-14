// Shared case-insensitive game-data resolver (path_case.hpp): authored spelling vs shipped spelling on a case-sensitive
// file system. On Windows the same assertions hold because the file system already folds case.
#include "../asset_catalog.hpp"
#include "../path_case.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;
using dh::foundation::AssetCatalog;
using dh::foundation::existing_path_ignoring_case;
using dh::foundation::find_path_ignoring_case;

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}
void write(const fs::path& path, const std::string& text) {
    fs::create_directories(path.parent_path());
    std::ofstream(path, std::ios::binary) << text;
}
} // namespace

int main() {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const fs::path root = fs::temp_directory_path() / ("dh-path-case-" + std::to_string(stamp));
    try {
        write(root / "data/3d/skybox/skybox_swamp.bdae", "sky");
        write(root / "data/3d/characters/npcs/animations/npc_m_dialog.bdae", "dialog");
        write(root / "data/3d/Mixed/Exact.bin", "exact");
        write(root / "worlds/level.mlx", "mlx");

        // Authored spellings from the shipped level data / scripts.
        const auto sky = find_path_ignoring_case(root, "data/3D/skybox/skybox_swamp.bdae");
        require(!sky.empty() && fs::is_regular_file(sky), "Skybox authored spelling must resolve");
        const auto dialog = find_path_ignoring_case(root, "data/3D/characters/npcs/animations/NPC_M_Dialog.bdae");
        require(!dialog.empty() && fs::is_regular_file(dialog), "Animation authored spelling must resolve");
        // The exact spelling always wins.
        const auto exact = find_path_ignoring_case(root, "data/3d/Mixed/Exact.bin");
        require(!exact.empty() && exact.filename() == "Exact.bin", "Exact spelling must be kept");
        // Absent stays absent.
        require(find_path_ignoring_case(root, "data/3d/skybox/nothing.bdae").empty(), "Missing file must stay missing");
        require(find_path_ignoring_case(root, "data/3d/nothing/skybox_swamp.bdae").empty(), "Missing directory must stay missing");

        // Full-path form keeps the caller's own spelling when nothing matches (its own error handling reports it).
        const auto full = existing_path_ignoring_case(root / "DATA/3d/SKYBOX/Skybox_Swamp.bdae");
        require(fs::is_regular_file(full), "Full path with a wrong-case prefix must resolve");
        const fs::path missing = root / "data/3d/skybox/nothing.bdae";
        require(existing_path_ignoring_case(missing) == missing, "Missing full path must be returned unchanged");

        // Through the catalog: the original failure ("Asset not found: data/3D/characters/npcs/animations/npc_m_dialog.bdae").
        AssetCatalog catalog(root);
        const auto bytes = catalog.read("data/3D/characters/npcs/animations/npc_m_dialog.bdae");
        require(std::string(bytes.begin(), bytes.end()) == "dialog", "Catalog read through authored spelling");
        const auto sky_bytes = catalog.read_authored("DATA/3D/SkyBox/Skybox_Swamp.bdae");
        require(std::string(sky_bytes.begin(), sky_bytes.end()) == "sky", "Catalog read_authored through authored spelling");
        bool escaped = false;
        try { catalog.resolve("../outside.bin"); } catch (const std::exception&) { escaped = true; }
        require(escaped, "Parent traversal must stay rejected");
        bool absent = false;
        try { catalog.read("data/3D/skybox/absent.bdae"); } catch (const std::exception&) { absent = true; }
        require(absent, "Absent catalog file must still throw");

        // A file created after the directory was first listed is found (listing cache follows the directory).
        find_path_ignoring_case(root, "worlds/LEVEL.mlx");
        write(root / "worlds/second.mlx", "two");
        require(!find_path_ignoring_case(root, "worlds/SECOND.MLX").empty(), "Late file must be found");
    } catch (const std::exception& failure) {
        std::error_code ec;
        fs::remove_all(root, ec);
        std::cerr << "path_case_tests FAILED: " << failure.what() << '\n';
        return 1;
    }
    std::error_code ec;
    fs::remove_all(root, ec);
    std::cout << "path_case_tests passed\n";
    return 0;
}
