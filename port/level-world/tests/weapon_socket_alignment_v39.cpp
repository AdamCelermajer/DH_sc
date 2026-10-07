#include "../../engine-skinning/visual_skin_owner_v6.hpp"
#include "../../engine-animation/animation.hpp"
#include "../authored_fx_alignment_v39.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <stdexcept>
using namespace dh2;
static unsigned checks,sockets;
static void ck(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error(p);return {std::istreambuf_iterator<char>(f),{}};}
static skinning::VisualAssetResultV6 file(void* raw,const char* name,std::vector<std::uint8_t>& out,std::string& e){std::string p=name;p=p.substr(p.find_last_of('/')+1);for(auto& c:p)c=char(std::tolower(static_cast<unsigned char>(c)));p=*static_cast<std::string*>(raw)+"/"+p;try{out=read(p);return skinning::VisualAssetResultV6::found;}catch(const std::exception& x){e=x.what();return skinning::VisualAssetResultV6::failed;}}
int main(int argc,char** argv){try{ck(argc==4,"model/weapon-directory/actual-animation required");skinning::VisualSkinResourcesV6 resources;std::string error;ck(resources.load(read(argv[1]),error),error);auto borrow=resources.borrow();
 auto scene=borrow.factory_scene();const auto pristine=scene;std::string directory=argv[2];skinning::VisualSkinOwnerV6 owner(borrow,scene,{&directory,file});ck(owner.initialize(error),error);
 animation::Player animator;auto clip=read(argv[3]);ck(animator.load(clip.data(),clip.size(),scene,error),error);
 const char* anchors[]{"anchor_shield_left_offset","anchor_weapon_right_offset","anchor_weapon_left_offset"};
 for(const char* weapon:{"mc_rweapon_longsword_01","mc_rweapon_twohandssword_01"}){
  auto raw=read(directory+"/"+weapon+".bdae");resources::BresView image{};ck(dh2_bres_open(&image,raw.data(),raw.size())==resources::BresError::ok,"actual weapon BRES");scene::Scene local;ck(scene::load(image,local,error),error);
  for(int slot=1;slot<=2;++slot)for(int mode=0;mode<3;++mode){ck(owner.set_weapon(nullptr,1,1,error)&&owner.set_weapon(nullptr,2,2,error),error);ck(owner.set_weapon(weapon,slot,mode,error),error);
   int anchor=-1;for(unsigned i=0;i<scene.graph.size();++i)if(scene.graph[i].name==anchors[mode]||scene.graph[i].id==anchors[mode]||scene.graph[i].sid==anchors[mode])anchor=i;ck(anchor>=0,"actual authored socket exists");
   for(unsigned heading=0;heading<8;++heading){scene=pristine;ck(animator.sample(scene,64+heading*16,error),error);math::Matrix4f outer{};float pos[]{137,251,367},rot[]{heading%2?.2f:0.f,heading%3?.1f:0.f,heading*.785398185f},scale[]{1,1,1};fx::source_fx_trs_matrix_v4(outer,pos,rot,scale);
    for(auto& n:scene.graph){math::Matrix4f node{},result{};std::copy(n.world.begin(),n.world.end(),node.m);fx::source_fx_matrix_multiply_v4(result,outer,node);std::copy_n(result.m,16,n.world.data());}
    std::vector<skinning::VisualDrawPartV6> parts;ck(owner.draw_parts(parts,error),error);unsigned i=0;
    for(const auto& p:parts)if(p.weapon_slot){ck(p.weapon_slot==slot&&i<local.instances.size(),"actual selected weapon/socket identity");math::Matrix4f socket{},node{},expected{};std::copy(scene.graph[anchor].world.begin(),scene.graph[anchor].world.end(),socket.m);std::copy(local.instances[i].world.begin(),local.instances[i].world.end(),node.m);ck(fx::authored_fx_draw_world_v39(expected,fx::AuthoredPositionSpaceV39::weapon_socket_local,&socket,&node,error),error);
     for(unsigned k=0;k<16;++k)ck(std::abs(expected.m[k]-p.world[k])<=.001f,"source actual socket*weapon local matrix");
     // Camera projection may change, but the retained source socket geometry
     // and world transform remain exactly these same owner-produced values.
     const auto saved=p.world;for(unsigned camera=0;camera<4;++camera){math::Matrix4f view{},wvp{};float cr[]{.3f,0,camera*.7f};fx::source_fx_trs_matrix_v4(view,pos,cr,scale);ck(fx::authored_fx_wvp_v39(wvp,view,expected,error),error);ck(saved==p.world,"camera changed weapon socket world");}++i;++sockets;
    }ck(i==local.instances.size(),"all actual weapon instances retained");
   }
  }
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_weapon_resources\":2,\"socket_modes\":3,\"slots\":2,\"headings\":8,\"weapon_parts\":"<<sockets<<",\"camera_and_root_poses\":\"fixtures\",\"actual_animation_sampled\":true,\"live_visual_acceptance\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
