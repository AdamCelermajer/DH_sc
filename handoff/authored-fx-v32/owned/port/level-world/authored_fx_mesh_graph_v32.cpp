#include "../engine-skinning/skinning.hpp"
#include "authored_fx_mesh_graph_v32.hpp"
#include "authored_fx_nonrender_geometry_v32.hpp"
#include "source_fx_node_matrix_v4.hpp"
#include "fx_texture_animation_v1.hpp"
#include "../engine-animation/material_color.hpp"
#include "../engine-animation/material_color_v3.hpp"
#include "../scene-materials/material_matrix_v4.hpp"
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cmath>
namespace dh2::fx {namespace {
skinning::VisualGeometryV6 geometry(const resources::BresView& image,unsigned index){
 assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&image,index)!=assets::Error::ok)throw std::runtime_error("Required authored FX mesh payload");
 skinning::VisualGeometryV6 out;out.id=mesh.id;std::copy_n(mesh.minimum,3,out.minimum);std::copy_n(mesh.maximum,3,out.maximum);
 for(unsigned i=0;i<mesh.attributes;++i){assets::Attribute a{};if(dh2_mesh_attribute(&mesh,i,&a)!=assets::Error::ok)throw std::runtime_error("Required authored FX vertex stream");
  skinning::VisualAttributeV6 stream{a.type,a.components,{}};stream.values.resize(std::size_t(mesh.vertices)*a.components);
  for(unsigned j=0;j<mesh.vertices;++j)if(!dh2_attribute_read(&a,j,stream.values.data()+std::size_t(j)*a.components))throw std::runtime_error("Required authored FX vertex data");out.attributes.push_back(std::move(stream));
 }
 int position=-1;for(unsigned i=0;i<mesh.primitives;++i){assets::Primitive p{};if(dh2_mesh_primitive(&mesh,i,&p)!=assets::Error::ok)throw std::runtime_error("Required authored FX primitive");
  skinning::VisualPrimitiveV6 primitive;primitive.material_symbol=p.material;primitive.collada_type=p.collada_type;primitive.engine_type=p.engine_type;std::copy_n(p.attributes,18,primitive.attributes.begin());
  if(position<0)position=p.attributes[0];else if(position!=p.attributes[0])throw std::runtime_error("Required FX multiple position-stream geometry");
  primitive.indices.resize(p.index_count);for(unsigned j=0;j<p.index_count;++j)if(!dh2_index_read(&p,j,&primitive.indices[j]))throw std::runtime_error("Required authored FX indices");out.primitives.push_back(std::move(primitive));
 }
 if(position<0||std::size_t(position)>=out.attributes.size()||out.attributes[position].components!=3)throw std::runtime_error("Required authored FX position3 stream");
 const auto& data=out.attributes[position].values;out.positions.resize(mesh.vertices);for(unsigned i=0;i<mesh.vertices;++i)std::copy_n(data.data()+3*i,3,out.positions[i].begin());return out;
}
}
struct AuthoredFxMeshGraphV32::Impl {
 struct Receiver{std::uint32_t node{};skinning::VisualGeometryV6 geometry;std::vector<std::uint32_t> materials;skinning::Skin skin;};
 struct Track{assets::Animation animation{};assets::Vector values{};std::uint32_t material{},type{};};
 struct Snapshot{std::shared_ptr<Impl> owner;skinning::VisualGeometryV6 geometry;std::vector<scene::Material> materials;std::vector<std::uint32_t> bindings;};
 std::shared_ptr<const std::vector<std::uint8_t>> bytes;std::shared_ptr<void> graph_owner;
 resources::BresView image{};scene::Scene& scene;std::vector<math::Matrix4f>& matrices;
 std::vector<std::unique_ptr<Receiver>> receivers;std::vector<Track> tracks;
 std::vector<AuthoredFxNonrenderGeometryV32> nonrender_geometry;
 Impl(std::shared_ptr<const std::vector<std::uint8_t>> b,std::shared_ptr<void> owner,scene::Scene& s,std::vector<math::Matrix4f>& m):bytes(std::move(b)),graph_owner(std::move(owner)),scene(s),matrices(m){}
 unsigned word(std::uint64_t offset)const{if(offset>image.size||4>image.size-offset)throw std::runtime_error("Required authored material range");unsigned value;std::memcpy(&value,image.bytes+offset,4);return value;}
 std::string text(unsigned at)const{if(!at||at>=image.size)throw std::runtime_error("Required authored material name");const auto* end=static_cast<const char*>(std::memchr(image.bytes+at,0,image.size-at));if(!end)throw std::runtime_error("Required authored material terminator");return {reinterpret_cast<const char*>(image.bytes+at),end};}
 void defaults(unsigned material){
  const auto* record=dh2_bres_library_item(&image,resources::Library::material,material);if(!record)throw std::runtime_error("Required same authored material record");const unsigned at=unsigned(record-image.bytes);
  if(word(at+8))throw std::runtime_error("Required external authored mesh effect");auto uri=text(word(at+12));if(uri.empty()||uri[0]!='#')throw std::runtime_error("Required authored mesh effect fragment");
  bool color=false,matrix=false;unsigned matches=0;
  for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::effect);++i){const auto* effect=dh2_bres_library_item(&image,resources::Library::effect,i);const unsigned e=unsigned(effect-image.bytes);if(text(word(e))!=uri.substr(1))continue;++matches;
   if(text(word(e+4)).rfind("ProfileCOMMON",0)!=0)throw std::runtime_error("Required authored mesh shader profile");
   const auto n=word(e+16),base=word(e+20);for(unsigned j=0;j<n;++j){const auto p=base+24*j;const auto name=text(word(p));
    if(name=="diffuse-sampler-matrix"){if(word(p+4)!=10||word(p+12)!=1)throw std::runtime_error("Required authored mesh matrix parameter");float values[16];for(unsigned k=0;k<16;++k){auto v=word(word(p+20)+4*k);std::memcpy(values+k,&v,4);}scene::material_matrix_from_bres_v4(matrices.at(material),values);matrix=true;
    }else if(name=="diffuse-color"){if(word(p+12)!=1)throw std::runtime_error("Required authored mesh color count");if(word(p+4)==16){for(unsigned k=0;k<4;++k){auto v=word(word(p+20)+4*k);std::memcpy(scene.materials[material].color+k,&v,4);}}
     else if(word(p+4)==15){animation::ColorParameter32 parameter{8,1,{0,0,0,0},-1,-1};const auto raw=word(word(p+20));std::uint8_t rgba[4];std::memcpy(rgba,&raw,4);if(dh2_material_color_set(&parameter,0,rgba)!=1)throw std::runtime_error("Required authored mesh SColor");std::memcpy(scene.materials[material].color,parameter.words,16);}else throw std::runtime_error("Required authored mesh color type");color=true;
    }
   }
  }
  if(matches!=1||!color||!matrix)throw std::runtime_error("Required complete authored mesh effect defaults");
 }
};
AuthoredFxMeshGraphV32::AuthoredFxMeshGraphV32(std::shared_ptr<const std::vector<std::uint8_t>> b,std::shared_ptr<void> owner,scene::Scene& scene,std::vector<math::Matrix4f>& matrices):impl_(std::make_shared<Impl>(std::move(b),std::move(owner),scene,matrices)){}
bool AuthoredFxMeshGraphV32::initialize(std::string& error){try{auto& s=*impl_;if(!s.bytes||!s.graph_owner||dh2_bres_open(&s.image,s.bytes->data(),s.bytes->size())!=resources::BresError::ok)throw std::runtime_error("Required same authored mesh graph lease");
 std::vector<bool> material(s.scene.materials.size());
 for(const auto& instance:s.scene.instances){AuthoredFxNonrenderGeometryV32 declaration{};bool nonrender=false;
  if(!authored_fx_nonrender_geometry_v32(s.image,instance.geometry,declaration,nonrender,error))return false;
  if(nonrender){if(instance.controller>=0)throw std::runtime_error("Required original kind1 controller continuation");s.nonrender_geometry.push_back(declaration);continue;}
  auto receiver=std::make_unique<Impl::Receiver>();receiver->node=instance.node_index;receiver->geometry=geometry(s.image,instance.geometry);if(instance.controller>=0){if(!skinning::load(s.image,unsigned(instance.controller),s.scene,receiver->skin,error))return false;if(receiver->skin.geometry!=instance.geometry)throw std::runtime_error("Required same source skinned geometry");}
  for(const auto& primitive:receiver->geometry.primitives){unsigned count=0,index=0;for(auto candidate:instance.materials)if(candidate<s.scene.materials.size()&&s.scene.materials[candidate].id==primitive.material_symbol){++count;index=candidate;}if(count!=1)throw std::runtime_error("Required source mesh material-symbol binding");receiver->materials.push_back(index);material[index]=true;}
  s.receivers.push_back(std::move(receiver));
 }
 for(unsigned i=0;i<material.size();++i)if(material[i])s.defaults(i);
 for(unsigned i=0;i<dh2_bres_library_count(&s.image,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&s.image,i,0)!=assets::Error::ok)throw std::runtime_error("Required authored mesh animation accessor");auto type=dh2_animation_type(&a,0);if(type!=86&&(type<87||type>91))continue;
  unsigned matches=0,index=0;for(unsigned m=0;m<s.scene.materials.size();++m)if(material[m]&&s.scene.materials[m].id==dh2_animation_target(&a)){++matches;index=m;}if(!matches)continue;if(matches!=1||(type==86&&(dh2_animation_channels(&a)!=1||dh2_animation_samplers(&a)!=1)))throw std::runtime_error("Required source mesh animation binding");
  const auto* channel=dh2_animation_channel(&a,0);unsigned property;std::memcpy(&property,channel+12,4);auto name=s.text(property);
  Impl::Track track{a,{},index,type};if(type==86){if(name!="diffuse-color"||!dh2_animation_vector(&a,0,true,&track.values)||track.values.type!=1||(track.values.components!=4&&track.values.components!=1)||dh2_animation_animator(&a)||dh2_animation_offsets(&a))throw std::runtime_error("Required authored mesh uchar4 color track");}
  if(type==86&&track.values.components==1&&(!dh2_animation_has_default(&a)||!dh2_animation_default(&a)))throw std::runtime_error("Required source uchar1 color default");
  else if(type!=86&&name!="diffuse-sampler-matrix")throw std::runtime_error("Required source mesh texture matrix track");s.tracks.push_back(track);
 }
 return true;
}catch(const std::exception& e){error=e.what();return false;}}
bool AuthoredFxMeshGraphV32::sample(std::int32_t ms,std::string& error){try{auto& s=*impl_;for(const auto& track:s.tracks){if(track.type==86){int key=0;float fraction=0;const bool between=dh2_animation_find(&track.animation,0,ms,&key,&fraction);if(key<0||unsigned(key)>=track.values.count||(between&&unsigned(key)+1>=track.values.count))throw std::runtime_error("Required authored mesh color key");std::uint8_t rgba[4];
  if(track.values.components==1){animation::ColorAccessor24 accessor{track.values.data,track.values.count,1,dh2_animation_default(&track.animation)};const int status=between?dh2_material_alpha_between(rgba,&accessor,key,key+1,fraction):dh2_material_alpha_key(rgba,&accessor,key);if(status)throw std::runtime_error("Required source mesh alpha interpretation");}
  else if(between){animation::MaterialColorAccessorV3 accessor{track.values.data,track.values.count};if(dh2_material_color_between_v3(rgba,&accessor,key,key+1,fraction))throw std::runtime_error("Required source mesh color interpolation");}else std::memcpy(rgba,track.values.data+4*key,4);
  animation::ColorParameter32 parameter{8,1,{0,0,0,0},-1,-1};if(dh2_material_color_set(&parameter,0,rgba)!=1)throw std::runtime_error("Required source mesh color application");std::memcpy(s.scene.materials[track.material].color,parameter.words,16);
 }else{TextureTransform20V1 value;if(!texture_sample_v1(value,track.animation,ms,true,error))return false;if(dh2_fx_texture_matrix_v1(&s.matrices[track.material],&value))throw std::runtime_error("Required source mesh texture matrix application");}}
 return true;
}catch(const std::exception& e){error=e.what();return false;}}
bool AuthoredFxMeshGraphV32::draw_sources(const math::Matrix4f& outer,std::vector<CharacterFxMeshDrawSourceV4>& out,std::string& error)const{try{auto s=impl_;std::vector<CharacterFxMeshDrawSourceV4> result;
 for(const auto& receiver:s->receivers){math::Matrix4f local{},world{};if(!source_fx_node_world_matrix_v4(s->scene,receiver->node,local,error))return false;source_fx_matrix_multiply_v4(world,outer,local);std::vector<std::array<float,3>> deformed;if(!receiver->skin.nodes.empty()){std::vector<skinning::Matrix> palette;if(!skinning::palette(receiver->skin,s->scene,palette,error)||!skinning::positions(receiver->skin,palette,receiver->geometry.positions,deformed,error))return false;world=outer;}
  for(unsigned i=0;i<receiver->geometry.primitives.size();++i){auto snapshot=std::make_shared<Impl::Snapshot>();snapshot->owner=s;snapshot->geometry=receiver->geometry;if(!deformed.empty()){snapshot->geometry.positions=deformed;for(const auto& primitive:snapshot->geometry.primitives){const auto index=primitive.attributes[0];if(index<0||std::size_t(index)>=snapshot->geometry.attributes.size())throw std::runtime_error("Required source skinned position stream");auto& stream=snapshot->geometry.attributes[index];if(stream.components!=3||stream.values.size()!=deformed.size()*3)throw std::runtime_error("Required source skin vertex count");for(unsigned v=0;v<deformed.size();++v)std::copy_n(deformed[v].data(),3,stream.values.data()+v*3);}}snapshot->materials=s->scene.materials;snapshot->bindings=receiver->materials;
   const auto material=receiver->materials.at(i);const auto uv=snapshot->geometry.primitives[i].attributes[4];if(uv<0||std::size_t(uv)>=snapshot->geometry.attributes.size())throw std::runtime_error("Required actual source mesh UV stream");auto& values=snapshot->geometry.attributes[uv];if(values.components!=2)throw std::runtime_error("Required actual source mesh UV2 layout");const auto& matrix=s->matrices[material];
   for(unsigned v=0;v<values.values.size();v+=2){const float u=values.values[v],t=values.values[v+1];values.values[v]=matrix.m[0]*u+matrix.m[4]*t+matrix.m[8];values.values[v+1]=matrix.m[1]*u+matrix.m[5]*t+matrix.m[9];}
   CharacterFxMeshDrawSourceV4 source;source.resource_bytes=s->bytes;source.image=&s->image;source.scene=&s->scene;source.node_identity=reinterpret_cast<std::uintptr_t>(receiver.get());source.node=receiver->node;source.material=material;source.primitive=i;source.source_part=i+1;source.source_texture_matrix68=&s->matrices[material];
   source.part.retention=snapshot;source.part.geometry=&snapshot->geometry;source.part.material_table=&snapshot->materials;source.part.materials=&snapshot->bindings;source.part.positions=snapshot->geometry.positions;std::copy_n(world.m,16,source.part.world.data());result.push_back(std::move(source));
  }
 }
 out=std::move(result);return true;
}catch(const std::exception& e){error=e.what();return false;}}
}

