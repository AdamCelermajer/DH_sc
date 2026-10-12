#include "main_level_root_filesystem_v52.hpp"
#include "../../material_image_identity_v43.hpp"
#include "../production-filesystem-adoption-v50/main-source-snapshot/port/android-native/app/src/main/cpp/original_cache_assets_v1.hpp"
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
#include <sstream>
#include "../production-filesystem-adoption-v50/main-source-snapshot/port/asset-payloads/zip_asset_pack_v1.hpp"
#include <fstream>
static std::string decode(const std::string& s){if(s=="-")return {};std::string out;for(std::size_t i=0;i<s.size();i+=2)out.push_back(static_cast<char>(std::stoul(s.substr(i,2),nullptr,16)));return out;}
int main(int argc,char**argv){try{ck(argc==4,"Required ZIP/original route/51 source fixtures");std::string e,line;unsigned source_routes{},source_cfs{};
std::ifstream cases(argv[2]);ck(bool(cases),"Original fixture unavailable");
while(std::getline(cases,line)){std::istringstream in(line);std::string kind,a,b,c,d;in>>kind;
 if(kind=="R"){unsigned nonempty{},count{};in>>nonempty>>a>>count;auto expected=std::vector<std::string>{};for(unsigned i=0;i<count;++i){in>>b;expected.push_back(decode(b));}LevelRootFilenameServicesV52 s;s.actual_filesystem_owner=std::make_shared<int>(52);s.is_using_uncompiled_data=[nonempty](const std::string& n,bool& out,std::string& e){out=original_level_uses_uncompiled_v52(nonempty!=0,n);e.clear();return true;};s.open_resource=[](const std::string&,bool& found,std::string&,std::string& e){found=false;e.clear();return true;};LevelRootFilenameResolverV52 route(std::move(s));std::string old="preserved";LevelRootFilenameTraceV52 trace;ck(!route.resolve(decode(a),old,&trace,e)&&old=="preserved"&&trace.open_resource_queries==expected,"Native original Level query order/mode/removal differs");++source_routes;}
 else{unsigned applied{};in>>a>>b>>c>>d;std::string want;in>>want>>applied;OriginalCfsFilenameMapV52 mapping;if(c!="-")mapping[decode(c)]=decode(d);std::string path;bool hit{};ck(original_cfs_open_filename_v52(decode(a),decode(b),mapping,path,hit,e)&&path==decode(want)&&hit==bool(applied)&&e.empty(),"Native original CFS filename projection differs");++source_cfs;}}
ck(source_routes==58&&source_cfs==75,"Original route/CFS fixtures incomplete");
AAssetManager manager{argv[1]};dh2::android_ui::OriginalCacheAssetsV1 actual(&manager);MainLevelRootFilesystemV52 fs;ck(borrow_current_mounted_cache_v50(actual,fs.mounted.actual_mounted_cache,e),"Actual Main mounted-cache borrow failed");auto runtime=std::make_shared<int>(52);fs.mounted.actual_renderer_owner=runtime;fs.mounted.require_current_renderer_owner=[](std::string& e){e.clear();return true;};fs.mounted.actual_source_globals=std::make_shared<OriginalWorkingDirectoryV50>();auto paths=std::make_shared<OriginalFileSystemPathsV50>();ck(construct_original_filesystem_paths_v50("",*paths,e),"Source Win32 path C1 failed");fs.actual_source_paths=paths;fs.actual_initialized_cfs_map=std::make_shared<OriginalCfsFilenameMapV52>();std::uint8_t lower=1;std::uintptr_t begin=0,end=0;fs.actual_application_owner=std::make_shared<int>(53);fs.actual_application_lowercase_a8=&lower;fs.actual_uncompiled_begin_cc=&begin;fs.actual_uncompiled_end_d0=&end;fs.actual_RES_PATH=std::make_shared<std::string>();
// Actual source-layout/init transport fixtures; these are not current Android
// Application/renderer owners or whole source bootstrap proof.
std::shared_ptr<LevelRootFilenameResolverV52> route;ck(bind_main_level_root_filename_v52(fs,route,e)&&e.empty(),"Typed current-cache/source filesystem binding failed");std::ifstream sources(argv[3]);ck(bool(sources),"51source fixture unavailable");unsigned files{};std::string a,b;
while(sources>>a>>b){std::string uri,want;ck(dh2::assets::ZipAssetPackV1::key(decode(b),want,e),"Original case failed actual mounted namespace canonicalization");LevelRootFilenameTraceV52 trace;const bool ok=route->resolve(decode(a),uri,&trace,e);if(!ok||uri!=want||!e.empty())std::cerr<<"V52 route name="<<decode(a)<<" actual="<<uri<<" expected="<<want<<" ok="<<ok<<" error="<<e<<'\n';ck(ok&&uri==want&&e.empty(),"Authentic bare original row failed source search/current cache metadata");++files;}
ck(files==51&&asset_opens==1,"All51 source rows/mounted alias proof incomplete");std::string unchanged="preserved";LevelRootFilenameTraceV52 trace;ck(!route->resolve("worlds/001_swamp.mlx",unchanged,&trace,e)&&unchanged=="preserved"&&trace.open_resource_queries.size()==4,"Modern worlds fixture became invented source alias");
auto missing=fs;missing.actual_initialized_cfs_map.reset();std::shared_ptr<LevelRootFilenameResolverV52> absent;ck(!bind_main_level_root_filename_v52(missing,absent,e)&&!absent,"Uninitialized source map fabricated empty/ready filesystem");
runtime.reset();fs.actual_application_owner.reset();ck(!route->resolve("001_swamp.mlx",unchanged,nullptr,e)&&unchanged=="preserved","Expired current renderer scope accepted");
std::cout<<"PASS original_Level_route_cases="<<source_routes<<" original_CFS_open_cases="<<source_cfs<<" current_Main_cache_actual_source=1 real_ZIP_root_metadata_files="<<files<<" worlds_alias_rejected=1 mounted_fd_single_owner=1 no_root_payload_read_or_XML_ready_claim=1 production_Android_owner_budget_GPU_claim=0\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
