#include "canonical_cached_file_v1.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;using namespace dh2::loader;
static void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
static assets::ZipAssetPackV1 pack(const char* path){
 auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);check(bool(*f),"cache unavailable");
 assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());
 b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));if(!*f){e="cache read failure";return false;}return true;};
 assets::ZipAssetPackV1 p;std::string e;check(p.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return p;
}
static LevelFileWalkStepV1 finish(CanonicalCachedFileV1& f,const std::string& uri){
 for(unsigned i=0;i<1000;++i){auto s=f.step(uri,"Level");if(s!=LevelFileWalkStepV1::pending)return s;}
 throw std::runtime_error("file failed to terminate");
}
int main(int argc,char** argv){if(argc!=3)return 2;try{
 auto original=pack(argv[1]),fixtures=pack(argv[2]);auto pin=std::make_shared<int>(1);
 world::CanonicalPropertyMapV1 properties({});world::CanonicalReceiverConstructionV1 construction;
 world::CanonicalClassReceiverBindingsV1 bindings(properties,construction);world::CanonicalObjectManagerV1 manager({});
 unsigned checks=0;std::string error;
 {
  unsigned parses=0,releases=0;bool rejected=false;CanonicalCachedFileV1* current=nullptr;
  CanonicalFileSourceServicesV1 source{[&](bool ok,std::string&){++parses;rejected=current->step("player.mlx","Level")==LevelFileWalkStepV1::failed;return ok;},[&](std::string&){++releases;return true;}};
  CanonicalCachedFileV1 file(fixtures,manager,bindings.services(),source,{pin},ObjectEntryRouteV1::level);current=&file;
  check(file.step("player.mlx","Level")==LevelFileWalkStepV1::failed&&rejected,"recursive direct file delivery was accepted");
  check(parses==1&&releases==0&&file.attempts().empty()&&file.file().source(),"recursive parser delivery lost source or executed child prefix");
  const auto diagnostic=file.error();check(finish(file,"player.mlx")==LevelFileWalkStepV1::failed&&file.error()==diagnostic&&parses==1,"recursive failure replayed original source");++checks;
 }
 {
  unsigned parses=0,releases=0;
  CanonicalFileSourceServicesV1 source{[&](bool,std::string&)->bool{++parses;throw std::runtime_error("declared parser continuation exception");},[&](std::string&){++releases;return true;}};
  CanonicalCachedFileV1 file(fixtures,manager,bindings.services(),source,{pin},ObjectEntryRouteV1::level);
  check(file.step("player.mlx","Level")==LevelFileWalkStepV1::failed&&file.error()=="declared parser continuation exception","parser exception escaped source failure latch");
  check(file.file().source()&&file.attempts().empty()&&parses==1&&releases==0,"parser exception lost source prefix");
  check(finish(file,"player.mlx")==LevelFileWalkStepV1::failed&&parses==1,"exception replayed source parser");
  unsigned owner_calls=0;check(file.discard_after_owner_release([&](std::string&){++owner_calls;return true;},error)&&owner_calls==1&&releases==1,error);++checks;
 }
 {
  unsigned owner_calls=0,releases=0;bool rejected=false;CanonicalCachedFileV1* current=nullptr;
  CanonicalFileSourceServicesV1 source{[&](bool ok,std::string&){std::string inner;rejected=!current->discard_after_owner_release([&](std::string&){++owner_calls;return true;},inner);return ok;},[&](std::string&){++releases;return true;}};
  CanonicalCachedFileV1 file(fixtures,manager,bindings.services(),source,{pin},ObjectEntryRouteV1::level);current=&file;
  check(finish(file,"player.mlx")==LevelFileWalkStepV1::complete&&rejected&&owner_calls==0&&releases==1,"delivery-time cleanup released active source");
  check(file.attempts().size()==1&&file.attempts()[0]->step()==CanonicalBoundSourceStepV1::original_source_skip,"original Player exclusion changed");++checks;
 }
 {
  unsigned owner_calls=0,releases=0;bool owner_rejected=false,source_rejected=false;CanonicalCachedFileV1* current=nullptr;
  CanonicalFileSourceServicesV1 source{[](bool ok,std::string&){return ok;},[&](std::string&){++releases;std::string inner;source_rejected=!current->discard_after_owner_release([&](std::string&){++owner_calls;return true;},inner);return true;}};
  CanonicalCachedFileV1 file(original,manager,bindings.services(),source,{pin},ObjectEntryRouteV1::level);current=&file;
  check(finish(file,"data/scene/001_swamp.mlx")==LevelFileWalkStepV1::failed&&file.attempts().size()==1,"original SWAMP factory prefix changed");
  check(*file.attempts()[0]->source().entry().source().attribute("gametype")=="LevelConfig","wrong original SWAMP first class");
  check(file.discard_after_owner_release([&](std::string&){++owner_calls;std::string inner;owner_rejected=!current->discard_after_owner_release([&](std::string&){++owner_calls;return true;},inner);return true;},error),error);
  check(owner_rejected&&source_rejected&&owner_calls==1&&releases==1&&file.discarded()&&file.attempts().empty(),"recursive owner/source cleanup replayed callbacks or lost journal");++checks;
 }
 std::cout<<"{\"validation\":\"PASS\",\"direct_file_reentry_checks\":"<<checks<<",\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
