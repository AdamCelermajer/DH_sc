#include "private_save_file_transport_v45.hpp"
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
namespace dh2::level {
PrivateSaveFileTransportV45::PrivateSaveFileTransportV45(std::string dir,std::uint64_t budget):directory_(std::move(dir)),budget_(budget){
 if(directory_.empty()||directory_[0]!='/'||!budget_||budget_>UINT32_MAX)throw std::invalid_argument("Required actual absolute Android files directory and explicit save byte budget");
}
PrivateSaveFileTransportV45::~PrivateSaveFileTransportV45(){for(auto& p:open_)std::fclose(p.second);}
bool PrivateSaveFileTransportV45::path(const std::string& name,std::string& out,std::string& e)const{
 if(name.empty()||name=="."||name==".."||name.find_first_of("/\\")!=std::string::npos||name.find('\0')!=std::string::npos){e="Invalid private source save filename";return false;}
 out=directory_+"/"+name;return true;
}
bool PrivateSaveFileTransportV45::read(void* raw,const std::string& name,bool& found,std::vector<std::uint8_t>& out,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);found=false;std::string p;if(!s.path(name,p,e))return false;
 const int fd=::open(p.c_str(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK);if(fd<0){if(errno==ENOENT){out.clear();return true;}e="Actual source save read open errno "+std::to_string(errno);return false;}
 struct Guard{int fd;~Guard(){::close(fd);}}guard{fd};found=true;struct stat info{};
 if(::fstat(fd,&info)||!S_ISREG(info.st_mode)||info.st_size<0||std::uint64_t(info.st_size)>s.budget_){e="Actual source save requires regular file within explicit transport budget";return false;}
 std::vector<std::uint8_t> bytes(std::size_t(info.st_size));std::size_t at=0;
 while(at<bytes.size()){const auto n=::read(fd,bytes.data()+at,bytes.size()-at);if(n<=0){e="Actual source save short read";return false;}at+=std::size_t(n);}
 std::uint8_t extra{};if(::read(fd,&extra,1)!=0){e="Actual source save grew or read failed";return false;}
 out=std::move(bytes);return true;
}
bool PrivateSaveFileTransportV45::backup(void* raw,const std::string& from,const std::string& to,bool& result,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);std::string a,b;if(!s.path(from,a,e)||!s.path(to,b,e)||to!=from+".bak"){e="Required exact source .bak destination";return false;}
 // Source removes prior .bak then renames primary. OS operation result is a
 // source response, not fabricated success and not a new temporary-file scheme.
 if(::unlink(b.c_str())&&errno!=ENOENT){result=false;return true;}
 result=::rename(a.c_str(),b.c_str())==0;return true;
}
bool PrivateSaveFileTransportV45::open(void* raw,const std::string& name,void*& out,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);out=nullptr;std::string p;if(!s.path(name,p,e))return false;
 const int fd=::open(p.c_str(),O_RDWR|O_CREAT|O_TRUNC|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK,0600);
 if(fd<0){e="Actual source save write open errno "+std::to_string(errno);return false;}
 struct stat info{};if(::fstat(fd,&info)||!S_ISREG(info.st_mode)){::close(fd);e="Actual source save write requires regular file";return false;}
 auto* file=::fdopen(fd,"w+b");if(!file){::close(fd);e="Actual source save fdopen failed";return false;}
 out=file;s.open_.emplace(out,file);return true;
}
bool PrivateSaveFileTransportV45::write(void* raw,void* handle,data::Bytes bytes,std::uint64_t& written,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);auto p=s.open_.find(handle);written=0;
 if(p==s.open_.end()||(!bytes.data&&bytes.size)){e="Required actual retained source save stream";return false;}
 const auto offset=::ftello(p->second);if(offset<0||std::uint64_t(offset)>s.budget_||bytes.size>s.budget_-std::uint64_t(offset)){e="Source save write exceeds explicit transport budget";return false;}
 written=std::fwrite(bytes.data,1,bytes.size,p->second);if(std::ferror(p->second)){e="Actual source save write I/O error";return false;}return true;
}
bool PrivateSaveFileTransportV45::seek(void* raw,void* handle,std::uint64_t pos,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);auto p=s.open_.find(handle);
 if(p==s.open_.end()||pos>s.budget_||::fseeko(p->second,off_t(pos),SEEK_SET)){e="Actual source save seek failed";return false;}return true;
}
bool PrivateSaveFileTransportV45::close(void* raw,void*& handle,std::string& e){
 auto& s=*static_cast<PrivateSaveFileTransportV45*>(raw);auto p=s.open_.find(handle);if(p==s.open_.end()){e="Required actual source save close handle";return false;}
 auto* file=p->second;s.open_.erase(p);handle=nullptr;if(std::fclose(file)){e="Actual source save close failed";return false;}return true;
}
bool PrivateSaveFileTransportV45::failure(void*,const char* why,std::string& e){e=std::string("Required source save assertion: ")+(why?why:"");return false;}
SavegameFileServicesV2 PrivateSaveFileTransportV45::services(){return {this,read,backup,open,write,seek,close,failure,shared_from_this()};}
}
