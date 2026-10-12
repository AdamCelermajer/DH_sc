#pragma once
#include "../production-filesystem-adoption-v50/main_mounted_image_filesystem_v50.hpp"
#include "level_root_cfs_filename_v52.hpp"
#include "../../level_root_filename_route_v52.hpp"
#include <cstdint>
namespace dh2::loader {
struct MainLevelRootFilesystemV52 {
 MainMountedImageFilesystemV50 mounted;
 std::shared_ptr<const OriginalFileSystemPathsV50> actual_source_paths;
 std::shared_ptr<const OriginalCfsFilenameMapV52> actual_initialized_cfs_map;
 std::shared_ptr<void> actual_application_owner;
 const std::uint8_t* actual_application_lowercase_a8{};
 const std::uintptr_t* actual_uncompiled_begin_cc{};
 const std::uintptr_t* actual_uncompiled_end_d0{};
 // Reached actual platform global, independently of source constructor buffers.
 std::shared_ptr<const std::string> actual_RES_PATH;
};
bool open_main_level_root_resource_v52(const MainLevelRootFilesystemV52&,
 const std::string& source_name,bool& found,std::string& canonical,std::string& error);
bool bind_main_level_root_filename_v52(MainLevelRootFilesystemV52,
 std::shared_ptr<LevelRootFilenameResolverV52>&,std::string& error);
} // namespace dh2::loader
