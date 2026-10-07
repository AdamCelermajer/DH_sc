#pragma once
#include "../game-data/data.hpp"
#include <functional>
#include <map>
#include <memory>
namespace dh2::character {
struct CharacterOidPreloadServicesV81 {
 std::shared_ptr<void> provider;
 //Actual AssetManager.preloadSceneNode forwards to SAME SceneManager
 //PreloadScene(filename,NULL). No GameObject/spawn/visible-root publication.
 std::function<bool(const std::string&,std::string&)> preload_scene;
};
//Native backing of the real Character.s_cachedCharOIDs process map9a292c.
//GlobalC1 3aabc8..d8 constructs an empty tree. It is independent of Arrays
//membership, live/dead character counts, and Level loading readiness.
class CharacterOidCacheV81 {
 std::map<std::int32_t,std::uint32_t> counts_;
public:
 CharacterOidCacheV81()=default;
 CharacterOidCacheV81(const CharacterOidCacheV81&)=delete;
 CharacterOidCacheV81& operator=(const CharacterOidCacheV81&)=delete;
 std::uint32_t has(std::int32_t)const noexcept;
 //Whole AddCharOIDToCache3ab674 semantic tree/preload domain. Invalid IDs
 //return, existing entries store unsigned max and return without preloading.
 //A new entry is committed BEFORE model lookup/preload, including failures.
 bool add(std::int32_t,std::uint32_t,const data::CharacterTable&,
  const data::Dictionary&,const CharacterOidPreloadServicesV81&,std::string&);
 //Actual LevelD1 3f90e4/3f9280 clears this SAME global map. Resource list
 //cleanup belongs to SceneManager and is deliberately a separate authority.
 void clear_at_level_d1()noexcept{counts_.clear();}
 //SAME source tree, borrowed only by the original Level D1 clear journal.
 std::map<std::int32_t,std::uint32_t>& source_tree_for_level_d1_v88()noexcept{return counts_;}
 const std::map<std::int32_t,std::uint32_t>& source_entries()const noexcept{return counts_;}
};
//One native process storage owner; consumers retain its shared lease through
//callbacks/destruction. It is never recreated per level or skill.
std::shared_ptr<CharacterOidCacheV81> character_oid_cache_process_v81();
}
