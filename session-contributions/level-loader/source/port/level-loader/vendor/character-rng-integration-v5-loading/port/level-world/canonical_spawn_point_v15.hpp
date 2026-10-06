#pragma once
#include "canonical_dummy_owner_v14.hpp"
#include "canonical_family_fields_v15.hpp"
namespace dh2::world {
struct SpawnPointServicesV15 {
 std::shared_ptr<void> owner;
 std::function<bool(const std::string&,bool,std::int32_t&,std::string&)> script_id;
 std::function<bool(std::uintptr_t,const float*,bool,std::string&)> set_object_position;
 std::function<bool(std::uintptr_t,const float*,std::string&)> set_object_rotation;
 std::function<bool(std::int32_t,std::int32_t,bool,std::string&)> start_script;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_base;
};
class CanonicalSpawnPointV15 {
 CanonicalGameObjectBaseOwnerV1 base_;GameObjectInitializationServicesV1 init_services_;
 GameObjectInitializationOwnerV1 initialization_;SpawnPointServicesV15 services_;
 std::int32_t entrypoint374_{-1},script390_{-1};std::string script378_;bool destroyed_{};
public:
 CanonicalSpawnPointV15(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,SpawnPointServicesV15);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> l){return base_.canonical(std::move(l));}
 CanonicalPropertyActorV1 properties()noexcept{return canonical_family_fields_v15(*this);}
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);bool write_string(std::uint32_t,const std::string&,std::string&);
 std::int32_t entrypoint()const noexcept{return entrypoint374_;}std::int32_t script_id()const noexcept{return script390_;}
 const std::string& script_name()const noexcept{return script378_;}
 bool init_post(std::string&);bool init_final(bool& b,std::string& e){return initialization_.init_final(b,e);}
 bool set_position(const std::array<float,3>&,bool,std::string&);bool place_object(std::uintptr_t,std::string&);bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalSpawnPointV15>,std::shared_ptr<const void>);
};
}
