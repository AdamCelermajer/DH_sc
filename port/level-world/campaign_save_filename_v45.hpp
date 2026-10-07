#pragma once
#include <cstdint>
#include <string>
namespace dh2::level {
// Whole SG_GetFilename463c84 string result; callers supply source slot/flags.
std::string campaign_save_filename_v45(std::uint32_t source_slot,bool checkpoint,bool multi);
}
