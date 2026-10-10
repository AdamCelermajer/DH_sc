#include "../asset_catalog.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using dh::foundation::AssetCatalog;

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

template <typename Action>
void require_failure(Action action, const std::string& message) {
    bool failed = false;
    try { action(); } catch (const std::exception&) { failed = true; }
    require(failed, message);
}

struct Fixture {
    fs::path directory;
    Fixture() {
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        directory = fs::temp_directory_path() / ("dh-foundation-assets-" + std::to_string(stamp));
        require(fs::create_directory(directory), "Could not create isolated asset fixture");
    }
    ~Fixture() {
        std::error_code error;
        fs::remove_all(directory, error);
    }
};

void write_file(const fs::path& path, const std::vector<unsigned char>& bytes) {
    fs::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary);
    output.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    require(output.good(), "Could not write asset fixture");
}

void fixture_checks() {
    Fixture fixture;
    const auto assets = fixture.directory / "assets";
    for (const auto* name : {"models", "worlds", "animations", "data"})
        fs::create_directories(assets / name);
    const std::vector<unsigned char> bytes{0, 1, 127, 128, 255};
    write_file(assets / "models/prince_fixture.bdae", bytes);
    write_file(assets / "worlds/opening.dwld", {9, 8});
    write_file(assets / "data/prince-animation-bank.json", {'{', '}'});
    write_file(fixture.directory / "outside.bin", {42});
    fs::create_directories(fixture.directory / "build/nested");

    require(fs::equivalent(AssetCatalog::discover_root(assets), assets), "Direct asset-root discovery failed");
    require(fs::equivalent(AssetCatalog::discover_root(fixture.directory / "build/nested"), assets),
            "Asset-root discovery from a nested build directory failed");
    AssetCatalog catalog(assets);
    require(fs::equivalent(catalog.root(), assets), "Catalog retained an incorrect root");
    require(catalog.read("models/prince_fixture.bdae") == bytes, "Binary asset reads changed bytes");
    require(fs::equivalent(catalog.resolve("models/prince_fixture.bdae"), assets / "models/prince_fixture.bdae"),
            "Asset resolution returned an incorrect file");

    require_failure([&] { catalog.resolve("../outside.bin"); }, "Parent traversal was accepted");
    require_failure([&] { catalog.resolve("models/../../outside.bin"); }, "Nested traversal was accepted");
    require_failure([&] { catalog.resolve(fixture.directory / "outside.bin"); }, "Absolute asset path was accepted");
    require_failure([&] { catalog.resolve(""); }, "Empty asset path was accepted");
    require_failure([&] { catalog.resolve("models"); }, "Directory was accepted as an asset file");
    require_failure([&] { catalog.read("models/missing.bdae"); }, "Missing asset read did not fail");
    require_failure([&] { AssetCatalog missing(fixture.directory / "absent"); }, "Missing asset root was accepted");
    require_failure([&] { AssetCatalog file_root(fixture.directory / "outside.bin"); }, "File was accepted as asset root");

    const auto inventory = catalog.inventory();
    require(inventory.size() == 3, "Fixture inventory included files outside the asset root or missed files");
    for (std::size_t index = 0; index < inventory.size(); ++index) {
        require(!inventory[index].relative_path.is_absolute(), "Inventory exposed an absolute path");
        require(inventory[index].bytes == fs::file_size(assets / inventory[index].relative_path), "Inventory size mismatch");
        if (index) require(inventory[index - 1].relative_path.generic_string() < inventory[index].relative_path.generic_string(),
                           "Inventory order is not deterministic");
    }
    require(catalog.find("PRINCE_FIXTURE.BDAE").size() == 1, "Case-insensitive filename lookup failed");
    require(catalog.find("models/prince_").size() == 1, "Relative-path substring lookup failed");
    require(catalog.original_levels().size() == 1, "Original level inventory failed");
    require(catalog.prince_resources().size() == 2, "Prince resource inventory failed");
    std::cout << "Fixture asset-path, binary-read, discovery, and inventory checks passed\n";
}

void original_content_checks(const fs::path& start) {
    const auto root = AssetCatalog::discover_root(start);
    AssetCatalog catalog(root);
    const auto inventory = catalog.inventory();
    require(!inventory.empty(), "Original resource inventory is empty");
    require(!catalog.original_levels().empty(), "No original world resources found");
    require(!catalog.prince_resources().empty(), "No original Prince resources found");
    for (const auto* relative : {"models/prince_modular.bdae", "data/prince-animation-bank.json", "original-media/intro.mp4"}) {
        const auto file = catalog.resolve(relative);
        require(fs::file_size(file) > 0, std::string("Original resource is empty: ") + relative);
    }
    const auto prince = catalog.read("models/prince_modular.bdae");
    require(prince.size() == fs::file_size(root / "models/prince_modular.bdae"), "Original Prince binary read was truncated");
    std::cout << "Actual-content availability checks passed: " << inventory.size() << " resources at " << root.string()
              << "\nAvailability checks do not establish Act 1 coverage, decoding, or cinematic playback.\n";
}
}

int main(int argc, char** argv) {
    try {
        fixture_checks();
        original_content_checks(argc > 1 ? fs::path(argv[1]) : fs::current_path());
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Asset test failure: " << error.what() << '\n';
        return 1;
    }
}
