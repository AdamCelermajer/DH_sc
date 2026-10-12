#pragma once
#include "retained_visual_child_v91.hpp"
#include "source_assertion_process_v76.hpp"
#include <zone_collision_runtime_v83.hpp>
namespace model_renderer {
template<class Self,class Zone>
bool initialize_colzone_box_v91(Self& self,Zone& zone,const std::array<float,6>& box,std::string& e){
 auto& base=zone.base();auto& dimensions=zone.source_dimensions374();
 // Original397594: all extent results, then Y/Z/X stores, then six copied
 // relative144 words, then Y/Z/X center expressions and SetPosition(true).
 const float y=box[4]-box[1],z=box[5]-box[2],x=box[3]-box[0];
 dimensions[1]=y;dimensions[2]=z;dimensions[0]=x;
 auto actual=base.relative_aabb144();if(!actual){e="Required SAME Zone relative144";return false;}
 for(unsigned i=0;i<6;++i)actual[i]=box[i];
 const float center_y=(box[1]+box[4])*0.5f,center_z=(box[2]+box[5])*0.5f,center_x=(box[0]+box[3])*0.5f;
 const float center[]{center_x,center_y,center_z};typename Self::Bound* bound{};
 if(!self.ensure(base,bound,e)||!dh2::world::game_object_set_position_v2(base,center,true,bound->position,e))return false;
 return !zone.physical()||self.create_zone_physical_v82(base,zone.trigger(),e);
}
template<class Self>
bool actual_colzone_box_v91(Self& self,dh2::world::CanonicalGameObjectBaseOwnerV1& base,const std::array<float,6>& box,std::string& e){
 for(const auto& weak:self.auxiliary_journal){auto r=weak.lock();if(!r||!r->class_name||!r->identity||r->identity()!=base.identity())continue;
#define ACTUAL_COLZONE_BOX(name,Record) if(!std::strcmp(r->class_name,name)){auto record=std::static_pointer_cast<dh2::loader::Record>(r->record);if(!record||!record->owner||&record->owner->base()!=&base){e="Changed SAME Zone bounding-box receiver";return false;}return initialize_colzone_box_v91(self,*record->owner,box,e);}
  ACTUAL_COLZONE_BOX("TriggerZone",CanonicalTriggerZoneRecordV22)
  ACTUAL_COLZONE_BOX("TriggerZoneExitLevel",CanonicalExitZoneRecordV29)
  ACTUAL_COLZONE_BOX("CheckpointZone",CanonicalCheckpointRecordV26)
  ACTUAL_COLZONE_BOX("QuestMoveInZone",CanonicalQuestMoveRecordV31)
  ACTUAL_COLZONE_BOX("Door",CanonicalDoorRecordV27)
  ACTUAL_COLZONE_BOX("TriggerObject",CanonicalTriggerObjectRecordV28)
#undef ACTUAL_COLZONE_BOX
 }
 e="Required SAME actual Zone dimensions/physical flags for InitWithBoundingBox";return false;
}
template<class Self>
bool bind_retained_colzone_v91(Self& self,dh2::world::CanonicalGameObjectBaseOwnerV1& base,const char* name,
 std::uintptr_t& node,std::shared_ptr<void>& lease,std::string& e){
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;const auto slot=base.pointer(0x2d8);
 if(!slot||!*slot||!self.actual_visual(*slot,visual,e))return false;std::uintptr_t found{};
 if(!visual->node_from_name(name,found,e))return false;
 node=found; // Source384 publication BEFORE intrusive grab/query.
 if(!found){lease.reset();e.clear();return true;}
 std::shared_ptr<dh2::world::RetainedVisualNodeV91> child;if(!visual->grab_node_v91(found,child,e))return false;
 lease=child; // actual node object/controlblock; never Visual/base/BRES alias
 std::vector<std::shared_ptr<dh2::world::RetainedMeshNodeV91>> meshes;child->collect_meshes(meshes);
 if(meshes.size()!=1){
  auto assertion=dh2::world::SourceAssertionProcessV76::borrow();const auto mode=*assertion->source_level();
  if(mode==2){e="Original Zone mesh-count assertion mode2 NULL write refused";return false;}
  if(mode==1&&!assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Zones\\Zone.cpp",0x60,"meshes.size() == 1",e))return false;
  e.clear();return true; // original leaves grabbed384, skips mesh/selector
 }
 auto mesh=meshes.front();if(!mesh||!mesh->mesh){e="Required actual unique child mesh resource";return false;}
 const auto position=base.vector3(0x160);if(!position){e="Required SAME Zone source position160";return false;}
 const std::array<float,3> captured{position[0],position[1],position[2]};
 auto box=mesh->mesh->bounds;
 for(unsigned i=0;i<3;++i){box[i]=captured[i]+box[i];box[i+3]=captured[i]+box[i+3];}
 if(!actual_colzone_box_v91(self,base,box,e))return false;
 mesh->source_hide(); // actual mesh node local120/flags, even if Visual retired
 // Actual CTriangleSelector(mesh,NULL,true): no scene-node transform owner,
 // selector holds SAME concrete mesh/vertex streams and attaches to384.
 auto selector=std::make_shared<dh2::world::RetainedTriangleSelectorV91>(mesh->mesh);
 child->selector=std::move(selector);e.clear();return true;
}
template<class Self>
bool retained_colzone_inside_v91(Self& self,dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::uintptr_t id,
 const dh2::world::ZonePeerBorrowV83& peer,bool& inside,std::string& e){
 std::shared_ptr<dh2::world::RetainedVisualNodeV91> child;
 for(const auto& weak:self.auxiliary_journal){auto r=weak.lock();if(!r||!r->identity||r->identity()!=base.identity())continue;
#define ACTUAL_COLZONE_LEASE(name,Record,expr) if(!std::strcmp(r->class_name,name)){auto record=std::static_pointer_cast<dh2::loader::Record>(r->record);if(!record||!record->owner){e="Released actual colzone receiver";return false;}auto lease=(expr);if(!lease||lease.get()!=reinterpret_cast<void*>(id)){e="Required SAME retained source384 child";return false;}child=std::static_pointer_cast<dh2::world::RetainedVisualNodeV91>(lease);break;}
  ACTUAL_COLZONE_LEASE("TriggerZone",CanonicalTriggerZoneRecordV22,record->owner->source_colzone_lease_v91())
  ACTUAL_COLZONE_LEASE("TriggerZoneExitLevel",CanonicalExitZoneRecordV29,record->owner->source_colzone_lease_v91())
  ACTUAL_COLZONE_LEASE("CheckpointZone",CanonicalCheckpointRecordV26,record->owner->source_colzone_lease())
  ACTUAL_COLZONE_LEASE("QuestMoveInZone",CanonicalQuestMoveRecordV31,record->owner->source_colzone_lease())
  ACTUAL_COLZONE_LEASE("Door",CanonicalDoorRecordV27,record->owner->source_colzone_lease())
  ACTUAL_COLZONE_LEASE("TriggerObject",CanonicalTriggerObjectRecordV28,record->owner->source_colzone_lease())
#undef ACTUAL_COLZONE_LEASE
 }
 if(!child||!peer.receiver||!peer.position160){e="Required actual colzone/peer position resources";return false;}
 const auto position=base.vector3(0x160);if(!position){e="Required actual Zone160";return false;}
 // Vec3f_K is (0,0,1), written by Point3D static init312e3c/54/78.
 // Source397110 creates peer-relative line +/- float bits42c80000 (100).
 const float axis[]{0.f,0.f,1.f};float start[3],end[3];
 for(unsigned i=0;i<3;++i){const float relative=peer.position160[i]-position[i];start[i]=relative+axis[i]*100.f;end[i]=relative+axis[i]*-100.f;}
 std::array<float,3> intersection{};inside=false;
 if(child->selector&&!child->selector->line(start,end,inside,intersection,e))return false;
 bool show{};auto world=std::static_pointer_cast<WorldScriptContext>(self.scope.actual_world.lock());std::uint32_t value{};
 if(!world||!world->debug||dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||
    dh2_character_debug_get(&value,world->debug.get(),"DisplayColZoneWhenColliding",&world->debug_files)!=1){e="Required actual colzone diagnostic Debug query";return false;}
 show=value!=0;if(show){
  std::vector<std::shared_ptr<dh2::world::RetainedMeshNodeV91>> meshes;child->collect_meshes(meshes);
  if(meshes.size()==1){const auto slot=base.pointer(0x2d8);std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
   if(!slot||!*slot||!self.actual_visual(*slot,visual,e))return false;
   if(!visual->source_set_visible_recur_v91(!inside,e))return false;
   meshes.front()->source_set_visible(inside);
  }
 }
 e.clear();return true;
}
}
