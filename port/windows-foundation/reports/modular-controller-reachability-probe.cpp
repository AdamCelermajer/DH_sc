#include "../modular_defaults.hpp"
#include "../../scene-materials/scene.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
int main(int argc,char**argv){
 if(argc!=2)return 2;
 std::ifstream file(argv[1],std::ios::binary);std::vector<std::uint8_t>bytes{std::istreambuf_iterator<char>(file),{}};
 dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return 3;
 std::vector<dh::foundation::ModularDefaultCategory>categories;std::string error;
 if(!dh::foundation::decode_modular_defaults(bytes,categories,error)){std::cerr<<error;return 4;}
 dh2::scene::Scene scene;if(!dh2::scene::load(view,scene,error)){std::cerr<<error;return 5;}
 std::set<std::string>available,defaults;for(const auto&c:categories){available.insert(c.available_controller_ids.begin(),c.available_controller_ids.end());defaults.insert(c.controller_id);}
 std::cout<<"CONTROLLERS "<<dh2_bres_library_count(&view,dh2::resources::Library::controller)<<" CATEGORY_AVAILABLE "<<available.size()<<" DEFAULTS "<<defaults.size()<<" VISIBLE_INSTANCES "<<scene.instances.size()<<'\n';
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){
  auto p=dh2_bres_library_item(&view,dh2::resources::Library::controller,i);auto off=std::uint32_t(p[4])|std::uint32_t(p[5])<<8|std::uint32_t(p[6])<<16|std::uint32_t(p[7])<<24;
  std::string id=reinterpret_cast<const char*>(view.bytes+off);unsigned referenced=0;for(const auto&instance:scene.instances)if(instance.controller==int(i))++referenced;
  if(!available.count(id)||referenced)std::cout<<"CONTROLLER\t"<<i<<'\t'<<id<<"\tcategory="<<available.count(id)<<"\tdefault="<<defaults.count(id)<<"\tvisibleRefs="<<referenced<<'\n';
 }
 for(const auto&instance:scene.instances){
  std::cout<<"INSTANCE\t"<<instance.node<<"\tcontroller="<<instance.controller<<'\n';
  for(auto material:instance.materials){const auto&m=scene.materials.at(material);std::cout<<"MATERIAL\t"<<m.id<<"\tdiffuse="<<m.diffuse<<"\teffect="<<m.effect_file<<"\ttechnique="<<m.gles2_technique<<"\talpha="<<m.color[3]<<'\n';}
 }
}
