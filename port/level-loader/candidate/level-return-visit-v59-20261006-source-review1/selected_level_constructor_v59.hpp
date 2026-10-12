#pragma once
#include "canonical_level_context_v1.hpp"
#include <algorithm>
#include <cctype>
#include <utility>
namespace dh2::loader {
// Native adapter preflight over existing immutable selected request/table.
// It owns no save/campaign/Level fields and calls no source helper. Original
// Level C1 filename lookup and save constructor remain unchanged.
inline bool validate_selected_level_constructor_v59(
 const CanonicalLevelContextV1& level,const LevelConstructorArgumentsV3& arguments,
 const LevelConstructorServicesV3& services,std::string& error){
 const auto& request=level.source_request();
 if(!arguments.name||!services.levels||services.levels->level_names.size()!=services.levels->levels.size()){
  error="Required selected Level request and SAME complete Arrays::LevelList";return false;
 }
 const auto& names=services.levels->level_names;auto selected=std::find(names.begin(),names.end(),request.identity);
 if(selected==names.end()){error="Selected Level identity missing from actual LevelList";return false;}
 const auto selected_row=static_cast<std::size_t>(selected-names.begin());
 std::size_t original_row=services.levels->levels.size();const std::string name(arguments.name);
 for(std::size_t i=0;i<services.levels->levels.size();++i){const auto& file=services.levels->levels[i].file;
  if(file.size()>1023||file.find('\0')!=std::string::npos){error="Level filename outside original CString1024 domain";return false;}
  std::string lower=file;std::transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char ch){return char(std::tolower(ch));});
  if(name.find(lower)!=std::string::npos){original_row=i;break;}
 }
 if(original_row!=selected_row){error="Selected Level identity differs from original C1 filename row";return false;}
 if(arguments.seed!=request.seed){error="Selected Level seed differs from original C1/save seed";return false;}
 error.clear();return true;
}
// Use only for an explicitly selected authored Level-table request. Existing
// raw C1 remains available for original unmatched-name/diagnostic domains.
inline bool construct_selected_level_v59(
 const std::shared_ptr<CanonicalLevelContextV1>& level,LevelConstructorArgumentsV3 arguments,
 LevelConstructorServicesV3 services,std::string& error){
 if(!level){error="Required SAME selected Level receiver";return false;}
 if(!validate_selected_level_constructor_v59(*level,arguments,services,error))return false;
 return level->construct_source_v3(arguments,std::move(services),error);
}
}