from pathlib import Path
import subprocess,json,hashlib
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/production-filesystem-adoption-v50'
(r/'main_mounted_image_filesystem_v50.hpp').write_text('''#pragma once
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
''',newline='\n')
(r/'main_mounted_image_filesystem_v50.cpp.proposed').write_text('''#include "main_mounted_image_filesystem_v50.hpp"
#include "../../original_filename_policy_v49.hpp"
#include "../../material_image_identity_v43.hpp"
#include "main-source-snapshot/port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp"
#include "main-source-snapshot/port/asset-payloads/zip_asset_pack_v1.hpp"
namespace dh2::loader {
bool borrow_current_mounted_cache_v50(const android_ui::OriginalCacheAssetsV1& current,
 std::shared_ptr<android_ui::OriginalCacheAssetsV1>& out,std::string& error){
 std::size_t files{};
 if(!current.mount(files,error))return false;
 auto pin=std::make_shared<android_ui::OriginalCacheAssetsV1>(current);
 out=std::move(pin);error.clear();return true;
}
bool resolve_main_mounted_image_filename_v50(const MainMountedImageFilesystemV50& fs,
 const std::string& directory,const MaterialImageIdentityV43& image,
 std::string& out,std::string& error){
 auto runtime=fs.actual_renderer_owner.lock();
 if(!runtime||!fs.actual_mounted_cache||!fs.actual_source_globals||!fs.require_current_renderer_owner){
  error="Required same current renderer/mounted-cache/source-filesystem owners";return false;
 }
 if(!fs.require_current_renderer_owner(error))return false;
 if(image.state!=MaterialImageStateV43::authored_image){
  error="Source no-image must bypass filename/cache/admission services";return false;
 }
 const auto working=fs.actual_source_globals->value();
 std::array<std::string,2> candidates;
 if(!original_image_search_candidates_v49(directory,image.authored_uri,candidates,error))return false;
 for(const auto& source:candidates){
  OriginalFilenameResultV49 name;
  if(!original_filename_policy_v49(working,source,name,error))return false;
  // This is the current mounted ZIP's exact canonical namespace operation,
  // NOT q:/iphone stripping or an original compressed-format filename policy.
  std::string canonical;
  if(!assets::ZipAssetPackV1::key(name.filename,canonical,error)){
   error="Required source filesystem absolute/domain mount route: "+error;return false;
  }
  bool stop=false,found=false;std::vector<std::uint8_t> unused;
  const std::string stopped="V50 owned metadata stop before payload";
  const bool read=fs.actual_mounted_cache->read_admitted(canonical,found,unused,
   [&](std::uint32_t,std::string& e){stop=true;e=stopped;return false;},error);
  if(fs.actual_source_globals->value()!=working||!fs.require_current_renderer_owner(error)){
   if(error.empty())error="Source filesystem working-directory changed during filename scope";
   return false;
  }
  if(stop){
   if(read||found||error!=stopped||!unused.empty()||unused.capacity()!=0){
    error="Actual cache metadata probe allocated/read payload or changed stop contract";return false;
   }
   out=std::move(canonical);error.clear();return true;
  }
  if(!read)return false;
  if(found||!unused.empty()||unused.capacity()!=0){error="Actual cache lookup unexpectedly bypassed before-payload probe";return false;}
 }
 error="Original filename candidates absent from actual mounted cache; source route/format provider remains required";
 return false;
}
} // namespace dh2::loader
''',newline='\n')
# Required-only syntax fixture, not an implementation of any Android host API.
(r/'host-include/android').mkdir(parents=True,exist_ok=True)
(r/'host-include/android/asset_manager.h').write_text('#pragma once\nstruct AAssetManager; // exact opaque declaration used by main header only\n',newline='\n')
def lx(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
main=Path('C:/Users/adamc/Desktop/workspace/DH_sc')
cmd=['wsl','--','/usr/bin/g++','-std=c++17','-Wall','-Wextra','-Werror','-fsyntax-only','-x','c++',lx(r/'main_mounted_image_filesystem_v50.cpp.proposed'),'-I'+lx(r/'host-include'),'-I'+lx(main/'port/engine-resources'),'-I'+lx(main/'port/scene-materials'),'-I'+lx(main/'port/engine-math')]
run=subprocess.run(cmd,capture_output=True,text=True)
(r/'mounted-adapter-syntax-v50.log').write_text(run.stdout+run.stderr)
assert run.returncode==0,run.stderr
(r/'mounted-adapter-syntax-v50.json').write_text(json.dumps({'validation':'SYNTAX_PASS_NOT_LINKED','command':cmd,'actual_main_API_header_snapshot':True,'Android_API_implementation_supplied':False,'production_working_directory_binding':False,'current_main_runtime_mount_GPU_probe':False},indent=2)+'\n')
print('V50 typed current-mounted-cache proposal: strict syntax PASS, not linked/integrated/runtime verified.')
