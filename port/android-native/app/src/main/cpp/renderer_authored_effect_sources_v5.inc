struct AuthoredEffectDrawSourceV5 {
 std::shared_ptr<const std::vector<std::uint8_t>> resource_bytes;
 const dh2::resources::BresView* image{};const dh2::scene::Scene* scene{};
 std::uintptr_t fx_identity{},render_identity{};
 std::uint32_t node{},material{},source_part{},camera_offset_word{},rendering_layer{};
 bool source_node_render_fields_ready{},particle{};
 std::array<float,16> emitter_world{};
 dh2::skinning::VisualDrawPartV6 part;
 const dh2::math::Matrix4f* source_texture_matrix68{};
};
static AuthoredEffectDrawSourceV5 authored_effect_source_v5(dh2::fx::CharacterParticleDrawSourceV3 s){
 AuthoredEffectDrawSourceV5 out;out.resource_bytes=s.resource_bytes;out.image=s.image;out.scene=s.scene;
 out.fx_identity=s.fx_identity;out.render_identity=s.particle_identity;out.node=s.emitter_node;out.material=s.material;
 out.source_part=0;out.camera_offset_word=s.camera_offset_word;out.rendering_layer=s.rendering_layer;
 out.source_node_render_fields_ready=s.source_node_render_fields_ready;out.particle=true;
 out.emitter_world=s.emitter_world;out.part=std::move(s.part);out.source_texture_matrix68=s.source_texture_matrix68;return out;
}
static AuthoredEffectDrawSourceV5 authored_effect_source_v5(dh2::fx::CharacterFxMeshDrawSourceV4 s){
 // Source CMeshSceneNode receiver accepts exact unskinned IMesh prepare16.
 // This submission adapter currently supports ONE real buffer per node;
 // richer nodes retain explicit continuation instead of aliasing GPU owners.
 if(!s.part.geometry||s.part.geometry->primitives.size()!=1||s.primitive!=0||s.source_part!=1)
  throw std::runtime_error("Required multi-buffer authored mesh GPU receiver");
 AuthoredEffectDrawSourceV5 out;out.resource_bytes=s.resource_bytes;out.image=s.image;out.scene=s.scene;
 out.fx_identity=s.fx_identity;out.render_identity=s.node_identity;out.node=s.node;out.material=s.material;
 out.source_part=s.source_part;out.camera_offset_word=s.camera_offset_word;out.rendering_layer=s.rendering_layer;
 out.source_node_render_fields_ready=true;out.particle=false;out.emitter_world=s.part.world;
 out.part=std::move(s.part);out.source_texture_matrix68=s.source_texture_matrix68;return out;
}
