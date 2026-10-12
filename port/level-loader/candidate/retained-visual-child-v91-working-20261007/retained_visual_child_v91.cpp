#include "retained_visual_child_v91.hpp"
#include <cmath>
#include <limits>
#include "math.hpp"
#include <cstring>
namespace dh2::world {
bool RetainedMeshDataV91::create(const resources::BresView& source,std::uint32_t geometry,
 std::shared_ptr<RetainedMeshDataV91>& out,std::string& e){
 assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&source,static_cast<std::int32_t>(geometry))!=assets::Error::ok){e="Invalid actual retained child mesh";return false;}
 auto data=std::make_shared<RetainedMeshDataV91>();for(unsigned i=0;i<3;++i){data->bounds[i]=mesh.minimum[i];data->bounds[i+3]=mesh.maximum[i];}
 std::int32_t position_index=-1;assets::Attribute positions{};
 for(std::uint32_t p=0;p<mesh.primitives;++p){assets::Primitive primitive{};
  if(dh2_mesh_primitive(&mesh,p,&primitive)!=assets::Error::ok||primitive.collada_type||primitive.index_count%3){e="Child triangle mesh outside SAME admitted render primitive domain";return false;}
  const auto attribute=primitive.attributes[0];
  if(attribute<0||(position_index>=0&&attribute!=position_index)){e="Actual child mesh requires one source position stream";return false;}
  if(position_index<0){position_index=attribute;if(dh2_mesh_attribute(&mesh,attribute,&positions)!=assets::Error::ok||positions.components!=3){e="Invalid retained child XYZ stream";return false;}
   data->source_positions.resize(positions.vertices);for(std::uint32_t v=0;v<positions.vertices;++v)if(!dh2_attribute_read(&positions,v,data->source_positions[v].data())){e="Invalid source child vertex";return false;}}
  for(std::uint32_t i=0;i<primitive.index_count;i+=3){std::array<std::uint32_t,3> triangle{};
   for(unsigned k=0;k<3;++k)if(!dh2_index_read(&primitive,i+k,&triangle[k])||triangle[k]>=data->source_positions.size()){e="Invalid actual child triangle index";return false;}
   data->triangles.push_back(triangle);}
 }
 data->positions=data->source_positions;out=std::move(data);e.clear();return true;
}
void RetainedVisualNodeV91::collect_meshes(std::vector<std::shared_ptr<RetainedMeshNodeV91>>& out)const{
 for(const auto& mesh:meshes)if(mesh&&!mesh->fields->detached)out.push_back(mesh);
 for(const auto& node:children)if(node&&!node->fields->detached)node->collect_meshes(out);
}
bool RetainedTriangleSelectorV91::line(const float* start,const float* end,bool& hit,std::array<float,3>& point,std::string& e)const{
 hit=false;if(!mesh_||!start||!end){e="Required actual retained triangle selector/line";return false;}
 const auto subtract=[](const auto& a,const auto& b){return std::array<float,3>{a[0]-b[0],a[1]-b[1],a[2]-b[2]};};
 const auto cross=[](const auto& a,const auto& b){return std::array<float,3>{a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};};
 const auto dot=[](const auto& a,const auto& b){return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];};
 const std::array<float,3> origin{start[0],start[1],start[2]},direction{end[0]-start[0],end[1]-start[1],end[2]-start[2]};
 float nearest=std::numeric_limits<float>::infinity();
 // Original selector reads IMesh SVertexStream, not the port's derived
 // CPU skinned render output. Both streams belong to this SAME mesh owner.
 for(const auto& indices:mesh_->triangles){if(indices[0]>=mesh_->source_positions.size()||indices[1]>=mesh_->source_positions.size()||indices[2]>=mesh_->source_positions.size()){e="Changed actual selector vertex domain";return false;}
  const auto& a=mesh_->source_positions[indices[0]];const auto& b=mesh_->source_positions[indices[1]];const auto& c=mesh_->source_positions[indices[2]];
  const auto u=subtract(b,a),v=subtract(c,a),crossed=cross(u,v);
  dh2::math::Vector3f native_normal{-crossed[0],-crossed[1],-crossed[2]};dh2_vec3_normalize(&native_normal);
  const std::array<float,3> normal{native_normal.x,native_normal.y,native_normal.z};
  const auto denominator=dot(normal,direction);std::uint32_t epsilon_bits=0x358637bd;float epsilon;std::memcpy(&epsilon,&epsilon_bits,4);
  if(std::fabs(denominator)<=epsilon)continue;const auto relative=subtract(a,origin);const auto t=dot(normal,relative)/denominator;
  if(t<0.f||t>1.f||t>=nearest)continue;
  const std::array<float,3> candidate{origin[0]+direction[0]*t,origin[1]+direction[1]*t,origin[2]+direction[2]*t};
  const auto same_side=[&](const auto& edge_a,const auto& edge_b,const auto& opposite){
   const auto edge=subtract(edge_b,edge_a);return dot(cross(edge,subtract(candidate,edge_a)),cross(edge,subtract(opposite,edge_a)))>=0.f;};
  if(!same_side(b,c,a)||!same_side(a,c,b)||!same_side(a,b,c))continue;
  nearest=t;point=candidate;hit=true;
 }
 e.clear();return true;
}
}
