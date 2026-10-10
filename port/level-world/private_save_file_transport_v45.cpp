#include "private_save_file_transport_v45.hpp"
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <io.h>
#include <filesystem>
#else
#include <unistd.h>
#endif
#include <stdexcept>
namespace dh2::level {
#ifdef _WIN32
namespace {
std::wstring native_path(const std::string& path){return std::filesystem::u8path(path).wstring();}
bool regular_handle(HANDLE handle,std::uint64_t budget,std::uint64_t& size,std::string& error){
 BY_HANDLE_FILE_INFORMATION info{};
 if(GetFileType(handle)!=FILE_TYPE_DISK||!GetFileInformationByHandle(handle,&info)||
    (info.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))){
  error="Actual private source save requires a regular non-reparse disk file";return false;
 }
 size=(std::uint64_t(info.nFileSizeHigh)<<32)|info.nFileSizeLow;
 if(size>budget){error="Actual source save exceeds explicit transport budget";return false;}
 return true;
}
}
#endif
PrivateSaveFileTransportV45::PrivateSaveFileTransportV45(std::string dir,std::uint64_t budget):directory_(std::move(dir)),budget_(budget){
#ifdef _WIN32
 if(directory_.empty()||directory_.find('\0')!=std::string::npos||!std::filesystem::u8path(directory_).is_absolute()||!budget_||budget_>UINT32_MAX)throw std::invalid_argument("Required actual absolute Windows files directory and explicit save byte budget");
#else
 if(directory_.empty()||directory_[0]!='/'||!budget_||budget_>UINT32_MAX)throw std::invalid_argument("Required actual absolute Android files directory and explicit save byte budget");
#endif
}
PrivateSaveFileTransportV45::~PrivateSaveFileTransportV45(){for(auto& p:open_)std::fclose(p.second);}
bool PrivateSaveFileTransportV45::path(const std::string& name,std::string& out,std::string& e)const{
 if(name.empty()||name=="."||name==".."||name.find_first_of("/\\")!=std::string::npos||name.find('\0')!=std::string::npos){e="Invalid private source save filename";return false;}
#ifdef _WIN32
 // Alternate streams and Win32 trailing-dot/space aliases are not source filenames.
 if(name.find_first_of(":*?\"<>|")!=std::string::npos||name.back()=='.'||name.back()==' '){e="Invalid private Windows source save filename";return false;}
#endif
 out=directory_+"/"+name;return true;
}
bool PrivateSaveFileTransportV45::source_exists_v115(const std::string& name,bool& found,std::string& e){
 std::lock_guard<std::recursive_mutex> lock(io_mutex_v102_);found=false;e.clear();std::string p;
 if(!path(name,p,e))return false;
#ifdef _WIN32
 const auto handle=CreateFileW(native_path(p).c_str(),FILE_READ_ATTRIBUTES,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
 if(handle==INVALID_HANDLE_VALUE){const auto code=GetLastError();if(code==ERROR_FILE_NOT_FOUND||code==ERROR_PATH_NOT_FOUND)return true;e="Actual source save existence Win32 error "+std::to_string(code);return false;}
 std::uint64_t size{};const bool valid=regular_handle(handle,UINT64_MAX,size,e);CloseHandle(handle);if(!valid)return false;
#else
 struct stat info{};
 if(::lstat(p.c_str(),&info)){if(errno==ENOENT||errno==ENOTDIR)return true;e="Actual source save existence errno "+std::to_string(errno);return false;}
 if(!S_ISREG(info.st_mode)){e="Actual private source save requires regular file";return false;}
#endif
 found=true;return true;
}
bool PrivateSaveFileTransportV45::read(void* raw,const std::string& name,bool& found,std::vector<std::uint8_t>& out,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::lock_guard<std::recursive_mutex> lock(s.io_mutex_v102_);found=false;std::string p;if(!s.path(name,p,e))return false;
#ifdef _WIN32
 const auto handle=CreateFileW(native_path(p).c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
 if(handle==INVALID_HANDLE_VALUE){const auto code=GetLastError();if(code==ERROR_FILE_NOT_FOUND){out.clear();return true;}e="Actual source save read Win32 error "+std::to_string(code);return false;}
 struct Guard{HANDLE handle;~Guard(){CloseHandle(handle);}}guard{handle};found=true;std::uint64_t size{};
 if(!regular_handle(handle,s.budget_,size,e))return false;
 std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));std::size_t at=0;
 while(at<bytes.size()){DWORD count{};const auto request=static_cast<DWORD>(bytes.size()-at);if(!ReadFile(handle,bytes.data()+at,request,&count,nullptr)||!count){e="Actual source save short read";return false;}at+=count;}
 std::uint8_t extra{};DWORD count{};if(!ReadFile(handle,&extra,1,&count,nullptr)||count){e="Actual source save grew or read failed";return false;}
#else
 const int fd=::open(p.c_str(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK);if(fd<0){if(errno==ENOENT){out.clear();return true;}e="Actual source save read open errno "+std::to_string(errno);return false;}
 struct Guard{int fd;~Guard(){::close(fd);}}guard{fd};found=true;struct stat info{};
 if(::fstat(fd,&info)||!S_ISREG(info.st_mode)||info.st_size<0||std::uint64_t(info.st_size)>s.budget_){e="Actual source save requires regular file within explicit transport budget";return false;}
 std::vector<std::uint8_t> bytes(std::size_t(info.st_size));std::size_t at=0;
 while(at<bytes.size()){const auto n=::read(fd,bytes.data()+at,bytes.size()-at);if(n<=0){e="Actual source save short read";return false;}at+=std::size_t(n);}
 std::uint8_t extra{};if(::read(fd,&extra,1)!=0){e="Actual source save grew or read failed";return false;}
#endif
 out=std::move(bytes);return true;
}
bool PrivateSaveFileTransportV45::backup(void* raw,const std::string& from,const std::string& to,bool& result,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::lock_guard<std::recursive_mutex> lock(s.io_mutex_v102_);std::string a,b;if(!s.path(from,a,e)||!s.path(to,b,e)||to!=from+".bak"){e="Required exact source .bak destination";return false;}
 // Source removes prior .bak then renames primary. OS operation result is a
 // source response, not fabricated success and not a new temporary-file scheme.
#ifdef _WIN32
 if(!DeleteFileW(native_path(b).c_str())&&GetLastError()!=ERROR_FILE_NOT_FOUND){result=false;return true;}
 result=MoveFileW(native_path(a).c_str(),native_path(b).c_str())!=0;return true;
#else
 if(::unlink(b.c_str())&&errno!=ENOENT){result=false;return true;}
 result=::rename(a.c_str(),b.c_str())==0;return true;
#endif
}
bool PrivateSaveFileTransportV45::open(void* raw,const std::string& name,void*& out,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::lock_guard<std::recursive_mutex> lock(s.io_mutex_v102_);out=nullptr;std::string p;if(!s.path(name,p,e))return false;
#ifdef _WIN32
 // Validate the opened receiver before truncating: CREATE_ALWAYS would truncate
 // an existing target before its regular-file/reparse policy could be checked.
 const auto handle=CreateFileW(native_path(p).c_str(),GENERIC_READ|GENERIC_WRITE,0,nullptr,OPEN_ALWAYS,FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
 if(handle==INVALID_HANDLE_VALUE){e="Actual source save write Win32 error "+std::to_string(GetLastError());return false;}
 std::uint64_t size{};
 if(!regular_handle(handle,UINT64_MAX,size,e)){CloseHandle(handle);return false;}
 const int fd=::_open_osfhandle(reinterpret_cast<std::intptr_t>(handle),_O_RDWR|_O_BINARY);
 if(fd<0){CloseHandle(handle);e="Actual source save CRT handle adoption failed";return false;}
 if(::_chsize_s(fd,0)){::_close(fd);e="Actual source save truncate failed";return false;}
 auto* file=::_fdopen(fd,"w+b");if(!file){::_close(fd);e="Actual source save fdopen failed";return false;}
#else
 const int fd=::open(p.c_str(),O_RDWR|O_CREAT|O_TRUNC|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK,0600);
 if(fd<0){e="Actual source save write open errno "+std::to_string(errno);return false;}
 struct stat info{};if(::fstat(fd,&info)||!S_ISREG(info.st_mode)){::close(fd);e="Actual source save write requires regular file";return false;}
 auto* file=::fdopen(fd,"w+b");if(!file){::close(fd);e="Actual source save fdopen failed";return false;}
#endif
 out=file;s.open_.emplace(out,file);return true;
}
bool PrivateSaveFileTransportV45::write(void* raw,void* handle,data::Bytes bytes,std::uint64_t& written,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::lock_guard<std::recursive_mutex> lock(s.io_mutex_v102_);auto p=s.open_.find(handle);written=0;
 if(p==s.open_.end()||(!bytes.data&&bytes.size)){e="Required actual retained source save stream";return false;}
#ifdef _WIN32
 const auto offset=::_ftelli64(p->second);
#else
 const auto offset=::ftello(p->second);
#endif
 if(offset<0||std::uint64_t(offset)>s.budget_||bytes.size>s.budget_-std::uint64_t(offset)){e="Source save write exceeds explicit transport budget";return false;}
 written=std::fwrite(bytes.data,1,bytes.size,p->second);if(std::ferror(p->second)){e="Actual source save write I/O error";return false;}return true;
}
bool PrivateSaveFileTransportV45::seek(void* raw,void* handle,std::uint64_t pos,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::lock_guard<std::recursive_mutex> lock(s.io_mutex_v102_);auto p=s.open_.find(handle);
 if(p==s.open_.end()||pos>s.budget_){e="Actual source save seek failed";return false;}
#ifdef _WIN32
 const auto result=::_fseeki64(p->second,static_cast<std::int64_t>(pos),SEEK_SET);
#else
 const auto result=::fseeko(p->second,off_t(pos),SEEK_SET);
#endif
 if(result){e="Actual source save seek failed";return false;}return true;
}
bool PrivateSaveFileTransportV45::close(void* raw,void*& handle,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::lock_guard<std::recursive_mutex> lock(s.io_mutex_v102_);auto p=s.open_.find(handle);if(p==s.open_.end()){e="Required actual source save close handle";return false;}
 auto* file=p->second;s.open_.erase(p);handle=nullptr;if(std::fclose(file)){e="Actual source save close failed";return false;}return true;
}
bool PrivateSaveFileTransportV45::failure(void*,const char* why,std::string& e){e=std::string("Required source save assertion: ")+(why?why:"");return false;}
SavegameFileServicesV2 PrivateSaveFileTransportV45::services(){return {this,read,backup,open,write,seek,close,failure,shared_from_this()};}
}
