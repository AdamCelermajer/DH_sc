#include "original_filesystem_fields_v50.hpp"
#include <cstring>
namespace dh2::loader {
std::shared_ptr<OriginalWorkingDirectoryV50> source_filesystem_globals_v50(){
 static auto shared=std::make_shared<OriginalWorkingDirectoryV50>();
 return shared;
}

bool OriginalWorkingDirectoryV50::observe_chdir_result(const std::string& path,int status,
 bool& source_return,std::string& error){
 if(path.find('\0')!=std::string::npos||path.size()>=source_buffer.size()){
  error="Original working-directory path outside proven C-string/1024-byte domain";return false;
 }
 source_return=status==0;
 if(source_return)std::memcpy(source_buffer.data(),path.c_str(),path.size()+1);
 error.clear();return true;
}
bool construct_original_filesystem_paths_v50(const std::string& root,OriginalFileSystemPathsV50& out,std::string& error){
 // _getPaths sprintf uses a 256-byte stack temporary, including data/ and NUL.
 // Reject original overflow domain; do not quietly truncate or relocate it.
 if(root.find('\0')!=std::string::npos||root.size()>250){error="Original RES_PATH outside proven 256-byte trace temporary domain";return false;}
 OriginalFileSystemPathsV50 result;
 auto copy=[&](auto& buffer,const std::string& value){std::memcpy(buffer.data(),value.c_str(),value.size()+1);};
 copy(result.root2c,root);copy(result.copy130,root);copy(result.save43c,root);
 copy(result.trace338,root+"data/"); // resources234 remains actual C1 zeros.
 out=std::move(result);error.clear();return true;
}
} // namespace dh2::loader
