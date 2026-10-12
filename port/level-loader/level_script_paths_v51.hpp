#pragma once
#include <algorithm>
#include <cstdint>
#include <string>
namespace dh2::loader {
struct LevelScriptPathsV51 {bool reached{};std::string command_file,name_file;};
// Original Level._LoadScriptFile3f38a0 reverse-searches the LAST exact,
// case-sensitive .pyscript marker. It replaces the ENTIRE suffix from it.
// Only a reached marker mutates the SAME input string's backslashes.
inline LevelScriptPathsV51 level_script_paths_v51(std::string& actual_config_script150){
 LevelScriptPathsV51 out;if(actual_config_script150.size()<=8)return out;
 const auto pos=actual_config_script150.rfind(".pyscript");if(pos==std::string::npos)return out;
 std::replace(actual_config_script150.begin(),actual_config_script150.end(),'\\','/');
 out.reached=true;out.command_file=actual_config_script150.substr(0,pos)+"_pyscripts.bin";out.name_file=actual_config_script150.substr(0,pos)+"_pyscriptnames.bin";return out;
}
inline constexpr const char* common_command_file_v51="data/pydata/scripts_pyscripts.bin";
inline constexpr const char* common_command_names_v51="data/pydata/scripts_pyscriptnames.bin";
}
