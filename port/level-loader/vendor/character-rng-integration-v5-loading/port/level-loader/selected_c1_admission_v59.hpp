#pragma once
#include "level_preparation_v1.hpp"
#include "level_constructor_v3.hpp"
#include <algorithm>
#include <cctype>
#include <array>
#include <cstring>
namespace dh2::loader {
// Optional native admission policy for an authored selected context. No Level,
// Save, campaign, helper, RNG or source field mutation occurs here. Raw seed0
// remains valid: it must match raw constructor0 and is forwarded unchanged.
inline bool validate_selected_c1_admission_v59(
 const LevelSourceRequestV1& request,const LevelConstructorArgumentsV3& arguments,
 const LevelConstructorServicesV3& services,std::string& error){
 if(!arguments.name||!services.levels||services.levels->level_names.size()!=services.levels->levels.size()){
  error="Required selected Level request and SAME complete Arrays::LevelList";return false;
 }
 const auto& names=services.levels->level_names;auto selected=std::find(names.begin(),names.end(),request.identity);
 if(selected==names.end()){error="Selected Level identity missing from actual LevelList";return false;}
 const auto selected_row=static_cast<std::size_t>(selected-names.begin());
 std::size_t original_row=services.levels->levels.size();std::array<char,1024> lower{};
 for(std::size_t i=0;i<services.levels->levels.size();++i){const auto& file=services.levels->levels[i].file;
  if(file.size()>1023||file.find('\0')!=std::string::npos){error="Level filename outside original CString1024 domain";return false;}
  std::transform(file.begin(),file.end(),lower.begin(),[](unsigned char ch){return char(std::tolower(ch));});lower[file.size()]=0;
  if(std::strstr(arguments.name,lower.data())){original_row=i;break;}
 }
 if(original_row!=selected_row){error="Selected Level identity differs from original C1 filename row";return false;}
 if(arguments.seed!=request.seed){error="Selected Level seed differs from original C1/save seed";return false;}
 error.clear();return true;
}
}