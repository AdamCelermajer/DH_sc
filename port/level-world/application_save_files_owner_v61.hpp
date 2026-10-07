#pragma once
#include "private_save_file_transport_v45.hpp"
#include <functional>
namespace dh2::application {
class ApplicationServicesOwnerV5;
// Modern retention around the EXISTING FileManager syscall transport and
// original Savegame job owner. The source jobs are advanced on the Application
// thread, not a fabricated worker/instant-write backend. Profile and Level
// constructors borrow this same pair; no profile/World creates another queue.
class ApplicationSaveFilesOwnerV61 final:public std::enable_shared_from_this<ApplicationSaveFilesOwnerV61> {
 mutable std::recursive_mutex native_io_mutex_v102_;
 friend class ApplicationServicesOwnerV5;
 std::weak_ptr<ApplicationServicesOwnerV5> application_;
 std::shared_ptr<level::PrivateSaveFileTransportV45> files_;
 std::shared_ptr<level::SavegameJobsOwnerV2> jobs_;
 bool native_call_active_{};
 explicit ApplicationSaveFilesOwnerV61(const std::shared_ptr<ApplicationServicesOwnerV5>&);
 bool admit(std::string&)const;
 static bool valid_name(const char*,std::string&);
public:
 // Explicit existing application I/O policy, NOT a native format maximum or
 // proof that every future level save fits. No file is truncated to fit.
 static constexpr std::uint64_t private_save_budget=16u*1024u*1024u;
 static bool acquire(const std::shared_ptr<ApplicationServicesOwnerV5>&,
  const std::string& actual_private_directory,
  std::shared_ptr<ApplicationSaveFilesOwnerV61>&,std::string&,
  std::shared_ptr<level::PrivateSaveFileTransportV45> actual_existing_files={},
  std::shared_ptr<level::SavegameJobsOwnerV2> actual_existing_jobs={});
 bool belongs_to_application(const std::shared_ptr<ApplicationServicesOwnerV5>&)const noexcept;
 bool matches_directory(const std::string&)const noexcept;
 const auto& files()const noexcept{return files_;}
 const auto& jobs()const noexcept{return jobs_;}
 // Read runs the source openSavefile matching-job flush prefix. Low-level
 // jobs themselves retain files()->services(), avoiding recursive flush.
 bool read_save(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&);
 // Source UpdateJobs: one queued backup or one2048-byte block per call.
 // Accepted enqueue is not completion. Caller serializes source operations
 // with the existing Application/frame lifetime, as for the original owner.
 bool update(bool& progressed,std::string&);
 // Source FlushJobs: matching request may drain the shared queue. A busy
 // original leaf returning true is not reported as completed here.
 bool flush(const char* filename,std::string&);
 //Distinct settings worker body: source matched open flush, raw options and
 //tutorial write, close. Does not enqueue a framed Savegame/profile job.
 bool write_settings_stream_v102(const std::function<std::vector<std::uint8_t>()>& serialize,std::string&);
};
}
