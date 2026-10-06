#include "../module_selected_scene_v2.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
using namespace dh2;
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
 assert(world::module_selected_scene_v2(b,"_module_obj_3of4_brdwalk_sw_00",out,found,e));assert(found);assert(out.scene.nodes==100);
 assert(out.scene.graph.front().id=="_module_obj_3of4_brdwalk_sw_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_obj_1of4_brdwalk_nse_00",out,found,e));assert(found);assert(out.scene.nodes==83);
 assert(out.scene.graph.front().id=="_module_obj_1of4_brdwalk_nse_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_corner_ruin_ws_00",out,found,e));assert(found);assert(out.scene.nodes==59);
 assert(out.scene.graph.front().id=="_module_corner_ruin_ws_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_merchantcamp_ruins_swe_00",out,found,e));assert(found);assert(out.scene.nodes==59);
 assert(out.scene.graph.front().id=="_module_merchantcamp_ruins_swe_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_corner_brdwalk_se_00",out,found,e));assert(found);assert(out.scene.nodes==49);
 assert(out.scene.graph.front().id=="_module_corner_brdwalk_se_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_deadend_brdwalk_w_00",out,found,e));assert(found);assert(out.scene.nodes==42);
 assert(out.scene.graph.front().id=="_module_deadend_brdwalk_w_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_bossroom_ruins_ns_",out,found,e));assert(found);assert(out.scene.nodes==12);
 assert(out.scene.graph.front().id=="_module_bossroom_ruins_ns_-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"_module_obj_2of4_brdwalk_sw_00",out,found,e));assert(found);assert(out.scene.nodes==108);
 assert(out.scene.graph.front().id=="_module_obj_2of4_brdwalk_sw_00-node");
 assert(out.node_visibility.size()==out.scene.graph.size());assert(out.instance_visibility.size()==out.scene.instances.size());
 assert(!out.scene.instances.empty());++count;
 assert(world::module_selected_scene_v2(b,"source_name_miss",out,found,e));assert(!found&&out.scene.graph.empty());
 assert(!world::module_selected_scene_v2(b,"",out,found,e));
 std::cout<<"PASS actual SWAMP selected subscenes "<<count<<" plus missing/empty cases\n";
}
