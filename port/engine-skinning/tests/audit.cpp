#include "skinning.hpp"
#include "animation.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <cmath>
#include <random>
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
void require(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
 require(argc==3,"Expected model and clip");auto bytes=read(argv[1]),clip=read(argv[2]);dh2::resources::BresView view{};
 require(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"BRES");
 dh2::scene::Scene scene;std::string error;require(dh2::scene::load(view,scene,error),error);
 dh2::animation::Player player;require(player.load(clip.data(),clip.size(),scene,error),error);require(player.track_count()==29&&player.skipped==0,"Expected all Prince tracks");
 std::vector<dh2::skinning::Skin> skins;unsigned vertices=0;
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){dh2::skinning::Skin skin;
  if(!dh2::skinning::load(view,i,scene,skin,error))throw std::runtime_error(std::to_string(i)+": "+error);
  vertices+=skin.influences.size();skins.push_back(std::move(skin));}
 for(const auto& skin:skins)if(skin.id.find("_default_warrior-mesh-skin")!=std::string::npos){dh2::assets::Mesh mesh{};dh2_mesh_open(&mesh,&view,skin.geometry);
  for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive primitive{};dh2_mesh_primitive(&mesh,j,&primitive);
   for(const auto& material:scene.materials)if(material.id==primitive.material)std::cerr<<skin.id<<" texture="<<material.diffuse<<" alpha="<<material.alpha_map<<'\n';}}
 unsigned poses=0,changed=0;std::vector<std::vector<std::array<float,3>>> original;
 for(int ms:{0,100,500,1000,1499,1500,1501,1700,2000,2999,3000}){
  require(player.sample(scene,ms,error),error);
  for(unsigned i=0;i<skins.size();++i){const auto& skin=skins[i];dh2::assets::Mesh mesh{};dh2_mesh_open(&mesh,&view,skin.geometry);dh2::assets::Primitive primitive{};dh2_mesh_primitive(&mesh,0,&primitive);
   dh2::assets::Attribute attr{};require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&attr)==dh2::assets::Error::ok,"position");
   std::vector<std::array<float,3>> input(mesh.vertices),output;for(unsigned v=0;v<mesh.vertices;++v){float value[4]{};dh2_attribute_read(&attr,v,value);std::copy(value,value+3,input[v].begin());}
   std::vector<dh2::skinning::Matrix> matrices;require(dh2::skinning::palette(skin,scene,matrices,error),error);require(dh2::skinning::positions(skin,matrices,input,output,error),error);
   if(ms==0)original.push_back(output);else if(output!=original[i])++changed;
  }++poses;
 }
 require(changed>0,"No deformation across poses");
 // Mutation input remains bounded and validated by the real readers.
 std::mt19937 random(20261002);unsigned rejected=0;
 for(unsigned i=0;i<1000;++i){auto altered=bytes;const auto* c=dh2_bres_library_item(&view,dh2::resources::Library::controller,i%skins.size());
  const unsigned s=c[8]|(unsigned(c[9])<<8)|(unsigned(c[10])<<16)|(unsigned(c[11])<<24);
  const unsigned target=i%2?s+random()%156:random()%altered.size();altered[target]^=1u<<(random()%8);
  dh2::resources::BresView candidate=view;candidate.bytes=altered.data();dh2::skinning::Skin ignored;
  if(!dh2::skinning::load(candidate,i%skins.size(),scene,ignored,error))++rejected;
 }
 std::cout<<"{\"controllers\":"<<skins.size()<<",\"vertices\":"<<vertices<<",\"nodes\":"<<scene.nodes<<",\"instances\":"<<scene.instances.size()<<",\"tracks\":"<<player.track_count()<<",\"poses\":"<<poses<<",\"changed_skin_poses\":"<<changed<<",\"mutations\":1000,\"rejected_mutations\":"<<rejected<<"}\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
