#pragma once
#include "level_savegame_cache_v1.hpp"
#include <functional>
namespace dh2::level {
struct LevelCheckpointValidationServicesV115 {
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(bool&,std::string&)> hosting;
 std::function<bool(std::uint8_t&,std::string&)> manager719;
 std::function<bool(const std::string&,bool&,std::string&)> exists;
};
// Original LevelSavegame::ValidateCheckpoint463288, on the existing cache.
// Delivery success is separate from the original bool result. Failures retain
// reached filename/cache/INFO prefixes; no scope-exit rollback is invented.
inline bool level_validate_checkpoint_v115(LevelSavegameCacheV1& cache,
 LevelSavegameFieldsV1& fields,std::uint32_t seed,std::int32_t difficulty,
 std::int32_t row,const LevelCheckpointValidationServicesV115& services,
 bool& valid,std::string& e){
 e.clear();valid=false;
 if(!cache.ready()||!fields.level8||!services.online||!services.exists){e="Required SAME constructed checkpoint receiver/online/FileManager";return false;}
 const auto mode=fields.mode0c; //4632c8: read before original network queries.
 bool online=false;if(!services.online(online,e))return false;
 bool multi=false;
 if(online){
  bool host=false;if(!services.hosting||!services.hosting(host,e)){if(e.empty())e="Required actual checkpoint IsLocalPlayerHosting";return false;}
  if(!host)multi=true;
  else{std::uint8_t flag=0;if(!services.manager719||!services.manager719(flag,e)){if(e.empty())e="Required actual checkpoint manager719";return false;}multi=flag!=0;}
 }
 const auto checkpoint=LevelSavegameOwnerV1::checkpoint_filename(seed,mode,multi);
 bool exists=false;if(!services.exists(checkpoint,exists,e))return false;
 std::string selected=checkpoint;
 if(!exists){selected+=".bak";if(!services.exists(selected,exists,e))return false;if(!exists)return true;}
 if(!cache.source_filename_store_v83(selected,e)||!cache.source_cache_file_v115(e)||
    !cache.load_section("INFO",LevelSavegameSectionV1::info,fields,e))return false;
 const bool original_valid=fields.loaded_row2c==row;
 //463458 rereads mode0c after load(INFO); preserve mutable source inputs.
 if(!cache.source_filename_store_v83(LevelSavegameOwnerV1::filename(seed,difficulty,row,fields.mode0c),e)||
    !cache.source_cache_file_v115(e))return false;
 valid=original_valid;e.clear();return true;
}
}
