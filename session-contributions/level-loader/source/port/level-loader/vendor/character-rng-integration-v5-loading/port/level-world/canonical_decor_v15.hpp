#pragma once
#include "canonical_spawn_point_v15.hpp"
namespace dh2::world {
struct DecorServicesV15 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync;
 std::function<bool(std::uintptr_t,bool&,std::string&)> visual_physical;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t&,std::string&)> construct_podecor;
 std::function<bool(std::uintptr_t,bool,std::string&)> set_physical;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> visual_root;
 // PFWorld.LoadRoom returns genuine nullable room and its SAME flags24.
 std::function<bool(std::uintptr_t,std::int32_t,const std::string&,std::uintptr_t&,std::uint32_t*&,std::string&)> load_room;
 std::function<bool(std::uintptr_t,const float*,std::string&)> extend_bounds;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_base;
};
class CanonicalDecorV15 {
 CanonicalGameObjectBaseOwnerV1 base_;GameObjectInitializationServicesV1 init_services_;GameObjectInitializationOwnerV1 initialization_;DecorServicesV15 services_;
 std::uint8_t load_floor375_{1},solid376_{1};bool destroyed_{};
public:
 CanonicalDecorV15(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,DecorServicesV15);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> l){return base_.canonical(std::move(l));}
 CanonicalPropertyActorV1 properties()noexcept{return canonical_family_fields_v15(*this);}
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);bool write_string(std::uint32_t,const std::string&,std::string&);
 // Exact inherited field borrow for derived342680 store; no new field.
 std::uint8_t& source_load_floor375()noexcept{return load_floor375_;}
 std::uint8_t load_floor()const noexcept{return load_floor375_;}std::uint8_t solid()const noexcept{return solid376_;}
 bool init_post(std::string&);bool init_final(bool& b,std::string& e){return initialization_.init_final(b,e);}
 bool load_floor_map(std::string&);bool set_position(const std::array<float,3>&,bool,std::string&);bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalDecorV15>,std::shared_ptr<const void>);
};
}
