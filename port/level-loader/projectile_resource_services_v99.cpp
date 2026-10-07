#include "projectile_resource_services_v99.hpp"
#include "canonical_projectile_v99.hpp"
#include <game_object_source_destroy_v72.hpp>
#include <exception>
#include <tuple>
#include <utility>
namespace dh2::world {
namespace {
bool actual_projectile_v99(CanonicalProjectileV99& r,std::string& e){
 if(!r.base().identity()||r.base().type_f4()!=(r.is_laser_type()?10u:9u)){e="Required SAME canonical Projectile9/LaserTypeProjectile10 base";return false;}return true;
}
template<class F,class Live>F guarded_v99(F actual,Live live){
 if(!actual)return {};
 return [actual=std::move(actual),live](auto&&... args){
  auto tuple=std::forward_as_tuple(args...);auto& e=std::get<sizeof...(args)-1>(tuple);
  if(!live(e))return false;const bool ok=actual(std::forward<decltype(args)>(args)...);const auto saved=e;
  if(!live(e))return false;e=saved;return ok;
 };
}
}
bool projectile_init_post_v99(CanonicalProjectileV99& r,GameObjectInitializationServicesV1 init,ProjectileResourceServicesV99& s,std::string& e){
 if(!actual_projectile_v99(r,e))return false;
 if(s.receiver_&&s.receiver_!=r.base().identity()){e="Projectile resource bag belongs to another actual receiver";return false;}s.receiver_=r.base().identity();
 if(s.destroy_started_){e="Projectile InitPost after qualified destruction began";return false;}
 if(s.busy_){if(s.nested_failure_.empty())s.nested_failure_="Projectile source InitPost reentered";e=s.nested_failure_;return false;}
 s.busy_=true;struct Exit{bool& busy;~Exit(){busy=false;}}exit{s.busy_};
 auto live=[&](std::string& e){if(!s.nested_failure_.empty()){e=s.nested_failure_;return false;}
  if(!s.owner||!s.current){e="Required actual scoped Projectile resource authority";return false;}
  const bool ok=s.current(r,e);if(!s.nested_failure_.empty()){e=s.nested_failure_;return false;}return ok;
 };
 try{
  if(!live(e))return false;
  //Both original vtables inherit38be5c unchanged, including ObjectBase
  //conditions, cached spawn, transform/visual and signed-short sound order.
  init.condition_init=guarded_v99(init.condition_init,live);
  init.check_spawn_probability=guarded_v99(init.check_spawn_probability,live);
  init.set_position=guarded_v99(init.set_position,live);
  init.device_high_performance=guarded_v99(init.device_high_performance,live);
  init.load_visual=guarded_v99(init.load_visual,live);
  init.visual_sync=guarded_v99(init.visual_sync,live);
  GameObjectInitializationOwnerV1 inherited(r.base(),std::move(init));bool eligible{};
  if(!inherited.init_post(eligible,e))return false;return live(e);
 }catch(const std::exception& x){e=x.what();return false;}catch(...){e="Projectile inherited resource InitPost threw";return false;}
}
bool projectile_destroy_source_v99(CanonicalProjectileV99& r,ProjectileResourceServicesV99& s,std::string& e){
 if(s.destroy_done_){e.clear();return true;}
 if(!actual_projectile_v99(r,e))return false;
 if(s.receiver_&&s.receiver_!=r.base().identity()){e="Projectile D1 bag belongs to another actual receiver";return false;}s.receiver_=r.base().identity();
 if(s.busy_){if(s.nested_failure_.empty())s.nested_failure_="Projectile qualified D1 reentered";e=s.nested_failure_;return false;}
 if(!s.nested_failure_.empty()){e=s.nested_failure_;return false;}
 if(!s.owner||!s.current){e="Required actual Projectile D1 resource authority";return false;}
 if(!s.current(r,e))return false;s.destroy_started_=true;s.busy_=true;
 struct Exit{bool& busy;~Exit(){busy=false;}}exit{s.busy_};
 try{
  //LaserD1 3e4ac8 -> ProjectileD2 3e5e00 -> GameObjectD2 38d378.
  //ProjectileD1 3e5d9c -> SAME GameObjectD2. Neither deletes tail cells
  //374..3e0 or manager378; do not invent a manager/resource D0 there.
  //V107 owns existing per-offset native visual2d8/physical2dc/aux2e0,
  //Lua300, Vox370, strings/TargetList304/PF1c8/ObjectBase source order.
  bool done{};
  if(s.game_object_d2)done=s.game_object_d2(r.base(),e);
  else done=game_object_source_destroy_v72(r.base(),s.gameobject,s.actual_physical_owner_v99,e);
  if(!done)return false; //Actual partial release stays pinned; retry same owner.
  //Do not reborrow retired ObjectBase facets after the real D2 continuation.
  s.destroy_done_=true;
  if(!s.nested_failure_.empty()){e=s.nested_failure_;return false;}e.clear();return true;
 }catch(const std::exception& x){e=x.what();return false;}catch(...){e="Projectile qualified inherited D1 threw";return false;}
}
bool projectile_set_manager_v99(CanonicalProjectileV99& r,std::uintptr_t manager,std::string& e){
 if(!actual_projectile_v99(r,e))return false;
 if(!manager){e="Required original NULL SetManager assertion-mode continuation";return false;}
 r.source_manager378_v99()=manager;e.clear();return true;
}
bool projectile_set_manager_v99(CanonicalProjectileV99& r,std::uintptr_t manager,ProjectileResourceServicesV99& s,std::string& e){
 if(!actual_projectile_v99(r,e))return false;
 if(s.receiver_&&s.receiver_!=r.base().identity()){e="Projectile SetManager bag belongs to another receiver";return false;}s.receiver_=r.base().identity();
 if(s.destroy_started_){e="Projectile SetManager after qualified destruction began";return false;}
 if(!s.owner||!s.current||!s.current(r,e)){if(e.empty())e="Required actual current Projectile SetManager authority";return false;}
 if(manager)return projectile_set_manager_v99(r,manager,e); //3e4ffc direct378 store.
 if(s.manager_busy_){e="Projectile NULL SetManager assertion delivery reentered";return false;}
 s.manager_busy_=true;struct Exit{bool& busy;~Exit(){busy=false;}}exit{s.manager_busy_};
 try{
  std::int32_t mode{};if(!s.null_manager_assertion_mode||!s.null_manager_assertion_mode(mode,e)){if(e.empty())e="Required original SetManager NULL assertion-mode global";return false;}
  if(!s.current(r,e))return false;
  if(mode==2){e="Original SetManager(NULL) fatal assertion writes through NULL; source store interrupted";return false;}
  if(mode==1){if(!s.report_null_manager_assertion||!s.report_null_manager_assertion(r,e)){if(e.empty())e="Required original SetManager NULL assertion report";return false;}if(!s.current(r,e))return false;}
  r.source_manager378_v99()=0;e.clear();return true; //Native remaining modes and returned report path.
 }catch(const std::exception& x){e=x.what();return false;}catch(...){e="Projectile NULL SetManager assertion provider threw";return false;}
}
}
