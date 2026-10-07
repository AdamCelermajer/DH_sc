#include "campaign_profile_files_v1.hpp"
#include <cerrno>
#include <cstdio>
#include <memory>
#include <sys/stat.h>
#include <utility>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
namespace dh2::data {
namespace {
bool filename(const std::string& dir,std::uint32_t slot,std::string& path,std::string& error){
    if(slot>=4||dir.empty()){error="Invalid campaign slot or directory";return false;}
    char name[32];std::snprintf(name,sizeof(name),"dh2_%03u.savegame",slot);path=dir+"/"+name;return true;
}
enum class Read {ok,missing,error};
struct Close {void operator()(FILE* f)const{if(f)std::fclose(f);}};
Read read(const std::string& path,std::vector<std::uint8_t>& bytes,std::string& error){
    struct stat info{};
    if(stat(path.c_str(),&info)){if(errno==ENOENT)return Read::missing;error="Cannot inspect campaign: "+path;return Read::error;}
    if((info.st_mode&S_IFMT)!=S_IFREG||info.st_size<0||info.st_size>32*1024*1024){error="Unsafe campaign file size/type: "+path;return Read::error;}
    std::unique_ptr<FILE,Close> file(std::fopen(path.c_str(),"rb"));
    if(!file){error="Cannot open campaign: "+path;return Read::error;}
    bytes.resize(static_cast<std::size_t>(info.st_size));
    if(!bytes.empty()&&std::fread(bytes.data(),1,bytes.size(),file.get())!=bytes.size()){error="Short campaign read: "+path;return Read::error;}
    // Reject a concurrent size change rather than publish a mixed snapshot.
    if(std::fgetc(file.get())!=EOF||std::ferror(file.get())){error="Campaign changed during read: "+path;return Read::error;}
    return Read::ok;
}
bool usable_header(const std::vector<std::uint8_t>& bytes){
    return bytes.size()>3&&!(bytes[0]==255&&bytes[1]==255&&bytes[2]==255&&bytes[3]==255);
}
}
bool campaign_profile_exists_v1(const std::string& directory,std::uint32_t slot,bool& occupied,std::string& error){
    std::string path;if(!filename(directory,slot,path,error))return false;
    bool result=false;
    for(const auto& suffix:{std::string(),std::string(".bak")}){
        struct stat info{};
        if(!stat((path+suffix).c_str(),&info)){result=true;break;}
        if(errno!=ENOENT){error="Cannot inspect campaign: "+path+suffix;return false;}
    }
    occupied=result;error.clear();return true;
}
bool read_campaign_profile_v1(const std::string& directory,std::uint32_t slot,
    CampaignProfileFileV1& output,std::string& error){
    std::string path;if(!filename(directory,slot,path,error))return false;
    CampaignProfileFileV1 candidate;
    const auto base=read(path,candidate.bytes,error);
    if(base==Read::error)return false;
    if(base==Read::missing||!usable_header(candidate.bytes)){
        candidate.origin=CampaignProfileOriginV1::backup;
        const auto backup=read(path+".bak",candidate.bytes,error);
        if(backup==Read::error)return false;
        if(backup==Read::missing||!usable_header(candidate.bytes)){error="No usable campaign base/backup header: "+path;return false;}
    }
    output=std::move(candidate);error.clear();return true;
}
bool erase_campaign_slot_files_v1(const std::string& directory,std::uint32_t slot,
    std::uint32_t& deleted,std::string& error){
    deleted=0;
    if(slot>=4||directory.empty()||directory[0]!='/'){error="Invalid erase campaign slot or absolute directory";return false;}
    // Anchor every unlink to the opened directory, never a computed shell path.
    const int fd=open(directory.c_str(),O_RDONLY|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC);
    if(fd<0){error="Cannot open campaign directory for erase";return false;}
    DIR* raw=fdopendir(fd);
    if(!raw){close(fd);error="Cannot enumerate campaign directory for erase";return false;}
    std::unique_ptr<DIR,int(*)(DIR*)> dir(raw,closedir);
    char pattern[16];std::snprintf(pattern,sizeof(pattern),"dh2_%03u",slot);
    std::vector<std::string> names;
    for(;;){
        errno=0;const auto* entry=readdir(raw);
        if(!entry){if(errno){error="Campaign directory enumeration failed";return false;}break;}
        const std::string name=entry->d_name;
        if(name.find(pattern)==std::string::npos)continue;
        struct stat info{};
        if(fstatat(fd,name.c_str(),&info,AT_SYMLINK_NOFOLLOW)||(info.st_mode&S_IFMT)!=S_IFREG){
            error="Unsafe campaign erase entry: "+name;return false;
        }
        names.push_back(name);
    }
    for(const auto& name:names){
        if(unlinkat(fd,name.c_str(),0)){error="Cannot erase campaign entry: "+name;return false;}
        ++deleted;
    }
    error.clear();return true;
}
}
