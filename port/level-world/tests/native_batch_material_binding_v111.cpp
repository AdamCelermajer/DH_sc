#include "../native_batch_compiler_v111.hpp"
#include "../../scene-materials/scene.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace {
void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const char* path){
 std::ifstream file(path,std::ios::binary);
 if(!file)throw std::runtime_error("Could not open swamp BDAE");
 return {std::istreambuf_iterator<char>(file),{}};
}
}
int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: native_batch_material_binding_v111 <swamp.bdae>");
  auto bytes=read(argv[1]);dh2::resources::BresView image{};dh2::scene::Scene scene;std::string error;
  require(dh2_bres_open(&image,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"Swamp BRES open");
  const bool loaded=dh2::scene::load(image,scene,error);
  require(loaded,error.empty()?"Swamp scene load":error.c_str());
  unsigned aliases=0;
  for(const auto& instance:scene.instances){
   dh2::assets::Mesh mesh{};
   require(dh2_mesh_open(&mesh,&image,instance.geometry)==dh2::assets::Error::ok,"Swamp instance mesh open");
   require(instance.material_symbols_v1.size()==instance.materials.size(),"Parser lost symbol/target binding rows");
   std::vector<dh2::scene::InstanceMaterialBindingV1> bindings;
   for(std::size_t i=0;i<instance.materials.size();++i){
    require(instance.materials[i]<scene.materials.size(),"Parser target material index invalid");
    bindings.push_back({instance.material_symbols_v1[i].symbol,scene.materials[instance.materials[i]]});
   }
   for(unsigned p=0;p<mesh.primitives;++p){
    dh2::assets::Primitive primitive{};
    require(dh2_mesh_primitive(&mesh,p,&primitive)==dh2::assets::Error::ok&&primitive.material,"Swamp primitive material symbol");
    dh2::scene::Material target;
    require(dh2::world::resolve_material_binding_v111(bindings,primitive.material,target,error),error.c_str());
    if(std::string(primitive.material)=="ColorMaterial"&&target.id=="ColorMaterial_0057E157")++aliases;
   }
  }
  require(aliases==16,"Expected sixteen authored swamp ColorMaterial aliases");
  std::vector<dh2::scene::InstanceMaterialBindingV1> missing;
  dh2::scene::Material ignored;
  require(!dh2::world::resolve_material_binding_v111(missing,"ColorMaterial",ignored,error),"Missing binding accepted");
  std::vector<dh2::scene::InstanceMaterialBindingV1> duplicate(2);
  duplicate[0].symbol=duplicate[1].symbol="ColorMaterial";
  duplicate[0].target.id="first";duplicate[1].target.id="second";
  require(!dh2::world::resolve_material_binding_v111(duplicate,"ColorMaterial",ignored,error),"Duplicate binding accepted");
  std::cout<<"{\"swamp_color_material_aliases\":"<<aliases<<",\"missing_rejected\":true,\"duplicate_rejected\":true}\n";
  return 0;
 }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
