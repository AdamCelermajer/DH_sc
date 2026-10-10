// Shared by the live cleanup and its host regression. Ownership, rather than
// this frame's visible draw list, keeps an allocation live through loading.
template<class DrawOwner>
void retain_native_draw_resources_v125(const DrawOwner& draw,
 std::set<std::uint32_t>& textures,std::set<std::uint32_t>& buffers){
 for(auto name:{draw.diffuse,draw.alpha})if(name)textures.insert(name);
 for(auto name:{draw.vertices,draw.indices})if(name)buffers.insert(name);
 if(draw.native_shader_v113){
  for(auto name:draw.native_shader_v113->attributes)if(name)buffers.insert(name);
  for(auto name:draw.native_shader_v113->sampler_textures)if(name)textures.insert(name);
 }
}
template<class GeometryOwner>
void retain_native_geometry_resources_v125(const GeometryOwner& gpu,
 std::set<std::uint32_t>& textures,std::set<std::uint32_t>& buffers){
 for(auto name:gpu.textures)if(name)textures.insert(name);
 const auto draws=[&](const auto& owner){for(const auto& draw:owner.batches)retain_native_draw_resources_v125(draw,textures,buffers);};
 for(const auto& entry:gpu.meshes)draws(entry.second);
 for(const auto& entry:gpu.compiled)draws(entry.second);
 const auto actors=[&](const auto& owners){for(const auto& entry:owners){
  draws(entry.second);for(auto name:entry.second.private_images)if(name)textures.insert(name);
 }};
 actors(gpu.actors);actors(gpu.objects);
 for(const auto& draw:gpu.skybox_draws_v124)retain_native_draw_resources_v125(draw,textures,buffers);
}
