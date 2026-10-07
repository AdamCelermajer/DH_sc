#include "save_slot_date_v1.hpp"
namespace dh2::ui {
const char* save_slot_date_format_v1(std::uint32_t language) {
    switch (language) {
        case 1: case 3: case 7: return "%d/%m     %H:%M";
        case 2: return "%d.%m.     %H:%M";
        case 4: case 5: case 6: return "%m.%d.     %H:%M";
        default: return "%m/%d     %H:%M";
    }
}
bool format_save_slot_local_date_v1(const std::tm& local_date,
    std::uint32_t language, std::string& output, std::string& error) {
    char buffer[80]{};
    const auto count = std::strftime(buffer, sizeof(buffer),
        save_slot_date_format_v1(language), &local_date);
    if (!count) { error = "Cannot format save-slot local date"; return false; }
    output.assign(buffer, count);
    error.clear();
    return true;
}
}
