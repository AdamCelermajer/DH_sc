#include "source_fx_node_matrix_v4.hpp"
#include "visual_motion.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
namespace dh2::fx {namespace {
float mul(float a,float b){volatile float value=a*b;return value;}
float add(float a,float b){volatile float value=a+b;return value;}
}
void source_fx_matrix_multiply_v4(math::Matrix4f& out,const math::Matrix4f& left,const math::Matrix4f& right){
 const auto a=left,b=right;math::Matrix4f value;std::memset(&value,0,sizeof(value));
 if(a.identity_hint)std::memcpy(&value,&b,65);
 else if(b.identity_hint)std::memcpy(&value,&a,65);
 else {for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row){
  float sum=add(mul(a.m[row],b.m[col*4]),mul(a.m[4+row],b.m[col*4+1]));
  sum=add(sum,mul(a.m[8+row],b.m[col*4+2]));value.m[col*4+row]=add(sum,mul(a.m[12+row],b.m[col*4+3]));
 }value.identity_hint=0;}
 std::memcpy(&out,&value,sizeof(value));
}
void source_fx_trs_matrix_v4(math::Matrix4f& out,const float* position,const float* rotation,const float* scale){
 // AnimatedFX::SyncIrrData calls VisualObject::SetRotation472874, which
 // constructs Euler(Y,-X,-Z). Authored node quaternion tracks are already
 // in engine coordinates and must NOT receive this owner-boundary mapping.
 math::Quaternion q{};float quaternion[4]{};dh2_visual_rotation(quaternion,rotation);std::memcpy(&q,quaternion,16);math::Matrix4f value;std::memset(&value,0,sizeof(value));dh2_quat_matrix_transposed(&q,&value);
 for(unsigned col=0;col<3;++col){for(unsigned row=0;row<3;++row)value.m[col*4+row]=mul(value.m[col*4+row],scale[col]);value.m[12+col]=position[col];}std::memcpy(&out,&value,sizeof(value));
}
bool source_fx_node_world_matrix_v4(const scene::Scene& scene,std::uint32_t node,math::Matrix4f& out,std::string& error){
 if(node>=scene.graph.size()){error="Required same FX graph node";return false;}
 const auto& n=scene.graph[node];if(n.parent< -1||n.parent>=std::int32_t(node)){error="Required source FX graph parent order";return false;}
 for(float x:n.world)if(!std::isfinite(x)){error="Required finite source FX world matrix";return false;}
 math::Quaternion q{n.quaternion[0],n.quaternion[1],n.quaternion[2],n.quaternion[3]};math::Matrix4f local{};dh2_quat_matrix_transposed(&q,&local);
 math::Matrix4f value;std::memset(&value,0,sizeof(value));std::copy(n.world.begin(),n.world.end(),value.m);value.identity_hint=local.identity_hint;std::memcpy(&out,&value,sizeof(value));return true;
}
bool source_fx_rebuild_graph_world_v4(scene::Scene& scene,std::string& error){
 std::vector<math::Matrix4f> matrices;matrices.reserve(scene.graph.size());
 for(unsigned i=0;i<scene.graph.size();++i){const auto& n=scene.graph[i];
  if(n.parent< -1||n.parent>=std::int32_t(i)){error="Required source FX graph parent order";return false;}
  math::Quaternion q{n.quaternion[0],n.quaternion[1],n.quaternion[2],n.quaternion[3]};math::Matrix4f local;std::memset(&local,0,sizeof(local));dh2_quat_matrix_transposed(&q,&local);
  for(unsigned col=0;col<3;++col){for(unsigned row=0;row<3;++row)local.m[4*col+row]=mul(local.m[4*col+row],n.scale[col]);local.m[12+col]=n.translation[col];}
  math::Matrix4f world;std::memset(&world,0,sizeof(world));if(n.parent<0)std::memcpy(&world,&local,65);else source_fx_matrix_multiply_v4(world,matrices[unsigned(n.parent)],local);
  for(float x:world.m)if(!std::isfinite(x)){error="Required finite source FX graph matrix";return false;}matrices.push_back(world);
 }
 for(auto& instance:scene.instances)if(instance.node_index>=matrices.size()){error="Required same source mesh node";return false;}
 for(unsigned i=0;i<matrices.size();++i)std::copy_n(matrices[i].m,16,scene.graph[i].world.data());
 for(auto& instance:scene.instances)instance.world=scene.graph[instance.node_index].world;return true;
}
// ---- P16 LEVELUP2: glitch CBillboardSceneNode (level_up.bdae PIVOT) ----------------------
// Decoded from libDungeonHunter2.so: CColladaDatabase::constructNode 0x61b2f4 (SNode+76 != 0
// -> createBillboard, slot 64), CBillboardSceneNode::updateAbsolutePosition 0x60cf88 (target
// branch: camera = SceneManager+228 through this+272, mode != 2, sub != 2). Per frame, with
// O = parent absolute position, C = camera up vector (row 1 of the glitch camera matrix, i.e. the
// view matrix row (m1,m5,m9)), Pcam = camera eye position, D = normalize(Pcam-O):
//   Abasis: A = Mrot*axisA, B = Mrot*axisB (normalized), P = normalize(B x A), Q = normalize(A x P)
//   S = normalize(D x C), U = normalize(S x D), F = [-S, U, D] (columns)
//   absolute = T(O) * F * Frame^T * Mrot * TRS(node),  Frame = [P, Q, A]
// Matrices are column-major (m[col*4+row]), as the rest of this file.
namespace {
using M16 = std::array<float,16>;
M16 mul16(const M16& a,const M16& b){
 M16 o{};
 for(unsigned c=0;c<4;++c)for(unsigned r=0;r<4;++r){float s=0;for(unsigned k=0;k<4;++k)s+=a[k*4+r]*b[c*4+k];o[c*4+r]=s;}
 return o;
}
bool inverse_affine16(const M16& m,M16& out){
 const float a=m[0],b=m[4],c=m[8],d=m[1],e=m[5],f=m[9],g=m[2],h=m[6],i=m[10];
 const float det=a*(e*i-f*h)-b*(d*i-f*g)+c*(d*h-e*g);
 if(!(std::fabs(det)>1e-12f))return false;
 const float k=1.0f/det;
 const float r00=(e*i-f*h)*k,r01=(c*h-b*i)*k,r02=(b*f-c*e)*k;
 const float r10=(f*g-d*i)*k,r11=(a*i-c*g)*k,r12=(c*d-a*f)*k;
 const float r20=(d*h-e*g)*k,r21=(b*g-a*h)*k,r22=(a*e-b*d)*k;
 out={};
 out[0]=r00;out[1]=r10;out[2]=r20;out[4]=r01;out[5]=r11;out[6]=r21;out[8]=r02;out[9]=r12;out[10]=r22;
 const float tx=m[12],ty=m[13],tz=m[14];
 out[12]=-(r00*tx+r01*ty+r02*tz);out[13]=-(r10*tx+r11*ty+r12*tz);out[14]=-(r20*tx+r21*ty+r22*tz);out[15]=1;
 return true;
}

void normalize3(float v[3]){
 const float l=std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
 if(l>0){v[0]/=l;v[1]/=l;v[2]/=l;}
}
void cross3(const float a[3],const float b[3],float o[3]){
 o[0]=a[1]*b[2]-a[2]*b[1];o[1]=a[2]*b[0]-a[0]*b[2];o[2]=a[0]*b[1]-a[1]*b[0];
}
bool nonzero3(const float v[3]){return std::fabs(v[0])+std::fabs(v[1])+std::fabs(v[2])>1e-9f;}
bool billboard_supported(const scene::BillboardRecordV1& b){return b.mode!=2&&b.sub!=2;}
M16 trs16(const scene::Node& n){
 math::Quaternion q{n.quaternion[0],n.quaternion[1],n.quaternion[2],n.quaternion[3]};
 math::Matrix4f local{};std::memset(&local,0,sizeof(local));dh2_quat_matrix_transposed(&q,&local);
 M16 m{};std::copy_n(local.m,16,m.data());
 for(unsigned col=0;col<3;++col){for(unsigned row=0;row<3;++row)m[4*col+row]*=n.scale[col];m[12+col]=n.translation[col];}
 m[3]=m[7]=m[11]=0;m[15]=1;
 return m;
}
// Billboard absolute for one node from its (already computed) parent absolute and the camera.
// Returns false when a basis is degenerate; the caller then keeps the plain TRS composition.
bool billboard_absolute16(const scene::BillboardRecordV1& bb,const M16& parent_world,bool has_parent,
    const M16& local,const M16& camera_scene,M16& out){
 float O[3]={0,0,0};M16 mrot{};mrot[0]=mrot[5]=mrot[10]=mrot[15]=1;
 if(has_parent){O[0]=parent_world[12];O[1]=parent_world[13];O[2]=parent_world[14];
  mrot=parent_world;mrot[12]=mrot[13]=mrot[14]=0;mrot[15]=1;}
 auto rotate=[&](const float v[3],float o[3]){
  for(unsigned k=0;k<3;++k)o[k]=mrot[0+k]*v[0]+mrot[4+k]*v[1]+mrot[8+k]*v[2];
 };
 float A[3],B[3],P[3],Q[3],D[3],S[3],U[3],C[3];
 rotate(bb.axis_a.data(),A);rotate(bb.axis_b.data(),B);
 if(!nonzero3(A)||!nonzero3(B))return false;
 normalize3(A);normalize3(B);
 cross3(B,A,P);if(!nonzero3(P))return false;normalize3(P);
 cross3(A,P,Q);if(!nonzero3(Q))return false;normalize3(Q);
 C[0]=camera_scene[1];C[1]=camera_scene[5];C[2]=camera_scene[9];
 for(unsigned k=0;k<3;++k)D[k]=camera_scene[12+k]-O[k];
 if(!nonzero3(D))return false;normalize3(D);
 cross3(D,C,S);if(!nonzero3(S))return false;normalize3(S);
 cross3(S,D,U);if(!nonzero3(U))return false;normalize3(U);
 // Frame^T (orthonormal inverse of [P Q A]): column j = (P[j], Q[j], A[j]).
 M16 frame_t{};
 for(unsigned j=0;j<3;++j){frame_t[j*4+0]=P[j];frame_t[j*4+1]=Q[j];frame_t[j*4+2]=A[j];}
 frame_t[15]=1;
 // F = [-S, U, D] columns.
 M16 facing{};
 for(unsigned k=0;k<3;++k){facing[0+k]=-S[k];facing[4+k]=U[k];facing[8+k]=D[k];}
 facing[15]=1;
 M16 translate{};translate[0]=translate[5]=translate[10]=translate[15]=1;
 translate[12]=O[0];translate[13]=O[1];translate[14]=O[2];
 out=mul16(mul16(mul16(translate,facing),mul16(frame_t,mrot)),local);
 return true;
}
}
bool source_fx_scene_has_billboards_v1(const scene::Scene& scene) noexcept {
 for(const auto& n:scene.graph)if(n.source_storage_v91().billboard.present)return true;
 return false;
}
bool source_fx_rebuild_graph_world_billboards_v1(scene::Scene& scene,const math::Matrix4f& outer,
    const float view16[16],const float position3[3],std::string& error){
 error.clear();
 M16 view{};std::copy_n(view16,16,view.data());
 M16 outer16{};std::memcpy(outer16.data(),outer.m,sizeof(outer16));
 M16 outer_inverse{};
 if(!inverse_affine16(outer16,outer_inverse)){error="Required invertible FX outer transform for billboards";return false;}
 // The glitch camera absolute matrix is the world-to-eye view matrix: its row 1 (m1,m5,m9) is the
 // camera up vector, while the camera position is the separate absolute position (the eye). Both are
 // expressed in FX scene space (view * outer; eye through inverse outer).
 M16 camera_scene=mul16(view,outer16);
 const float eye_world[3]={position3[0],position3[1],position3[2]};
 for(unsigned r=0;r<3;++r)camera_scene[12+r]=outer_inverse[0+r]*eye_world[0]+outer_inverse[4+r]*eye_world[1]+outer_inverse[8+r]*eye_world[2]+outer_inverse[12+r];
 camera_scene[15]=1;
 std::vector<M16> matrices;matrices.reserve(scene.graph.size());
 for(unsigned i=0;i<scene.graph.size();++i){const auto& n=scene.graph[i];
  if(n.parent< -1||n.parent>=std::int32_t(i)){error="Required source FX graph parent order";return false;}
  const M16 local=trs16(n);
  const bool has_parent=n.parent>=0;
  const M16 parent_world=has_parent?matrices[unsigned(n.parent)]:M16{};
  M16 world{};
  const auto& bb=n.source_storage_v91().billboard;
  const bool facing=bb.present&&billboard_supported(bb)&&billboard_absolute16(bb,parent_world,has_parent,local,camera_scene,world);
  if(!facing)world=has_parent?mul16(parent_world,local):local;
  for(float x:world)if(!std::isfinite(x)){error="Required finite source FX billboard matrix";return false;}
  matrices.push_back(world);
 }
 for(auto& instance:scene.instances)if(instance.node_index>=matrices.size()){error="Required same source mesh node";return false;}
 for(unsigned i=0;i<matrices.size();++i){
  std::memcpy(scene.graph[i].world.data(),matrices[i].data(),sizeof(M16));
 }
 for(auto& instance:scene.instances)instance.world=scene.graph[instance.node_index].world;
 return true;
}
}
