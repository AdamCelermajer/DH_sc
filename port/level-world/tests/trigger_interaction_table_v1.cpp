#include "../trigger_interaction_table_v1.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error(std::string("cannot open ") + path);
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) {
            std::cerr << "usage: trigger_interaction_table_v1_audit <original-cache/data/pydata>\n";
            return 2;
        }
        const auto root = std::string(argv[1]);
        const auto data = read((root + "/game_objects_pyarray.bin").c_str());
        const auto names = read((root + "/game_objects_pyarraynames.bin").c_str());
        const auto fields = read((root + "/game_objects_pystructnames.bin").c_str());
        dh2::trigger_interaction_table_v1::Tables tables;
        std::string error;
        require(tables.load({data.data(), data.size()}, {names.data(), names.size()},
            {fields.data(), fields.size()}, error), error.c_str());
        require(tables.row_count() == 6, "TriggerObjects row count differs");
        struct Expected { const char* name; std::int32_t value; } expected[] = {
            {"Buff_Cauldron", 6}, {"Lever_Basic", 2}, {"Lever_Swamp", 2},
            {"Restoration_Cauldron", 6}, {"XP_Major_Cauldron", 6},
            {"XP_Minor_Cauldron", 6}};
        for (std::int32_t i = 0; i < 6; ++i) {
            std::int32_t data_id = -1;
            const auto* row = tables.find(expected[i].name, &data_id);
            require(row && data_id == i && row->interaction_type == expected[i].value,
                "TriggerObject source row/name/value differs");
        }
        require(tables.find("__missing__") == nullptr,
            "unknown TriggerObject data name resolved");
        std::cout << "trigger interaction table source rows=6 mismatches=0\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
