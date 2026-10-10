#include "../module_static_scene_v2.hpp"
#include "../native_batch_compiler_v111.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <memory>
using namespace dh2;
namespace {
void check_module_material_bindings(const world::ModuleSelectedSceneV2& selected,
                                   const resources::BresView& bres){
 for(const auto& instance:selected.scene.instances){
  assets::Mesh mesh{};
  assert(dh2_mesh_open(&mesh,&bres,instance.geometry)==assets::Error::ok);
  assert(mesh.primitives==instance.materials.size());
  assert(instance.material_symbols_v1.size()==instance.materials.size());
  for(unsigned i=0;i<mesh.primitives;++i){
   assets::Primitive primitive{};
   assert(dh2_mesh_primitive(&mesh,i,&primitive)==assets::Error::ok);
   assert(primitive.material&&instance.material_symbols_v1[i].symbol==primitive.material);
   assert(instance.materials[i]<selected.scene.materials.size());
   // This is the exact symbol -> instance_material target handoff used by
   // the retained Module mesh material borrower.
   const scene::InstanceMaterialBindingV1 binding{
     instance.material_symbols_v1[i].symbol,
     selected.scene.materials[instance.materials[i]]};
   assert(binding.symbol==primitive.material&&!binding.target.id.empty());
   scene::Material resolved;std::string error;
   assert(world::resolve_material_binding_v111({binding},primitive.material,resolved,error));
   assert(resolved.id==binding.target.id&&resolved.effect_uri==binding.target.effect_uri&&
          resolved.gles2_technique==binding.target.gles2_technique&&
          resolved.diffuse==binding.target.diffuse&&resolved.alpha_map==binding.target.alpha_map);
  }
 }
}
void check_retained_module_material_borrow(world::ModuleSelectedSceneV2 selected){
 physical::ObjectVisualTransformV1 root{};
 root.root_matrix[0]=root.root_matrix[5]=root.root_matrix[10]=root.root_matrix[15]=1.0f;
 auto owner=std::make_shared<world::ModuleStaticSceneV2>();std::string error;
 assert(owner->initialize(std::move(selected),root,error));
 std::shared_ptr<world::RetainedMeshNodeV91> mesh;
 assert(owner->borrow_mesh_source_v111(0,mesh,error)&&mesh);
 world::RetainedSceneNodeBorrowV109 borrowed;
 auto current=[](std::string& e){e.clear();return true;};
 const std::shared_ptr<void> lease=owner;
 assert(owner->borrow_scene_node_v109(lease,current,reinterpret_cast<std::uintptr_t>(mesh.get()),borrowed,error));
 assert(borrowed.materials_v111);
 std::vector<scene::InstanceMaterialBindingV1> bindings;
 assert(borrowed.materials_v111(bindings,error));
 assert(bindings.size()==mesh->fields->materials.size());
 assert(bindings.size()==mesh->fields->material_symbols_v1.size());
 for(std::size_t i=0;i<bindings.size();++i){
  assert(bindings[i].symbol==mesh->fields->material_symbols_v1[i].symbol);
  assert(bindings[i].target.id==owner->selected().scene.materials[mesh->fields->materials[i]].id);
 }
}
}
int main(int argc,char** argv){
 const char* file=argc>1?argv[1]:"/data/local/tmp/dh2-swamp-module-v2.bdae";
 std::ifstream f(file,std::ios::binary);assert(f);
 std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)),{});
 resources::BresView b{};assert(dh2_bres_open(&b,bytes.data(),bytes.size())==resources::BresError::ok);
 std::string e;world::ModuleSelectedSceneV2 out;bool found=false;unsigned count=0;
 assert(world::module_selected_scene_v2(b,"_module_obj_4of4_brdwalk_sw_00",out,found,e));assert(found);assert(out.scene.nodes==103);
 assert(out.scene.graph.front().id=="_module_obj_4of4_brdwalk_sw_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 check_retained_module_material_borrow(out);
 assert(world::module_selected_scene_v2(b,"_module_obj_3of4_brdwalk_sw_00",out,found,e));assert(found);assert(out.scene.nodes==100);
 assert(out.scene.graph.front().id=="_module_obj_3of4_brdwalk_sw_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_obj_1of4_brdwalk_nse_00",out,found,e));assert(found);assert(out.scene.nodes==83);
 assert(out.scene.graph.front().id=="_module_obj_1of4_brdwalk_nse_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_corner_ruin_ws_00",out,found,e));assert(found);assert(out.scene.nodes==59);
 assert(out.scene.graph.front().id=="_module_corner_ruin_ws_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_merchantcamp_ruins_swe_00",out,found,e));assert(found);assert(out.scene.nodes==59);
 assert(out.scene.graph.front().id=="_module_merchantcamp_ruins_swe_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_corner_brdwalk_se_00",out,found,e));assert(found);assert(out.scene.nodes==49);
 assert(out.scene.graph.front().id=="_module_corner_brdwalk_se_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_deadend_brdwalk_w_00",out,found,e));assert(found);assert(out.scene.nodes==42);
 assert(out.scene.graph.front().id=="_module_deadend_brdwalk_w_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_bossroom_ruins_ns_",out,found,e));assert(found);assert(out.scene.nodes==12);
 assert(out.scene.graph.front().id=="_module_bossroom_ruins_ns_-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"_module_obj_2of4_brdwalk_sw_00",out,found,e));assert(found);assert(out.scene.nodes==108);
 assert(out.scene.graph.front().id=="_module_obj_2of4_brdwalk_sw_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 check_module_material_bindings(out,b);
 assert(world::module_selected_scene_v2(b,"source_name_miss",out,found,e));assert(!found&&out.scene.graph.empty());
 assert(!world::module_selected_scene_v2(b,"",out,found,e));
 std::cout<<"PASS actual SWAMP selected subscenes "<<count<<" plus missing/empty cases\n";
}
