#include "authored_fx_geometry_packet_v7.hpp"
#include <algorithm>
#include <stdexcept>
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
}
