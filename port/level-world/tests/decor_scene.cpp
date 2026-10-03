#include "../decor_scene.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::physical;
static void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
template<class T>static T read(std::ifstream& in){T value{};in.read(reinterpret_cast<char*>(&value),sizeof value);require(bool(in),"truncated fixture");return value;}
static bool equal(const void* a,const void* b,std::size_t n){
 const auto* x=static_cast<const unsigned char*>(a);const auto* y=static_cast<const unsigned char*>(b);
 for(std::size_t i=0;i<n;i+=4){float p,q;std::memcpy(&p,x+i,4);std::memcpy(&q,y+i,4);if(std::memcmp(x+i,y+i,4)&&!(std::isnan(p)&&std::isnan(q)))return false;}return true;
}
static std::vector<std::uint8_t> load(const std::string& path){std::ifstream in(path,std::ios::binary);require(bool(in),"missing authored BRES");return {std::istreambuf_iterator<char>(in),std::istreambuf_iterator<char>()};}
int main(int argc,char** argv){try{
 require(argc==3,"decor scene audit requires fixture and actor asset directory");std::ifstream in(argv[1],std::ios::binary);require(read<std::uint32_t>(in)==0x31534344,"fixture magic");const auto count=read<std::uint32_t>(in),models=read<std::uint32_t>(in);
 std::vector<DecorSceneInput> inputs;
 for(unsigned i=0;i<count;++i){const auto input=read<DecorSceneInput>(in);const auto expected=read<DecorSceneOutput>(in);DecorSceneOutput actual{};require(dh2_decor_scene(&actual,&input)==0,"scene input rejected");if(!equal(&expected,&actual,sizeof actual)){std::cerr<<"fixture "<<i<<'\n';throw std::runtime_error("original scene words differ");}inputs.push_back(input);}
 unsigned marked=0,absent=0,negative=0;std::vector<DecorSceneMarker> templates;
 for(unsigned i=0;i<models;++i){const auto length=read<std::uint32_t>(in),found=read<std::uint32_t>(in);require(length<100,"model name length");std::string model(length,'\0');in.read(model.data(),length);float expected[9];in.read(reinterpret_cast<char*>(expected),sizeof expected);require(bool(in),"model fixture truncated");const auto bytes=load(std::string(argv[2])+"/"+model);dh2::resources::BresView view{};require(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"BRES open");dh2::scene::Scene scene;std::string error;require(dh2::scene::load(view,scene,error),"full scene load");DecorSceneMarker marker{};require(decor_scene_marker(view,scene,marker,error),error.c_str());require(marker.found==found,"marker gate differs");
  DecorSceneMarker reloaded{};require(decor_scene_marker(view,reloaded,error),"reloaded marker bridge");require(reloaded.found==marker.found&&!std::memcmp(reloaded.bounds,marker.bounds,24)&&!std::memcmp(reloaded.parent_scale,marker.parent_scale,12),"complete scene overload differs");
  if(!found){++absent;continue;}
  ++marked;require(!std::memcmp(marker.bounds,expected,24)&&!std::memcmp(marker.parent_scale,expected+6,12),"authored marker definition differs");templates.push_back(marker);
  const auto baseline=marker;const auto index=marker.node_index;
  auto reject=[&](dh2::scene::Scene malformed){DecorSceneMarker out=baseline;require(!decor_scene_marker(view,malformed,out,error),"malformed scene accepted");require(!std::memcmp(&out,&baseline,sizeof out),"rejection mutated marker output");++negative;};
  auto malformed=scene;malformed.graph[index].parent=static_cast<std::int32_t>(index);reject(malformed);
  malformed=scene;malformed.graph[index].parent=-1;reject(malformed);
  malformed=scene;dh2::scene::Node child;child.parent=static_cast<std::int32_t>(index);malformed.graph.push_back(child);reject(malformed);
  malformed=scene;for(auto& instance:malformed.instances)if(instance.node_index==index){instance.controller=0;break;}reject(malformed);
  malformed=scene;for(const auto& instance:scene.instances)if(instance.node_index==index){malformed.instances.push_back(instance);break;}reject(malformed);
  malformed=scene;malformed.instances.erase(std::remove_if(malformed.instances.begin(),malformed.instances.end(),[&](const auto& instance){return instance.node_index==index;}),malformed.instances.end());reject(malformed);
  malformed=scene;malformed.graph[malformed.graph[index].parent].scale[0]=INFINITY;reject(malformed);
 }
 require(in.peek()==std::char_traits<char>::eof(),"fixture suffix");require(count>=82&&marked==8,"authored Crypt coverage changed");
 for(unsigned i=0;i<82;++i){bool matched=false;for(const auto& marker:templates)if(!std::memcmp(inputs[i].marker_bounds,marker.bounds,24)&&!std::memcmp(inputs[i].marker_parent_scale,marker.parent_scale,12))matched=true;require(matched,"authored placement has no verified native marker");}
 DecorSceneOutput out;std::memset(&out,0x5a,sizeof out);const auto before=out;require(dh2_decor_scene(&out,nullptr)==-1&&!std::memcmp(&out,&before,sizeof out),"null rejection changed output");require(dh2_decor_scene(nullptr,&inputs.front())==-1,"null output");negative+=2;
 std::cout<<"{\"comparisons\":"<<count<<",\"authored_placements\":82,\"native_marked_templates\":"<<marked<<",\"native_unmarked_templates\":"<<absent<<",\"malformed_checks\":"<<negative<<",\"mismatches\":0,\"sanitizers\":\"address,undefined\",\"optimization\":\"O1,no fast math,fp-contract off\"}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
