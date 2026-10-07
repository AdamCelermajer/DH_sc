// Include after renderer_npc_animation_v1.inc inside the private namespace.
// Requires character_world_npc_bounds_v1.hpp in the translation unit.
// Pure source CalcMeshBox/SetRelativeAABB projection; physical/PF publication
// remains the responsibility of the retained physical lifecycle composition.
void initialize_npc_owner_bounds_v1(ObjectActor& actor,AAssetManager* assets){
 auto& m=*actor.script;
 if(!m.animation||!m.bounds_root_ready)
  throw std::runtime_error("Required same NPC model-space pose and InitPost root matrix");
 auto raw=read(assets,actor.model,"actors");
 dh2::resources::BresView view{};
 if(dh2_bres_open(&view,raw.data(),raw.size())!=dh2::resources::BresError::ok)
  throw std::runtime_error("NPC owner bounds BRES rejected");
 dh2::physical::CharacterOwnerBounds bounds{};std::string error;
 if(!dh2::character::character_npc_visual_bounds_v1(view,m.animation->resources().factory(),
     m.bounds_root_matrix.data(),m.runtime.subobjects.position,
     actor.properties().resolved[16],m.source_bounds_flat,bounds,error))
  throw std::runtime_error("Source NPC owner bounds: "+error);
 std::copy_n(bounds.relative_box,6,m.runtime.subobjects.local_bounds);
 std::copy_n(bounds.absolute_box,6,m.runtime.subobjects.absolute_bounds);
 m.source_bounds_flat=std::uint8_t(bounds.flat);
 m.source_bounds_ready=true;
}
