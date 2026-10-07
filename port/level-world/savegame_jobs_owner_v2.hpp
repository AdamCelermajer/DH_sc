#pragma once
#include "savegame_stream_v2.hpp"
#include <deque>
#include <memory>
#include <mutex>
namespace dh2::level {
struct SavegameFileServicesV2 {
 void* context{};
 bool (*read_file)(void*,const std::string&,bool&,std::vector<std::uint8_t>&,std::string&){};
 // Source backupSavefile removes old destination (ENOENT accepted), renames
 // primary to .bak. completed invocation and source operation result differ.
 bool (*backup)(void*,const std::string&,const std::string&,bool& result,std::string&){};
 bool (*open_write)(void*,const std::string&,void*&,std::string&){};
 bool (*write)(void*,void*,data::Bytes,std::uint64_t& written,std::string&){};
 bool (*seek_write)(void*,void*,std::uint64_t,std::string&){};
 bool (*close)(void*,void*&,std::string&){};
 // Actual source debug assertion delivery for short writes/open NULL.
 bool (*write_failure)(void*,const char*,std::string&){};
 std::shared_ptr<void> storage_lease{};
};
// Application-global Savegame job list: share one owner across all save files.
// Accepted enqueue means queued, not persisted. Externally update per frame or
// flush; LevelSavegame destruction does not flush this global owner.
class SavegameJobsOwnerV2 {
 mutable std::recursive_mutex queue_mutex_v102_;
 struct Job{std::string filename;bool backup{};std::unique_ptr<SavegameStreamV2> stream;};
 SavegameFileServicesV2 files_;std::deque<Job> pending_;Job active_;
 bool active_valid_{},busy_{},failed_{};void* output_{};
 std::uint32_t block_{},block_count_{},commit_count_{0xffffffffu};
 std::uint32_t observed_short_writes_{}; // diagnostic, never gameplay authority
 bool failure(std::string&,const char*);bool finish(std::string&);
public:
 explicit SavegameJobsOwnerV2(SavegameFileServicesV2 f):files_(f){}
 SavegameJobsOwnerV2(const SavegameJobsOwnerV2&)=delete;
 bool add_backup(const std::string&,std::string&);
 bool add_write(const std::string&,std::unique_ptr<SavegameStreamV2>,std::string&);
 bool update(bool& progressed,std::string&);
 bool flush(const char* filename,std::string&);
 // Explicit native teardown of a retained failed prefix; does not replay jobs.
 bool release(std::string&);
 std::size_t pending()const {std::lock_guard<std::recursive_mutex> lock(queue_mutex_v102_);return pending_.size()+(active_valid_?1:0);}
 bool failed()const {std::lock_guard<std::recursive_mutex> lock(queue_mutex_v102_);return failed_;}
 bool busy_v61()const {std::lock_guard<std::recursive_mutex> lock(queue_mutex_v102_);return busy_;}
 std::uint32_t short_writes()const {std::lock_guard<std::recursive_mutex> lock(queue_mutex_v102_);return observed_short_writes_;}
 const std::shared_ptr<void>& file_storage_v59()const noexcept{return files_.storage_lease;}
};
}
