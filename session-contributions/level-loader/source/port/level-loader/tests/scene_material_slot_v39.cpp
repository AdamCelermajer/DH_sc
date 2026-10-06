#include "scene_material_slot_v39.hpp"
#include <scene.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static void require(bool v,const std::string& e){if(!v)throw std::runtime_error(e);}
int main(int argc,char** argv){if(argc!=3)return 2;try{
 std::ifstream input(argv[1],std::ios::binary);require(bool(input),"gold unavailable");
 auto word=[&](){std::uint32_t value;input.read(reinterpret_cast<char*>(&value),4);require(bool(input),"gold truncated");return value;};
 auto text=[&](){auto n=word();std::string value(n,'\0');input.read(value.data(),n);require(bool(input),"gold string truncated");return value;};
 char magic[4];input.read(magic,4);require(std::string(magic,4)=="SCM9","gold magic differs");
 const auto assets=word();unsigned slots=0,mismatches=0,instances=0;std::string error;
 for(unsigned a=0;a<assets;++a){auto asset=text();scene::Scene scene;auto count=word();
  for(unsigned i=0;i<count;++i){scene::Material m;m.id=text();scene.materials.push_back(std::move(m));}
  auto n=word();scene.instances.reserve(n);
  for(unsigned i=0;i<n;++i){scene::Instance instance{};instance.node=text();auto bindings=word();for(unsigned j=0;j<bindings;++j)instance.materials.push_back(word());
   auto primitives=word();std::vector<std::string> symbols;for(unsigned p=0;p<primitives;++p)symbols.push_back(text());
   scene.instances.push_back(std::move(instance));auto& actual=scene.instances.back();++instances;
   for(unsigned p=0;p<primitives;++p){const scene::Material* material=nullptr;require(loader::resolve_retained_scene_material_slot_v39(scene,actual,p,material,error)&&error.empty(),asset+": "+error);
    require(material==&scene.materials[actual.materials[p]],"native material receiver differs from authored source slot");++slots;if(material->id!=symbols[p])++mismatches;
   }
   const scene::Material* preserved=scene.materials.empty()?nullptr:&scene.materials.front();const auto* previous=preserved;
   require(!loader::resolve_retained_scene_material_slot_v39(scene,actual,actual.materials.size(),preserved,error)&&preserved==previous,"missing slot defaulted/corrupted output");
   const auto foreign=actual;require(!loader::resolve_retained_scene_material_slot_v39(scene,foreign,0,preserved,error)&&preserved==previous,"foreign instance accepted");
   if(!actual.materials.empty()){const auto value=actual.materials[0];actual.materials[0]=UINT32_MAX;require(!loader::resolve_retained_scene_material_slot_v39(scene,actual,0,preserved,error)&&preserved==previous,"invalid catalog defaulted/corrupted output");actual.materials[0]=value;}
  }
 }

 std::ifstream original(argv[2],std::ios::binary);require(bool(original),"original loop gold unavailable");
 auto original_word=[&](){std::uint32_t value;original.read(reinterpret_cast<char*>(&value),4);require(bool(original),"original loop gold truncated");return value;};
 const auto original_cases=original_word();unsigned original_calls=0;
 for(unsigned c=0;c<original_cases;++c){scene::Scene scene;for(unsigned i=0;i<8;++i){scene::Material m;m.id="ExplicitHostCatalog_"+std::to_string(i);scene.materials.push_back(m);}
  scene.instances.emplace_back();auto& instance=scene.instances.back();auto n=original_word();for(unsigned i=0;i<n;++i)instance.materials.push_back(original_word());
  auto calls=original_word();require(calls==n,"source endpoint case counts differ");
  for(unsigned i=0;i<calls;++i){auto source_slot=original_word(),source_catalog=original_word();const scene::Material* actual=nullptr;
   require(loader::resolve_retained_scene_material_slot_v39(scene,instance,source_slot,actual,error)&&actual==&scene.materials[source_catalog],"native source-slot result differs from whole original constructGeometry call");++original_calls;
  }
 }
 require(slots==11764&&mismatches==442,"authored exact material coverage differs");
 std::cout<<"PASS authored_assets="<<assets<<" actual_instances="<<instances<<" exact_source_slots="<<slots<<" symbol_search_disagreements="<<mismatches<<" original_cases="<<original_cases<<" original_assignment_calls="<<original_calls<<" fallback=0 foreign_missing_invalid_rejected=1\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
