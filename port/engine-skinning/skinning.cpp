#include "skinning.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
namespace {
float add(float a,float b){volatile float value=a+b;return value;}
float mul(float a,float b){volatile float value=a*b;return value;}
struct Reader {
 const dh2::resources::BresView& v;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{
  if(!v.bytes||p>v.size||n>v.size-p)throw std::runtime_error("Skin field outside BRES");
  return v.bytes+p;
 }
 unsigned w(std::uint64_t p)const{const auto* b=at(p,4);return b[0]|(unsigned(b[1])<<8)|(unsigned(b[2])<<16)|(unsigned(b[3])<<24);}
 float f(std::uint64_t p)const{const auto raw=w(p);float value;std::memcpy(&value,&raw,4);if(!std::isfinite(value))throw std::runtime_error("Nonfinite skin component");return value;}
 std::string text(unsigned p)const{
  if(!p)throw std::runtime_error("Missing skin identifier");
  const auto* start=at(p,1);
  const auto* end=static_cast<const std::uint8_t*>(std::memchr(start,0,std::min<std::size_t>(4096,v.size-p)));
  if(!end)throw std::runtime_error("Unterminated skin identifier");
  return {reinterpret_cast<const char*>(start),std::size_t(end-start)};
 }
 unsigned buffer(unsigned p,std::uint64_t size)const{
  if(static_cast<std::int32_t>(w(v.root_offset+100))>0){at(p,16);if(w(p+12)||size>(w(p+8)&~3u))throw std::runtime_error("Invalid deferred skin buffer");p=w(p+4);}
  at(p,size);return p;
 }
};
}
extern "C" void dh2_skin_matrix(float* out,const float* a,const float* b){
 // Original SMatrix multiplication treats both operands as affine. Its
 // inverse-bind matrices contain no runtime identity byte.
 float result[16]{};
 for(unsigned col=0;col<4;++col)for(unsigned row=0;row<3;++row){
  float value=add(add(mul(a[row],b[col*4]),mul(a[4+row],b[col*4+1])),mul(a[8+row],b[col*4+2]));
  result[col*4+row]=col==3?add(value,a[12+row]):value;
 }
 result[15]=1;std::copy(result,result+16,out);
}
extern "C" void dh2_skin_point(float* out,const float* matrices,const std::uint8_t* indices,const float* weights,unsigned count,const float* point){
 float result[3]{};
 for(unsigned i=0;i<count&&weights[i]!=0.f;++i){const auto* m=matrices+indices[i]*16;
  for(unsigned row=0;row<3;++row){
   float value=add(add(add(mul(point[0],m[row]),mul(point[1],m[4+row])),mul(point[2],m[8+row])),m[12+row]);
   result[row]=add(result[row],mul(value,weights[i]));
  }
 }
 std::copy(result,result+3,out);
}
extern "C" void dh2_skin_palette_matrix(float* out,const float* world,const float* inverse_bind,const float* bind_shape){
 float intermediate[16];dh2_skin_matrix(intermediate,world,inverse_bind);dh2_skin_matrix(out,intermediate,bind_shape);
}
namespace dh2::skinning {
bool load(const resources::BresView& view,unsigned controller,const scene::Scene& scene,Skin& out,std::string& error){
 out={};error.clear();try{
  Reader r{view};const auto* record=dh2_bres_library_item(&view,resources::Library::controller,controller);
  if(!record)throw std::runtime_error("Controller index out of range");
  const auto c=record-view.bytes;
  if(r.w(c)!=0)throw std::runtime_error("Controller is not a skin");
  const auto s=r.w(c+8);r.at(s,156);
  Skin skin;skin.id=r.text(r.w(c+4));skin.influence_count=*r.at(s+152,1);
  const auto count=r.w(s+116);
  if(!count||count>256||!skin.influence_count||skin.influence_count>4)
   throw std::runtime_error("Unsupported skin dimensions");
  // prepareCache multiplies world*inverse-bind, then bind-shape, at
  // 0x66fefc and 0x66ff10. Raw mesh positions are used by the vertex kernel.
  for(unsigned i=0;i<16;++i)skin.bind_shape[i]=r.f(s+16+i*4);
  if(skin.bind_shape[3]!=0||skin.bind_shape[7]!=0||skin.bind_shape[11]!=0||skin.bind_shape[15]!=1)throw std::runtime_error("Nonaffine bind-shape matrix");
  const auto source=r.text(r.w(s+112));if(source.empty()||source[0]!='#')throw std::runtime_error("External skin geometry");
  bool found=false;const auto geometries=dh2_bres_library_count(&view,resources::Library::geometry);
  for(unsigned i=0;i<geometries;++i){const auto* g=dh2_bres_library_item(&view,resources::Library::geometry,i);if(!g)throw std::runtime_error("Missing geometry record");
   if(r.text(r.w(g-view.bytes))==source.substr(1)){skin.geometry=i;found=true;break;}}
  if(!found)throw std::runtime_error("Unresolved skin geometry");
  assets::Mesh mesh{};const auto mesh_error=dh2_mesh_open(&mesh,&view,skin.geometry);
  if(mesh_error!=assets::Error::ok||!mesh.vertices||mesh.vertices>1000000)throw std::runtime_error("Skin mesh rejected");
  // prepareSkinStreams and software skin() use the mesh vertex count,
  // rather than interpreting SSkin+0x7c as that count.
  const auto vertices=mesh.vertices;
  const auto names=r.w(s+120),bind=r.w(s+4);r.at(names,count*4);r.at(bind,count*64);
  for(unsigned j=0;j<count;++j){const auto name=r.text(r.w(names+j*4));unsigned matches=0,index=0;
   for(unsigned i=0;i<scene.graph.size();++i)if(scene.graph[i].sid==name){++matches;index=i;}
   if(matches!=1)throw std::runtime_error("Missing or ambiguous skin joint: "+name);
   skin.nodes.push_back(index);Matrix matrix;for(unsigned k=0;k<16;++k)matrix[k]=r.f(bind+j*64+k*4);
   if(matrix[3]!=0||matrix[7]!=0||matrix[11]!=0||matrix[15]!=1)throw std::runtime_error("Nonaffine inverse-bind matrix");
   skin.inverse_bind.push_back(matrix);
  }
  const unsigned stride=(skin.influence_count+1)*4;const auto data=r.buffer(r.w(s+128),std::uint64_t(vertices)*stride);
  skin.influences.reserve(vertices);
  for(unsigned v=0;v<vertices;++v){const auto p=std::uint64_t(data)+std::uint64_t(v)*stride;Influence influence;
   std::copy(r.at(p,4),r.at(p,4)+4,influence.joints.begin());float sum=0;bool zero=false;
   for(unsigned j=0;j<skin.influence_count;++j){const float weight=r.f(p+4+j*4);influence.weights[j]=weight;
    if(weight<0||weight>1||influence.joints[j]>=count||(zero&&weight!=0))throw std::runtime_error("Invalid skin influence");
    zero=zero||weight==0;sum+=weight;
   }
   // Original animated decors contain unweighted vertices. Preserve zero
   // records: software skin() writes zero for them at 0x670964; the native
   // object renderer applies its explicit rigid-vertex policy separately.
   if(sum!=0.f&&std::abs(sum-1.f)>.002f)throw std::runtime_error("Skin weights are not normalized");
   skin.influences.push_back(influence);
  }
  out=std::move(skin);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool palette(const Skin& skin,const scene::Scene& scene,std::vector<Matrix>& out,std::string& error){
 error.clear();std::vector<Matrix> candidate;
 if(skin.nodes.empty()||skin.nodes.size()!=skin.inverse_bind.size()){error="Skin joint table differs";return false;}
 for(unsigned i=0;i<skin.nodes.size();++i){if(skin.nodes[i]>=scene.graph.size()){error="Skin joint out of range";return false;}
  Matrix value;dh2_skin_palette_matrix(value.data(),scene.graph[skin.nodes[i]].world.data(),skin.inverse_bind[i].data(),skin.bind_shape.data());
  for(float x:value)if(!std::isfinite(x)){error="Skin matrix overflow";return false;}
  candidate.push_back(value);
 }
 out=std::move(candidate);return true;
}
bool positions(const Skin& skin,const std::vector<Matrix>& matrices,const std::vector<std::array<float,3>>& input,std::vector<std::array<float,3>>& output,std::string& error){
 error.clear();if(input.size()!=skin.influences.size()||matrices.size()!=skin.nodes.size()||matrices.empty()||!skin.influence_count||skin.influence_count>4){error="Skin input dimensions differ";return false;}
 std::vector<std::array<float,3>> candidate(input.size());
 std::vector<float> contiguous;contiguous.reserve(matrices.size()*16);
 for(const auto& matrix:matrices)contiguous.insert(contiguous.end(),matrix.begin(),matrix.end());
 for(unsigned i=0;i<input.size();++i){const auto& influence=skin.influences[i];
  for(unsigned j=0;j<skin.influence_count;++j)if(influence.joints[j]>=matrices.size()){error="Skin influence out of range";return false;}
  dh2_skin_point(candidate[i].data(),contiguous.data(),influence.joints.data(),influence.weights.data(),skin.influence_count,input[i].data());
  for(float x:candidate[i])if(!std::isfinite(x)){error="Skin position overflow";return false;}
 }
 output=std::move(candidate);return true;
}
bool directions_v113(const Skin& skin,const std::vector<Matrix>& matrices,const std::vector<std::array<float,3>>& input,std::vector<std::array<float,3>>& output,std::string& error){
 if(input.size()!=skin.influences.size()||matrices.size()!=skin.nodes.size()||!skin.influence_count||skin.influence_count>4){error="Actual software-skin normal/influence domain differs";return false;}
 output.resize(input.size());
 for(std::size_t i=0;i<input.size();++i){auto& result=output[i];result={0,0,0};const auto& influence=skin.influences[i];
  for(unsigned j=0;j<skin.influence_count;++j){const float weight=influence.weights[j];if(weight==0.f)break;if(influence.joints[j]>=matrices.size()){error="Actual normal joint outside SAME palette";return false;}
   const auto& matrix=matrices[influence.joints[j]];const auto& v=input[i];
   for(unsigned axis=0;axis<3;++axis){const float rotated=(matrix[axis]*v[0]+matrix[4+axis]*v[1])+matrix[8+axis]*v[2];result[axis]+=rotated*weight;}
  }
  for(float value:result)if(!std::isfinite(value)){error="Nonfinite actual software-skin normal";return false;}
 }
 error.clear();return true;
}
}
