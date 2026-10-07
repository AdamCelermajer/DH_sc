#pragma once
#include "level_savegame_owner_v1.hpp"
#include "../game-data/player_profile_index_v1.hpp"
namespace dh2::level {
struct LevelSavegameCacheServicesV1 {
 void* context{};
 // Actual FileManager open/read ownership. found=false is a genuine miss.
 bool (*read_file)(void*,const std::string&,bool& found,std::vector<std::uint8_t>&,std::string&){};
 bool (*load_objects)(void*,data::Bytes,LevelSavegameFieldsV1&,std::string&){};
 // Source Savegame.load exposes the complete cache, not a bounded section.
 bool (*load_objects_stream_v2)(void*,data::Bytes,std::uint64_t,LevelSavegameFieldsV1&,std::string&){};
 // Pin actual callback/provider storage, never the whole App/Level graph.
 // Production application bindings must retain their genuine FileManager
 // and object-dispatch storage here; stack-only fixtures are scoped tests.
 std::shared_ptr<void> storage_lease{};
};
// Source Savegame C1/cache/register/load subset. File jobs/saveAll stay an
// explicit whole service; this owner never invents a writable empty profile.
class LevelSavegameCacheV1 {
 LevelSavegameCacheServicesV1 services_; data::PlayerProfileIndexV1 index_;
 std::string filename_; bool attempted_{},ready_{},has_file_{};
 std::array<LevelSavegameFieldsV1*,2> sections_{};
public:
 explicit LevelSavegameCacheV1(LevelSavegameCacheServicesV1 s):services_(s){}
 bool construct(const std::string&,bool,std::string&);
 bool register_section(const char*,LevelSavegameSectionV1,LevelSavegameFieldsV1&,std::string&);
 bool load_section(const char*,LevelSavegameSectionV1,LevelSavegameFieldsV1&,std::string&);
 bool recache_v2(data::Bytes,std::string&);
 // Original _cacheFile(NULL): release old cache, open current filename/backup,
 // retain existing INFO/OBJS receiver registrations; never replay C1.
 bool source_cache_file_v115(std::string&);
 bool ready()const noexcept{return ready_;}
 bool has_cached_file()const noexcept{return has_file_;}
 const std::string& filename()const noexcept{return filename_;}
 bool source_filename_store_v83(const std::string& filename,std::string& error){
  if(!ready_){error="Source Level profile filename store before actual C1";return false;}
  filename_=filename;error.clear();return true;
 }
 data::PlayerProfileIndexV1::Borrow profile()const{return index_.borrow();}
};
}
