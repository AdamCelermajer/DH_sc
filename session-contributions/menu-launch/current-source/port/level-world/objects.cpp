#include "objects.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <set>
#include <stdexcept>
namespace dh2::objects {
namespace {
unsigned word(const std::uint8_t* p){return p[0]|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);}
float number(const std::uint8_t* p,float limit){auto bits=word(p);float v;std::memcpy(&v,&bits,4);if(!std::isfinite(v)||std::abs(v)>limit)throw std::runtime_error("Object component exceeds limit");return v;}
std::string text(const std::uint8_t* p,bool empty=false){const auto* end=static_cast<const std::uint8_t*>(std::memchr(p,0,64));if(!end||(!empty&&end==p))throw std::runtime_error("Object identifier rejected");for(auto* i=p;i<end;++i)if(*i<32||*i>126)throw std::runtime_error("Object identifier encoding rejected");for(auto* i=end;i<p+64;++i)if(*i)throw std::runtime_error("Object identifier padding rejected");return {reinterpret_cast<const char*>(p),std::size_t(end-p)};}
std::string basename(std::string path){auto i=path.find_last_of("/\\");return i==std::string::npos?path:path.substr(i+1);}
std::array<float,16> placement(const Record& r){
 // XYZ Euler degrees are an explicit XML adapter policy. Serialized model
 // node matrices still use the recovered original quaternion arithmetic.
 const float c=3.14159265358979323846f/360.f;
 const float x=r.rotation_degrees[0]*c,y=r.rotation_degrees[1]*c,z=r.rotation_degrees[2]*c;
 const float sx=std::sin(x),cx=std::cos(x),sy=std::sin(y),cy=std::cos(y),sz=std::sin(z),cz=std::cos(z);
 const float q[]{sx*cy*cz-cx*sy*sz,cx*sy*cz+sx*cy*sz,cx*cy*sz-sx*sy*cz,cx*cy*cz+sx*sy*sz};
 std::array<float,16> m;dh2_node_matrix(m.data(),r.position.data(),q,r.scale.data());return m;
}
}
bool load_records(const std::uint8_t* input,std::size_t size,unsigned rooms,const data::CharacterTable& table,const data::Dictionary& models,std::vector<Record>& out,std::string& error){
 out.clear();error.clear();try{
  if(!input||size<16||std::memcmp(input,"DACT",4)||word(input+4)!=1||word(input+12)||!rooms||rooms>512)throw std::runtime_error("Object descriptor rejected");
  unsigned count=word(input+8);if(!count||count>10000||size!=16+std::uint64_t(count)*256)throw std::runtime_error("Object record count rejected");
  std::vector<Record> candidate;std::set<std::pair<unsigned,std::string>> names;
  for(unsigned i=0;i<count;++i){const auto* p=input+16+i*256;Record r;r.kind=word(p);r.room=word(p+4);r.name=text(p+8);r.character=text(p+72,true);r.model=text(p+136);
   if((r.kind!=1&&r.kind!=2)||r.room>=rooms||basename(r.model)!=r.model||r.model.find("..")!=std::string::npos||!names.insert({r.room,r.name}).second)throw std::runtime_error("Object type, room or resource rejected");
   for(unsigned j=0;j<3;++j){r.position[j]=number(p+200+j*4,10000000);r.rotation_degrees[j]=number(p+212+j*4,3600);r.scale[j]=number(p+224+j*4,100);if(r.scale[j]<=0)throw std::runtime_error("Object scale rejected");}
   for(unsigned j=236;j<256;++j)if(p[j])throw std::runtime_error("Object reserved bytes rejected");
   if(r.kind==1){auto* id=data::property(table,r.character,"ModelFile");if(!id||*id<0||unsigned(*id)>=models.values.size()||basename(models.values[*id])!=r.model)throw std::runtime_error("Character model differs from original table");}
   else if(!r.character.empty())throw std::runtime_error("Decor contains character link");
   r.placement=placement(r);for(float f:r.placement)if(!std::isfinite(f))throw std::runtime_error("Object placement overflow");candidate.push_back(std::move(r));
  }out=std::move(candidate);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
std::string idle_clip(const std::string& model){
 if(model=="skeleton.bdae")return "skeleton_idle_01.bdae";
 if(model=="slime_green_v2.bdae")return "slime_idle.bdae";
 if(model=="ghost.bdae")return "ghost_idle_01.bdae";
 return {};
}
bool sample(Resource& r,std::int32_t ms,std::string& error){
 return sample(r,r.animation,ms,error);
}
bool sample(Resource& r,const animation::Player& clip,std::int32_t ms,std::string& error){
 if(!clip.sample(r.scene,ms,error))return false;
 for(auto& p:r.primitives)if(!p.skin.nodes.empty()){
  std::vector<skinning::Matrix> palette;std::vector<world::Point> output;
  if(!skinning::palette(p.skin,r.scene,palette,error)||!skinning::positions(p.skin,palette,p.rest_positions,output,error))return false;
  for(unsigned i=0;i<output.size();++i){
   // Native rendering policy for rigid vertices in partly skinned decors.
   // The recovered software kernel returns zero for an empty influence;
   // original hardware shader parity has not yet been established.
   if(p.skin.influences[i].weights[0]==0){const auto& m=r.scene.graph[p.node].world;for(unsigned row=0;row<3;++row){output[i][row]=m[12+row];for(unsigned col=0;col<3;++col)output[i][row]+=m[col*4+row]*p.rest_positions[i][col];}}
   for(float f:output[i])if(!std::isfinite(f)){error="Rigid object position overflow";return false;}
   std::copy(output[i].begin(),output[i].end(),p.vertices[i].p);
  }
 }return true;
}
static bool load_resource_impl(const std::uint8_t* input,std::size_t size,const std::vector<std::string>* modules,const std::uint8_t* clip,std::size_t clip_size,Resource& out,std::string& error){
 out={};error.clear();try{
  resources::BresView view{};if(dh2_bres_open(&view,input,size)!=resources::BresError::ok)throw std::runtime_error("Object BRES rejected");
  Resource candidate;if(!scene::load(view,candidate.scene,error))throw std::runtime_error(error);
  if(modules){
   if(modules->empty()||modules->size()>32)throw std::runtime_error("Modular controller selection empty or oversized");
   const auto root=std::find_if(candidate.scene.graph.begin(),candidate.scene.graph.end(),[](const scene::Node& n){return n.id=="prince_modular-node";});
   if(root==candidate.scene.graph.end())throw std::runtime_error("Original prince modular root absent");
   std::set<std::string> selected;candidate.scene.instances.clear();
   for(const auto& id:*modules){
    if(!selected.insert(id).second)throw std::runtime_error("Duplicate modular controller");
    unsigned found=0;
    for(unsigned i=0;i<dh2_bres_library_count(&view,resources::Library::controller);++i){
     skinning::Skin skin;if(!skinning::load(view,i,candidate.scene,skin,error))throw std::runtime_error(error);
     if(skin.id!=id)continue;++found;
     assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,skin.geometry)!=assets::Error::ok)throw std::runtime_error("Modular geometry rejected");
     scene::Instance instance{root->id,unsigned(root-candidate.scene.graph.begin()),skin.geometry,root->world,{}};instance.controller=i;
     for(unsigned j=0;j<mesh.primitives;++j){assets::Primitive p{};if(dh2_mesh_primitive(&mesh,j,&p)!=assets::Error::ok)throw std::runtime_error("Modular primitive rejected");
      const auto material=std::find_if(candidate.scene.materials.begin(),candidate.scene.materials.end(),[&](const scene::Material& m){return m.id==p.material;});
      if(material==candidate.scene.materials.end())throw std::runtime_error("Modular material unresolved");
      instance.materials.push_back(unsigned(material-candidate.scene.materials.begin()));
     }
     candidate.scene.instances.push_back(std::move(instance));
    }
    if(found!=1)throw std::runtime_error("Original modular controller absent or ambiguous: "+id);
   }
  }
  auto& instances=candidate.scene.instances;const auto before=instances.size();
  instances.erase(std::remove_if(instances.begin(),instances.end(),[](const scene::Instance& i){return i.node.find("_colbox_")!=std::string::npos||i.node.find("_mesh_shadow_")!=std::string::npos;}),instances.end());candidate.removed_helpers=before-instances.size();
  unsigned total=0;
  for(const auto& instance:instances){assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,instance.geometry)!=assets::Error::ok||mesh.primitives!=instance.materials.size())throw std::runtime_error("Object mesh/binding rejected");
   for(unsigned j=0;j<mesh.primitives;++j){assets::Primitive raw{};dh2_mesh_primitive(&mesh,j,&raw);
    if(raw.collada_type||raw.index_count%3||raw.index_count>3000000||mesh.vertices>65536||total>1000000-mesh.vertices)throw std::runtime_error("Object primitive budget rejected");
    total+=mesh.vertices;
    Primitive p;p.node=instance.node_index;p.material=instance.materials[j];if(candidate.scene.materials.at(p.material).id!=raw.material)throw std::runtime_error("Object material binding differs");
    if(instance.controller>=0&&!skinning::load(view,instance.controller,candidate.scene,p.skin,error))throw std::runtime_error(error);
    assets::Attribute positions{},uv{},color{};if(dh2_mesh_attribute(&mesh,raw.attributes[0],&positions)!=assets::Error::ok||positions.components<3)throw std::runtime_error("Object position missing");
    const bool have_uv=dh2_mesh_attribute(&mesh,raw.attributes[4],&uv)==assets::Error::ok&&uv.components>=2;
    const bool have_color=dh2_mesh_attribute(&mesh,raw.attributes[2],&color)==assets::Error::ok;
    p.vertices.resize(mesh.vertices);
    for(unsigned k=0;k<mesh.vertices;++k){float value[4]{};dh2_attribute_read(&positions,k,value);std::copy(value,value+3,p.vertices[k].p);
     if(have_uv){dh2_attribute_read(&uv,k,value);std::copy(value,value+2,p.vertices[k].uv);}
     if(have_color){dh2_attribute_read(&color,k,value);for(unsigned c=0;c<color.components;++c)p.vertices[k].color[c]=value[c]/(color.type==1?255.f:1.f);}
     for(float f:p.vertices[k].p)if(!std::isfinite(f))throw std::runtime_error("Nonfinite object position");
     for(float f:p.vertices[k].uv)if(!std::isfinite(f))throw std::runtime_error("Nonfinite object UV");
     for(float f:p.vertices[k].color)if(!std::isfinite(f))throw std::runtime_error("Nonfinite object color");
    }
    if(!p.skin.nodes.empty()){p.rest_positions.resize(mesh.vertices);for(unsigned k=0;k<mesh.vertices;++k)std::copy(p.vertices[k].p,p.vertices[k].p+3,p.rest_positions[k].begin());}
    p.indices.resize(raw.index_count);for(unsigned k=0;k<raw.index_count;++k){unsigned index;dh2_index_read(&raw,k,&index);p.indices[k]=index;}candidate.triangles+=raw.index_count/3;candidate.primitives.push_back(std::move(p));
   }
  }
  if(candidate.primitives.empty())throw std::runtime_error("Object has no visible primitives");
  candidate.rest_scene=candidate.scene;
  if(!candidate.animation.load(clip?clip:input,clip?clip_size:size,candidate.scene,error,animation::MissingTargets::ignore)||!sample(candidate,candidate.animation.start,error))throw std::runtime_error(error);
  out=std::move(candidate);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool load_resource(const std::uint8_t* input,std::size_t size,const std::uint8_t* clip,std::size_t clip_size,Resource& out,std::string& error){
 return load_resource_impl(input,size,nullptr,clip,clip_size,out,error);
}
bool load_modular_resource(const std::uint8_t* input,std::size_t size,const std::vector<std::string>& modules,const std::uint8_t* clip,std::size_t clip_size,Resource& out,std::string& error){
 return load_resource_impl(input,size,&modules,clip,clip_size,out,error);
}
}
