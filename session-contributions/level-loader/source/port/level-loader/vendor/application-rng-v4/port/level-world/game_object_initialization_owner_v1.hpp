#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include <functional>
#include <vector>
namespace dh2::world {
// All services operate on this SAME canonical base/visual/PF graph. A missing
// reached service is an error, not a successful no-op or a generated visual.
struct GameObjectInitializationServicesV1 {
 std::shared_ptr<void> owner;
 const std::vector<std::string>* difficulty_names{};
 const std::vector<std::string>* sound_names{};
 std::function<bool(std::uint32_t,std::string&)> condition_init;
 std::function<bool(std::int32_t&,std::string&)> check_spawn_probability;
 std::function<bool(const float*,bool,std::string&)> set_position;
 std::function<bool(bool&,std::string&)> device_high_performance;
 std::function<bool(std::string&)> load_visual;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync;
 std::function<bool(bool,std::string&)> set_visible;
 std::function<bool(bool,const float*,float,std::uintptr_t,std::string&)> init_pf_object;
 std::function<bool(const std::string&,std::int32_t&,std::string&)> light_set_id;
 std::function<bool(std::uintptr_t,std::int32_t,std::string&)> visual_set_light_set;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> visual_root;
 std::function<bool(std::uintptr_t,const char*,std::uintptr_t&,std::string&)> node_from_name;
 std::function<bool(std::string&)> update_pf_object;
};
// Whole ObjectBase::InitPost33ec0c, GameObject::InitPost38be5c and
// GameObject::InitFinal38cd48 control/stores over real constructor/property
// storage. Visual construction, condition compilation and PF are explicit
// concrete providers. A loader must discard a failed candidate; no retry gate
// is invented inside these original methods.
class GameObjectInitializationOwnerV1 {
 CanonicalGameObjectBaseOwnerV1& base_;
 GameObjectInitializationServicesV1 services_;
 bool missing(const char*,std::string&)const;
 bool spawn(std::int32_t&,std::string&);
public:
 GameObjectInitializationOwnerV1(CanonicalGameObjectBaseOwnerV1& b,GameObjectInitializationServicesV1 s):base_(b),services_(std::move(s)){}
 bool object_base_init_post(std::string&);
 bool init_post(bool& source_eligible,std::string&);
 bool init_final(bool& source_eligible,std::string&);
};
}
