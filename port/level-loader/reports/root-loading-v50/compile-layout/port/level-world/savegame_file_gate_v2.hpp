#pragma once
#include "savegame_jobs_owner_v2.hpp"
#include "level_savegame_cache_v1.hpp"
namespace dh2::level {
// FileSystem.openSavefile34fee4's matching-job flush prefix. This is a small
// retained provider adapter, not a second Application/FileManager or Level.
class SavegameFileGateV2 : public std::enable_shared_from_this<SavegameFileGateV2> {
 SavegameFileServicesV2 platform_;std::shared_ptr<SavegameJobsOwnerV2> jobs_;
 void* object_context_{};
 bool(*objects_)(void*,data::Bytes,std::uint64_t,LevelSavegameFieldsV1&,std::string&){};
 std::shared_ptr<void> object_storage_;
 static bool read(void*,const std::string&,bool&,std::vector<std::uint8_t>&,std::string&);
 static bool objects(void*,data::Bytes,std::uint64_t,LevelSavegameFieldsV1&,std::string&);
public:
 SavegameFileGateV2(SavegameFileServicesV2 platform,std::shared_ptr<SavegameJobsOwnerV2> jobs):platform_(std::move(platform)),jobs_(std::move(jobs)){}
 void bind_objects(void* context,decltype(objects_) callback,std::shared_ptr<void> actual_provider_storage){object_context_=context;objects_=callback;object_storage_=std::move(actual_provider_storage);}
 bool cache_services(LevelSavegameCacheServicesV1&,std::string&);
 bool read_file(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&);
};
}
