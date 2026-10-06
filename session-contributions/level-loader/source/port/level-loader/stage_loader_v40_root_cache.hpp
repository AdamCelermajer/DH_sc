#pragma once
#include <canonical_cached_file_v1.hpp>
#include <string>
#include <vector>
namespace dh2::loader {
struct RootCacheAliasV40 {std::string source_prefix,cache_prefix;};
struct RootCacheResolutionV40 {std::string raw_name,cache_uri,rule;};
// Explicit modern packaging adapter, not a recovered Level.LoadFile rule.
// Resolves only an exact normalized archive entry or caller-declared prefix
// alias. No basename search, request.definition substitution or missing success.
inline bool resolve_root_cache_v40(const assets::ZipAssetPackV1& archive,
 const std::string& raw,const std::vector<RootCacheAliasV40>& aliases,
 RootCacheResolutionV40& out,std::string& error){
 std::string key;if(!archive.mounted()||!assets::ZipAssetPackV1::key(raw,key,error)){
  if(error.empty())error="Required mounted original root archive";return false;
 }
 const auto entries=archive.entries();RootCacheResolutionV40 next{raw,{},{}};
 auto choose=[&](const std::string& candidate,const std::string& rule){
  for(const auto& entry:entries){std::string existing,why;
   if(!assets::ZipAssetPackV1::key(entry.uri,existing,why)||existing!=candidate)continue;
   if(!next.cache_uri.empty()&&next.cache_uri!=entry.uri){error="Ambiguous explicit root cache aliases";return false;}
   next.cache_uri=entry.uri;next.rule=rule;
  }return true;
 };
 if(!choose(key,"exact archive URI"))return false;
 // Exact entry takes precedence over packaging aliases.
 if(next.cache_uri.empty())for(const auto& alias:aliases){
  std::string from,to,why;
  if(alias.source_prefix.empty()||alias.cache_prefix.empty()||alias.source_prefix.back()!='/'||alias.cache_prefix.back()!='/'){
   error="Require explicit directory-prefix root alias";return false;
  }
  if(!assets::ZipAssetPackV1::key(alias.source_prefix+"__alias__",from,why)||!assets::ZipAssetPackV1::key(alias.cache_prefix+"__alias__",to,why)){
   error="Invalid explicit root cache prefix";return false;
  }
  from.resize(from.size()-9);to.resize(to.size()-9);
  if(key.compare(0,from.size(),from)==0&&!choose(to+key.substr(from.size()),alias.source_prefix+" -> "+alias.cache_prefix))return false;
 }
 if(next.cache_uri.empty()){error="Required original root archive entry for raw name: "+raw;return false;}
 out=std::move(next);error.clear();return true;
}
}
