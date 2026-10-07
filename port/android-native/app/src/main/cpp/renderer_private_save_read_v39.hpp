#pragma once
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstdint>
#include <exception>
#include <limits>
#include <string>
#include <vector>
#include <utility>
#include <sys/stat.h>
#include <fcntl.h>
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
namespace dh2::android_ui {
// Explicit application transport policy, NOT a recovered native format maximum.
// Source StreamBuffer/ProfileIndex permit a UINT32_MAX complete byte domain and
// variable OBJS virtual Save bodies. No whole-game16MiB adequacy is claimed.
inline constexpr std::uint64_t level_save_transport_budget_v39=16u*1024u*1024u;
inline bool read_private_level_save_v39(const std::string& directory,const std::string& name,
    std::uint64_t budget,bool& found,std::vector<std::uint8_t>& bytes,std::string& error){
 found=false;bytes.clear();
 if(name.empty()||name.find_first_of("/\\")!=std::string::npos||name.find('\0')!=std::string::npos){error="Invalid source LevelSavegame private filename";return false;}
 if(!budget||budget>std::numeric_limits<std::uint32_t>::max()){error="Invalid explicit LevelSavegame byte budget";return false;}
 const auto path=directory+"/"+name;errno=0;
#if defined(_WIN32)
 const int fd=::_open(path.c_str(),_O_RDONLY|_O_BINARY|_O_NOINHERIT);
#else
 // Nonblocking open avoids waiting on a non-regular private filename before
 // fstat can reject it. O_NONBLOCK has no effect on a genuine regular save.
 const int fd=::open(path.c_str(),O_RDONLY|O_NONBLOCK|O_CLOEXEC);
#endif
 if(fd<0){if(errno==ENOENT){error.clear();return true;}error="Actual LevelSavegame file open failed: "+std::to_string(errno);return false;}
 found=true;
 struct Descriptor{int value;~Descriptor(){if(value>=0){
#if defined(_WIN32)
 ::_close(value);
#else
 ::close(value);
#endif
 }}} descriptor{fd};
#if defined(_WIN32)
 struct _stat64 info{};const int stat_status=::_fstat64(fd,&info);const bool regular=stat_status==0&&(info.st_mode&_S_IFMT)==_S_IFREG;
#else
 struct stat info{};const int stat_status=::fstat(fd,&info);const bool regular=stat_status==0&&S_ISREG(info.st_mode);
#endif
 if(stat_status||!regular||info.st_size<0){error="Actual LevelSavegame bounded read requires a regular file/stat";return false;}
 const auto length=static_cast<std::uint64_t>(info.st_size);
 if(length>budget){error="Actual LevelSavegame file exceeds explicit byte budget: "+std::to_string(length)+" > "+std::to_string(budget);return false;}
#if defined(_WIN32)
 auto* file=::_fdopen(fd,"rb");
#else
 auto* file=::fdopen(fd,"rb");
#endif
 if(!file){error="Actual LevelSavegame file stream failed: "+std::to_string(errno);return false;}
 descriptor.value=-1;
 struct Stream{std::FILE* value;~Stream(){if(value)std::fclose(value);}} stream{file};
 try{
  // Allocate exactly the accepted immutable-size snapshot, never a doubling
  // vector to EOF. A growing/shrinking file is a required failure, not truncation.
  std::vector<std::uint8_t> candidate(static_cast<std::size_t>(length));std::size_t done=0;
  while(done<candidate.size()){
   const auto request=std::min<std::size_t>(8192,candidate.size()-done);
   const auto count=std::fread(candidate.data()+done,1,request,file);
   if(!count){error="Actual LevelSavegame file changed/short read or I/O failure";return false;}done+=count;
  }
  const int extra=std::fgetc(file);
  if(extra!=EOF){error="Actual LevelSavegame file grew during bounded read";return false;}
  if(std::ferror(file)){error="Actual LevelSavegame private file read failed";return false;}
  stream.value=nullptr;
  if(std::fclose(file)){error="Actual LevelSavegame private file close failed";return false;}
  bytes=std::move(candidate);error.clear();return true;
 }catch(const std::exception& e){error=std::string("Actual LevelSavegame bounded read allocation: ")+e.what();return false;}
}
}
