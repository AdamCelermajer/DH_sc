#include "../campaign_profile_files_v1.hpp"
#include "../player_profile_index_v1.hpp"
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
using namespace dh2::data;
static void require(bool b){if(!b){std::fprintf(stderr,"FAIL\n");std::exit(1);}}
static void write(const std::string& p,const std::vector<std::uint8_t>& bytes){FILE* f=std::fopen(p.c_str(),"wb");require(f);require(bytes.empty()||std::fwrite(bytes.data(),1,bytes.size(),f)==bytes.size());require(!std::fclose(f));}
int main(){
    char pattern[]="/data/local/tmp/dh2-campaign-files-v69-XXXXXX";
    char* raw=mkdtemp(pattern);require(raw);std::string dir=raw,error;
    const std::string base=dir+"/dh2_002.savegame",backup=base+".bak";
    CampaignProfileFileV1 out;out.bytes={42};bool occupied=true;
    require(campaign_profile_exists_v1(dir,2,occupied,error)&&!occupied);
    require(!read_campaign_profile_v1(dir,2,out,error)&&out.bytes==std::vector<std::uint8_t>({42}));
    const std::vector<std::uint8_t> valid={0,0,0,0};write(backup,valid);
    for(int len=0;len<4;++len){write(base,std::vector<std::uint8_t>(len,0));require(read_campaign_profile_v1(dir,2,out,error)&&out.origin==CampaignProfileOriginV1::backup&&out.bytes==valid);}
    write(base,{255,255,255,255,12});require(read_campaign_profile_v1(dir,2,out,error)&&out.origin==CampaignProfileOriginV1::backup);
    write(base,valid);require(read_campaign_profile_v1(dir,2,out,error)&&out.origin==CampaignProfileOriginV1::base);
    require(campaign_profile_exists_v1(dir,2,occupied,error)&&occupied);
    // A structurally truncated nonmarker profile selects base, then fails the
    // actual bounded section index rather than silently preferring backup.
    write(base,{1,0,0,0});require(read_campaign_profile_v1(dir,2,out,error)&&out.origin==CampaignProfileOriginV1::base);
    PlayerProfileIndexV1 index;require(!index.load({out.bytes.data(),out.bytes.size()},error));
    require(!std::remove(base.c_str()));write(backup,{255,255,255,255});out.bytes={42};
    require(!read_campaign_profile_v1(dir,2,out,error)&&out.bytes==std::vector<std::uint8_t>({42}));
    require(!std::remove(backup.c_str()));require(!mkdir(base.c_str(),0700));
    require(!read_campaign_profile_v1(dir,2,out,error)&&out.bytes==std::vector<std::uint8_t>({42}));require(!rmdir(base.c_str()));
    require(!read_campaign_profile_v1(dir,4,out,error));require(!campaign_profile_exists_v1("",2,occupied,error));
    std::uint32_t deleted=99;
    require(erase_campaign_slot_files_v1(dir,2,deleted,error)&&deleted==0);
    const std::vector<std::string> erased={"dh2_002.savegame","dh2_002.savegame.bak","dh2_002_checkpoint","dh2_002_level_041.savegame","prefix-dh2_002-extra"};
    const std::vector<std::string> retained={"dh2_000.savegame","dh2_003.savegame","dh2_02.savegame","settings"};
    for(const auto& name:erased)write(dir+"/"+name,valid);
    for(const auto& name:retained)write(dir+"/"+name,valid);
    // Whole enumeration is checked before removals. A matching directory or
    // symlink fails without deleting a valid matching campaign beside it.
    const auto unsafe=dir+"/dh2_002_unsafe";require(!mkdir(unsafe.c_str(),0700));
    require(!erase_campaign_slot_files_v1(dir,2,deleted,error)&&deleted==0);
    require(!access(base.c_str(),F_OK));require(!rmdir(unsafe.c_str()));
    require(!symlink((dir+"/settings").c_str(),unsafe.c_str()));
    require(!erase_campaign_slot_files_v1(dir,2,deleted,error)&&deleted==0);
    require(!access(base.c_str(),F_OK));require(!unlink(unsafe.c_str()));
    require(erase_campaign_slot_files_v1(dir,2,deleted,error)&&deleted==erased.size());
    for(const auto& name:erased)require(access((dir+"/"+name).c_str(),F_OK)!=0);
    for(const auto& name:retained){require(!access((dir+"/"+name).c_str(),F_OK));require(!unlink((dir+"/"+name).c_str()));}
    require(!erase_campaign_slot_files_v1(dir,4,deleted,error)&&deleted==0);
    require(!erase_campaign_slot_files_v1("relative",2,deleted,error));
    require(!rmdir(dir.c_str()));
    std::printf("PASS campaign reads and erase: source substring matching, base/backup/checkpoint/level removal, adjacent slots retained, directory/symlink preflight rejection, bounded slots; temporary files removed\n");
}
