#include "objects.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <set>
std::vector<std::uint8_t> read(const std::string& name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
int main(int argc,char** argv){
 if(argc!=2)return 2;const std::string root=argv[1];auto a=read(root+"/data/character_properties_pyarray.bin"),b=read(root+"/data/character_properties_pyarraynames.bin"),c=read(root+"/data/character_properties_pystructnames.bin"),d=read(root+"/data/character_models_dictionary_pyarraynames.bin"),e=read(root+"/data/character_models_dictionary_pyarray.bin");
 dh2::data::CharacterTable table;dh2::data::Dictionary models;std::string error;
 if(!dh2::data::load_characters(bytes(a),bytes(b),bytes(c),table,error)||!dh2::data::load_dictionary(bytes(d),bytes(e),models,error)){std::cerr<<error;return 3;}
 auto descriptor=read(root+"/worlds/crypt01.dact");std::vector<dh2::objects::Record> records;
 if(!dh2::objects::load_records(descriptor.data(),descriptor.size(),8,table,models,records,error)){std::cerr<<error;return 4;}
 if(records.size()!=95||std::count_if(records.begin(),records.end(),[](const auto& r){return r.kind==1;})!=11)return 5;
 std::set<std::string> names;for(const auto& r:records){names.insert(r.model);for(unsigned i=0;i<3;++i)if(r.placement[12+i]!=r.position[i])return 6;}
 std::mt19937 rng(20261003);unsigned rejected=0;
 for(unsigned i=0;i<3000;++i){auto mutated=descriptor;if(i%2)mutated.resize(rng()%mutated.size());else{auto index=rng()%mutated.size();mutated[index]^=1u<<(rng()%8);}std::vector<dh2::objects::Record> out;
  if(!dh2::objects::load_records(mutated.data(),mutated.size(),8,table,models,out,error)){++rejected;if(!out.empty())return 7;}}
 std::cout<<"{\"objects\":95,\"monsters\":11,\"decors\":84,\"descriptor_mutations\":3000,\"descriptor_rejected\":"<<rejected<<",\"resources\":[";
 unsigned index=0,total_poses=0,unbound=0,unsupported=0,triangles=0;
 for(const auto& name:names){auto model=read(root+"/actors/"+name);auto clip_name=dh2::objects::idle_clip(name);auto clip=clip_name.empty()?std::vector<std::uint8_t>{}:read(root+"/actors/"+clip_name);
  dh2::objects::Resource resource;if(!dh2::objects::load_resource(model.data(),model.size(),clip.empty()?nullptr:clip.data(),clip.size(),resource,error)){std::cerr<<name<<": "<<error;return 8;}
  unsigned poses=0,changed=0;const auto rest=resource.scene.graph;
  for(int ms=resource.animation.start;ms<=resource.animation.end;++ms){if(!dh2::objects::sample(resource,ms,error)){std::cerr<<error;return 9;}bool different=false;
   for(unsigned i=0;i<rest.size();++i)different=different||rest[i].world!=resource.scene.graph[i].world;changed+=different;
   for(const auto& p:resource.primitives)for(const auto& vertex:p.vertices)for(float v:vertex.p)if(!std::isfinite(v))return 10;++poses;
  }
  if(resource.animation.track_count()&&changed<100)return 11;
  if(name=="skeleton.bdae"){
   dh2::resources::BresView view{},cv{};dh2_bres_open(&view,model.data(),model.size());dh2_bres_open(&cv,clip.data(),clip.size());dh2::scene::Scene scene;dh2::scene::load(view,scene,error);dh2::animation::Player strict;
   if(strict.load(clip.data(),clip.size(),scene,error)||resource.animation.unbound!=3||resource.animation.track_count()!=23)return 12;
   bool found=false;for(unsigned i=0;i<dh2_bres_library_count(&cv,dh2::resources::Library::animation);++i){dh2::assets::Animation track{};dh2_animation_open(&track,&cv,i,0);auto* target=dh2_animation_target(&track);
    if(std::any_of(scene.graph.begin(),scene.graph.end(),[&](const auto& n){return n.id==target;}))continue;
    dh2::assets::Vector values{};if(!dh2_animation_vector(&track,0,true,&values)||values.type!=6)return 13;auto bad=clip;const auto offset=values.data-clip.data();bad[offset]=0;bad[offset+1]=0;bad[offset+2]=0xc0;bad[offset+3]=0x7f;
    dh2::animation::Player permissive;if(permissive.load(bad.data(),bad.size(),scene,error,dh2::animation::MissingTargets::ignore))return 14;found=true;break;
   }if(!found)return 15;
  }
  if(index++)std::cout<<',';std::cout<<"{\"model\":\""<<name<<"\",\"primitives\":"<<resource.primitives.size()<<",\"triangles\":"<<resource.triangles<<",\"removed_helpers\":"<<resource.removed_helpers<<",\"tracks\":"<<resource.animation.track_count()<<",\"unbound\":"<<resource.animation.unbound<<",\"unsupported\":"<<resource.animation.skipped<<",\"segments\":"<<resource.animation.segment_count()<<",\"sampled_poses\":"<<poses<<",\"changed_poses\":"<<changed<<'}';
  total_poses+=poses;unbound+=resource.animation.unbound;unsupported+=resource.animation.skipped;triangles+=resource.triangles;
 }
 std::cout<<"],\"resource_count\":"<<names.size()<<",\"sampled_poses\":"<<total_poses<<",\"unbound_tracks\":"<<unbound<<",\"unsupported_tracks\":"<<unsupported<<",\"resource_triangles\":"<<triangles<<",\"strict_missing_target_rejected\":true,\"malformed_unbound_key_rejected\":true}\n";return 0;
}
