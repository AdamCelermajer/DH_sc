#include "character_script_assets_v1.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
namespace dh2::character {
struct CharacterScriptAssetsV1::Snapshot {
 std::vector<ScriptSessionFile> files;
 std::map<std::string,std::size_t> index;
 std::map<std::string,std::size_t> authored_names;
 std::vector<std::string> missing_scripts;
 data::SkillTables::Borrow skills;
 data::FaeryTables faeries;
 std::size_t bytes{};
};
namespace {
bool script_key(const std::string& p){
 if(p.size()<19||p.compare(0,13,"data/scripts/")||p.compare(p.size()-5,5,".luac"))return false;
 if(p.find('\\')!=std::string::npos||p.find('\0')!=std::string::npos)return false;
 std::size_t start=0;
 while(start<p.size()){
  auto end=p.find('/',start);if(end==std::string::npos)end=p.size();
  auto part=p.substr(start,end-start);
  if(part.empty()||part=="."||part=="..")return false;
  start=end+1;
 }
 return true;
}
std::string canonical(std::string path){
 for(auto& c:path)if(c>='A'&&c<='Z')c=char(c-'A'+'a');
 return path;
}
}
bool CharacterScriptAssetsV1::load(const ScriptAssetServicesV1& services,data::SkillTables::Borrow skills,std::string& error){
 if(snapshot_&&snapshot_.use_count()!=1){error="Script cache reload denied while characters borrow its resources";return false;}
 if(!services.directory||!services.read||!skills){error="Actual cache directory/read and skill owner are required";return false;}
 try{
  auto next=std::make_shared<Snapshot>();next->skills=std::move(skills);
  std::vector<std::string> directory;
  if(!services.directory(directory,error))return false;
  if(directory.size()>65536){error="Script cache directory exceeds native bound";return false;}
  std::set<std::string> seen;
  for(const auto& path:directory){
   if(path.compare(0,13,"data/scripts/")||path.size()<5||path.compare(path.size()-5,5,".luac"))continue;
   if(!script_key(path)||!seen.insert(canonical(path)).second){error="Invalid or duplicate script cache URI: "+path;return false;}
   if(next->files.size()>=4096){error="Script cache count exceeds native bound";return false;}
   bool found=false;std::vector<std::uint8_t> bytes;
   if(!services.read(path,found,bytes,error))return false;
   if(!found||bytes.empty()){error="Listed script cache file missing or empty: "+path;return false;}
   if(bytes.size()>8u*1024u*1024u||next->bytes>32u*1024u*1024u-bytes.size()){error="Script cache bytes exceed native bound";return false;}
   next->bytes+=bytes.size();next->files.push_back({canonical(path),std::move(bytes)});
  }
  std::sort(next->files.begin(),next->files.end(),[](const auto& a,const auto& b){return a.filename<b.filename;});
  for(std::size_t i=0;i<next->files.size();++i)next->index.emplace(next->files[i].filename,i);
  for(const auto* p:{"data/scripts/ai/_commons.luac","data/scripts/skills/_commons.luac"})
   if(!next->index.count(p)){error=std::string("Required source script missing: ")+p;return false;}
  std::array<std::vector<std::uint8_t>,3> raw;
  const char* suffix[]{"_pyarray.bin","_pyarraynames.bin","_pystructnames.bin"};
  for(unsigned i=0;i<3;++i){
   const auto path=std::string("data/pydata/faeries")+suffix[i];bool found=false;
   if(!services.read(path,found,raw[i],error))return false;
   if(!found||raw[i].empty()){error="Required source Faery table missing: "+path;return false;}
  }
  if(!next->faeries.load({raw[0].data(),raw[0].size()},{raw[1].data(),raw[1].size()},{raw[2].data(),raw[2].size()},error))return false;
  auto require=[&](const std::string& script){
   if(script.empty())return true;
   const auto path="data/scripts/skills/"+script+".luac";
   auto found=next->index.find(canonical(path));
   if(!script_key(path)){error="Invalid authored skill/spell script URI: "+path;return false;}
   // Source LoadFile false publishes a null skill/spell slot. Preserve actual
   // cache misses; do not map a class suffix to another script or reject the
   // complete cache because a table names an absent optional instance.
   if(found==next->index.end()){
    if(std::find(next->missing_scripts.begin(),next->missing_scripts.end(),path)==next->missing_scripts.end())next->missing_scripts.push_back(path);
    return true;
   }
   next->authored_names.emplace(path,found->second);
   return true;
  };
  for(const auto& row:next->skills.skills())if(!require(row.script))return false;
  const auto faeries=next->faeries.borrow();
  for(const auto& row:faeries.faeries())if(!require(row.script))return false;
  snapshot_=std::move(next);error.clear();return true;
 }catch(const std::exception& e){error=std::string("Script cache load failed: ")+e.what();return false;}
}
const std::vector<ScriptSessionFile>& CharacterScriptAssetsV1::Borrow::files()const{
 if(!snapshot_)throw std::logic_error("Script cache borrow missing");
 return snapshot_->files;
}
const std::vector<std::uint8_t>* CharacterScriptAssetsV1::Borrow::find(const std::string& path)const{
 if(!snapshot_)return nullptr;
 if(!script_key(path))return nullptr;
 auto i=snapshot_->index.find(canonical(path));
 return i==snapshot_->index.end()?nullptr:&snapshot_->files[i->second].bytes;
}
data::Bytes CharacterScriptAssetsV1::Borrow::common()const{
 auto* bytes=find("data/scripts/ai/_commons.luac");return bytes?data::Bytes{bytes->data(),bytes->size()}:data::Bytes{};
}
bool CharacterScriptAssetsV1::Borrow::session_files(const std::string& external,std::vector<ScriptSessionFile>& out,std::string& error)const{
 if(!snapshot_){error="Script resource borrow missing";return false;}
 if(!external.empty()&&(!script_key(external)||!find(external))){error="Actual external script resource missing: "+external;return false;}
 const auto exclude=canonical(external);
 std::vector<ScriptSessionFile> next;
 std::set<std::string> names;
 auto append=[&](const std::string& path,std::size_t index){
  const auto key=canonical(path);
  if(key=="data/scripts/ai/_commons.luac"||(!exclude.empty()&&key==exclude))return;
  if(names.insert(path).second)next.push_back({path,snapshot_->files[index].bytes});
 };
 for(const auto& item:snapshot_->index)append(item.first,item.second);
 for(const auto& item:snapshot_->authored_names)append(item.first,item.second);
 out=std::move(next);error.clear();return true;
}
data::SkillTables::Borrow CharacterScriptAssetsV1::Borrow::skills()const{return snapshot_?snapshot_->skills:data::SkillTables::Borrow{};}
data::FaeryTables::Borrow CharacterScriptAssetsV1::Borrow::faeries()const{return snapshot_?snapshot_->faeries.borrow():data::FaeryTables::Borrow{};}
const std::vector<std::string>& CharacterScriptAssetsV1::Borrow::missing_scripts()const{
 if(!snapshot_)throw std::logic_error("Script cache borrow missing");
 return snapshot_->missing_scripts;
}
std::size_t CharacterScriptAssetsV1::Borrow::bytes()const noexcept{return snapshot_?snapshot_->bytes:0;}
}
