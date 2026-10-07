#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <canonical_class_receiver_bindings_v1.hpp>
#include <game_object_initialization_owner_v1.hpp>
#include <game_object_set_position_v2.hpp>
#include <object_save_restore_v3.hpp>
#include <array>
namespace dh2::world {
struct ProjectileResourceServicesV99;
//Native factory340d14 allocates3d4/C13e5e40(ID9); factory340cf0 allocates
//3e4/LaserC13e4b6c(ID10) then SAME ProjectileC23e5edc/GameObjectC238c398.
//This owns constructor storage, never an ARM executable record or a second base.
class CanonicalProjectileV99 {
 CanonicalGameObjectBaseOwnerV1 base_;
 bool laser_type_{};
 //Opaque source words remain uninitialized unless original C1 wrote them.
 //Native pointer378 has a distinct native-width cell, never a uint32 alias.
 std::array<std::uint32_t,(0x3e4-0x374)/4> words_;
 std::uintptr_t manager378_;
 std::array<std::uintptr_t,10> native_pointers_v112_; //380,384,3a4,3b8,3bc,3c0,3cc,3d4,3d8,3dc; unwritten callback cells remain opaque.
 std::uint8_t byte3d0_,byte3d1_;
 GameObjectInitializationServicesV1 initialization_;
 GameObjectSetPositionServicesV2 position_;
 std::shared_ptr<ProjectileResourceServicesV99> resources_;
 bool bound_{};
protected:
 CanonicalProjectileV99(std::shared_ptr<void>,actor::RuntimeState&,bool laser);
public:
 CanonicalProjectileV99(std::shared_ptr<void>,actor::RuntimeState&);
 virtual ~CanonicalProjectileV99(); //host storage ONLY; never claimed as native D0.
 CanonicalProjectileV99(const CanonicalProjectileV99&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 const CanonicalGameObjectBaseOwnerV1& base()const noexcept{return base_;}
 actor::RuntimeState& runtime()noexcept{return base_.runtime();}
 bool is_laser_type()const noexcept{return laser_type_;}
 std::uint32_t* source_word_v99(std::uint32_t)noexcept;
 std::uintptr_t* source_pointer_v99(std::uint32_t)noexcept;
 std::uint8_t* source_byte_v99(std::uint32_t)noexcept;
 std::uintptr_t& source_manager378_v99()noexcept{return manager378_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> pin){return base_.canonical(std::move(pin));}
 CanonicalPropertyActorV1 properties()noexcept{return base_.properties();} //both original virtual18 inherit GameObject.
 bool retain_resources_v99(std::shared_ptr<ProjectileResourceServicesV99>,std::string&);
 bool bind_platform_v99(GameObjectInitializationServicesV1,GameObjectSetPositionServicesV2,
  std::shared_ptr<ProjectileResourceServicesV99>,std::string&);
 bool init_post(std::string&);
 bool init_final(std::string&);
 bool destroy_source(std::string&);
 bool set_manager(std::uintptr_t,std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool position(std::array<float,3>&,std::string&)const;
 bool source_loading_fields_v95(std::shared_ptr<void>,CanonicalObjectLoadingFieldsV95&,std::string&);
 bool source_is_updatable_v95(bool&,std::string&);
 bool source_is_zonable_v99(bool&,std::string&);
 bool source_save_borrow_v99(ObjectSaveRestoreBorrowV3&,std::string&);
};
class CanonicalLaserTypeProjectileV99 final:public CanonicalProjectileV99 {
public:
 CanonicalLaserTypeProjectileV99(std::shared_ptr<void>,actor::RuntimeState&);
};
}
