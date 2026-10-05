#include "../save_slot_date_v1.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
int main(int argc, char** argv) {
    if (argc != 2) return 2;
    std::ifstream input(argv[1]);
    std::string line;
    unsigned count = 0;
    while (std::getline(input, line)) {
        std::vector<std::string> fields;
        std::istringstream row(line);
        std::string field;
        while (std::getline(row, field, '|')) fields.push_back(field);
        if (fields.size() != 9) return 3;
        const auto language = static_cast<std::uint32_t>(std::stoull(fields[0]));
        std::tm date{};
        date.tm_year = std::stoi(fields[1]) - 1900;
        date.tm_mon = std::stoi(fields[2]) - 1;
        date.tm_mday = std::stoi(fields[3]);
        date.tm_hour = std::stoi(fields[4]);
        date.tm_min = std::stoi(fields[5]);
        date.tm_sec = std::stoi(fields[6]);
        std::string output = "old", error = "old";
        if (fields[7] != dh2::ui::save_slot_date_format_v1(language) ||
            !dh2::ui::format_save_slot_local_date_v1(date, language, output, error) ||
            output != fields[8] || !error.empty()) {
            std::fprintf(stderr, "Mismatch row %u: %s\n", count, output.c_str());
            return 4;
        }
        ++count;
    }
    if (count != 1028) return 5;
    std::printf("PASS %u original-language date projections\n", count);
}
