#pragma once
#include <canonical_colbox_v71.hpp>
#include <game_object_initialization_owner_v1.hpp>
#include <cstdint>
#include <memory>
#include <functional>
#include <string>
namespace dh2::world {
class CanonicalProjectileV99;
//One bag retained by the SAME canonical receiver before resource preparation.
//Independent engine-leaf owner, weak actual receiver/World callback captures.
//Both original classes inherit GameObject.InitPost/InitFinal; neither has an
//additional ProjectileTable/startup body or tail-owned D1 allocation.
struct ProjectileResourceServicesV99 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalProjectileV99&,std::string&)> current;
 //Established qualified shared GameObjectD2 source algorithm (V107). This
 //receives ONLY this actual inherited base, never a whole Projectile delegate.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> game_object_d2;
 ColBoxServicesV71 gameobject; //optional established per-offset V72 fallback.
 //SetManager(NULL): original assertion mode global, not a guessed default.
 std::function<bool(std::int32_t&,std::string&)> null_manager_assertion_mode;
 std::function<bool(CanonicalProjectileV99&,std::string&)> report_null_manager_assertion;
 //Actual physical facet may be supplied by the real runtime owner. A failed
 //native release keeps it and the admitted per-offset continuation pinned.
 std::shared_ptr<void> actual_physical_owner_v99;
private:
 std::uintptr_t receiver_{};
 bool busy_{},manager_busy_{},destroy_started_{},destroy_done_{};
 std::string nested_failure_;
 friend bool projectile_init_post_v99(CanonicalProjectileV99&,GameObjectInitializationServicesV1,ProjectileResourceServicesV99&,std::string&);
 friend bool projectile_destroy_source_v99(CanonicalProjectileV99&,ProjectileResourceServicesV99&,std::string&);
 friend bool projectile_set_manager_v99(CanonicalProjectileV99&,std::uintptr_t,ProjectileResourceServicesV99&,std::string&);
public:
 bool destroy_complete_v99()const noexcept{return destroy_done_;}
 bool destroy_started_v99()const noexcept{return destroy_started_;}
};
bool projectile_init_post_v99(CanonicalProjectileV99&,GameObjectInitializationServicesV1,ProjectileResourceServicesV99&,std::string&);
bool projectile_destroy_source_v99(CanonicalProjectileV99&,ProjectileResourceServicesV99&,std::string&);
bool projectile_set_manager_v99(CanonicalProjectileV99&,std::uintptr_t,ProjectileResourceServicesV99&,std::string&);
//Authentic nonNULL branch has no manager callback/query. NULL callers need the
//typed overload above so original assertion mode/report semantics are explicit.
bool projectile_set_manager_v99(CanonicalProjectileV99&,std::uintptr_t,std::string&);
}
