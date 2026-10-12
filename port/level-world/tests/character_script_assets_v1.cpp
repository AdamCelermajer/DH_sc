#include "../character_script_assets_v1.hpp"
#include "../../asset-payloads/zip_asset_pack_v1.hpp"
#include "../../asset-payloads/sha256.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
namespace {
unsigned checks{},guards{};
void check(bool ok,const std::string& e){++checks;if(!ok)throw std::runtime_error(e);}
struct File {
 int fd;std::uint64_t bytes;
 explicit File(const char* path):fd(open(path,O_RDONLY)),bytes(0){check(fd>=0,"Open cache ZIP");struct stat st{};check(!fstat(fd,&st)&&st.st_size>0,"Stat cache ZIP");bytes=st.st_size;}
 ~File(){if(fd>=0)close(fd);}
};
assets::ZipBackingV1 source(const char* path){
 auto owner=std::make_shared<File>(path);assets::ZipBackingV1 out;out.owner=owner;out.bytes=owner->bytes;
 out.read=[owner](std::uint64_t at,void* out,std::size_t n,std::string& e){
  auto* p=static_cast<unsigned char*>(out);std::size_t done=0;
  while(done<n){auto count=pread(owner->fd,p+done,n-done,static_cast<off_t>(at+done));if(count<0&&errno==EINTR)continue;if(count<=0){e="Short cache backing read";return false;}done+=count;}
  e.clear();return true;
 };return out;
}
std::string digest(const std::vector<std::uint8_t>& bytes){
 assets::Sha256Digest raw{};check(assets::sha256(bytes.data(),bytes.size(),raw),"Digest script");
 std::string out;const char* hex="0123456789abcdef";for(auto b:raw){out+=hex[b>>4];out+=hex[b&15];}return out;
}
}
int main(int argc,char** argv){try{
 check(argc==3,"Usage: script_assets archive expected-script-sha.tsv");std::string e;
 auto pack=std::make_shared<assets::ZipAssetPackV1>();check(pack->mount(source(argv[1]),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);
 character::ScriptAssetServicesV1 services;
 services.directory=[pack](auto& out,auto& e){out.clear();for(auto& item:pack->entries())out.push_back(item.uri);e.clear();return true;};
 services.read=[pack](const auto& path,auto& found,auto& bytes,auto& e){return pack->read(path,found,bytes,e);};
 std::array<std::vector<std::uint8_t>,3> raw;
 const char* suffix[]{"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
 for(unsigned i=0;i<3;++i){bool found=false;check(services.read(std::string("data/pydata/skills")+suffix[i],found,raw[i],e)&&found,e);}
 auto skills=std::make_unique<data::SkillTables>();check(skills->load({raw[0].data(),raw[0].size()},{raw[1].data(),raw[1].size()},{raw[2].data(),raw[2].size()},e),e);
 auto owner=std::make_unique<character::CharacterScriptAssetsV1>();check(owner->load(services,skills->borrow(),e),e);
 auto borrow=owner->borrow();check(bool(borrow)&&borrow.common().data&&borrow.files().size()==219,"Complete authored .luac universe");
 const auto* original=borrow.find("data/scripts/ai/_commons.luac");check(original&&original->data()==borrow.common().data,"Same common-script backing");
 const auto common_hash=digest(*original);
 std::ifstream manifest(argv[2]);check(bool(manifest),"Open independent script manifest");
 std::string line;unsigned matched=0;std::size_t bytes=0;
 while(std::getline(manifest,line)){
  auto a=line.find('\t'),b=line.find('\t',a+1);check(a!=line.npos&&b!=line.npos,"Manifest row");
  auto* script=borrow.find(line.substr(0,a));check(script&&script->size()==std::stoull(line.substr(a+1,b-a-1))&&digest(*script)==line.substr(b+1),"Independent cache script hash mismatch");
  bytes+=script->size();++matched;
 }
 check(matched==219&&bytes==borrow.bytes(),"Complete script hash/count/bytes coverage");
 check(borrow.find("data/scripts/skills/DarkQueen2_ranged_00.luac")==borrow.find("data/scripts/skills/darkqueen2_ranged_00.luac"),"Source table spelling uses actual ZIP case policy");
 std::vector<character::ScriptSessionFile> includes;
 check(borrow.session_files("data/scripts/ai/monster.luac",includes,e),e);
 check(std::none_of(includes.begin(),includes.end(),[](const auto& f){return f.filename=="data/scripts/ai/_commons.luac"||f.filename=="data/scripts/ai/monster.luac";}),"Session-owned common/external not duplicated");
 auto authored=std::find_if(includes.begin(),includes.end(),[](const auto& f){return f.filename=="data/scripts/skills/DarkQueen2_ranged_00.luac";});
 check(authored!=includes.end()&&authored->bytes==*borrow.find(authored->filename),"Existing exact-key session gets real authored spelling");
 auto prior=includes.size();check(!borrow.session_files("data/scripts/ai/missing.luac",includes,e)&&includes.size()==prior,"Failed external setup is atomic");++guards;
 const auto session_files=includes.size();
 auto skill_count=borrow.skills().skills().size(),faery_count=borrow.faeries().faeries().size();
 unsigned required=0,missing=0;
 {const auto skill_rows=borrow.skills();const auto faery_rows=borrow.faeries();
 auto binding=[&](const auto& row){if(row.script.empty())return;auto name="data/scripts/skills/"+row.script+".luac";
  if(borrow.find(name))++required;
  else{check(std::find(borrow.missing_scripts().begin(),borrow.missing_scripts().end(),name)!=borrow.missing_scripts().end(),"Actual cache miss retained without a replacement");++missing;}};
 for(const auto& row:skill_rows.skills())binding(row);
 for(const auto& row:faery_rows.faeries())binding(row);}
 check(missing>0&&!borrow.find("data/scripts/skills/faerie_celest_mage.luac")&&borrow.find("data/scripts/skills/faerie_celest.luac"),"Original optional class spell miss is not rewritten to base spell");
 check(!owner->load(services,skills->borrow(),e)&&e.find("borrow")!=e.npos,"Live cache reload denied");++guards;
 borrow={};
 auto fail=[&](character::ScriptAssetServicesV1 bad,const std::string& label){
  check(!owner->load(bad,skills->borrow(),e),label);
  auto retained=owner->borrow();check(retained.find("data/scripts/ai/_commons.luac")==original,"Failure preserves prior snapshot: "+label);++guards;
 };
 auto bad=services;bad.directory={};fail(bad,"Missing directory provider");
 bad=services;bad.read={};fail(bad,"Missing read provider");
 bad=services;bad.directory=[](auto&,auto& e){e="Directory failure";return false;};fail(bad,"Directory failure");
 bad=services;bad.directory=[services](auto& out,auto& e){if(!services.directory(out,e))return false;out.push_back("data/scripts/ai/_commons.luac");return true;};fail(bad,"Duplicate cache URI");
 for(const auto& path:{std::string("data/scripts/../bad.luac"),std::string("data/scripts//bad.luac"),std::string("data/scripts/ai\\bad.luac"),std::string("data/scripts/ai/bad")+std::string(1,'\0')+"suffix.luac"}){
  bad=services;bad.directory=[services,path](auto& out,auto& e){if(!services.directory(out,e))return false;out.push_back(path);return true;};fail(bad,"Unsafe exact script identity");
 }
 for(const auto& path:{"data/scripts/ai/_commons.luac","data/scripts/skills/_commons.luac","data/pydata/faeries_pyarray.bin","data/pydata/faeries_pyarraynames.bin","data/pydata/faeries_pystructnames.bin"}){
  bad=services;bad.read=[services,path](const auto& name,auto& found,auto& out,auto& e){if(name==path){found=false;out.clear();e.clear();return true;}return services.read(name,found,out,e);};fail(bad,"Missing required authored file");
 }
 bad=services;bad.read=[services](const auto& name,auto& found,auto& out,auto& e){if(!services.read(name,found,out,e))return false;if(name=="data/pydata/faeries_pyarray.bin")out.resize(2);return true;};fail(bad,"Malformed Faery table");
 bad=services;bad.read=[services](const auto& name,auto& found,auto& out,auto& e){if(!services.read(name,found,out,e))return false;if(name=="data/scripts/ai/_commons.luac")out.clear();return true;};fail(bad,"Empty script rejected");
 bad=services;bad.read=[services](const auto& name,auto& found,auto& out,auto& e){if(!services.read(name,found,out,e))return false;if(name=="data/scripts/ai/_commons.luac")out.resize(8u*1024u*1024u+1);return true;};fail(bad,"Oversize script rejected");
 bad=services;bad.directory=[services](auto& out,auto& e){if(!services.directory(out,e))return false;out.erase(std::remove(out.begin(),out.end(),"data/scripts/skills/_commons.luac"),out.end());return true;};fail(bad,"Missing source common directory entry");
 check(owner->load(services,skills->borrow(),e),"Valid reload without live borrowers");
 borrow=owner->borrow();owner.reset();skills.reset();pack.reset();services={};bad={};raw={};
 check(borrow.files().size()==219&&borrow.skills().skills().size()==skill_count&&borrow.faeries().faeries().size()==faery_count,"Owned scripts/tables survive all upstream owners");
 check(digest(*borrow.find("data/scripts/ai/_commons.luac"))==common_hash,"Retained script bytes available");
 std::cout<<"{\"validation\":\"PASS\",\"script_files\":"<<matched<<",\"script_bytes\":"<<bytes<<",\"skills\":"<<skill_count<<",\"faeries\":"<<faery_count<<",\"authored_script_bindings\":"<<required<<",\"authored_missing_script_rows\":"<<missing<<",\"session_files\":"<<session_files<<",\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"same_cache_common_backing\":true,\"upstream_owner_release\":true}"<<std::endl;
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
