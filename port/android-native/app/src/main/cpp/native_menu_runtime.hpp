#pragma once
#include <android/asset_manager.h>
#include <cstdint>
#include <string>
namespace dh2::native::menu {
// Platform I/O/clock adapter for the selected source NativeCreateSaveSlot.
// Each call owns only its original temporary indexed Save. Gameplay and the
// menu's preview Save remain separate and load this persisted profile later.
bool create_profile(AAssetManager*,const std::string& directory,
 std::int32_t* current_difficulty,const std::uint8_t* online,
 void* difficulty_context,bool (*difficulty_count)(void*,std::uint32_t*,std::string&),
 const std::string& name,const std::string& class_name,
 std::int32_t& slot,bool& result_published,std::string& error);
}
