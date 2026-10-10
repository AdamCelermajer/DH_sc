// Runs the shipping ZIP reader and shipping process Arrays decoder directly.
// No Python parser, Android, fabricated World, or accepted native callback.
#include "source_process_arrays_v101.hpp"
#include "zip_asset_pack_v1.hpp"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace {
std::string quoted(const std::string& s){
 std::string out="\"";const char* hex="0123456789abcdef";
 for(unsigned char c:s){if(c=='"'||c=='\\'){out+='\\';out+=c;}
  else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=c;}
 return out+'"';
}
struct File {
 int fd{-1};std::uint64_t size{};
 ~File(){if(fd>=0)::close(fd);}
 bool read(std::uint64_t at,void* dst,std::size_t n,std::string& e){
  if(at>size||n>size-at){e="Host positional read exceeds archive";return false;}
  auto* p=static_cast<unsigned char*>(dst);std::size_t done=0;
  while(done<n){auto r=::pread(fd,p+done,n-done,static_cast<off_t>(at+done));
   if(r<0&&errno==EINTR)continue;
   if(r<=0){e="Host positional archive read failed";return false;}
   done+=std::size_t(r);}
  return true;
 }
};
struct Report {
 unsigned failures{};
 void result(const std::string& category,const std::string& subject,bool ok,const std::string& e={}){
  if(!ok)++failures;
  std::cout<<"{\"category\":"<<quoted(category)<<",\"subject\":"<<quoted(subject)
   <<",\"status\":"<<quoted(ok?"PASS":"FAIL")<<",\"detail\":"<<quoted(e)<<"}\n";
 }
};
}
int main(int argc,char** argv){
 if(argc!=2){std::cerr<<"usage: startup_data_host_v127 supplied-cache.zip\n";return 2;}
 Report report;
 try{
  auto file=std::make_shared<File>();file->fd=::open(argv[1],O_RDONLY);struct stat st{};
  if(file->fd<0||::fstat(file->fd,&st)||st.st_size<=0){report.result("cache","open",false,"Cannot open supplied cache");return 1;}
  file->size=std::uint64_t(st.st_size);
  dh2::assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=file->size;
  backing.read=[file](auto at,auto* p,auto n,auto& e){return file->read(at,p,n,e);};
  dh2::assets::ZipAssetPackV1 pack;std::string error;
  if(!pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error)){
   report.result("cache","mount",false,error);return 1;}
  const auto entries=pack.entries();report.result("cache","canonical directory count",entries.size()==6833,std::to_string(entries.size()));
  unsigned archive_ok=0;std::uint64_t read_bytes=0;
  for(const auto& entry:entries){
   bool found=false;std::vector<std::uint8_t> bytes;
   //Read one asset at a time; the production reader checks payload size/CRC.
   auto ok=pack.read(entry.uri,found,bytes,error)&&found&&bytes.size()==entry.bytes;
   if(ok){++archive_ok;read_bytes+=bytes.size();}else report.result("archive payload",entry.uri,false,error);
  }
  report.result("cache","all archive payloads",archive_ok==entries.size(),std::to_string(archive_ok)+" entries / "+std::to_string(read_bytes)+" decoded bytes");
  using namespace dh2::android_ui;
  const SourceProcessArraysV101::Read read=[&](const auto& uri,auto& bytes,auto& e){
   bool found{};if(!pack.read(uri,found,bytes,e))return false;if(!found){e="Missing real cache URI: "+uri;return false;}return true;};
  //Probe every URI independently so a missing early stream does not conceal others.
  for(const auto& f:process_pydata_constants_v101){bool found{};dh2::assets::ZipEntryV1 entry;
   std::string uri="data/"+std::string(f.uri);auto ok=pack.entry(uri,found,entry,error)&&found;
   report.result("constant URI",uri,ok,error);}
  for(const char* uri:{"data/pydata/trophies_pystructnames.bin","data/menus/dqshared_droid.swf",
      "data/menus/dqmenus_droid.swf","data/menus/loadanims_droid.swf","data/menus/dqhud_droid.swf"}){
   bool found{};dh2::assets::ZipEntryV1 entry;auto ok=pack.entry(uri,found,entry,error)&&found;report.result("startup URI",uri,ok,error);}
  SourceProcessArraysV101 arrays;bool complete{};
  while(arrays.stage()<70){const auto before=arrays.stage();auto ok=arrays.load_stage(read,complete,error);
   report.result("shipping Arrays decode",std::string("data/")+process_pydata_arrays_v101[before].uri,ok,error);if(!ok)break;}
  report.result("shipping Arrays decode","all 70 stages ready",arrays.ready());
  unsigned safety_ok=0;
  //Every stage must reject empty input, incident-sized count and a count that
  //is within the numeric cap but cannot fit the remaining wire bytes.
  //Fresh canonical owners preserve strict stage ordering and failure latching.
  for(unsigned target=0;target<70;++target)for(unsigned mode=0;mode<3;++mode){
   SourceProcessArraysV101 candidate;bool reached=false,rejected=false;
   auto altered=[&](const auto& uri,auto& bytes,auto& e){
    const auto selected="data/"+std::string(process_pydata_arrays_v101[target].uri);
    if(uri!=selected)return read(uri,bytes,e);
    reached=true;bytes.clear();if(mode){const std::uint32_t count=mode==1?1946157056u:1000000u;
     for(unsigned b=0;b<4;++b)bytes.push_back(static_cast<std::uint8_t>(count>>(8*b)));}return true;};
   for(unsigned step=0;step<=target;++step){if(!candidate.load_stage(altered,complete,error)){rejected=reached;break;}}
   if(rejected)++safety_ok;else report.result("decoder allocation safety",std::to_string(target)+"/"+std::to_string(mode),false,error);
  }
  report.result("decoder allocation safety","all 210 malformed-stream cases",safety_ok==210,std::to_string(safety_ok));
  unsigned nested_ok=0;
  for(const char* name:{"ai_factions_pyarray.bin","character_classes_pyarray.bin",
      "character_templates_pyarray.bin","item_powers_monopoly_pyarray.bin"}){
   unsigned target=70;for(unsigned i=0;i<70;++i)if(std::string(process_pydata_arrays_v101[i].registration)==name){target=i;break;}
   for(unsigned mode=0;mode<2;++mode){
    SourceProcessArraysV101 candidate;bool reached=false,rejected=false;
    auto altered=[&](const auto& uri,auto& bytes,auto& e){
     if(!read(uri,bytes,e))return false;
     if(uri!="data/"+std::string(process_pydata_arrays_v101[target].uri))return true;
     if(bytes.size()<8){e="Nested attack fixture lacks first actual vector";return false;}
     reached=true;const std::uint32_t count=mode?1000000u:1946157056u;
     for(unsigned b=0;b<4;++b)bytes[4+b]=static_cast<std::uint8_t>(count>>(8*b));
     if(mode)bytes.resize(8);return true;};
    if(target<70)for(unsigned step=0;step<=target;++step){if(!candidate.load_stage(altered,complete,error)){rejected=reached;break;}}
    if(rejected)++nested_ok;else report.result("nested allocation safety",std::string(name)+"/"+std::to_string(mode),false,error);
   }
  }
  report.result("nested allocation safety","all 8 nested-vector count attacks",nested_ok==8,std::to_string(nested_ok));
 }catch(const std::exception& x){report.result("host runner","exception",false,x.what());}
 std::cout<<"{\"summary\":true,\"failures\":"<<report.failures
  <<",\"full_app_startup_verified\":false,\"coverage\":\"shipping archive + Arrays + URI + allocation rejection\"}\n";
 return report.failures?1:0;
}
