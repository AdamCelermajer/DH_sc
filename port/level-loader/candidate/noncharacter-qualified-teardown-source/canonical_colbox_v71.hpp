#pragma once
#include <canonical_family_fields_v15.hpp>
#include <canonical_class_receiver_bindings_v1.hpp>
#include <game_object_initialization_owner_v1.hpp>
#include <game_object_set_position_v2.hpp>
namespace dh2::world {
struct ColBoxPhysicalReferenceV71 {std::uintptr_t actual{};std::shared_ptr<void> owner;};
struct ColBoxPhysicalArgumentsV71 {
 bool flag0{true},flag1{},flag2{},flag3{};
 std::int16_t short0{};std::uint16_t type0{1},mask0{0xffff};std::int32_t integer0{1};
};
// Genuine reached engine/native resource leaves. They act on the SAME actual
// canonical base/runtime and native physical facet; never construct ColBox.
struct ColBoxServicesV71 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> validate_current;
 std::function<bool(std::uintptr_t&,std::shared_ptr<void>&,std::string&)> physical_world;
 std::function<bool(std::uintptr_t,CanonicalGameObjectBaseOwnerV1&,const ColBoxPhysicalArgumentsV71&,
  ColBoxPhysicalReferenceV71&,std::string&)> construct_po_colmap;
 std::function<bool(const char*,bool&,std::string&)> debug_value;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uint32_t,std::uintptr_t,
  std::shared_ptr<void>&,std::string&)> destroy_native;
 // Dedicated genuine LuaScript D0 route for LoadExternalScript38ef60's
 // SAME LuaScriptC1(37c584) allocation in GameObject+300. Never physics.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t actual_lua_script,std::string&)> destroy_lua_script300;
 // Explicit failed/unassigned resource journal companion at its source-owned
 // native domain, after assigned D0/NULL. No original absence substitution.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uint32_t,std::string&)> destroy_unpublished_native_prefixes_v92;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> update_pf_object;
 std::function<bool(std::int16_t,std::int32_t,std::string&)> stop_sound;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_target_list;
 std::function<bool(actor::RuntimeState&,std::string&)> destroy_pf_object;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_object_base;
};
// Distinct original ColBox340fe0 -> GameObject20. Source declarations389938,
// InitPost3884fc, D1=3884b8 / D0=388e60, IsUpdatable3883f4 are recovered.
// Semantic native borrows; no ARM allocation/vtable ABI or substitute subclass.
class CanonicalColBoxV71 final {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 init_services_;
 GameObjectInitializationOwnerV1 initialization_;
 ColBoxServicesV71 services_;
 std::array<float,3> dimensions374_{};
 ColBoxPhysicalReferenceV71 pending_physical_;
 std::shared_ptr<void> pending_physical_pin_,physical_owner_;
 bool init_attempted_{},destroy_attempted_{},destroy_done_{};
 std::string destroy_error_;
 bool live(std::string&);
 bool set_physical_object(ColBoxPhysicalReferenceV71&,bool,std::string&);
 bool release_pointer(std::uint32_t,std::string&);
public:
 CanonicalColBoxV71(std::shared_ptr<void>,actor::RuntimeState&,
  GameObjectInitializationServicesV1,ColBoxServicesV71);
 CanonicalColBoxV71(const CanonicalColBoxV71&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> p){return base_.canonical(std::move(p));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);
 bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 bool write_vector3(std::uint32_t,const std::array<float,3>&,std::string&);
 std::array<float,3>& source_dimensions374()noexcept{return dimensions374_;}
 bool init_post(std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool is_updatable()const noexcept{return false;} // actual mov0;bx lr
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalColBoxV71>,std::shared_ptr<const void>);
};
}

