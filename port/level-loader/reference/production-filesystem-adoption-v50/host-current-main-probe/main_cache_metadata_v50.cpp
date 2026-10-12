#include "../main_mounted_image_filesystem_v50.hpp"
#include "../../../material_image_identity_v43.hpp"
#include "../main-source-snapshot/port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp"
#include <android/asset_manager.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
static unsigned asset_opens{};
AAsset* AAssetManager_open(AAssetManager* m,const char* name,int){if(!m||std::strcmp(name,"dh2-original-cache.zip"))return nullptr;int fd=::open(m->supplied_zip_path.c_str(),O_RDONLY);if(fd<0)return nullptr;++asset_opens;return new AAsset{fd};}
int AAsset_openFileDescriptor64(AAsset* a,off64_t* offset,off64_t* length){struct stat s{};if(!a||fstat(a->fd,&s))return -1;*offset=0;*length=s.st_size;return dup(a->fd);}
void AAsset_close(AAsset* a){if(a){close(a->fd);delete a;}}
int __android_log_print(int,const char*,const char*,...){return 0;}
using namespace dh2::loader;
static void ck(bool b,const char* m){if(!b)throw std::runtime_error(m);}
int main(int argc,char**argv){try{ck(argc==2,"Required supplied ZIP");AAssetManager manager{argv[1]};dh2::android_ui::OriginalCacheAssetsV1 actual(&manager);std::shared_ptr<dh2::android_ui::OriginalCacheAssetsV1> held;std::string e;
ck(borrow_current_mounted_cache_v50(actual,held,e)&&asset_opens==1,"Actual original-cache mount/copy failed");auto runtime=std::make_shared<int>(50);MainMountedImageFilesystemV50 fs;fs.actual_mounted_cache=held;fs.actual_source_globals=std::make_shared<OriginalWorkingDirectoryV50>();fs.actual_renderer_owner=runtime;
// Transport fixture for current renderer identity, not live Android UI binding.
fs.require_current_renderer_owner=[](std::string& e){e.clear();return true;};MaterialImageIdentityV43 image;image.state=MaterialImageStateV43::authored_image;image.authored_uri="q:/data/iphone/3d/textures/pvr2_env_crypt_alpha.tga";image.source_resource_uri="data/3d/Modules/Crypt/crypt.bdae";std::string uri="preserved";
ck(resolve_main_mounted_image_filename_v50(fs,"data/3d/Modules/Crypt",image,uri,e)&&e.empty()&&uri=="data/3d/textures/pvr2_env_crypt_alpha.tga"&&asset_opens==1,"Genuine mounted cache metadata route lost source prefix/alias or did not clear stop error");
unsigned absent{};
for(const auto* name:{"env_crypt_alpha","env_darktemple_alpha","env_earth_temple_alpha","env_castle_alpha","env_windtemple_alpha"}){image.authored_uri=std::string("q:/data/iphone/3d/textures/")+name+".tga";uri="preserved";ck(!resolve_main_mounted_image_filename_v50(fs,"data/3d/Modules",image,uri,e)&&uri=="preserved","Absent source image became pvr fallback");++absent;}
image.state=MaterialImageStateV43::source_no_image;uri="preserved";ck(!resolve_main_mounted_image_filename_v50(fs,"data/3d/Modules",image,uri,e)&&uri=="preserved","No-image entered mount/admission route");image.state=MaterialImageStateV43::authored_image;
auto missing=fs;missing.actual_source_globals.reset();ck(!resolve_main_mounted_image_filename_v50(missing,"data/3d/Modules",image,uri,e)&&uri=="preserved","Missing actual source globals accepted");runtime.reset();ck(!resolve_main_mounted_image_filename_v50(fs,"data/3d/Modules",image,uri,e)&&uri=="preserved","Expired actual runtime scope accepted");
// Existing mount copy retains immutable State/fd when original manager changes.
actual.manager(nullptr);bool stopped{},found{};std::vector<std::uint8_t> none;ck(!held->read_admitted("data/3d/textures/pvr2_env_crypt_alpha.tga",found,none,[&](auto,std::string& e){stopped=true;e="owned stop";return false;},e)&&stopped&&!found&&none.empty()&&none.capacity()==0&&asset_opens==1,"Actual mounted copy did not independently retain descriptor or metadata-before-bytes");held.reset();fs.actual_mounted_cache.reset();
std::cout<<"PASS actual_current_Main_cache_source=1 actual_supplied_ZIP=1 mounted_copy_retains_fd=1 metadata_before_payload=1 filename_source_kernel=1 five_missing_routes_preserved="<<absent<<" source_no_image_guard=1 missing_globals_expired_scope_guard=1 production_Android_UI_owner_binding=0 budget_GPU_claim=0\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
