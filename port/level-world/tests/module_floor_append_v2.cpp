#include "../module_selected_scene_v2.hpp"
#include "../module_floor_append_v2.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
using namespace dh2;
int main(int argc,char**argv){
 std::ifstream f(argc>1?argv[1]:"/data/local/tmp/dh2-swamp-module-v2.bdae",std::ios::binary);assert(f);
 std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)),{});
 resources::BresView b{};assert(dh2_bres_open(&b,bytes.data(),bytes.size())==resources::BresError::ok);
 floors::World world;world::ModuleSelectedSceneV2 selected;bool found;std::string e;unsigned room=0;
assert(world::module_selected_scene_v2(b,"_module_obj_4of4_brdwalk_sw_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_obj_3of4_brdwalk_sw_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_obj_1of4_brdwalk_nse_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_corner_ruin_ws_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_merchantcamp_ruins_swe_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_corner_brdwalk_se_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_deadend_brdwalk_w_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_bossroom_ruins_ns_",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
assert(world::module_selected_scene_v2(b,"_module_obj_2of4_brdwalk_sw_00",selected,found,e)&&found);
for(const auto& i:selected.scene.instances){const auto& n=selected.scene.graph[i.node_index];if(n.name.find("floor")==std::string::npos)continue;
 if(!world::module_floor_append_v2(b,selected.scene,i,room,world,e)){std::cerr<<n.name<<": "<<e<<"\n";return 1;}
 const auto& record=*world.records.back();assert(record.room==room);assert(!record.triangles.empty());
 if(n.user_properties.find("water")!=std::string::npos)assert(record.flags.floor&2u);
 if(n.user_properties.find("hole")!=std::string::npos)assert(record.flags.floor&1u);
 }++room;
std::cout<<"PASS actual SWAMP authored-floor records "<<world.records.size()<<" in "<<room<<" modules\n";}
