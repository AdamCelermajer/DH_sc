#pragma once
#include "savegame_jobs_owner_v2.hpp"
#include <map>
#include <cstdio>
#include <mutex>
namespace dh2::level {
// Android getFilesDir transport; source framing/backup/commit belongs to the
// Savegame owners. This budget is explicit platform policy, not source maximum.
class PrivateSaveFileTransportV45 : public std::enable_shared_from_this<PrivateSaveFileTransportV45> {
 std::string directory_;std::uint64_t budget_;
 std::map<void*,std::FILE*> open_;
 std::recursive_mutex io_mutex_v102_;
 bool path(const std::string&,std::string&,std::string&)const;
 static bool read(void*,const std::string&,bool&,std::vector<std::uint8_t>&,std::string&);
 static bool backup(void*,const std::string&,const std::string&,bool&,std::string&);
 static bool open(void*,const std::string&,void*&,std::string&);
 static bool write(void*,void*,data::Bytes,std::uint64_t&,std::string&);
 static bool seek(void*,void*,std::uint64_t,std::string&);
 static bool close(void*,void*&,std::string&);
 static bool failure(void*,const char*,std::string&);
public:
 PrivateSaveFileTransportV45(std::string actual_files_directory,std::uint64_t budget);
 ~PrivateSaveFileTransportV45();
 PrivateSaveFileTransportV45(const PrivateSaveFileTransportV45&)=delete;
 SavegameFileServicesV2 services();
 // Actual existence query; unlike openSavefile, it neither reads nor flushes jobs.
 bool source_exists_v115(const std::string&,bool&,std::string&);
 const std::string& directory_v59()const noexcept{return directory_;}
};
}
