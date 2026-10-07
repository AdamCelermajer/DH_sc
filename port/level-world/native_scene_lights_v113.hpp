#pragma once
#include "light_set_name_owner_v3.hpp"
#include "character_target_providers.hpp"
#include <array>
#include <functional>
#include <memory>
#include <vector>
#include "../engine-resources/resources.hpp"
#include <optional>
namespace dh2::scene {struct Scene;}
namespace dh2::world {
struct NativeLightNodeV113;
//Actual CLight59fe08 fields. Node's retained transformation owns position;
//no duplicated Character or guessed renderer light lives in these values.
struct NativeLightV113 {
 std::uint32_t references0{}; //59fe08 C1=0; allocate59feb8 actual first grab.
 bool native_destroyed_v113{};
 std::array<float,4> ambient4{0,0,0,1},diffuse14{1,1,1,1},specular24{1,1,1,1};
 std::array<float,3> attenuation34{1,0,0};
 float radius40{},falloff44{},cutoff48{},exponent4c{2};
 std::uint16_t type58{};std::uint8_t enabled5a{1};
 //Actual CLight50 transformation source; node constructor must publish it.
 std::function<bool(std::array<float,16>&,std::string&)> transformation50;
 NativeLightV113();
 bool grab(std::string&);
 void drop()noexcept;
};
//Actual CLightSceneNode5840f0/CLight.allocate59feb8 resource. Native actor,
//Scene parent and shader-light references are distinct from diagnostic pins.
class NativeSceneLightNodeV113 final:public std::enable_shared_from_this<NativeSceneLightNodeV113> {
 std::uint32_t references158_{1};bool destroyed_{},dirty_{true};
 std::shared_ptr<NativeLightV113> light134_;
 std::array<float,3> positionac_{}; //ISceneNode relative translationac; absolute matrix24 is world_ below
 std::array<float,16> world_{};
 std::uint32_t flags11c_{0x60f};std::uintptr_t parentec_{},manager110_{};
 std::uint8_t local120_{1},parent121_{1}; //ISceneNodeC1 599230/59924c, distinct from flags11c
 std::string name24_v113_; //actual inherited C1-empty node name, independent of library SLight ID
 std::array<float,6> box13c_{-1,-1,-1,1,1,1};std::optional<std::uint32_t> culling118_;
 std::uint16_t type138_{};
 bool recalc(std::string&);
public:
 NativeSceneLightNodeV113();
 static bool construct(std::shared_ptr<NativeSceneLightNodeV113>&,std::string&);
 bool load_light(const resources::BresView&,std::uint32_t,std::string&);
 bool grab(std::string&);void drop()noexcept;
 bool live()const noexcept{return !destroyed_;}
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 std::uint32_t& flags()noexcept{return flags11c_;}
 std::uintptr_t& parent()noexcept{return parentec_;}
 bool notify_parent_visibility_v113(bool value,std::string& e){if(destroyed_){e="Retired light visibility receiver";return false;}parent121_=value?1:0;e.clear();return true;}
 bool set_scene_manager_v113(std::uintptr_t value,std::string& e){if(destroyed_){e="Retired light SceneManager receiver";return false;}manager110_=value;e.clear();return true;}
 const std::string& source_name_v113()const noexcept{return name24_v113_;}
 const std::shared_ptr<NativeLightV113>& light()const noexcept{return light134_;}
 bool set_position(const std::array<float,3>&,std::string&);
 bool set_authored_matrix_v113(const std::array<float,16>&,std::string&);
 bool bind_source_matrix_v113(std::function<bool(std::array<float,16>&,std::string&)>,std::string&);
 bool position(std::array<float,3>&,std::string&);
 bool matrix(std::array<float,16>&,std::string&);
 bool scene_phase(std::string&);
 bool set_type(std::uint16_t,std::string&);
 NativeLightNodeV113 borrow();
};
struct RetainedVisualNodeV91;struct RetainedMeshNodeV91;
//Construct source tag4 children on the existing Root/Scene graph. The callback
//lends RootSceneNode.light188 ID lookup; no ObjectManager or Scene is created.
bool construct_retained_scene_lights_v113(const resources::BresView&,const scene::Scene&,
 const std::vector<std::shared_ptr<RetainedVisualNodeV91>>&,
 const std::vector<std::shared_ptr<RetainedMeshNodeV91>>&,std::string&);
struct NativeLightNodeV113 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 std::shared_ptr<NativeLightV113> light;
 std::function<bool(std::array<float,3>&,std::string&)> position;
 std::function<bool(const std::array<float,3>&,std::string&)> set_position;
};
struct NativeLightUpdateServicesV113 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,bool&,std::string&)> debug;
 std::function<bool(target_providers::Handle16&,bool&,std::array<float,3>&,std::string&)> resolve_position;
};
//Source LightSetManager field successor. Adopts the already executed SAME
//name-C1; it neither copies names nor constructs a parallel name authority.
class NativeLightSetV113 {
 struct Static {NativeLightNodeV113 node;};
 struct Active {NativeLightNodeV113 node;target_providers::Handle16 handle;};
 std::shared_ptr<LightSetNameOwnerV3> names_;
 std::array<std::array<std::shared_ptr<NativeLightV113>,5>,4> lights78_{};
 std::array<std::shared_ptr<NativeLightV113>,5> offc8_{};
 std::array<std::array<std::uint8_t,5>,4> changed64_{};
 std::vector<Static> static_dc_;std::vector<Active> active11c_;
public:
 explicit NativeLightSetV113(std::shared_ptr<LightSetNameOwnerV3> names):names_(std::move(names)){}
 ~NativeLightSetV113(){reset_light_sets();}
 const auto& names()const noexcept{return names_;}
 static void initialize_filter(std::vector<bool>&,bool);
 bool set_light(std::int32_t,std::int32_t,std::shared_ptr<NativeLightV113>,std::string&);
 bool get_light(std::int32_t,std::int32_t,std::shared_ptr<NativeLightV113>&,std::string&)const;
 bool set_dummy_off(std::int32_t,std::shared_ptr<NativeLightV113>,std::string&);
 bool select(std::int32_t,const std::vector<bool>&,std::array<std::shared_ptr<NativeLightV113>,4>&,std::string&)const;
 bool add_static(NativeLightNodeV113,std::string&);
 bool add_active(target_providers::Handle16,NativeLightNodeV113,std::string&);
 bool closest_static(const std::array<float,3>&,NativeLightNodeV113&,std::string&)const;
 bool update(const NativeLightUpdateServicesV113&,std::string&);
 void reset_light_sets()noexcept;
 void source_assign_default_v113(); //3544d8..4f0 temporary LightSet C1→assignment→D1
};
}
