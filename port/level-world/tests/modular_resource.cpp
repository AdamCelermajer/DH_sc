#include "../objects.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
using Raw=std::vector<std::uint8_t>;
static Raw read(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Missing fixture "+path);return {std::istreambuf_iterator<char>(f),{}};}
static void check(bool value){if(!value)throw std::runtime_error("Modular resource check failed");}
int main(int argc,char** argv){try{
 check(argc==3);auto model=read(argv[1]);
 const char* types[]={"warrior","rogue","mage"};const char* clips[]={"knight","rogue","mage"};
 const int controllers[][4]={{123,171,42,85},{123,170,41,84},{123,169,40,83}};
 unsigned total=0;double changes[3]{};
 for(unsigned i=0;i<3;++i){
  std::vector<std::string> ids{"MC_Head__naked-mesh-skin"};
  for(const char* category:{"Torso","Feet","Hands"})ids.push_back(std::string("MC_")+category+"_default_"+types[i]+"-mesh-skin");
  auto clip=read(std::string(argv[2])+"/prince_menu_idle_"+clips[i]+".bdae");dh2::objects::Resource r;std::string error;
  check(dh2::objects::load_modular_resource(model.data(),model.size(),ids,clip.data(),clip.size(),r,error));
  check(r.scene.instances.size()==4&&r.primitives.size()>=4&&r.animation.track_count()>0&&!r.animation.skipped&&!r.animation.unbound&&r.animation.end>r.animation.start);
  for(unsigned j=0;j<4;++j)check(r.scene.instances[j].controller==controllers[i][j]);
  check(dh2::objects::sample(r,r.animation.start,error));
  std::vector<std::vector<dh2::objects::Vertex>> first;for(const auto& p:r.primitives)first.push_back(p.vertices);
  check(dh2::objects::sample(r,r.animation.start+(r.animation.end-r.animation.start)/2,error));
  for(unsigned p=0;p<r.primitives.size();++p){check(!r.primitives[p].skin.nodes.empty());total+=r.primitives[p].vertices.size();
   for(unsigned v=0;v<r.primitives[p].vertices.size();++v)for(unsigned a=0;a<3;++a){const float value=r.primitives[p].vertices[v].p[a];check(std::isfinite(value));changes[i]+=std::abs(value-first[p][v].p[a]);}
  }
  check(changes[i]>1);
  auto missing=ids;missing.back()="MC_Hands_absent-mesh-skin";dh2::objects::Resource rejected;
  check(!dh2::objects::load_modular_resource(model.data(),model.size(),missing,clip.data(),clip.size(),rejected,error)&&rejected.primitives.empty());
  auto duplicate=ids;duplicate.back()=ids[0];
  check(!dh2::objects::load_modular_resource(model.data(),model.size(),duplicate,clip.data(),clip.size(),rejected,error)&&rejected.primitives.empty());
  std::cout<<"MODULAR "<<types[i]<<" | controllers 4 | draws "<<r.primitives.size()<<" | tracks "<<r.animation.track_count()<<" | pose change "<<changes[i]<<" | guards PASS\n";
 }
 std::cout<<"MODULAR_RESOURCE PASS | classes 3 | skin vertices "<<total<<" | exact original controllers | real idle clips\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
