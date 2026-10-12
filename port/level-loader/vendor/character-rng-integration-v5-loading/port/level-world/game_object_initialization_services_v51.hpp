#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
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
}
