#include "character_scene.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace {
float literal(std::uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
void empty(float* bounds){for(unsigned k=0;k<3;++k){bounds[k]=std::numeric_limits<float>::max();bounds[k+3]=-std::numeric_limits<float>::max();}}
void point(float* bounds,const float* value){for(unsigned k=0;k<3;++k){if(value[k]>bounds[k+3])bounds[k+3]=value[k];if(value[k]<bounds[k])bounds[k]=value[k];}}
void transform_box(float* box,const dh2::math::Matrix4f& matrix){
 if(matrix.identity_hint)return;
 float endpoints[6];
 for(unsigned e=0;e<2;++e)for(unsigned row=0;row<3;++row){float value=box[e*3]*matrix.m[row];value=value+box[e*3+1]*matrix.m[row+4];value=value+box[e*3+2]*matrix.m[row+8];endpoints[e*3+row]=value+matrix.m[row+12];}
 for(unsigned k=0;k<3;++k){box[k]=endpoints[k];box[k+3]=endpoints[k+3];if(endpoints[k]>endpoints[k+3]){box[k]=endpoints[k+3];box[k+3]=endpoints[k];}}
}
struct Reader {
 const dh2::resources::BresView& view;
 const std::uint8_t* at(std::uint64_t offset,std::uint64_t size)const{if(!view.bytes||offset>view.size||size>view.size-offset)throw std::runtime_error("Character bounds outside BRES");return view.bytes+offset;}
 std::uint32_t word(std::uint64_t offset)const{const auto* b=at(offset,4);return b[0]|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);}
 float scalar(std::uint64_t offset)const{const auto bits=word(offset);float value;std::memcpy(&value,&bits,4);if(!std::isfinite(value))throw std::runtime_error("Invalid character joint bound");return value;}
};
}
extern "C" int dh2_character_visual_scale(float* out,const std::int32_t* base){
 if(!out||!base)return -1;
 float result[3];for(unsigned i=0;i<3;++i)result[i]=static_cast<float>(base[i])*literal(i==2?0x3c23d70a:0x3c1374bc);
 std::copy(result,result+3,out);return 0;
}
extern "C" int dh2_character_owner_bounds(dh2::physical::CharacterOwnerBounds* out,
 const dh2::physical::CharacterOwnerBoundsInput* in){
 if(!out||!in||in->already_scaled>1||in->previous_flat>255)return -1;
 dh2::physical::DecorBodyInput input{};input.owner=out;input.visual_present=1;input.previous_flat=in->previous_flat;
 const float scale=static_cast<float>(in->collision_scale)*literal(0x3c23d70a);
 for(unsigned i=0;i<6;++i)input.mesh_box[i]=in->already_scaled?in->mesh_box[i]:scale*in->mesh_box[i];
 std::copy(in->position,in->position+3,input.position);
 dh2::physical::DecorBodyConfig config{};if(dh2_decor_body_config(&config,&input)!=0)return -1;
 dh2::physical::CharacterOwnerBounds result{};std::copy(config.relative_box,config.relative_box+6,result.relative_box);std::copy(config.absolute_box,config.absolute_box+6,result.absolute_box);result.flat=config.flat;result.update_pf_count=config.mesh_pf_updates;*out=result;return 0;
}
extern "C" int dh2_character_skin_bounds(float* out,const dh2::physical::CharacterSkinBoundsInput* in){
 if(!out||!in||in->joint_count>256)return -1;
 const auto count=in->joint_count&255u;
 if(count&&(!in->joints||(in->box_count&&(!in->joint_boxes||in->box_count<count))))return -1;
 for(unsigned i=0;i<count;++i)if(in->joints[i].identity_hint>1)return -1;
 float result[6];empty(result);
 for(unsigned i=0;i<count;++i){
  if(!in->box_count)point(result,in->joints[i].m+12);
  else{float box[6];std::copy(in->joint_boxes+6*i,in->joint_boxes+6*i+6,box);transform_box(box,in->joints[i]);point(result,box+3);point(result,box);}
 }
 std::copy(result,result+6,out);return 0;
}
extern "C" int dh2_character_mesh_box(dh2::physical::DecorSceneOutput* out,
 const dh2::physical::CharacterMeshBoxInput* in){
 if(!out||!in||in->reserved||in->count>10000||(in->count&&!in->entries))return -1;
 bool skinned=false;for(unsigned i=0;i<in->count;++i){if(in->entries[i].skinned>1)return -1;skinned=skinned||in->entries[i].skinned!=0;}
 auto placement=in->placement;empty(placement.marker_bounds);
 for(unsigned i=0;i<in->count;++i){const auto& entry=in->entries[i];if(bool(entry.skinned)!=skinned)continue;for(unsigned k=0;k<3;++k){const float low=entry.bounds[k]*entry.parent_scale[k],high=entry.bounds[k+3]*entry.parent_scale[k];if(placement.marker_bounds[k]>low)placement.marker_bounds[k]=low;if(placement.marker_bounds[k+3]<high)placement.marker_bounds[k+3]=high;}}
 std::fill(placement.marker_parent_scale,placement.marker_parent_scale+3,1.0f);
 dh2::physical::DecorSceneOutput result{};if(dh2_decor_scene(&result,&placement)!=0)return -1;
 if(!in->count)std::fill(result.mesh_box,result.mesh_box+6,0.0f);
 *out=result;return 0;
}
namespace dh2::physical {
bool character_scene_entries(const resources::BresView& view,const scene::Scene& scene,
 std::vector<CharacterMeshEntry>& out,std::string& error){
 error.clear();try{
  Reader reader{view};std::vector<CharacterMeshEntry> result;std::vector<unsigned> nodes;
  for(const auto& instance:scene.instances){
   if(instance.node_index>=scene.graph.size())throw std::runtime_error("Character instance node out of range");
   const auto& node=scene.graph[instance.node_index];
   float bounds[6];
   if(instance.controller>=0){
    skinning::Skin skin;if(!skinning::load(view,instance.controller,scene,skin,error))return false;
    const auto* record=dh2_bres_library_item(&view,resources::Library::controller,instance.controller);
    if(!record)throw std::runtime_error("Character controller missing");
    const auto data=reader.word(record-view.bytes+8);const auto box_count=reader.word(data+140),boxes=reader.word(data+144);
    if(box_count&&box_count<skin.nodes.size())throw std::runtime_error("Character joint boxes shorter than joint table");
    std::vector<float> joint_boxes;std::vector<math::Matrix4f> joints;
    for(unsigned i=0;i<skin.nodes.size();++i){math::Matrix4f matrix{};std::copy(scene.graph[skin.nodes[i]].world.begin(),scene.graph[skin.nodes[i]].world.end(),matrix.m);matrix.identity_hint=0;joints.push_back(matrix);if(box_count)for(unsigned k=0;k<6;++k)joint_boxes.push_back(reader.scalar(std::uint64_t(boxes)+24*i+4*k));}
    const CharacterSkinBoundsInput input{joints.data(),joint_boxes.data(),static_cast<std::uint32_t>(joints.size()),box_count};
    if(dh2_character_skin_bounds(bounds,&input)!=0)throw std::runtime_error("Character skin bounds rejected");
   }else{assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,instance.geometry)!=assets::Error::ok)throw std::runtime_error("Character mesh bounds rejected");std::copy(mesh.minimum,mesh.minimum+3,bounds);std::copy(mesh.maximum,mesh.maximum+3,bounds+3);}
   const auto found=std::find(nodes.begin(),nodes.end(),instance.node_index);
   if(found!=nodes.end()){
    auto& existing=result[found-nodes.begin()];if(existing.skinned!=std::uint32_t(instance.controller>=0))throw std::runtime_error("Mixed mesh provider kinds on one character node");point(existing.bounds,bounds);point(existing.bounds,bounds+3);continue;
   }
   CharacterMeshEntry entry{};std::copy(bounds,bounds+6,entry.bounds);entry.skinned=instance.controller>=0;
   for(unsigned k=0;k<3;++k){if(!std::isfinite(node.scale[k]))throw std::runtime_error("Invalid character node scale");entry.parent_scale[k]=node.scale[k];}
   nodes.push_back(instance.node_index);result.push_back(entry);
  }
  out=std::move(result);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
