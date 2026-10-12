#pragma once
#include "floors.hpp"
namespace dh2::world {
// Actual source floor record producer with UserProperties._ParseProperties
// before substring flags; extends the existing no-override floor domain.
// Appends into the caller's SAME floors::World, never a parallel PF graph.
bool module_floor_append_v2(const resources::BresView&,const scene::Scene&,
 const scene::Instance&,unsigned room,floors::World&,std::string&);
// Whole PFRoom continuation calls this AFTER publishing its constructor-owned
// record and delivering source Debug. Prefix writes remain on failure.
bool module_floor_load_record_v2(const resources::BresView&,const scene::Scene&,
 const scene::Instance&,unsigned room,floors::Record&,std::string&);
// Static OptimizeStatic modifies the real mesh child's local quaternion.
// The caller must borrow that producer; no identity fallback for this path.
bool module_floor_load_record_pose_v2(const resources::BresView&,const scene::Scene&,
 const scene::Instance&,unsigned room,const float* mesh_local_quaternion4,
 const float* mesh_local_scale3,floors::Record&,std::string&);
bool module_floor_append_pose_v2(const resources::BresView&,const scene::Scene&,
 const scene::Instance&,unsigned room,const float* mesh_local_quaternion4,
 const float* mesh_local_scale3,floors::World&,std::string&);
}
