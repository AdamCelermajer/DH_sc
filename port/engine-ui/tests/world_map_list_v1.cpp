#include "../world_map_list_v1.hpp"
#include <cstdlib>
#include <iostream>

namespace {
void require(bool value, const char* message) {
    if (!value) {
        std::cerr << "FAIL " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    using namespace dh2::data;
    using namespace dh2::ui;
    std::vector<FastTravelDestination> source{
        {"SWAMP_CAMP", 1101, 1, "001_swamp.mlx", 7, 2101},
        {"CRYPT_GATE", 1102, 2, "007_crypt_01.rule.xml", 9, 2102},
        {"FROZEN_PASS", 1103, 3, "009_frozen.mlx", 4, 2103}};
    std::array<std::uint32_t, 2> words{{0x80000003u, 1u}};
    std::vector<WorldMapListRowV1> output;
    std::string error = "stale";
    require(world_map_list_v1(source, words, output, error), "project actual rows");
    require(error.empty() && output.size() == 3, "clear error and preserve source count");
    require(output[0].level_name == source[0].level_name &&
            output[0].description_id == 1101 && output[0].importance == 7 &&
            output[0].entry_point_id == 1 &&
            output[0].description_string_id == 2101 && output[0].visible,
            "row fields map to SWF source fields and saved bit 1");
    require(output[1].visible && !output[2].visible,
            "unlock bits use source FastTravelList ordinal across word boundary");
    std::vector<FastTravelDestination> empty;
    require(world_map_list_v1(empty, words, output, error) && output.empty(),
            "empty catalogue returns a valid empty projection");
    source.resize(65);
    output.push_back({});
    require(!world_map_list_v1(source, words, output, error) &&
            output.size() == 1 && !error.empty(),
            "source bitset overflow fails without partial output");
    std::cout << "PASS world_map_list_v1 checks=5\n";
}
