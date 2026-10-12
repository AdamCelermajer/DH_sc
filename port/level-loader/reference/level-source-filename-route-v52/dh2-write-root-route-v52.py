from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-source-filename-route-v52'
(b/'level_root_filename_route_v52.hpp').write_text('''#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::loader {
struct LevelRootFilenameServicesV52 {
 std::shared_ptr<void> actual_filesystem_owner;
 // Actual Application receiver query, once for each fresh prefixed name.
 std::function<bool(const std::string&,bool& uncompiled,std::string&)> is_using_uncompiled_data;
 // Exact reached FileSystemWin32.openResource boundary: missing is successful
 // found=false, errors are false. Actual mounted namespace returns canonical URI.
 std::function<bool(const std::string&,bool& found,std::string& canonical,std::string&)> open_resource;
};
struct LevelRootFilenameTraceV52 {std::vector<std::string> mode_queries,open_resource_queries;};
class LevelRootFilenameResolverV52 {
 LevelRootFilenameServicesV52 services_;bool busy_{};
public:
 explicit LevelRootFilenameResolverV52(LevelRootFilenameServicesV52 services):services_(std::move(services)){}
 // Only filename-open branch (Level field140==NULL). An assigned generated
 // stream bypasses this original branch; do not resolve its rule as Level XML.
 bool resolve(const std::string& original_C1_name,std::string& canonical,
              LevelRootFilenameTraceV52* trace,std::string& error);
};
// Original320678 scalar body: nonempty source cc/d0 range plus case-sensitive
// substring in [.mlx,.mgp,.mvp,.xml]. Range must be borrowed from SAME Application.
bool original_level_uses_uncompiled_v52(bool actual_range_nonempty,const std::string& name);
} // namespace dh2::loader
''',newline='\n')
(b/'level_root_filename_route_v52.cpp').write_text('''#include "level_root_filename_route_v52.hpp"
#include "resource_paths_v1.hpp"
#include <exception>
namespace dh2::loader {
bool original_level_uses_uncompiled_v52(bool nonempty,const std::string& name){
 if(!nonempty)return false;
 for(const char* token:{".mlx",".mgp",".mvp",".xml"})if(name.find(token)!=std::string::npos)return true;
 return false;
}
bool LevelRootFilenameResolverV52::resolve(const std::string& name,std::string& canonical,
 LevelRootFilenameTraceV52* trace,std::string& error){
 if(busy_){error="Source Level filename resolver reentered on owning runtime thread";return false;}
 struct Guard{bool& b;explicit Guard(bool& flag):b(flag){b=true;}~Guard(){b=false;}} guard(busy_);
 try{
  if(!services_.actual_filesystem_owner||!services_.is_using_uncompiled_data||!services_.open_resource){error="Required actual Application filename mode and filesystem openResource owners";return false;}
  if(name.empty()||name.find('\\0')!=std::string::npos){error="Source Level filename outside proven nonempty C-string domain";return false;}
  const auto compiled=compiled_level_paths_v1(name);LevelRootFilenameTraceV52 next;
  auto attempt=[&](const std::string& candidate,bool& found,std::string& uri){
   next.open_resource_queries.push_back(candidate);uri.clear();found=false;
   if(!services_.open_resource(candidate,found,uri,error))return false;
   if(found&&(uri.empty()||uri.find('\\0')!=std::string::npos)){error="Actual filesystem openResource found invalid canonical identity";return false;}
   return true;
  };
  std::size_t index{};
  for(const char* prefix:{"","data/","data/scene/","data/3d/modules/"}){
   const std::string raw=std::string(prefix)+name;next.mode_queries.push_back(raw);bool uncompiled{};
   if(!services_.is_using_uncompiled_data(raw,uncompiled,error))return false;
   bool found{};std::string uri;
   if(uncompiled){
    if(!attempt(raw,found,uri))return false;
    if(found){canonical=std::move(uri);if(trace)*trace=std::move(next);error.clear();return true;}
   }
   if(!attempt(compiled[index++],found,uri))return false;
   if(found){canonical=std::move(uri);if(trace)*trace=std::move(next);error.clear();return true;}
  }
  if(trace)*trace=std::move(next);
  error="Original Level.LoadFile filename attempts absent from actual filesystem";return false;
 }catch(const std::exception& e){error=e.what();return false;}
}
} // namespace dh2::loader
''',newline='\n')
(r/'level_root_cfs_filename_v52.hpp').write_text('''#pragma once
#include "../production-filesystem-adoption-v50/original_filesystem_fields_v50.hpp"
#include <map>
#include <string>
namespace dh2::loader {
// SAME original initialized CFS ObfuscationMap, not inferred ZIP-name aliases.
using OriginalCfsFilenameMapV52=std::map<std::string,std::string>;
// Original56dc40 filename projection only, ending before fopen/physical I/O.
bool original_cfs_open_filename_v52(const std::string& source_filename,
 const std::string& actual_working_directory,const OriginalCfsFilenameMapV52&,
 std::string& physical_filename,bool& map_applied,std::string& error);
// Exact FileHandle source buffer: root+name, .savegame selects RES_PATH+name,
// original Application.byteA8 controls ASCII lowercase. No ApplyFilenameHacks.
bool original_root_resource_join_v52(const std::string& root,const std::string& name,
 const std::string& actual_RES_PATH,bool application_lowercase_a8,
 std::string& out,std::string& error);
} // namespace dh2::loader
''',newline='\n')
(r/'level_root_cfs_filename_v52.cpp').write_text('''#include "level_root_cfs_filename_v52.hpp"
namespace dh2::loader {
namespace {bool text(const std::string& s){return s.find('\\0')==std::string::npos;}}
bool original_cfs_open_filename_v52(const std::string& source,const std::string& working,
 const OriginalCfsFilenameMapV52& mapping,std::string& out,bool& mapped,std::string& error){
 if(!text(source)||!text(working)){error="Original CFS open requires C-string filename/directory";return false;}
 std::string filename=source;bool applied=false;
 if(!mapping.empty()){
  std::string key=source;if(key.rfind("./",0)==0||key.rfind(".\\\\",0)==0)key.erase(0,2);
  if(!working.empty()&&key.find(working)!=std::string::npos){
   const auto skip=working.size()+((working.back()=='/'||working.back()=='\\\\')?0u:1u);
   if(skip>key.size()){error="Original CFS working-directory strip enters source overread domain";return false;}
   key.erase(0,skip); // Source advances FROM START, not from strstr result.
  }
  for(char& c:key)if(c=='\\\\')c='/';
  auto found=mapping.find(key);if(found!=mapping.end()){filename=found->second;applied=true;}
 }
 if(!text(filename)){error="Original CFS mapped filename outside C-string domain";return false;}
 if(filename.find(':')==std::string::npos&&!working.empty())filename=working+((working.back()=='/'||working.back()=='\\\\')?"":"/")+filename;
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
''',newline='\n')
print('Prepared unique V52 source LoadFile route and downstream scalar kernels.')
