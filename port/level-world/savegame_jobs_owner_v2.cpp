#include "savegame_jobs_owner_v2.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::level {
bool SavegameJobsOwnerV2::failure(std::string& e,const char* why){failed_=true;if(e.empty())e=why;return false;}
bool SavegameJobsOwnerV2::add_backup(const std::string& name,std::string& e){if(failed_){e="Source save job owner failed";return false;}for(auto i=pending_.begin();i!=pending_.end();)if(i->backup&&i->filename==name)i=pending_.erase(i);else++i;pending_.push_back({name,true,{}});return true;}
bool SavegameJobsOwnerV2::add_write(const std::string& name,std::unique_ptr<SavegameStreamV2> s,std::string& e){if(failed_||!s){e="Required owned source Savegame stream job";return false;}for(auto i=pending_.begin();i!=pending_.end();)if(!i->backup&&i->filename==name)i=pending_.erase(i);else++i;pending_.push_back({name,false,std::move(s)});return true;}
bool SavegameJobsOwnerV2::finish(std::string& e){if(output_&&(!files_.close||!files_.close(files_.context,output_,e)))return failure(e,"Required source FileManager closeFile");active_=Job{};active_valid_=false;block_=block_count_=0;return true;}
bool SavegameJobsOwnerV2::update(bool& progressed,std::string& e){
 e.clear();progressed=false;if(failed_){e="Source save job failure prefix cannot retry";return false;}if(busy_)return true;busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};
 if(!active_valid_){if(pending_.empty())return true;active_=std::move(pending_.front());pending_.pop_front();active_valid_=true;progressed=true;
  if(active_.backup){bool found=false;std::vector<std::uint8_t>b;if(!files_.read_file||!files_.read_file(files_.context,active_.filename,found,b,e))return failure(e,"Required backup source open/read");if(found&&b.size()>=4&&!(b[0]==255&&b[1]==255&&b[2]==255&&b[3]==255)){bool result=false;if(!files_.backup||!files_.backup(files_.context,active_.filename,active_.filename+".bak",result,e))return failure(e,"Required whole source backupSavefile");}return finish(e);}
  if(!files_.open_write||!files_.open_write(files_.context,active_.filename,output_,e))return failure(e,"Required source openSavefile(write=true)");
  if(!output_){if(!files_.write_failure||!files_.write_failure(files_.context,"openSavefile returned NULL",e))return failure(e,"Required source open-save failure diagnostic");return finish(e);}
  active_.stream->seek(0);if(!active_.stream->read_u32(commit_count_,e))return failure(e,"Required source stream section-count read");
  active_.stream->seek_write(0);if(!active_.stream->write_u32(0xffffffffu,e))return failure(e,"Required source invalid-header publication");
  block_count_=static_cast<std::uint32_t>((active_.stream->size()+2047)/2048);block_=0;
 }
 progressed=true;const auto& b=active_.stream->bytes();std::size_t offset=std::size_t(block_)*2048,n=std::min<std::size_t>(2048,b.size()-offset);std::uint64_t written=0;
 if(!files_.write||!files_.write(files_.context,output_,{b.data()+offset,n},written,e))return failure(e,"Required source file block write");
 // Original UpdateJobs ignores the bulk write return; only its final integer
 // commit uses the asserting uint32 writer. Preserve that distinction.
 if(written!=n)++observed_short_writes_;
 if(++block_==block_count_){if(!files_.seek_write||!files_.seek_write(files_.context,output_,0,e))return failure(e,"Required source header commit seek");std::uint8_t count[4];for(unsigned i=0;i<4;++i)count[i]=static_cast<std::uint8_t>(commit_count_>>(8*i));written=0;
  if(!files_.write(files_.context,output_,{count,4},written,e))return failure(e,"Required source final header commit");if(written!=4&&(!files_.write_failure||!files_.write_failure(files_.context,"source header short write",e)))return failure(e,"Required source header assertion");return finish(e);}
 return true;
}
bool SavegameJobsOwnerV2::flush(const char* name,std::string& e){if(busy_)return true;bool found=!name||(active_valid_&&active_.filename==name);if(!found)for(const auto& j:pending_)if(j.filename==name){found=true;break;}if(!found)return true;bool progressed;do{if(!update(progressed,e))return false;}while(progressed);return true;}
bool SavegameJobsOwnerV2::release(std::string& e){e.clear();if(busy_){e="Savegame job teardown is busy";return false;}if(output_&&(!files_.close||!files_.close(files_.context,output_,e)))return failure(e,"Required actual file close on native job teardown");pending_.clear();active_=Job{};active_valid_=false;failed_=true;return true;}
}
