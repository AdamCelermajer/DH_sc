#include "authored_fx_geometry_packet_v7.hpp"
#include <algorithm>
#include <stdexcept>
#include <cstring>
namespace dh2::fx {
bool authored_fx_geometry_packet_v7(const skinning::VisualDrawPartV6& part,
 std::uint32_t material_index,AuthoredFxGeometryPacketV7& out,std::string& error){try{
 if(!part.geometry||!part.material_table||!part.materials||material_index>=part.material_table->size()||part.geometry->primitives.size()!=1||part.materials->size()!=1||part.materials->front()!=material_index||part.positions.empty()||part.positions.size()!=part.geometry->positions.size()||part.positions.size()>65535)
  throw std::runtime_error("Required retained authored effect draw backing");
 const auto& material=part.material_table->at(material_index);const auto& primitive=part.geometry->primitives.front();
 if(primitive.engine_type!=6||primitive.collada_type!=0||primitive.material_symbol!=material.id||primitive.indices.empty()||primitive.indices.size()%3||primitive.attributes[4]<0)
  throw std::runtime_error("Required source triangle-list packet: material "+material.id+" collada "+std::to_string(primitive.collada_type)+" engine "+std::to_string(primitive.engine_type)+" indices "+std::to_string(primitive.indices.size()));
 const auto color_index=primitive.attributes[2],uv_index=primitive.attributes[4];
 if(std::size_t(uv_index)>=part.geometry->attributes.size()||(color_index>=0&&std::size_t(color_index)>=part.geometry->attributes.size()))throw std::runtime_error("Required source effect attribute index");
 const auto* colors=color_index<0?nullptr:&part.geometry->attributes[color_index];const auto& uv=part.geometry->attributes[uv_index];
 if((colors&&(colors->type!=1||colors->components!=4||colors->values.size()!=part.positions.size()*4))||uv.components!=2||uv.values.size()!=part.positions.size()*2)
  throw std::runtime_error("Required source effect vertex streams");
 AuthoredFxGeometryPacketV7 result;result.source_color_missing=!colors;result.vertices.resize(part.positions.size());
 for(std::size_t i=0;i<result.vertices.size();++i){auto& vertex=result.vertices[i];std::copy_n(part.positions[i].data(),3,vertex.p);std::copy_n(uv.values.data()+i*2,2,vertex.uv);if(colors)for(unsigned j=0;j<4;++j)vertex.color[j]=colors->values[i*4+j]/255.f;}
 result.indices.reserve(primitive.indices.size());for(auto index:primitive.indices){if(index>=result.vertices.size())throw std::runtime_error("Source effect index range");result.indices.push_back(std::uint16_t(index));}
 out=std::move(result);return true;
 }catch(const std::exception& e){error=e.what();return false;}}
bool AuthoredFxGeometryCacheV34::bind_cpu_budget_v41(std::shared_ptr<resources::ContextResourceBudgetV37> budget,std::string& error){
 if(!budget||(cpu_budget_v41_&&cpu_budget_v41_!=budget)){error="V41 same FX CPU budget owner required";return false;}
 if(!cpu_budget_v41_&&(packet_.vertices.capacity()||packet_.indices.capacity()||scratch_vertices_.capacity()||scratch_indices_.capacity()||source_indices_.capacity())){error="V41 FX vector adoption after allocation prohibited";return false;}
 cpu_budget_v41_=std::move(budget);return true;
}
void AuthoredFxGeometryCacheV34::reset_buffers_v41()noexcept{
 std::vector<objects::Vertex>().swap(packet_.vertices);std::vector<std::uint16_t>().swap(packet_.indices);
 std::vector<objects::Vertex>().swap(scratch_vertices_);std::vector<std::uint16_t>().swap(scratch_indices_);std::vector<std::uint32_t>().swap(source_indices_);
 packet_vertices_v41_.reset();packet_indices_v41_.reset();scratch_vertices_v41_.reset();scratch_indices_v41_.reset();source_indices_v41_.reset();
 packet_.source_color_missing=false;validated_vertex_count_=0;counters_={};
}
bool AuthoredFxGeometryCacheV34::update(const skinning::VisualDrawPartV6& part,
 std::uint32_t material_index,AuthoredFxGeometryChangeV34& change,std::string& error){try{
 change={};
 if(!part.geometry||!part.material_table||!part.materials||material_index>=part.material_table->size()||part.geometry->primitives.size()!=1||part.materials->size()!=1||part.materials->front()!=material_index||part.positions.empty()||part.positions.size()!=part.geometry->positions.size()||part.positions.size()>65535)
  throw std::runtime_error("Required retained authored effect draw backing");
 const auto& material=part.material_table->at(material_index);const auto& primitive=part.geometry->primitives.front();
 if(primitive.engine_type!=6||primitive.collada_type!=0||primitive.material_symbol!=material.id||primitive.indices.empty()||primitive.indices.size()%3||primitive.attributes[4]<0)
  throw std::runtime_error("Required source triangle-list packet: material "+material.id+" collada "+std::to_string(primitive.collada_type)+" engine "+std::to_string(primitive.engine_type)+" indices "+std::to_string(primitive.indices.size()));
 const auto color_index=primitive.attributes[2],uv_index=primitive.attributes[4];
 if(std::size_t(uv_index)>=part.geometry->attributes.size()||(color_index>=0&&std::size_t(color_index)>=part.geometry->attributes.size()))throw std::runtime_error("Required source effect attribute index");
 const auto* colors=color_index<0?nullptr:&part.geometry->attributes[color_index];const auto& uv=part.geometry->attributes[uv_index];
 if((colors&&(colors->type!=1||colors->components!=4||colors->values.size()!=part.positions.size()*4))||uv.components!=2||uv.values.size()!=part.positions.size()*2)
  throw std::runtime_error("Required source effect vertex streams");
 const bool topology=source_indices_!=primitive.indices;
 if(topology||validated_vertex_count_!=part.positions.size())for(auto i:primitive.indices)if(i>=part.positions.size())throw std::runtime_error("Source effect index range");
 if(cpu_budget_v41_&&!resources::reserve_cpu_vector_v41(scratch_vertices_,scratch_vertices_v41_,cpu_budget_v41_,resources::ResourceScopeV37::fx,part.positions.size(),error))return false;
 scratch_vertices_.resize(part.positions.size());
 for(std::size_t i=0;i<scratch_vertices_.size();++i){auto& vertex=scratch_vertices_[i];vertex=objects::Vertex{};
  std::copy_n(part.positions[i].data(),3,vertex.p);std::copy_n(uv.values.data()+i*2,2,vertex.uv);
  if(colors)for(unsigned j=0;j<4;++j)vertex.color[j]=colors->values[i*4+j]/255.f;
 }
 static_assert(sizeof(objects::Vertex)==9*sizeof(float),"Exact renderer vertex bytes have no padding");
 const bool vertices=packet_.vertices.size()!=scratch_vertices_.size()||std::memcmp(packet_.vertices.data(),scratch_vertices_.data(),scratch_vertices_.size()*sizeof(objects::Vertex));
 if(topology){
  if(cpu_budget_v41_&&!resources::reserve_cpu_vector_v41(scratch_indices_,scratch_indices_v41_,cpu_budget_v41_,resources::ResourceScopeV37::fx,primitive.indices.size(),error))return false;
  scratch_indices_.resize(primitive.indices.size());for(std::size_t i=0;i<scratch_indices_.size();++i)scratch_indices_[i]=static_cast<std::uint16_t>(primitive.indices[i]);}
 // Publication follows all validation/conversion. Invalid streams preserve
 // the accepted packet and cached topology, and cannot hide a later retry.
 // Source index snapshot allocation precedes packet publication. Numeric
 // assignment and swaps below cannot throw after admitted capacity exists.
 if(topology){
  if(cpu_budget_v41_&&!resources::reserve_cpu_vector_v41(source_indices_,source_indices_v41_,cpu_budget_v41_,resources::ResourceScopeV37::fx,primitive.indices.size(),error))return false;
  source_indices_=primitive.indices;
 }
 if(vertices){packet_.vertices.swap(scratch_vertices_);std::swap(packet_vertices_v41_,scratch_vertices_v41_);}
 if(topology){packet_.indices.swap(scratch_indices_);std::swap(packet_indices_v41_,scratch_indices_v41_);}
 validated_vertex_count_=part.positions.size();packet_.source_color_missing=!colors;
 change={vertices,topology};++counters_.updates;if(vertices)++counters_.vertex_changes;else ++counters_.unchanged_vertices;if(topology)++counters_.topology_rebuilds;
 error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}}
}
