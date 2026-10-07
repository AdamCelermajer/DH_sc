#pragma once
#include <projectile_manager_owner_v108.hpp>
#include <projectile_precache_source_v96.hpp>
namespace model_renderer {
//Source manager algorithm is inside SAME V108 owner. Main lends actual
//ObjectManager.Spawn/class projection/SetManager and reached diagnostic only.
inline bool bind_actual_projectile_manager_preload_v96(
 dh2::world::ProjectileManagerOwnerV108::CreateNativeV96 native,
 std::function<bool(std::string&)> current,
 dh2::world::ProjectileManagerOwnerV108::MissingProjectileV96 missing,
 dh2::loader::ProjectilePrecacheSourcesV96& out,std::string& e){
 if(!native.owner||!current||
    out.owner||out.validate_current||out.borrow_manager||out.table_owner||
    out.table_count||out.table_fx_set14||out.register_fx_set){
  e="Require unbound independent actual Projectile Spawn/receiver primitive authority";return false;
 }
 dh2::loader::ProjectilePrecacheSourcesV96 source;source.owner=native.owner;source.validate_current=current;
 source.borrow_manager=[current=std::move(current),native=std::move(native),missing=std::move(missing)](
  dh2::loader::ProjectileManagerPrecacheBorrowV96& out,std::string& error){
  auto actual=dh2::world::projectile_manager_process_v108();
  if(!actual){error="Required SAME process ProjectileManager9a2878";return false;}
  dh2::loader::ProjectileManagerPrecacheBorrowV96 loan;loan.owner=actual;loan.identity=actual->identity();
  const auto weak=std::weak_ptr<dh2::world::ProjectileManagerOwnerV108>(actual);
  for(unsigned pool=0;pool<2;++pool){const bool laser=pool!=0;auto& fields=loan.pools[pool];
   fields.capacity=[weak,laser](std::size_t& n,std::string& e){auto p=weak.lock();if(!p){e="Actual Projectile pool owner expired";return false;}return p->pool_capacity_v96(laser,n,e);};
   fields.count=[weak,laser](std::size_t& n,std::string& e){auto p=weak.lock();if(!p){e="Actual Projectile pool owner expired";return false;}return p->pool_count_v96(laser,n,e);};
   fields.reserve=[weak,laser](std::size_t n,std::string& e){auto p=weak.lock();if(!p){e="Actual Projectile pool owner expired";return false;}return p->pool_reserve_v96(laser,n,e);};
   fields.entry=[weak,laser](std::size_t n,std::uintptr_t& id,std::shared_ptr<void>& pin,std::string& e){auto p=weak.lock();if(!p){e="Actual Projectile pool owner expired";return false;}return p->pool_entry_v96(laser,n,id,pin,e);};
  }
  loan.create=[weak,current,native](bool first,bool laser,std::uintptr_t& id,std::string& e){auto p=weak.lock();if(!p){e="Actual Projectile _Create owner expired";return false;}return p->create_source_v96(first,laser,native,current,id,e);};
  loan.despawn=[weak,current,missing](std::uintptr_t id,bool laser,std::string& e){auto p=weak.lock();if(!p){e="Actual Projectile DeSpawn owner expired";return false;}return p->despawn_source_v96(id,laser,current,missing,e);};
  out=std::move(loan);error.clear();return true;
 };
 out=std::move(source);e.clear();return true;
}
}
