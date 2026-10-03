#include "../component_applicator.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace {
[[maybe_unused]] void check(bool condition,const char* reason){if(!condition)throw std::runtime_error(reason);}
}
#ifndef DH2_COMPONENT_ORACLE
namespace {
using dh2::animation::ComponentNodeState;
unsigned bits(float x){unsigned w;std::memcpy(&w,&x,4);return w;}
bool same(float a,float b,bool arithmetic){return bits(a)==bits(b)||(arithmetic&&std::isnan(a)&&std::isnan(b));}
void state_equal(const ComponentNodeState& a,const ComponentNodeState& b,bool arithmetic,unsigned type){for(unsigned i=0;i<3;++i){check(same(a.position[i],b.position[i],arithmetic&&type<=4),"Position output differs");check(same(a.scale[i],b.scale[i],arithmetic&&type>=11),"Scale output differs");}check(a.dirty==b.dirty,"Dirty flags differ");}
struct Reader{std::vector<char> bytes;std::size_t at=0;explicit Reader(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f),"Missing component corpus");bytes.assign(std::istreambuf_iterator<char>(f),{});}void take(void* p,std::size_t n){check(n<=bytes.size()-at,"Truncated component corpus");if(n)std::memcpy(p,bytes.data()+at,n);at+=n;}unsigned word(){unsigned w;take(&w,4);return w;}};
}
int main(int argc,char**argv){try{
 if(argc!=2)return 2;
 Reader r(argv[1]);check(r.word()==0x31504143,"Wrong component corpus");const unsigned cases=r.word();unsigned direct=0,blended=0,contributions=0,bridges=0,guards=0;
 for(unsigned ci=0;ci<cases;++ci){const unsigned type=r.word(),operation=r.word(),count=r.word(),weight_count=r.word();ComponentNodeState initial;r.take(&initial,28);std::vector<float> values(count*3),weights(weight_count);r.take(values.data(),values.size()*4);r.take(weights.data(),weights.size()*4);ComponentNodeState expected;r.take(&expected,28);float expected_out[3];r.take(expected_out,12);
  ComponentNodeState state=initial;float out[3]{1,2,3};const float* vp=values.empty()?nullptr:values.data();const float* wp=weights.empty()?nullptr:weights.data();const bool arithmetic=operation!=1&&count>1;
  if(operation==0){check(!dh2_animation_component_blend(out,type,vp,wp,count),"Blend rejected valid source input");for(unsigned i=0;i<3;++i)check(same(out[i],expected_out[i],arithmetic),"Contribution differs");++contributions;}
  else if(operation==1){check(!dh2_animation_component_apply(&state,type,vp),"Apply rejected source input");++direct;}
  else{check(!dh2_animation_component_apply_blended(&state,type,vp,wp,count),"Blended apply rejected source input");++blended;}
  state_equal(state,expected,arithmetic,type);
  if(operation){dh2::scene::Node node;std::memcpy(node.translation,initial.position,12);std::memcpy(node.scale,initial.scale,12);node.name="Borrowed component node";node.world.fill(17);auto world=node.world;auto dirty=initial.dirty;
   const int result=operation==1?dh2::animation::apply_component(node,dirty,type,vp):dh2::animation::apply_blended_component(node,dirty,type,vp,wp,count);check(!result,"Scene graph bridge rejected source input");ComponentNodeState graph;std::memcpy(graph.position,node.translation,12);std::memcpy(graph.scale,node.scale,12);graph.dirty=dirty;state_equal(graph,expected,arithmetic,type);check(node.world==world&&node.name=="Borrowed component node","Component setter changed world/identity");++bridges;
  }
 }
 check(r.at==r.bytes.size(),"Trailing component corpus");ComponentNodeState node{{1,2,3},{4,5,6},0x400};const auto old=node;float values[6]{10,11,12,13,14,15},weights[2]{.5f,.5f};
 for(unsigned i=0;i<8;++i){int status=i==0?dh2_animation_component_apply(&node,5,values):i==1?dh2_animation_component_apply(&node,2,nullptr):i==2?dh2_animation_component_apply(&node,2,node.position):i==3?dh2_animation_component_apply_blended(&node,2,values,weights,-1):i==4?dh2_animation_component_apply_blended(&node,2,values,weights,65537):i==5?dh2_animation_component_apply_blended(&node,2,values,nullptr,2):i==6?dh2_animation_component_apply_blended(&node,11,values,reinterpret_cast<float*>(&node.dirty),2):dh2_animation_component_apply(&node,2,reinterpret_cast<float*>(reinterpret_cast<char*>(values)+1));check(status==1&&!std::memcmp(&node,&old,28),"Malformed component call changed state");++guards;}
 for(unsigned type=6;type<=10;++type){check(dh2_animation_component_apply(&node,type,values)==1&&!std::memcmp(&node,&old,28),"Non-component factory type was accepted");++guards;}
 dh2::scene::Scene scene;scene.graph.resize(2);scene.graph[0].parent=-1;scene.graph[1].parent=0;scene.graph[0].translation[0]=10;scene.graph[0].scale[0]=2;scene.graph[0].scale[1]=3;scene.graph[0].scale[2]=4;unsigned dirty=0x400;float first[3]{1,2,3},second[3]{4,5,6};
 check(!dh2::animation::apply_component(scene.graph[1],dirty,2,first)&&!dh2::animation::apply_component(scene.graph[1],dirty,3,second),"Ordered graph component writes failed");check(!std::memcmp(scene.graph[1].translation,second,12)&&dirty==0x408,"Component application incorrectly merged axes");std::string error;check(dh2::scene::update_world(scene,error),error.c_str());check(scene.graph[1].world[12]==18&&scene.graph[1].world[13]==15&&scene.graph[1].world[14]==24,"Owner world refresh failed");
 const auto prior=scene.graph[1].world;check(dh2::animation::apply_component(scene.graph[1],dirty,2,scene.graph[1].translation)==1&&scene.graph[1].world==prior,"Graph alias rejection changed world");++guards;
 std::cout<<"{\"validation\":\"PASS\",\"cases\":"<<cases<<",\"direct_applies\":"<<direct<<",\"blended_applies\":"<<blended<<",\"contributions\":"<<contributions<<",\"scene_bridge_applies\":"<<bridges<<",\"atomic_rejections\":"<<guards<<",\"world_refresh_checks\":1,\"sanitizer_findings\":0}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
