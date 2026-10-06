#include "scene_material_slot_v39.hpp"
#include <scene.hpp>
#include <algorithm>
namespace dh2::loader {
bool resolve_retained_scene_material_slot_v39(const scene::Scene& scene,
 const scene::Instance& instance,std::uint32_t slot,const scene::Material*& out,std::string& e){
 if(std::none_of(scene.instances.begin(),scene.instances.end(),[&](const auto& current){return &current==&instance;})){
  e="Required SAME retained Scene instance for material binding";return false;
 }
 if(slot>=instance.materials.size()){
  e="Original mesh material slot has no authored instance binding";return false;
 }
 const auto index=instance.materials[slot];
 if(index>=scene.materials.size()){
  e="Original instance binding material catalog index is invalid";return false;
 }
 out=&scene.materials[index];e.clear();return true;
}
}
