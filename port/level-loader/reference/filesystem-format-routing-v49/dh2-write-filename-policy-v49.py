from pathlib import Path
import json
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/filesystem-format-routing-v49'
(b/'original_filename_policy_v49.hpp').write_text('''#pragma once
#include <functional>
#include <memory>
#include <string>
#include <array>
namespace dh2::loader {
struct OriginalFilenameResultV49 {
 std::string input,working_directory,filename;
 bool working_directory_matched{},texture_basename_relocated{};
};
// Exact source projection of 34e96c; working-directory is an actual FS virtual
// producer, not q:/iphone or GPU policy. Embedded NUL/undefined end-match reject.
bool original_filename_policy_v49(const std::string& working_directory,
 const std::string& input,OriginalFilenameResultV49&,std::string& error);
struct OriginalFilesystemBorrowV49 {
 std::shared_ptr<void> actual_filesystem_owner;
 std::function<bool(std::string& working_directory,std::string& error)> borrow_working_directory;
};
// Call synchronously on the FS owner's runtime thread; working-directory must
// remain unchanged throughout the borrow. No detached current-directory cache.
bool borrowed_original_filename_policy_v49(const OriginalFilesystemBorrowV49&,
 const std::string& input,OriginalFilenameResultV49&,std::string& error);
// CResFactory6594a0 order. Preserve duplicates and exact authored path. Caller
// must try second only if actual first open fails; no format variant added here.
bool original_image_search_candidates_v49(const std::string& resource_directory,
 const std::string& full_authored_image_path,std::array<std::string,2>&,
 std::string& error);
} // namespace dh2::loader
''',newline='\n')
(b/'original_filename_policy_v49.cpp').write_text('''#include "original_filename_policy_v49.hpp"
namespace dh2::loader {
namespace {bool c_string(const std::string& s){return s.find('\\0')==std::string::npos;}}
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
  const auto slash=suffix.find_last_of("/\\\\");
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
''',newline='\n')
(r/'filesystem-provider-v49.hpp.proposed').write_text('''// Proposal only: main must bind the actual retained filesystem owner/thread.
// Existing OriginalCacheAssets ZIP case/slash lookup does not by itself produce
// FileSystemBase WorkingDirectory, CFS ObfuscationMap, or a texture-format policy.
struct RequiredOriginalFilesystemImagePolicyV49 {
 std::shared_ptr<void> actual_filesystem_owner;
 // Actual virtual CFileSystem::getWorkingDirectory (+0x2c from address point).
 std::function<bool(std::string&,std::string&)> working_directory;
 // Optional exact CFS ObfuscationMap service56e064, SAME actual initialized map:
 // path slash conversion/removal follows its original map key rules. Empty map
 // preserves original input. No pvr2_ entries may be synthesized here.
 std::function<bool(const std::string&,std::string&,std::string&)> obfuscated_name;
 // Original CResFactory resource-directory+/+SImage.path then SImage.path.
 // Open returns real canonical identity and a metadata/resource handle, BEFORE
 // V45 reserves/reads payload. It must consume actual filename-policy result.
 std::function<bool(const std::string&,bool&,std::string&,std::string&)> lookup_actual_mount;
};
// Texture-format selection is intentionally NOT an enum/guess in this proposal:
// no original producer selecting pvr2_ from GPU/config fields has been established.
// Main must supply such a source-proven service before resolving that domain.
''',newline='\n')
print('Created unique V49 source kernel and filesystem provider proposal.')
