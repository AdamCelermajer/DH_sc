#pragma once
#include <cstdint>
#include <ctime>
#include <string>
namespace dh2::ui {
// NativeGetSaveSlotDetails 0x44aa28 uses SavegameManager's numeric language.
const char* save_slot_date_format_v1(std::uint32_t language);
// Caller supplies the local calendar time derived from the saved LNAM date.
// This formats the original 80-byte date field; it does not own campaign data.
bool format_save_slot_local_date_v1(const std::tm& local_date,
    std::uint32_t language, std::string& output, std::string& error);
}
