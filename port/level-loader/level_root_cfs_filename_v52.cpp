#include "level_root_cfs_filename_v52.hpp"
namespace dh2::loader {
namespace {bool text(const std::string& s){return s.find('\0')==std::string::npos;}}
bool original_cfs_open_filename_v52(const std::string& source,const std::string& working,
 const OriginalCfsFilenameMapV52& mapping,std::string& out,bool& mapped,std::string& error){
 if(!text(source)||!text(working)){error="Original CFS open requires C-string filename/directory";return false;}
 std::string filename=source;bool applied=false;
 if(!mapping.empty()){
  std::string key=source;if(key.rfind("./",0)==0||key.rfind(".\\",0)==0)key.erase(0,2);
  if(!working.empty()&&key.find(working)!=std::string::npos){
   const auto skip=working.size()+((working.back()=='/'||working.back()=='\\')?0u:1u);
   if(skip>key.size()){error="Original CFS working-directory strip enters source overread domain";return false;}
   key.erase(0,skip); // Source advances FROM START, not from strstr result.
  }
  for(char& c:key)if(c=='\\')c='/';
  auto found=mapping.find(key);if(found!=mapping.end()){filename=found->second;applied=true;}
 }
 if(!text(filename)){error="Original CFS mapped filename outside C-string domain";return false;}
 if(filename.find(':')==std::string::npos&&!working.empty())filename=working+((working.back()=='/'||working.back()=='\\')?"":"/")+filename;
 out=std::move(filename);mapped=applied;error.clear();return true;
}
bool original_root_resource_join_v52(const std::string& root,const std::string& name,
 const std::string& resource,bool lower,std::string& out,std::string& error){
 if(!text(root)||!text(name)||!text(resource)){error="Original resource path requires C-string domain";return false;}
 std::string path=(name.find(".savegame")!=std::string::npos?resource:root)+name;
 if(path.size()>259){error="Original resource path outside proven FileHandle260 buffer domain";return false;}
 if(lower)for(char& c:path)if(c>='A'&&c<='Z')c=static_cast<char>(c+32);
 out=std::move(path);error.clear();return true;
}
} // namespace dh2::loader
