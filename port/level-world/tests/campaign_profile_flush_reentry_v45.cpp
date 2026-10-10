#include "../campaign_save_profile_v45.hpp"
#include "../private_save_file_transport_v45.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <unistd.h>

using namespace dh2;

namespace {
struct ReadReentry {
 level::SavegameFileServicesV2 backing;
 std::shared_ptr<level::CampaignSaveProfileV45> profile;
 bool trigger{};
 bool accepted{};
 bool ready{};
 std::string nested_error;
};
ReadReentry* active_reentry{};

bool read_file(void* raw,const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& error){
 auto& state=*active_reentry;
 if(state.trigger){
  state.trigger=false;
  state.accepted=state.profile->construct(state.nested_error);
  state.ready=state.profile->ready();
 }
 return state.backing.read_file(state.backing.context,name,found,bytes,error);
}

std::unique_ptr<level::SavegameStreamV2> profile_bytes(std::uint32_t level_value,std::string& error){
 auto stream=std::make_unique<level::SavegameStreamV2>();
 const std::array<std::uint8_t,4> payload{{
  static_cast<std::uint8_t>(level_value),static_cast<std::uint8_t>(level_value>>8),
  static_cast<std::uint8_t>(level_value>>16),static_cast<std::uint8_t>(level_value>>24)}};
 static constexpr std::uint8_t tag[]{'P','L','V','L'};
 if(!stream->write_u32(1,error)||!stream->write_u32(static_cast<std::uint32_t>(payload.size()),error)||
    !stream->write({tag,sizeof(tag)},error)||!stream->write({payload.data(),payload.size()},error))return {};
 return stream;
}
}

int main(){
 unsigned checks{};std::string error;
 char folder[]="/tmp/dh2-profile-flush-v45-XXXXXX";assert(::mkdtemp(folder));
 auto transport=std::make_shared<level::PrivateSaveFileTransportV45>(folder,1024*1024);
 ReadReentry state;state.backing=transport->services();active_reentry=&state;
 auto files=state.backing;files.read_file=&read_file;
 auto jobs=std::make_shared<level::SavegameJobsOwnerV2>(files);
 constexpr const char* filename="dh2_000.savegame";

 assert(jobs->add_write(filename,profile_bytes(1,error),error));
 assert(jobs->flush(filename,error));++checks;
 state.profile=std::make_shared<level::CampaignSaveProfileV45>(filename,files,jobs);
 assert(jobs->add_backup(filename,error));
 assert(jobs->add_write(filename,profile_bytes(9,error),error));
 auto nested=std::make_shared<level::CampaignSaveProfileV45>(filename,files,jobs);
 state.profile=nested;state.trigger=true;
 bool progressed{};assert(jobs->update(progressed,error)&&progressed);++checks;
 assert(!state.accepted&&!state.ready&&state.nested_error.find("active source save job")!=std::string::npos);++checks;
 assert(jobs->flush(nullptr,error)&&jobs->pending()==0);++checks;
 assert(jobs->release(error));++checks;

 auto relaunched_jobs=std::make_shared<level::SavegameJobsOwnerV2>(files);
 auto relaunched=std::make_shared<level::CampaignSaveProfileV45>(filename,files,relaunched_jobs);
 assert(relaunched->construct(error)&&relaunched->ready()&&relaunched->cached());++checks;
 {auto cache=relaunched->cache();data::PlayerSavegameV1 restored;std::size_t consumed{};
  assert(restored.load_level(cache.payload("PLVL"),consumed,error)&&restored.level()==9);++checks;}

 assert(relaunched_jobs->release(error));
 ::unlink((std::string(folder)+"/"+filename).c_str());
 ::unlink((std::string(folder)+"/"+filename+".bak").c_str());
 assert(::rmdir(folder)==0);
 std::cout<<"Campaign profile C1 rejects reentrant read during queued backup/write; post-flush relaunch sees committed PLVL PASS "<<checks<<" checks\n";
}
