#include "original_filename_policy_v49.hpp"
namespace dh2::loader {
namespace {bool c_string(const std::string& s){return s.find('\0')==std::string::npos;}}
bool original_filename_policy_v49(const std::string& working,const std::string& input,
 OriginalFilenameResultV49& output,std::string& error){
 if(!c_string(working)||!c_string(input)){error="Original filename policy requires C-string domain";return false;}
 OriginalFilenameResultV49 result;result.input=input;result.working_directory=working;
 const auto pos=input.find(working);const bool found=pos!=std::string::npos;
 std::size_t cut=0;
 if(found){
  cut=pos+working.size();
  if(cut==input.size()){error="Original filename policy end-match enters source overread domain";return false;}
  ++cut;
 }
 result.working_directory_matched=found;std::string suffix=found?input.substr(cut):input;
 result.texture_basename_relocated=suffix.find(".tga")!=std::string::npos;
 if(result.texture_basename_relocated){
  const auto slash=suffix.find_last_of("/\\");
  // Exact 16 bytes at8c0740; prefix restored later from original input.
  suffix="ata/3d/textures/"+suffix.substr(slash==std::string::npos?0:slash+1);
 }
 for(char& c:suffix)if(c>='A'&&c<='Z')c=static_cast<char>(c+32);
 result.filename=(found?input.substr(0,cut):std::string{})+suffix;
 output=std::move(result);error.clear();return true;
}
bool borrowed_original_filename_policy_v49(const OriginalFilesystemBorrowV49& fs,
 const std::string& input,OriginalFilenameResultV49& output,std::string& error){
 if(!fs.actual_filesystem_owner||!fs.borrow_working_directory){error="Required actual filesystem working-directory owner/provider";return false;}
 std::string working,after;if(!fs.borrow_working_directory(working,error))return false;
 OriginalFilenameResultV49 result;if(!original_filename_policy_v49(working,input,result,error))return false;
 if(!fs.borrow_working_directory(after,error))return false;
 if(after!=working){error="Actual filesystem working-directory changed during synchronous filename borrow";return false;}
 output=std::move(result);error.clear();return true;
}
bool original_image_search_candidates_v49(const std::string& directory,const std::string& authored,
 std::array<std::string,2>& output,std::string& error){
 if(!c_string(directory)||!c_string(authored)){error="Original image search requires C-string domain";return false;}
 output={directory+"/"+authored,authored};error.clear();return true;
}
} // namespace dh2::loader
