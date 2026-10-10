#include "runtime_creation_source_loader_v1.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::frontend::creation;

int main(int argc, char** argv) {
    assert(argc == 2);
    const std::filesystem::path asset_root(argv[1]);
    AssetCatalog assets(asset_root);
    dh2::data::LootRandom8V2 caller_random{0x12345678u, 19};
    RuntimeCreationSourceOwnerV1 loaded;
    std::string error;
    if (!load_runtime_creation_source_v1(assets, caller_random, loaded, error))
        throw std::runtime_error("Android-root source-table load failed: " + error);
    assert(loaded.valid());
    assert(loaded.caller_owned_random_adapter == &caller_random);
    assert(caller_random.seed == 0x12345678u && caller_random.calls == 19);
    assert(loaded.character_design_caps_known);
    for (const auto cap : loaded.character_design_skill_caps) assert(cap > 0);
    std::cout << "CharacterDesign caps BNormal/CHard/DVeryHard: "
              << loaded.character_design_skill_caps[0] << '/'
              << loaded.character_design_skill_caps[1] << '/'
              << loaded.character_design_skill_caps[2] << '\n';
    assert(loaded.source_hashes.size() == 13);

    const auto source = loaded.source();
    assert(source.properties.get() == loaded.properties.get());
    assert(source.loot && source.skills);
    assert(source.source_loot_random == &caller_random);
    assert(source.source_skill_grant_context_known);
    assert(source.source_skill_level_caps == loaded.character_design_skill_caps);
    assert(source.properties->characters.names.size() > 300);
    assert(source.loot.loots().size() > 100);
    assert(source.skills.skills().size() == 127);
    assert(source.skills.lists().size() == 36);

    constexpr const char* expected[] = {
        "516ba82f631174d4c0a24708342549b5993c402f68c2c1dabd5ddc9eea138784",
        "ba0d987fe70d900dac7d8eb29dc28d87f48e48a15063b87c38e78bf5a2f83801",
        "7c955f759840dc7808b17ddb54213ce886abd5cdf6607cacc9048a3626803cc2",
        "e6349ebb1622404586a286f46ddea26d08e68c2d8a5e3e56bd711159d4becc55",
        "f1f84258e8fd5dd1967abb1f4ca7320d7c192c176ca26f20773dec140e1fd6fe",
        "3347d6e0b30aec9af544eafeb1c0883cc284c1a0585fdc472f51f407d578d268",
        "4dd85c8656d60c38a0c651e999fd18a1ed4d936f7cfdfcec52a1dfb4b568847f",
        "8ea5bc26e47d3b9aba1e1ab0fd3e96c4a989bbc2a6a647bf7c2b20071e8782ba",
        "ed6721327d6779e47e4def679ddeb796cf5da7ba0348d10996172e64d58bd323",
        "e336986d5aee78fd1aed7fe7ec43cc8d58fa03d4fd27216779c0e96e47acc2d6",
        "31642dc0d8bf11f1ebd13698bbefda4fda0aefa2c727fb4ae9449f2df073fb9f",
        "fd882501b0b7af0191272e6802558d1e1ac4031f6237cf5e2c767ebae946f4b9",
        "db014f9832c1d9e60c62272a2ee67d15568d003c191ae32bbce070cb49056931"};
    constexpr const char* filenames[] = {
        "character_properties_pyarray.bin", "character_properties_pyarraynames.bin",
        "character_properties_pystructnames.bin", "character_classes_pyarray.bin",
        "character_classes_pyarraynames.bin", "character_classes_pystructnames.bin",
        "loot_table_pyarray.bin", "loot_table_pyarraynames.bin", "loot_table_pystructnames.bin",
        "skills_pyarray.bin", "skills_pyarraynames.bin", "skills_pystructnames.bin",
        "design_pycst.bin"};
    const bool packaged_flattened = std::filesystem::exists(
        asset_root / "data/character_properties_pyarray.bin");
    for (std::size_t i = 0; i < loaded.source_hashes.size(); ++i) {
        assert(loaded.source_hashes[i].sha256 == expected[i]);
        assert(loaded.source_hashes[i].byte_count > 0);
        const auto expected_path = packaged_flattened
            ? std::string("data/") + filenames[i]
            : std::string("original-cache/data/pydata/") + filenames[i];
        assert(loaded.source_hashes[i].asset_path == expected_path);
        std::cout << loaded.source_hashes[i].asset_path << ' '
                  << loaded.source_hashes[i].byte_count << ' '
                  << loaded.source_hashes[i].sha256 << '\n';
    }

    // The previous shared-cache catalog is still supported through the same
    // content resolver's original-cache candidate. CTest's primary root is
    // the Android package; this exercises the retained cache-root form too.
    const auto shared_assets = std::filesystem::current_path().parent_path() / "windows-shared-assets";
    if (std::filesystem::exists(shared_assets / "original-cache/data/pydata/character_properties_pyarray.bin")) {
        AssetCatalog legacy(shared_assets);
        dh2::data::LootRandom8V2 legacy_random{0x76543210u, 3};
        RuntimeCreationSourceOwnerV1 legacy_loaded;
        assert(load_runtime_creation_source_v1(legacy, legacy_random, legacy_loaded, error));
        assert(legacy_loaded.valid());
        assert(legacy_loaded.source_hashes.size() == 13);
        for (std::size_t i = 0; i < legacy_loaded.source_hashes.size(); ++i) {
            assert(legacy_loaded.source_hashes[i].sha256 == expected[i]);
            assert(legacy_loaded.source_hashes[i].asset_path ==
                   std::string("original-cache/data/pydata/") + filenames[i]);
            std::cout << "legacy " << legacy_loaded.source_hashes[i].asset_path << ' '
                      << legacy_loaded.source_hashes[i].byte_count << ' '
                      << legacy_loaded.source_hashes[i].sha256 << '\n';
        }
    }

    // An incomplete catalog must fail atomically and retain the prior loaded
    // table owners, receipt set, and external RNG adapter.
    const auto scratch = std::filesystem::temp_directory_path() /
        ("dh-runtime-source-loader-empty-" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(scratch);
    AssetCatalog empty_assets(scratch);
    RuntimeCreationSourceOwnerV1 before = loaded;
    assert(!load_runtime_creation_source_v1(empty_assets, caller_random, loaded, error));
    assert(!error.empty());
    assert(loaded.valid());
    assert(loaded.properties.get() == before.properties.get());
    assert(loaded.loot_owner.get() == before.loot_owner.get());
    assert(loaded.skill_owner.get() == before.skill_owner.get());
    assert(loaded.source_hashes.size() == before.source_hashes.size());
    assert(loaded.caller_owned_random_adapter == before.caller_owned_random_adapter);
    std::error_code cleanup_error;
    std::filesystem::remove(scratch, cleanup_error);
    assert(!cleanup_error);
    return 0;
}
