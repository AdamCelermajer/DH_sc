#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include "../../engine-skinning/skinning.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>
namespace fs=std::filesystem;
using Bytes=std::vector<std::uint8_t>;
void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
std::uint32_t word(const Bytes& b,std::size_t p){check(p+4<=b.size(),"Fixture field outside input");return b[p]|(std::uint32_t(b[p+1])<<8)|(std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24);}
void store(Bytes& b,std::size_t p,std::uint32_t v){check(p+4<=b.size(),"Fixture patch outside input");for(unsigned i=0;i<4;++i)b[p+i]=std::uint8_t(v>>(8*i));}
std::uint32_t skinned_instance(const Bytes& b,std::uint32_t node,unsigned depth=0){
 check(depth<64,"Fixture scene cycle");auto n=word(b,node+64),base=word(b,node+68);
 for(unsigned i=0;i<n;++i)if(word(b,base+8*i)==2)return word(b,base+8*i+4);
 n=word(b,node+56);base=word(b,node+60);for(unsigned i=0;i<n;++i)if(auto found=skinned_instance(b,base+80*i,depth+1))return found;
 return 0;
}
std::uint32_t first_skinned_instance(const Bytes& b){
 auto root=word(b,32),n=word(b,root+152),base=word(b,root+156);
 for(unsigned i=0;i<n;++i){auto row=base+16*i,count=word(b,row+8),nodes=word(b,row+12);
  for(unsigned j=0;j<count;++j)if(auto found=skinned_instance(b,nodes+80*j))return found;}
 return 0;
}
dh::foundation::CharacterVisual load(const dh::foundation::AssetCatalog& assets,const std::string& path){
 dh::foundation::CharacterVisual result;dh::foundation::CharacterVisualConfig config;config.model_path=path;std::string error;
 check(result.load(assets,config,error),error);return result;
}
int main(int argc,char**argv){try{
 check(argc==3,"Expected shared asset root and fixture directory");dh::foundation::AssetCatalog assets(argv[1]);
 const std::string priest="original-cache/data/3d/characters/npcs/priest_good.bdae";
 auto original=load(assets,priest);check(original.original_materials().size()==1&&original.original_materials()[0].id=="diffuse","Original priest binding changed");
 auto prisoner=load(assets,"original-cache/data/3d/characters/npcs/prisoner.bdae");
 check(prisoner.original_materials().size()==2&&prisoner.original_materials()[0].id=="diffuse"&&prisoner.original_materials()[1].id=="alpha","Original prisoner bindings changed");
 Bytes b=assets.read(priest);const auto instance=first_skinned_instance(b);check(instance!=0,"No source skin instance");
 check(word(b,instance+12)==1,"Expected one fixture material");const auto binding=word(b,instance+16);
 // Only the actual instance target changes. Geometry primitive symbol remains
 // diffuse, and the original global diffuse material remains unchanged.
 const auto appended=static_cast<std::uint32_t>(b.size());const std::string target="#additive";b.insert(b.end(),target.begin(),target.end());b.push_back(0);
 store(b,binding+4,appended);store(b,12,static_cast<std::uint32_t>(b.size()));
 fs::create_directories(argv[2]);const auto file=fs::path(argv[2])/"override.bdae";{std::ofstream out(file,std::ios::binary);out.write(reinterpret_cast<const char*>(b.data()),b.size());check(bool(out),"Fixture write failed");}
 dh::foundation::AssetCatalog fixture(argv[2]);auto changed=load(fixture,"override.bdae");
 check(changed.original_materials().size()==1&&changed.original_materials()[0].id=="additive","Skin ignored authored instance override in favor of primitive symbol");
 check(changed.meshes()[0].ranges[0].material.additive,"Overridden material additive state not retained");
 check(std::abs(changed.meshes()[0].ranges[0].material.color[0]-.588f)<.001f,"Overridden material color not retained");
 check(changed.meshes()[0].indices==original.meshes()[0].indices,"Override altered geometry");
 check(changed.original_materials()[0].diffuse==original.original_materials()[0].diffuse,"Fixture should retain atlas identity");
 std::cout<<"PASS authored skinned override diffuse symbol -> additive target; original priest/prisoner bindings preserved\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
