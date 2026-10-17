#include "../../world-data/source_layout_adapter.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: source_spawnpoints_adapter_audit original-cache-root expected-spwn\n";
        return 2;
    }
    try {
        const std::string root = argv[1];
        const auto mlx = read(root + "/data/scene/001_swamp.mlx");
        const auto expected = read(argv[2]);
        constexpr std::array<const char*, 9> names = {
            "obj_4of4_brdwalk_sw_00.mgp",
            "obj_3of4_brdwalk_sw_00.mgp",
            "obj_1of4_brdwalk_nse_00.mgp",
            "corner_ruin_ws_00.mgp",
            "merchantcamp_ruins_swe_00.mgp",
            "corner_brdwalk_se_00.mgp",
            "deadend_brdwalk_w_00.mgp",
            "bossroom_ruins_ns_.mgp",
            "obj_2of4_brdwalk_sw_00.mgp"
        };
        std::array<std::vector<std::uint8_t>, names.size()> bytes;
        std::array<std::string, names.size()> paths;
        std::array<dh2_world_source_mgp, names.size()> mgps{};
        for (std::size_t i = 0; i < names.size(); ++i) {
            paths[i] = std::string("data/3d/modules/swamp/mgp/") + names[i];
            bytes[i] = read(root + "/" + paths[i]);
            mgps[i] = {paths[i].c_str(), bytes[i].data(), bytes[i].size()};
        }

        std::array<std::uint8_t, 512> output{};
        std::size_t output_size = 0;
        char error[192]{};
        require(dh2_world_compile_static_spawnpoints(mlx.data(), mlx.size(),
                    "001_swamp", "data/scene/001_swamp.mlx", mgps.data(), mgps.size(),
                    output.data(), output.size(), &output_size, error, sizeof(error)),
                error[0] ? error : "SWAMP SpawnPoint source rejected");
        require(output_size == expected.size(), "SPWN v1 byte count changed");
        require(std::equal(expected.begin(), expected.end(), output.begin()),
                "source MGP SpawnPoints differ from checked-in SPWN v1 bytes");

        // Changing the direct ID 0 to ID 3 creates a duplicate supported ID.
        // The adapter must fail closed before returning a partial SPWN table.
        auto duplicate_bytes = bytes;
        auto duplicate_mgps = mgps;
        std::string first_mgp(duplicate_bytes[0].begin(), duplicate_bytes[0].end());
        const std::string source_id = "entrypointID=\"0\"";
        const auto id_offset = first_mgp.find(source_id);
        require(id_offset != std::string::npos &&
                    first_mgp.find(source_id, id_offset + source_id.size()) == std::string::npos,
                "expected a single source SpawnPoint ID 0");
        first_mgp.replace(id_offset, source_id.size(), "entrypointID=\"3\"");
        duplicate_bytes[0].assign(first_mgp.begin(), first_mgp.end());
        duplicate_mgps[0].data = duplicate_bytes[0].data();
        duplicate_mgps[0].size = duplicate_bytes[0].size();
        output_size = 123;
        require(!dh2_world_compile_static_spawnpoints(mlx.data(), mlx.size(),
                    "001_swamp", "data/scene/001_swamp.mlx", duplicate_mgps.data(),
                    duplicate_mgps.size(), output.data(), output.size(), &output_size,
                    error, sizeof(error)), "duplicate supported SpawnPoint ID was accepted");
        require(output_size == 0, "failed SPWN compilation exposed partial output size");

        std::cout << "source SpawnPoint adapter: exact SPWN v1 match; duplicate ID rejected\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
