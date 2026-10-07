#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <array>

namespace dh2::level {
// Names are source scalar inputs, not profile-slot or world-handle aliases.
struct LevelSavegameRequestV1 {
 const void* level{}; std::uint32_t seed{};
 std::int32_t difficulty{},row{},mode{}; bool checkpoint{};
};
struct LevelSavegameFieldsV1 {
 const void* level8{}; std::int32_t mode0c{};
 std::string string10;
 std::int32_t row28{},loaded_row2c{-1},field30{-1},field34{-1};
 std::uint8_t initializing38{1},inhibit_save39{};
};
enum class LevelSavegameSectionV1 {info,objects};
// The one returned lease must own the actual Savegame cache/directory/job
// receiver. It must not create a PlayerSavegame or copy the live Level graph.
// construct_cache implements Savegame C1 including _cacheFile(NULL).
struct LevelSavegameServicesV1 {
 void* context{};
 bool (*construct_cache)(void*,const std::string&,bool,std::shared_ptr<void>&,std::string&){};
 bool (*register_section)(void*,void*,const char*,LevelSavegameSectionV1,LevelSavegameFieldsV1&,std::string&){};
 bool (*load_section)(void*,void*,const char*,LevelSavegameSectionV1,LevelSavegameFieldsV1&,std::string&){};
 bool (*network_online)(void*,bool&,std::string&){};
 bool (*is_hosting)(void*,bool&,std::string&){};
 bool (*manager_flag719)(void*,std::uint8_t&,std::string&){};
 bool (*save_all)(void*,void*,std::string&){};
 // Source Savegame D1/D0, before lease reset. D1 releases cache/directory;
 // it does not call FlushJobs (global job delivery is a separate boundary).
 bool (*destroy_cache)(void*,void*,std::string&){};
 // Delivery success and original checkpoint validity are separate results.
 bool (*validate_checkpoint)(void*,void*,LevelSavegameFieldsV1&,std::uint32_t,std::int32_t,std::int32_t,bool&,std::string&){};
};
class LevelSavegameOwnerV1 {
 LevelSavegameServicesV1 services_; LevelSavegameFieldsV1 fields_;
 std::shared_ptr<void> cache_; std::string filename_;
 unsigned phase_{}; bool attempted_{},ready_{},released_{},busy_{};
 bool fail(std::string&,const char*) const;
public:
 explicit LevelSavegameOwnerV1(LevelSavegameServicesV1 s):services_(s){}
 LevelSavegameOwnerV1(const LevelSavegameOwnerV1&)=delete;
 LevelSavegameOwnerV1& operator=(const LevelSavegameOwnerV1&)=delete;
 static std::string filename(std::uint32_t,std::int32_t,std::int32_t,std::int32_t);
 static std::string checkpoint_filename(std::uint32_t,std::int32_t,bool);
 bool construct(const LevelSavegameRequestV1&,std::string&);
 bool load(std::string&); bool save(std::string&); bool release(std::string&);
 bool validate_checkpoint(std::uint32_t seed,std::int32_t difficulty,std::int32_t row,bool& valid,std::string&);
 bool ready()const noexcept{return ready_;}
 unsigned phase()const noexcept{return phase_;}
 void* native_savegame()const noexcept{return cache_.get();}
 const std::string& current_filename()const noexcept{return filename_;}
 LevelSavegameFieldsV1& fields()noexcept{return fields_;}
 const LevelSavegameFieldsV1& fields()const noexcept{return fields_;}
};
}
