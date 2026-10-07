#pragma once
#include <cstdint>
#include <string>
namespace dh2::android_ui {
// Native port of original DungeonHunter2.NotifyTrophy Java body. The original
// external Android package files directory is the actual private directory here.
bool source_android_notify_trophy_v114(const std::string&,std::int32_t,std::string&);
}
