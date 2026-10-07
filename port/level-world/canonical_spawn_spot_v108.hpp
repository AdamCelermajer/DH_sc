#pragma once
#include "canonical_dummy_owner_v14.hpp"
#include "canonical_family_fields_v15.hpp"
namespace dh2::world {
class CanonicalSpawnSpotV108;
struct SpawnSpotServicesV108 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalSpawnSpotV108&,std::string&)> insert,erase;
 std::function<bool(std::uintptr_t,const float*,bool,std::string&)> set_object_position;
 std::function<bool(std::uintptr_t,const float*,std::string&)> set_object_rotation;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_base;
};
class CanonicalSpawnSpotV108 {
 CanonicalGameObjectBaseOwnerV1 base_;GameObjectInitializationServicesV1 init_services_;
 GameObjectInitializationOwnerV1 initialization_;SpawnSpotServicesV108 services_;
 std::string group374_;bool destruction_started_{},destroyed_{};
public:
 CanonicalSpawnSpotV108(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,SpawnSpotServicesV108);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 const std::string& group()const noexcept{return group374_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> pin){return base_.canonical(std::move(pin));}
 CanonicalPropertyActorV1 properties()noexcept{return canonical_family_fields_v15(*this);}
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);
 bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 bool init_post(std::string& e){e.clear();return true;} //3ea7fc literal BX LR
 bool init_final(bool&,std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool place_object(std::uintptr_t,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalSpawnSpotV108>,std::shared_ptr<const void>);
};
}
