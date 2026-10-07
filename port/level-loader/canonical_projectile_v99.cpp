#include "canonical_projectile_v99.hpp"
#include "projectile_resource_services_v99.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::world {
CanonicalProjectileV99::CanonicalProjectileV99(std::shared_ptr<void> services,actor::RuntimeState& runtime,bool laser)
 :base_(reinterpret_cast<std::uintptr_t>(this),laser?10:9,std::move(services),runtime),laser_type_(laser){
 //Source 3e5e40/C2: same inherited GameObject C2 has completed.
 words_[(0x3c8-0x374)/4]=0;words_[0]=UINT32_MAX;
 byte3d1_=0;*base_.byte(0x85)=1;manager378_=0;
 for(auto offset:{0x380u,0x384u,0x3a4u,0x3b8u,0x3ccu})*source_pointer_v99(offset)=0;
 for(auto offset:{0x388u,0x38cu,0x390u,0x394u,0x398u,0x39cu,0x3c4u})words_[(offset-0x374)/4]=0;
 byte3d0_=0;
}
CanonicalProjectileV99::CanonicalProjectileV99(std::shared_ptr<void> owner,actor::RuntimeState& runtime)
 :CanonicalProjectileV99(std::move(owner),runtime,false){}
CanonicalLaserTypeProjectileV99::CanonicalLaserTypeProjectileV99(std::shared_ptr<void> owner,actor::RuntimeState& runtime)
 :CanonicalProjectileV99(std::move(owner),runtime,true){
 //Laser3e4b6c continues that SAME base, no second Projectile receiver.
 *source_pointer_v99(0x3dc)=0;*source_pointer_v99(0x3d4)=0;*source_word_v99(0x3e0)=UINT32_MAX;*source_pointer_v99(0x3d8)=0;
}
CanonicalProjectileV99::~CanonicalProjectileV99()=default;
std::uint32_t* CanonicalProjectileV99::source_word_v99(std::uint32_t offset)noexcept{
 if(offset<0x374||offset>=(laser_type_?0x3e4u:0x3d4u)||(offset-0x374)%4||source_pointer_v99(offset)||offset==0x3d0)return nullptr;
 return &words_[(offset-0x374)/4]; //unwritten opaque C1 words may only be lent to a real producer, not read as defaults.
}
std::uintptr_t* CanonicalProjectileV99::source_pointer_v99(std::uint32_t offset)noexcept{
 if(offset==0x378)return &manager378_;
 constexpr std::array<std::uint32_t,10> offsets{0x380,0x384,0x3a4,0x3b8,0x3bc,0x3c0,0x3cc,0x3d4,0x3d8,0x3dc};
 for(unsigned i=0;i<offsets.size();++i)if(offset==offsets[i]&&(i<7||laser_type_))return &native_pointers_v112_[i];
 return base_.pointer(offset);
}
std::uint8_t* CanonicalProjectileV99::source_byte_v99(std::uint32_t offset)noexcept{
 if(offset==0x37c)return reinterpret_cast<std::uint8_t*>(&words_[(0x37c-0x374)/4]); //same source byte, not a copied word.
 if(offset==0x3d0)return &byte3d0_;if(offset==0x3d1)return &byte3d1_;return base_.byte(offset);
}
bool CanonicalProjectileV99::retain_resources_v99(std::shared_ptr<ProjectileResourceServicesV99> resources,std::string& e){
 if(!resources||(resources_&&(resources_.get()!=resources.get()||resources_.owner_before(resources)||resources.owner_before(resources_)))){e="Required SAME persistent Projectile resource prefix";return false;}
 resources_=std::move(resources);e.clear();return true;
}
bool CanonicalProjectileV99::bind_platform_v99(GameObjectInitializationServicesV1 init,GameObjectSetPositionServicesV2 position,
 std::shared_ptr<ProjectileResourceServicesV99> resources,std::string& e){
 if(bound_||!init.owner||!position.owner||!resources){e="Required once-bound SAME Projectile platform/resource services";return false;}
 if(!retain_resources_v99(std::move(resources),e))return false;
 initialization_=std::move(init);position_=std::move(position);bound_=true;e.clear();return true;
}
bool CanonicalProjectileV99::init_post(std::string& e){
 if(!bound_||!resources_){e="Required actual Projectile inherited GameObject.InitPost services";return false;}
 return projectile_init_post_v99(*this,initialization_,*resources_,e);
}
bool CanonicalProjectileV99::init_final(std::string& e){
 if(!bound_){e="Required actual Projectile inherited InitFinal services";return false;}
 bool eligible{};GameObjectInitializationOwnerV1 source(base_,initialization_);return source.init_final(eligible,e);
}
bool CanonicalProjectileV99::destroy_source(std::string& e){
 if(!resources_){e="Required SAME Projectile qualified GameObject D2 services";return false;}
 return projectile_destroy_source_v99(*this,*resources_,e);
}
bool CanonicalProjectileV99::set_manager(std::uintptr_t manager,std::string& e){
 if(resources_)return projectile_set_manager_v99(*this,manager,*resources_,e);
 return projectile_set_manager_v99(*this,manager,e); //positive pure source setter; NULL still requires real policy.
}
bool CanonicalProjectileV99::set_position(const std::array<float,3>& point,bool destination,std::string& e){
 if(!bound_){e="Required SAME Projectile SetPosition services";return false;}
 return game_object_set_position_v2(base_,point.data(),destination,position_,e);
}
bool CanonicalProjectileV99::position(std::array<float,3>& out,std::string& e)const{
 const auto& runtime=const_cast<CanonicalGameObjectBaseOwnerV1&>(base_).runtime();
 std::memcpy(out.data(),runtime.subobjects.position,12);e.clear();return true;
}
bool CanonicalProjectileV99::source_loading_fields_v95(std::shared_ptr<void> pin,CanonicalObjectLoadingFieldsV95& out,std::string& e){
 if(!pin||pin.get()!=this){e="Required SAME pinned Projectile source-loading receiver";return false;}
 out={};out.receiver=std::move(pin);out.gameobject_base=&base_;out.archetype48=base_.string(0x48);out.enabled8a=base_.byte(0x8a);
 out.minimum_ec=base_.integer(0xec);out.disabled_f1=base_.byte(0xf1);out.condition_a8=base_.pointer(0xa8);out.condition_cc=base_.pointer(0xcc);
 out.tested_ac=base_.byte(0xac);out.tested_d0=base_.byte(0xd0);e.clear();return true;
}
bool CanonicalProjectileV99::source_is_updatable_v95(bool& out,std::string& e){out=true;e.clear();return true;} //literal3e3e0c
bool CanonicalProjectileV99::source_is_zonable_v99(bool& out,std::string& e){out=false;e.clear();return true;} //literal3e3e04
bool CanonicalProjectileV99::source_save_borrow_v99(ObjectSaveRestoreBorrowV3& out,std::string& e){return canonical_gameobject_save_borrow_v3(base_,out,e);}
}
