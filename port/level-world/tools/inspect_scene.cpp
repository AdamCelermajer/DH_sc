#include "scene.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> b((std::istreambuf_iterator<char>(f)),{});dh2::resources::BresView v{};
 if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)return 3;
 dh2::scene::Scene scene;std::string error;if(!dh2::scene::load(v,scene,error)){std::cerr<<error;return 4;}
 std::cout<<"nodes "<<scene.nodes<<" instances "<<scene.instances.size()<<" ignored "<<scene.ignored_instances<<'\n';
 for(unsigned i=0;i<scene.graph.size();++i){const auto& n=scene.graph[i];if(i<43||n.parent<0||n.id.find("module")!=std::string::npos)std::cout<<"node "<<i<<" parent "<<n.parent<<" id "<<n.id<<" pos "<<n.world[12]<<","<<n.world[13]<<","<<n.world[14]<<'\n';}
 unsigned triangles=0,bad=0,mismatches=0;
 for(const auto& i:scene.instances){dh2::assets::Mesh mesh{};auto result=dh2_mesh_open(&mesh,&v,i.geometry);if(result!=dh2::assets::Error::ok){std::cerr<<"mesh rejected "<<i.node<<" error="<<int(result)<<'\n';++bad;continue;}
  for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive p{};dh2_mesh_primitive(&mesh,j,&p);triangles+=p.index_count/3;
   if(j>=i.materials.size()||scene.materials[i.materials[j]].id!=p.material){++mismatches;if(mismatches<6)std::cout<<"binding mismatch "<<i.node<<" primitive "<<j<<" symbol="<<p.material<<" bound="<<(j<i.materials.size()?scene.materials[i.materials[j]].id:"absent")<<'\n';}
   if(p.collada_type||p.index_count%3)std::cout<<"topology "<<i.node<<" type "<<p.collada_type<<" count "<<p.index_count<<'\n';
  }
 }
 std::cout<<"triangles "<<triangles<<" rejected "<<bad<<" binding_mismatches "<<mismatches<<'\n';
 for(const auto& m:scene.materials)std::cout<<"material "<<m.id<<" diffuse="<<m.diffuse<<" alpha="<<m.alpha_map<<'\n';return bad?5:0;
}
