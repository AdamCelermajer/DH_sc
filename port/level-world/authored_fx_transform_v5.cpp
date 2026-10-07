#include "authored_fx_transform_v5.hpp"
#include "../engine-animation/animation.hpp"
#include "../engine-animation/angle_interpreter.hpp"
#include "../engine-animation/component_applicator.hpp"
#include "source_fx_node_matrix_v4.hpp"
#include "source_fx_segment_v87.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
#include <stdexcept>
namespace dh2::fx {
struct AuthoredFxTransformOwnerV5::Impl {
 struct Track{assets::Animation a{};assets::Vector values{};std::uint32_t node{},type{},segment{};};
 resources::BresView image;scene::Scene& scene;std::vector<Track> tracks;std::vector<std::uint32_t> dirty;
 Impl(const resources::BresView& i,scene::Scene& s):image(i),scene(s),dirty(s.graph.size()){}
};
AuthoredFxTransformOwnerV5::AuthoredFxTransformOwnerV5(const resources::BresView& i,scene::Scene& s):impl_(std::make_unique<Impl>(i,s)){}
AuthoredFxTransformOwnerV5::~AuthoredFxTransformOwnerV5()=default;
bool AuthoredFxTransformOwnerV5::initialize(std::string& error){try{auto& s=*impl_;
 const auto segments=dh2_animation_segments(&s.image);if(dh2_bres_library_count(&s.image,resources::Library::animation)&&(!segments||segments>256))throw std::runtime_error("FX source transform segment domain");
 for(unsigned segment=0;segment<segments;++segment)for(unsigned i=0;i<dh2_bres_library_count(&s.image,resources::Library::animation);++i){Impl::Track t;t.segment=segment;
  if(dh2_animation_open(&t.a,&s.image,i,segment)!=assets::Error::ok)throw std::runtime_error("Required FX source transform accessor");t.type=dh2_animation_type(&t.a,0);if(t.type<1||t.type>13)continue;
  const auto* target=dh2_animation_target(&t.a);unsigned found=0;for(unsigned n=0;n<s.scene.graph.size();++n)if(target&&s.scene.graph[n].id==target){t.node=n;++found;}
  if(found!=1)throw std::runtime_error("Required exact FX transform target");
  const bool component=(t.type>=2&&t.type<=4)||(t.type>=11&&t.type<=13),angle=t.type>=6&&t.type<=9;
  const unsigned width=t.type==5?4:component||angle?1:3;
  if(dh2_animation_channels(&t.a)!=1||dh2_animation_samplers(&t.a)!=1||dh2_animation_animator(&t.a)||dh2_animation_offsets(&t.a)||dh2_animation_scales(&t.a)||!dh2_animation_vector(&t.a,0,true,&t.values)||t.values.type!=6||t.values.components!=width||!t.values.count)
   throw std::runtime_error("Required FX source transform layout");
  const auto* defaults=dh2_animation_default(&t.a);if((component||angle)&&(!defaults||defaults<s.image.bytes||std::size_t(defaults-s.image.bytes)>s.image.size||(angle?16u:12u)>s.image.size-std::size_t(defaults-s.image.bytes)))throw std::runtime_error("Required FX source component/angle default");
  for(unsigned k=1;k<t.values.count;++k)if(dh2_animation_key_time(&t.a,0,k)<dh2_animation_key_time(&t.a,0,k-1))throw std::runtime_error("Required ordered FX transform times");s.tracks.push_back(t);
 }return true;
 }catch(const std::exception& e){error=e.what();return false;}}
bool AuthoredFxTransformOwnerV5::sample(std::int32_t ms,std::string& error){try{auto& s=*impl_;if(s.tracks.empty())return source_fx_rebuild_graph_world_v4(s.scene,error);std::uint32_t segment{};if(!source_fx_segment_v87(s.image,ms,segment,error))return false;for(const auto& t:s.tracks){if(t.segment!=segment)continue;int key=0;float fraction=0;const bool between=dh2_animation_find(&t.a,0,ms,&key,&fraction);if(key<0||unsigned(key)>=t.values.count||(between&&unsigned(key)+1>=t.values.count))throw std::runtime_error("Required FX transform key");
  float first[4]{},second[4]{},value[4]{};if(!dh2_vector_read(&t.values,key,first)||(between&&!dh2_vector_read(&t.values,key+1,second)))throw std::runtime_error("Required FX transform values");auto& node=s.scene.graph[t.node];
  if(t.type==1||t.type==10){if(between)dh2_animation_lerp3(value,first,second,fraction);else std::copy_n(first,3,value);std::copy_n(value,3,t.type==1?node.translation:node.scale);}
  else if(t.type==5){if(between)dh2_animation_quaternion(value,first,second,fraction);else std::copy_n(first,4,value);std::copy_n(value,4,node.quaternion);}
  else if((t.type>=2&&t.type<=4)||(t.type>=11&&t.type<=13)){
   std::memcpy(value,dh2_animation_default(&t.a),12);float scalar=first[0];if(between){volatile float delta=second[0]-first[0];volatile float product=fraction*delta;volatile float sum=first[0]+product;scalar=sum;}value[t.type<=4?t.type-2:t.type-11]=scalar;
   if(animation::apply_component(node,s.dirty[t.node],t.type,value))throw std::runtime_error("Required source FX component setter");
  }else{float axis[4];std::memcpy(axis,dh2_animation_default(&t.a),16);const float values[]{first[0],second[0]};animation::AngleAccessor24 accessor{values,axis,2,0};math::Quaternion q;
   const int status=between?dh2_animation_angle_between(&q,&accessor,0,1,fraction):dh2_animation_angle_key(&q,&accessor,0);if(status)throw std::runtime_error("Required source FX angle interpreter");std::memcpy(node.quaternion,&q,16);
  }
 }return source_fx_rebuild_graph_world_v4(s.scene,error);
 }catch(const std::exception& e){error=e.what();return false;}}
}
