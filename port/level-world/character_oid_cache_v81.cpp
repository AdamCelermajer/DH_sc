#include "character_oid_cache_v81.hpp"
#include <algorithm>
#include <exception>
namespace dh2::character {
namespace {
//Actual global initializer precedes application/runtime calls. Consumers keep
//shared leases so native static destruction never outlives a retained borrower.
const auto process_cache_v81=std::make_shared<CharacterOidCacheV81>();
}
std::uint32_t CharacterOidCacheV81::has(std::int32_t id)const noexcept{
 const auto at=counts_.find(id);return at==counts_.end()?0:at->second;
}
bool CharacterOidCacheV81::add(std::int32_t id,std::uint32_t count,
 const data::CharacterTable& characters,const data::Dictionary& models,
 const CharacterOidPreloadServicesV81& services,std::string& e)try{
 if(id<0||static_cast<std::size_t>(id)>=characters.rows.size()){e.clear();return true;}
 const auto old=counts_.find(id);
 if(old!=counts_.end()){old->second=std::max(old->second,count);e.clear();return true;}
 auto inserted=counts_.emplace(id,0).first;inserted->second=count;
 //Source CharProps row900 includes a vptr; word+16 maps to native scalar3.
 const auto model=characters.rows[static_cast<std::size_t>(id)][3];
 if(model<0||static_cast<std::size_t>(model)>=models.values.size()){e.clear();return true;}
 if(!services.provider||!services.preload_scene){e="Required actual SAME SceneManager.PreloadScene359b68";return false;}
 return services.preload_scene(models.values[static_cast<std::size_t>(model)],e);
}catch(const std::exception& failure){e=failure.what();return false;}
std::shared_ptr<CharacterOidCacheV81> character_oid_cache_process_v81(){
 return process_cache_v81;
}
}
