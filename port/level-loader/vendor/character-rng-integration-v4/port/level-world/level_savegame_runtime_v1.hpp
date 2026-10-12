#pragma once
#include "level_savegame_cache_v1.hpp"
namespace dh2::level {
struct LevelSavegameApplicationV1 {
 LevelSavegameCacheServicesV1 files;
 void* context{};
 bool (*network_online)(void*,bool&,std::string&){};
 bool (*is_hosting)(void*,bool&,std::string&){};
 bool (*manager_flag719)(void*,std::uint8_t&,std::string&){};
 // The source saveAll section serialization, global jobs and file delivery.
 // INFO must write SAME fields.row28; OBJS must enumerate SAME ObjectManager.
 bool (*save_all)(void*,LevelSavegameCacheV1&,LevelSavegameFieldsV1&,std::string&){};
};
// One native Savegame cache per LevelSavegame, and one borrowed Level identity.
// Allocate this before the outer Level rereads its constructor arguments.
class LevelSavegameRuntimeV1 {
 LevelSavegameApplicationV1 application_;
 std::weak_ptr<LevelSavegameCacheV1> cache_;
 LevelSavegameOwnerV1 owner_;
 static bool create(void*,const std::string&,bool,std::shared_ptr<void>&,std::string&);
 static bool section(void*,void*,const char*,LevelSavegameSectionV1,LevelSavegameFieldsV1&,std::string&);
 static bool load(void*,void*,const char*,LevelSavegameSectionV1,LevelSavegameFieldsV1&,std::string&);
 static bool online(void*,bool&,std::string&);
 static bool hosting(void*,bool&,std::string&);
 static bool flag(void*,std::uint8_t&,std::string&);
 static bool save(void*,void*,std::string&);
 static bool destroy(void*,void*,std::string&);
 bool same_cache(void*,std::string&)const;
public:
 explicit LevelSavegameRuntimeV1(LevelSavegameApplicationV1);
 LevelSavegameRuntimeV1(const LevelSavegameRuntimeV1&)=delete;
 LevelSavegameRuntimeV1& operator=(const LevelSavegameRuntimeV1&)=delete;
 LevelSavegameOwnerV1& owner()noexcept{return owner_;}
 const LevelSavegameOwnerV1& owner()const noexcept{return owner_;}
 std::shared_ptr<const LevelSavegameCacheV1> cache()const{return cache_.lock();}
};
}
