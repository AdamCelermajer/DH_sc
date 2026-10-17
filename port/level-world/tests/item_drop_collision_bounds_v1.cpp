#include "../item_drop_collision_bounds_v1.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
void require(bool ok,const char* message){if(!ok){std::fprintf(stderr,"%s\n",message);std::exit(1);}}
bool near(float a,float b){return std::fabs(a-b)<1e-6f;}
bool near_source(float a,float b){return std::fabs(a-b)<1e-4f;}
}

int main(){
 using namespace dh2::item_drop_collision_bounds_v1;
 Bounds bounds{};std::string error;
 const float root[16]={1,0,0,0,0,1,0,0,0,0,1,0,0.014819f,0.024443f,17.5303f,1};
 const float child[16]={1,0,0,0,0,1,0,0,0,0,1,0,-61.4644f,-66.2507f,28.8724f,1};
 float local[16]{};
 require(relative_to_root(root,child,local,error),"translated item visual root rejected");
 require(near_source(local[12],-61.479219f)&&near_source(local[13],-66.275143f)&&near_source(local[14],11.3421f),
         "mesh transform was not expressed relative to the source visual root");
 const float rotated_root[16]={0,1,0,0,-1,0,0,0,0,0,1,0,10,20,3,1};
 const float rotated_child[16]={0,1,0,0,-1,0,0,0,0,0,1,0,7,24,5,1};
 require(relative_to_root(rotated_root,rotated_child,local,error),"rotated item visual root rejected");
 require(near(local[12],4.f)&&near(local[13],3.f)&&near(local[14],2.f),
         "rotated root inverse did not recover child-local translation");
 const float singular[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
 require(!relative_to_root(singular,child,local,error),"singular item visual root accepted");
 const float lo[3]={-2.f,-1.f,-.5f},hi[3]={2.f,1.f,.5f};
 const float rotated[16]={0,1,0,0,-1,0,0,0,0,0,1,0,10,20,0,1};
 require(include_mesh_box(bounds,lo,hi,rotated,error),"rotated authored mesh AABB rejected");
 require(near(bounds.minimum[0],9.f)&&near(bounds.maximum[0],11.f)&&
         near(bounds.minimum[1],18.f)&&near(bounds.maximum[1],22.f),
         "node transform did not match CSceneNode bounds");
 float radius=0.f;
 require(sensor_radius(bounds,false,1.f,1.f,radius,error)&&near(radius,.03f),
         "ItemObject no-colbox 1.5 AABB padding changed");
 require(sensor_radius(bounds,true,1.f,1.f,radius,error)&&near(radius,.02f),
         "ItemObject colbox AABB sensor changed");
 require(interaction_radius(bounds,false,1.f,1.f,radius,error)&&near(radius,1.5f),
         "ItemObject interaction radius omitted source 1.5 padding");
 require(interaction_radius(bounds,true,1.f,1.f,radius,error)&&near(radius,1.f),
         "ItemObject colbox interaction radius changed");
 Bounds axe_fallback{};const float glow_min[3]={-110.f,-110.f,71.5873f};
 const float glow_max[3]={110.f,110.f,71.5873f};
 const float identity[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
 require(include_mesh_box(axe_fallback,glow_min,glow_max,identity,error),
         "BattleAxe source glow bounds rejected");
 require(interaction_radius(axe_fallback,false,1.f,1.f,radius,error)&&near(radius,165.f),
         "BattleAxe no-colbox source game-unit interaction radius changed");
 require(!interaction_radius(axe_fallback,false,
         std::numeric_limits<float>::infinity(),1.f,radius,error),
         "non-finite source interaction scale accepted");
 const float invalid[3]={3.f,0.f,0.f};
 require(!include_mesh_box(bounds,invalid,hi,rotated,error),
         "inverted source mesh AABB accepted");
 std::puts("item_drop_collision_bounds_v1: 15 checks passed");
}
