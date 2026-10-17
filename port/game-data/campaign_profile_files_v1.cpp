#include "campaign_profile_files_v1.hpp"
#include <cerrno>
#include <cstdio>
#include <memory>
#include <sys/stat.h>
#include <utility>
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
}
