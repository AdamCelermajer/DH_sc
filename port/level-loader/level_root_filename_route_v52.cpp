#include "level_root_filename_route_v52.hpp"
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
  if(name.empty()||name.find('\0')!=std::string::npos){error="Source Level filename outside proven nonempty C-string domain";return false;}
  const auto compiled=compiled_level_paths_v1(name);LevelRootFilenameTraceV52 next;
  auto attempt=[&](const std::string& candidate,bool& found,std::string& uri){
   next.open_resource_queries.push_back(candidate);uri.clear();found=false;
   if(!services_.open_resource(candidate,found,uri,error))return false;
   if(found&&(uri.empty()||uri.find('\0')!=std::string::npos)){error="Actual filesystem openResource found invalid canonical identity";return false;}
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
