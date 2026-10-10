#include "../../../level-world/character_candidate_cache_v62.hpp"
#include "../../../engine-resources/resources.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    try {
        if (argc != 4) return 2;
        const std::filesystem::path pydata_root = argv[1];
        const std::filesystem::path asset_root = argv[2];
        const std::filesystem::path template_source_root = argv[3];
        dh2::character::ScriptAssetServicesV1 files;
        files.read = [pydata_root, asset_root, template_source_root](const std::string& uri, bool& found,
                            std::vector<std::uint8_t>& bytes, std::string& error) {
            bytes.clear();
            constexpr char pydata_prefix[] = "data/pydata/";
            std::vector<std::filesystem::path> candidates;
            if (uri.rfind(pydata_prefix, 0) == 0) {
                const auto relative = std::filesystem::path(uri.substr(sizeof(pydata_prefix) - 1));
                candidates.push_back(pydata_root / relative);
                candidates.push_back(asset_root / std::filesystem::path(uri));
                candidates.push_back(template_source_root / relative);
            } else {
                candidates.push_back(asset_root / std::filesystem::path(uri));
            }
            for (const auto& path : candidates) {
                std::ifstream input(path, std::ios::binary);
                if (!input) continue;
                found = true;
                bytes.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
                if (!input.eof() && input.fail()) {
                    error = "Source cache read failed: " + path.string();
                    return false;
                }
                error.clear();
                return true;
            }
            found = false;
            error.clear();
            return true;
        };

        auto cache = std::make_shared<dh2::character::CharacterCandidateCacheV62>(files);
        std::string error;
        if (!cache->load(error)) throw std::runtime_error("Actual Character cache load: " + error);
        if (!cache->ready() || cache->models().values.size() <= 74 ||
            cache->models().values[74] != "data/3D/characters/npcs/Priest_good.bdae") {
            throw std::runtime_error("WanderingPriest model dictionary decision differs");
        }

        bool found = false;
        std::vector<std::uint8_t> bytes;
        if (!cache->files().read(cache->models().values[74], found, bytes, error) || !found ||
            bytes.size() != 42136) {
            throw std::runtime_error("Actual Priest_good BRES read: " + error);
        }
        dh2::resources::BresView image{};
        if (dh2_bres_open(&image, bytes.data(), bytes.size()) != dh2::resources::BresError::ok) {
            throw std::runtime_error("Original-cache Priest_good payload is not a valid BRES");
        }
        std::cout << "CharacterCandidateCacheV62 loaded actual source cache and read original Priest_good BRES PASS bytes="
                  << bytes.size() << '\n';
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
