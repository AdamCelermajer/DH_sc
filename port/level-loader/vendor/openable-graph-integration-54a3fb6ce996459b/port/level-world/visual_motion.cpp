#include "visual_motion.hpp"
#include <cstring>
#include "../engine-math/math.hpp"
namespace {
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float neg(float a){std::uint32_t v;std::memcpy(&v,&a,4);v^=0x80000000u;std::memcpy(&a,&v,4);return a;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return a&&b&&(x<=y?y-x<an:x-y<bn);}
bool valid(const dh2::visual::Request* r){
 if(!r||r->reserved||(r->flags&~3u)||(r->root&&(r->root->presence&~3u))||(r->body&&(!r->transform||r->body->flags>65535))||!r->services||!r->services->invoke)return false;
 const void* p[]={r,r->state,r->root,r->body,r->transform,r->services};const std::size_t n[]={48,128,96,48,16,16};
 for(unsigned i=0;i<6;++i)for(unsigned j=i+1;j<6;++j)if(overlap(p[i],n[i],p[j],n[j]))return false;
 return true;
}
void sync(const dh2::visual::Request* r){
 if(r->state&&r->root){std::memcpy(r->root->position,r->state->position,12);r->root->flags|=8;float p[3];std::memcpy(p,r->root->position,12);r->services->invoke(r->services->context,dh2::visual::absolute_position,p);}
}
}
extern "C" int dh2_visual_calculate_delta(dh2::visual::Delta* s,std::uint32_t t,const float* p){
 if(!s||!p||overlap(s,28,p,12))return -1;
 for(unsigned i=0;i<3;++i)s->value[i]=s->timestamp==t?0.f:sub(p[i],s->previous[i]);
 std::memcpy(s->previous,p,12);s->timestamp=t;return 0;
}
extern "C" int dh2_visual_reset_delta(dh2::visual::Delta* s,std::uint32_t t,const float* p){
 if(!s||!p||overlap(s,28,p,12))return -1;
 std::memcpy(s->previous,p,12);s->timestamp=t;std::memset(s->value,0,12);return 0;
}
extern "C" int dh2_visual_displace(const dh2::visual::Displacement* r){
 if(!r||!r->root||r->reserved||r->count>65536||(r->count&&!r->deltas)||(r->root->presence&~3u)||overlap(r,24,r->root,96)||overlap(r->root,96,r->deltas,std::size_t(r->count)*12)||overlap(r,24,r->deltas,std::size_t(r->count)*12))return -1;
 auto& s=*r->root;float d[3]{};for(unsigned j=0;j<r->count;++j)for(unsigned i=0;i<3;++i)d[i]=add(d[i],r->deltas[j*3+i]);
 float v[]={mul(d[0],s.scale[0]),mul(d[1],s.scale[1]),0.f};const float* q=s.quaternion;
 const float t0=add(mul(neg(q[1]),v[2]),mul(q[2],v[1]));
 const float t1=add(mul(neg(q[2]),v[0]),mul(q[0],v[2]));
 const float t2=add(mul(v[1],neg(q[0])),mul(q[1],v[0]));
 const float twice=add(q[3],q[3]);
 float u0=add(mul(neg(q[1]),t2),mul(q[2],t1));
 float u1=add(mul(t0,neg(q[2])),mul(q[0],t2));
 float u2=add(mul(t1,neg(q[0])),mul(q[1],t0));
 float out[]={add(add(u0,u0),add(v[0],mul(twice,t0))),add(add(u1,u1),add(v[1],mul(twice,t1))),add(add(u2,u2),add(v[2],mul(twice,t2)))};
 for(unsigned i=0;i<3;++i)s.position[i]=add(s.position[i],out[i]);
 s.flags|=8;
 if(s.presence&1){for(unsigned i=0;i<3;++i)s.helper[i]=neg(s.animated[i]);s.helper_flags|=8;}
 else {float raw[3];std::memcpy(raw,s.animated,12);s.animated[0]=s.animated[1]=0.f;s.animated_flags|=8;if(s.presence&2){for(unsigned i=0;i<3;++i)s.secondary[i]=sub(s.secondary[i],raw[i]);s.secondary_flags|=8;}}
 return out[0]!=0.f||out[1]!=0.f||out[2]!=0.f;
}
extern "C" int dh2_visual_sync_position(const dh2::visual::Request* r){if(!valid(r))return -1;sync(r);return 0;}
extern "C" int dh2_visual_rotation(float* out,const float* euler){
 if(!out||!euler||overlap(out,16,euler,12))return -1;
 dh2::math::Quaternion q;
 dh2_quat_from_euler(&q,euler[1],neg(euler[0]),neg(euler[2]));std::memcpy(out,&q,16);return 0;
}
extern "C" int dh2_visual_apply_position(const dh2::visual::Request* r){
 if(!valid(r))return -1;
 if(!r->state)return 0;
 auto& s=*r->state;
 float point[3]{};if(r->root)std::memcpy(point,r->root->position,12);
 if(r->flags&1){float dy=sub(point[1],s.position[1]),dz=sub(point[2],s.position[2]),dx=sub(point[0],s.position[0]);s.auxiliary_position[0]=add(s.auxiliary_position[0],dx);s.auxiliary_position[1]=add(s.auxiliary_position[1],dy);s.auxiliary_position[2]=add(s.auxiliary_position[2],dz);}
 std::memcpy(s.position,point,12);for(unsigned i=0;i<6;++i)s.absolute_bounds[i]=add(s.local_bounds[i],s.position[i%3]);
 if(r->body){dh2_physical_request_position(r->transform,r->body,s.position);float p[]={r->transform->position[0],r->transform->position[1],r->transform->angle};r->services->invoke(r->services->context,dh2::subobjects::apply_body_transform,p);}
 if(r->flags&2)sync(r);
 return 0;
}
#ifndef DH2_VISUAL_KERNEL_ONLY
#include <cmath>
namespace dh2::visual {
bool SceneBinding::set_rotation(const float* euler){if(dh2_visual_rotation(root.quaternion,euler))return false;root.flags|=4;return true;}
bool SceneBinding::bind(const scene::Scene& s,std::string& error){
 error.clear();animated=-1;identities.clear();
 // Original findSceneNodeRef scans by name in depth-first child-list order.
 std::vector<std::vector<unsigned>> children(s.graph.size()+1);std::vector<unsigned> order,pending;
 for(unsigned i=0;i<s.graph.size();++i){auto parent=s.graph[i].parent;if(parent< -1||parent>=std::int32_t(i)){error="Root motion graph order rejected";return false;}children[parent<0?s.graph.size():unsigned(parent)].push_back(i);}
 for(auto i=children.back().rbegin();i!=children.back().rend();++i)pending.push_back(*i);
 while(!pending.empty()){auto i=pending.back();pending.pop_back();order.push_back(i);for(auto j=children[i].rbegin();j!=children[i].rend();++j)pending.push_back(*j);}
 for(const char* name:{"root_camera","Bip01","Root","root_character"}){
  for(unsigned i:order)if(s.graph[i].name==name){animated=i;break;}
  if(animated>=0)break;
 }
 if(animated<0){error="Original animation root name is absent";return false;}
 for(const auto& n:s.graph)identities.push_back(n.id);
 history={};root.presence=1;
 return true;
}
bool SceneBinding::update_world(scene::Scene& s,std::string& error)const{
 error.clear();if(animated<0||s.graph.size()!=identities.size()){error="Root motion scene differs from binding";return false;}
 std::array<float,16> owner,helper;const float q[]={0,0,0,1},scale[]={1,1,1};
 dh2_node_matrix(owner.data(),root.position,root.quaternion,root.scale);dh2_node_matrix(helper.data(),root.helper,q,scale);
 const auto parent=scene::multiply(owner,helper);std::vector<std::array<float,16>> matrices;matrices.reserve(s.graph.size());
 for(unsigned i=0;i<s.graph.size();++i){const auto& n=s.graph[i];if(n.id!=identities[i]||n.parent< -1||n.parent>=std::int32_t(i)){error="Root motion graph order differs from binding";return false;}
  std::array<float,16> local;dh2_node_matrix(local.data(),n.translation,n.quaternion,n.scale);matrices.push_back(scene::multiply(n.parent<0?parent:matrices[n.parent],local));
  for(float f:matrices.back())if(!std::isfinite(f)){error="Root motion transform overflow";return false;}
 }
 for(const auto& i:s.instances)if(i.node_index>=matrices.size()){error="Root motion instance link rejected";return false;}
 for(unsigned i=0;i<matrices.size();++i)s.graph[i].world=matrices[i];
 for(auto& i:s.instances)i.world=matrices[i.node_index];
 return true;
}
bool SceneBinding::sample(scene::Scene& s,const animation::Player& clip,std::int32_t ms,std::uint32_t timestamp,bool reset,std::string& error){
 return sample(s,clip,ms,timestamp,reset,true,error);
}
bool SceneBinding::sample(scene::Scene& s,const animation::Player& clip,std::int32_t ms,std::uint32_t timestamp,bool reset,bool displacement,std::string& error){
 if(animated<0||s.graph.size()!=identities.size()){error="Root motion scene differs from binding";return false;}
 if(reset){if(!clip.sample(s,clip.start,error))return false;dh2_visual_reset_delta(&history,timestamp+1,s.graph[animated].translation);}
 if(!clip.sample(s,ms,error))return false;
 std::memcpy(root.animated,s.graph[animated].translation,12);
 dh2_visual_calculate_delta(&history,timestamp,root.animated);
 if(displacement){Displacement request{&root,history.value,1,0};dh2_visual_displace(&request);}
 return update_world(s,error);
}
}
#endif
