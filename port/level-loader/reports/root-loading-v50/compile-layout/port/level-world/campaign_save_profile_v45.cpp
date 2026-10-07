#include "campaign_save_profile_v45.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::level {
CampaignSaveProfileV45::CampaignSaveProfileV45(std::string name,SavegameFileServicesV2 files,std::shared_ptr<SavegameJobsOwnerV2> jobs):filename_(std::move(name)),files_(std::move(files)),jobs_(std::move(jobs)){}
bool CampaignSaveProfileV45::construct(std::string& e){
 if(attempted_){e="Campaign Savegame C1 cannot replay";return false;}attempted_=true;
 if(filename_.empty()||!files_.storage_lease||!files_.read_file||!jobs_){e="Required actual profile filename/FileManager/shared Application save jobs";return false;}
 if(!jobs_->flush(filename_.c_str(),e))return false; // actual openSavefile matching-job prefix
 // Same original non-raw Savegame C1 primary/backup header branch as the
 // independently proved LevelSavegameCacheV1. No empty on malformed payload.
 auto valid=[](const auto& b){return b.size()>=4&&!(b[0]==255&&b[1]==255&&b[2]==255&&b[3]==255);};
 bool found{};std::vector<std::uint8_t> bytes;
 if(!files_.read_file(files_.context,filename_,found,bytes,e))return false;
 if(!found||!valid(bytes)){found=false;bytes.clear();if(!files_.read_file(files_.context,filename_+".bak",found,bytes,e))return false;if(!found||!valid(bytes)){ready_=true;return true;}}
 if(!index_.load({bytes.data(),bytes.size()},e))return false;cached_=true;ready_=true;return true;
}
bool CampaignSaveProfileV45::register_writer(const char* tag,std::shared_ptr<void> lease,std::function<bool(SavegameStreamV2&,std::string&)> writer,std::string& e){
 if(!ready_||!tag||std::strlen(tag)!=4||!lease||!writer||writing_){e="Required actual ready four-byte campaign section writer/lifetime";return false;}
 writers_[tag]={std::move(lease),std::move(writer)};return true; // source registration overwrites same key
}
bool CampaignSaveProfileV45::register_named_writer(const char* tag,std::shared_ptr<data::PlayerSaveNamedWriterV1> named,std::string& e){
 if(!named||!tag){e="Required SAME actual named PlayerSave writer";return false;}
 const std::string name=tag;
 // Reject unsupported registration before jobs can become an apparent save.
 if(name!="PNAM"&&name!="PLVL"&&name!="SKIL"&&name!="FAES"&&name!="CFEE"){e="Required unrecovered player section writer "+name;return false;}
 std::weak_ptr<data::PlayerSaveNamedWriterV1> weak=named;
 return register_writer(tag,named,[weak,name](SavegameStreamV2& stream,std::string& error){
  auto named=weak.lock();if(!named){error="Required retained SAME named Save writer lifetime";return false;}
  data::PlayerSaveByteWriterV1 sink{named,[&stream](data::Bytes b,std::string& x){return stream.write(b,x);}};
  return named->write(name.c_str(),sink,error);
 },e);
}
bool CampaignSaveProfileV45::save_all(std::string& e){
 if(!ready_||writing_||!jobs_){e="Required nonrecursive ready SAME campaign profile/jobs";return false;}
 writing_=true;struct End{bool& v;~End(){v=false;}}end{writing_};
 if(!jobs_->add_backup(filename_,e))return false;
 std::vector<SavegameSectionWriterV2> writers;writers.reserve(writers_.size());
 auto invoke=[](void* raw,SavegameStreamV2& stream,std::string& error){auto& writer=*static_cast<Writer*>(raw);auto lease=writer.lease.lock();if(!lease){error="Required actual registered campaign writer lifetime";return false;}return writer.write(stream,error);};
 for(auto& p:writers_){SavegameSectionWriterV2 writer;std::copy_n(p.first.data(),4,writer.tag.data());writer.context=&p.second;writer.write=invoke;writers.push_back(writer);}
 std::unique_ptr<SavegameStreamV2> output;
 {auto existing=index_.borrow();if(!savegame_build_frame_v2(existing,writers,output,e))return false;}
 const auto& bytes=output->bytes();if(!index_.load({bytes.data(),bytes.size()},e))return false;cached_=true;
 return jobs_->add_write(filename_,std::move(output),e); // queue acceptance, not persisted receipt
}
data::PlayerSaveWriteServicesV1 CampaignSaveProfileV45::write_services(std::function<bool(const data::PlayerSaveWriteRequestV1&,data::PlayerSaveWriteResponseV1&,std::string&)> remaining){
 auto self=shared_from_this();return {self,[self,remaining=std::move(remaining)](const auto& q,auto& out,auto& e){
  if(!q.authority||q.profile.identity!=reinterpret_cast<std::uintptr_t>(self.get())||q.save!=&q.authority->save()){
   e="Required identical campaign Save/Profile authority";return false;
  }
  if(q.operation==data::PlayerSaveWriteOpV1::save_all)return self->save_all(e);
  if(q.operation==data::PlayerSaveWriteOpV1::profile_has_cache){out.flag=self->cached_;return true;}
  if(!remaining){e="Required campaign source operation "+std::to_string(unsigned(q.operation));return false;}
  return remaining(q,out,e);
 }};
}
}
