#pragma once
#include <canonical_class_receiver_bindings_v1.hpp>
#include <game_object_initialization_owner_v1.hpp>
namespace dh2::world {
struct FloorServicesV72 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> validate_current;
 std::function<bool(std::uintptr_t,std::string&)> visual_apply_mesh_box;
 std::function<bool(std::uintptr_t,bool,std::string&)> visual_set_visible;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> visual_root;
 std::function<bool(std::uintptr_t,bool,std::uint32_t&,std::string&)> get_node_poly_count;
 // Actual GameObjectD2 engine/resource leaves;300 can ONLY be LuaScript D0.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uint32_t,std::uintptr_t,std::shared_ptr<void>&,std::string&)> destroy_native;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::string&)> destroy_lua_script300;
 std::function<bool(std::int16_t,std::int32_t,std::string&)> stop_sound;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_target_list;
 std::function<bool(actor::RuntimeState&,std::string&)> destroy_pf_object;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> destroy_object_base;
};
// Distinct original Floor34104c -> GameObjectC1(20), static84=1. Actual
// vtable964320 aliases GameObject DeclareProperties38cee8; InitPost3886b4;
// D1=388470/D0=388e10; IsUpdatable3883e4 and IsZonable3883ec both literal0.
class CanonicalFloorV72 final {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 init_services_;
 GameObjectInitializationOwnerV1 initialization_;
 FloorServicesV72 services_;
 std::shared_ptr<void> physical_owner_;
 bool init_attempted_{},destroy_attempted_{},destroy_done_{};
 std::string destroy_error_;
 bool live(std::string&);
public:
 CanonicalFloorV72(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,FloorServicesV72);
 CanonicalFloorV72(const CanonicalFloorV72&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> p){return base_.canonical(std::move(p));}
 CanonicalPropertyActorV1 properties()noexcept{return base_.properties();}
 bool init_post(std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool is_updatable()const noexcept{return false;}
 bool is_zonable()const noexcept{return false;}
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalFloorV72>,std::shared_ptr<const void>);
};
}
