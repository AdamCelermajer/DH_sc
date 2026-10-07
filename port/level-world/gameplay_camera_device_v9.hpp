#pragma once
#include <cstdint>
#include <string>
namespace dh2::camera {
// Original Android DungeonHunter2.Get_PhoneManufacturer DEX + appInit switch.
// Call with the SAME JNI-provided Build.MANUFACTURER; no GL-vendor inference.
bool source_android_manufacturer_v9(const char*,std::int32_t& code,std::string&);
bool source_lg_devices_v9(const char*,std::uint8_t& lg,std::string&);
}
