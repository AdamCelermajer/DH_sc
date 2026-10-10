// Exercise the production draw include without a GL context. Recording
// providers observe which authored actor/attachment transforms are submitted.
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using Matrix=std::array<float,16>;
Matrix identity(){return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};}
struct Vertex{float p[3];};
struct Node{Matrix world=identity();};
struct Scene{std::vector<Node> graph;};
struct Primitive{struct Skin{std::vector<int> nodes;}skin;std::vector<Vertex> vertices;unsigned node{};};
struct Resource{struct Clip{int start=0,end=1000;}animation;Scene scene;std::vector<Primitive> primitives;unsigned identity{};};
struct Draw{unsigned vertices{},node{},identity{};};
struct Weapon{unsigned anchor{};Resource resource;std::vector<Draw> draws;};
struct Actor{Resource resource;std::vector<Draw> draws;std::vector<Weapon> weapons;unsigned anchor{};float scale[3]{1,1,1};std::uint64_t sample_elapsed{};};
std::vector<unsigned> sampled;
namespace dh2 {
namespace scene {
Matrix multiply(const Matrix& a,const Matrix& b){Matrix out{};for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row)for(unsigned k=0;k<4;++k)out[col*4+row]+=a[k*4+row]*b[col*4+k];return out;}
}
namespace objects {bool sample(Resource& resource,int,std::string&){sampled.push_back(resource.identity);return true;}}
namespace resources {enum class ResourceScopeV37{actor};}
}
constexpr int GL_ARRAY_BUFFER=0;
std::size_t buffer_bytes_v41(std::size_t count,std::size_t stride){return count*stride;}
void buffer_subdata_v41(unsigned,int,std::size_t,const void*,dh2::resources::ResourceScopeV37){}
bool class_scene{},menu_background{};
int class_selected=-1,menu_persona_class=-1;
std::chrono::steady_clock::time_point menu_persona_epoch;
std::vector<Actor> class_preview_actors;
Scene current_scene;
Matrix projection=identity();
struct Submission{unsigned id;Matrix transform;};
std::vector<Submission> submitted;
void draw(){
 auto submit=[](const Draw& batch,const Matrix& transform){submitted.push_back({batch.identity,transform});};
#include "../app/src/main/cpp/renderer_front_draw_v87.inc"
}
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
int main(){try{
 current_scene.graph.resize(3);
 for(unsigned i=0;i<3;++i){
  current_scene.graph[i].world[12]=float(i)*400.f-400.f;
  current_scene.graph[i].world[13]=float(i)*10.f;
  Actor actor;actor.anchor=i;actor.resource.identity=i;actor.resource.scene.graph.resize(1);
  // Non-skinned body nodes and weapon nodes both carry authored local offsets.
  actor.resource.scene.graph[0].world[14]=3.f;
  actor.resource.primitives.push_back({{},{{{0,0,0}}},0});
  actor.draws.push_back({1,0,10+i});
  Weapon weapon;weapon.anchor=0;weapon.resource.scene.graph.resize(1);
  weapon.resource.scene.graph[0].world[12]=5.f;weapon.draws.push_back({2,0,20+i});
  actor.weapons.push_back(weapon);class_preview_actors.push_back(actor);
 }
 // Includes the initial Show state and every adjacent transition direction.
 // Selection controls camera animation, never the scene's actor visibility.
 class_scene=true;
 for(int selection:{-1,0,1,2,1,0}){
  class_selected=selection;sampled.clear();submitted.clear();draw();
  require(sampled==std::vector<unsigned>({0,1,2}),"An authored class actor stopped animating");
  require(submitted.size()==6,"A class body/weapon disappeared on selection change");
  for(unsigned i=0;i<3;++i){const auto& body=submitted[i*2];const auto& weapon=submitted[i*2+1];
   require(body.id==10+i&&weapon.id==20+i,"Submission used a different class owner");
   require(body.transform[12]==float(i)*400.f-400.f&&body.transform[13]==float(i)*10.f&&body.transform[14]==3.f,"Body lost its authored class anchor");
   require(weapon.transform[12]==body.transform[12]+5.f&&weapon.transform[14]==3.f,"Weapon lost its same actor attachment");
  }
 }
 class_scene=false;sampled.clear();submitted.clear();draw();
 require(sampled.empty()&&submitted.empty(),"Creation actors leaked into an inactive scene");
 std::cout<<"PASS all three class bodies/weapons retain authored transforms through Show and class transitions; inactive scene submits none\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
