#include "../data.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::data;

namespace {
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("missing Character table input: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}
Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) return 2;
        const std::string root = argv[1];
        const auto records = read(root + "/character_properties_pyarray.bin");
        const auto names = read(root + "/character_properties_pyarraynames.bin");
        const auto fields = read(root + "/character_properties_pystructnames.bin");
        CharacterTable table;
        std::string error;
        check(load_characters(bytes(records), bytes(names), bytes(fields), table, error),
              "actual Character table did not load");
        check(table.rows.size() == 448, "actual Character table row count differs");

        const auto name_field = std::find(table.fields.begin(), table.fields.end(), "ClassString");
        const auto description_field = std::find(table.fields.begin(), table.fields.end(), "ClassDescString");
        check(name_field != table.fields.end() && description_field != table.fields.end(),
              "actual specialization text fields are missing");
        const auto name_index = std::size_t(name_field - table.fields.begin());
        const auto description_index = std::size_t(description_field - table.fields.begin());
        for (const auto current : {263, 290, 325}) {
            std::array<std::int32_t, 4> actual{};
            check(possible_class_specialization_text_ids(table, current, actual, error),
                  "source specialization projection failed");
            const auto& first = table.rows.at(std::size_t(current + 1));
            const auto& second = table.rows.at(std::size_t(current + 2));
            check(actual == std::array<std::int32_t, 4>{first[name_index], first[description_index],
                      second[name_index], second[description_index]},
                  "source specialization IDs differ from adjacent class rows");
        }

        std::array<std::int32_t, 4> retained{7, 8, 9, 10};
        check(!possible_class_specialization_text_ids(table, -1, retained, error) &&
                  retained == std::array<std::int32_t, 4>{7, 8, 9, 10},
              "negative class changed output");
        check(!possible_class_specialization_text_ids(table, 446, retained, error) &&
                  retained == std::array<std::int32_t, 4>{7, 8, 9, 10},
              "out-of-range specialization rows changed output");

        CharacterTable malformed = table;
        malformed.fields.erase(std::remove(malformed.fields.begin(), malformed.fields.end(),
                                            "ClassDescString"), malformed.fields.end());
        check(!possible_class_specialization_text_ids(malformed, 263, retained, error) &&
                  retained == std::array<std::int32_t, 4>{7, 8, 9, 10},
              "missing description field changed output");
        std::cout << "{\"validation\":\"PASS\",\"actual_character_rows\":448,"
                     "\"class_families\":3,\"invalid_inputs\":3,\"mismatches\":0}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 3;
    }
}
