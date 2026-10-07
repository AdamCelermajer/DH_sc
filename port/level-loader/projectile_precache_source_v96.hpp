#pragma once
#include "stage_loader_v46_early.hpp"
#include <array>
#include <limits>
namespace dh2::loader {
struct ProjectilePrecachePoolBorrowV96 {
 // SAME Main vector pool10 or24. No storage/vector/registry is constructed.
 std::function<bool(std::size_t&,std::string&)> capacity,count;
 std::function<bool(std::size_t,std::string&)> reserve;
 std::function<bool(std::size_t,std::uintptr_t&,std::shared_ptr<void>&,std::string&)> entry;
};
struct ProjectileManagerPrecacheBorrowV96 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::array<ProjectilePrecachePoolBorrowV96,2> pools;
 // _Create(first=false, laser_type=pool) return is ignored by native PreCache.
 // These are actual Main Spawn/class constructors, not allocation stand-ins.
 std::function<bool(bool,bool,std::uintptr_t&,std::string&)> create;
 std::function<bool(std::uintptr_t,bool,std::string&)> despawn;
};
struct ProjectilePrecacheSourcesV96 {
 std::shared_ptr<void> owner; // independent actual Main primitive authority
 std::function<bool(std::string&)> validate_current; // actual epoch/cancel scope
 std::function<bool(ProjectileManagerPrecacheBorrowV96&,std::string&)> borrow_manager;
 std::shared_ptr<void> table_owner;
 std::function<bool(std::uint32_t&,std::string&)> table_count;
 // Read CURRENT same member row+14 at each reached point, not a copied table.
 std::function<bool(std::uint32_t,std::int32_t&,std::string&)> table_fx_set14;
 std::function<bool(std::int32_t,std::string&)> register_fx_set;
};
// Original ProjectileManager.PreCache3e71cc source order only. Main retains
// real _Create/DeSpawn and their functional object/visibility/Stop leaves.
class ProjectilePrecacheBodyV96 final {
 ProjectilePrecacheSourcesV96 source_;ProjectileManagerPrecacheBorrowV96 manager_;
 bool attempted_{},failed_{},busy_{},completed_{};std::string failure_;
 bool fail(std::string& e,const char* missing){failed_=true;if(failure_.empty())failure_=e.empty()?missing:e;e=failure_;return false;}
 bool live(std::string& e){if(failed_){e=failure_;return false;}const bool valid=source_.owner&&source_.validate_current&&source_.validate_current(e);if(failed_){e=failure_;return false;}return valid;}
public:
 explicit ProjectilePrecacheBodyV96(ProjectilePrecacheSourcesV96 source):source_(std::move(source)){}
 bool precache(std::string& e){
  if(failed_){e=failure_;return false;}if(busy_||attempted_)return fail(e,"ProjectileManager.PreCache source prefix cannot replay");
  attempted_=busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
  try{
   if(!live(e)||!source_.borrow_manager||!source_.borrow_manager(manager_,e)||!live(e)||!manager_.owner||!manager_.identity)return fail(e,"Required SAME actual Main ProjectileManager receiver/epoch");
   for(unsigned pool=0;pool<2;++pool){auto& actual=manager_.pools[pool];std::size_t capacity{};
    if(!actual.capacity||!actual.capacity(capacity,e)||!live(e))return fail(e,"Required actual source projectile vector capacity10/24");
    if(capacity<10){if(!actual.reserve||!actual.reserve(10,e)||!live(e))return fail(e,"Required SAME projectile pool reserve10");}
    // Exactly ten source calls, not a target count or ten invented receivers.
    for(unsigned i=0;i<10;++i){std::uintptr_t ignored{};if(!manager_.create||!manager_.create(false,pool!=0,ignored,e)||!live(e))return fail(e,"Required actual ProjectileManager._Create(false,laser_type)");}
    std::size_t count{};if(!actual.count||!actual.count(count,e)||!live(e))return fail(e,"Required actual post-Create projectile pool length");
    for(std::size_t i=0;i<count;++i){std::uintptr_t identity{};std::shared_ptr<void> receiver;
     if(!actual.entry||!actual.entry(i,identity,receiver,e)||!live(e))return fail(e,"Required current SAME projectile vector entry");
     if(identity&&!receiver)return fail(e,"Required live actual projectile receiver loan");
     // Native invokes DeSpawn even for NULL; its genuine assertion path is Main.
     if(!manager_.despawn||!manager_.despawn(identity,pool!=0,e)||!live(e))return fail(e,"Required actual ProjectileManager.DeSpawn");
    }
   }
   std::uint32_t count{};if(!source_.table_owner||!source_.table_count||!source_.table_count(count,e)||!live(e))return fail(e,"Required actual Arrays.ProjectileTable size");
   for(std::uint32_t i=0;i<count;++i){std::int32_t set{};
    if(!source_.table_fx_set14||!source_.table_fx_set14(i,set,e)||!live(e))return fail(e,"Required current SAME ProjectileTable row14 FXSet");
    if(!source_.register_fx_set||!source_.register_fx_set(set,e)||!live(e))return fail(e,"Required SAME VisualFXManager.RegisterFXSetToLoad");
   }
  }catch(const std::exception& ex){e=ex.what();return fail(e,"Projectile precache leaf threw");}catch(...){return fail(e,"Projectile precache leaf threw");}
  completed_=true;e.clear();return true;
 }
 bool completed()const noexcept{return completed_;}
};
struct Stage29PrecacheServicesV96 {
 EarlyLoadingDebugV46 debug;
 std::function<bool(std::string&)> validate_current;
 std::function<bool(std::string&)> actual_item_precache;
 // Borrow AFTER genuine Item145 prefix; absent manager/resources fail there.
 std::function<bool(ProjectilePrecacheSourcesV96&,std::string&)> projectile_sources;
};
inline std::function<LifecycleStepV36(std::string&)> stage29_precache_source_v96(Stage29PrecacheServicesV96 source){
 return [source=std::move(source),busy=false,attempted=false,failed=false,failure=std::string{},projectiles=std::shared_ptr<ProjectilePrecacheBodyV96>{}](std::string& e)mutable{
  auto fail=[&](const char* missing){failed=true;if(failure.empty())failure=e.empty()?missing:e;e=failure;return LifecycleStepV36::failed;};
  if(failed){e=failure;return LifecycleStepV36::failed;}if(busy||attempted)return fail("Stage29 source prefix cannot replay");attempted=busy=true;
  struct Guard{bool& b;~Guard(){b=false;}} guard{busy};
  auto live=[&]{if(failed){e=failure;return false;}const bool valid=source.validate_current&&source.validate_current(e);if(failed){e=failure;return false;}return valid;};
  try{
   if(!live()||!early_loading_trace_v46(source.debug,e)||!live())return fail("Required actual state29 debug/current scope");
   if(!source.actual_item_precache||!source.actual_item_precache(e)||!live())return fail("Required genuine current ItemManager145 PreCache");
   ProjectilePrecacheSourcesV96 actual;if(!source.projectile_sources||!source.projectile_sources(actual,e)||!live())return fail("Required actual ProjectileManager precache services AFTER Item prefix");
   projectiles=std::make_shared<ProjectilePrecacheBodyV96>(std::move(actual));
   if(!projectiles->precache(e)||!live())return fail("Required complete original ProjectileManager precache body");
  }catch(const std::exception& ex){e=ex.what();return fail("Stage29 source leaf threw");}catch(...){return fail("Stage29 source leaf threw");}
  e.clear();return LifecycleStepV36::complete; // dispatcher increments130
 };
}
}
