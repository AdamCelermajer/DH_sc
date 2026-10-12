#pragma once
#include "original_filesystem_fields_v50.hpp"
#include <functional>
#include <memory>
#include <string>
namespace dh2::android_ui {class OriginalCacheAssetsV1;}
namespace dh2::loader {
struct MaterialImageIdentityV43;
struct MainMountedImageFilesystemV50 {
 std::shared_ptr<android_ui::OriginalCacheAssetsV1> actual_mounted_cache;
 std::shared_ptr<OriginalWorkingDirectoryV50> actual_source_globals;
 std::weak_ptr<void> actual_renderer_owner;
 // Main rechecks its existing live_script_world/equipment_platform identities.
 std::function<bool(std::string&)> require_current_renderer_owner;
};
// Must borrow current cache AFTER mount; copy pins actual immutable State/fd.
// Never create a detached cache with a guessed AAssetManager or resource root.
bool borrow_current_mounted_cache_v50(const android_ui::OriginalCacheAssetsV1&,
 std::shared_ptr<android_ui::OriginalCacheAssetsV1>& out,std::string& error);
// Main's V45 images.canonical_uri callback supplies SAME reached CResFactory
// resource_directory. No per-actor inspection clone or assumed basename folder.
// Metadata-only lookup stops actual read_admitted before byte-vector allocation.
bool resolve_main_mounted_image_filename_v50(const MainMountedImageFilesystemV50&,
 const std::string& actual_CResFactory_resource_directory,
 const MaterialImageIdentityV43&,std::string& out,std::string& error);
} // namespace dh2::loader
