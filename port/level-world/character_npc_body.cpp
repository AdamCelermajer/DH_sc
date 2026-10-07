#include "character_npc_body.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace dh2::physical {
struct CharacterNpcBodyModel::Snapshot {
 std::vector<std::uint8_t> bytes;
 scene::Scene complete;
 DecorSceneMarker marker{};
 std::vector<CharacterMeshEntry> entries;
};
CharacterNpcBodyModel::CharacterNpcBodyModel()=default;
CharacterNpcBodyModel::~CharacterNpcBodyModel()=default;
bool CharacterNpcBodyModel::ready() const{return bool(snapshot_);}
const scene::Scene* CharacterNpcBodyModel::complete_scene() const{return snapshot_?&snapshot_->complete:nullptr;}
const DecorSceneMarker* CharacterNpcBodyModel::marker() const{return snapshot_?&snapshot_->marker:nullptr;}
const std::vector<CharacterMeshEntry>* CharacterNpcBodyModel::entries() const{return snapshot_?&snapshot_->entries:nullptr;}
bool CharacterNpcBodyModel::initialize(data::Bytes input,const scene::Scene* cached,std::string& error){
 error.clear();try {
  if(!input.data||!input.size||input.size>256*1024*1024||input.size>std::numeric_limits<std::uintptr_t>::max()-reinterpret_cast<std::uintptr_t>(input.data))throw std::runtime_error("NPC model byte span malformed");
  auto next=std::make_unique<Snapshot>();next->bytes.assign(input.data,input.data+input.size);
  resources::BresView view{};
  if(dh2_bres_open(&view,next->bytes.data(),next->bytes.size())!=resources::BresError::ok)throw std::runtime_error("NPC BRES rejected");
  if(cached)next->complete=*cached;
  else if(!scene::load(view,next->complete,error))return false;
  if(next->complete.graph.empty())throw std::runtime_error("NPC complete scene missing");
  if(!decor_scene_marker(view,next->complete,next->marker,error))return false;
  // CalcMeshBox marker-first: skin providers are not evaluated on that path.
  if(!next->marker.found){
   if(!character_scene_entries(view,next->complete,next->entries,error))return false;
   if(next->entries.empty())throw std::runtime_error("NPC mesh providers missing");
  }
  snapshot_=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool CharacterNpcBodyModel::project(const NpcBodyRequest& in,NpcBodyProjection& out,std::string& error) const{
 error.clear();try {
  if(!snapshot_||!in.properties||dh2_property_validate(in.properties)||!in.ai||in.ai->rows.size()<=8||in.ai->rows.size()>std::size_t(std::numeric_limits<std::int32_t>::max())||!in.owner||!in.new_physical||in.static_owner>255||in.previous_flat>255||in.collision_group_override>1||in.disable_physical>1||in.reserved)throw std::runtime_error("NPC body caller projection malformed");
  for(unsigned i=0;i<3;++i)if(!std::isfinite(in.position[i])||!std::isfinite(in.rotation_degrees[i]))throw std::runtime_error("NPC placement is not finite");
  NpcBodyProjection result{};const auto requested_ai=in.properties->resolved[1];result.collision_scale=in.properties->resolved[16];
  result.ai_id=requested_ai<0||std::size_t(requested_ai)>=in.ai->rows.size()?8:requested_ai;
  result.character_type=in.ai->rows[result.ai_id].type;
  if(result.character_type!=4)throw std::runtime_error("NPC projection requires source AI type4; other IsPlayer branches unavailable");
  std::copy(in.properties->base+12,in.properties->base+15,result.base_scale);
  DecorSceneInput placement{};std::copy(in.position,in.position+3,placement.position);std::copy(in.rotation_degrees,in.rotation_degrees+3,placement.rotation_degrees);
  if(dh2_character_visual_scale(placement.scale,result.base_scale))throw std::runtime_error("NPC source visual scale failed");
  result.marker=snapshot_->marker.found;
  if(result.marker){
   std::copy(snapshot_->marker.bounds,snapshot_->marker.bounds+6,placement.marker_bounds);std::copy(snapshot_->marker.parent_scale,snapshot_->marker.parent_scale+3,placement.marker_parent_scale);
   if(dh2_decor_scene(&result.visual,&placement))throw std::runtime_error("NPC marker mesh box failed");
  }else{
   CharacterMeshBoxInput mesh{snapshot_->entries.data(),static_cast<std::uint32_t>(snapshot_->entries.size()),0,placement};
   if(dh2_character_mesh_box(&result.visual,&mesh))throw std::runtime_error("NPC skin mesh box failed");
  }
  CharacterOwnerBoundsInput bounds{};std::copy(result.visual.mesh_box,result.visual.mesh_box+6,bounds.mesh_box);std::copy(in.position,in.position+3,bounds.position);bounds.collision_scale=result.collision_scale;bounds.previous_flat=in.previous_flat;bounds.already_scaled=result.marker;
  if(dh2_character_owner_bounds(&result.bounds,&bounds))throw std::runtime_error("NPC owner bounds failed");
  CharacterBodyInput body{};body.owner=in.owner;body.new_physical=in.new_physical;body.previous_physical=in.previous_physical;body.character_type=result.character_type;body.is_player=result.is_player;body.special_owner_byte=in.static_owner;body.collision_group_override=in.collision_group_override;body.disable_physical=in.disable_physical;
  const auto* b=result.bounds.absolute_box;body.absolute_bounds[0]=b[0];body.absolute_bounds[1]=b[1];body.absolute_bounds[2]=b[3];body.absolute_bounds[3]=b[4];std::copy(in.position,in.position+2,body.position);
  if(dh2_character_body_config(&result.body,&body))throw std::runtime_error("NPC body configuration failed");
  out=result;return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
