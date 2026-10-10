#include "campaign_save_profile_v45.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::level {
CampaignSaveProfileV45::CampaignSaveProfileV45(std::string name,SavegameFileServicesV2 files,std::shared_ptr<SavegameJobsOwnerV2> jobs):filename_(std::move(name)),files_(std::move(files)),jobs_(std::move(jobs)){}
bool CampaignSaveProfileV45::construct(std::string& e){
 if(destroyed_v108_){e="Campaign profile delivery after source D0";return false;}
 if(attempted_){e="Campaign Savegame C1 cannot replay";return false;}attempted_=true;
 if(filename_.empty()||!files_.storage_lease||!files_.read_file||!jobs_){e="Required actual profile filename/FileManager/shared Application save jobs";return false;}
 // SavegameJobsOwnerV2::flush is intentionally a no-op when called from an
 // active job callback. A C1 read in that window would otherwise publish the
 // previous on-disk profile as if the queued backup/write prefix had drained.
 if(jobs_->busy_v61()){e="Campaign profile C1 cannot read during an active source save job";return false;}
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
 if(destroyed_v108_||!ready_||!tag||std::strlen(tag)!=4||!lease||!writer||writing_){e="Required actual ready four-byte campaign section writer/lifetime";return false;}
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
 if(destroyed_v108_||!ready_||writing_||!jobs_){e="Required nonrecursive ready SAME campaign profile/jobs";return false;}
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
bool CampaignSaveProfileV45::source_filename_store_v83(const std::string& filename,std::string& e){
 if(destroyed_v108_||!ready_||writing_){e="Source profile filename store requires SAME quiescent constructed receiver";return false;}
 filename_=filename;e.clear();return true;
}
bool CampaignSaveProfileV45::destroy_source_v108(std::string& e){
 if(destroyed_v108_){e.clear();return true;}
 if(writing_){e="Savegame D0 cannot destroy its active writer callback";return false;}
 //313dfc: cache1c D0/NULL, section tree20 D1, filename4 D1. The
 //native immutable index combines cache bytes with their section projection;
 //retiring its authority preserves outstanding host read-only copies.
 index_.retire_source_v108();cached_=false;
 writers_.clear();std::string{}.swap(filename_);
 ready_=false;destroyed_v108_=true;
 //App jobs own their streams independently: source D1 never flushes/cancels
 //that queue. Releasing this receiver's backend loans leaves it unchanged.
 files_={};jobs_.reset();e.clear();return true;
}
data::PlayerSaveWriteServicesV1 CampaignSaveProfileV45::write_services(std::function<bool(const data::PlayerSaveWriteRequestV1&,data::PlayerSaveWriteResponseV1&,std::string&)> remaining){
 auto self=shared_from_this();return {self,[self,remaining=std::move(remaining)](const auto& q,auto& out,auto& e){
  if(self->destroyed_v108_){e="Campaign writer delivery after source D0";return false;}
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
