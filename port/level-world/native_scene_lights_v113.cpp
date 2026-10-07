#include "native_scene_lights_v113.hpp"
#include "retained_visual_child_v91.hpp"
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh2::world {namespace {
bool required(std::string& e,const char* body){e=body;return false;}
bool valid(const NativeLightNodeV113& n){return n.owner&&n.identity&&n.light&&n.position&&n.set_position;}
}
NativeLightV113::NativeLightV113(){std::uint32_t maximum=0x7f7fffff,cutoff=0x42340000;std::memcpy(&radius40,&maximum,4);std::memcpy(&cutoff48,&cutoff,4);}
bool NativeLightV113::grab(std::string& e){if(native_destroyed_v113||references0==UINT32_MAX)return required(e,"Actual native CLight reference unavailable");++references0;e.clear();return true;}
NativeMaterialLightsV113::~NativeMaterialLightsV113(){for(auto& light:lights)if(light)light->drop();}
bool NativeMaterialLightsV113::assign(unsigned index,std::shared_ptr<NativeLightV113> light,std::string& e){if(index>=lights.size())return required(e,"Actual material light parameter range");if(light&&!light->grab(e))return false;auto previous=std::move(lights[index]);lights[index]=std::move(light);source_assignment_v113[index]=1;if(previous)previous->drop();e.clear();return true;}
void NativeLightV113::drop()noexcept{if(!references0||native_destroyed_v113)std::terminate();if(--references0==0){native_destroyed_v113=true;transformation50={};}}
NativeSceneLightNodeV113::NativeSceneLightNodeV113():light134_(std::make_shared<NativeLightV113>()),world_{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}{std::string e;if(!light134_->grab(e))throw std::runtime_error(e);}
bool NativeSceneLightNodeV113::construct(std::shared_ptr<NativeSceneLightNodeV113>& out,std::string& e){
 auto node=std::make_shared<NativeSceneLightNodeV113>();const std::weak_ptr<NativeSceneLightNodeV113> weak=node;
 node->light134_->transformation50=[weak](auto& m,auto& e){auto actual=weak.lock();return actual?actual->matrix(m,e):required(e,"Retired actual CLight50 transformation owner");};
 if(!node->recalc(e))return false;out=std::move(node);e.clear();return true;
}
bool NativeSceneLightNodeV113::grab(std::string& e){if(destroyed_||references158_==UINT32_MAX)return required(e,"Actual light-node native grab unavailable");++references158_;e.clear();return true;}
void NativeSceneLightNodeV113::drop()noexcept{if(!references158_||destroyed_)std::terminate();if(--references158_==0){destroyed_=true;if(light134_)light134_->drop();light134_.reset();}}
bool NativeSceneLightNodeV113::set_position(const std::array<float,3>& value,std::string& e){if(destroyed_)return required(e,"Destroyed actual CLightSceneNode");for(float v:value)if(!std::isfinite(v))return required(e,"Nonfinite actual light-node position");positionac_=value;dirty_=true;e.clear();return true;}
bool NativeSceneLightNodeV113::set_authored_matrix_v113(const std::array<float,16>& value,std::string& e){if(destroyed_)return required(e,"Destroyed actual imported light transform");for(float v:value)if(!std::isfinite(v))return required(e,"Nonfinite actual imported light transform");world_=value;positionac_={value[12],value[13],value[14]};dirty_=false;e.clear();return true;}
bool NativeSceneLightNodeV113::bind_source_matrix_v113(std::function<bool(std::array<float,16>&,std::string&)> source,std::string& e){if(destroyed_||!light134_||!source)return required(e,"Actual authored light matrix source absent");light134_->transformation50=std::move(source);e.clear();return true;}
bool NativeSceneLightNodeV113::scene_phase(std::string& e){if(destroyed_)return required(e,"Destroyed actual light Scene phase");if(dirty_){for(unsigned i=0;i<3;++i)world_[12+i]=positionac_[i];dirty_=false;}e.clear();return true;}
bool NativeSceneLightNodeV113::position(std::array<float,3>& out,std::string& e){if(!scene_phase(e))return false;out={world_[12],world_[13],world_[14]};return true;}
bool NativeSceneLightNodeV113::matrix(std::array<float,16>& out,std::string& e){if(!scene_phase(e))return false;out=world_;return true;}
bool NativeSceneLightNodeV113::recalc(std::string& e){
 if(destroyed_||!light134_||light134_->native_destroyed_v113)return required(e,"Actual doLightRecalc owner missing");const auto type=light134_->type58;
 if(type<=1){if(light134_->radius40==std::numeric_limits<float>::max())culling118_=0;
  else{const float extent=(light134_->radius40*light134_->radius40)*.5f;box13c_={-extent,-extent,-extent,extent,extent,extent};culling118_=1;}}
 else if(type==2){box13c_.fill(0);culling118_=0;}type138_=type;e.clear();return true;
}
bool NativeSceneLightNodeV113::set_type(std::uint16_t type,std::string& e){if(destroyed_||!light134_)return required(e,"Actual CLight58 type store owner missing");light134_->type58=type;return recalc(e);}
bool construct_retained_scene_lights_v113(const resources::BresView& image,const scene::Scene& graph,
 const std::vector<std::shared_ptr<RetainedVisualNodeV91>>& nodes,
 const std::vector<std::shared_ptr<RetainedMeshNodeV91>>& meshes,std::string& e){
 using Entry=std::pair<std::string,std::weak_ptr<NativeSceneLightNodeV113>>;
 auto list=std::make_shared<std::vector<Entry>>();list->reserve(graph.lights_v113.size());
 for(const auto& instance:graph.lights_v113){
  if(instance.node_index>=nodes.size()||!nodes[instance.node_index]||!nodes[instance.node_index]->fields)return required(e,"Actual authored light parent node absent");
  auto parent=nodes[instance.node_index];const auto row=dh2_bres_library_item(&image,resources::Library::light,instance.light);if(!row||!image.bytes)return required(e,"Actual Root light188 source row absent");
  std::uint32_t id;std::memcpy(&id,row,4);if(!id||id>=image.size)return required(e,"Actual SLight ID outside BRES");const auto end=static_cast<const std::uint8_t*>(std::memchr(image.bytes+id,0,image.size-id));if(!end)return required(e,"Actual SLight ID unterminated");
  const std::string name(reinterpret_cast<const char*>(image.bytes+id),reinterpret_cast<const char*>(end));
  std::shared_ptr<NativeSceneLightNodeV113> light;if(!NativeSceneLightNodeV113::construct(light,e)||!light->load_light(image,instance.light,e))return false;
  //The same authored CSceneNode world-cache owns this child transformation.
  //CLight's reference pins the matrix cell independently during native D1;
  //dynamic compiled owners retain and update that SAME graph through V112.
  auto fields=parent->fields;if(!light->bind_source_matrix_v113([fields](auto& out,std::string& e){out=fields->world;e.clear();return true;},e))return false;
  light->parent()=reinterpret_cast<std::uintptr_t>(parent.get());parent->lights_v113.push_back(light);
  list->emplace_back(name,light); //Root188 ordered lookup aliases the SAME native child
 }
 for(const auto& mesh:meshes){if(!mesh)return required(e,"Actual material mesh receiver absent");
  mesh->local_light_v113=[list](const std::string& name,std::shared_ptr<NativeLightV113>& out,std::string& e){out.reset();for(const auto& entry:*list)if(entry.first==name){auto node=entry.second.lock();if(!node||!node->live()){e="Actual Root.light188 receiver retired";return false;}out=node->light();break;}e.clear();return true;};
 }
 e.clear();return true;
}
bool NativeSceneLightNodeV113::load_light(const resources::BresView& image,std::uint32_t index,std::string& e)try{
 if(destroyed_||!light134_)return required(e,"Actual imported light receiver missing");const auto* row=dh2_bres_library_item(&image,resources::Library::light,index);if(!row||row<image.bytes||std::size_t(row-image.bytes)>image.size||image.size-std::size_t(row-image.bytes)<24)return required(e,"Actual SLight library row absent");
 auto word=[](const std::uint8_t* b){return std::uint32_t(b[0])|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);};
 auto scalar=[&](std::uint32_t offset){if(offset>image.size||image.size-offset<4)throw std::runtime_error("Actual SLight component outside BRES");const auto bits=word(image.bytes+offset);float v;std::memcpy(&v,&bits,4);if(!std::isfinite(v))throw std::runtime_error("Nonfinite actual authored light component");return v;};
 const auto type=word(row+8),data=word(row+20);float power;const auto raw=word(row+16);std::memcpy(&power,&raw,4);power/=255.f;std::array<float,4> color;for(unsigned i=0;i<4;++i)color[i]=float(row[12+i])*power;
 auto& light=*light134_;light.specular24=color;
 if(type==0){light.type58=3;light.ambient4=color;light.diffuse14.fill(0);light.specular24.fill(0);}
 else if(type==3){light.type58=2;light.diffuse14=color;}
 else if(type==1||type==2){light.type58=type==1?0:1;light.diffuse14=color;for(unsigned i=0;i<3;++i)light.attenuation34[i]=scalar(data+4*i);if(type==2){light.cutoff48=scalar(data+12);light.exponent4c=scalar(data+16);}}
 //Original out-of-range type keeps reached C1 fields and still recalculates.
 return recalc(e);
}catch(const std::exception& failure){e=failure.what();return false;}
NativeLightNodeV113 NativeSceneLightNodeV113::borrow(){auto self=shared_from_this();const std::weak_ptr<NativeSceneLightNodeV113> weak=self;NativeLightNodeV113 out;out.owner=self;out.identity=identity();out.light=light134_;out.position=[weak](auto& value,auto& e){auto n=weak.lock();return n?n->position(value,e):required(e,"Actual light position owner expired");};out.set_position=[weak](const auto& value,auto& e){auto n=weak.lock();return n?n->set_position(value,e):required(e,"Actual light setter owner expired");};return out;}
void NativeLightSetV113::initialize_filter(std::vector<bool>& filter,bool enabled){
 //40cff0 resizes the SAME bit vector to five, preserving its allocation,
 //then writes all five actual source values from the supplied bool.
 filter.resize(5,false);for(std::size_t i=0;i<5;++i)filter[i]=enabled;
}
bool NativeLightSetV113::set_light(std::int32_t set,std::int32_t index,std::shared_ptr<NativeLightV113> light,std::string& e){
 if(set<0||set>=4||index<0||index>=5)return required(e,"Original LightSet.Get/SetLight range assertion");
 if(lights78_[set][index]!=light)changed64_[set][index]=1;if(light&&!light->grab(e))return false;auto previous=std::move(lights78_[set][index]);lights78_[set][index]=std::move(light);if(previous)previous->drop();e.clear();return true;
}
bool NativeLightSetV113::get_light(std::int32_t set,std::int32_t index,std::shared_ptr<NativeLightV113>& out,std::string& e)const{
 if(set<0||set>=4||index<0||index>=5)return required(e,"Original LightSet.GetLight range assertion");out=lights78_[set][index];e.clear();return true;
}
bool NativeLightSetV113::set_dummy_off(std::int32_t index,std::shared_ptr<NativeLightV113> light,std::string& e){
 if(index<0||index>=5)return required(e,"Original dummy-off slot outside native source array");if(light&&!light->grab(e))return false;auto previous=std::move(offc8_[index]);offc8_[index]=std::move(light);if(previous)previous->drop();e.clear();return true;
}
bool NativeLightSetV113::select(std::int32_t set,const std::vector<bool>& filter,std::array<std::shared_ptr<NativeLightV113>,4>& out,std::string& e)const{
 if(!names_||set<0||set>=4||filter.size()<4)return required(e,"Actual material LightSet/filter owner absent");
 //40c9a8 first compacts enabled source slots, then appends corresponding
 //disabled-slot dummy fields. Actual NULL light remains NULL for the renderer.
 std::size_t target=0;for(unsigned source=0;source<4;++source)if(filter[source])out[target++]=lights78_[set][source];
 for(unsigned source=0;source<4;++source)if(!filter[source])out[target++]=offc8_[source];e.clear();return true;
}
bool NativeLightSetV113::add_static(NativeLightNodeV113 node,std::string& e){
 if(!valid(node)||static_dc_.size()>6)return required(e,"Original AddStaticLight6b assertion or missing actual node");static_dc_.push_back({std::move(node)});e.clear();return true;
}
bool NativeLightSetV113::add_active(target_providers::Handle16 handle,NativeLightNodeV113 node,std::string& e){
 if(!valid(node)||active11c_.size()>2)return required(e,"Original AddActiveLight61 assertion or missing actual node");active11c_.push_back({std::move(node),handle});e.clear();return true;
}
bool NativeLightSetV113::closest_static(const std::array<float,3>& point,NativeLightNodeV113& out,std::string& e)const{
 out={};if(static_dc_.empty())return required(e,"Original GetClosestStaticLight44 empty-list assertion");std::uint32_t bits=0x4e6e6b28;float closest;std::memcpy(&closest,&bits,4);std::size_t selected=SIZE_MAX;
 for(std::size_t i=0;i<static_dc_.size();++i){std::array<float,3> p;if(!static_dc_[i].node.position(p,e))return false;const float dx=p[0]-point[0],dy=p[1]-point[1],dz=p[2]-point[2];const float length=static_cast<float>(std::sqrt(static_cast<double>((dx*dx+dy*dy)+dz*dz)));if(closest>length){closest=length;selected=i;}}
 if(selected==SIZE_MAX)return required(e,"Original closest-static sentinel had no selected finite light");out=static_dc_[selected].node;e.clear();return true;
}
bool NativeLightSetV113::update(const NativeLightUpdateServicesV113& s,std::string& e){
 //Genuine C1 empty collections return BEFORE Debug or ObjectHandle services.
 if(static_dc_.empty()||active11c_.empty()){e.clear();return true;}
 if(!s.owner||!s.debug||!s.resolve_position)return required(e,"Actual LightSet.Update Debug/handle transport required");bool enabled{};
 if(!s.debug("EnablePlayerHeadLight",enabled,e))return false;if(enabled){e.clear();return true;}
 for(auto& active:active11c_){bool found{};std::array<float,3> actor;if(!s.resolve_position(active.handle,found,actor,e))return false;if(!found)continue;
  NativeLightNodeV113 selected;if(!closest_static(actor,selected,e))return false;std::array<float,3> p;if(!selected.position(p,e))return false;
  for(auto& value:p)value+=20.f;if(!active.node.set_position(p,e))return false;
 }
 e.clear();return true;
}
void NativeLightSetV113::reset_light_sets()noexcept{
 for(auto& set:lights78_)for(auto& light:set){if(light)light->drop();light.reset();}for(auto& flags:changed64_)flags.fill(0);for(auto& light:offc8_){if(light)light->drop();light.reset();}
}
void NativeLightSetV113::source_assign_default_v113(){reset_light_sets();static_dc_.clear();active11c_.clear();LightSetNameOwnerV3 original;names_->source_names()=original.source_names();}
}
